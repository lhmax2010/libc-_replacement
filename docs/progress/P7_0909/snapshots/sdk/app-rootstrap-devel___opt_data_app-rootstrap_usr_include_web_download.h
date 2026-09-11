/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_WEB_DOWNLOAD_H__
#define __TIZEN_WEB_DOWNLOAD_H__

#include <tizen.h>
#include <app_control.h>

#ifndef DEPRECATED
	#define DEPRECATED __attribute__((deprecated))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file download.h
 */

 /**
 * @addtogroup CAPI_WEB_DOWNLOAD_MODULE
 * @{
 */

/**
 * @brief Enumeration for error codes of URL download.
 * @since_tizen 2.3
 */
typedef enum {
	DOWNLOAD_ERROR_NONE = TIZEN_ERROR_NONE,                                 /**< Successful */
	DOWNLOAD_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,       /**< Invalid parameter */
	DOWNLOAD_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,               /**< Out of memory */
	DOWNLOAD_ERROR_NETWORK_UNREACHABLE = TIZEN_ERROR_NETWORK_UNREACHABLE,   /**< Network is unreachable */
	DOWNLOAD_ERROR_CONNECTION_TIMED_OUT = TIZEN_ERROR_CONNECTION_TIME_OUT,  /**< HTTP session time-out */
	DOWNLOAD_ERROR_NO_SPACE = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE,          /**< No space left on device */
	DOWNLOAD_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,       /**< Permission denied */
	DOWNLOAD_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,               /**< Not supported */
	DOWNLOAD_ERROR_INVALID_STATE = TIZEN_ERROR_DOWNLOAD | 0x21,             /**< Invalid state */
	DOWNLOAD_ERROR_CONNECTION_FAILED = TIZEN_ERROR_DOWNLOAD | 0x22,         /**< Connection failed */
	DOWNLOAD_ERROR_INVALID_URL = TIZEN_ERROR_DOWNLOAD | 0x24,               /**< Invalid URL */
	DOWNLOAD_ERROR_INVALID_DESTINATION = TIZEN_ERROR_DOWNLOAD | 0x25,       /**< Invalid destination */
	DOWNLOAD_ERROR_TOO_MANY_DOWNLOADS = TIZEN_ERROR_DOWNLOAD | 0x26,        /**< Too many simultaneous downloads */
	DOWNLOAD_ERROR_QUEUE_FULL = TIZEN_ERROR_DOWNLOAD | 0x27,                /**< Download server queue is full*/
	DOWNLOAD_ERROR_ALREADY_COMPLETED = TIZEN_ERROR_DOWNLOAD | 0x28,         /**< The download is already completed */
	DOWNLOAD_ERROR_FILE_ALREADY_EXISTS = TIZEN_ERROR_DOWNLOAD | 0x29,       /**< Failed to rename the downloaded file */
	DOWNLOAD_ERROR_CANNOT_RESUME = TIZEN_ERROR_DOWNLOAD | 0x2a,             /**< Cannot resume */
	DOWNLOAD_ERROR_FIELD_NOT_FOUND = TIZEN_ERROR_DOWNLOAD | 0x2b,           /**< Specified field not found */
	DOWNLOAD_ERROR_TOO_MANY_REDIRECTS = TIZEN_ERROR_DOWNLOAD | 0x30,        /**< Too many redirects from HTTP response header*/
	DOWNLOAD_ERROR_UNHANDLED_HTTP_CODE = TIZEN_ERROR_DOWNLOAD | 0x31,       /**< The download cannot handle the HTTP status value */
	DOWNLOAD_ERROR_REQUEST_TIMEOUT = TIZEN_ERROR_DOWNLOAD | 0x32,           /**< No action after client creates a download ID*/
	DOWNLOAD_ERROR_RESPONSE_TIMEOUT = TIZEN_ERROR_DOWNLOAD | 0x33,          /**< No call to start API for some time although the download is created*/
	DOWNLOAD_ERROR_SYSTEM_DOWN = TIZEN_ERROR_DOWNLOAD | 0x34,               /**< No response from client after rebooting download daemon*/
	DOWNLOAD_ERROR_ID_NOT_FOUND = TIZEN_ERROR_DOWNLOAD | 0x35,              /**< Download ID does not exist in download service module*/
	DOWNLOAD_ERROR_INVALID_NETWORK_TYPE = TIZEN_ERROR_DOWNLOAD | 0x36,      /**< Network bonding is set but network type is not set as #DOWNLOAD_NETWORK_ALL*/
	DOWNLOAD_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA,                           /**< No data because the set API is not called */
	DOWNLOAD_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR ,                        /**< Internal I/O error */
} download_error_e;


