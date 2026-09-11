/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_SOUND_POOL_TYPE_H__
#define __TIZEN_SOUND_POOL_TYPE_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file  sound_pool_type.h
 * @brief This file contains handles, enumerations and callbacks types
 *        definitions required by Tizen SoundPool API.
 */

/**
 * @addtogroup CAPI_SOUND_POOL_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration for Tizen Sound Pool error.
 *
 * @since_tizen 4.0
 */
typedef enum {
	SOUND_POOL_ERROR_NONE
			= TIZEN_ERROR_NONE,                /**< Successful */
	SOUND_POOL_ERROR_KEY_NOT_AVAILABLE
			= TIZEN_ERROR_KEY_NOT_AVAILABLE,   /**< Key not available */
	SOUND_POOL_ERROR_OUT_OF_MEMORY
			= TIZEN_ERROR_OUT_OF_MEMORY,       /**< Out of memory */
	SOUND_POOL_ERROR_INVALID_PARAMETER
			= TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	SOUND_POOL_ERROR_INVALID_OPERATION
			= TIZEN_ERROR_INVALID_OPERATION,   /**< Invalid operation */
	SOUND_POOL_ERROR_NOT_PERMITTED
			= TIZEN_ERROR_NOT_PERMITTED,       /**< Not permitted */
	SOUND_POOL_ERROR_NO_SUCH_FILE
			= TIZEN_ERROR_NO_SUCH_FILE,        /**< File not found */

} sound_pool_error_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief Sound pool handle type.
 *
 * @since_tizen 4.0
 */
typedef void *sound_pool_h;

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration of sound pool stream state.
 *
 * @since_tizen 4.0
 */
typedef enum {
	SOUND_POOL_STREAM_STATE_NONE,       /**< Stream state isn't determined */
	SOUND_POOL_STREAM_STATE_PLAYING,    /**< Stream state is playing */
	SOUND_POOL_STREAM_STATE_PAUSED,     /**< Stream state is paused */
	SOUND_POOL_STREAM_STATE_STOPPED,    /**< Stream state is stopped */
	SOUND_POOL_STREAM_STATE_FINISHED,   /**< Stream state is finished */
	SOUND_POOL_STREAM_STATE_SUSPENDED   /**< Stream state is suspended */
} sound_pool_stream_state_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration of sound pool stream priority policy.
 *
 * @since_tizen 4.0
 */
typedef enum {
	SOUND_POOL_STREAM_PRIORITY_POLICY_MUTE,           /**< Stream priority policy is mute */
	SOUND_POOL_STREAM_PRIORITY_POLICY_SUSPENDED       /**< Stream priority policy is suspended */
} sound_pool_stream_priority_policy_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration of sound pool state.
 *
 * @since_tizen 4.0
 */
typedef enum {
	SOUND_POOL_STATE_ACTIVE,    /**< Sound pool active state: streams can be played */
	SOUND_POOL_STATE_INACTIVE   /**< Sound pool inactive state: streams can't be played */
} sound_pool_state_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_SOUND_POOL_TYPE_H__ */
