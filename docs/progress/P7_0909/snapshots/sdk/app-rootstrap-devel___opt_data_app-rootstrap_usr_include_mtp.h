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

#ifndef __TIZEN_CAPI_NETWORK_MTP_H__
#define __TIZEN_CAPI_NETWORK_MTP_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file mtp.h
 * @brief This file contains the MTP API
 */

/**
 * @addtogroup CAPI_NETWORK_MTP_MANAGER_MODULE
 * @{
 */

/**
 * @brief The handle to the mtp device.
 * @since_tizen 3.0
 */
typedef int mtp_device_h;

/**
 * @brief The handle to the mtp storage.
 * @since_tizen 3.0
 */
typedef int mtp_storage_h;

/**
 * @brief The handle to the mtp object.
 * @since_tizen 3.0
 */
typedef int mtp_object_h;

/**
 * @brief Error codes reported by the MTP API.
 * @since_tizen 3.0
 */
typedef enum {
	MTP_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	MTP_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
	MTP_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	MTP_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	MTP_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	MTP_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	MTP_ERROR_COMM_ERROR = TIZEN_ERROR_MTP | 0x01, /**< MTP communication error */
	MTP_ERROR_CONTROLLER = TIZEN_ERROR_MTP | 0x02, /**< MTP controller error */
	MTP_ERROR_NO_DEVICE = TIZEN_ERROR_MTP | 0x03, /**< MTP have not any device */
	MTP_ERROR_NOT_INITIALIZED = TIZEN_ERROR_MTP | 0x04, /**< MTP is not initialized */
	MTP_ERROR_NOT_ACTIVATED = TIZEN_ERROR_MTP | 0x05, /**< MTP is not activated */
	MTP_ERROR_NOT_COMM_INITIALIZED = TIZEN_ERROR_MTP | 0x06, /**< MTP communication is not initialized */
	MTP_ERROR_PLUGIN_FAIL = TIZEN_ERROR_MTP | 0x07, /**< MTP Plugin failed */
} mtp_error_e;

/**
 * @brief Enumerations for MTP Storage type.
 * @since_tizen 3.0
 */
typedef enum {
	MTP_STORAGE_TYPE_UNDEFINED, /**< Storage type is undefined */
	MTP_STORAGE_TYPE_FIXED_ROM, /**< Storage type is fixed ROM */
	MTP_STORAGE_TYPE_REMOVABLE_ROM, /**< Storage type is removable ROM */
	MTP_STORAGE_TYPE_FIXED_RAM, /**< Storage type is fixed RAM */
	MTP_STORAGE_TYPE_REMOVABLE_RAM /**< Storage type is Removable RAM */
} mtp_storage_type_e;

/**
 * @brief Enumerations for MTP file type.
 * @since_tizen 3.0
 */
