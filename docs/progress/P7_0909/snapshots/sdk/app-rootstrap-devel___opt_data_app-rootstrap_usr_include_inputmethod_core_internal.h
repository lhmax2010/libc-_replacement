/*
 * Copyright (c) 2026 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_INPUTMETHOD_CORE_INTERNAL_H__
#define __TIZEN_UIX_INPUTMETHOD_CORE_INTERNAL_H__

/**
 * @file inputmethod_core_internal.h
 * @brief This file contains input method internal APIs and related enumeration.
 */

#include <glib.h>
#include <sclcore.h>
#include <inputmethod_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for input method optimization hint.
 *
 * @since_tizen 11.0
 */
typedef enum {
	INPUTMETHOD_OPTIMIZATION_HINT_NONE = 0, /**< No hint provided */
	INPUTMETHOD_OPTIMIZATION_HINT_SHOW_PREPARE, /**< This input method is going to be displayed on screen soon */
} inputmethod_optimization_hint_e;

/**
 * @brief Called when a candidate list provided by IMEngine should be shown.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_candidate_show_cb)(int context_id, void *user_data);

/**
 * @brief Called when a candidate list provided by IMEngine should be hidden.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_candidate_hide_cb)(int context_id, void *user_data);

/**
 * @brief Called when a candidate list provided by IMEngine is changed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a list should not be released.
 *
 * @param[in] list Candidate list
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_lookup_table_changed_cb)(GList *list, void *user_data);

/**
 * @brief Called when an optimization hint value is set.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] hint The hint value used for optimization
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_optimization_hint_set_cb)(inputmethod_optimization_hint_e hint, void *user_data);

/**
 * @brief Called when a key event is received from external devices or inputmethod_send_key_event().
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key The key event of the keyboard
 * @param[in] serial The serial number of the key event
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_process_key_event_with_imengine_cb)(scim::KeyEvent &key, uint32_t serial, void *user_data);

/**
 * @brief Called when a trigger property value is set.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] property The trigger property
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_trigger_property_set_cb)(const char *property, void *user_data);

/**
 * @brief Called when a candidate more window is shown.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_candidate_more_window_show_cb)(void *user_data);

/**
 * @brief Called when a candidate more window is hidden.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_candidate_more_window_hide_cb)(void *user_data);

/**
 * @brief Called when an AUX item is selected.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] item The AUX item
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_aux_select_cb)(uint32_t item, void *user_data);

/**
 * @brief Called when a candidate item is selected.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] item The candidate item
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_candidate_select_cb)(uint32_t item, void *user_data);

/**
 * @brief Called when a candidate table page is up.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_candidate_table_page_up_cb)(void *user_data);

/**
 * @brief Called when a candidate table page is down.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_candidate_table_page_down_cb)(void *user_data);

/**
 * @brief Called when a page size of the candidate window is changed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] size The page size of the candidate window
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_candidate_table_page_size_chaned_cb)(uint32_t size, void *user_data);

/**
 * @brief Called when a candidate item layout is set.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] item The candidate item layout
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_candidate_item_layout_set_cb)(std::vector<uint32_t> item, void *user_data);

/**
 * @brief Called when a displayed candidate number is changed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] page_num The displayed candidate number
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_displayed_candidate_number_chaned_cb)(uint32_t page_num, void *user_data);

/**
 * @brief Called when the candidate item is long pressed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] index The index of the candidate item
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_candidate_item_long_pressed_cb)(uint32_t index, void *user_data);

/* ======================== Event set functions ======================== */

/**
 * @brief Sets the event callback function that is called when a candidate list provided by IMEngine should be shown.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_show_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_show_cb(inputmethod_candidate_show_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate list provided by IMEngine should be hidden.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_hide_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_hide_cb(inputmethod_candidate_hide_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate list provided by IMEngine is changed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_lookup_table_changed_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_lookup_table_changed_cb(inputmethod_lookup_table_changed_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an optimization hint value is set.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_optimization_hint_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_optimization_hint_set_cb(inputmethod_optimization_hint_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a key event is received from external devices or inputmethod_send_key_event().
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_process_key_event_with_imengine_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_process_key_event_with_imengine_cb(inputmethod_process_key_event_with_imengine_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a trigger property value is set.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_trigger_property_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_trigger_property_set_cb(inputmethod_trigger_property_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate more window is shown.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_more_window_show_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_more_window_show_cb(inputmethod_candidate_more_window_show_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate more window is hidden.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_more_window_hide_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_more_window_hide_cb(inputmethod_candidate_more_window_hide_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an AUX item is selected.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_aux_select_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_aux_select_cb(inputmethod_aux_select_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate item is selected.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_select_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_select_cb(inputmethod_candidate_select_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate table page is up.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_table_page_up_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_table_page_up_cb(inputmethod_candidate_table_page_up_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate table page is down.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_table_page_down_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_table_page_down_cb(inputmethod_candidate_table_page_down_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a page size of the candidate window is changed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_table_page_size_chaned_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_table_page_size_chaned_cb(inputmethod_candidate_table_page_size_chaned_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a candidate item layout is set.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_item_layout_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_item_layout_set_cb(inputmethod_candidate_item_layout_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a displayed candidate number is changed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_displayed_candidate_number_chaned_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_displayed_candidate_number_chaned_cb(inputmethod_displayed_candidate_number_chaned_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when the candidate item is long pressed.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c inputmethod_candidate_item_long_pressed_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_event_set_candidate_item_long_pressed_cb(inputmethod_candidate_item_long_pressed_cb callback_func, void *user_data);

/* ======================== Action functions ======================== */

