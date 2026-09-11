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

#ifndef __TIZEN_SOUND_POOL_H__
#define __TIZEN_SOUND_POOL_H__

#include <sound_pool_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file   sound_pool.h
 * @brief  This file contains Tizen Sound Pool API.
 */

/**
 * @addtogroup CAPI_SOUND_POOL_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 10.0
 * @brief Called when the sound pool state is changed.
 *
 * @since_tizen 4.0
 * @remarks @a pool is the object for which the callback was set.
 * @param[in]  pool          The sound pool handle
 * @param[in]  prev_state    Previous pool state
 * @param[in]  cur_state     Current pool state
 * @param[in]  user_data     The user data passed from the code where
 *                           sound_pool_set_state_changed_cb() was
 *                           called.
 *
 * @pre Create sound pool handle by calling sound_pool_create()
 * @pre Call sound_pool_set_state_changed_cb()
 *
 * @see sound_pool_create()
 * @see sound_pool_set_state_changed_cb()
 * @see sound_pool_state_e
 */
typedef void (*sound_pool_state_changed_cb) (sound_pool_h pool,
		sound_pool_state_e prev_state, sound_pool_state_e cur_state,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0
 * @brief Called when the sound pool stream state is changed.
 *
 * @since_tizen 4.0
 * @remarks @a pool is the object for which the callback was set.
 * @param[in]  pool        The sound pool handle
 * @param[in]  id          Unique stream identifier
 * @param[in]  prev_state  Previous stream state
 * @param[in]  cur_state   Current stream state
 * @param[in]  user_data   The user data passed to the callback
 *
 * @pre Create sound pool handle by calling sound_pool_create()
 * @pre Load source to pool by calling sound_pool_load_source_from_file()
 * @pre Start source playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 * @see sound_pool_stream_state_e
 */
typedef void (*sound_pool_stream_state_changed_cb) (sound_pool_h pool,
		unsigned id, sound_pool_stream_state_e prev_state,
		sound_pool_stream_state_e cur_state, void *user_data);

/**
 * @deprecated Deprecated since 10.0
 * @brief Creates a sound pool instance that can be used for sound sources
 *        loading/unloading.
 * @details Up to 8 sound pools can be created. Several pools can be active
 *          at the same time. Streams can be in playing state only when pool is
 *          active. Memory is allocated for sound pool. User should aware that
 *          creation of more number of pools means more memory is allocated.
 * @since_tizen 4.0
 * @remarks When pool has been created, pool state is
 *          #SOUND_POOL_STATE_INACTIVE. To activate the pool use
 *          sound_pool_activate() function.
 *          When no longer needed, @a pool should be destroyed with sound_pool_destroy().
 *
 * @param[out] pool    The created sound pool handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER
 *         Invalid parameter (@a pool is NULL)
 * @retval #SOUND_POOL_ERROR_OUT_OF_MEMORY
 *         Not enough memory to create sound pool
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION
 *         Maximal amount of sound pools is exceeded (maximum 8 pools allowed)
 *
 * @see sound_pool_destroy()
 */
int sound_pool_create(sound_pool_h *pool) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Destroys a sound pool and cleans allocated memory.
 * @details Stops all streams and unloads all sources associated with pool.
 *
 * @since_tizen 4.0
 * @remarks @a pool should be destroyed with sound_pool_destroy().
 * @param[in]  pool     The sound pool handle will be destroyed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER
 *         Invalid parameter (@a pool is NULL or corrupted)
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create sound pool handle by calling sound_pool_create()
 *
 * @see sound_pool_create()
 */
int sound_pool_destroy(sound_pool_h pool) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Loads sound source data from a file to the pool.
 * @details After calling this routine the source can be accessed by its @a tag.
 * @since_tizen 4.0
 * @remarks Input data can be either raw or encoded.
 *          Each of loaded sources must have unique @a tag
 *          It is synchronous operation.
 *
 * @param[in]  pool         The sound pool handle
 * @param[in]  file_name    The name of the file that contains sound data
 * @param[in]  tag          Unique string that will be used to identify source
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER
 *         Invalid parameter (@a pool is NULL or corrupted, @a file_name is
 *         NULL, @a tag is NULL or @a tag/@a file_name length is too long)
 * @retval #SOUND_POOL_ERROR_OUT_OF_MEMORY     Not enough memory to allocate source
 * @retval #SOUND_POOL_ERROR_NO_SUCH_FILE      No file determined by @a file_name
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @post Unload source from the pool by calling sound_pool_unload_source()
 *
 * @see sound_pool_create()
 * @see sound_pool_destroy()
 * @see sound_pool_unload_source()
 */
int sound_pool_load_source_from_file(sound_pool_h pool,
		const char *file_name, const char *tag) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Unloads source from the @a pool.
 * @details Cleans memory. This operation is synchronous.
 * @since_tizen 4.0
 * @remarks The usage of @a tag name that was associated with unloaded source
 *          has no effect. It can be reused as well.
 *
 * @param[in]  pool    The sound pool handle
 * @param[in]  tag     Unique string that identifies the source
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER
 *         Invalid parameter (@a pool is NULL or corrupted, @a tag is NULL)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No sources tagged by @a tag exist
 *         in the pool
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Load source to the pool by calling sound_pool_load_source_from_file()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 */
int sound_pool_unload_source(sound_pool_h pool, const char *tag);

/**
 * @deprecated Deprecated since 10.0
 * @brief Changes the current @a state of a @a pool to #SOUND_POOL_STATE_ACTIVE.
 * @since_tizen 4.0
 * @remarks When activation is performed, all suspended streams with highest
 *          priority in the pool will resume their playback. Paused streams will
 *          remain their state.
 *
 * @param[in]  pool     The sound pool handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted)
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation or sound pool
 *         is already in #SOUND_POOL_STATE_ACTIVE state
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 *
 * @see sound_pool_create()
 * @see sound_pool_get_state()
 * @see sound_pool_deactivate()
 * @see sound_pool_state_e
 */
int sound_pool_activate(sound_pool_h pool) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Changes the current @a state of a @a pool to #SOUND_POOL_STATE_INACTIVE.
 * @since_tizen 4.0
 * @remarks When deactivation is performed, all playing streams in the pool will
 *          be suspended (change state to #SOUND_POOL_STREAM_STATE_SUSPENDED).
 *          Paused streams will remain in their state.
 *
 * @param[in]  pool     The sound pool handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted)
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation or sound pool
 *         is already in #SOUND_POOL_STATE_INACTIVE state
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre @a pool has to be in #SOUND_POOL_STATE_ACTIVE state
 *
 * @see sound_pool_create()
 * @see sound_pool_get_state()
 * @see sound_pool_activate()
 * @see sound_pool_state_e
 */
int sound_pool_deactivate(sound_pool_h pool) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Sets the pool's global volume parameter.
 * @details Volume of all streams related to the @a pool will be scaled
 *          in accordance to global pool volume parameter
 *          (i.e. [stream real volume] = [global volume] * [stream volume],
 *          where [stream volume] is the volume parameter of arbitrary stream).
 *
 * @since_tizen 4.0
 * @param[in]  pool      The sound pool handle
 * @param[in]  volume    Pool global volume in 0.0~1.0 range
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, or @a volume isn't in 0.0~1.0 range)
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 *
 * @see sound_pool_create()
 */
int sound_pool_set_volume(sound_pool_h pool, float volume) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the pool's global volume parameter.
 *
 * @since_tizen 4.0
 * @param[in]   pool      The sound pool handle
 * @param[out]  volume    Pool global volume in 0.0~1.0 range
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, or @a volume is NULL)
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 *
 * @see sound_pool_create()
 */
