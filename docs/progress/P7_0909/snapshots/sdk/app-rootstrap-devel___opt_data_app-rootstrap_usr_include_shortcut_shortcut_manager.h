/*
 * Copyright (c) 2011 - 2017 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __SHORTCUT_MANAGER_H__
#define __SHORTCUT_MANAGER_H__

#include <tizen.h>
#include <shortcut_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file shortcut_manager.h
 * @brief This file declares the API of the libshortcut library.
 */

/**
 * @addtogroup SHORTCUT_MODULE
 * @{
 */


/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for shortcut types.
 * @details Basically, two types of shortcuts are defined.
 *          Every homescreen developer should support these types of shortcuts.
 *          Or return, a proper errno to figure out why the application failed to add a shortcut.
 *          #LAUNCH_BY_APP is used for adding a package itself as a shortcut.
 *          #LAUNCH_BY_URI is used for adding a shortcut for "uri" data.
 * @since_tizen 2.3
 */
typedef enum _shortcut_type {
	LAUNCH_BY_APP = 0x00000000, /**< Launch the application itself */
	LAUNCH_BY_URI = 0x00000001, /**< Launch the application with the given data(URI) */
} shortcut_type;

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for sizes of shortcut widget.
 * @since_tizen 2.4
 */
typedef enum shortcut_widget_size {
	WIDGET_SIZE_DEFAULT = 0x10000000, /**< Type mask for the normal mode widget , don't use this value for specific size.*/
	WIDGET_SIZE_1x1 = 0x10010000, /**< 1x1 */
	WIDGET_SIZE_2x1 = 0x10020000, /**< 2x1 */
	WIDGET_SIZE_2x2 = 0x10040000, /**< 2x2 */
	WIDGET_SIZE_4x1 = 0x10080000, /**< 4x1 */
	WIDGET_SIZE_4x2 = 0x10100000, /**< 4x2 */
	WIDGET_SIZE_4x3 = 0x10200000, /**< 4x3 */
	WIDGET_SIZE_4x4 = 0x10400000, /**< 4x4 */
	WIDGET_SIZE_4x5 = 0x11000000, /**< 4x5 */
	WIDGET_SIZE_4x6 = 0x12000000, /**< 4x6 */
	WIDGET_SIZE_EASY_DEFAULT = 0x30000000, /**< Type mask for the easy mode widget, don't use this value for specific size. */
	WIDGET_SIZE_EASY_1x1 = 0x30010000, /**< Easy mode 1x1 */
	WIDGET_SIZE_EASY_3x1 = 0x30020000, /**< Easy mode 3x2 */
	WIDGET_SIZE_EASY_3x3 = 0x30040000, /**< Easy mode 3x3 */
} shortcut_widget_size_e;


/**
 * @deprecated Deprecated since 9.0
 * @brief Called to receive the result of shortcut_add_to_home().
 * @since_tizen 2.3
 * @param[in] ret The result value, it could be @c 0 if it succeeds to add a shortcut,
 *                otherwise it returns an errno
 * @param[in] user_data The callback data
 * @return int @c 0 if there is no error,
 *             otherwise errno
 * @see shortcut_add_to_home()
 */
typedef int (*result_cb)(int ret, void *user_data);


