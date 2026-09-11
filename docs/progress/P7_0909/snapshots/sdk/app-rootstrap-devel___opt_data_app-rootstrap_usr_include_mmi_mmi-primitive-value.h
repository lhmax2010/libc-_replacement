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


#ifndef __TIZEN_UIX_MMI_PRIMITIVE_VALUE_H__
#define __TIZEN_UIX_MMI_PRIMITIVE_VALUE_H__


#include <stdlib.h>


/**
* @file mmi-primitive-value.h
*/


/**
* @addtogroup CAPI_UIX_MMI_COMMON_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Enumeration for primitive value types.
 * @details This enumeration defines the different types of primitive values supported by the MMI framework.
 * @since_tizen 9.0
*/
typedef enum {
    MMI_PRIMITIVE_VALUE_TYPE_INT,       /**< Integer type */
    MMI_PRIMITIVE_VALUE_TYPE_FLOAT,     /**< Floating-point type */
    MMI_PRIMITIVE_VALUE_TYPE_STRING,    /**< String type */
    MMI_PRIMITIVE_VALUE_TYPE_BOOL,      /**< Boolean type */
    MMI_PRIMITIVE_VALUE_TYPE_ARRAY,     /**< Array type */
} mmi_primitive_value_type_e;

/**
 * @brief Handle for MMI primitive value.
 * @details This handle represents a primitive value that an attribute can store within the MMI framework.
 * @since_tizen 9.0
 */
typedef struct mmi_primitive_value_s* mmi_primitive_value_h;


/**
 * @brief Creates a primitive value handle for an integer.
 * @details This function creates a handle for an integer primitive value.
 * @since_tizen 9.0
 * @remarks The @a handle should be released using mmi_primitive_value_destroy().
 *
 * @param[in] data The integer value to be stored in the handle.
 * @param[out] handle Pointer to the handle which will be populated with the newly created handle.
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
   mmi_primitive_value_h handle = NULL;
   mmi_primitive_value_create_int(3, &handle);
   ...
   mmi_primitive_value_destroy(handle);
 * @endcode
 */
int mmi_primitive_value_create_int(int data, mmi_primitive_value_h *handle);

/**
 * @brief Creates a primitive value handle for a floating-point number.
 * @details This function creates a handle for a floating-point primitive value.
 * @since_tizen 9.0
 * @remarks The @a handle should be released using mmi_primitive_value_destroy().
 *
 * @param[in] data The floating-point value to be stored in the handle.
 * @param[out] handle Pointer to the handle which will be populated with the newly created handle.
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
   mmi_primitive_value_h handle = NULL;
   mmi_primitive_value_create_float(3.14, &handle);
   ...
   mmi_primitive_value_destroy(handle);
 * @endcode
 */
int mmi_primitive_value_create_float(float data, mmi_primitive_value_h *handle);

/**
 * @brief Creates a primitive value handle for a string.
 * @details This function creates a handle for a string primitive value.
 * @since_tizen 9.0
 * @remarks The @a handle should be released using mmi_primitive_value_destroy().
 *
 * @param[in] data The string value to be stored in the handle.
 * @param[out] handle Pointer to the handle which will be populated with the newly created handle.
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
   mmi_primitive_value_h handle = NULL;
   mmi_primitive_value_create_string("Test", &handle);
   ...
   mmi_primitive_value_destroy(handle);
 * @endcode
 */
int mmi_primitive_value_create_string(const char *data, mmi_primitive_value_h *handle);

/**
 * @brief Creates a primitive value handle for a boolean.
 * @details This function creates a handle for a boolean primitive value.
 * @since_tizen 9.0
 * @remarks The @a handle should be released using mmi_primitive_value_destroy().
 *
 * @param[in] data The boolean value to be stored in the handle.
 * @param[out] handle Pointer to the handle which will be populated with the newly created handle.
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
   mmi_primitive_value_h handle = NULL;
   mmi_primitive_value_create_bool(true, &handle);
   ...
   mmi_primitive_value_destroy(handle);
 * @endcode
 */
int mmi_primitive_value_create_bool(bool data, mmi_primitive_value_h *handle);

/**
 * @brief Creates a primitive value handle for an array.
 * @details This function creates a handle for an array primitive value.
 * @since_tizen 9.0
 * @remarks The @a handle should be released using mmi_primitive_value_destroy().
 *
 * @param[out] handle Pointer to the handle which will be populated with the newly created handle.
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
   mmi_primitive_value_h handle = NULL;
   mmi_primitive_value_create_array(&handle);
   ...
   mmi_primitive_value_destroy(handle);
 * @endcode
 */
int mmi_primitive_value_create_array(mmi_primitive_value_h *handle);