/**
 * @brief Enumeration for download states.
 * @since_tizen 2.3
 */
typedef enum {
	DOWNLOAD_STATE_NONE,         /**< Unhandled exception */
	DOWNLOAD_STATE_READY,        /**< Ready to download */
	DOWNLOAD_STATE_QUEUED,       /**< Queued to start downloading */
	DOWNLOAD_STATE_DOWNLOADING,  /**< Currently downloading */
	DOWNLOAD_STATE_PAUSED,       /**< The download is waiting to resume */
	DOWNLOAD_STATE_COMPLETED,    /**< The download is completed */
	DOWNLOAD_STATE_FAILED,       /**< The download failed */
	DOWNLOAD_STATE_CANCELED,     /**< User canceled the download item */
} download_state_e;


/**
 * @brief Enumeration for network type for downloading.
 * @since_tizen 2.3
 */
typedef enum {
	DOWNLOAD_NETWORK_DATA_NETWORK, /**< Download is available through data network */
	DOWNLOAD_NETWORK_WIFI,         /**< Download is available through WiFi */
	DOWNLOAD_NETWORK_WIFI_DIRECT,  /**< Download is available through WiFi-Direct */
	DOWNLOAD_NETWORK_ALL           /**< Download is available through either data network or WiFi */
} download_network_type_e;


/**
 * @brief Enumeration for notification types when a client wants to register.
 * @since_tizen 2.3
 *
 * @see download_set_notification_type()
 * @see download_get_notification_type()
 */
typedef enum {
	DOWNLOAD_NOTIFICATION_TYPE_NONE = 0,       /**< Do not register notification */
	DOWNLOAD_NOTIFICATION_TYPE_COMPLETE_ONLY,  /**< Completion notification for success state and failed state */
	DOWNLOAD_NOTIFICATION_TYPE_ALL             /**< All download notifications for ongoing state, success state and failed state */
} download_notification_type_e;


/**
 * @brief Enumeration for the type of notification app control action which the client wants to set when registering notification.
 * @since_tizen 2.3
 *
 * @see download_set_notification_app_control()
 * @see download_get_notification_app_control()
 */
typedef enum {
	DOWNLOAD_NOTIFICATION_APP_CONTROL_TYPE_ONGOING = 0,  /**< App control action for failed and ongoing notification */
	DOWNLOAD_NOTIFICATION_APP_CONTROL_TYPE_COMPLETE,     /**< App control action for completed notification */
	DOWNLOAD_NOTIFICATION_APP_CONTROL_TYPE_FAILED        /**< App control action for failed notification*/
} download_notification_app_control_type_e;


/**
 * @brief Called when a download state is changed.
 *
 * @since_tizen 2.3
 *
 * @param[in] download_id  The download ID
 * @param[in] state        The state of download
 * @param[in] user_data    The user data passed from download_set_state_changed_cb()
 *
 * @pre download_start() will invoke this callback if you register this callback using download_set_state_changed_cb().
 *
 * @see download_set_state_changed_cb()
 * @see download_unset_state_changed_cb()
 */
typedef void (*download_state_changed_cb) (int download_id,
		download_state_e state, void *user_data);


