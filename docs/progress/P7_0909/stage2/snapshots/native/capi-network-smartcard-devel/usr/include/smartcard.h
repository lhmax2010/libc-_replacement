/*
 * Copyright (c) 2012, 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_CAPI_NETWORK_SMARTCARD_H__
#define __TIZEN_CAPI_NETWORK_SMARTCARD_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file smartcard.h
 * @brief This file contains the smartcard api
 * @since_tizen 2.3.1
 */
#ifndef SMARTCARD_API
#define SMARTCARD_API /**< Smartcard API */
#endif

#ifndef SMARTCARD_ERROR_CLASS
#define SMARTCARD_ERROR_CLASS TIZEN_ERROR_SMARTCARD /**< Smartcard error class */
#endif

/**
 * @deprecated Deprecated since 10.0.
 * @brief Error codes reported by the smartcard API.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_SE_SERVICE_MODULE
 */
typedef enum {
	SMARTCARD_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	SMARTCARD_ERROR_GENERAL = SMARTCARD_ERROR_CLASS | 0x01, /**< A general error occurred */
	SMARTCARD_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
	SMARTCARD_ERROR_NO_SUCH_ELEMENT = SMARTCARD_ERROR_CLASS | 0x02, /**< No such element error */
	SMARTCARD_ERROR_ILLEGAL_STATE = SMARTCARD_ERROR_CLASS | 0x03, /**< Illegal state of execution error */
	SMARTCARD_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid function parameter */
	SMARTCARD_ERROR_ILLEGAL_REFERENCE = SMARTCARD_ERROR_CLASS | 0x04, /**< Illegal reference */
	SMARTCARD_ERROR_OPERATION_NOT_SUPPORTED = SMARTCARD_ERROR_CLASS | 0x05, /**< Operation not supported from SE */
	SMARTCARD_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied  */
	SMARTCARD_ERROR_CHANNEL_NOT_AVAILABLE = SMARTCARD_ERROR_CLASS | 0x06, /**< No channel available */
	SMARTCARD_ERROR_NOT_INITIALIZED = SMARTCARD_ERROR_CLASS | 0x07, /**< Smartcard service not initialized */
	SMARTCARD_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	SMARTCARD_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
} smartcard_error_e;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Enumerations for Smartcard reader event type.
 * @since_tizen 3.0
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 */
typedef enum {
	SMARTCARD_READER_EVENT_TYPE_IO_ERROR = 0, /**< This event notifies the application that I/O error occurs on smartcard reader */
	SMARTCARD_READER_EVENT_TYPE_INSERTED = 1, /**< This event notifies the application that smartcard reader is inserted */
	SMARTCARD_READER_EVENT_TYPE_REMOVED = 2, /**< This event notifies the application that smartcard reader is removed */
	SMARTCARD_READER_EVENT_TYPE_CARD_INSERTED = 3, /**< This event notifies the application that smartcard reader is inserted (Since 6.5) */
	SMARTCARD_READER_EVENT_TYPE_CARD_REMOVED = 4, /**< This event notifies the application that smartcard reader is removed (Since 6.5) */
} smartcard_reader_event_type_e;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when a smartcard reader event occurs.
 * @since_tizen 3.0
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 *
 * @param[in] reader The reader handle
 * @param[in] event_type The reader event type
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see smartcard_reader_set_event_cb()
 * @see smartcard_reader_unset_event_cb()
 */