/**
 * @deprecated Deprecated since 9.0
 * @brief Adds a shortcut to home, asynchronously.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @remarks If a homescreen does not support this feature, you will get a proper error code.\n
 *          Application must check the return value of this function.\n
 *          Application must check the return status from the callback function.\n
 *          Application should set the callback function to get the result of this request.
 * @param[in] name The name of the created shortcut icon
 * @param[in] type The type of shortcuts
 * @param[in] uri The specific information for delivering to the viewer for creating a shortcut
 * @param[in] icon The absolute path of an icon file
 * @param[in] allow_duplicate @c 1 if it accepts the duplicated shortcut,
 *                            otherwise @c 0
 * @param[in] cb The address of the callback function that is called when the result comes back from the viewer
 * @param[in] data The callback data that is used in the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SHORTCUT_ERROR_NONE Successful
 * @retval #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @retval #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SHORTCUT_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SHORTCUT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SHORTCUT_ERROR_IO_ERROR I/O Error
 * @retval #SHORTCUT_ERROR_RESOURCE_BUSY Device or resource busy
 * @retval #SHORTCUT_ERROR_NO_SPACE There is no space to add a new shortcut
 * @retval #SHORTCUT_ERROR_EXIST Shortcut is already exist
 * @retval #SHORTCUT_ERROR_FAULT Unrecoverable error
 * @retval #SHORTCUT_ERROR_COMM Connection not established or communication problem
 * @pre You have to prepare the callback function.
 * @post You have to check the return status from the callback function which is passed by the argument.
 * @see result_cb()
 * @par Example
 * @code
#include <stdio.h>
#include <shortcut_manager.h>
static int _result_cb(int ret, int pid, void *data)
{
	if (ret < 0)
		dlog_print("Failed to add a shortcut: %s\n", perror(ret));

	dlog_print("Processed by the %d\n", pid);
	return 0;
}

static int app_create(void *data)
{
	char* data_path = app_get_data_path();
	int path_len = strlen(data_path)+10;
	char * path = malloc(path_len);
	memset(path, 0, path_len);
	strncat(path, data_path, path_len);
	strncat(path, "Friend.jpg", path_len);

	shortcut_add_to_home("With friends", LAUNCH_BY_URI, "gallery:0000-0000", path, 0, _result_cb, NULL);
	free(path);

	return 0;
}
 * @endcode
 */