/**
 * @brief Called when the progress of download changes.
 *
 * @since_tizen 2.3
 *
 * @remarks This callback function is only invoked in the downloading state.
 *
 * @param[in] download_id  The download ID
 * @param[in] received     The size of the data received in bytes
 * @param[in] user_data    The user data passed from download_set_progress_cb()
 *
 * @pre This callback function is invoked if you register this callback using download_set_progress_cb().
 *
 * @see download_cancel()
 * @see download_set_progress_cb()
 * @see download_unset_progress_cb()
 */
typedef void (*download_progress_cb) (int download_id, unsigned long long received, void *user_data);


/**
 * @brief Initializes a new download request and returns download ID that can be used to control the download process.
 * @details @a download_id is the handle for managing download progress, pausing, resuming, or canceling downloads.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a download_id using download_destroy().
 *          Even if the device is off, @a download_id is available for 48 hours if the user does not call download_destroy().

 * @param[out] download_id The download ID that is newly created on success
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_IO_ERROR          Internal I/O error
 * @retval #DOWNLOAD_ERROR_QUEUE_FULL        Download server queue is full
 *
 * @post The download state will be #DOWNLOAD_STATE_READY.
 *
 * @see download_destroy()
 */
int download_create(int *download_id);


/**
 * @brief Frees the resources associated with a previously created download ID.
 * @details This function should be called when the download is completed or no longer needed to prevent memory leaks.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks After calling this function, the @a download_id is existed at DB in certain time.
 * Within that time, it is able to use the other functions with the @a download_id.
 * If #DOWNLOAD_ERROR_ID_NOT_FOUND is returned, it means that the @a download_id is completely removed from DB.
 *
 * @param[in] download_id The download ID to be destroyed
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_create()
 */
int download_destroy(int download_id);


/**
 * @brief Sets the URL for the file to be downloaded.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *
 * @param[in] download_id  The download ID
 * @param[in] url          The URL to download \n
 *                         If @a url is @c NULL, it clears the previous value.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_get_url()
 */
int download_set_url(int download_id, const char *url);


/**
 * @brief Gets the URL for the file to be downloaded.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a url using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] url          The URL to download
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_url()
 */
int download_get_url(int download_id, char **url);


/**
 * @brief Sets the type of network (e.g., Wi-Fi) that should be used for the download.
 * @details The file can be downloaded only under the allowed network.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *
 * @param[in] download_id  The download ID
 * @param[in] net_type     The network type that the user prefers
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_get_network_type()
 * @see #download_network_type_e
 */
int download_set_network_type(int download_id, download_network_type_e net_type);


/**
 * @brief Gets the current network type that is set for the download.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in] download_id  The download ID
 * @param[out] net_type    The network type that is defined by the user
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_network_type()
 * @see #download_network_type_e
 */
int download_get_network_type(int download_id, download_network_type_e *net_type);


/**
 * @brief Sets the file path where the downloaded content will be saved on the device.
 * @details This function allows the application to define the location on the filesystem where the downloaded file should be stored.
 *          If the destination is not specified, the file will be downloaded to a default storage (see the @ref CAPI_SYSTEM_STORAGE_MODULE API).
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()). \n
 *          The mediastorage privilege %http://tizen.org/privilege/mediastorage is needed if @a path is relevant to media storage.\n
 *          The externalstorage privilege %http://tizen.org/privilege/externalstorage is needed if @a path is relevant to external storage.
 *
 * @param[in] download_id  The download ID
 * @param[in] path         The absolute path where the downloaded file will be saved \n
 *                         If @a path is @c NULL, it clears the previous value.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE                Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE       Invalid state
 * @retval #DOWNLOAD_ERROR_INVALID_DESTINATION Invalid destination
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND        No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_get_destination()
 */
int download_set_destination(int download_id, const char *path);


/**
 * @brief Gets the file path where the downloaded content will be or has been saved.
 * @details This function allows the application to verify or retrieve the path where the downloaded file is stored.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a path using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] path         A pointer to store the absolute path where the downloaded file is saved
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_destination()
 */
int download_get_destination(int download_id, char **path);


