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


#ifndef __TIZEN_UIX_MMI_NODE_H__
#define __TIZEN_UIX_MMI_NODE_H__


#include <mmi-attribute.h>
#include <mmi-data.h>
#include <mmi-error.h>
#include <mmi-port.h>
#include <mmi-signal.h>

/**
* @file mmi-node.h
*/


/**
* @addtogroup CAPI_UIX_MMI_NODE_MODULE
* @ingroup CAPI_UIX_MMI_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for MMI node types.
 * @details This enumeration defines the different types of nodes in the MMI framework.
 * @since_tizen 9.0
*/
typedef enum {
    MMI_NODE_TYPE_NONE,          /**< Indicates no specific node type. */
    MMI_NODE_TYPE_SOURCE,        /**< Represents a source node. */
    MMI_NODE_TYPE_PROCESSOR,     /**< Represents a processor node. */
    MMI_NODE_TYPE_LOGIC,         /**< Represents a logic node. */
    MMI_NODE_TYPE_CONTROLLER,    /**< Represents a controller node. */
    MMI_NODE_TYPE_ACTION,        /**< Represents an action node. */
    MMI_NODE_TYPE_CUSTOM,        /**< Represents a custom node. */
} mmi_node_type_e;

/**
 * @brief Handle for MMI node instance.
 * @details This handle represents an instance of a node created within the MMI framework.
 * @since_tizen 9.0
*/
typedef void* mmi_node_instance_h;

/**
 * @brief Callback function type for MMI node initialization.
 * @details This callback function is called when a node instance is initialized.
 * @since_tizen 9.0
 * @remarks The @a instance should not be released.
 *          The @a instance is available until the node instance is released.
 *
 * @param[in] instance The handle of the initialized node instance.
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @return An integer value indicating the result of the callback execution.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
*/
typedef int (*mmi_node_initialized_cb)(mmi_node_instance_h instance, void *user_data);

/**
 * @brief Callback function type for MMI node deinitialization.
 * @details This callback function is called when a node instance is deinitialized.
 * @since_tizen 9.0
 * @remarks The @a instance should not be released.
 *          The @a instance is available until the node instance is released.
 *
 * @param[in] instance The handle of the deinitialized node instance.
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @return An integer value indicating the result of the callback execution.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
*/
typedef int (*mmi_node_deinitialized_cb)(mmi_node_instance_h instance, void *user_data);

/**
 * @brief Callback function type for setting MMI node attributes.
 * @details This callback function is called when an attribute is set on a node instance.
 * @since_tizen 9.0
 * @remarks The @a instance should not be released.
 *          The @a instance is available until the node instance is released.
 * @remarks The @a attribute should not be released.
 *          The @a attribute can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] instance The handle of the node instance.
 * @param[in] attribute The handle of the attribute being set.
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @return An integer value indicating the result of the callback execution.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
*/
typedef int (*mmi_node_attribute_set_cb)(mmi_node_instance_h instance, mmi_attribute_h attribute, void *user_data);

/**
 * @brief Callback function type for MMI node activation.
 * @details This callback function is called when a node instance is activated.
 * @since_tizen 9.0
 * @remarks The @a instance should not be released.
 *          The @a instance is available until the node instance is released.
 *
 * @param[in] instance The handle of the activated node instance.
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @return An integer value indicating the result of the callback execution.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
*/
typedef int (*mmi_node_activated_cb)(mmi_node_instance_h instance, void *user_data);

/**
 * @brief Callback function type for MMI node deactivation.
 * @details This callback function is called when a node instance is deactivated.
 * @since_tizen 9.0
 * @remarks The @a instance should not be released.
 *          The @a instance is available until the node instance is released.
 *
 * @param[in] instance The handle of the deactivated node instance.
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @return An integer value indicating the result of the callback execution.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
*/
typedef int (*mmi_node_deactivated_cb)(mmi_node_instance_h instance, void *user_data);

/**
 * @brief Callback function type for receiving MMI signals.
 * @details This callback function is called when a signal is received by a node instance.
 * @since_tizen 9.0
 * @remarks The @a instance should not be released.
 *          The @a instance is available until the node instance is released.
 * @remarks The @a signal should not be released.
 *          The @a signal can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] instance The handle of the node instance receiving the signal.
 * @param[in] signal The handle of the received signal.
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @return An integer value indicating the result of the callback execution.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
*/
typedef int (*mmi_node_signal_received_cb)(mmi_node_instance_h instance, mmi_signal_h signal, void *user_data);

/**
 * @brief Handle for MMI node.
 * @details This handle represents a node created within the MMI framework.
 * @since_tizen 9.0
*/
typedef struct mmi_node_s* mmi_node_h;

/**
 * @brief Sets a callback function to be invoked when the node is initialized.
 * @details This function sets a callback function that will be called when the node is initialized.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the node for which the initialized callback function is to be set.
 * @param[in] callback The callback function to be called when the node is initialized.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_initialized_cb()
 */
int mmi_node_set_initialized_cb(mmi_node_h node, mmi_node_initialized_cb callback, void *user_data);

