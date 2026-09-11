/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
*/
#ifndef __TIZEN_NET_ASP_H__
#define __TIZEN_NET_ASP_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_NETWORK_ASP_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for Application Service Platform (ASP) error code.
 * @since_tizen 4.0
 */
typedef enum {
	ASP_ERROR_NONE                   = TIZEN_ERROR_NONE,                 /**< Successful */
	ASP_ERROR_NOT_PERMITTED          = TIZEN_ERROR_NOT_PERMITTED,        /**< Operation not permitted(1) */
	ASP_ERROR_OUT_OF_MEMORY          = TIZEN_ERROR_OUT_OF_MEMORY,        /**< Out of memory(12) */
	ASP_ERROR_PERMISSION_DENIED      = TIZEN_ERROR_PERMISSION_DENIED,    /**< Permission denied(13) */
	ASP_ERROR_RESOURCE_BUSY          = TIZEN_ERROR_RESOURCE_BUSY,        /**< Device or resource busy(16) */
	ASP_ERROR_INVALID_PARAMETER      = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid function parameter(22) */
	ASP_ERROR_CONNECTION_TIME_OUT    = TIZEN_ERROR_CONNECTION_TIME_OUT,  /**< Connection timed out(110) */
	ASP_ERROR_NOT_SUPPORTED          = TIZEN_ERROR_NOT_SUPPORTED,        /**< Not supported */
	ASP_ERROR_NOT_INITIALIZED        = TIZEN_ERROR_ASP|0x01,             /**< Not initialized */
	ASP_ERROR_ALREADY_INITIALIZED    = TIZEN_ERROR_ASP|0x02,             /**< Already initialized */
	ASP_ERROR_COMMUNICATION_FAILED   = TIZEN_ERROR_ASP|0x03,             /**< I/O error */
	ASP_ERROR_OPERATION_FAILED       = TIZEN_ERROR_ASP|0x04,             /**< Operation failed */
	ASP_ERROR_IN_PROGRESS            = TIZEN_ERROR_ASP|0x05,             /**< In progress */
	ASP_ERROR_SERVICE_NOT_FOUND      = TIZEN_ERROR_ASP|0x06,             /**< Service not found */
	ASP_ERROR_SESSION_NOT_FOUND      = TIZEN_ERROR_ASP|0x07,             /**< Session not found */
	ASP_ERROR_NETWORK_ROLE_REJECTED  = TIZEN_ERROR_ASP|0x08              /**< Requested network role rejected */
} asp_error_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for Advertise Status event reason.
 * @since_tizen 4.0
 */
typedef enum {
    ASP_ADVERT_STATUS_REASON_SUCCESS,             /**< Status reason: Success */
    ASP_ADVERT_STATUS_REASON_SERVICE_DUPLICATED,  /**< Status reason: Service duplicated */
    ASP_ADVERT_STATUS_REASON_OTHER_FAILURE,       /**< Status reason: Other failure */
} asp_advert_status_reason_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for the Wi-Fi P2P role assignment scheme.
 * @since_tizen 4.0
 * @see asp_advert_set_p2p_role_scheme()
 * @see asp_advert_get_p2p_role_scheme()
 * @see asp_session_set_p2p_role_scheme()
 * @see asp_session_get_p2p_role_scheme()
 */
typedef enum {
    ASP_ADVERT_P2P_ROLE_SCHEME_ANY,  /**< All roles are acceptable */
    ASP_ADVERT_P2P_ROLE_SCHEME_GO,   /**< The interface should assume the GO role */
    ASP_ADVERT_P2P_ROLE_SCHEME_GC,   /**< The interface should assume the GC role */
} asp_advert_p2p_role_scheme_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for service discovery mechanism.
 * @since_tizen 4.0
 * @see asp_advert_set_discovery_tech()
 * @see asp_seek_set_discovery_tech()
 */
typedef enum {
	ASP_DISCOVERY_TECH_P2P    = 1 << 0,  /**< Use Wi-Fi P2P for discovery mechanism */
	ASP_DISCOVERY_TECH_BLE    = 1 << 1,  /**< Use BLE for discovery mechanism */
	ASP_DISCOVERY_TECH_NFC    = 1 << 2,  /**< Use NFC for discovery mechanism */
	ASP_DISCOVERY_TECH_INFRA  = 1 << 3,  /**< Use Wi-Fi Infrastructured for discovery mechanism */
	ASP_DISCOVERY_TECH_NAN    = 1 << 4,  /**< Use Wi-Fi NAN for discovery mechanism */
} asp_discovery_tech_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for Wi-Fi WPS type.
 * @see asp_advert_set_p2p_config_method()
 * @see asp_advert_get_p2p_config_method()
 * @see asp_session_set_p2p_config_method()
 * @see asp_session_get_p2p_config_method()
 * @since_tizen 4.0
 */
typedef enum {
	ASP_WPS_TYPE_NONE,         /**< No WPS type */
	ASP_WPS_TYPE_DEFAULT,      /**< Default WPS type both P2PS and PIN */
	ASP_WPS_TYPE_PIN_BOTH,     /**< WPS type PIN code both display and keypad*/
	ASP_WPS_TYPE_PIN_DISPLAY,  /**< WPS type display PIN code only*/
	ASP_WPS_TYPE_PIN_KEYPAD,   /**< WPS type keypad to input the PIN only*/
} asp_wps_type_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for Application Service Platform (ASP) Session connect status.
 * @since_tizen 4.0