/**
 * @brief Sets keyboard engine.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] engine_id The engine UUID
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_set_imengine(const char *engine_id);

/**
 * @brief Flushes state in Input Method engine.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_flush_imengine(void);

/**
 * @brief Resets state in Input Method engine.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_reset_imengine(void);

/**
 * @brief Sends an event to IMEngine.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] command The command to be sent
 * @param[in] value The value corresponding to the command
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_send_imengine_event(int command, unsigned int value);

/**
 * @brief Sends the selected item index in the candidate list.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] index The selected index in the candidate list
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_select_candidate(unsigned int index);

/**
 * @brief Sends the input context event.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] type The event type
 * @param[in] value The event value
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_update_input_context(unsigned int type, unsigned int value);

/**
 * @brief Sets flag whether IME is called from dotnet.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] set Set with Dotnet mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_set_dotnet_flag(bool set);

/**
 * @brief Sets flag whether creating IME window should be deferred until there is an access request.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] flag @c true if deferring window creation is desired
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_set_window_creation_defer_flag(bool flag);

/**
 * @brief Sets flag whether IMEngine is loaded by engine_loader.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] flag Set with Engine Loader mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_set_engine_loader_flag(bool flag);

/**
 * @brief Requests to show aux string.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_show_aux_string(void);

/**
 * @brief Requests to hide aux string.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_hide_aux_string(void);

/**
 * @brief Requests to show lookup table.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_show_lookup_table(void);

/**
 * @brief Requests to hide lookup table.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_hide_lookup_table(void);

/**
 * @brief Requests to update preedit caret.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] caret The preedit caret
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_update_preedit_caret(int caret);

/**
 * @brief Requests to update preedit string with commit string.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] preedit The UTF-8 string to be updated in preedit
 * @param[in] commit The UTF-8 string to be committed
 * @param[in] attrs @a preedit can be composed of multiple string attributes: underline, highlight color and reversal color
 * @param[in] caret The preedit caret
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_update_preedit_string_with_commit(const char *preedit, const char *commit, scim::AttributeList &attrs, int caret);

/**
 * @brief Requests to update aux string.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] str The aux string
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_update_aux_string(const char *str);

/**
 * @brief Requests to update recapture string.
 *
 * @since_tizen 11.0
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
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_recapture_string(int offset, int len, const char *preedit, const char *commit, scim::AttributeList &attrs);

/**
 * @brief Requests to update lookup table.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] table The lookup table
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_update_lookup_table(scim::LookupTable &table);

/**
 * @brief Requests to register properties.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] properties The property list
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_register_properties(scim::PropertyList &properties);

/**
 * @brief Requests to update property.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] property The imengine property
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_update_property(scim::Property &property);

/**
 * @brief Requests to expand candidate.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_expand_candidate(void);

/**
 * @brief Requests to contract candidate.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_contract_candidate(void);

/**
 * @brief Requests to set candidate style.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] portrait_line The candidate portrait line
 * @param[in] mode The candidate mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_set_candidate_style(scim::ISF_CANDIDATE_PORTRAIT_LINE_T portrait_line, scim::ISF_CANDIDATE_MODE_T mode);

/**
 * @brief Sends the key event processing result.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key The key event
 * @param[in] serial The serial number of the key event
 * @param[in] is_success The result value indicating whether the key event processing succeeded or not
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_send_key_event_processing_result(scim::KeyEvent &key, uint32_t serial, bool is_success);

/**
 * @brief Requests to replace surrounding text without flickering.
 *
 * @since_tizen 11.0
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] offset The offset value from the cursor position (in characters not bytes)
 * @param[in] len The length of the text to replace (in characters not bytes)
 * @param[in] preedit The UTF-8 string to be updated in preedit
 * @param[in] commit The UTF-8 string to be committed
 * @param[in] attrs The GList which has preedit attribute lists; can be NULL if no attributes to set
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_replace_surrounding_text(int offset, int len, const char *preedit, const char *commit, GList *attrs);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_INPUTMETHOD_CORE_INTERNAL_H__ */