typedef enum {
	MTP_FILETYPE_FOLDER, /**< FOLDER file type */
	MTP_FILETYPE_WAV, /**< WAV file type */
	MTP_FILETYPE_MP3, /**< MP3 file type */
	MTP_FILETYPE_WMA, /**< WMA file type */
	MTP_FILETYPE_OGG, /**< OGG file type */
	MTP_FILETYPE_AUDIBLE, /**< AUDIBLE file type */
	MTP_FILETYPE_MP4, /**< MP4 file type */
	MTP_FILETYPE_UNDEF_AUDIO, /**< UNDEF_AUDIO file type */
	MTP_FILETYPE_WMV, /**< WMV file type */
	MTP_FILETYPE_AVI, /**< AVI file type */
	MTP_FILETYPE_MPEG, /**< MPEG file type */
	MTP_FILETYPE_ASF, /**< ASF file type */
	MTP_FILETYPE_QT, /**< QT file type */
	MTP_FILETYPE_UNDEF_VIDEO, /**< UNDEF_VIDEO file type */
	MTP_FILETYPE_JPEG, /**< JPEG file type */
	MTP_FILETYPE_JFIF, /**< JFIF file type */
	MTP_FILETYPE_TIFF, /**< TIFF file type */
	MTP_FILETYPE_BMP, /**< BMP file type */
	MTP_FILETYPE_GIF, /**< GIF file type */
	MTP_FILETYPE_PICT, /**< PICT file type */
	MTP_FILETYPE_PNG, /**< PNG file type */
	MTP_FILETYPE_VCALENDAR1, /**< VCALENDAR1 file type */
	MTP_FILETYPE_VCALENDAR2, /**< VCALENDAR2 file type */
	MTP_FILETYPE_VCARD2, /**< VCARD2 file type */
	MTP_FILETYPE_VCARD3, /**< VCARD3 file type */
	MTP_FILETYPE_WINDOWSIMAGEFORMAT, /**< WINDOWSIMAGEFORMAT file type */
	MTP_FILETYPE_WINEXEC, /**< WINEXEC file type */
	MTP_FILETYPE_TEXT, /**< TEXT file type */
	MTP_FILETYPE_HTML, /**< HTML file type */
	MTP_FILETYPE_FIRMWARE, /**< FIRMWARE file type */
	MTP_FILETYPE_AAC, /**< AAC file type */
	MTP_FILETYPE_MEDIACARD, /**< MEDIACARD file type */
	MTP_FILETYPE_FLAC, /**< FLAC file type */
	MTP_FILETYPE_MP2, /**< MP2 file type */
	MTP_FILETYPE_M4A, /**< M4A file type */
	MTP_FILETYPE_DOC, /**< DOC file type */
	MTP_FILETYPE_XML, /**< XML file type */
	MTP_FILETYPE_XLS, /**< XLS file type */
	MTP_FILETYPE_PPT, /**< PPT file type */
	MTP_FILETYPE_MHT, /**< MHT file type */
	MTP_FILETYPE_JP2, /**< JP2 file type */
	MTP_FILETYPE_JPX, /**< JPX file type */
	MTP_FILETYPE_ALBUM, /**< ALBUM file type */
	MTP_FILETYPE_PLAYLIST, /**< PLAYLIST file type */
	MTP_FILETYPE_UNKNOWN, /**< Unknown file type */

	MTP_FILETYPE_ALL, /**< Helper enum value for all file types */
	MTP_FILETYPE_ALL_IMAGE, /**< Helper enum value for all image files types */
	MTP_FILETYPE_HEIF /**< HEIF file type (Since 6.5) */
} mtp_filetype_e;

/**
 * @brief Enumerations for MTP event type.
 * @since_tizen 3.0
 */
typedef enum {
	MTP_EVENT_UNKNOWN, /**< Unknown event type */
	MTP_EVENT_STORAGE_ADDED, /**< Storage is added */
	MTP_EVENT_STORAGE_REMOVED, /**< Storage is removed */
	MTP_EVENT_OBJECT_ADDED, /**< Object is added */
	MTP_EVENT_OBJECT_REMOVED, /**< Object is removed */
	MTP_EVENT_DEVICE_ADDED, /**< Device is added */
	MTP_EVENT_DEVICE_REMOVED, /**< Device is removed */
	MTP_EVENT_TURNED_OFF /**< MTP is turned off */
} mtp_event_e;

/**
 * @brief Called when mtp event is occured.
 * @since_tizen 3.0
 * @remarks Depending on the type of event, the meaning of event parameter is different.
 * - If event is device event, then event_parameter type is #mtp_device_h.
 * - If event is storage event, then event_parameter type is #mtp_storage_h.
 * - If event is object event, then event_parameter type is #mtp_object_h.
 * - If event is #MTP_EVENT_TURNED_OFF, then event_parameter is 0.
 *
 * @param[in] event The event
 * @param[in] event_parameter The event parameter
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see mtp_add_mtp_event_cb()
 * @see mtp_remove_mtp_event_cb()
 */
typedef void (*mtp_event_cb)(mtp_event_e event, int event_parameter, void *user_data);

/**
 * @brief Initializes for using MTP.
 * @details This function sets up the necessary environment for utilizing the Media Transfer Protocol (MTP).
 *          It must be called before performing any other MTP operations. By invoking this API, you enable
 *          your application to communicate with MTP devices such as digital cameras, smartphones, and tablets.
 * @since_tizen 3.0
 * @remarks This function must be called before proceeding any other mtp functions.
 * @remarks http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
 * http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage. \n
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 *
 * @see mtp_deinitialize()
 */
int mtp_initialize(void);

/**
 * @brief Gets the mtp devices.
 * @since_tizen 3.0
 *
 * @remarks The @a mtp_devices should be released using free().
 * @details This function retrieves the list of available MTP (Media Transfer Protocol) devices connected to
 *          your system. By calling this API, you can explore the variety of MTP-enabled gadgets such as
 *          digital cameras, smartphones, and tablets accessible through your application.
 *
 * @param[out] mtp_devices The MTP device list
 * @param[out] device_count Length of device list
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 *
 * @see mtp_initialize()
 */
int mtp_get_devices(mtp_device_h **mtp_devices, int *device_count);

