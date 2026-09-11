/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */


#ifndef __TIZEN_UIX_MMI_DATA_H__
#define __TIZEN_UIX_MMI_DATA_H__


#include <stdlib.h>


/**
* @file mmi-data.h
*/


/**
* @addtogroup CAPI_UIX_MMI_COMMON_MODULE
* @ingroup CAPI_UIX_MMI_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for MMI data types.
 * @details This enumeration defines the different data types supported by the MMI framework.
 * @since_tizen 9.0
 */
typedef enum {
    MMI_DATA_TYPE_BOOLEAN,       /**< Boolean data type */
    MMI_DATA_TYPE_INTEGER,       /**< Integer data type */
    MMI_DATA_TYPE_FLOAT,         /**< Float data type */
    MMI_DATA_TYPE_TEXT,          /**< Text data type */
    MMI_DATA_TYPE_JSON,          /**< JSON data type */
    MMI_DATA_TYPE_USER_IDENTIFICATION, /**< User identification data type */
    MMI_DATA_TYPE_AUDIO,         /**< Audio data type */
    MMI_DATA_TYPE_VIDEO,         /**< Video data type */
    MMI_DATA_TYPE_COORDINATE,    /**< Coordinate data type */
    MMI_DATA_TYPE_BOUNDING_BOX,  /**< Bounding box data type */
    MMI_DATA_TYPE_ARRAY,         /**< Array data type */
    MMI_DATA_TYPE_STRUCT,        /**< Struct data type */
    MMI_DATA_TYPE_ANY,           /**< Any data type */
} mmi_data_type_e;

/**
 * @brief Handle for MMI data.
 * @details This handle represents a data that can flow among nodes in MMI framework.
 * @since_tizen 9.0
 */
typedef struct mmi_data_s *mmi_data_h;

/**
 * @brief Creates a boolean data object.
 * @details This function creates a boolean data object with the specified value.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] value The boolean value to set in the data object.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data = NULL;
   mmi_data_create_bool(true, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_bool(bool value, mmi_data_h *data);

/**
 * @brief Creates an integer data object.
 * @details This function creates an integer data object with the specified value.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] value The integer value to set in the data object.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data = NULL;
   mmi_data_create_int(3, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_int(int value, mmi_data_h *data);

/**
 * @brief Creates a float data object.
 * @details This function creates a float data object with the specified value.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] value The float value to set in the data object.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data = NULL;
   mmi_data_create_float(3.14f, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_float(float value, mmi_data_h *data);

/**
 * @brief Creates a text data object.
 * @details This function creates a text data object with the specified value.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] value The text value to set in the data object.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   const char *value; // Indicates the text to be set in the data object.
   mmi_data_h data = NULL;
   mmi_data_create_text(value, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_text(const char *value, mmi_data_h *data);

/**
 * @brief Creates an audio data object.
 * @details This function creates an audio data object with the specified buffer and length.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] ptr The pointer to the audio buffer.
 * @param[in] len The length of the audio buffer.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   const void *ptr; // Indicates the address of the data to be stored in the data object.
   size_t len; // Indicates the length of the data to be stored in the data object.
   mmi_data_h data = NULL;
   mmi_data_create_audio(ptr, len, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_audio(const void *ptr, size_t len, mmi_data_h *data);

/**
 * @brief Creates a video data object.
 * @details This function creates a video data object with the specified buffer and length.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] ptr The pointer to the video buffer.
 * @param[in] len The length of the video buffer.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   const void *ptr; // Indicates the address of the data to be stored in the data object.
   size_t len; // Indicates the length of the data to be stored in the data object.
   mmi_data_h data = NULL;
   mmi_data_create_video(ptr, len, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_video(const void *ptr, size_t len, mmi_data_h *data);

/**
 * @brief Creates a user identification data object.
 * @details This function creates a user identification data object with the specified buffer and length.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] ptr The pointer to the user identification buffer.
 * @param[in] len The length of the user identification buffer.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   const void *ptr; // Indicates the address of the data to be stored in the data object.
   size_t len; // Indicates the length of the data to be stored in the data object.
   mmi_data_h data = NULL;
   mmi_data_create_user_identification(ptr, len, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_user_identification(const void *ptr, size_t len, mmi_data_h *data);

/**
 * @brief Creates a coordinate data object.
 * @details This function creates a coordinate data object with the specified x and y values.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] x The x-coordinate value.
 * @param[in] y The y-coordinate value.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data = NULL;
   mmi_data_create_coordinate(0, 0, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_coordinate(int x, int y, mmi_data_h *data);

/**
 * @brief Creates a bounding box data object.
 * @details This function creates a bounding box data object with the specified x, y, width, and height values.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] x The x-coordinate value.
 * @param[in] y The y-coordinate value.
 * @param[in] w The width value.
 * @param[in] h The height value.
 * @param[out] data A pointer to store the handle of the created data object.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data = NULL;
   mmi_data_create_bounding_box(0, 0, 10, 10, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_create_bounding_box(int x, int y, int w, int h, mmi_data_h *data);

/**
 * @brief Creates a new data array handle.
 * @details This function allocates memory for a new data array handle and initializes it.
 * @since_tizen 9.0
 * @remarks The @a array_handle should be released using mmi_data_destroy().
 *
 * @param[out] array_handle A pointer to the newly created data array handle.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h array_handle = NULL;
   mmi_data_create_array(&array_handle);
   ...
   mmi_data_destroy(array_handle);
 * @endcode
 */