int sound_pool_get_volume(sound_pool_h pool, float *volume) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the current @a state of @a pool.
 *
 * @since_tizen 4.0
 * @param[in]  pool     The sound pool handle
 * @param[out] state    Current state of @a pool
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER
 *         Invalid parameter (@a pool is NULL or corrupted, @a tag is NULL)
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 *
 * @see sound_pool_create()
 * @see sound_pool_state_e
 */
int sound_pool_get_state(sound_pool_h pool,
		sound_pool_state_e *state) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Sets the callback for handling sound @a pool state change.
 *
 * @since_tizen 4.0
 * @param[in]  pool        The sound pool handle
 * @param[in]  callback    The callback that will be called after pool state
 *                         will be changed.  @a callback will be called
 *                         synchronously
 * @param[in]  user_data   The user data to be passed to the @a callback
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, or @a callback is NULL)
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @post Use sound_pool_unset_state_changed_cb() function to unset the
 *       @a callback
 *
 * @see sound_pool_create()
 * @see sound_pool_state_changed_cb()
 */
int sound_pool_set_state_changed_cb(sound_pool_h pool,
		sound_pool_state_changed_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Unsets the callback for handling sound @a pool state change.
 *
 * @since_tizen 4.0
 * @param[in]  pool     The sound pool handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted)
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Set state change callback by calling
 *      sound_pool_set_state_changed_cb()
 *
 * @see sound_pool_create()
 * @see sound_pool_set_state_changed_cb()
 */
