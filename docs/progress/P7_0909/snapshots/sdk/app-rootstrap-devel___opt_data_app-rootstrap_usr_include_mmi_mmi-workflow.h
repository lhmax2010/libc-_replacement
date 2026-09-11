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


#ifndef __TIZEN_UIX_MMI_WORKFLOW_H__
#define __TIZEN_UIX_MMI_WORKFLOW_H__


#include <mmi-attribute.h>
#include <mmi-data.h>
#include <mmi-node.h>


/**
* @file mmi-workflow.h
*/


/**
* @addtogroup CAPI_UIX_MMI_WORKFLOW_MODULE
* @ingroup CAPI_UIX_MMI_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for standard MMI workflows.
 * @details This enumeration defines the different types of standard workflows supported by the MMI framework.
 * @since_tizen 9.0
*/
typedef enum {
    MMI_STANDARD_WORKFLOW_NONE = 0, /**< Indicates that no standard workflow is selected. */
    MMI_STANDARD_WORKFLOW_VOICE_TOUCH, /**< Indicates the Voice Touch workflow. */
} mmi_standard_workflow_type_e;

/**
 * @brief Handle for MMI workflow prototypes.
 * @details This handle represents a workflow prototype created within the MMI framework.
 * @since_tizen 9.0
*/
typedef struct mmi_workflow_s* mmi_workflow_h;

/**
 * @brief Creates a new workflow prototype.
 * @details This function allocates memory for a new workflow prototype and returns its handle.
 * @since_tizen 9.0
 * @remarks The @a workflow should be released using mmi_workflow_destroy().
 *
 * @param[out] workflow A pointer to store the handle of the newly created workflow prototype.
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
   mmi_workflow_h workflow = NULL;
   mmi_workflow_create(&workflow);
   ...
   mmi_workflow_destroy(workflow);
 * @endcode
 *
 * @see mmi_workflow_destroy()
 */
int mmi_workflow_create(mmi_workflow_h *workflow);

/**
 * @brief Sets the type of a workflow prototype.
 * @details This function assigns a standard workflow type to the given workflow prototype.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle of the workflow prototype.
 * @param[in] type The standard workflow type to be set.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_standard_workflow_type_e
 * @see mmi_workflow_get_type()
 */
int mmi_workflow_set_type(mmi_workflow_h workflow, mmi_standard_workflow_type_e type);

/**
 * @brief Gets the type of a workflow prototype.
 * @details This function retrieves the standard workflow type assigned to the given workflow prototype.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle of the workflow prototype.
 * @param[out] type A pointer to store the retrieved standard workflow type.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_standard_workflow_type_e
 * @see mmi_workflow_set_type()
 */
int mmi_workflow_get_type(mmi_workflow_h workflow, mmi_standard_workflow_type_e *type);

/**
 * @brief Adds a node to a workflow prototype.
 * @details This function adds a node with a given name to the workflow prototype.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle of the workflow prototype.
 * @param[in] node_name The name of the node to be added.
 * @param[in] node The handle of the node to be added.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_h
 */
int mmi_workflow_node_add(mmi_workflow_h workflow, const char *node_name, mmi_node_h node);

/**
 * @brief Links two nodes in a workflow prototype using their names and port names.
 * @details This function links the output port of one node to the input port of another node.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle of the workflow prototype.
 * @param[in] from_node_name The name of the source node.
 * @param[in] from_port_name The name of the output port of the source node.
 * @param[in] to_node_name The name of the destination node.
 * @param[in] to_port_name The name of the input port of the destination node.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_node_h
 */
int mmi_workflow_link_nodes_by_names(mmi_workflow_h workflow, const char *from_node_name, const char *from_port_name, const char *to_node_name, const char *to_port_name);

/**
 * @brief Assigns an attribute of a workflow to an attribute of a specific node.
 * @details This function assigns a workflow attribute to a target attribute of a specific node in the workflow prototype.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle of the workflow prototype.
 * @param[in] attribute_name The name of the workflow attribute to be assigned.
 * @param[in] target_node_name The name of the target node.
 * @param[in] target_attribute_name The name of the target attribute in the node.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_attribute_h
 */