int mmi_data_create_array(mmi_data_h *array_handle);

/**
 * @brief Adds an element to a data array.
 * @details This function adds a new element to the specified data array.
 * @since_tizen 9.0
 *
 * @param[in] array_handle The handle to the data array where the element will be added.
 * @param[in] element The handle to the element to be added to the array.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_data_add_array_element(mmi_data_h array_handle, mmi_data_h element);

/**
 * @brief Creates a new data structure handle.
 * @details This function allocates memory for a new data structure handle and initializes it.
 * @since_tizen 9.0
 * @remarks The @a struct_handle should be released using mmi_data_destroy().
 *
 * @param[out] struct_handle A pointer to the newly created data structure handle.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h struct_handle = NULL;
   mmi_data_create_struct(&struct_handle);
   ...
   mmi_data_destroy(struct_handle);
 * @endcode
 */
int mmi_data_create_struct(mmi_data_h *struct_handle);

/**
 * @brief Sets an element in a data structure.
 * @details This function sets the value of an element in the specified data structure.
 * @since_tizen 9.0
 *
 * @param[in] struct_handle The handle to the data structure where the element will be set.
 * @param[in] name The name of the element to be set.
 * @param[in] element The handle to the element to be set in the structure.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_data_set_struct_element(mmi_data_h struct_handle, const char *name, mmi_data_h element);

/**
 * @brief Retrieves the type of a data handle.
 * @details This function retrieves the type of the specified data handle.
 * @since_tizen 9.0
 *
 * @param[in] data The handle to the data whose type is to be retrieved.
 * @param[out] type A pointer to the variable where the type will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_data_get_type(mmi_data_h data, mmi_data_type_e *type);

/**
 * @brief Retrieves the boolean value from a data handle.
 * @details This function retrieves the boolean value stored in the specified data handle.
 * @since_tizen 9.0
 *
 * @param[in] data The handle to the data from which the boolean value will be retrieved.
 * @param[out] value A pointer to the variable where the boolean value will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_data_get_bool(mmi_data_h data, bool *value);

/**
 * @brief Retrieves the integer value from a data handle.
 * @details This function retrieves the integer value stored in the specified data handle.
 * @since_tizen 9.0
 *
 * @param[in] data The handle to the data from which the integer value will be retrieved.
 * @param[out] value A pointer to the variable where the integer value will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_data_get_int(mmi_data_h data, int *value);

/**
 * @brief Retrieves the floating-point value from a data handle.
 * @details This function retrieves the floating-point value stored in the specified data handle.
 * @since_tizen 9.0
 *
 * @param[in] data The handle to the data from which the floating-point value will be retrieved.
 * @param[out] value A pointer to the variable where the floating-point value will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_data_get_float(mmi_data_h data, float *value);

/**
 * @brief Retrieves the text string from a data handle.
 * @details This function retrieves the text string stored in the specified data handle.
 * @since_tizen 9.0
 * @remarks The @a text should not be released.
 *          The @a text is available until data is released.
 *
 * @param[in] data The handle to the data from which the text string will be retrieved.
 * @param[out] text A pointer to the variable where the text string will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the data handle
   const char *text = NULL;
   mmi_data_get_text(data, &text);
 * @endcode
 */