int shortcut_add_to_home(const char *name, shortcut_type type, const char *uri, const char *icon, int allow_duplicate, result_cb cb, void *data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Adds a widget to home, asynchronously.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @remarks If a homescreen does not support this feature, you will get a proper error code.\n
 *          Application must check the return value of this function.\n
 *          Application must check the return status from the callback function.\n
 *          Application should set the callback function to get the result of this request.
 * @param[in] name The name of the created widget which will be shown when the widget is not prepared
 * @param[in] size The size of widget
 * @param[in] widget_id Widget ID
 * @param[in] icon The absolute path of an icon file which will be shown when the widget is not prepared
 * @param[in] period The Update period in seconds
 * @param[in] allow_duplicate @c 1 if it accepts the duplicated widget,
 *                            otherwise @c 0
 * @param[in] cb The address of the callback function that is called when the result comes back from the viewer
 * @param[in] data The callback data that is used in the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SHORTCUT_ERROR_NONE Successful
 * @retval #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @retval #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SHORTCUT_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SHORTCUT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SHORTCUT_ERROR_IO_ERROR I/O Error
 * @retval #SHORTCUT_ERROR_FAULT Unrecoverable error
 * @retval #SHORTCUT_ERROR_COMM Connection not established or communication problem
 * @pre You have to prepare the callback function.
 * @post You have to check the return status from the callback function which is passed by the argument.
 * @see result_cb()
 * @see shortcut_widget_size_e
 * @par Example
 * @code
#include <stdio.h>
#include <shortcut.h>
#include <storage.h>

#define TIZEN_PATH_MAX 1024
static int _result_cb(int ret, int pid, void *data)
{
	if (ret < 0)
		dlog_print("Failed to add a widget: %s\n", perror(ret));

	dlog_print("Processed by the %d\n", pid);

	return 0;
}

static int app_create(void *data)
{
	char *image_root = NULL;
	char image_path[TIZEN_PATH_MAX] = {0,};

	storage_get_directory(STORAGE_TYPE_INTERNAL, STORAGE_DIRECTORY_IMAGES, &image_root);
	snprintf(image_path, TIZEN_PATH_MAX, "%s/alter_icon.png", image_root);

	shortcut_add_to_home_widget("alter_name", WIDGET_SIZE_1x1, "org.tizen.testwidget",
					image_path, -1.0f, 0, _result_cb, NULL);

	return 0;
}
 * @endcode
 */
int shortcut_add_to_home_widget(const char *name, shortcut_widget_size_e size, const char *widget_id, const char *icon, double period, int allow_duplicate, result_cb cb, void *data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Removes a shortcut from home, asynchronously.
 * @details If the callback function registered for a widget, the shortcut deletion is possible.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @param[in] name The name of the created shortcut icon
 * @param[in] cb The address of the callback function that is called when the result comes back from the viewer
 * @param[in] user_data The callback data that is used in the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #SHORTCUT_ERROR_NONE Successful
 * @retval #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @retval #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SHORTCUT_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SHORTCUT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SHORTCUT_ERROR_IO_ERROR I/O Error
 * @retval #SHORTCUT_ERROR_COMM Connection not established or communication problem
 * @pre You have to prepare the callback function.
 * @post You have to check the return status from the callback function which is passed by the argument.
 * @see result_cb()
 * @par Example
 * @code
#include <shortcut_manager.h>

int _result_cb(int ret, void *data)
{
	if (ret < 0)
		dlog_print("Failed to remove a shortcut: %d\n", ret);

	return 0;
}

{
	int result;

	result = shortcut_remove_from_home("shortcut_name", _result_cb, NULL);
	if (result != SHORTCUT_ERROR_NONE) {
		dlog_print("Failed to remove a shortcut: %d\n", result);
		return result;
	}
}

 * @endcode
 */
int shortcut_remove_from_home(const char *name, result_cb cb, void *user_data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Called to receive the result of shortcut_get_list().
 * @since_tizen 2.4
 * @param[in] package_name The name of package
 * @param[in] icon The absolute path of an icon file for this shortcut
 * @param[in] name The name of the created shortcut icon
 * @param[in] extra_key The user data. A property of shortcut element in manifest file
 * @param[in] extra_data The user data. A property of shortcut element in manifest file
 * @param[in] user_data The callback user data
 * @return SHORTCUT_ERROR_NONE to continue with the next iteration of the loop, other error values to break out of the loop
 * @see shortcut_get_list()
 */
typedef int (*shortcut_list_cb)(const char *package_name, const char *icon, const char *name, const char *extra_key, const char *extra_data, void *user_data);


/**
 * @deprecated Deprecated since 9.0
 * @brief Gets the preset list of shortcut template from the installed package, synchronously.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @remarks If a homescreen does not support this feature, you will get a proper error code.\n
 *          Application must check the return value of this function.\n
 *          Application must check the return status from the callback function.\n
 *          Application should set the callback function to get the result of this request.
 * @param[in] package_name The package name
 * @param[in] list_cb The callback function to get the shortcut item information
 * @param[in] data The callback data that is used in the callback function
 * @return @c N Number of items (call count of the callback function),
 *         otherwise a negative error value
 * @retval #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @retval #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SHORTCUT_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SHORTCUT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SHORTCUT_ERROR_IO_ERROR I/O Error
 * @retval #SHORTCUT_ERROR_FAULT Unrecoverable error
 * @retval #SHORTCUT_ERROR_COMM Connection not established or communication problem
 * @pre You have to prepare the callback function.
 * @post You have to check the return status from the callback function which is passed by the argument.
 *
 */
int shortcut_get_list(const char *package_name, shortcut_list_cb list_cb, void *data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Called to the add_to_home request.
 * @details The homescreen should define a callback as this type and implement the service code
 *          for adding a new application shortcut.
 * @since_tizen 2.4
 * @param[in] package_name The name of package
 * @param[in] name The name of the created shortcut icon
 * @param[in] type One of the three defined types
 * @param[in] content_info The specific information for creating a new shortcut
 * @param[in] icon The absolute path of an icon file for this shortcut
 * @param[in] pid The process ID of who request add_to_home
 * @param[in] period The Update period in seconds
 * @param[in] allow_duplicate @c 1 if the shortcut can be duplicated,
 *                            otherwise a shortcut should exist only once
 * @param[in] user_data The callback data
 * @return The result of handling a shortcut creation request\n
 *         This returns @c 0 if the add_to_home request is handled successfully,
 *         otherwise it returns a proper errno
 * @see shortcut_set_request_cb()
 */
typedef int (*shortcut_request_cb)(const char *package_name, const char *name, int type, const char *content_info, const char *icon, int pid, double period, int allow_duplicate, void *user_data);


/**
 * @deprecated Deprecated since 9.0
 * @brief Called to the shortcut_remove_from_home() request.
 * @since_tizen 3.0
 * @param[in] package_name The name of package
 * @param[in] name The name of the created shortcut icon
 * @param[in] sender_pid The process ID of who request shortcut_remove_from_home()
 * @param[in] user_data  The user data passed from the callback register function
 * @return The result of handling a shortcut remove request\n
 *             This returns @c 0 if the remove_from_home request is handled successfully,
 *             otherwise it returns a proper errno.
 * @see shortcut_set_remove_cb()
 */
typedef int (*shortcut_remove_cb)(const char *package_name, const char *name, int sender_pid, void *user_data);


/**
 * @deprecated Deprecated since 9.0
 * @brief Sets a callback function to listen requests from applications.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @remarks Should be used in the homescreen.\n
 *          Should check the return value of this function.
 *          Prospective Clients: Homescreen.
 * @param[in] request_cb The callback function pointer that is invoked when add_to_home is requested
 * @param[in] data The callback data to deliver to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SHORTCUT_ERROR_NONE Successful
 * @retval #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @retval #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SHORTCUT_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SHORTCUT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SHORTCUT_ERROR_IO_ERROR I/O Error
 * @retval #SHORTCUT_ERROR_COMM Connection not established or communication problem
 * @pre You have to prepare a callback function.
 * @post If a request is sent from the application, the registered callback will be invoked.
 * @see shortcut_request_cb()
 * @see shortcut_error_e
 */
int shortcut_set_request_cb(shortcut_request_cb request_cb, void *data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0
 * @brief Unsets a callback for the shortcut request.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @remarks The specific error code can be obtained using the gat_last_result() method. Error codes are described in Exception section.
 * @return None
 * @exception #SHORTCUT_ERROR_NONE Successful
 * @exception #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @exception #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @see shortcut_set_request_cb()
 * @see get_last_result()
 */
void shortcut_unset_request_cb(void) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Sets the callback function to listen the remove requests from applications.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @remarks Should be used in the homescreen.\n
 *          Should check the return value of this function.
 *          Prospective Clients: Homescreen.
 * @param[in] remove_cb The callback function pointer that is invoked when remove_from_home is requested
 * @param[in] data The callback data to deliver to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #SHORTCUT_ERROR_NONE Successful
 * @retval #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @retval #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SHORTCUT_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #SHORTCUT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SHORTCUT_ERROR_IO_ERROR I/O Error
 * @retval #SHORTCUT_ERROR_COMM Connection not established or communication problem
 * @pre You have to prepare a callback function.
 * @post If a request is sent from the application, the registered callback will be invoked.
 * @see shortcut_remove_cb()
 * @see shortcut_error_e
 */
int shortcut_set_remove_cb(shortcut_remove_cb remove_cb, void *data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0
 * @brief Unsets a callback for the shortcut remove.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/shortcut
 * @remarks The specific error code can be obtained using the gat_last_result() method. Error codes are described in Exception section.
 * @return None
 * @exception #SHORTCUT_ERROR_NONE Successful
 * @exception #SHORTCUT_ERROR_NOT_SUPPORTED Not supported
 * @exception #SHORTCUT_ERROR_PERMISSION_DENIED Permission denied
 * @see shortcut_set_remove_cb()
 * @see get_last_result()
 */
void shortcut_unset_remove_cb(void) TIZEN_DEPRECATED_API;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
