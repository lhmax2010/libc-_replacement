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


#ifndef __TIZEN_UIX_MMI_ATTRIBUTE_H__
#define __TIZEN_UIX_MMI_ATTRIBUTE_H__


#include <stdlib.h>
#include <mmi-primitive-value.h>


/**
* @file mmi-attribute.h
*/


/**
* @addtogroup CAPI_UIX_MMI_COMMON_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handle for MMI attributes.
 * @details This handle represents an attribute of a workflow or a node
 * @since_tizen 9.0
 */
typedef struct mmi_attribute_s* mmi_attribute_h;


/**
 * @brief Creates a new MMI attribute with a given primitive value and name.
 * @details This function initializes a new MMI attribute with the provided primitive value and name.
 * @since_tizen 9.0
 * @remarks The @a attribute should be released using mmi_attribute_destroy().
 *
 * @param[in] value The primitive value to set for the attribute.
 * @param[in] name The name to assign to the attribute.
 * @param[out] attribute A pointer to store the newly created attribute handle.
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
   mmi_primitive_value_h value; // Indicates the primitive value to be set in the attribute
   mmi_attribute_h attribute = NULL;
   mmi_attribute_create(value, "Name", &attribute);
   ...
   mmi_attribute_destroy(attribute);
 * @endcode
 */
int mmi_attribute_create(mmi_primitive_value_h value, const char *name, mmi_attribute_h *attribute);

/**
 * @brief Sets the name of an MMI attribute.
 * @details This function updates the name of the specified MMI attribute.
 * @since_tizen 9.0
 *
 * @param[in] attribute The attribute handle whose name needs to be updated.
 * @param[in] name The new name to assign to the attribute.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_attribute_set_name(mmi_attribute_h attribute, const char *name);

/**
 * @brief Retrieves the name of an MMI attribute.
 * @details This function fetches the name associated with the specified MMI attribute.
 * @since_tizen 9.0
 * @remarks The @a name should be released using free().
 *
 * @param[in] attribute The attribute handle from which to retrieve the name.
 * @param[out] name A pointer to store the retrieved name.
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
   mmi_attribute_h attribute; // Indicates the handle of the attribute
   char *name = NULL;
   mmi_attribute_get_name(attribute, &name):
   ...
   free(name);
 * @endcode
 */
int mmi_attribute_get_name(mmi_attribute_h attribute, char **name);

/**
 * @brief Retrieves the primitive value of an MMI attribute.
 * @details This function fetches the primitive value stored in the specified MMI attribute.
 * @since_tizen 9.0
 * @remarks The @a value should be released using mmi_primitive_value_destroy().
 *
 * @param[in] attribute The attribute handle from which to retrieve the value.
 * @param[out] value A pointer to store the retrieved primitive value handle.
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
   mmi_attribute_h attribute; // Indicates the handle of the attribute
   mmi_primitive_value_h value = NULL;
   mmi_attribute_get_value(attribute, &value);
   ...
   mmi_primitive_value_destroy(value);
 * @endcode
 */
int mmi_attribute_get_value(mmi_attribute_h attribute, mmi_primitive_value_h *value);

/**
 * @brief Clones an existing MMI attribute.
 * @details This function creates a deep copy of the specified MMI attribute.
 * @since_tizen 9.0
 * @remarks The @a cloned should be released using mmi_attribute_destroy().
 *
 * @param[in] attribute The attribute handle to clone.
 * @param[out] cloned A pointer to store the handle of the cloned attribute.
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
   mmi_attribute_h attribute; // Indicates the handle of the attribute
   mmi_attribute_h cloned = NULL;
   mmi_attribute_clone(attribute, &cloned);
   ...
   mmi_attribute_destroy(cloned);
 * @endcode
 */
int mmi_attribute_clone(mmi_attribute_h attribute, mmi_attribute_h *cloned);

/**
 * @brief Destroys an MMI attribute.
 * @details This function releases all resources associated with the specified MMI attribute.
 * @since_tizen 9.0
 *
 * @param[in] attribute The attribute handle to destroy.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_attribute_destroy(mmi_attribute_h attribute);

/**
 * @brief Converts an MMI attribute to a byte array.
 * @details This function serializes the specified MMI attribute into a byte array.
 * @since_tizen 9.0
 * @remarks The @a bytes should be released using free().
 *
 * @param[in] attribute The attribute handle to serialize.
 * @param[out] bytes A pointer to store the serialized byte array.
 * @param[out] length A pointer to store the length of the serialized byte array.
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
   mmi_attribute_h attribute; // Indicates the handle of the attribute
   unsigned char *bytes = nullptr;
   size_t size = 0;
   mmi_attribute_to_bytes(attribute, &bytes, &size);
   ...
   free(bytes);
 * @endcode
 */
int mmi_attribute_to_bytes(mmi_attribute_h attribute, unsigned char **bytes, size_t *length);

/**
 * @brief Reconstructs an MMI attribute from a byte array.
 * @details This function deserializes the specified byte array into an MMI attribute.
 * @since_tizen 9.0
 * @remarks The @a attribute should be released using mmi_attribute_destroy().
 *
 * @param[in] bytes The byte array to deserialize.
 * @param[in] length The length of the byte array.
 * @param[out] attribute A pointer to store the reconstructed attribute handle.
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
   mmi_attribute_h attribute = NULL;
   mmi_attribute_from_bytes(bytes, size, &attribute);
   ...
   mmi_attribute_destroy(attribute);
 * @endcode
 */
int mmi_attribute_from_bytes(const unsigned char *bytes, size_t length, mmi_attribute_h *attribute);

/**
 * @brief Creates a new MMI attribute containing a string array.
 * @details This function initializes a new MMI attribute with the provided string array and name.
 * @since_tizen 9.0
 * @remarks The @a attribute should be released using mmi_attribute_destroy().
 *
 * @param[in] name The name to assign to the attribute.
 * @param[in] strings The array of strings to set for the attribute.
 * @param[in] count The number of strings in the array.
 * @param[out] attribute A pointer to store the newly created attribute handle.
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
   mmi_attribute_h attribute = NULL;
   const char *commands[] = {"Open", "Close"};
   mmi_attribute_create_string_array("COMMANDS", commands, 2, &attribute);
   ...
   mmi_attribute_destroy(attribute);
 * @endcode
 */
int mmi_attribute_create_string_array(const char *name, const char *strings[], size_t count, mmi_attribute_h *attribute);


#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_ATTRIBUTE_H__ */
