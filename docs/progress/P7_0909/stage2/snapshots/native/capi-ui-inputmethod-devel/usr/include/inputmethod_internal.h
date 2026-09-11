/*
 * Copyright (c) 2014-2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_INPUTMETHOD_INTERNAL_H__
#define __TIZEN_UIX_INPUTMETHOD_INTERNAL_H__

/**
 * @file inputmethod_internal.h
 * @brief This file contains input method internal APIs and related enumeration.
 */

#include <tizen.h>
#include <Ecore_Wl2.h>
#include <sclcore.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for input method optimization hint.
 *
 * @since_tizen 5.0
 */
typedef enum {
	IME_OPTIMIZATION_HINT_NONE = 0, /**< No hint provided */
	IME_OPTIMIZATION_HINT_SHOW_PREPARE, /**< This IME is going to be displayed on screen soon */
} ime_optimization_hint_e;

typedef struct _ime_context *ime_context_h;

/**
 * @brief Called when an caps mode is changed.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] mode caps mode
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_caps_mode_changed_cb)(int mode, void *user_data);

/**
 * @brief Called when a candidate list provided by IMEngine should be shown.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_candidate_show_cb)(int context_id, void *user_data);

/**
 * @brief Called when a candidate list provided by IMEngine should be hidden.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_candidate_hide_cb)(int context_id, void *user_data);

/**
 * @brief Called when a candidate list provided by IMEngine is changed.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a list should not be released.
 *
 * @param[in] list candidate list
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_lookup_table_changed_cb)(GList *list, void *user_data);

/**
 * @brief Called when a optimization hint value is set.
 *
 * @since_tizen 5.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] hint The hint value used for optimization
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_optimization_hint_set_cb)(ime_optimization_hint_e hint, void *user_data);

/**
 * @brief Called when a key event is received from external devices or ime_send_key_event().
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key The key event of the keyboard
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_process_key_event_with_imengine_cb)(scim::KeyEvent &key, uint32_t serial, void *user_data);

/**
 * @brief Called when a autocapital type is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] type The autocapital type to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_autocapital_type_set_cb)(uint32_t type, void *user_data);

/**
 * @brief Called when a prediction allow is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] prediction_allow The prediction allow to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_prediction_allow_set_cb)(uint32_t prediction_allow, void *user_data);

/**
 * @brief Called when a trigger property value is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] property The trigger property
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_trigger_property_set_cb)(const char *property, void *user_data);

/**
 * @brief Called when a candidate more window is shown.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_candidate_more_window_show_cb)(void *user_data);

/**
 * @brief Called when a candidate more window is hidden.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_candidate_more_window_hide_cb)(void *user_data);

/**
 * @brief Called when an AUX item is selected.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] item The AUX item
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_aux_select_cb)(uint32_t item, void *user_data);

/**
 * @brief Called when a candidate item is selected.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] item The cadidate item
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_candidate_select_cb)(uint32_t item, void *user_data);

/**
 * @brief Called when a candidate table page is up.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_candidate_table_page_up_cb)(void *user_data);

/**
 * @brief Called when a candidate table page is down.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_candidate_table_page_down_cb)(void *user_data);

/**
 * @brief Called when a page size of the candidate window is changed.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] size The page size of the candidate window
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_candidate_table_page_size_chaned_cb)(uint32_t size, void *user_data);

/**
 * @brief Called when a candidate item layout is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] item The candidate item layout
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*ime_candidate_item_layout_set_cb)(std::vector<uint32_t> item, void *user_data);

/**
 * @brief Called when a displayed candidate number is changed.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] page_num The displayed candidate number
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_displayed_candidate_number_chaned_cb)(uint32_t page_num, void *user_data);

/**
 * @brief Called when the candidate item is long pressed.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] index The index of the candidate item
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*ime_candidate_item_long_pressed_cb)(uint32_t index, void *user_data);

/**
 * @brief Sets the event callback function that is called when a caps mode is changed.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_caps_mode_changed_cb() callback function is called when an associated text input
 * UI control sends the change of caps mode.
 *
 * @param[in] callback_func @c ime_caps_mode_changed_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_caps_mode_changed_cb(ime_caps_mode_changed_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate list provided by IMEngine should be shown.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_show_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_show_cb(ime_candidate_show_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate list provided by IMEngine should be hidden.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_hide_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_hide_cb(ime_candidate_hide_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate list provided by IMEngine is changed.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_lookup_table_changed_cb() callback function is called when an associated text input
 * UI control sends the change of caps mode.
 *
 * @param[in] callback_func @c ime_lookup_table_changed_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_lookup_table_changed_cb(ime_lookup_table_changed_cb callback_func, void *user_data);

/**
 * @brief Gets the caps mode information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the caps mode information in ime_show_cb() callback function.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] caps_mode Caps mode information \n @c true to turn on shift mode
 * text feature if available, @c false to disable the predictive text feature
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 */
int ime_context_get_caps_mode(ime_context_h context, bool *caps_mode);