/**
 * @brief Adds an element to a primitive value array.
 * @details This function adds an element to a primitive value array.
 * @since_tizen 9.0
 *
 * @param[in] array The handle of the array to which the element will be added.
 * @param[in] element The handle of the element to be added to the array.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_primitive_value_add_array_element(mmi_primitive_value_h array, mmi_primitive_value_h element);

/**
 * @brief Gets the type of the primitive value.
 * @details This function retrieves the type of the primitive value represented by the given handle.
 * @since_tizen 9.0
 *
 * @param[in] handle The handle to the primitive value.
 * @param[out] type A pointer to store the type of the primitive value.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_primitive_value_get_type(mmi_primitive_value_h handle, mmi_primitive_value_type_e *type);

/**
 * @brief Gets the integer value from the primitive value.
 * @details This function retrieves the integer value stored in the primitive value represented by the given handle.
 * @since_tizen 9.0
 *
 * @param[in] handle The handle to the primitive value.
 * @param[out] value A pointer to store the integer value.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_primitive_value_get_int(mmi_primitive_value_h handle, int *value);

/**
 * @brief Gets the floating-point value from the primitive value.
 * @details This function retrieves the floating-point value stored in the primitive value represented by the given handle.
 * @since_tizen 9.0
 *
 * @param[in] handle The handle to the primitive value.
 * @param[out] value A pointer to store the floating-point value.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_primitive_value_get_float(mmi_primitive_value_h handle, float *value);

/**
 * @brief Gets the string value from the primitive value.
 * @details This function retrieves the string value stored in the primitive value represented by the given handle.
 * @since_tizen 9.0
 * @remarks The @a string should not be released.
 *          The @a string is available until data is released.
 *
 * @param[in] handle The handle to the primitive value.
 * @param[out] string A pointer to store the string value.
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
   mmi_primitive_value_h value; // Indicates the primitive value handle
   const char *string = NULL;
   mmi_primitive_value_get_string(value, &string);
 * @endcode
 */
int mmi_primitive_value_get_string(mmi_primitive_value_h handle, const char **string);

/**
 * @brief Gets the boolean value from the primitive value.
 * @details This function retrieves the boolean value stored in the primitive value represented by the given handle.
 * @since_tizen 9.0
 *
 * @param[in] handle The handle to the primitive value.
 * @param[out] value A pointer to store the boolean value.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_primitive_value_get_bool(mmi_primitive_value_h handle, bool *value);

/**
 * @brief Gets the count of elements in the array.
 * @details This function retrieves the number of elements in the array represented by the given handle.
 * @since_tizen 9.0
 *
 * @param[in] array The handle to the array.
 * @param[out] count A pointer to store the number of elements in the array.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_primitive_value_get_array_count(mmi_primitive_value_h array, size_t *count);

/**
 * @brief Gets the element at the specified index in the array.
 * @details This function retrieves the element at the specified index in the array represented by the given handle.
 * @since_tizen 9.0
 * @remarks The @a element should not be released.
 *          The @a element is available until struct_handle is released.
 *
 * @param[in] array The handle to the array.
 * @param[in] index The index of the element to retrieve.
 * @param[out] element A pointer to store the handle of the retrieved element.
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
   mmi_primitive_value_h value; // Indicates the handle of the primitive value
   mmi_primitive_value_h element = NULL;
   mmi_primitive_value_get_array_element(value, 1, &element);
   ...
 * @endcode
 */
int mmi_primitive_value_get_array_element(mmi_primitive_value_h array, size_t index, mmi_primitive_value_h *element);

/**
 * @brief Clones a primitive value handle.
 * @details This function creates a copy of the given primitive value handle.
 * @since_tizen 9.0
 * @remarks The @a cloned should be released using mmi_attribute_destroy().
 *
 * @param[in] handle The handle of the primitive value to be cloned.
 * @param[out] cloned A pointer to store the cloned primitive value handle.
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
   mmi_primitive_value_h value; // Indicates the handle of the primitive value
   mmi_primitive_value_h cloned = NULL;
   mmi_primitive_value_clone(value, &cloned);
   ...
   mmi_primitive_value_destroy(cloned);
 * @endcode
 */
int mmi_primitive_value_clone(mmi_primitive_value_h handle, mmi_primitive_value_h *cloned);

/**
 * @brief Destroys a primitive value handle.
 * @details This function frees the resources associated with the given primitive value handle.
 * @since_tizen 9.0
 *
 * @param[in] handle The handle of the primitive value to be destroyed.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_primitive_value_destroy(mmi_primitive_value_h handle);

/**
 * @brief Converts a primitive value handle to a byte array.
 * @details This function serializes the given primitive value handle into a byte array.
 * @since_tizen 9.0
 * @remarks The @a bytes should be released using free().
 *
 * @param[in] handle The handle of the primitive value to be converted.
 * @param[out] bytes A pointer to store the resulting byte array.
 * @param[out] size A pointer to store the size of the byte array.
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
   mmi_primitive_value_h value; // Indicates the handle of the primitive value
   unsigned char *bytes = nullptr;
   size_t size = 0;
   mmi_primitive_value_to_bytes(value, &bytes, &size);
   ...
   free(bytes);
 * @endcode
 */
int mmi_primitive_value_to_bytes(mmi_primitive_value_h handle, unsigned char **bytes, size_t *size);

/**
 * @brief Creates a primitive value handle from a byte array.
 * @details This function deserializes a byte array into a primitive value handle.
 * @since_tizen 9.0
 * @remarks The @a handle should be released using mmi_primitive_value_destroy().
 *
 * @param[in] bytes The byte array containing the serialized primitive value.
 * @param[in] size The size of the byte array.
 * @param[out] handle A pointer to store the resulting primitive value handle.
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
   mmi_primitive_value_h value = NULL;
   mmi_primitive_value_from_bytes(bytes, size, &value);
   ...
   mmi_primitive_value_destroy(data);
 * @endcode
 */
int mmi_primitive_value_from_bytes(unsigned char *bytes, size_t size, mmi_primitive_value_h *handle);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_PRIMITIVE_VALUE_H__ */