int mmi_data_get_text(mmi_data_h data, const char **text);

/**
 * @brief Retrieves the audio data from a data handle.
 * @details This function retrieves the audio data stored in the specified data handle.
 * @since_tizen 9.0
 * @remarks The @a ptr should not be released.
 *          The @a ptr is available until data is released.
 *
 * @param[in] data The handle to the data from which the audio data will be retrieved.
 * @param[out] ptr A pointer to the variable where the audio data will be stored.
 * @param[out] len A pointer to the variable where the length of the audio data will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the data handle
   const void *ptr = NULL;
   size_t len = 0;
   mmi_data_get_audio(data, &ptr, &len);
 * @endcode
 */
int mmi_data_get_audio(mmi_data_h data, const void **ptr, size_t *len);

/**
 * @brief Retrieves the video data from a data handle.
 * @details This function retrieves the video data stored in the specified data handle.
 * @since_tizen 9.0
 * @remarks The @a ptr should not be released.
 *          The @a ptr is available until data is released.
 *
 * @param[in] data The handle to the data from which the video data will be retrieved.
 * @param[out] ptr A pointer to the variable where the video data will be stored.
 * @param[out] len A pointer to the variable where the length of the video data will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the data handle
   const void *ptr = NULL;
   size_t len = 0;
   mmi_data_get_video(data, &ptr, &len);
 * @endcode
 */
int mmi_data_get_video(mmi_data_h data, const void **ptr, size_t *len);

/**
 * @brief Retrieves the user identification data from a data handle.
 * @details This function retrieves the user identification data stored in the specified data handle.
 * @since_tizen 9.0
 * @remarks The @a ptr should not be released.
 *          The @a ptr is available until data is released.
 *
 * @param[in] data The handle to the data from which the user identification data will be retrieved.
 * @param[out] ptr A pointer to the variable where the user identification data will be stored.
 * @param[out] len A pointer to the variable where the length of the user identification data will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the data handle
   const void *ptr = NULL;
   size_t len = 0;
   mmi_data_get_user_identification(data, &ptr, &len);
 * @endcode
 */
int mmi_data_get_user_identification(mmi_data_h data, const void **ptr, size_t *len);

/**
 * @brief Retrieves the coordinate data from a data handle.
 * @details This function retrieves the coordinate data stored in the specified data handle.
 * @since_tizen 9.0
 *
 * @param[in] data The handle to the data from which the coordinate data will be retrieved.
 * @param[out] x A pointer to the variable where the x-coordinate value will be stored.
 * @param[out] y A pointer to the variable where the y-coordinate value will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_data_get_coordinate(mmi_data_h data, int *x, int *y);

/**
 * @brief Retrieves the bounding box of the given data.
 * @details This function retrieves the bounding box coordinates (x, y, width, height) of the specified data.
 *
 * @since_tizen 9.0
 *
 * @param[in] data The handle to the data object.
 * @param[out] x Pointer to an integer where the x-coordinate will be stored.
 * @param[out] y Pointer to an integer where the y-coordinate will be stored.
 * @param[out] w Pointer to an integer where the width will be stored.
 * @param[out] h Pointer to an integer where the height will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_data_get_bounding_box(mmi_data_h data, int *x, int *y, int *w, int *h);

/**
 * @brief Gets the number of elements in the array.
 * @details This function retrieves the total count of elements present in the specified array.
 *
 * @since_tizen 9.0
 *
 * @param[in] array The handle to the array object.
 * @param[out] count Pointer to a size_t where the element count will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_data_get_array_count(mmi_data_h array, size_t *count);

/**
 * @brief Retrieves an element from the array at the specified index.
 * @details This function retrieves a handle to the element located at the specified index in the array.
 * @since_tizen 9.0
 * @remarks The @a element should not be released.
 *          The @a element is available until struct_handle is released.
 *
 * @param[in] array The handle to the array object.
 * @param[in] index The index of the element to retrieve.
 * @param[out] element Pointer to a #mmi_data_h where the element handle will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the handle of the data
   mmi_data_h element = NULL;
   mmi_data_get_array_element(data, 1, &element);
   ...
 * @endcode
 */
int mmi_data_get_array_element(mmi_data_h array, size_t index, mmi_data_h *element);