*/
typedef enum {
	ASP_CONNECT_STATUS_NETWORK_ROLE_REJECTED,      /**< Network role rejected */
	ASP_CONNECT_STATUS_CONNECTION_LIMIT_REACHED,   /**< The device's connection limit has been reached */
	ASP_CONNECT_STATUS_REQUEST_SENT,               /**< Session request sent */
	ASP_CONNECT_STATUS_REQUEST_RECEIVED,           /**< Session request received */
	ASP_CONNECT_STATUS_REQUEST_DEFERRED,           /**< Session request deferred */
	ASP_CONNECT_STATUS_REQUEST_ACCEPTED,           /**< Session request accepted */
	ASP_CONNECT_STATUS_REQUEST_FAILED,             /**< Session request failed */
	ASP_CONNECT_STATUS_GROUP_FORMATION_STARTED,    /**< Group formation started */
	ASP_CONNECT_STATUS_GROUP_FORMATION_COMPLETED,  /**< Group formation completed */
	ASP_CONNECT_STATUS_GROUP_FORMATION_FAILED,     /**< Group formation failed*/
} asp_connect_status_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for Application Service Platform (ASP) general session state.
 * @since_tizen 4.0
*/
typedef enum {
	ASP_SESSION_STATE_ERROR,      /**< Session state error*/
	ASP_SESSION_STATE_CLOSED,     /**< Session state closed*/
	ASP_SESSION_STATE_INITIATED,  /**< Session state initiated*/
	ASP_SESSION_STATE_REQUESTED,  /**< Session state requested*/
	ASP_SESSION_STATE_OPEN,       /**< Session state open*/
} asp_session_state_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for Application Service Platform (ASP) session close status.
 * @since_tizen 4.0
*/
typedef enum {
	ASP_SESSION_CLOSE_OK,              /**< Session closed without error */
	ASP_SESSION_CLOSE_DISASSOCIATED,   /**< Session closed by disassociation (Wi-Fi Direct connection was broken) */
	ASP_SESSION_CLOSE_LOCAL_CLOSE,     /**< Session closed with local close */
	ASP_SESSION_CLOSE_REMOTE_CLOSE,    /**< Session closed with remote close */
	ASP_SESSION_CLOSE_SYSTEM_FAILURE,  /**< Session closed with system failure */
	ASP_SESSION_CLOSE_NO_RESPONSE,     /**< Session closed with no response from remote */
} asp_session_close_status_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for Application Service Platform (ASP) port status.
 * @since_tizen 4.0
*/
typedef enum {
	ASP_PORT_STATUS_LOCAL_PORT_ALLOWED,   /**< Incoming connections are allowed on this local port for this ASP session */
	ASP_PORT_STATUS_LOCAL_PORT_BLOCKED,   /**< Incoming connections are no longer allowed in the ASP session for this local port */
	ASP_PORT_STATUS_FAILURE,              /**< The ASP was unable to set up this local port */
	ASP_PORT_STATUS_REMOTE_PORT_ALLOWED,  /**< The remote service has allowed access to the given remote port for the given protocol */
} asp_port_status_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for service status.
 * @since_tizen 4.0
*/
typedef enum {
	ASP_SERVICE_STATUS_NOT_ADVERTISED,  /**< Service status not advertised */
	ASP_SERVICE_STATUS_ADVERTISED,       /**< Service status advertised */
} asp_service_status_e;

/**
 * @deprecated Deprecated since 9.0
 * @brief The advertised service description.
 * @since_tizen 4.0
 */
typedef void *asp_advert_service_h;

/**
 * @deprecated Deprecated since 9.0
 * @brief The searching service description.
 * @since_tizen 4.0
 */
typedef void *asp_seek_service_h;

/**
 * @deprecated Deprecated since 9.0
 * @brief The Application Service Platform (ASP) session description.
 * @since_tizen 4.0
 */
typedef void *asp_session_h;

/**
* @deprecated Deprecated since 9.0
* @brief Called when the Application Service Platform (ASP) Service is found.
* @since_tizen 4.0
* @remarks @a service_mac, @a instance_name and @a service_info are valid only in the callback.
* To use outside the callback, make a copy.
* @param[in] error_code     The error code. \n
*                           #ASP_ERROR_NONE              Successful\n
*                           #ASP_ERROR_OUT_OF_MEMORY     Out of memory\n
*                           #ASP_ERROR_OPERATION_FAILED  Operation failed\n
* @param[in] seek_service   The seek service which found the current result;
*                           the handle to the same object for which the search was started
* @param[in] service_mac    The P2P device address of the device which provides the service;
*                           NULL if not available
* @param[in] adv_id         The service advertisement ID defined by remote P2P device
* @param[in] config_method  The preferred Wi-Fi Simple Config (WSC) configuration method
* @param[in] instance_name  The advertised service name defined by the remote P2P device;
*                           NULL if not available
* @param[in] service_info   The advertised service information defined by the remote P2P device;
*                           NULL if not available
* @param[in] info_size      The advertised service information payload size;
*                           if the information is not available, this is set to -1
* @param[in] status         The status of the service
* @param[in] user_data      The user data passed from the callback registration function
* @pre The callback must be registered using asp_seek_set_search_result_cb().\n
* asp_seek_start() must be called to invoke this callback.
* @see asp_seek_set_search_result_cb()
* @see asp_seek_unset_search_result_cb()
* @see asp_seek_start()
*/
typedef void (*asp_seek_search_result_cb) (int error_code,
                                           asp_seek_service_h seek_service,
                                           const char *service_mac,
                                           unsigned int adv_id,
                                           asp_wps_type_e config_method,
                                           const char *instance_name,
                                           const char *service_info,
                                           int info_size,
                                           unsigned char status,
                                           void *user_data);