/**
 * @brief Sets keyboard engine.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] engine_id The engine UUID
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 */
int ime_set_imengine(const char *engine_id);

/**
 * @brief Flushes state in Input Method engine.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 */
int ime_flush_imengine(void);

/**
 * @brief Resets state in Input Method engine.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 */
int ime_reset_imengine(void);

/**
 * @brief Sends an Event to IMEngine.
 *
 * @since_tizen 5.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] command The command to be sent.
 * @param[in] value The value corresponding to the command.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_send_imengine_event(int command, unsigned int value);

/**
 * @brief Sends the selected item index in the candidate list.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] index the selected index in the candidate list
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 */
int ime_select_candidate(unsigned int index);

/**
 * @brief Sends the input context event.
 *
 * @since_tizen 3.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] type the event type
 * @param[in] value the event value
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 */
int ime_update_input_context(unsigned int type, unsigned int value);

/**
 * @brief Requests IME to initialize explicitly.
 *
 * @details When using ime_run() API, the initialize / prepare / finalize procedures
 * are processed automatically inside the IME application loop.
 * But in case of not using ime_run() API, which means the IME application has
 * its own main loop, these procedures need to be requested explicitly.
 *
 * @since_tizen 4.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_initialize(void);

/**
 * @brief Requests IME to prepare resources such as IME window and socket connection.
 *
 * @details Like ime_initialize() function, this procedure is automatically processed
 * when using ime_run() API. Call this function only when ime_run() is not used and
 * the IME application has to handle main loop by itself.
 *
 * @since_tizen 4.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 */
int ime_prepare(void);

/**
 * @brief Requests IME to finalize explicitly.
 *
 * @since_tizen 4.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_finalize(void);

/**
 * @brief Sets flag whether IME is called from dotnet.
 *
 * @since_tizen 4.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] set Set with Dotnet mode
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_set_dotnet_flag(bool set);

/**
 * @brief Sets flag whether creating IME window should be deferred until there is a access request.
 *
 * @since_tizen 5.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] flag true if deferring window creation is desired
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_set_window_creation_defer_flag(bool flag);

/**
 * @brief Sets the event callback function that is called when a optimization hint value is set.
 *
 * @since_tizen 5.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_optimization_hint_set_cb() callback function is called when an optimization hint is set
 *
 * @param[in] callback_func @c ime_optimization_hint_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_optimization_hint_set_cb(ime_optimization_hint_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a key event is received from external devices or @c ime_send_key_event().
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_process_key_event_cb() callback function is called when the key event
 * is received from the external keyboard devices or ime_send_key_event() function.
 *
 * @param[in] callback_func @c ime_process_key_event_with_imengine_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_process_key_event_with_imengine_cb(ime_process_key_event_with_imengine_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a autocapital type is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_autocapital_type_set_cb() callback function is called when an optimization hint is set
 *
 * @param[in] callback_func @c ime_autocapital_type_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_autocapital_type_set_cb(ime_autocapital_type_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a prediction allow is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_prediction_allow_set_cb() callback function is called when an optimization hint is set
 *
 * @param[in] callback_func @c ime_prediction_allow_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_prediction_allow_set_cb(ime_prediction_allow_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a trigger property value is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_trigger_property_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_trigger_property_set_cb(ime_trigger_property_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate more window is shown.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_more_window_show_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_more_window_show_cb(ime_candidate_more_window_show_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate more window is hidden.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_more_window_hide_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_more_window_hide_cb(ime_candidate_more_window_hide_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an AUX item is selected.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_aux_select_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_aux_select_cb(ime_aux_select_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate item is selected.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_select_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_select_cb(ime_candidate_select_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate table page is up.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_table_page_up_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_table_page_up_cb(ime_candidate_table_page_up_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate table page is down.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_table_page_down_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_table_page_down_cb(ime_candidate_table_page_down_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a page size of the candidate window is changed.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_table_page_size_chaned_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_table_page_size_chaned_cb(ime_candidate_table_page_size_chaned_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate item layout is set.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_item_layout_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_item_layout_set_cb(ime_candidate_item_layout_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a displayed candidate number is changed.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_displayed_candidate_number_chaned_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_displayed_candidate_number_chaned_cb(ime_displayed_candidate_number_chaned_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when the candidate item is long pressed.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c ime_candidate_item_long_pressed_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start to run IME application's main loop.
 *
 * @see ime_run()
 */
int ime_event_set_candidate_item_long_pressed_cb(ime_candidate_item_long_pressed_cb callback_func, void *user_data);