/**
 * @brief Sets the name to be used for the downloaded content.
 * @details This function allows the application to specify the file name for the content, independent of the URL or server-provided name.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *
 * @param[in] download_id  The download ID
 * @param[in] file_name    The desired file name for the downloaded content. \n
 *                         If @a file_name is @c NULL it clears the previous value.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_get_file_name()
 */
int download_set_file_name(int download_id, const char *file_name);


/**
 * @brief Gets the name of the downloaded content previously set by download_set_file_name().
 * @details This function allows the application to check the file name that has been assigned for the download.
 *          If the name is not set, @c NULL is returned.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a file_name using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] file_name    The file name which is set by the application
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_file_name()
 */
int download_get_file_name(int download_id, char **file_name);


/**
 * @brief Gets the absolute file path of the downloaded content.
 * @details This function returns the absolute file path where the downloaded content has been saved after the download has been completed.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function returns #DOWNLOAD_ERROR_INVALID_STATE if the download is not completed.
 *          The caller must release @a path using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] path         A pointer to store the file path of the downloaded content
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE               Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND       No download ID
 *
 * @pre The download state must be #DOWNLOAD_STATE_COMPLETED.
 *
 * @see download_set_file_name()
 * @see download_set_destination()
 */
int download_get_downloaded_file_path(int download_id, char **path);


/**
 * @brief Gets the MIME type of the downloaded content.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function returns #DOWNLOAD_ERROR_INVALID_STATE if the download has not been started.
 *          The caller must release @a mime_type using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] mime_type    The MIME type of the downloaded file
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_file_name()
 * @see download_set_destination()
 * @see download_get_downloaded_file_path()
 */
int download_get_mime_type(int download_id, char **mime_type);


/**
 * @brief Enables or disables automatic downloading of the file.
 * @details This function allows the application to control whether the download should start automatically
 *          after the download daemon is restarted, or if it should be manually triggered.
 *          The download progress continues after the application process is terminated.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The default value is @c false.
 *
 * @param[in] download_id The download ID
 * @param[in] enable      A boolean value indicating whether to enable automatic download \n
 *                        @c true to start automatically,
 *                        @c false to require manual start
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_get_auto_download()
 *
 */
int download_set_auto_download(int download_id, bool enable);


/**
 * @brief Gets whether the download is set to start automatically.
 *
 * @details This function allows the application to check if the download has been configured to start automatically
 *          or if it requires manual initiation.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in]  download_id The download ID
 * @param[out] enable      A pointer to a boolean variable where the auto-download status will be stored \n
 *                         @c true if the download starts automatically,
 *                         @c false if it requires manual initiation
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_auto_download()
 */
int download_get_auto_download(int download_id, bool *enable);


/**
 * @brief Adds an HTTP header field to the download request.
 * @details This function allows the application to append custom headers to the download request,
 *          which can be used for authentication, content negotiation, or other HTTP operations.
 *          For more information, see <a href="HTTP://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.2">HTTP/1.1: HTTP Message Headers</a>.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()). \n
 *          It replaces any existing value for the given key. \n
 *          It returns #DOWNLOAD_ERROR_INVALID_PARAMETER if @a field or @a value is a zero-length string.
 *
 * @param[in] download_id  The download ID
 * @param[in] field        The name of the HTTP header field
 * @param[in] value        The value of the HTTP header field
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_IO_ERROR Internal I/O error
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_get_http_header_field()
 * @see download_remove_http_header_field()
 */
int download_add_http_header_field(int download_id, const char *field, const char *value);


/**
 * @brief Gets the value of a specific HTTP header field that was added to the download request.
 * @details This function allows the application to query the value of an HTTP header field previously set using download_add_http_header_field().
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function returns #DOWNLOAD_ERROR_INVALID_PARAMETER if @a field is zero-length string.
 *          The caller must release @a value using free().
 *
 * @param[in]  download_id  The download ID
 * @param[in]  field        The name of the HTTP header field
 * @param[out] value        The value associated with given field
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_FIELD_NOT_FOUND   Specified field not found
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_add_http_header_field()
 * @see download_remove_http_header_field()
 */