/**
 * @brief Retrieves an element from the structure by its name.
 * @details This function retrieves a handle to the element in the structure identified by the specified name.
 * @since_tizen 9.0
 * @remarks The @a element should not be released.
 *          The @a element is available until struct_handle is released.
 *
 * @param[in] struct_handle The handle to the structure object.
 * @param[in] name The name of the element to retrieve.
 * @param[out] element Pointer to a #mmi_data_h where the element handle will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the handle of the data
   mmi_data_h element = NULL;
   mmi_data_get_struct_element(data, "element_1", &element);
   ...
 * @endcode
 */
int mmi_data_get_struct_element(mmi_data_h struct_handle, const char *name, mmi_data_h *element);

/**
 * @brief Gets the number of elements in the structure.
 * @details This function retrieves the total count of elements present in the specified structure.
 *
 * @since_tizen 9.0
 *
 * @param[in] struct_handle The handle to the structure object.
 * @param[out] count Pointer to a size_t where the element count will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_data_get_struct_count(mmi_data_h struct_handle, size_t *count);

/**
 * @brief Retrieves the name of the element at the specified index in the structure.
 * @details This function retrieves the name of the element located at the specified index in the structure.
 * @since_tizen 9.0
 * @remarks The @a name should not be released.
 *          The @a name is available until struct_handle is released.
 *
 * @param[in] struct_handle The handle to the structure object.
 * @param[in] index The index of the element whose name is to be retrieved.
 * @param[out] name Pointer to a const char pointer where the name will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the handle of the data structure
   char *name = NULL;
   mmi_data_get_struct_element_name(data, 1, &name):
   ...
 * @endcode
 */
int mmi_data_get_struct_element_name(mmi_data_h struct_handle, size_t index, const char **name);

/**
 * @brief Retrieves the of the element at the specified index in the structure.
 * @details This function retrieves a handle to the value of the element located at the specified index in the structure.
 * @since_tizen 9.0
 * @remarks The @a element should not be released.
 *          The @a element is available until struct_handle is released.
 *
 * @param[in] struct_handle The handle to the structure object.
 * @param[in] index The index of the element whose value is to be retrieved.
 * @param[out] element Pointer to a #mmi_data_h where the element handle will be stored.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the handle of the data structure
   mmi_data_h element = NULL;
   mmi_data_get_struct_element_value(data, 1, &element);
   ...
 * @endcode
 */
int mmi_data_get_struct_element_value(mmi_data_h struct_handle, size_t index, mmi_data_h *element);

/**
 * @brief Converts the MMI data handle to a byte array.
 * @details This function converts the given MMI data handle into a byte array representation.
 * @since_tizen 9.0
 * @remarks The @a bytes should be released using free().
 *
 * @param[in] data The handle to the MMI data.
 * @param[out] bytes A pointer to store the resulting byte array.
 * @param[out] length A pointer to store the length of the byte array.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_data_h data; // Indicates the handle of the data
   unsigned char *bytes = nullptr;
   size_t size = 0;
   mmi_data_to_bytes(data, &bytes, &size);
   ...
   free(bytes);
 * @endcode
 */
int mmi_data_to_bytes(mmi_data_h data, unsigned char **bytes, size_t *length);

/**
 * @brief Creates an MMI data handle from a byte array.
 * @details This function creates an MMI data handle from the provided byte array.
 * @since_tizen 9.0
 * @remarks The @a data should be released using mmi_data_destroy().
 *
 * @param[in] bytes The byte array containing the data.
 * @param[in] length The length of the byte array.
 * @param[out] data A pointer to store the resulting MMI data handle.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   unsigned char *bytes; // Indicates the byte array to be converted
   size_t size; // Indicates the size of the byte array
   mmi_data_h data = NULL;
   mmi_data_from_bytes(bytes, size, &data);
   ...
   mmi_data_destroy(data);
 * @endcode
 */
int mmi_data_from_bytes(unsigned char *bytes, size_t length, mmi_data_h *data);

/**
 * @brief Destroys the given MMI data handle.
 * @details This function destroys the specified MMI data handle and releases associated resources.
 * @since_tizen 9.0
 *
 * @param[in] data The handle to the MMI data.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_data_destroy(mmi_data_h data);


#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_DATA_H__ */