/**
 * @brief Sets a callback function to be invoked when the node is deinitialized.
 * @details This function sets a callback function that will be called when the node is deinitialized.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the node for which the deinitialized callback function is to be set.
 * @param[in] callback The callback function to be called when the node is deinitialized.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_deinitialized_cb()
 */
int mmi_node_set_deinitialized_cb(mmi_node_h node, mmi_node_deinitialized_cb callback, void *user_data);

/**
 * @brief Sets a callback function to be invoked when an attribute is set on the node.
 * @details This function sets a callback function that will be called when an attribute is set on the node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the node for which the attribute set callback function is to be set.
 * @param[in] callback The callback function to be called when an attribute is set on the node.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_attribute_set_cb()
 */
int mmi_node_set_attribute_set_cb(mmi_node_h node, mmi_node_attribute_set_cb callback, void *user_data);

/**
 * @brief Sets a callback function to be invoked when the node is activated.
 * @details This function sets a callback function that will be called when the node is activated.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the node for which the activated callback function is to be set.
 * @param[in] callback The callback function to be called when the node is activated.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_activated_cb()
 */
int mmi_node_set_activated_cb(mmi_node_h node, mmi_node_activated_cb callback, void *user_data);

/**
 * @brief Sets a callback function to be invoked when the specified MMI node is deactivated.
 * @details This function sets a callback function that will be called whenever the specified node is deactivated.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the MMI node.
 * @param[in] callback The callback function to be called when the node is deactivated.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_deactivated_cb()
 * @see mmi_node_unset_deactivated_cb()
 */
int mmi_node_set_deactivated_cb(mmi_node_h node, mmi_node_deactivated_cb callback, void *user_data);

/**
 * @brief Sets a callback function to be invoked when a signal is received by the specified MMI node.
 * @details This function sets a callback function that will be called whenever a signal is received by the specified node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the MMI node.
 * @param[in] callback The callback function to be called when a signal is received.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_signal_received_cb()
 * @see mmi_node_unset_signal_received_cb()
 */
int mmi_node_set_signal_received_cb(mmi_node_h node, mmi_node_signal_received_cb callback, void *user_data);

/**
 * @brief Adds a static port to a static node.
 * @details This function adds an already created port to a node. The port must be created before calling this function.
 * @since_tizen 9.0
 *
 * @param[in] node The handle to the node where the port will be added.
 * @param[in] port The handle to the port that will be added to the node.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The port must be created before adding it to the node.
 * @post The port is added to the node.
 */
int mmi_node_add_port(mmi_node_h node, mmi_port_h port);

/**
 * @brief Finds a port in a node matching the given name and type.
 * @details This function searches for a port in a node that matches the specified name and type.
 * @since_tizen 9.0
 * @remarks The @a port should not be released.
 *          The @a port is available until node is released.
 *
 * @param[in] node The handle to the node where the search will be performed.
 * @param[in] port_type The type of the port to be found.
 * @param[in] port_name The name of the port to be found.
 * @param[out] port A pointer to store the handle of the found port.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The node must exist and contain ports.
 * @post If the port is found, its handle is stored in the provided pointer.
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_node_h node; // Indicates the data handle
   mmi_port_h port = NULL;
   mmi_node_find_port(node, MMI_PORT_TYPE_OUT, "OUTPUT", &port);
 * @endcode
 */
int mmi_node_find_port(mmi_node_h node, mmi_port_type_e port_type, const char *port_name, mmi_port_h *port);

/**
 * @brief Retrieves the type of a node.
 * @details This function retrieves the type of the node represented by the given handle.
 * @since_tizen 9.0
 *
 * @param[in] node The handle to the node whose type will be retrieved.
 * @param[out] type A pointer to store the type of the node.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The node must exist.
 * @post The type of the node is stored in the provided pointer.
 */
int mmi_node_get_type(mmi_node_h node, mmi_node_type_e *type);

/**
 * @brief Retrieves the number of ports in a node.
 * @details This function retrieves the total number of ports present in the node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle the node whose port count will be retrieved.
 * @param[out] port_count A pointer to store the number of ports in the node.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The node must exist.
 * @post The number of ports in the node is stored in the provided pointer.
 */
int mmi_node_get_port_count(mmi_node_h node, size_t *port_count);

/**
 * @brief Retrieves the port at the given index.
 * @details This function retrieves the port at the specified index in the node.
 * @since_tizen 9.0
 * @remarks The @a port should not be released.
 *          The @a port is available until node is released.
 *
 * @param[in] node The handle to the node from which the port will be retrieved.
 * @param[in] index The index of the port to be retrieved.
 * @param[out] port A pointer to store the handle of the retrieved port.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The node must exist and contain ports.
 * @post The handle of the port at the specified index is stored in the provided pointer.
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_node_h node; // Indicates the data handle
   mmi_port_h port = NULL;
   mmi_node_get_port(node, 0, &port);
 * @endcode
 */
int mmi_node_get_port(mmi_node_h node, size_t index, mmi_port_h *port);