/**
 * @brief Gets mtp storages from the given device.
 * @details This function fetches the list of available MTP (Media Transfer Protocol) storages present in
 *          a specified MTP device. By invoking this API, you can explore the various storage options within
 *          the selected gadget, enabling seamless data transfer between your application and the device's
 *          internal or external storage compartments.
 *
 * @since_tizen 3.0
 * @remarks The @a mtp_storages should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[out] mtp_storages Current mtp storage list
 * @param[out] storage_count Length of storage list
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 *
 * @see mtp_get_devices()
 */
int mtp_get_storages(mtp_device_h mtp_device, mtp_storage_h **mtp_storages, int* storage_count);

/**
 * @brief Gets object handles from the given device and storage.
 * @details This function retrieves the list of object handles associated with a specified MTP device and storage.
 *          By specifying the desired file type and parent object handle, you can navigate through the hierarchical
 *          structure of objects within the chosen storage compartment.
 *
 * @since_tizen 3.0
 * @remarks The @a object_handles should be freed using free().
 * @remarks If the @a parent is 0, it means "root folder" of mtp storage.
 *
 * @param[in] mtp_device The MTP device
 * @param[in] mtp_storage The MTP storage
 * @param[in] file_type The file type what you want
 * @param[in] parent The parent object handle
 * @param[out] object_handles The object handle list
 * @param[out] object_count Length of object handle list
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 *
 * @see mtp_get_devices()
 * @see mtp_get_storages()
 */
int mtp_get_object_handles(mtp_device_h mtp_device, mtp_storage_h mtp_storage, mtp_filetype_e file_type,
	mtp_object_h parent, mtp_object_h **object_handles, int* object_count);

/**
 * @brief Gets object for a given path from object handle.
 * @details This function extracts an object from a specified MTP device based on its object handle and transfers
 *          it to a destination path on the host storage. By providing the MTP device, object handle, and destination
 *          path as inputs, you can seamlessly move files and directories between the MTP device's internal or external
 *          storage and your application's local storage.
 * @since_tizen 3.0
 * @remarks The @a dest_path is host storage path.
 * @remarks http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
 * http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage. \n
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[in] dest_path The dest path
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_IO_ERROR I/O error
 *
 * @see mtp_get_devices()
 * @see mtp_get_object_handles()
 */
int mtp_get_object(mtp_device_h mtp_device, mtp_object_h object_handle, char *dest_path);

/**
 * @brief Gets thumbnail from the given object handle.
 * @details This function extracts a thumbnail image from a specified object handle in a MTP device and saves
 *          it to a designated location on the host storage. By providing the MTP device, object handle, and
 *          destination path as inputs, you can easily obtain thumbnails of images and videos stored within
 *          the MTP device's internal or external storage.
 * @since_tizen 3.0
 * @remarks The @a dest_path is host storage path.
 * @remarks http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
 * http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[in] dest_path The dest path
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_IO_ERROR I/O error
 *
 * @see mtp_get_devices()
 * @see mtp_get_object_handles()
 */
int mtp_get_thumbnail(mtp_device_h mtp_device, mtp_object_h object_handle, char *dest_path);

/**
 * @brief Adds a callback function for receiving MTP event.
 * @details This function registers a callback function that will be invoked whenever an MTP event occurs.
 *          By providing the callback function and optional user data as arguments, you can receive notifications
 *          regarding changes in device, storage, or object states, as well as the turning off of the MTP service.
 *
 * @since_tizen 3.0
 * @remarks You can register multiple callback. \n
 * If you don't want to receive the event, then using the mtp_remove_mtp_event_cb() function to unregister a callback.
 * @remarks If device state is changed, #MTP_EVENT_DEVICE_ADDED or #MTP_EVENT_DEVICE_REMOVED event is occur. \n
 * If storage state is changed, #MTP_EVENT_STORAGE_ADDED or #MTP_EVENT_STORAGE_REMOVED event is occur. \n
 * If object state is changed, #MTP_EVENT_OBJECT_ADDED or #MTP_EVENT_OBJECT_REMOVED event is occur. \n
 * If mtp service is turned off, #MTP_EVENT_TURNED_OFF event is occur. \n
 *
 * @param[in] event_cb The callback
 * @param[in] user_data The user data
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 *
 * @see mtp_remove_mtp_event_cb()
 */
int mtp_add_mtp_event_cb(mtp_event_cb event_cb, void *user_data);

