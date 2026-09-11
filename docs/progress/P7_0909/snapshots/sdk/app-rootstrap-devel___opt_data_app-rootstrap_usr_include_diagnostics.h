/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_SYSTEM_DIAGNOSTICS_H__
#define __TIZEN_SYSTEM_DIAGNOSTICS_H__

#include <stdio.h>
#include <tizen.h>
#include <bundle.h>

#ifdef __cplusplus
extern "C" {
#endif

#define API __attribute__((visibility("default")))

/**
 * @addtogroup CAPI_SYSTEM_DIAGNOSTICS_MODULE
 * @{
 */

/**
 * @brief An opaque handle to a system diagnostics context object.
 * @details The diagnostics context is provided in data event callbacks.
 * @since_tizen 6.0
 */
typedef void* diagnostics_ctx_h;

/**
 * @brief An opaque handle to a system diagnostics data object.
 * @details A single piece of diagnostics data, to be read or written to.
 * @since_tizen 6.0
 */
typedef void* diagnostics_data_h;

/**
 * @brief Enumeration for error codes returned by the diagnostics API.
 * @details Most diagnostics API functions may return some of these values to indicate specific errors.
 * @since_tizen 6.0
 */
typedef enum {
    DIAGNOSTICS_ERROR_NONE = TIZEN_ERROR_NONE,                           /**< Successful */
    DIAGNOSTICS_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
    DIAGNOSTICS_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR,                   /**< I/O error */
    DIAGNOSTICS_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,         /**< Out of memory */
    DIAGNOSTICS_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY,         /**< Device or resource busy */
    DIAGNOSTICS_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT,                 /**< Time out */
    DIAGNOSTICS_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,         /**< Not supported */
    DIAGNOSTICS_ERROR_TRY_AGAIN = TIZEN_ERROR_TRY_AGAIN,                 /**< Try again */
    DIAGNOSTICS_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED  /**< Permission denied */
} diagnostics_error_e;

/**
 * @brief Notification callback fired when new diagnostics event arrives.
 * @since_tizen 6.0
 * @details @a ctx should be released with diagnostics_destroy() after use.
 *
 * @param[in] ctx Diagnostics context handle
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void(*diagnostics_notification_cb)(diagnostics_ctx_h ctx, void *user_data);

/**
 * @brief Request callback fired when someone requests diagnostics data.
 * @since_tizen 6.5
 * @details @a data should be released with diagnostics_data_destroy().
 *
 * @param[in] data Diagnostics data handle
 * @param[in] params Array of request's parameters \n
 *            This array is owned by @a data, so it is available until @a data is released \n
 *            @a params are the same as passed to diagnostics_request_client_data() or diagnostics_get_data()
 * @param[in] params_size Number of parameters
 * @param[in] ctx Diagnostics context handling an event that the request is related to \n
 *            @a ctx is available when data has been requested with diagnostics_get_data() \n
 *            @a ctx is NULL when data has been requested with diagnostics_request_client_data() \n
 *            This parameter allows diagnostics client to find diagnostics data related to the specific event
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*diagnostics_request_cb)(diagnostics_data_h data, char **params, int params_size, diagnostics_ctx_h ctx, void *user_data);

/**
 * @brief Sets the callback for diagnostics event notification.
 * @since_tizen 6.0
 * @details This function is dedicated to diagnostic information subscribers.
 *
 * @param[in] callback A callback function to set
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 * @retval #DIAGNOSTICS_ERROR_RESOURCE_BUSY Callback already registered
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 *
 * @code
 * #include <diagnostics.h>
 *
 * static void notification_handler(diagnostics_ctx_h ctx, void *user_data)
 * {
 *     // Process diagnostics event
 *
 *     diagnostics_destroy(ctx);
 * }
 *
 * static GMainLoop *mainloop = NULL;
 *
 * int main(int argc, char **argv)
 * {
 *     diagnostics_set_notification_cb(notification_handler, NULL);
 *     diagnostics_subscribe_event("ConnectionBroken", "org.tizen.someapp");
 *
 *     mainloop = g_main_loop_new(NULL, FALSE);
 *     g_main_loop_run(mainloop);
 * }
 *
 * @endcode
 */
API int diagnostics_set_notification_cb(diagnostics_notification_cb callback, void *user_data);

/**
 * @brief Unsets the callback for diagnostics event notification.
 * @since_tizen 6.0
 * @details This function is dedicated to diagnostic information subscribers. \n
 * It clears all the events added with diagnostics_subscribe_event().
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 */
API int diagnostics_unset_notification_cb(void);

/**
 * @platform
 * @brief Subscribes to an event sent by a diagnostics client.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege
 * @details This function is dedicated to diagnostic information subscribers. \n
 * It is permitted only to an app signed by platform level certificates.
 *
 * @param[in] event_name Event name \n
 *            Its length is limited to 255 characters + null terminator
 * @param[in] client_id The ID of the diagnostics client (event sender) \n
 *            Its length is limited to 255 characters + null terminator
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 * @retval #DIAGNOSTICS_ERROR_OUT_OF_MEMORY Out of memory
 */
API int diagnostics_subscribe_event(const char *event_name, const char *client_id);

/**
 * @platform
 * @brief Sets the callback for a system diagnostics data request.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege
 * @details This function is dedicated to diagnostic information providers. \n
 * It is permitted only to an app signed by platform level certificates.
 *
 * @param[in] callback A callback function to set
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid or the caller ID not set (required for system services only)
 * @retval #DIAGNOSTICS_ERROR_RESOURCE_BUSY Callback already registered
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 *
 * @code
 * #include <diagnostics.h>
 *
 * static void data_request_cb(diagnostics_data_h data, char **params, int params_size, diagnostics_ctx_h ctx, void *user_data)
 * {
 *     const char *buf = "Some data";
 *     size_t bytes_written;
 *
 *     diagnostics_data_write(data, buf, sizeof(buf)/sizeof(char), &bytes_written);
 *
 *     diagnostics_data_destroy(data);
 * }
 *
 * static GMainLoop *mainloop = NULL;
 *
 * int main(int argc, char **argv)
 * {
 *     diagnostics_data_h data;
 *
 *     // This is required for services only, not applications
 *     diagnostics_set_client_id("diagnostics-client");
 *
 *     diagnostics_set_data_request_cb(data_request_cb, NULL);
 *
 *     mainloop = g_main_loop_new(NULL, FALSE);
 *     g_main_loop_run(mainloop);
 * }
 *
 * @endcode
 */
API int diagnostics_set_data_request_cb(diagnostics_request_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the callback for a diagnostics data request.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege
 * @details This function is dedicated to diagnostic information providers. \n
 * It is permitted only to an app signed by platform level certificates.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 */
API int diagnostics_unset_data_request_cb(void);

/**
 * @platform
 * @brief Requests a system diagnostics client to dump given data.
 * @since_tizen 6.0
 * @privlevel platform
 * @privilege
 * @details This function is dedicated to diagnostic information subscribers. \n
 * It is permitted only to an app signed by platform level certificates. \n
 * @a data should be released with diagnostics_data_destroy().
 *
 * @param[in] client_id An ID of application or service to request
 * @param[in] params Array of parameters \n
 *                   Refer to specific diagnostics client's documentation for available parameters
 * @param[in] params_size Number of parameters
 * @param[out] data Dumpsys data handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 * @retval #DIAGNOSTICS_ERROR_OUT_OF_MEMORY Not enough memory to create data handle
 *
 * @code
 * #include <diagnostics.h>
 *
 * int main(int argc, char **argv)
 * {
 *     diagnostics_data_h data;
 *     const char *params[] = {"foo", "bar", "baz"};
 *     char buf[1000];
 *     size_t bytes_read;
 *
 *     diagnostics_request_client_data("diagnostics-client", params, sizeof(params)/sizeof(char*), &data);
 *
 *     while (true) {
 *         diagnostics_data_read(data, buf, sizeof(buf)/sizeof(char), -1, &bytes_read);
 *         if (bytes_read == 0) // Reached EOF
 *             break;
 *
 *         // Process the chunk of data
 *     }
 *
 *     diagnostics_data_destroy(data);
 * }
 *
 * @endcode
 */
API int diagnostics_request_client_data(const char *client_id, const char **params, int params_size, diagnostics_data_h *data);

/**
 * @platform
 * @brief Requests diagnostics context's provider to dump data.
 * @since_tizen 6.0
 * @privlevel platform
 * @privilege
 * @details This function is dedicated to diagnostic information subscribers. \n
 * It is permitted only to an app signed by platform level certificates. \n
 * @a data should be released with diagnostics_data_destroy(). \n
 * The difference between this function and diagnostics_request_client_data() is that
 * this function sends back the whole context to the context's provider to help it find the data specific to the generated event.
 *
 * @param[in] ctx Diagnostics context handle
 * @param[in] params Array of parameters \n
 *                   Refer to specific diagnostics client's documentation for available parameters
 * @param[in] params_size Number of parameters
 * @param[out] data Diagnostics data handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 * @retval #DIAGNOSTICS_ERROR_OUT_OF_MEMORY Not enough memory to create data handle
 *
 * @code
 *
 * void notification_handler(diagnostics_ctx_h ctx, void *user_data)
 * {
 *     diagnostics_data_h data;
 *     const char *params[] = {"foo", "bar", "baz"};
 *
 *     // Request more data from the client that the event came from
 *     diagnostics_get_data(ctx, params, sizeof(params)/sizeof(char*), &data);
 *
 *     // Read data
 *
 *     diagnostics_data_destroy(data);
 *     diagnostics_destroy(ctx);
 * }
 *
 * @endcode
 */
API int diagnostics_get_data(diagnostics_ctx_h ctx, const char **params, int params_size, diagnostics_data_h *data);

/**
 * @brief Gets a file descriptor for custom diagnostics data handling.
 * @since_tizen 6.5
 * @details This function provides a file descriptor to read/write diagnostic data in a custom way
 * instead of using our diagnostics_data_read() and diagnostics_data_write() functions. \n
 * Moreover, keep in mind that reading is allowed for diagnostic data subscribers and writing is allowed for providers.
 * @param[in] data Diagnostics data handle
 * @param[in,out] fd A file descriptor related to diagnostics data
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 */
API int diagnostics_data_get_fd(diagnostics_data_h data, int *fd);

/**
 * @brief Perform a single iteration of reading diagnostics data.
 * @since_tizen 6.0
 * @details This function is dedicated to diagnostic information subscribers. \n
 * It is intended for use in loop until EOF is reached. EOF is when @a bytes_read == 0 and function returns #DIAGNOSTICS_ERROR_NONE.
 *
 * @param[in] data Diagnostics data handle
 * @param[in,out] buf Buffer to store read data \n
 *                    Provided buffer must be large enough to contain a @a count number of bytes
 * @param[in] count Number of bytes to read
 * @param[in] timeout_ms Timeout [ms] for reading requested number of bytes (timeout_ms <= 0 means to wait forever)
 * @param[out] bytes_read Real number of bytes read
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 * @retval #DIAGNOSTICS_ERROR_TIMED_OUT Timeout occurred
 * @retval #DIAGNOSTICS_ERROR_TRY_AGAIN Try again
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred while trying to read data, result is unspecified and *bytes_read is not updated
 */
API int diagnostics_data_read(diagnostics_data_h data, void *buf, size_t count, int timeout_ms, size_t *bytes_read);

/**
 * @brief Perform a single iteration of writing diagnostics data.
 * @since_tizen 6.5
 * @details This function is dedicated to diagnostic information providers. \n
 * Data is being written to the special file descriptor and then copied to the diagnostic information subscriber's endpoint.
 *
 * @param[in] data Diagnostics data handle
 * @param[in] buf Buffer with data to write \n
 *                Provided buffer must be of size not less than a @a count number of bytes
 * @param[in] count Number of bytes to write
 * @param[out] bytes_written Real number of bytes written
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 * @retval #DIAGNOSTICS_ERROR_TRY_AGAIN Try again
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred while trying to write data, result is unspecified and *bytes_written is not updated
 */
API int diagnostics_data_write(diagnostics_data_h data, const void *buf, size_t count, size_t *bytes_written);

/**
 * @platform
 * @brief Sends a system diagnostics event to a previously specified client.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege
 * @details This function is dedicated to diagnostic information providers. \n
 * It is permitted only to an app signed by platform level certificates.
 *
 * @param[in] event_name Diagnostics event name
 * @param[in] event_data Event data or NULL.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid or the caller ID not set (required for system services only)
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 *
 * @code
 *
 * int main(int argc, char *argv[])
 * {
 *     bundle *event_data = bundle_create();
 *
 *     // This is required for services only, not applications
 *     diagnostics_set_client_id("diagnostics-client");
 *
 *     bundle_add_str(event_data, "somekey", "somevalue");
 *     diagnostics_send_event("ConnectionBroken", event_data);
 *
 *     bundle_free(event_data);
 * }
 *
 * @endcode
 */
API int diagnostics_send_event(const char *event_name, bundle *event_data);

/**
 * @brief Gets a system diagnostics client's ID (event sender).
 * @since_tizen 6.0
 * @details This function is dedicated to diagnostic information subscribers. \n
 * @a client_id should be released with free().
 *
 * @param[in] ctx Diagnostics context handle
 * @param[out] client_id ID of the diagnostics client
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 *
 * @code
 *
 * void notification_handler(diagnostics_ctx_h ctx, void *user_data)
 * {
 *     char *client_id;
 *
 *     diagnostics_get_client_id(ctx, &client_id);
 *
 *     // Process diagnostics event
 *
 *     free(client_id);
 *     diagnostics_destroy(ctx);
 * }
 *
 * @endcode
 */
API int diagnostics_get_client_id(diagnostics_ctx_h ctx, char **client_id);

/**
 * @brief Gets a copy of system diagnostics client's event name.
 * @since_tizen 6.5
 * @details This function is dedicated to diagnostic information subscribers. \n
 * @a event_name should be released with free().
 *
 * @param[in] ctx Diagnostics context handle
 * @param[out] event_name Name of the diagnostics event
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 *
 * @code
 *
 * void notification_handler(diagnostics_ctx_h ctx, void *user_data)
 * {
 *     char *event_name;
 *
 *     diagnostics_get_event_name(ctx, &event_name);
 *
 *     // Process diagnostics event
 *
 *     free(event_name);
 *     diagnostics_destroy(ctx);
 * }
 *
 * @endcode
 */
API int diagnostics_get_event_name(diagnostics_ctx_h ctx, char **event_name);

/**
 * @brief Gets a copy of a system diagnostics event's attached data.
 * @since_tizen 6.5
 * @details This function is dedicated to diagnostic information subscribers. \n
 * @a event_data should be released with bundle_free().
 *
 * @param[in] ctx Diagnostics context handle
 * @param[out] event_data Data that came with the diagnostics event (may be NULL when no data has been provided)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Diagnostics feature is missing or the function has been called by application, not system service
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 *
 * @code
 *
 * void notification_handler(diagnostics_ctx_h ctx, void *user_data)
 * {
 *     bundle *event_data;
 *
 *     diagnostics_get_event_data(ctx, &event_data);
 *
 *     // Process diagnostics event
 *
 *     bundle_free(event_data);
 *     diagnostics_destroy(ctx);
 * }
 *
 * @endcode
 */
API int diagnostics_get_event_data(diagnostics_ctx_h ctx, bundle **event_data);

/**
 * @brief Sets the system diagnostics client ID for further calls.
 * @since_tizen 6.5
 * @details This function is dedicated to diagnostic information providers. \n
 * As services do not have any name associated with them, ID must be set explicitly with this function before calling any of the following functions: \n
 * diagnostics_set_data_request_cb() \n
 * diagnostics_send_event() \n
 * diagnostics_request_bugreport() \n
 * For applications, client ID is set to the application ID and cannot be changed.
 *
 * @param[in] client_id Diagnostics client ID to set.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 * @retval #DIAGNOSTICS_ERROR_RESOURCE_BUSY Client ID has already been set, not possible to change
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 */
API int diagnostics_set_client_id(const char *client_id);

/**
 * @platform
 * @brief Requests the creation of a bugreport, either systemwide or livedump a specific process.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bugreport.admin
 * @details This function is dedicated to diagnostic information subscribers. \n
 * To get bugreport's content, user must subscribe to 'BugreportCreated' diagnostics event, which is sent by crash-service after creating the report.
 *
 * @param[in] pid ID of a process that should be livedumped. When @a pid <= 0, system-wide bugreport is created.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid or the caller ID not set (required for system services only)
 * @retval #DIAGNOSTICS_ERROR_IO_ERROR Internal error occurred
 */
API int diagnostics_request_bugreport(int pid);

/**
 * @brief Frees the resources of a system diagnostics data handle.
 * @details This function must be used at the end of the handle's lifetime to avoid a memory leak.
 * @since_tizen 6.0
 *
 * @param[in] data Diagnostics data handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 */
API int diagnostics_data_destroy(diagnostics_data_h data);

/**
 * @brief Frees the resources of a system diagnostics context handle.
 * @details This function must be used at the end of the handle's lifetime, in the notification handler, to avoid a memory leak.
 * @since_tizen 6.0
 *
 * @param[in] ctx Diagnostics context handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #DIAGNOSTICS_ERROR_NONE Success
 * @retval #DIAGNOSTICS_ERROR_NOT_SUPPORTED Not supported
 * @retval #DIAGNOSTICS_ERROR_INVALID_PARAMETER Provided parameter is invalid
 */
API int diagnostics_destroy(diagnostics_ctx_h ctx);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_DIAGNOSTICS_H__ */