/**
 * @brief Requests to show aux string.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_show_aux_string(void);

/**
 * @brief Requests to hide aux string.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_hide_aux_string(void);

/**
 * @brief Requests to show lookup table.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_show_lookup_table(void);

/**
 * @brief Requests to hide lookup table.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_hide_lookup_table(void);

/**
 * @brief Requests to update preedit caret.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] caret The preedit caret
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ime_update_preedit_caret(int caret);

/**
 * @brief Requests to update preedit string with commit string.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] preedit The UTF-8 string to be updated in preedit
 * @param[in] commit The UTF-8 string to be committed
 * @param[in] attrs @a preedit can be composed of multiple string attributes: underline, highlight color and reversal color
 * @param[in] caret The preedit caret
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ime_update_preedit_string_with_commit(const char *preedit, const char *commit, scim::AttributeList &attrs, int caret);

/**
 * @brief Requests to update aux string.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] str The aux string
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ime_update_aux_string(const char *str);

/**
 * @brief Requests to update recapture string.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] offset The offset value from the cursor position
 * @param[in] len The length of the preedit
 * @param[in] preedit The UTF-8 string to be updated in preedit
 * @param[in] commit The UTF-8 string to be committed
 * @param[in] attrs @a preedit can be composed of multiple string attributes: underline, highlight color and reversal color
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ime_recapture_string(int offset, int len, const char *preedit, const char *commit, scim::AttributeList &attrs);

/**
 * @brief Requests to update lookup table.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] table The lookup table
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_update_lookup_table(scim::LookupTable &table);

/**
 * @brief Requests to register properties.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] properties The property list
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_register_properties(scim::PropertyList &properties);

/**
 * @brief Requests to update property.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] property The imengine property
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_update_property(scim::Property &property);

/**
 * @brief Requests to expand candidate.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_expand_candidate(void);

/**
 * @brief Requests to contract candidate.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 */
int ime_contract_candidate(void);

/**
 * @brief Requests to set candidate style.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] portrait_line The candidate portrait line
 * @param[in] mode The candidate mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ime_set_candidate_style(scim::ISF_CANDIDATE_PORTRAIT_LINE_T portrait_line, scim::ISF_CANDIDATE_MODE_T mode);

/**
 * @brief Sets flag whether IMEngine is loaded by engine_loader.
 *
 * @since_tizen 5.5
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] flag Set with Engine Loader mode
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 */
int ime_set_engine_loader_flag(bool flag);

/**
 * @brief Sends the key event processing result.
 *
 * @since_tizen 6.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] is_success The result value to be sent, indicating whether the key event processing succeeded or not
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_NOT_RUNNING IME main loop isn't started yet
 */
int ime_send_key_event_processing_result(scim::KeyEvent &key, uint32_t serial, bool is_success);

/**
 * @brief Updates the given input panel window's size information.
 *
 * @since_tizen 6.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] window The native window of IME
 * @param[in] portrait_width The width in portrait mode
 * @param[in] portrait_height The height in portrait mode
 * @param[in] landscape_width The width in landscape mode
 * @param[in] landscape_height The height in landscape mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_create_cb()
 */
int ime_set_native_window_size(Ecore_Wl2_Window *window, int portrait_width, int portrait_height, int landscape_width, int landscape_height);

/**
 * @brief Requests to replace surrounding text without flickering.
 *
 * @since_tizen 10.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] offset The offset value from the cursor position (in characters not bytes). For example, please use -3 if you want to replace 3 characters in front of current cursor position.
 * @param[in] len The length of the text to replace (in characters not bytes)
 * @param[in] preedit The UTF-8 string to be updated in preedit
 * @param[in] commit The UTF-8 string to be committed
 * @param[in] attrs The GList which has #ime_preedit_attribute lists; @a str can be composed of multiple
 * string attributes: underline, highlight color and reversal color. The @a attrs will be released internally
 * on success and it can be NULL if no attributes to set
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 */
int ime_replace_surrounding_text(int offset, int len, const char *preedit, const char *commit, GList *attrs);

/**
 * @brief Sets the command line arguments for the IME application.
 *
 * @details This function allows the IME application to set or update command line arguments
 * that were passed to the application during startup. This is particularly useful when the IME
 * needs to process command line options or configuration parameters after initialization.
 * The function validates all input parameters to ensure they are properly formatted and safe to use.
 *
 * @since_tizen 10.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] argc The number of command line arguments. Must be non-negative.
 *                   If argc is 0, argv can be NULL.
 * @param[in] argv An array of command line argument strings. Each argument must be a valid
 *                 null-terminated UTF-8 string with length between 1 and 1024 characters.
 *                 Cannot be NULL when argc > 0. All array elements must be non-NULL.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter - argc is negative, argv is NULL when argc > 0,
 *                                         any argv element is NULL, or any argument string is empty or too long
 * @see ime_run()
 */
int ime_set_args(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_INPUTMETHOD_INTERNAL_H__ */