int download_get_http_header_field(int download_id, const char *field, char **value);


/**
 * @brief Gets a list of all HTTP header fields that were added to the download request.
 * @details This function provides access to the names of all HTTP headers that were appended using download_add_http_header_field().
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a fields using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] fields       A pointer to a list that will contain the names of the HTTP header fields
 * @param[out] length       The number of the HTTP header fields
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_add_http_header_field()
 * @see download_remove_http_header_field()
 */
int download_get_http_header_field_list(int download_id, char ***fields, int *length);


/**
 * @brief Removes a specific HTTP header field from the download request.
 * @details This function allows the application to delete a previously added HTTP header.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *          It returns #DOWNLOAD_ERROR_INVALID_PARAMETER if the field is zero-length string.
 *
 * @param[in] download_id  The download ID
 * @param[in] field        The name of the HTTP header field to remove
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_FIELD_NOT_FOUND   Specified field not found
 * @retval #DOWNLOAD_ERROR_IO_ERROR          Internal I/O error
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_add_http_header_field()
 * @see download_get_http_header_field()
 */
int download_remove_http_header_field(int download_id, const char *field);


/**
 * @brief Sets a callback function to be called when the download state changes.
 * @details This function allows the application to register a callback that is triggered
 *          when the download's state changes, such as when the download starts, pauses, completes, or fails.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *
 * @param[in] download_id  The download ID
 * @param[in] callback     The callback function to be invoked when the download state changes
 * @param[in] user_data    The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @post download_state_changed_cb() will be invoked.
 *
 * @see download_unset_state_changed_cb()
 * @see download_state_changed_cb()
*/
int download_set_state_changed_cb(int download_id, download_state_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback function for the download state changes.
 * @details This function removes the previously set callback for state changes,
 *          ensuring that no callback is invoked when the download's state changes.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *
 * @param[in] download_id The download ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_state_changed_cb()
 * @see download_state_changed_cb()
*/
int download_unset_state_changed_cb(int download_id);


/**
 * @brief Sets a callback function to be called to track the progress of the download.
 * @details This function allows the application to monitor the progress of the download in real time,
 *          typically used to update the UI with the current download percentage.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *
 * @param[in] download_id  The download ID
 * @param[in] callback     The callback function to be invoked as the download progresses
 * @param[in] user_data    The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @post download_progress_cb() will be invoked.
 *
 * @see download_unset_progress_cb()
 * @see download_progress_cb()
*/
int download_set_progress_cb(int download_id, download_progress_cb callback, void *user_data);


/**
 * @brief Unsets the callback function for the download progress.
 * @details This function removes the previously set progress callback,
 *          so no further notifications about download progress will be received.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks This function should be called before downloading (see download_start()).
 *
 * @param[in] download_id The download ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_progress_cb()
 * @see download_progress_cb()
*/
int download_unset_progress_cb(int download_id);


/**
 * @brief Starts or resumes the download process asynchronously for the given download ID.
 * @details This function starts to download the specified URL, or resumes the download if paused.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The URL is the mandatory information to start the download.
 * @remarks The application should call download_set_progress_cb() and download_set_state_changed_cb() again
 *          after the application process is restarted or download_destroy() is called.
 *
 * @param[in] download_id The download ID
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_IO_ERROR          Internal I/O error
 * @retval #DOWNLOAD_ERROR_INVALID_URL       Invalid URL
 * @retval #DOWNLOAD_ERROR_INVALID_DESTINATION Invalid destination
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 * @retval #DOWNLOAD_ERROR_QUEUE_FULL        Download server queue is full
 *
 * @pre  The download state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_PAUSED, #DOWNLOAD_STATE_CANCELED, or #DOWNLOAD_STATE_FAILED.
 * @post The download state will be #DOWNLOAD_STATE_QUEUED or #DOWNLOAD_STATE_DOWNLOADING.
 *
 * @see download_set_url()
 * @see download_pause()
 * @see download_cancel()
 */
int download_start(int download_id);


/**
 * @brief Pauses an ongoing download asynchronously.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The download can later be resumed using download_start() or canceled using download_cancel().
 *
 * @param[in] download_id The download ID
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_IO_ERROR          Internal I/O error
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre  The download state must be #DOWNLOAD_STATE_DOWNLOADING.
 * @post The download state will be #DOWNLOAD_STATE_PAUSED.
 *
 * @see download_start()
 * @see download_cancel()
 */
int download_pause(int download_id);


/**
 * @brief Cancels an active download, asynchronously.
 * @details This function cancels the running download and its state will be #DOWNLOAD_STATE_READY.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The canceled download can be restarted using download_start().
 *
 * @param[in] download_id The download ID
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #DOWNLOAD_ERROR_INVALID_STATE     Invalid state
 * @retval #DOWNLOAD_ERROR_IO_ERROR          Internal I/O error
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre  The download state must be #DOWNLOAD_STATE_QUEUED, #DOWNLOAD_STATE_DOWNLOADING, or #DOWNLOAD_STATE_PAUSED.
 * @post The download state will be #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_start()
 */
int download_cancel(int download_id);


/**
 * @brief Gets the current state of the download, such as whether it is in progress, paused, canceled, or completed.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in]  download_id  The download ID
 * @param[out] state        A pointer to store the retrieved state of the download
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see #download_state_e
 */
int download_get_state(int download_id, download_state_e *state);


/**
 * @brief Gets the temporary file path where the partially downloaded content is being stored.
 * @details This function provides access to the location where the file is saved during the download process before it is completed.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a temp_path using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] temp_path    A pointer to store the retrieved temporary file path
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The download state must be one of the states after #DOWNLOAD_STATE_DOWNLOADING.
 *
 * @see download_set_state_changed_cb()
 * @see download_unset_state_changed_cb()
 * @see download_start()
 */
int download_get_temp_path(int download_id, char **temp_path);


/**
 * @brief Sets the temporary file path used in the previous download request.
 * @details This is only useful when resuming download to make HTTP request header at the application side.
 *          Otherwise, the path should be ignored internally.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks If the etag value is not present in the download database, it is useless to set the temporary path. \n
 *          When resuming download, the data is attached at the end of this temporary file. \n
 *          The mediastorage privilege %http://tizen.org/privilege/mediastorage is needed if @a path is relevant to media storage.\n
 *          The externalstorage privilege %http://tizen.org/privilege/externalstorage is needed if @a path is relevant to external storage.

 *
 * @param[in]  download_id  The download ID
 * @param[out] path         The path where the temporary file will be stored
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_INVALID_DESTINATION Invalid destination
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_get_etag()
 */
int download_set_temp_file_path(int download_id, char *path);


/**
 * @brief Gets the content name of downloaded file.
 * @details This can be received when HTTP response header is received.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a content_name using free().
 *
 * @param[in]  download_id   The download ID
 * @param[out] content_name  A pointer to store the retrieved content name
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The download state must be one of the states after #DOWNLOAD_STATE_DOWNLOADING.
 *
 * @see download_set_state_changed_cb()
 * @see download_unset_state_changed_cb()
 * @see download_start()
 */
int download_get_content_name(int download_id, char **content_name);


/**
 * @brief Gets the size of the content being downloaded.
 * @details This information is received from the server.
 *          If the server does not send the total size of the content, @a content_size is set to zero.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in]  download_id  The download ID
 * @param[out] content_size A pointer to store the size of the content in bytes.
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The download state must be one of the states after #DOWNLOAD_STATE_DOWNLOADING.
 *
 * @see download_set_state_changed_cb()
 * @see download_unset_state_changed_cb()
 * @see download_start()
 */
int download_get_content_size(int download_id, unsigned long long *content_size);


/**
 * @brief Gets the last error that occurred during the download process.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in]  download_id The download ID
 * @param[out] error       A pointer to store the error code associated with the download
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The download state must be #DOWNLOAD_STATE_FAILED.
 * @pre The download state must be #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_set_state_changed_cb()
 * @see download_unset_state_changed_cb()
 * @see download_start()
 * @see download_error_e
 */
int download_get_error(int download_id, download_error_e *error);


/**
 * @brief Gets the HTTP status code when the download error occurs.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in]  download_id  The download ID
 * @param[out] HTTP_status  A pointer to store the retrieved HTTP status code defined in RFC 2616
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The download state must be #DOWNLOAD_STATE_FAILED.
 *
 * @see download_start()
 */
int download_get_http_status(int download_id, int *HTTP_status);


/**
 * @brief Sets an app control handle to register notification messages.
 * @details Three types of notification message can be posted: completion, failed and ongoing type.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks When the notification message is clicked, the action to take is decided by the app control handle. \n
 *          If the app control handle is not set, the following default operation is executed when the notification message is clicked: \n
 *          1) download completed state - the viewer application is executed according to extension name of downloaded content, \n
 *          2) download failed state and ongoing state - the client application is executed. \n
 *             This function should be called before starting the download. \n
 *             The app control handle MUST BE FREED by the client when it is not used any more.
 *
 * @param[in] download_id The download ID
 * @param[in] type        The enumeration type \n
 *                        See #download_notification_app_control_type_e.
 * @param[in] handle      The app control handle pointer value
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_set_notification_type()
 * @see download_get_notification_app_control()
 */
int download_set_notification_app_control(int download_id, download_notification_app_control_type_e type, app_control_h handle);


/**
 * @brief Gets the app control handle which is set by download_set_notification_app_control().
 * @details When the notification message is clicked, the action is decided by the app control handle.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a handle using free().
 *
 * @param[in]  download_id The download ID
 * @param[in]  type        The enumeration type \n
 *                         See #download_notification_app_control_type_e.
 * @param[out] handle      A pointer to store the retrieved app control handle
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 * @retval #DOWNLOAD_ERROR_NO_DATA           The app control handle has not been set
 *
 * @code
 * #include <download.h>
 * #include <app_control.h>
 * ...
 * int ret = download_get_notification_app_control(id, DOWNLOAD_NOTIFICATION_APP_CONTROL_TYPE_COMPLETE, &handle);
 * ...
 *
 * ret = app_control_destroy(handle);
 *
 * @endcode
 *
 * @see download_set_notification_app_control()
 */
int download_get_notification_app_control(int download_id, download_notification_app_control_type_e type, app_control_h *handle);


/**
 * @brief Sets the text to be displayed in the download notification.
 * @details When registering a notification, the title is displayed in the title area of the notification message.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in] download_id The download ID
 * @param[in] title       The title text to display in the notification
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre If a notification option is not enabled, this title is not shown to user.
 *
 * @see download_set_notification_type()
 * @see download_get_notification_title()
 */
int download_set_notification_title(int download_id, const char *title);


/**
 * @brief Gets the title text that is set to be displayed in the download notification.
 * @details When registering a notification, the title is displayed in the title area of the notification message.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a title using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] title        A pointer to store the retrieved title text
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 * @retval #DOWNLOAD_ERROR_NO_DATA           The title has not been set
 *
 * @pre It can get the title value before calling this function.
 *
 * @see download_set_notification_title()
  */
int download_get_notification_title(int download_id, char **title);


/**
 * @brief Sets the description text to be displayed in the download notification.
 * @details When registering a notification, the description is displayed in the description area of the notification message.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in] download_id  The download ID
 * @param[in] description  The description text to display in the notification
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre If the notification option is not enabled, this description is not shown to user.
 *
 * @see download_set_notification_type()
 * @see download_get_notification_description()
  */
int download_set_notification_description(int download_id, const char *description);


/**
 * @brief Gets the description text that is set to be displayed in the download notification.
 * @details When registering a notification, the description is displayed in the description area of the notification message.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The caller must release @a description using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] description  The description for displaying to user
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 * @retval #DOWNLOAD_ERROR_NO_DATA           The description has not been set
 *
 * @pre It can get the title value before calling this function.
 *
 * @see download_set_notification_description()
  */
int download_get_notification_description(int download_id, char **description);


/**
 * @brief Sets the type of notification to be displayed during the download process.
 * @details Three types of notification message can be posted: completion, failed and ongoing type.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks When the notification message is clicked, the action to take is decided by the app control handle (set by download_set_notification_app_control()). \n
 *          If the app control is not set, the following default operation is executed when the notification message is clicked: \n
 *          1) download completed state - the viewer application is executed according to extension name of downloaded content, \n
 *          2) download failed state and ongoing state - the client application is executed. \n
 *             The default type is #DOWNLOAD_NOTIFICATION_TYPE_NONE. \n
 *             This function should be called before starting the download.
 *
 * @param[in] download_id  The download ID
 * @param[in] type         The enumeration type \n
 *                         See #download_notification_type_e.
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @pre The state must be #DOWNLOAD_STATE_READY, #DOWNLOAD_STATE_FAILED, or #DOWNLOAD_STATE_CANCELED.
 *
 * @see download_set_notification_app_control()
 * @see download_get_notification_type()
 */
int download_set_notification_type(int download_id, download_notification_type_e type);


/**
 * @brief Gets the type of notification currently set for the download.
 * @details This function allows the application to check what type of notification is configured for the download process.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks When the notification message is clicked, the action is decided by the app control from download_set_notification_app_control(). \n
 *          The default type is #DOWNLOAD_NOTIFICATION_TYPE_NONE.
 *
 * @param[in]  download_id  The download ID
 * @param[out] type         A pointer to store the retrieved notification type \n
 *                          See #download_notification_type_e.
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_notification_type()
 */
int download_get_notification_type(int download_id, download_notification_type_e *type);


/**
 * @brief Gets the ETag (Entity Tag) from the server response for the download request when making a HTTP request for resume.
 * @details The ETag is a unique identifier assigned by the server to the content, which can be used to check for updates or changes to the file.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The ETag is available or not depending on the web server. \n
 *          After download is started, it can get the ETag.
 *
 * @remarks The caller must release @a etag using free().
 *
 * @param[in]  download_id  The download ID
 * @param[out] etag         A pointer to store the retrieved ETag
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_temp_file_path()
 */
int download_get_etag(int download_id, char **etag);


/**
 * @brief Enables or disables the use of cache for the download.
 * @details This function allows the developer to control whether the downloaded content should be cached locally,
 *          reducing the need for re-downloading the same content in the future.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in] download_id  The download id
 * @param[in] enable       A boolean value indicating whether to use the cache \n
 *                         @c true to enable caching,
 *                         @c false to disable it
 *
 * @return 0 on success, otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 * @retval #DOWNLOAD_ERROR_IO_ERROR          Internal I/O error
 *
 * @see download_get_cache()
*/
int download_set_cache(int download_id, bool enable);


/**
 * @brief Gets whether caching is enabled for the download.
 * @details This function allows the application to check if the download is set to use the cache.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in]  download_id  The download id
 * @param[out] enable       A pointer to a boolean variable that will store the cache setting
 *  *                       @c true to enable caching,
 *                          @c false to disable it
 *
 * @return 0 on success, otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 * @retval #DOWNLOAD_ERROR_IO_ERROR          Internal I/O error
 *
 * @see download_set_cache()
 */
int download_get_cache(int download_id, bool *enable);


/**
 * @brief Clears previously cached data.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @return 0 on success, otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE               Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED      Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DOWNLOAD_ERROR_TOO_MANY_DOWNLOADS Too many simultaneous downloads
 * @retval #DOWNLOAD_ERROR_IO_ERROR           Internal I/O error
 */
int download_reset_cache(void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_WEB_DOWNLOAD_H__ */