int sound_pool_unset_state_changed_cb(sound_pool_h pool) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Plays a source by @a tag.
 * @details Creates stream with @a id that can be used to change parameters and
 *          get additional information.
 *          Sets stream state to #SOUND_POOL_STREAM_STATE_PLAYING.
 *          If a callback is set with sound_pool_stream_play(), and another
 *          callback is set with sound_pool_stream_set_state_changed_cb(),
 *          the second callback will overwrite the first one.
 * @since_tizen 4.0
 * @remarks Resultant stream volume will depend on global pool volume.
 *
 * @param[in]  pool        The sound pool handle
 * @param[in]  tag         Unique string that identifies source
 * @param[in]  loop        Number of times stream will be repeated
 *                         (pass 0 if stream should be repeated continuously)
 * @param[in]  volume      Stream volume in 0.0~1.0 range
 * @param[in]  priority    Stream priority (0 = lowest priority). Check
 *                         sound_pool_stream_set_priority() documentation
 *                         for details on prioritization rules.
 * @param[in]  priority_policy    Stream priority policy.\n
 *                         (#SOUND_POOL_STREAM_PRIORITY_POLICY_MUTE - Mute)\n
 *                         (#SOUND_POOL_STREAM_PRIORITY_POLICY_SUSPENDED - Suspended)\n
 *                         If priority_policy is set to Mute, the stream will be
 *                         playing with mute even in the presence of high priority
 *                         stream and it will not be Suspended.
 * @param[in]  callback    The callback that will be called after stream state
 *                         will be changed, or NULL if this callback call
 *                         isn't needed. If @a callback is set, then it will
 *                         be called asynchronously.
 * @param[in]  user_data   The user data to be passed to the @a callback
 * @param[out] id          Unique stream identifier that can be used to
 *                         change parameters and get additional information
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, @a tag is NULL, @a volume is out of
 *         0.0~1.0 range, or @a id is NULL)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No sources tagged by @a tag exist
 *         in the pool
 * @retval #SOUND_POOL_ERROR_OUT_OF_MEMORY Not enough memory to allocate new
 *         sound stream
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create sound pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @post When playback is finished normally (i.e. sound_pool_stream_stop()
 *       will be not used for stream termination) state will be changed to
 *       #SOUND_POOL_STREAM_STATE_FINISHED and memory will be cleared from the
 *       stream allocated resources automatically
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_set_volume()
 * @see sound_pool_get_volume()
 */
int sound_pool_stream_play(sound_pool_h pool, const char *tag,
		unsigned loop, float volume, unsigned priority,
		sound_pool_stream_priority_policy_e priority_policy,
		sound_pool_stream_state_changed_cb callback, void *user_data,
		unsigned *id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Pauses a stream by @a id.
 * @details Sets stream state to #SOUND_POOL_STREAM_STATE_PAUSED.
 * @since_tizen 4.0
 * @remarks Stream state has to be #SOUND_POOL_STREAM_STATE_PLAYING.
 *
 * @param[in]  pool    The sound pool handle
 * @param[in]  id      Unique stream identifier
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation or stream is in
 *         the state which is not allowed for pause operation
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 */
int sound_pool_stream_pause(sound_pool_h pool, unsigned id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Resumes a stream by @a id.
 * @details Sets stream state to #SOUND_POOL_STREAM_STATE_PLAYING.
 * @since_tizen 4.0
 * @remarks Stream state has to be #SOUND_POOL_STREAM_STATE_PAUSED.
 *
 * @param[in]  pool    The sound pool handle
 * @param[in]  id      Unique stream identifier
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation or stream is in
 *         the state which is not allowed for resume operation
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 * @pre Pause stream playback by calling sound_pool_stream_pause()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 * @see sound_pool_stream_pause()
 */
int sound_pool_stream_resume(sound_pool_h pool, unsigned id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Stops a stream by @a id.
 * @details Sets stream state to #SOUND_POOL_STREAM_STATE_STOPPED.
 *          After stream was stopped it can not be resumed and @a id value
 *          becomes invalid. Moreover, stream will never gets
 *          #SOUND_POOL_STREAM_STATE_FINISHED state as it will be terminated
 *          after this function call.
 *
 * @since_tizen 4.0
 * @param[in]  pool    The sound pool handle
 * @param[in]  id      Unique stream identifier
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation or stream is in
 *         the state which is not allowed for stop operation
 *
 * @pre Create sound pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 */
int sound_pool_stream_stop(sound_pool_h pool, unsigned id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Sets the stream's volume parameters by stream @a id.
 * @since_tizen 4.0
 * @remarks Resultant stream volume will depend on global pool volume.
 *
 * @param[in]  pool      The sound pool handle
 * @param[in]  id        Unique stream identifier
 * @param[in]  volume    Stream volume in 0.0~1.0 range
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, or @a volume isn't in 0.0~1.0 range)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 * @see sound_pool_set_volume
 * @see sound_pool_get_volume
 */
int sound_pool_stream_set_volume(sound_pool_h pool, unsigned id,
		float volume) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the stream's volume parameters by stream @a id.
 *
 * @since_tizen 4.0
 * @param[in]  pool      The sound pool handle
 * @param[in]  id        Unique stream identifier
 * @param[out] volume    Stream volume in 0.0~1.0 range
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, or @a volume is NULL)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 */
int sound_pool_stream_get_volume(sound_pool_h pool, unsigned id,
		float *volume) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Sets the stream's priority parameter by stream @a id.
 * @since_tizen 4.0
 * @details The below rules are applicable for streams with priority policy
 *          #SOUND_POOL_STREAM_PRIORITY_POLICY_SUSPENDED.
 *          Only streams with highest priority in the active Sound Pool can be
 *          played. If at least one stream with highest priority enters
 *          #SOUND_POOL_STREAM_STATE_PLAYING state, then all streams currently
 *          being played with lower priorities will be transferred to the
 *          #SOUND_POOL_STREAM_STATE_SUSPENDED state. If there are no more
 *          playing streams in a given priority group (group of streams with the
 *          same priority), then all previously suspended streams in the group
 *          with one level lower priority will be resumed.
 *
 *          For example, if we have N suspended streams in 'LOW' group, M
 *          suspended streams in 'MEDIUM' group and K playing streams in 'HIGH'
 *          group. Then the following rules are valid: When all K streams in HIGH
 *          priority group will be finished, paused, or stopped and Sound Pool is
 *          in active state, only then M streams from MEDIUM priority group will
 *          be resumed. When all M+K streams from HIGH and MEDIUM priority groups
 *          are finished, paused or stopped and Sound Pool is in active state,
 *          only then N streams from LOW priority group will be resumed.
 *          Transferring at least one stream from higher priority group will lead
 *          to suspending of all the streams from lower priorities group(s).
 *
 *          Priorities don't take any effect in inactive Sound Pools. But after
 *          pool activation, all mentioned rules will be applied.
 *
 *          If stream priority policy is set to mute
 *          #SOUND_POOL_STREAM_PRIORITY_POLICY_MUTE, all above rules are not valid
 *          for the stream and it will be played with mute even in the presence
 *          of high priority steams.
 *          streams.
 * @param[in]  pool        The sound pool handle
 * @param[in]  id          Unique stream identifier
 * @param[in]  priority    Stream priority (0 = lowest priority)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 * @retval #SOUND_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 */
int sound_pool_stream_set_priority(sound_pool_h pool, unsigned id,
		unsigned priority) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the stream's priority parameter by stream @a id.
 *
 * @since_tizen 4.0
 * @param[in]   pool        The sound pool handle
 * @param[in]   id          Unique stream identifier
 * @param[out]  priority    Stream priority (0 = lowest priority). Check
 *                          sound_pool_stream_set_priority() documentation
 *                          for details on prioritization rules.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, or @a priority is NULL)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 *
 * @pre Create a sound @a pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 */
int sound_pool_stream_get_priority(sound_pool_h pool, unsigned id,
		unsigned *priority) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the current @a state of stream by @a id.
 *
 * @since_tizen 4.0
 * @param[in]  pool     The sound pool handle
 * @param[in]  id       Unique stream identifier
 * @param[out] state    Current state of stream
 * @return @c 0 on success, otherwise a negative error value
 * @retval #SOUND_POOL_ERROR_NONE              Successful
 * @retval #SOUND_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 *         (@a pool is NULL or corrupted, or @a state is NULL)
 * @retval #SOUND_POOL_ERROR_KEY_NOT_AVAILABLE No streams identified by @a id
 *         exist in the pool
 *
 * @pre Create sound pool handle by calling sound_pool_create()
 * @pre Load source to @a pool by calling sound_pool_load_source_from_file()
 * @pre Start stream playback by calling sound_pool_stream_play()
 *
 * @see sound_pool_create()
 * @see sound_pool_load_source_from_file()
 * @see sound_pool_stream_play()
 * @see sound_pool_stream_state_e
 */
int sound_pool_stream_get_state(sound_pool_h pool, unsigned id,
		sound_pool_stream_state_e *state) TIZEN_DEPRECATED_API;
/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SOUND_POOL_H__ */
