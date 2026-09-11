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


#ifndef __TIZEN_UIX_MMI_NODE_TYPES_H__
#define __TIZEN_UIX_MMI_NODE_TYPES_H__



#include <mmi-node.h>

/**
* @file mmi-node-types.h
*/


/**
* @addtogroup CAPI_UIX_MMI_NODE_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Enumeration for MMI source node type.
 * @details This enumeration defines the different source node types supported by the MMI framework.
 * @since_tizen 9.0
 */
typedef enum {
    MMI_NODE_SOURCE_TYPE_NONE,                 /**< Indicates that there is no source node type. */
    MMI_NODE_SOURCE_TYPE_SCREEN_ANALYZER,      /**< Represents a screen analyzer node type. */
} mmi_node_source_type_e;

/**
 * @brief Enumeration for MMI processor node type.
 * @details This enumeration defines the different processor node types supported by the MMI framework.
 * @since_tizen 9.0
 */
typedef enum {
    MMI_NODE_PROCESSOR_TYPE_NONE               /**< Indicates that there is no processor node type. */
} mmi_node_processor_type_e;

/**
 * @brief Enumeration for MMI logic node type.
 * @details This enumeration defines the different logic node types supported by the MMI framework.
 * @since_tizen 9.0
 */
typedef enum {
    MMI_NODE_LOGIC_TYPE_NONE,                  /**< Indicates that there is no logic node type. */
    MMI_NODE_LOGIC_TYPE_FIXED_STRING_MATCH,    /**< Represents a fixed string match logic node type. */
} mmi_node_logic_type_e;

/**
 * @brief Enumeration for MMI controller node type.
 * @details This enumeration defines the different controller node types supported by the MMI framework.
 * @since_tizen 9.0
 */
typedef enum {
    MMI_NODE_CONTROLLER_TYPE_NONE,             /**< Indicates that there is no controller node type. */
} mmi_node_controller_type_e;

/**
 * @brief Enumeration for MMI action node type.
 * @details This enumeration defines the different action node types supported by the MMI framework.
 * @since_tizen 9.0
 */
typedef enum {
    MMI_NODE_ACTION_TYPE_NONE,                 /**< Indicates that there is no action node type. */
    MMI_NODE_ACTION_TYPE_MOUSE_EVENT,          /**< Represents a mouse event action node type. */
    MMI_NODE_ACTION_TYPE_KEY_EVENT,            /**< Represents a key event action node type. */
} mmi_node_action_type_e;

/**
 * @brief Creates a new source node.
 * @details This function creates a new source node of the specified type and returns a handle to it.
 * @since_tizen 9.0
 * @remarks The @a node should be released using mmi_node_destroy().
 *
 * @param[in] type The type of the source node to be created.
 * @param[out] node A pointer to store the handle of the newly created source node.
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
   mmi_node_h node = NULL;
   mmi_node_create_source(MMI_NODE_SOURCE_TYPE_NONE, &node);
   ...
   mmi_node_destroy(node);
 * @endcode
 *
 * @see mmi_node_source_type_e
 * @see mmi_node_destroy()
 */
int mmi_node_create_source(mmi_node_source_type_e type, mmi_node_h *node);

/**
 * @brief Gets the type of the source node.
 * @details This function retrieves the type of the specified source node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the source node.
 * @param[out] type A pointer to store the type of the source node.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The source node must be created using mmi_node_create_source().
 *
 * @see mmi_node_source_type_e
 * @see mmi_node_create_source()
 */
int mmi_node_get_source_type(mmi_node_h node, mmi_node_source_type_e *type);

/**
 * @brief Creates a new processor node.
 * @details This function creates a new processor node of the specified type and returns a handle to it.
 * @since_tizen 9.0
 * @remarks The @a node should be released using mmi_node_destroy().
 *
 * @param[in] type The type of the processor node to be created.
 * @param[out] node A pointer to store the handle of the newly created processor node.
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
   mmi_node_h node = NULL;
   mmi_node_create_processor(MMI_NODE_PROCESSOR_TYPE_NONE, &node);
   ...
   mmi_node_destroy(node);
 * @endcode

 * @see mmi_node_processor_type_e
 * @see mmi_node_destroy()
 */
int mmi_node_create_processor(mmi_node_processor_type_e type, mmi_node_h *node);

/**
 * @brief Gets the type of the processor node.
 * @details This function retrieves the type of the specified processor node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the processor node.
 * @param[out] type A pointer to store the type of the processor node.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The processor node must be created using mmi_node_create_processor().
 *
 * @see mmi_node_processor_type_e
 * @see mmi_node_create_processor()
 */
