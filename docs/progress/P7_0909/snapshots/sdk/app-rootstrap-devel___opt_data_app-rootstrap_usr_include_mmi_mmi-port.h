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


#ifndef __TIZEN_UIX_MMI_PORT_H__
#define __TIZEN_UIX_MMI_PORT_H__


#include <mmi-data.h>
#include <mmi-error.h>

/**
* @file mmi-port.h
*/


/**
* @addtogroup CAPI_UIX_MMI_NODE_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for port types.
 * @details This enumeration defines the different types of ports available in the MMI framework.
 * @since_tizen 9.0
*/
typedef enum {
    MMI_PORT_TYPE_IN,     /**< Indicates an input port. */
    MMI_PORT_TYPE_OUT,    /**< Indicates an output port. */
} mmi_port_type_e;

/**
 * @brief Handle for MMI port instance.
 * @details This handle represents a port instance created within the MMI framework.
 * @since_tizen 9.0
*/
typedef void* mmi_port_instance_h;

/**
 * @brief Callback function type for input data reception.
 * @details This callback function is called whenever input data is received for a port instance.
 * @since_tizen 9.0
 * @remarks The @a instance should not be released.
 *          The @a instance is available until the port instance is released.
 * @remarks The @a data should not be released.
 *          The @a data can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] instance The port instance handle.
 * @param[in] data The received input data.
 * @param[in] user_data The user data passed from the registration function.
 *
 * @return An integer value indicating the result of the callback execution.
 * @retval 0 on success, otherwise a negative error value.
*/
typedef int (*mmi_port_input_data_received_cb)(mmi_port_instance_h instance, mmi_data_h data, void *user_data);

/**
 * @brief Handle for MMI port.
 * @details This handle represents a port created within the MMI framework.
 * @since_tizen 9.0
*/
typedef struct mmi_port_s* mmi_port_h;

/**
 * @brief Creates a new port.
 * @details This function creates a new port and initializes its resources.
 * @since_tizen 9.0
 * @remarks The @a port should be released using mmi_port_destroy().
 *
 * @param[out] port A pointer to the newly created port handle.
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
   mmi_port_h port = NULL;
   mmi_port_create(&port);
   ...
   mmi_port_destroy(port);
 * @endcode
 *
 * @see mmi_port_destroy()
 */
int mmi_port_create(mmi_port_h *port);

/**
 * @brief Gets the name of a port.
 * @details This function retrieves the name associated with the specified port.
 * @since_tizen 9.0
 * @remarks The @a name should be released using free().
 *
 * @param[in] port The port handle.
 * @param[out] name A pointer to the buffer where the name will be stored.
 * @param[out] length A pointer to the variable that will receive the length of the name.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The port handle must be valid and initialized.
 * @post The caller is responsible for freeing the allocated memory for the name.
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_port_h port; // Indicates the handle of the port
   char *name = NULL;
   size_t length = 0;
   mmi_port_get_name(port, &name, &length);
   ...
   free(name);
 * @endcode
 *
 * @see mmi_port_set_name()
 */
int mmi_port_get_name(mmi_port_h port, char **name, size_t *length);

/**
 * @brief Gets the type of a port.
 * @details This function retrieves the type associated with the specified port.
 * @since_tizen 9.0
 *
 * @param[in] port The port handle.
 * @param[out] type A pointer to the variable that will receive the type of the port.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The port handle must be valid and initialized.
 *
 * @see mmi_port_set_type()
 * @see mmi_port_type_e
 */
int mmi_port_get_type(mmi_port_h port, mmi_port_type_e *type);

/**
 * @brief Gets the data type of a port.
 * @details This function retrieves the data type associated with the specified port.
 * @since_tizen 9.0
 *
 * @param[in] port The port handle.
 * @param[out] data_type A pointer to the variable that will receive the data type of the port.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The port handle must be valid and initialized.
 *
 * @see mmi_port_set_data_type()
 * @see mmi_data_type_e
 */
int mmi_port_get_data_type(mmi_port_h port, mmi_data_type_e *data_type);

/**
 * @brief Sets the name of a port.
 * @details This function sets the name for the specified port.
 * @since_tizen 9.0
 *
 * @param[in] port The port handle.
 * @param[in] name The new name to be set for the port.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The port handle must be valid and initialized.
 *
 * @see mmi_port_get_name()
 */
int mmi_port_set_name(mmi_port_h port, const char *name);

/**
 * @brief Sets the type of a port.
 * @details This function sets the type for the specified port.
 * @since_tizen 9.0
 *
 * @param[in] port The port handle.
 * @param[in] type The new type to be set for the port.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The port handle must be valid and initialized.
 *
 * @see mmi_port_get_type()
 * @see mmi_port_type_e
 */
int mmi_port_set_type(mmi_port_h port, mmi_port_type_e type);

/**
 * @brief Sets the data type of a port.
 * @details This function sets the data type for the specified port.
 * @since_tizen 9.0
 *
 * @param[in] port The port handle.
 * @param[in] data_type The new data type to be set for the port.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The port handle must be valid and initialized.
 *
 * @see mmi_port_get_data_type()
 * @see mmi_data_type_e
 */
int mmi_port_set_data_type(mmi_port_h port, mmi_data_type_e data_type);

/**
 * @brief Sets the input data received callback function.
 * @details This function sets the callback function that will be invoked when input data is received on the specified port.
 * @since_tizen 9.0
 *
 * @param[in] port The handle of the MMI port.
 * @param[in] callback The callback function to set.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful.
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter.
 */
int mmi_port_set_input_data_received_cb(mmi_port_h port, mmi_port_input_data_received_cb callback, void *user_data);

/**
 * @brief Clones a port.
 * @details This function creates a copy of the specified port.
 * @since_tizen 9.0
 * @remarks The @a cloned should be released using mmi_port_destroy().
 *
 * @param[in] port The port handle to be cloned.
 * @param[out] cloned A pointer to the newly created cloned port handle.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre The port handle must be valid and initialized.
 * @post The cloned port handle must be destroyed using mmi_port_destroy() after use.
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_port_h port; // Indicates the handle of the port
   mmi_port_h cloned = NULL;
   mmi_port_clone(port, &cloned);
   ...
   mmi_port_destroy(cloned);
 * @endcode
 *
 * @see mmi_port_create()
 * @see mmi_port_destroy()
 */
int mmi_port_clone(mmi_port_h port, mmi_port_h *cloned);

/**
 * @brief Destroys a port.
 * @details This function destroys the specified port, releasing all associated resources.
 * @since_tizen 9.0
 *
 * @param[in] port The handle of the port to be destroyed.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_port_destroy(mmi_port_h port);

/**
 * @brief Generates data to the port with the given port instance.
 * @details This function generates data to the specified port using the provided port instance and data handle.
 * @since_tizen 9.0
 *
 * @param[in] instance The handle of the port instance.
 * @param[in] data The handle of the data to be generated.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_port_instance_generate_output(mmi_port_instance_h instance, mmi_data_h data);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_PORT_H__ */
