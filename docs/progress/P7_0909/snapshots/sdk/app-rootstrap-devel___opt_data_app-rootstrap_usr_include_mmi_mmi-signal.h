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


#ifndef __TIZEN_UIX_MMI_SIGNAL_H__
#define __TIZEN_UIX_MMI_SIGNAL_H__


#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mmi-primitive-value.h>

/**
* @file mmi-signal.h
*/


/**
* @addtogroup CAPI_UIX_MMI_COMMON_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handle for MMI signal parameter.
 * @details This handle represents a parameter that a signal can embed within the MMI framework.
 * @since_tizen 9.0
 */
typedef struct mmi_signal_parameter_s* mmi_signal_parameter_h;

/**
 * @brief Handle for MMI signal.
 * @details This handle represents a signal that can be sent to a workflow or a node within the MMI framework.
 * @since_tizen 9.0
 */
typedef struct mmi_signal_s* mmi_signal_h;

/**
 * @brief Creates a new signal parameter.
 * @details This function creates a new signal parameter with the given primitive value and name.
 * @since_tizen 9.0
 * @remarks The @a parameter should be released using mmi_signal_parameter_destroy().
 *
 * @param[in] value The primitive value to associate with the parameter.
 * @param[in] name The name of the parameter.
 * @param[out] parameter A pointer to store the newly created parameter handle.
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
   mmi_primitive_value_h value; // Indicates the primitive value to be set in the signal_parameter
   mmi_signal_parameter_h parameter = NULL;
   mmi_signal_parameter_create(value, "Name", &parameter);
   ...
   mmi_signal_parameter_destroy(parameter);
 * @endcode
 */
int mmi_signal_parameter_create(mmi_primitive_value_h value, const char *name,  mmi_signal_parameter_h *parameter);

/**
 * @brief Retrieves the name of a signal parameter.
 * @details This function retrieves the name associated with the given signal parameter.
 * @since_tizen 9.0
 * @remarks The @a name should be released using free().
 *
 * @param[in] parameter The signal parameter handle.
 * @param[out] name A pointer to store the name of the parameter.
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
   mmi_signal_parameter_h parameter; // Indicates the handle of the signal parameter
   char *name = NULL;
   mmi_signal_parameter_get_name(parameter, &name):
   ...
   free(name);
 * @endcode
 */
int mmi_signal_parameter_get_name(mmi_signal_parameter_h parameter, char **name);

/**
 * @brief Retrieves the primitive value of a signal parameter.
 * @details This function retrieves the primitive value associated with the given signal parameter.
 * @since_tizen 9.0
 * @remarks The @a value should be released using mmi_primitive_value_destroy().
 *
 * @param[in] parameter The signal parameter handle.
 * @param[out] value A pointer to store the primitive value of the parameter.
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
   mmi_signal_parameter_h parameter; // Indicates the handle of the signal parameter
   mmi_primitive_value_h value = NULL;
   mmi_signal_parameter_get_value(parameter, &value);
   ...
   mmi_primitive_value_destroy(value);
 * @endcode
 */
int mmi_signal_parameter_get_value(mmi_signal_parameter_h parameter, mmi_primitive_value_h *value);

/**
 * @brief Clones a signal parameter.
 * @details This function creates a deep copy of the given signal parameter.
 * @since_tizen 9.0
 * @remarks The @a cloned should be released using mmi_signal_parameter_destroy().
 *
 * @param[in] parameter The signal parameter handle to clone.
 * @param[out] cloned A pointer to store the cloned parameter handle.
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
   mmi_signal_parameter_h parameter; // Indicates the handle of the signal parameter
   mmi_signal_parameter_h cloned = NULL;
   mmi_signal_parameter_clone(parameter, &cloned);
   ...
   mmi_signal_parameter_destroy(cloned);
 * @endcode
 */
int mmi_signal_parameter_clone(mmi_signal_parameter_h parameter, mmi_signal_parameter_h *cloned);

/**
 * @brief Destroys a signal parameter.
 * @details This function destroys the given signal parameter and releases its resources.
 * @since_tizen 9.0
 *
 * @param[in] parameter The signal parameter handle to destroy.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_signal_parameter_destroy(mmi_signal_parameter_h parameter);

/**
 * @brief Creates a new signal.
 * @details This function creates a new signal with the given name.
 * @since_tizen 9.0
 * @remarks The @a handle should be released using mmi_signal_destroy().
 *
 * @param[in] name The name of the signal.
 * @param[out] handle A pointer to store the newly created signal handle.
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
   mmi_signal_h handle = NULL;
   mmi_signal_create("Name", &handle);
   ...
   mmi_signal_destroy(handle);
 * @endcode
 */
int mmi_signal_create(const char *name, mmi_signal_h *handle);

/**
 * @brief Adds a parameter to a signal.
 * @details This function adds the given parameter to the specified signal.
 * @since_tizen 9.0
 *
 * @param[in] handle The signal handle to which the parameter will be added.
 * @param[in] parameter The parameter handle to add to the signal.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_signal_add_parameter(mmi_signal_h handle, mmi_signal_parameter_h parameter);

/**
 * @brief Retrieves the name of a signal.
 * @details This function retrieves the name associated with the given signal.
 * @since_tizen 9.0
 * @remarks The @a name should be released using free().
 *
 * @param[in] handle The signal handle.
 * @param[out] name A pointer to store the name of the signal.
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
   mmi_signal_h handle; // Indicates the handle of the signal
   char *name = NULL;
   mmi_signal_get_name(handle, &name):
   ...
   free(name);
 * @endcode
 */
int mmi_signal_get_name(mmi_signal_h handle, char **name);

/**
 * @brief Retrieves the number of parameters in a signal.
 * @details This function retrieves the count of parameters currently associated with the given signal.
 * @since_tizen 9.0
 *
 * @param[in] handle The signal handle.
 * @param[out] count A pointer to store the number of parameters.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_signal_get_parameter_count(mmi_signal_h handle, int *count);

/**
 * @brief Retrieves a parameter from a signal by index.
 * @details This function retrieves the parameter at the specified index from the given signal.
 * @since_tizen 9.0
 * @remarks The @a parameter should be released using mmi_signal_parameter_destroy().
 *
 * @param[in] handle The signal handle.
 * @param[in] index The index of the parameter to retrieve.
 * @param[out] parameter A pointer to store the retrieved parameter handle.
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
   mmi_signal_h handle; // Indicates the handle of the signal
   mmi_signal_parameter_h parameter = NULL;
   mmi_signal_get_parameter(handle, 1, &parameter);
   ...
   mmi_signal_parameter_destroy(parameter);
 * @endcode
 */
int mmi_signal_get_parameter(mmi_signal_h handle, int index, mmi_signal_parameter_h *parameter);

/**
 * @brief Destroys a signal.
 * @details This function destroys the given signal and releases its resources.
 * @since_tizen 9.0
 *
 * @param[in] handle The signal handle to destroy.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_signal_destroy(mmi_signal_h handle);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_SIGNAL_H__ */