/**
* @deprecated Deprecated since 9.0
* @brief Called when the status of an advertisement to a service is changed.
* @since_tizen 4.0
* @param[in] adv_service  The service whose status has changed;
*                         the handle to the same object for which the callback was set
* @param[in] status       The status of the service
* @param[in] reason       The reason of the state change
* @param[in] user_data    The user data passed from the callback registration function
* @pre The callback must be registered using asp_advert_set_status_cb().\n
* asp_advert_start_advertising() and asp_change_service_state() invoke this callback.
* @see asp_advert_set_status_changed_cb()
* @see asp_advert_unset_status_changed_cb()
* @see asp_advert_start_advertising()
*/
typedef void (*asp_advert_status_changed_cb) (asp_advert_service_h adv_service,
                                              asp_service_status_e status,
                                              asp_advert_status_reason_e reason,
                                              void *user_data);

/**
* @deprecated Deprecated since 9.0
* @brief Called when a remote device is attempting to initiate an ASP session.
* @since_tizen 4.0
* @remarks @a network_config_pin, @a device_name and @a info are valid only in the callback.
* To use outside the callback, make a copy.
* @param[in] error_code              The error code.\n
*                                    #ASP_ERROR_NONE              Successful\n
*                                    #ASP_ERROR_OUT_OF_MEMORY     Out of memory\n
*                                    #ASP_ERROR_OPERATION_FAILED  Operation failed\n
* @param[in] adv_service             Service descriptor handle provided by the asp_advert_create()
*                                    it is the "original" object not created specifically for the callback
* @param[in] session                 The session for which the request is made;
*                                    the handle to the same object for which the callback was set
* @param[in] device_name             Device name of the remote peer
* @param[in] info                    The service-specific data payload (up to 144 bytes);
*                                    NULL if not available
* @param[in] info_size               The service-specific data payload size (up to 144)
* @param[in] get_network_config_pin  (@c true = requires WSC PIN to be entered by
*                                    the asp_session_confirm(),
*                                    @c false = does not require PIN)
* @param[in] network_config_pin      The WSC PIN value to be displayed; NULL if not available
* @param[in] user_data               The user data passed from the callback registration function
* @pre The callback must be registered using asp_session_set_request_cb().
* @see asp_session_set_request_cb()
*/
typedef void (*asp_session_request_cb) (int error_code,
                                        asp_advert_service_h adv_service,
                                        asp_session_h session,
                                        const char *device_name,
                                        const char *info,
                                        int info_size,
                                        bool get_network_config_pin,
                                        const char *network_config_pin,
                                        void *user_data);

/**
* @deprecated Deprecated since 9.0
* @brief Called when the Application Service Platform (ASP) has to provide
* a PIN value or collect a PIN value.
* @since_tizen 4.0
* @remarks @a config_pin is valid only in the callback.
* To use outside the callback, make a copy.
* @param[in] error_code  The error code.\n
*                        #ASP_ERROR_NONE              Successful\n
*                        #ASP_ERROR_OUT_OF_MEMORY     Out of memory\n
*                        #ASP_ERROR_OPERATION_FAILED  Operation failed\n
* @param[in] session     The session for which the request is made;
*                        the handle to the same object for which the callback was set
* @param[in] get_pin     (@c true = instruct the user to enter the PIN,
*                        @c false = display the PIN provided in the @a config_pin parameter)
* @param[in] config_pin  The WSC PIN value used for setting up; ignored if @a get_pin is @c true
* @param[in] user_data   The user data passed from the callback registration function
* @pre The callback must be registered using asp_session_set_config_request_cb().\n
* asp_session_connect() must be called to invoke this callback.
* @see asp_session_set_request_cb()
* @see asp_session_connect()
*/
typedef void (*asp_session_config_request_cb) (int error_code,
                                               asp_session_h session,
                                               bool get_pin,
                                               const char *config_pin,
                                               void *user_data);

/**
* @deprecated Deprecated since 9.0
* @brief Called when the Application Service Platform (ASP) reports progress on group formation.
* @since_tizen 4.0
* @remarks @a deferred_resp is valid only in the callback.
* To use outside the callback, make a copy.
* @param[in] error_code     The error code.\n
*                           #ASP_ERROR_NONE              Successful\n
*                           #ASP_ERROR_OUT_OF_MEMORY     Out of memory\n
*                           #ASP_ERROR_OPERATION_FAILED  Operation failed\n
* @param[in] session        The session for which progress is reported;
*                           the handle to the same object for which the callback was set
* @param[in] status         The connection progress status
* @param[in] deferred_resp  The service-specific data payload up to 144 bytes
* @param[in] resp_size      The service-specific data payload size(up to 144)
* @param[in] user_data      The user data passed from the callback registration function
* @pre The callback must be registered using asp_session_set_connect_status_cb()
* @see asp_session_set_connect_status_cb()
*/
typedef void (*asp_session_connect_status_cb) (int error_code,
                                               asp_session_h session,
                                               asp_connect_status_e status,
                                               const char *deferred_resp,
                                               int resp_size,
                                               void *user_data);

/**
* @deprecated Deprecated since 9.0
* @brief Called when the Application Service Platform (ASP) reports the state and status of an ASP session.
* @since_tizen 4.0
* @remarks @a additional_info is valid only in the callback.
* To use outside the callback, make a copy.
* @param[in] error_code       The error code.\n
*                             #ASP_ERROR_NONE              Successful\n
*                             #ASP_ERROR_OUT_OF_MEMORY     Out of memory\n
*                             #ASP_ERROR_OPERATION_FAILED  Operation failed\n
* @param[in] session          The session for which status is reported;
*                             the handle to the same object for which the callback was set
* @param[in] state            The session state
* @param[in] additional_info  Additional information related to the SessionStatus event
* @param[in] user_data        The user data passed from the callback registration function
* @pre The callback must be registered using asp_session_set_status_cb()
* @see asp_session_set_status_cb()
*/
typedef void (*asp_session_status_cb) (int error_code,
                                       asp_session_h session,
                                       asp_session_state_e state,
                                       const char *additional_info,
                                       void *user_data);