int mmi_node_get_processor_type(mmi_node_h node, mmi_node_processor_type_e *type);

/**
 * @brief Creates a new logic node.
 * @details This function creates a new logic node of the specified type and returns a handle to it.
 * @since_tizen 9.0
 * @remarks The @a node should be released using mmi_node_destroy().
 *
 * @param[in] type The type of the logic node to be created.
 * @param[out] node A pointer to store the handle of the newly created logic node.
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
   mmi_node_h node = NULL;
   mmi_node_create_logic(MMI_NODE_LOGIC_TYPE_NONE, &node);
   ...
   mmi_node_destroy(node);
 * @endcode

 * @see mmi_node_logic_type_e
 * @see mmi_node_destroy()
 *
 */
int mmi_node_create_logic(mmi_node_logic_type_e type, mmi_node_h *node);

/**
 * @brief Gets the type of the logic node.
 * @details This function retrieves the type of the specified logic node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the logic node.
 * @param[out] type A pointer to store the type of the logic node.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The logic node must be created using mmi_node_create_logic().
 *
 * @see mmi_node_logic_type_e
 * @see mmi_node_create_logic()
 */
int mmi_node_get_logic_type(mmi_node_h node, mmi_node_logic_type_e *type);

/**
 * @brief Creates a new MMI controller node.
 * @details This function creates a new MMI controller node of the specified type.
 * @since_tizen 9.0
 * @remarks The @a node should be released using mmi_node_destroy().
 *
 * @param[in] type The type of the controller to be created.
 * @param[out] node A pointer to the newly created node handle.
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
   mmi_node_h node = NULL;
   mmi_node_create_controller(MMI_NODE_CONTROLLER_TYPE_NONE, &node);
   ...
   mmi_node_destroy(node);
 * @endcode
 */
int mmi_node_create_controller(mmi_node_controller_type_e type, mmi_node_h *node);

/**
 * @brief Gets the type of the MMI controller node.
 * @details This function retrieves the type of the specified MMI controller node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the node controller.
 * @param[out] type A pointer to store the type of the node controller.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_node_get_controller_type(mmi_node_h node, mmi_node_controller_type_e *type);

/**
 * @brief Creates a new MMI action node.
 * @details This function creates a new MMI action node of the specified type.
 * @since_tizen 9.0
 * @remarks The @a node should be released using mmi_node_destroy().
 *
 * @param[in] type The type of the action to be created.
 * @param[out] node A pointer to the newly created node handle.
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
   mmi_node_h node = NULL;
   mmi_node_create_action(MMI_NODE_ACTION_TYPE_NONE, &node);
   ...
   mmi_node_destroy(node);
 * @endcode
 */
int mmi_node_create_action(mmi_node_action_type_e type, mmi_node_h *node);

/**
 * @brief Gets the type of the MMI action node.
 * @details This function retrieves the type of the specified MMI action node.
 * @since_tizen 9.0
 *
 * @param[in] node The handle of the node action.
 * @param[out] type A pointer to store the type of the node action.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MMI_ERROR_NONE Successful
 * @retval #MMI_ERROR_NOT_SUPPORTED Not supported
 * @retval #MMI_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mmi_node_get_action_type(mmi_node_h node, mmi_node_action_type_e *type);

/**
 * @brief Creates a new MMI node with a custom type.
 * @details This function creates a new MMI node with a custom type identifier.
 * @since_tizen 9.0
 * @remarks The @a node should be released using mmi_node_destroy().
 *
 * @param[in] custom_type_id The custom type identifier for the node.
 * @param[out] node A pointer to the newly created node handle.
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
   mmi_node_h node = NULL;
   mmi_node_create_custom("Test", &node);
   ...
   mmi_node_destroy(node);
 * @endcode
 */
int mmi_node_create_custom(const char *custom_type_id, mmi_node_h *node);

/**
 * @brief Gets the custom type identifier of the MMI node.
 * @details This function retrieves the custom type identifier of the specified MMI node.
 * @since_tizen 9.0
 * @remarks The @a custom_type_id should not be released.
 * @remarks The @a custom_type_id is available until node is released.
 *
 * @param[in] node The handle of the node.
 * @param[out] custom_type_id A pointer to store the custom type identifier of the node.
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
   mmi_node_h node; // Indicates the handle of the node
   const char *custom_type_id = NULL;
   mmi_node_get_custom_type(node, &custom_type_id);
 * @endcode
 */
int mmi_node_get_custom_type(mmi_node_h node, const char **custom_type_id);

#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_NODE_TYPES_H__ */