/**
 * @brief Removes the callback function.
 * @details This function unregisters a previously added callback function for receiving MTP events.
 *          By providing the callback function as an argument, you can stop receiving notifications
 *          regarding changes in device, storage, or object states, as well as the turning off of the MTP service.
 *
 * @since_tizen 3.0
 *
 * @param[in] event_cb The callback
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 *
 * @see mtp_add_mtp_event_cb()
 */
int mtp_remove_mtp_event_cb(mtp_event_cb event_cb);

/**
 * @brief Deinitializes MTP operation.
 * @details This function terminates the usage of the Media Transfer Protocol (MTP) and frees up any associated resources.
 *          It is essential to call this API once you have completed all MTP operations and no longer require access to MTP devices
 *
 * @since_tizen 3.0
 * @remarks http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
 * http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage. \n
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @see mtp_initialize()
 */
int mtp_deinitialize(void);

/**
* @}
*/

/**
 * @addtogroup CAPI_NETWORK_MTP_DEVICEINFO_MODULE
 * @{
 */

/**
 * @brief Gets the manufacturer name of the device information.
 * @since_tizen 3.0
 * @remarks The @a manufacturer_name should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[out] manufacturer_name The manufacturer name of Device information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Memory Allocation failed
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_devices()
 */
int mtp_deviceinfo_get_manufacturer_name(mtp_device_h mtp_device, char **manufacturer_name);

/**
 * @brief Gets the model name of the device information.
 * @since_tizen 3.0
 * @remarks The @a model_name should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[out] model_name The model name of Device information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_devices()
 */
int mtp_deviceinfo_get_model_name(mtp_device_h mtp_device, char **model_name);

/**
 * @brief Gets the serial number of the device information.
 * @since_tizen 3.0
 * @remarks The @a serial_number should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[out] serial_number The serial number of Device information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_devices()
 */
int mtp_deviceinfo_get_serial_number(mtp_device_h mtp_device, char **serial_number);

/**
 * @brief Gets the device version of the device information.
 * @since_tizen 3.0
 * @remarks The @a device_version should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[out] device_version The device version of Device information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_devices()
 */
int mtp_deviceinfo_get_device_version(mtp_device_h mtp_device, char **device_version);

/**
* @}
*/

/**
 * @addtogroup CAPI_NETWORK_MTP_STORAGEINFO_MODULE
 * @{
 */

/**
 * @brief Gets the description of the storage information.
 * @since_tizen 3.0
 * @remarks The @a description should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[in] mtp_storage The MTP storage
 * @param[out] description The description of Storage information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_storages()
 */
int mtp_storageinfo_get_description(mtp_device_h mtp_device, mtp_storage_h mtp_storage, char **description);

/**
 * @brief Gets the free space of the storage information in bytes.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] mtp_storage The MTP storage
 * @param[out] free_space The free space of Storage information (bytes)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_storages()
 */
int mtp_storageinfo_get_free_space(mtp_device_h mtp_device, mtp_storage_h mtp_storage, unsigned long long *free_space);

/**
 * @brief Gets the max capacity of the storage information in bytes.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] mtp_storage The MTP storage
 * @param[out] max_capacity The max capacity of Storage information (bytes)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_storages()
 */
int mtp_storageinfo_get_max_capacity(mtp_device_h mtp_device, mtp_storage_h mtp_storage, unsigned long long *max_capacity);

/**
 * @brief Gets the storage type of the storage information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] mtp_storage The MTP storage
 * @param[out] storage_type The storage type of Storage information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_storages()
 */
int mtp_storageinfo_get_storage_type(mtp_device_h mtp_device, mtp_storage_h mtp_storage, mtp_storage_type_e *storage_type);

/**
 * @brief Gets the volume identifier of the storage information.
 * @since_tizen 3.0
 * @remarks The @a volume_identifier should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[in] mtp_storage The MTP storage
 * @param[out] volume_identifier The volume identifier of Storage information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 *
 * @see mtp_get_object_handles()
 */
int mtp_storageinfo_get_volume_identifier(mtp_device_h mtp_device, mtp_storage_h mtp_storage, char **volume_identifier);

/**
* @}
*/

/**
 * @addtogroup CAPI_NETWORK_MTP_OBJECTINFO_MODULE
 * @{
 */