/**
* @deprecated Deprecated since 9.0
* @brief Called when the Application Service Platform (ASP) reports the status of the network port on the local and remote end of the ASP session.
* @since_tizen 4.0
* @remarks @a ip_address is valid only in the callback.
* To use outside the callback, make a copy.
* @param[in] error_code   The error code.\n
*                         #ASP_ERROR_NONE              Successful\n
*                         #ASP_ERROR_OUT_OF_MEMORY     Out of memory\n
*                         #ASP_ERROR_OPERATION_FAILED  Operation failed\n
* @param[in] session      Application Service Platform session descriptor handle
* @param[in] ip_address   The IP address for the port, local or remote
* @param[in] port         The port number
* @param[in] proto        The IANA protocol number
* @param[in] status       The port status
* @param[in] user_data    The user data passed from the callback registration function
* @pre The callback must be registered using asp_session_set_port_status_cb()
* @see asp_session_set_port_status_cb()
*/
typedef void (*asp_session_port_status_cb) (int error_code,
                                            asp_session_h session,
                                            const char *ip_address,
                                            int port,
                                            int proto,
                                            asp_port_status_e status,
                                            void *user_data);

/**
 * @deprecated Deprecated since 9.0
 * @brief Initializes Application Service Platform (ASP).
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value.
 * @retval #ASP_ERROR_NONE                  Successful
 * @retval #ASP_ERROR_RESOURCE_BUSY         Device or resource busy
 * @retval #ASP_ERROR_NOT_SUPPORTED         Not supported
 * @retval #ASP_ERROR_COMMUNICATION_FAILED  Communication failed
 * @retval #ASP_ERROR_OPERATION_FAILED      Operation failed
 * @see asp_deinitialize()
 */
