/*
* Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_BRIDGE_INTERNAL_H__
#define __TIZEN_MEDIA_BRIDGE_INTERNAL_H__

#include <stdint.h>
#include <tizen.h>
#include <media_packet.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_bridge_internal.h
 * @brief This file contains the media bridge internal API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_MEDIA_BRIDGE_INTERNAL_MODULE
 * @{
 */

/**
 * @internal
 * @brief Media Bridge handle type.
 * @since_tizen 6.5
 */
typedef struct media_bridge_s *media_bridge_h;

/**
 * @internal
 * @brief Enumerations of media bridge error.
 * @since_tizen 6.5
 */
typedef enum {
	MEDIA_BRIDGE_ERROR_NONE = TIZEN_ERROR_NONE,                             /**< Successful */
	MEDIA_BRIDGE_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,           /**< Out of memory */
	MEDIA_BRIDGE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	MEDIA_BRIDGE_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,   /**< Invalid operation */
	MEDIA_BRIDGE_ERROR_INVALID_STATE = TIZEN_ERROR_MEDIA_TOOL | 0x11,       /**< Invalid state */
	MEDIA_BRIDGE_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,           /**< The feature is not supported */
} media_bridge_error_e;

/**
 * @internal
 * @brief Enumeration for the media bridge state.
 * @since_tizen 6.5
 */
typedef enum {
	MEDIA_BRIDGE_STATE_NONE,          /**< None */
	MEDIA_BRIDGE_STATE_CREATED,       /**< Created */
	MEDIA_BRIDGE_STATE_STREAMING      /**< Now streaming */
} media_bridge_state_e;

/**
 * @internal
 * @brief Enumeration for source of sink module of the media bridge.
 * @since_tizen 6.5
 */
typedef enum {
	MEDIA_BRIDGE_MODULE_CAMERA,     /**< Media camera */
	MEDIA_BRIDGE_MODULE_CODEC,      /**< Media codec */
	MEDIA_BRIDGE_MODULE_PLAYER,     /**< Media player */
	MEDIA_BRIDGE_MODULE_VISION,     /**< Media vision */
	MEDIA_BRIDGE_MODULE_WEBRTC,     /**< Media webrtc */
	MEDIA_BRIDGE_MODULE_NUM
} media_bridge_module_e;


/**
 * @internal
 * @brief The function prototype to set media bridge for source module.
 * @since_tizen 6.5
 * @remarks The source module should have the implementation for this function.
 * @param[in] module_handle The handle to the module
 * @param[in] bridge The handle to the media bridge
 * @see media_bridge_set_source()
 */
typedef int (*module_media_bridge_set_bridge_func)(void *module_handle, media_bridge_h bridge);

/**
 * @internal
 * @brief The function prototype to unset media bridge for source module.
 * @since_tizen 6.5
 * @remarks The source module should have the implementation for this function.
 * @param[in] module_handle The handle to the module
 * @see media_bridge_unset_source()
 */
typedef int (*module_media_bridge_unset_bridge_func)(void *module_handle);

/**
 * @internal
 * @brief The function prototype to push media packet to sink module.
 * @since_tizen 6.5
 * @remarks The sink module should have the implementation for this function.
 * @param[in] module_handle The handle to the module
 * @param[in] bridge The handle to the media bridge
 * @param[in] sink_id The id of the added sink
 * @param[in] packet The media packet from source module
 * @see media_bridge_unset_source()
 */
typedef int (*module_media_bridge_push_packet_func)(void *module_handle, media_bridge_h bridge, int sink_id, media_packet_h packet);


/**
 * @internal
 * @brief Creates a new media bridge handle.
 * @since_tizen 6.5
 * @param[out] bridge A newly returned handle to the media bridge
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @post If it succeeds, the state will be #MEDIA_BRIDGE_STATE_CREATED.
 * @see media_bridge_destroy()
 */
int media_bridge_create(media_bridge_h *bridge);

/**
 * @internal
 * @brief Destroys the media bridge handle and releases all its resources.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_CREATED.
 * @see media_bridge_create()
 */
int media_bridge_destroy(media_bridge_h bridge);

/**
 * @internal
 * @brief Sets the source module for media bridge.
 * @since_tizen 6.5
 * @remarks This function should be called before streaming(see media_bridge_start()).
 * @param[in] bridge The handle to the media bridge
 * @param[in] module The module that provides buffers
 * @param[in] module_handle The handle to the module
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_BRIDGE_ERROR_NOT_SUPPORTED The feature is not supported
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_CREATED.
 * @see media_bridge_create()
 * @see media_bridge_unset_source()
 */
int media_bridge_set_source(media_bridge_h bridge, media_bridge_module_e module, void *module_handle);

/**
 * @internal
 * @brief Unsets the source module for media bridge.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_OPERATION Invalid operation
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_CREATED.
 * @see media_bridge_set_source()
 */
int media_bridge_unset_source(media_bridge_h bridge);

/**
 * @internal
 * @brief Adds the sink module for media bridge.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @param[in] module The module that consumes buffers
 * @param[in] module_handle The handle to the module
 * @param[out] sink_id The id of the added sink
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_BRIDGE_ERROR_NOT_SUPPORTED The feature is not supported
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_CREATED.
 * @see media_bridge_create()
 * @see media_bridge_remove_sink()
 */
int media_bridge_add_sink(media_bridge_h bridge, media_bridge_module_e module, void *module_handle, int *sink_id);

/**
 * @internal
 * @brief Removes the sink module for media bridge.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @param[in] sink_id The id of the added sink
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_CREATED.
 * @see media_bridge_create()
 * @see media_bridge_add_sink()
 */
int media_bridge_remove_sink(media_bridge_h bridge, int sink_id);

/**
 * @internal
 * @brief Starts the streaming of media bridge.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_OPERATION Invalid operation
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_CREATED.
 * @pre The source module should be set through media_bridge_set_source() \n
        and the sink module should be added through media_bridge_add_sink().
 * @post If it succeeds, the state will be #MEDIA_BRIDGE_STATE_STREAMING and \n
 *       buffers will be delivered from source module to sink modules automatically.
 * @see media_bridge_stop()
 */
int media_bridge_start(media_bridge_h bridge);

/**
 * @internal
 * @brief Stops the streaming of media bridge.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_STREAMING.
 * @post If it succeeds, the state will be #MEDIA_BRIDGE_STATE_CREATED and \n
 *       it's stopped to deliver buffers from source module to sink modules.
 * @see media_bridge_start()
 */
int media_bridge_stop(media_bridge_h bridge);

/**
 * @internal
 * @brief Gets the state of media bridge.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @param[out] state The current state of the media bridge
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_bridge_get_state(media_bridge_h bridge, media_bridge_state_e *state);

/**
 * @internal
 * @brief Pushes media packet to media bridge.
 * @since_tizen 6.5
 * @param[in] bridge The handle to the media bridge
 * @param[in] packet The handle to the media packet
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_BRIDGE_ERROR_NONE Successful
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_STATE Invalid state
 * @retval #MEDIA_BRIDGE_ERROR_INVALID_OPERATION Invalid operation
 * @pre The state must be set to #MEDIA_BRIDGE_STATE_STREAMING.
 * @see media_bridge_start()
 */
int media_bridge_push_packet(media_bridge_h bridge, media_packet_h packet);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_BRIDGE_INTERNAL_H__ */