typedef void (*smartcard_reader_event_cb)(int reader, smartcard_reader_event_type_e event_type, void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Initializes smartcard service.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_SE_SERVICE_MODULE
 *
 * @remarks This function must be called before proceeding any other smartcard function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 *
 * @see smartcard_deinitialize()
 */
int smartcard_initialize(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Deinitializes smartcard service.
 * @details Releases all the resource of the smartcard service
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_SE_SERVICE_MODULE
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_initialize()
 */
int smartcard_deinitialize(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the version of SIMAlliance OpenMobileAPI specification.
 * @since_tizen 3.0
 * @ingroup CAPI_NETWORK_SMARTCARD_SE_SERVICE_MODULE
 *
 * @remarks The @a version must be released using free().
 *
 * @param[out] version The version of SIMAlliance OpenMobileAPI specification
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 * @retval #SMARTCARD_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int smartcard_get_version(char **version) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the list of available Secure Element readers.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_SE_SERVICE_MODULE
 *
 * @remarks readers must be released using free().
 *
 * @param[out] readers List of readers
 * @param[out] length The number of readers
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 */
int smartcard_get_readers(int **readers, int *length) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Returns the name of the given reader.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 *
 * @remarks reader_name must be released using free().
 * @remarks If the reader is a SIM reader, then its name must be "SIM[Slot]"
 * @remarks If the reader is a SD or micro SD reader, then its name must be "SD[slot]"
 * @remarks If the reader is a embedded SE reader, then its name must be "eSE[slot]"
 *
 * @param[in] reader Handle to the reader
 * @param[out] reader_name The name of reader
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_get_readers()
 */
int smartcard_reader_get_name(int reader, char **reader_name) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Checks if a Secure Element is present in the given reader.
 * @details This function checks if a Secure Element is present in the specified
 * reader. It returns true if a Secure Element is present, and false if it is
 * not. If an error occurs, the value of is_present is undefined.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 *
 * @param[in] reader Handle to the reader
 * @param[out] is_present True or false depending whether a secure element is present in the
reader - undefined when an error is returned
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_get_readers()
 */
int smartcard_reader_is_secure_element_present(int reader, bool *is_present) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Connects to a Secure Element in the given reader.
 * @details This function connects to a Secure Element in the specified reader
 * and creates a session handle for the given reader. The session handle can be
 * used to perform further operations on the Secure Element.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 *
 * @param[in] reader Handle to the reader
 * @param[out] session Handle to the session created for the given reader
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_get_readers()
 */
int smartcard_reader_open_session(int reader, int *session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Closes all the sessions opened on the given reader.
 * @details All the channels opened by all these sessions will be closed.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 *
 * @param[in] reader Handle to the reader
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_get_readers()
 */
int smartcard_reader_close_sessions(int reader) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Sets a callback function for receiving reader event.
 * @details Registers a callback function to receive notifications about reader
 * events. When a reader event occurs, the provided callback function will be
 * called with the new state and the user data passed during registration.
 * @since_tizen 3.0
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 *
 * @param[in] cb The reader event callback
 * @param[in] user_data The user data
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_unset_event_cb()
 */
int smartcard_reader_set_event_cb(smartcard_reader_event_cb cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Unsets the reader event callback function.
 * @details Unregisters the previously registered callback function for
 * receiving reader event notifications. After calling this function, the
 * callback function will no longer be called when a reader event occurs.
 * @since_tizen 3.0
 * @ingroup CAPI_NETWORK_SMARTCARD_READER_MODULE
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_set_event_cb()
 */
int smartcard_reader_unset_event_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the reader that provides the given session.
 * @details Retrieves the reader handle that provides the given session. The
 * reader handle is stored in the location pointed to by the reader parameter.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_SESSION_MODULE
 *
 * @param[in] session Handle to the session
 * @param[out] reader Reader handle that provides the given session
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_open_session()
 */
int smartcard_session_get_reader(int session, int *reader) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the Answer to Reset(ATR) of this Secure Element.
 * @details Retrieves the Answer to Reset (ATR) of the Secure Element associated
 * with the given session. The ATR is stored in the byte array pointed to by the
 * atr parameter, and its length is stored in the location pointed to by the
 * length parameter. Note that the atr buffer must be released using free()
 * after use.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_SESSION_MODULE
 *
 * @remarks atr must be released using free().
 * @remarks If the Answer to Reset(ATR) for this Secure Element is not available the returned length is set to zero and return value is Success.
 *
 * @param[in] session Handle to the session
 * @param[out] atr Byte array to retrieve the Answer to Reset(ATR)
 * @param[out] length The length of atr
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_open_session()
 */
int smartcard_session_get_atr(int session, unsigned char **atr, int *length) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Closes the connection with the Secure Element.
 * @details This will close any channels opened by this application with this Secure Element.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_SESSION_MODULE
 *
 * @param[in] session handle to the session
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_open_session()
 */
int smartcard_session_close(int session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Checks if the given session is closed.
 * @details Checks whether the given session is closed. The result is stored in
 * the location pointed to by the is_closed parameter.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_SESSION_MODULE
 *
 * @param[in] session Handle to the session
 * @param[out] is_closed True or false depending on the session state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_open_session()
 */
int smartcard_session_is_closed(int session, bool *is_closed) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Closes any channel opened on the given session.
 * @details Closes all channels that have been opened on the given session. This
 * function can be used to ensure that all resources associated with the session
 * are properly released.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_SESSION_MODULE
 *
 * @param[in] session Handle to the session
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_open_session()
 */
int smartcard_session_close_channels(int session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets an access to the basic channel, as defined in the ISO/IEC 7816-4 specification (the one that has number 0).
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_SESSION_MODULE
 *
 * @remarks ex) unsigned char aid[] = {0xA0, 0x00, 0x00, 0x00, 0x63, 0x50, 0x4B, 0x43, 0x53, 0x2D, 0x31, 0x35};
 * @remarks If the Application ID (AID) is null, it means no Applet is to be selected on this channel and the default Applet is used.
 * @remarks Opening of the basic channel to a specific applet is NOT allowed for UICC.
 *
 * @param[in] session Handle to the session
 * @param[in] aid Byte array containing the Application ID (AID) to be selected on the given channel
 * @param[in] aid_len Size of byte array or 0 when no SELECT should be executed
 * @param[in] P2 P2 byte of the SELECT command if executed
 * @param[out] channel Handle of the basic channel
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_CHANNEL_NOT_AVAILABLE No channel available
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NO_SUCH_ELEMENT No such element error
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_open_session()
 */
int smartcard_session_open_basic_channel(int session, unsigned char *aid, int aid_len, unsigned char P2, int *channel) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Opens a logical channel with the Secure Element, selecting the Applet represented by the given Application ID (AID).
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_SESSION_MODULE
 *
 * @remarks ex) unsigned char aid[] = {0xA0, 0x00, 0x00, 0x00, 0x63, 0x50, 0x4B, 0x43, 0x53, 0x2D, 0x31, 0x35};
 * @remarks If the Application ID (AID) is null, it means no Applet is to be selected on this channel and the default Applet is used.
 *
 * @param[in] session Handle to the session
 * @param[in] aid Byte array containing the Application ID (AID) to be selected on the given channel
 * @param[in] aid_len Size of byte array or 0 when no SELECT should be executed
 * @param[in] P2 P2 byte of the SELECT command if executed
 * @param[out] channel Handle of the new logical channel
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_CHANNEL_NOT_AVAILABLE No channel available
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NO_SUCH_ELEMENT No such element error
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_reader_open_session()
 */
int smartcard_session_open_logical_channel(int session, unsigned char *aid, int aid_len, unsigned char P2, int *channel) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Closes the given channel to the Secure Element.
 * @details Closes the given channel to the Secure Element. This function
 * releases any resources associated with the channel.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @param[in] channel Handle to the channel
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_close(int channel) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Checks if the given channel is the basic channel.
 * @details Determines whether the given channel is a basic channel. The result
 * is stored in the location pointed to by the is_basic_channel parameter.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @param[in] channel Handle to the channel
 * @param[out] is_basic_channel True or false depending on the channel type
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_is_basic_channel(int channel, bool *is_basic_channel) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Checks if the given channel is closed.
 * @details Checks whether the given channel is closed. The result is stored in
 * the location pointed to by the is_closed parameter.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @param[in] channel Handle to the channel
 * @param[out] is_closed True or false depending on the channel state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_is_closed(int channel, bool *is_closed) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the response to the select command.
 * @details Retrieves the response to the SELECT command for the given channel.
 * The response is stored in the byte array pointed to by the select_resp
 * parameter, and its length is stored in the location pointed to by the length
 * parameter. Note that the select_resp buffer must be released using free()
 * after use.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @remarks select_resp must be released using free().
 *
 * @param[in] channel Handle to the channel
 * @param[out] select_resp Byte array to retrieve the SELECT response
 * @param[out] length The length of select_resp
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_get_select_response(int channel, unsigned char **select_resp, int *length) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the session that has opened the given channel.
 * @details Retrieves the session handle that has opened the given channel. The
 * session handle is stored in the location pointed to by the session parameter.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @param[in] channel Handle to the channel
 * @param[out] session Session handle of the given channel
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_get_session(int channel, int *session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Transmits an APDU command (as per ISO/IEC 7816-4) to the Secure Element.
 * @details Transmits an APDU command to the Secure Element associated with the
 given channel. The response to the command is stored in the byte array pointed
 to by the resp parameter, and its length is stored in the location pointed to
 by the length parameter. Note that the resp buffer must be released using
 free() after use.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @remarks resp must be released using free().
 * @remarks MANAGE_CHANNEL commands are not allowed.
 * @remarks SELECT by DF Name (P1=04) are not allowed.
 * @remarks bytes with channel numbers are de-masked.

 * @param[in] channel Handle to the channel
 * @param[in] cmd Command APDU to be send to the secure element
 * @param[in] cmd_len Size of command APDU
 * @param[out] resp Byte array for the response APDU plus status words
 * @param[out] length The length of resp
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_ILLEGAL_STATE Illegal state of execution error
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_transmit(int channel, unsigned char *cmd, int cmd_len, unsigned char **resp, int *length) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Retrieves the response APDU of the previous transmit() call (helper function).
 * @details Retrieves the response APDU from the previous transmit() call for
 * the given channel. The response is stored in the byte array pointed to by the
 * resp parameter, and its length is stored in the location pointed to by the
 * length parameter. Note that the resp buffer must be released using free()
 * after use.
 * @since_tizen 2.3.1
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @remarks resp must be released using free().
 *
 * @param[in] channel Handle to the channel
 * @param[out] resp Byte array for the response APDU plus status words
 * @param[out] length The length of resp
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_ILLEGAL_STATE Illegal state of execution error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_transmit_retrieve_response(int channel, unsigned char **resp, int *length) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Performs a selection of the next Applet on the given channel that matches to the partial Application ID (AID).
 * @details Selects the next Applet on the given channel that matches the
partial Application ID (AID). The result is stored in the location pointed to by
the is_success parameter.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/secureelement
 * @ingroup CAPI_NETWORK_SMARTCARD_CHANNEL_MODULE
 *
 * @param[in] channel Handle to the channel
 * @param[out] is_success True or false depending whether another applet with the partial Application ID (AID)
could be selected on the given channel

 * @return 0 on success, otherwise a negative error value.
 * @retval #SMARTCARD_ERROR_NONE Successful
 * @retval #SMARTCARD_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTCARD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTCARD_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SMARTCARD_ERROR_GENERAL A general error occurred
 * @retval #SMARTCARD_ERROR_ILLEGAL_STATE Illegal state of execution error
 * @retval #SMARTCARD_ERROR_OPERATION_NOT_SUPPORTED Operation not supported from SE
 * @retval #SMARTCARD_ERROR_IO_ERROR I/O error
 * @retval #SMARTCARD_ERROR_NOT_INITIALIZED Smartcard service not initialized
 *
 * @see smartcard_session_open_basic_channel()
 * @see smartcard_session_open_logical_channel()
 */
int smartcard_channel_select_next(int channel, bool *is_success) TIZEN_DEPRECATED_API;

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_CAPI_NETWORK_SMARTCARD_H__ */
