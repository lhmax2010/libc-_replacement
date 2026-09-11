/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_UPDATE_CONTROL_H__
#define __TIZEN_SYSTEM_UPDATE_CONTROL_H__

#include <time.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SYSTEM_UPDATE_CONTROL_MODULE
 * @{
 */


/**
 * @brief Enumeration for the update control error.
 * @since_tizen 5.0
 */
typedef enum {
	UPDATE_CONTROL_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	UPDATE_CONTROL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	UPDATE_CONTROL_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	UPDATE_CONTROL_ERROR_FILE_NO_SPACE_ON_DEVICE = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< No space left on device */
	UPDATE_CONTROL_ERROR_KEY_NOT_FOUND = TIZEN_ERROR_KEY_NOT_AVAILABLE, /**< Specified key is not available */
	UPDATE_CONTROL_ERROR_KEY_REJECTED = TIZEN_ERROR_KEY_REJECTED, /**< Key is not available */
	UPDATE_CONTROL_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	UPDATE_CONTROL_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	UPDATE_CONTROL_ERROR_CONNECTION_ABORTED = TIZEN_ERROR_CONNECTION_ABORTED, /**< Software caused connection abort */
	UPDATE_CONTROL_ERROR_CONNECTION_REFUSED = TIZEN_ERROR_CONNECTION_REFUSED, /**< Connection refused */
	UPDATE_CONTROL_ERROR_PROTOCOL_NOT_SUPPORTED = TIZEN_ERROR_PROTOCOL_NOT_SUPPORTED, /**< Protocol not supported */
	UPDATE_CONTROL_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Time out */
	UPDATE_CONTROL_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY, /**<Device or resource busy */
	UPDATE_CONTROL_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION, /**< Function not implemented */
	UPDATE_CONTROL_ERROR_INVALID_PACKAGE = TIZEN_ERROR_UPDATE_CONTROL | 0x01, /**< Invalid package */
	UPDATE_CONTROL_ERROR_INVALID_URI = TIZEN_ERROR_UPDATE_CONTROL | 0x02, /**< Invalid URI */
	UPDATE_CONTROL_ERROR_PACKAGE_NOT_SUPPORTED = TIZEN_ERROR_UPDATE_CONTROL | 0x03, /**< Package type not supported */
	UPDATE_CONTROL_ERROR_SYSTEM_ERROR = TIZEN_ERROR_UPDATE_CONTROL | 0x04, /**< System error */
	UPDATE_CONTROL_ERROR_RO_UPDATE_IN_PROGRESS = TIZEN_ERROR_UPDATE_CONTROL | 0x05, /**< RO update is in progress */
	UPDATE_CONTROL_ERROR_RO_UPDATE_NOT_COMPLETED = TIZEN_ERROR_UPDATE_CONTROL | 0x06, /**< RO update has not completed */
} update_control_error_e;


/**
 * @brief Enumeration for the update control properties.
 * @since_tizen 5.0
 */
typedef enum {
	UPDATE_CONTROL_PROPERTY_NEW_VERSION = 0, /**< New version */
	UPDATE_CONTROL_PROPERTY_PACKAGE_URI, /**< URI for update package */
	UPDATE_CONTROL_PROPERTY_RESULT, /**< Update result */
	UPDATE_CONTROL_PROPERTY_PACKAGE_SIZE, /**< Size of update package */
	UPDATE_CONTROL_PROPERTY_DESCRIPTION, /**< Update description */
	UPDATE_CONTROL_PROPERTY_UPDATE_AVAILABLE, /**< Updatability flag */
} update_control_property_e;



/**
 * @brief Callback type to be invoked after the RO update or the finish update process finishes.
 * @details The callback will be triggered when the operation triggered by update_control_do_ro_update_async() \n
 * or update_control_do_finish_update_async() finishes (does not have to be successful). Examine the \p result parameter \n
 * to get the result of the operation. Both RO update and finish update operations can have following results:
 * - #UPDATE_CONTROL_ERROR_NONE Successful
 * - #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * - #UPDATE_CONTROL_ERROR_INVALID_OPERATION Upgrade script returned an error
 * - #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * Finish update operation can additionally result in following:
 * - #UPDATE_CONTROL_ERROR_RO_UPDATE_IN_PROGRESS - RO update is still in progress
 * - #UPDATE_CONTROL_ERROR_RO_UPDATE_NOT_COMPLETED - RO update has not finished (and is not currently in progress), \n
 * most likely meaning the RO update has not been triggered yet
 *
 * @since_tizen 9.0
 * @remarks This callback will be invoked in the main event loop of the client.
 * @param[in] result Result of the operation that triggered the callback
 * @param[in] user_data User data given when setting the callback
 * @pre Register the callback using update_control_set_ro_update_cb() or update_control_set_finish_update_cb(). \n
 * The callback will be invoked only when the operation associated with the registering function finishes. \n
 * @see update_control_set_ro_update_cb()
 * @see update_control_set_finish_update_cb()
 * @see update_control_unset_ro_update_cb()
 * @see update_control_unset_finish_update_cb()
 * @see update_control_do_ro_update_async()
 * @see update_control_do_finish_update_async()
 */
typedef void (*update_control_cb) (const update_control_error_e result, const void *user_data);


/**
 * @brief Initializes the update controller.
 *
 * @since_tizen 5.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 * @see update_control_deinitialize()
 */
int update_control_initialize(void);


/**
 * @brief Deinitializes the update controller.
 *
 * @since_tizen 5.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 * @see update_control_initialize()
 */
int update_control_deinitialize(void);


/**
 * @brief Requests checking new version of firmware.
 *
 * @since_tizen 5.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_CONNECTION_REFUSED Connection refused
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 */
int update_control_check_new_version(void);


/**
 * @brief Requests downloading new version of firmware.
 *
 * @since_tizen 5.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_FILE_NO_SPACE_ON_DEVICE No space left on device
 * @retval #UPDATE_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UPDATE_CONTROL_ERROR_CONNECTION_REFUSED Connection refused
 * @retval #UPDATE_CONTROL_ERROR_CONNECTION_ABORTED Software caused connection abort
 * @retval #UPDATE_CONTROL_ERROR_PROTOCOL_NOT_SUPPORTED Protocol not supported
 * @retval #UPDATE_CONTROL_ERROR_INVALID_URI Invalid URI
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 */
int update_control_download_package(void);


/**
 * @platform
 * @brief Triggers upgrade script and tells it to perform RO update and finish update operations (full upgrade process).
 * @details This starts the script in the background and quickly returns. It doesn't check the return value of the script.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/updatecontrol.admin
 * @return @c 0 if RO update and finish were triggered successfully (doesn't guarantee upgrade script COMPLETED successfully),
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #UPDATE_CONTROL_ERROR_TIMED_OUT Time out
 * @retval #UPDATE_CONTROL_ERROR_INVALID_PACKAGE Invalid package
 * @retval #UPDATE_CONTROL_ERROR_PACKAGE_NOT_SUPPORTED Package type not supported
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 */
int update_control_do_update(void);


/**
 * @brief Sets a callback to trigger once RO update has completed.
 *
 * @since_tizen 9.0
 * @remarks If there already is a callback set using this function, calling it again will overwrite it.
 * @param[in] user_cb The callback to be triggered once the operation has completed
 * @param[in] user_data Data to be passed to the callback
 * @return @c 0 if callback was set, otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_INVALID_PARAMETER @a user_cb is null
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @post @a user_cb will be called after an operation launched by update_control_do_ro_update_async() finishes.
 * @see update_control_unset_ro_update_cb()
 * @see update_control_do_ro_update_async()
 */
int update_control_set_ro_update_cb(update_control_cb user_cb, const void* user_data);


/**
 * @brief Unsets the callback set by update_control_set_ro_update_cb().
 *
 * @since_tizen 9.0
 * @remarks Using this function after launching associated operation but before \n
 * receiving the callback will not cause any errors, but will make tracking the result of \n
 * the operation impossible.
 * @return @c 0 if callback was unset, otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION No callback to unset
 * @pre A callback has to be set using update_control_set_ro_update_cb() before calling \n
 * this function.
 * @see update_control_set_ro_update_cb()
 */
int update_control_unset_ro_update_cb(void);


/**
 * @platform
 * @brief Triggers upgrade script and tells it to perform @b ONLY RO update asynchronously.
 * @details This function does not perform the finishing of update process (among others rebooting). \n
 * To finish the update use update_control_do_finish_update_async(). If you wish to trigger both operations synchronously, \n
 * use update_control_do_update().
 *
 * @since_tizen 9.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/updatecontrol.admin
 * @return @c 0 if basic checks succeeded (does not guarantee the operation triggered completed successfully), \n
 * otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Callback was not set
 * @pre update_control_initialize() has to be called before calling this function.
 * @pre A callback has to be set using update_control_set_ro_update_cb() before calling \n
 * this function.
 * @post A callback set by update_control_set_ro_update_cb() will be invoked \n
 * after RO update finishes (does not guarantee the process was successful). \n
 * To check the result of the operation, examine the result code passed to the callback.
 * @see update_control_initialize()
 * @see update_control_set_ro_update_cb()
 */
int update_control_do_ro_update_async(void);


/**
 * @brief Sets a callback to trigger once finish update operation has completed.
 *
 * @since_tizen 9.0
 * @remarks If there already is a callback set using this function, calling it again will overwrite it.
 * @param[in] user_cb The callback to be triggered once the operation has completed
 * @param[in] user_data Data to be passed to the callback
 * @return @c 0 if callback was set, otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_INVALID_PARAMETER @a user_cb is null
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @post @a user_cb will be called after an operation launched by update_control_do_finish_update_async() finishes.
 * @see update_control_unset_finish_update_cb()
 * @see update_control_do_finish_update_async()
 */
int update_control_set_finish_update_cb(update_control_cb user_cb, const void* user_data);


/**
 * @brief Unsets the callback set by update_control_set_finish_update_cb().
 *
 * @since_tizen 9.0
 * @remarks Using this function after launching associated operation but before \n
 * receiving the callback will not cause any errors, but will make tracking the result of \n
 * the operation impossible.
 * @return @c 0 if callback was unset, otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION No callback to unset
 * @pre A callback has to be set using update_control_set_finish_update_cb() before calling \n
 * this function.
 * @see update_control_set_finish_update_cb()
 */
int update_control_unset_finish_update_cb(void);


/**
 * @platform
 * @brief Triggers upgrade script and tells it to perform @b ONLY the finish update process asynchronously.
 * @details This function does not perform the RO update process. \n
 * To trigger RO use update_control_do_ro_update_async(). If you wish to trigger both operations synchronously, \n
 * use update_control_do_update().
 *
 * @since_tizen 9.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/updatecontrol.admin
 * @remarks This function can be used to check whether the RO update process is still in progress.
 * @return @c 0 if basic checks succeeded (does not guarantee the operation triggered finished successfully), \n
 * otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Callback was not set
 * @pre update_control_initialize() has to be called before calling this function.
 * @pre A callback has to be set using update_control_set_finish_update_cb() before calling \n
 * this function.
 * @pre In order for the finish update process to complete successfully, this function has \n
 * to be called after the RO update process launched by update_control_do_ro_update_async() \n
 * completes @b SUCCESSFULLY. Calling this function before that will not return an error.
 * @post A callback set by update_control_set_finish_update_cb() will be invoked \n
 * after the finish update process finishes. To check the result of the operation, examine \n
 * the result code passed to the callback. If the process was successful, the callback \n
 * will not be invoked, because the system will be rebooting.
 * @see update_control_initialize()
 * @see update_control_set_finish_update_cb()
 * @see update_control_do_ro_update_async()
 */
int update_control_do_finish_update_async(void);


/**
 * @platform
 * @brief Makes reservation for update.
 * @details Makes update reservation at a specific time. At the given time, it automatically checks, downloads and updates to a new version. To cancel the reservation, call update_control_cancel_reservation().
 *
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/updatecontrol.admin
 * @remarks Only one reservation can be used at any given time. If a new reservation is made, the previous one will be removed.
 * @param[in] reservation_time The time to start update
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #UPDATE_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 */
int update_control_make_reservation(struct tm *reservation_time);


/**
 * @platform
 * @brief Cancels the update reservation.
 *
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/updatecontrol.admin
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 */
int update_control_cancel_reservation(void);


/**
 * @brief Gets the update property of the given key.
 *
 * @since_tizen 5.0
 * @remarks The @a value must be released using free()
 * @param[in] property The key of property to get
 * @param[out] value The value of given property
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #UPDATE_CONTROL_ERROR_NONE Successful
 * @retval #UPDATE_CONTROL_ERROR_NOT_SUPPORTED Not supported
 * @retval #UPDATE_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UPDATE_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UPDATE_CONTROL_ERROR_KEY_NOT_FOUND Specified property not found
 * @retval #UPDATE_CONTROL_ERROR_KEY_REJECTED Specified property not available
 * @retval #UPDATE_CONTROL_ERROR_SYSTEM_ERROR System error
 * @retval #UPDATE_CONTROL_ERROR_INVALID_OPERATION Function not implemented
 */
int update_control_get_property(update_control_property_e property, void **value);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_UPDATE_CONTROL_H__ */