int mmi_workflow_attribute_assign(mmi_workflow_h workflow, const char *attribute_name, const char *target_node_name, const char *target_attribute_name);

/**
 * @brief Sets the default value of an attribute of a workflow.
 * @details This function sets the default value for a workflow attribute.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle of the workflow prototype.
 * @param[in] default_value The handle of the default value to be set.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mmi_attribute_h
 */
int mmi_workflow_attribute_set_default_value(mmi_workflow_h workflow, mmi_attribute_h default_value);

/**
 * @brief Assigns a signal of a workflow to a signal of a specific node in a workflow prototype.
 * @details This function assigns a signal from a workflow to a target signal of a specific node in the workflow prototype.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle to the workflow.
 * @param[in] signal_name The name of the signal to assign.
 * @param[in] target_node_name The name of the target node where the signal will be assigned.
 * @param[in] target_signal_name The name of the target signal in the target node.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mmi_workflow_h
*/
int mmi_workflow_signal_assign(mmi_workflow_h workflow, const char *signal_name, const char *target_node_name, const char *target_signal_name);

/**
 * @brief Assigns an output of a workflow to a OUT port of a specific node in a workflow prototype.
 * @details This function assigns an output from a workflow to a target OUT port of a specific node in the workflow prototype.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle to the workflow.
 * @param[in] workflow_output The name of the output to assign.
 * @param[in] out_node_name The name of the target node where the output will be assigned.
 * @param[in] node_out_port_name The name of the target OUT port in the target node.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mmi_workflow_h
*/
int mmi_workflow_output_assign(mmi_workflow_h workflow, const char *workflow_output, const char *out_node_name, const char *node_out_port_name);

/**
 * @brief Assigns an output of a workflow to a OUT port of a specific node in a workflow prototype using a port handle.
 * @details This function assigns an output from a workflow to a target OUT port of a specific node in the workflow prototype using a port handle.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle to the workflow.
 * @param[in] workflow_output The name of the output to assign.
 * @param[in] port The handle to the target port where the output will be assigned.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mmi_workflow_h
 * @see mmi_port_h
*/
int mmi_workflow_output_assign_by_port(mmi_workflow_h workflow, const char *workflow_output, mmi_port_h port);

/**
 * @brief Registers a standard workflow prototype to the workflow manager.
 * @details This function registers a standard workflow prototype to the workflow manager for further use.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle to the workflow prototype to register.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mmi_workflow_h
*/
int mmi_standard_workflow_register(mmi_workflow_h workflow);

/**
 * @brief Clones a workflow.
 * @details This function creates a clone of the specified workflow.
 * @since_tizen 9.0
 * @remarks The @a cloned should be released using mmi_workflow_destroy().
 *
 * @param[in] workflow The handle to the workflow to clone.
 * @param[out] cloned A pointer to store the handle of the cloned workflow.
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
   mmi_workflow_h workflow; // Indicates the handle of the workflow
   mmi_workflow_h cloned = NULL;
   mmi_workflow_clone(workflow, &cloned);
   ...
   mmi_workflow_destroy(cloned);
 * @endcode
 *
 * @see mmi_workflow_h
*/
int mmi_workflow_clone(mmi_workflow_h workflow, mmi_workflow_h *cloned);

/**
 * @brief Creates a workflow prototype from a script.
 * @details This function creates a workflow prototype from the provided script.
 * @since_tizen 9.0
 * @remarks The @a workflow should be released using mmi_workflow_destroy().
 *
 * @param[in] script The script to create the workflow prototype from.
 * @param[out] workflow A pointer to store the handle of the created workflow prototype.
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
   mmi_workflow_h workflow = NULL;
   mmi_workflow_create_from_script("@workflow", &workflow);
   ...
   mmi_workflow_destroy(workflow);
 * @endcode
 *
 * @see mmi_workflow_h
*/
int mmi_workflow_create_from_script(const char *script, mmi_workflow_h *workflow);

/**
 * @brief Destroys a workflow prototype.
 * @details This function destroys the specified workflow prototype.
 * @since_tizen 9.0
 *
 * @param[in] workflow The handle to the workflow prototype to destroy.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MMI_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mmi_workflow_h
*/
int mmi_workflow_destroy(mmi_workflow_h workflow);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_WORKFLOW_H__ */