/**
 * @brief Gets the filename of the object information.
 * @since_tizen 3.0
 * @remarks The @a file_name should be freed using free().
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] file_name The file name of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_file_name(mtp_device_h mtp_device, mtp_object_h object_handle, char **file_name);

/**
 * @brief Gets the keywords of the object information.
 * @since_tizen 3.0
 * @remarks The @a keywords should be freed using free().
 * @remarks The keywords are separated by comma.
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] keywords The keywords of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_keywords(mtp_device_h mtp_device, mtp_object_h object_handle, char **keywords);

/**
 * @brief Gets the association desc of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] asso_desc The association description of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_association_desc(mtp_device_h mtp_device, mtp_object_h object_handle, int *asso_desc);

/**
 * @brief Gets the association type of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] asso_type The association type of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_association_type(mtp_device_h mtp_device, mtp_object_h object_handle, int *asso_type);

/**
 * @brief Gets the size of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] size The size of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_size(mtp_device_h mtp_device, mtp_object_h object_handle, int *size);

/**
 * @brief Gets the parent object handle of the object information.
 * @since_tizen 3.0
 *
 * @remarks The @a parent_object_handle should not be released.
 * @remarks The @a parent_object_handle can be used until the MTP service stops.
 * @remarks The @a parent_object_handle is managed by the platform when the MTP service stop.
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] parent_object_handle The parent of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_parent_object_handle(mtp_device_h mtp_device, mtp_object_h object_handle, mtp_object_h *parent_object_handle);

/**
 * @brief Gets the mtp storage of the object information.
 * @since_tizen 3.0
 *
 * @remarks The @a mtp_storage should not be released.
 * @remarks The @a mtp_storage can be used until the MTP service stops.
 * @remarks The @a mtp_storage is managed by the platform when the MTP service stop.
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] mtp_storage The MTP storage of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_storage(mtp_device_h mtp_device, mtp_object_h object_handle, mtp_storage_h* mtp_storage);

/**
 * @brief Gets the object created time of the object information.
 * @since_tizen 3.0
 * @remarks When interpreted as an absolute time value, \n
 * @a date_created represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] date_created The object created time of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_date_created(mtp_device_h mtp_device, mtp_object_h object_handle, int *date_created);

/**
 * @brief Gets the object modified time of the object information.
 * @since_tizen 3.0
 * @remarks When interpreted as an absolute time value, \n
 * @a date_modified represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] date_modified The object modified time of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_date_modified(mtp_device_h mtp_device, mtp_object_h object_handle, int *date_modified);

/**
 * @brief Gets the file type of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] file_type The file type of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_file_type(mtp_device_h mtp_device, mtp_object_h object_handle, mtp_filetype_e *file_type);

/**
 * @brief Gets the image bit depth of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] depth The image bit depth of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_image_bit_depth(mtp_device_h mtp_device, mtp_object_h object_handle, int *depth);

/**
 * @brief Gets the image pixel width of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] width The image pixel width of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_image_pix_width(mtp_device_h mtp_device, mtp_object_h object_handle, int *width);

/**
 * @brief Gets the image pixel height of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] height The image pixel height of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_image_pix_height(mtp_device_h mtp_device, mtp_object_h object_handle, int *height);

/**
 * @brief Gets the thumbnail size of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] size The thumbnail size of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_thumbnail_size(mtp_device_h mtp_device, mtp_object_h object_handle, int *size);

/**
 * @brief Gets the thumbnail file type of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] file_type The file type of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_thumbnail_file_type(mtp_device_h mtp_device, mtp_object_h object_handle, mtp_filetype_e *file_type);

/**
 * @brief Gets the thumbnail pixel height of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] height The thumbnail pixel height of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_thumbnail_pix_height(mtp_device_h mtp_device, mtp_object_h object_handle, int *height);

/**
 * @brief Gets the thumbnail pixel width of the object information.
 * @since_tizen 3.0
 *
 * @param[in] mtp_device The MTP device
 * @param[in] object_handle The object handle
 * @param[out] width The thumbnail pixel width of Object information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MTP_ERROR_NONE Successful
 * @retval #MTP_ERROR_NOT_SUPPORTED MTP is not supported
 * @retval #MTP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MTP_ERROR_NOT_INITIALIZED MTP is not initialized
 * @retval #MTP_ERROR_NOT_ACTIVATED MTP is not activated
 * @retval #MTP_ERROR_NOT_COMM_INITIALIZED MTP communication is not initialized
 * @retval #MTP_ERROR_COMM_ERROR MTP communication error
 * @retval #MTP_ERROR_CONTROLLER MTP controller error
 * @retval #MTP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MTP_ERROR_NO_DEVICE MTP have not any device
 * @retval #MTP_ERROR_PLUGIN_FAIL Plugin failed
 *
 * @see mtp_get_object_handles()
 */
int mtp_objectinfo_get_thumbnail_pix_width(mtp_device_h mtp_device, mtp_object_h object_handle, int *width);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_CAPI_NETWORK_MTP_H__ */