/**
 * @brief Registers a node.
 * @details This function registers the node within the MMI framework.
 * @since_tizen 9.0
 *
 * @param[in] node The handle to the node to be registered.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The node must exist and be properly initialized.
 * @post The node is registered within the MMI framework.
 */
int mmi_node_register(mmi_node_h node);

/**
 * @brief Clones a node.
 * @details This function creates a copy of the node and returns a handle to the cloned node.
 * @since_tizen 9.0
 * @remarks The @a cloned should be released using mmi_attribute_destroy().
 *
 * @param[in] node The handle to the node to be cloned.
 * @param[out] cloned A pointer to store the handle of the cloned node.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The node must exist.
 * @post A clone of the node is created and its handle is stored in the provided pointer.
 *
 * @par Example
 * @code
   #include <mmi.h>
   ...
   mmi_node_h node; // Indicates the handle of the node
   mmi_node_h cloned = NULL;
   mmi_node_clone(node, &cloned);
   ...
   mmi_node_destroy(cloned);
 * @endcode
 */
int mmi_node_clone(mmi_node_h node, mmi_node_h *cloned);

/**
 * @brief Destroys a node.
 * @details This function destroys the node and releases all associated resources.
 * @since_tizen 9.0
 *
 * @param[in] node The handle to the node to be destroyed.
 *
 * @return An integer value indicating the result of the operation.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The node must exist.
 * @post The node is destroyed and all associated resources are released.
 */
int mmi_node_destroy(mmi_node_h node);

/**
 * @brief Provides an attribute to a node instance.
 * @details This function sets an attribute to a specified node instance.
 * @since_tizen 9.0
 *
 * @param[in] instance The handle to the node instance.
 * @param[in] attribute The handle to the attribute to be set.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 */
int mmi_node_instance_set_attribute(mmi_node_instance_h instance, mmi_attribute_h attribute);

/**
 * @brief Retrieves a port instance of a node instance.
 * @details This function finds and retrieves a port instance of a specified node instance based on its type and name.
 * @since_tizen 9.0
 * @remarks The @a port_instance should not be released.
 * @remarks The @a port_instance is available until node_instance is released.
 *
 * @param[in] node_instance The handle to the node instance.
 * @param[in] port_type The type of the port (input or output).
 * @param[in] port_name The name of the port.
 * @param[out] port_instance The handle to the retrieved port instance.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
   mmi_node_instance_h node; // Indicates the node instance handle.
   mmi_port_instance_h port = NULL;
   mmi_node_instance_find_port_instance(node, MMI_PORT_TYPE_OUT, "OUTPUT", &port);
 * @endcode
 */
int mmi_node_instance_find_port_instance(mmi_node_instance_h node_instance, mmi_port_type_e port_type, const char *port_name, mmi_port_instance_h *port_instance);

/**
 * @brief Finds a node instance that has the given port instance.
 * @details This function finds and retrieves the node instance that contains the specified port instance.
 * @since_tizen 9.0
 * @remarks The @a node_instance should not be released.
 * @remarks The @a node_instance is available until port_instance is released.
 *
 * @param[in] port_instance The handle to the port instance.
 * @param[out] node_instance The handle to the retrieved node instance.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
   mmi_port_instance_h port; // Indicates the port instance of the node.
   mmi_node_instance_h node = NULL;
   mmi_node_instance_find_by_port_instance(port, &node);
 * @endcode
 */
int mmi_node_instance_find_by_port_instance(mmi_port_instance_h port_instance, mmi_node_instance_h *node_instance);

/**
 * @brief Retrieves a sibling port instance of a port instance in a node.
 * @details This function finds and retrieves a sibling port instance of a specified port instance in a node, matching the given name.
 * @since_tizen 9.0
 * @remarks The @a sibling should not be released.
 * @remarks The @a sibling is available until node is released.
 *
 * @param[in] instance The handle to the port instance.
 * @param[in] sibling_name The name of the sibling port.
 * @param[out] sibling The handle to the retrieved sibling port instance.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
   mmi_port_instance_h port; // Indicates the port instance of the node.
   mmi_port_instance_h sibling = NULL;
   mmi_node_instance_find_sibling_port_instance(port, "PORT_2", &sibling);
 * @endcode
 */
int mmi_node_instance_find_sibling_port_instance(mmi_port_instance_h instance, const char *sibling_name, mmi_port_instance_h *sibling);

/**
 * @brief Emits a signal from a node instance.
 * @details This function emits a signal from a specified node instance.
 * @since_tizen 9.0
 *
 * @param[in] instance The handle to the node instance.
 * @param[in] signal The handle to the signal to be emitted.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_node_instance_emit_signal(mmi_node_instance_h instance, mmi_signal_h signal);

/**
 * @brief Updates the pending activation result of a node instance.
 * @details This function updates the result of the last activation callback call for a specified node instance.
 * @since_tizen 9.0
 *
 * @param[in] instance The handle to the node instance.
 * @param[in] result The result to be updated.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_node_instance_update_pending_activation_result(mmi_node_instance_h instance, mmi_error_e result);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_NODE_H__ */