int asp_initialize(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Deinitializes Application Service Platform (ASP).
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @return 0 on success, otherwise a negative error value.
 * @retval #ASP_ERROR_NONE                  Successful
 * @retval #ASP_ERROR_NOT_PERMITTED         Operation not permitted
 * @retval #ASP_ERROR_NOT_SUPPORTED         Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #ASP_ERROR_COMMUNICATION_FAILED  Communication failed
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 */
int asp_deinitialize(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Registers the callback function that will be invoked when a service found.
 * @since_tizen 4.0
 * @param[in] cb         The callback function to invoke
 * @param[in] user_data  The user data passed from the callback registration function
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @post asp_search_result_cb will be called under certain conditions,
 * after calling asp_seek_start()
 * @see asp_initialize()
 * @see asp_seek_unset_search_result_cb()
 * @see asp_seek_start()
 */
int asp_seek_set_search_result_cb(asp_seek_search_result_cb cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unregisters the callback function that will invoked when a service found.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE             Successful
 * @retval #ASP_ERROR_NOT_SUPPORTED    Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED  Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_seek_set_search_result_cb()
 */
int asp_seek_unset_search_result_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Registers the callback function that will invoked when the status of an advertisement to a service is changed.
 * @since_tizen 4.0
 * @param[in] cb         The callback function to invoke
 * @param[in] user_data  The user data passed from the callback registration function
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @post asp_seek_search_result_cb() will be called under certain conditions,
 * after calling asp_seek_start()
 * @see asp_initialize()
 * @see asp_advert_unset_status_changed_cb()
 * @see asp_advert_start_advertising()
 */
int asp_advert_set_status_changed_cb(asp_advert_status_changed_cb cb,
	                                   void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unregisters the callback function that will invoked when the status of an advertisement to a service is changed.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE             Successful
 * @retval #ASP_ERROR_NOT_SUPPORTED    Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED  Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_advert_set_status_changed_cb()
 */
int asp_advert_unset_status_changed_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Registers the callback called when ASP session connection is requested.
 * @since_tizen 4.0
 * @param[in] cb         The callback function to invoke
 * @param[in] user_data  The user data passed from the callback registration function
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_unset_request_cb()
 */
int asp_session_set_request_cb(asp_session_request_cb cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unregisters the callback called ASP when session connection is requested.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE             Successful
 * @retval #ASP_ERROR_NOT_SUPPORTED    Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED  Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_set_request_cb()
 */
int asp_session_unset_request_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Registers the callback called when the session configuration is requested.
 * @since_tizen 4.0
 * @param[in] cb         The callback function to invoke
 * @param[in] user_data  The user data passed from the callback registration function
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_unset_config_request_cb()
 * @see asp_session_config_request_cb()
 */
int asp_session_set_config_request_cb(asp_session_config_request_cb cb,
                                      void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unregisters the callback called when the session configuration is requested.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE             Successful
 * @retval #ASP_ERROR_NOT_SUPPORTED    Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED  Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_config_request_cb()
 */
int asp_session_unset_config_request_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Registers the callback called when the state of the session connection is changed.
 * @since_tizen 4.0
 * @param[in] cb         The callback function to invoke
 * @param[in] user_data  The user data passed from the callback registration function
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_unset_connect_status_cb()
 * @see asp_session_connect_status_cb()
 */
int asp_session_set_connect_status_cb(asp_session_connect_status_cb cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unregisters the callback called when the state of the session connection is changed.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE             Successful
 * @retval #ASP_ERROR_NOT_SUPPORTED    Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED  Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_set_connect_status_cb()
 */
int asp_session_unset_connect_status_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Registers the callback called when the state of the session is changed.
 * @since_tizen 4.0
 * @param[in] cb         The callback function to invoke
 * @param[in] user_data  The user data passed from the callback registration function
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_unset_status_cb()
 * @see asp_session_status_cb()
 */
int asp_session_set_status_cb(asp_session_status_cb cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unregisters the callback called when the state of the session is changed.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE             Successful
 * @retval #ASP_ERROR_NOT_SUPPORTED    Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED  Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_set_status_cb()
 */
int asp_session_unset_status_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Registers the callback called when the state of the port is changed.
 * @since_tizen 4.0
 * @param[in] cb         The callback function to invoke
 * @param[in] user_data  The user data passed from the callback registration function
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_unset_status_cb()
 */
int asp_session_set_port_status_cb(asp_session_port_status_cb cb, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unregisters the callback called when the state of the port is changed.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE             Successful
 * @retval #ASP_ERROR_NOT_SUPPORTED    Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED  Not initialized
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_initialize()
 * @see asp_session_set_port_status_cb()
 */
int asp_session_unset_port_status_cb(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Creates the description of a service to be advertised.
 * @since_tizen 4.0
 * @remarks The @a adv_service should be released using asp_advert_destroy().
 * @param[in] instance_name  Service instance name of a service type to be advertised.
 *                           The length of this parameter be 63 byte or less and
 *                           encoding shall be single-byte or multi-byte UTF-8 characters.
 *                           If you want to advertise one of the original P2PS defined services,
 *                           this shall be NULL.
 * @param[out] adv_service   service descriptor handle
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_advert_destroy()
 */
int asp_advert_create(char *instance_name, asp_advert_service_h *adv_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Destroys the description of a service to be advertised.
 * @details If asp_advert_start_advertising() was called for a service,
 * asp_advert_stop_advertising() should be called for it before destroying it.
 * @since_tizen 4.0
 * @param[in] adv_service Service descriptor handle provided by the asp_advert_create()
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_destroy(asp_advert_service_h adv_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the service type for a service to be advertised.
 * @details Application should set service type after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service   Service descriptor handle provided by the asp_advert_create()
 * @param[in]               service_type  The unique type of a particular service.
 *                          this shall be at least 1 character and no more than 15 characters long
 *                          contain only US-ASCII [ANSI.X3.4-1986] letters 'A' - 'Z' and
 *                          'a' - 'z', digits '0' - '9', and hyphens ('-', ASCII 0x2D or decimal 45)
 *                          contain at least one letter ('A' - 'Z' or 'a' - ‘z')
 *                          not begin or end with a hyphen. If you want to advertise one of the
 *                          original P2PS defined services, this means service name.
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_service_type(asp_advert_service_h adv_service,
		                            char *service_type) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets auto accept for a service to be advertised.
 * @details Application should set service auto accept after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] auto_accept  Enables/Disables auto accept based on the value TRUE/FALSE
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_auto_accept(asp_advert_service_h adv_service,
                               bool auto_accept) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Adds the information for a service to be advertised.
 * @details Application should set service information after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] key          Service-defined key data specified in Section 6 of RFC6763
 * @param[in] value        Service-defined value data specified in Section 6 of RFC6763
 *                         Only one value can be added for a given key. If a value is set for a key,
 *                         and another value was set for the key before, the old value will be
 *                         overwritten with the new one.
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_add_info(asp_advert_service_h adv_service,
                            const char *key, const char *value) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the information for a service to be advertised.
 * @details Application should set service information after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @remarks @a value should not be freed.
 * It is recommended to make a copy of it to use.
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] key          Service-defined key data specified in Section 6 of RFC6763
 * @param[in] length       Length of service-defined value data specified in Section 6 of RFC6763
 * @param[in] value        Service-defined value data specified in Section 6 of RFC6763
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_get_info(asp_advert_service_h adv_service, const char *key,
                        int *length, char **value) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the information for a service to be advertised.
 * @details Application should set service information after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] key          Service-defined key data specified to be removed
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_remove_info(asp_advert_service_h adv_service, const char *key) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the status for a service to be advertised.
 * @details Application should set service status after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] status       Status of the service:
 *                         (@c 1 = available to use, @c 0 = not available to use,
 *                         @c 2-255 = service specific information)
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_status(asp_advert_service_h adv_service,
                          unsigned char status) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the discovery mechanism for a service to be advertised.
 * @details Application should set discovery mechanism after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service     Service descriptor handle provided by the asp_advert_create()
 * @param[in] discovery_tech  The discovery mechanism; values of
 *                            #asp_discovery_tech_e combined with bitwise 'or'
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_discovery_tech(asp_advert_service_h adv_service, int discovery_tech) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the preferred connection for a service to be advertised.
 * @details Application should set preferred connection after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service           Service descriptor handle provided by the asp_advert_create()
 * @param[in] preferred_connection  The preferred connection
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_preferred_connection(asp_advert_service_h adv_service,
                                        unsigned char preferred_connection) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the Wi-Fi P2P role for a service to be advertised.
 * @details Application should set role after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] role         The role of the service: \n
 *                         #ASP_ADVERT_P2P_ROLE_SCHEME_ANY - All roles are acceptable \n
 *                         #ASP_ADVERT_P2P_ROLE_SCHEME_GO  - The receiving interface should assume \n
 *                         the GO role, if it's not possible, #ASP_ERROR_NETWORK_ROLE_REJECTED \n
 *                         is returned \n
 *                         #ASP_ADVERT_P2P_ROLE_SCHEME_GC  -
 *                         The receiving interface should assume the GC role
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE                   Successful
 * @retval #ASP_ERROR_NOT_PERMITTED          Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY          Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER      Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED          Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED       Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND      Service not found
 * @retval #ASP_ERROR_NETWORK_ROLE_REJECTED  Network role rejected
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_p2p_role_scheme(asp_advert_service_h adv_service,
                                   asp_advert_p2p_role_scheme_e role) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the Wi-Fi P2P role for a service to be advertised.
 * @details Application should Get role after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] role         The role of the service: \n
 *                         #ASP_ADVERT_P2P_ROLE_SCHEME_ANY - All roles are acceptable \n
 *                         #ASP_ADVERT_P2P_ROLE_SCHEME_GO  - The receiving interface should assume \n
 *                         the GO role \n
 *                         #ASP_ADVERT_P2P_ROLE_SCHEME_GC  -
 *                         The receiving interface should assume the GC role
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_get_p2p_role_scheme(asp_advert_service_h adv_service,
                                   asp_advert_p2p_role_scheme_e *role) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the P2P configuration method for a service to be advertised.
 * @details Application should set configuration method after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service    Service descriptor handle provided by the asp_advert_create()
 * @param[in] config_method  preferred Wi-Fi Simple Config (WSC) configuration method.
 *                           Default value is #ASP_WPS_TYPE_DEFAULT.
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_p2p_config_method(asp_advert_service_h adv_service,
                                     asp_wps_type_e config_method) TIZEN_DEPRECATED_API;
/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the P2P configuration method for a service to be advertised.
 * @details Application should Get configuration method after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @param[in] adv_service    Service descriptor handle provided by the asp_advert_create()
 * @param[in] config_method  preferred Wi-Fi Simple Config (WSC)configuration method.
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_get_p2p_config_method(asp_advert_service_h adv_service,
                                     asp_wps_type_e *config_method) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the service response for an advertised service.
 * @details Application should set service response after creating service using
 * asp_advert_create() and before advertising service using
 * asp_advert_create().
 * @since_tizen 4.0
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] rsp_info     Specified for a particular service up to 144 bytes.
 *                         You can refer to Wi-Fi Peer-to-Peer Services Technical Specification,
 *                         Wi-Fi Alliance. Available at: http://www.wi-fi.org
 * @param[in] length       The length of @a rsp_info in bytes, maximum length is 144
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 */
int asp_advert_set_p2p_response(asp_advert_service_h adv_service,
                                char *rsp_info, int length) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Starts to advertise a service.
 * @details Application should start to advertise after creating service using
 * asp_advert_create(). If the service is being advertised, no changes can be made to it.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_create() before use
 * @see asp_advert_create()
 * @see asp_advert_stop_advertising()
 */
int asp_advert_start_advertising(asp_advert_service_h adv_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Stops a service advertisement.
 * @details Application should cancel to advertise after advertising service using
 * asp_advert_start_advertising().
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_start_advertising() before use
 * @see asp_advert_start_advertising()
 */
int asp_advert_stop_advertising(asp_advert_service_h adv_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Changes the advertising service status.
 * @details Application should change the advertising service
 * status after advertising service using asp_advert_start_advertising().
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] adv_service  Service descriptor handle provided by the asp_advert_create()
 * @param[in] status       Status of the service:
 *                         (@c 1 = available to use, @c 0 = not available to use,
 *                         @c 2-255 = service specific information)
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_advert_start_advertising() before use
 * @see asp_advert_start_advertising()
 */
int asp_advert_change_service_status(asp_advert_service_h adv_service,
                                     unsigned char status) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Creates the description of a seek operation.
 * @remarks The @a seek_service should be released using asp_seek_destroy().
 * @since_tizen 4.0
 * @param[in] service_type   Service Type of a service being searched.
 *                           this shall be at least 1 character and no more than 15 characters long
 *                           contain only US-ASCII [ANSI.X3.4-1986] letters 'A' - 'Z' and
 *                           'a' - 'z', digits '0' - '9', and hyphens ('-', ASCII 0x2D or decimal 45)
 *                           contain at least one letter ('A' - 'Z' or 'a' - ‘z')
 *                           not begin or end with a hyphen. If you want to seek one of the
 *                           original P2PS defined services, this means service name.
 * @param[out] seek_service  Service seek handle
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @pre Application Service Platform (ASP) must be initialized by asp_initialize().
 * @see asp_seek_destroy()
 */
int asp_seek_create(char *service_type, asp_seek_service_h *seek_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Destroys the description of a seek operation.
 * @details If asp_seek_start() was called for a service,
 * asp_seek_stop() should be called for it before destroying it.
 * asp_seek_stop().
 * @since_tizen 4.0
 * @param[in] seek_service  Service seek handle provided by the asp_seek_create()
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre A seek service handle must be created with asp_seek_create().
 * @see asp_seek_create
 */
int asp_seek_destroy(asp_seek_service_h seek_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Adds the information for a service to be sought.
 * @details Application should set service information after creating service using
 * asp_seek_create() and before seeking service using asp_seek_start().
 * @since_tizen 4.0
 * @privlevel public
 * @param[in] seek_service  Service descriptor handle provided by the asp_seek_create()
 * @param[in] key           Service-defined key data to be searched,
 *                          specified in Section 6 of RFC6763
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_seek_create() before use
 * @see asp_seek_create()
 */
int asp_seek_add_info(asp_seek_service_h seek_service, const char *key) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Removes the information for a service to be sought.
 * @details Application should set service information after creating service using
 * asp_seek_create() and before seeking service using asp_seek_start().
 * @since_tizen 4.0
 * @param[in] seek_service  Service descriptor handle provided by the asp_seek_create()
 * @param[in] key           Service-defined key data to be searched,
 *                          specified in Section 6 of RFC6763
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_seek_create() before use
 * @see asp_seek_create()
 */
int asp_seek_remove_info(asp_seek_service_h seek_service, const char *key) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the discovery mechanism for a service to be sought.
 * @details Application should set service information after creating service using
 * asp_seek_create() and before seeking service using asp_seek_start().
 * @since_tizen 4.0
 * @param[in] seek_service    Service descriptor handle provided by the asp_seek_create()
 * @param[in] discovery_tech  The discovery mechanism, values of
 *                            #asp_discovery_tech_e combined with bitwise 'or'
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_seek_create() before use
 * @see asp_seek_create()
 */
int asp_seek_set_discovery_tech(asp_seek_service_h seek_service, int discovery_tech) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the preferred connection for a service to be sought.
 * @details Application should set service information after creating service using
 * asp_seek_create() and before seeking service using asp_seek_start().
 * @since_tizen 4.0
 * @param[in] seek_service          Service descriptor handle provided by the asp_seek_create()
 * @param[in] preferred_connection  The preferred connection
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre This API needs asp_seek_create() before use
 * @see asp_seek_create()
 */
int asp_seek_set_preferred_connection(asp_seek_service_h seek_service,
                                      unsigned char preferred_connection) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Starts to seek services on peer devices.
 * @details If the service is currently seeking, no changes can be made to it.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] seek_service  Service seek handle provided by the asp_seek_create()
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre A seek service handle must be created with asp_seek_create().
 * @see asp_seek_create()
 * @see asp_seek_stop()
 */
int asp_seek_start(asp_seek_service_h seek_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Stops seeking services.
 * @details Application should cancel to seek service after searching service using
 * asp_seek_start().
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] seek_service  Service seek handle provided by the asp_seek_create()
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SERVICE_NOT_FOUND  Service not found
 * @pre A seek service handle must be created with asp_seek_create().
 * @see asp_seek_start()
 */
int asp_seek_stop(asp_seek_service_h seek_service) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Creates a local Application Service Platform (ASP) session.
 * @since_tizen 4.0
 * @remarks The @a session should be released using asp_session_destroy().
 * @param[in] service_mac  P2P device address of remote P2P device provided by
 *                         asp_seek_search_result_cb()
 * @param[in] adv_id       The advertisement ID provided by
 *                         asp_seek_search_result_cb()
 * @param[out] session     ASP session descriptor handle
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @see asp_session_destroy()
 */
int asp_session_create(char *service_mac, unsigned int adv_id,
                       asp_session_h *session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Destroys a local Application Service Platform (ASP) session.
 * @details Application should create session by asp_session_create().
 * @since_tizen 4.0
 * @param[in] session  Application Service Platform session descriptor handle
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_destroy(asp_session_h session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the session MAC for an Application Service Platform session.
 * @details Application should request to connect session by
 * asp_session_connect() or receive request by asp_session_request_cb().
 * @since_tizen 4.0
 * @remarks @a session_mac should not be freed.
 * It is recommended to make a copy of it to use.
 * @param[in] session       Application Service Platform session descriptor handle
 * @param[out] session_mac  The session MAC is the MAC address of
 *                          P2P device which assigned the session ID
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_connect() before use.
 * @see asp_session_connect()
 * @see asp_session_request_cb()
 */
int asp_session_get_mac(asp_session_h session, char **session_mac) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the ID of an Application Service Platform session.
 * @details Application should create local session by
 * asp_session_create() or receive request by asp_session_request_cb().
 * @since_tizen 4.0
 * @param[in] session      Application Service Platform session descriptor handle
 * @param[out] session_id  The session ID
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 * @see asp_session_request_cb()
 */
int asp_session_get_id(asp_session_h session, unsigned int *session_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the information for an Application Service Platform session.
 * @details Application should set session_info after creating session by
 * asp_session_create() and before requesting to connect
 * session by asp_session_connect().
 * @since_tizen 4.0
 * @param[in] session       Application Service Platform session descriptor handle
 *                          provided by the asp_session_create()
 * @param[in] session_info  Detailed information about the session.
 *                          The session_info parameter is defined for each service section
 *                          and up to 144 bytes. You can refer to Wi-Fi Peer-to-Peer Services
 *                          Technical Specification, Wi-Fi Alliance.
 *                          Available at: http://www.wi-fi.org
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_set_info(asp_session_h session, char *session_info) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the information for an Application Service Platform session.
 * @details Application should get session_info after creating session by
 * asp_session_create() and before requesting to connect
 * session by asp_session_connect().
 * @since_tizen 4.0
 * @remarks @a session_info should not be freed.
 * It is recommended to make a copy of it to use.
 * @param[in] session        Application Service Platform session descriptor handle
 *                           provided by the asp_session_create()
 * @param[out] session_info  detailed information about session
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_get_info(asp_session_h session, char **session_info) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the Wi-Fi Direct group role in session.
 * @details Application should set role after creating session by
 * asp_session_create() and before requesting to connect
 * session by asp_session_connect().
 * @since_tizen 4.0
 * @param[in] session  Application Service Platform session descriptor handle
 *                     provided by the asp_session_create()
 * @param[in] role     The role of the service: \n
 *                     #ASP_ADVERT_P2P_ROLE_SCHEME_ANY - All roles are acceptable \n
 *                     #ASP_ADVERT_P2P_ROLE_SCHEME_GO  - The receiving interface should assume \n
 *                     the GO role, if it's not possible, #ASP_ERROR_NETWORK_ROLE_REJECTED \n
 *                     is returned \n
 *                     #ASP_ADVERT_P2P_ROLE_SCHEME_GC  -
 *                     The receiving interface should assume the GC role
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE                  Successful
 * @retval #ASP_ERROR_NOT_PERMITTED         Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY         Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED         Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED      Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND     Session not found
 * @retval #ASP_ERROR_NETWORK_ROLE_REJECTED Network role rejected
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_set_p2p_role_scheme(asp_session_h session,
                                    asp_advert_p2p_role_scheme_e role) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the Wi-Fi Direct group role in session.
 * @since_tizen 4.0
 * @param[in] session  Application Service Platform session descriptor handle
 *                     provided by the asp_session_create()
 * @param[in] role     The role of the service: \n
 *                     #ASP_ADVERT_P2P_ROLE_SCHEME_ANY - All roles are acceptable \n
 *                     #ASP_ADVERT_P2P_ROLE_SCHEME_GO  - The receiving interface should assume \n
 *                     the GO role\n
 *                     #ASP_ADVERT_P2P_ROLE_SCHEME_GC  -
 *                     The receiving interface should assume the GC role
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_get_p2p_role_scheme(asp_session_h session,
                                    asp_advert_p2p_role_scheme_e *role) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the Preferred WSC Configuration method.
 * @details Application should set the config method after creating session by
 * asp_session_create() and before requesting to connect
 * session by asp_session_connect().
 * @since_tizen 4.0
 * @param[in] session        Application Service Platform session descriptor handle
 *                           provided by the asp_session_create()
 * @param[in] config_method  Preferred WSC Configuration method.
 *                           Default value is #ASP_WPS_TYPE_DEFAULT.
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_set_p2p_config_method(asp_session_h session,
                                      asp_wps_type_e config_method) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the Preferred WSC Configuration method.
 * @since_tizen 4.0
 * @param[in] session         Application Service Platform session descriptor handle
 *                            provided by the asp_session_create()
 * @param[out] config_method  Preferred WSC Configuration method
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_get_p2p_config_method(asp_session_h session,
                                      asp_wps_type_e *config_method) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the session close status.
 * @details This call is valid only for closed sessions (sessions in the #ASP_SESSION_STATE_CLOSED
 *          state). If the @a session does not identify a closed session,
 *          #ASP_ERROR_NOT_PERMITTED is returned.
 * @since_tizen 4.0
 * @param[in] session  ASP session descriptor handle provided by
 *                     the asp_session_request_cb() callback,
 *                     asp_session_create(), or asp_session_get_handle()
 * @param[out] status  The session close status
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 */
int asp_session_get_close_status(asp_session_h session, int *status) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Starts to connect the Application Service Platform session.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] session  Application Service Platform session descriptor handle
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre This API needs asp_session_create() before use.
 * @see asp_session_create()
 */
int asp_session_connect(asp_session_h session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Confirms the Application Service Platform(ASP) session request.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] session    ASP session descriptor handle provided by
 *                       the asp_session_request_cb() callback or
 *                       asp_session_create() if pin is requested.
 * @param[in] confirmed  (@c true = confirm session, @c false = reject session)
 * @param[in] pin        If session requests the WSC PIN
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @see asp_advert_start_advertising()
 * @see asp_session_create()
 */
int asp_session_confirm(asp_session_h session, bool confirmed, char *pin) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the Description of Application Service Platform (ASP) session to be connected.
 * @since_tizen 4.0
 * @param[in] session_mac  The MAC address of the P2P device that assigned
 *                         the value of session_id, as provided by
 *                         the asp_session_request_cb() callback or
 *                         asp_session_create() if pin is requested.
 * @param[in] session_id   The session ID
 * @param[out] session     Peer-to-Peer service session descriptor handle
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @see asp_session_create()
 */
int asp_session_get_handle(char *session_mac, unsigned int session_id,
                           asp_session_h *session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Moves an Application Service Platform (ASP) session from Requested state to the Open state.
 * @since_tizen 4.0
 * @param[in] session  ASP session descriptor handle provided by
 *                     the asp_session_request_cb() callback
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @pre Peer-to-Peer service must be advertised by asp_advert_start_advertising().\n
 * Local device should be listening or finding remote P2P device by
 * asp_advert_start_advertising()
 * @see asp_advert_start_advertising()
 */
int asp_session_set_state_ready(asp_session_h session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Ends an Application Service Platform (ASP) session and releases associated resources.
 * @details The ASP may tear down the underlying ASP P2P group
 * if not other ASP sessions are using it.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] session  ASP session descriptor handle provided by
 *                     the asp_session_request_cb() callback or
 *                     asp_session_create()
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @see asp_session_create()
 * @see asp_session_request_cb()
 */
int asp_session_close(asp_session_h session) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Requests that incoming connections be allowed on a given port.
 * @since_tizen 4.0
 * @param[in] session     ASP session descriptor handle provided by
 *                        the asp_session_request_cb() callback or
 *                        asp_session_create()
 * @param[in] ip_address  The IP address on which the port is being used
 * @param[in] port        The port number
 * @param[in] proto       The IANA protocol number
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @see asp_session_create()
 * @see asp_session_request_cb()
 */
int asp_session_bind_port(asp_session_h session, char *ip_address,
                          int port, int proto) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Indicates that the service is no longer utilizing the port in the ASP session.
 * @since_tizen 4.0
 * @param[in] session     ASP session descriptor handle provided by
 *                        the asp_session_request_cb() callback or
 *                        asp_session_create()
 * @param[in] ip_address  The IP address on which the port is being used
 * @param[in] port        The port number
 * @param[in] proto       The IANA protocol number
 * @return 0 on success, otherwise a negative error value
 * @retval #ASP_ERROR_NONE               Successful
 * @retval #ASP_ERROR_NOT_PERMITTED      Operation not permitted
 * @retval #ASP_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #ASP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #ASP_ERROR_NOT_SUPPORTED      Not supported
 * @retval #ASP_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #ASP_ERROR_OPERATION_FAILED   Operation failed
 * @retval #ASP_ERROR_SESSION_NOT_FOUND  Session not found
 * @see asp_session_create()
 * @see asp_session_request_cb()
 */
int asp_session_release_port(asp_session_h session, char *ip_address,
                             int port, int proto) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_NET_ASP_H__ */

