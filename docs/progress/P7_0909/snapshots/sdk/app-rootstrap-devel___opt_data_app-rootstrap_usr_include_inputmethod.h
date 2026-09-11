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

#ifndef __TIZEN_UIX_INPUTMETHOD_H__
#define __TIZEN_UIX_INPUTMETHOD_H__

/**
 * @file inputmethod.h
 * @brief This file contains input method APIs and related enumeration.
 */

#include <tizen.h>
#include <inputmethod_keydef.h>
#include <inputmethod_device_event.h>

#include <Ecore_IMF.h>
#include <Evas.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_UIX_INPUTMETHOD_MODULE
 * @{
 */

/**
 * @brief Enumeration for input method function error.
 *
 * @since_tizen 2.4
 */
typedef enum {
	IME_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	IME_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	IME_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	IME_ERROR_NO_CALLBACK_FUNCTION = TIZEN_ERROR_IME | 0x0001, /**< Necessary callback function is not set */
	IME_ERROR_NOT_RUNNING = TIZEN_ERROR_IME | 0x0002, /**< IME main loop is not started yet */
	IME_ERROR_OPERATION_FAILED = TIZEN_ERROR_IME | 0x0003, /**< Operation failed */
	IME_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
} ime_error_e;

/**
 * @brief Enumeration of the option window type.
 *
 * @since_tizen 2.4
 *
 * @see ime_option_window_created_cb()
 */
typedef enum {
	IME_OPTION_WINDOW_TYPE_KEYBOARD,  /**< Open from Keyboard */
	IME_OPTION_WINDOW_TYPE_SETTING_APPLICATION, /**< Open from Setting application */
} ime_option_window_type_e;

/**
 * @brief Enumeration of layout variation.
 *
 * @since_tizen 2.4
 *
 * @see ime_context_get_layout_variation()
 */
typedef enum {
	IME_LAYOUT_NORMAL_VARIATION_NORMAL = 0, /**< The plain normal layout */
	IME_LAYOUT_NORMAL_VARIATION_FILENAME, /**< Filename layout; symbols such as '/', '*', '\', '|', '&lt;', '&gt;', '?', '&quot;' and ':' should be disabled */
	IME_LAYOUT_NORMAL_VARIATION_PERSON_NAME, /**< The name of a person */
	IME_LAYOUT_NUMBERONLY_VARIATION_NORMAL = 0, /**< The plain normal number layout */
	IME_LAYOUT_NUMBERONLY_VARIATION_SIGNED, /**< The number layout to allow a negative sign */
	IME_LAYOUT_NUMBERONLY_VARIATION_DECIMAL, /**< The number layout to allow decimal point to provide fractional value */
	IME_LAYOUT_NUMBERONLY_VARIATION_SIGNED_AND_DECIMAL, /**< The number layout to allow decimal point and negative sign */
	IME_LAYOUT_PASSWORD_VARIATION_NORMAL = 0, /**< The normal password layout */
	IME_LAYOUT_PASSWORD_VARIATION_NUMBERONLY, /**< The password layout to allow only number */
} ime_layout_variation_e;

/**
 * @brief Enumeration of string attribute type.
 *
 * @since_tizen 2.4
 *
 * @remarks Currently, a font style is available to use.
 *
 * @see #ime_preedit_attribute
 * @see ime_update_preedit_string()
 */
typedef enum {
	IME_ATTR_NONE, /**< No attribute */
	IME_ATTR_FONTSTYLE, /**< A font style attribute, e.g., underline, etc. */
} ime_attribute_type;

/**
 * @brief Enumeration containing input panel events.
 *
 * @since_tizen 5.5
 *
 * @see ime_update_input_panel_event()
 */
typedef enum {
	IME_EVENT_TYPE_LANGUAGE = 1, /**< The language of the input panel */
	IME_EVENT_TYPE_SHIFT_MODE, /**< The shift key state of the input panel */
	IME_EVENT_TYPE_GEOMETRY, /**< The size of the input panel */
} ime_event_type_e;

/**
 * @brief Value for #IME_ATTR_FONTSTYLE. Draw a line under the text.
 * @since_tizen 2.4
 */
#define IME_ATTR_FONTSTYLE_UNDERLINE    1

/**
 * @brief Value for #IME_ATTR_FONTSTYLE. Draw text in highlighted color.
 * @since_tizen 2.4
 */
#define IME_ATTR_FONTSTYLE_HIGHLIGHT    2

/**
 * @brief Value for #IME_ATTR_FONTSTYLE. Draw text in reversal color.
 * @since_tizen 2.4
 */
#define IME_ATTR_FONTSTYLE_REVERSAL     4

/**
 * @brief The structure type to contain the attributes for preedit string.
 *
 * @since_tizen 2.4
 *
 * @remarks A preedit string may have one or more different attributes. This structure describes each attribute of the string.
 *
 * @see ime_update_preedit_string()
 * @see #ime_attribute_type
 */
typedef struct {
	unsigned int start; /**< The start position in the string of this attribute */
	unsigned int length; /**< The character length of this attribute, the range is [start, start+length] */
	ime_attribute_type type; /**< The type of this attribute */
	unsigned int value; /**< The value of this attribute */
} ime_preedit_attribute;

/**
 * @brief Handle of an associated text input UI control's input context.
 *
 * @details This is one of parameters of ime_show_cb() callback function. IME application
 * should configure its input panel with this structure information.
 *
 * @since_tizen 2.4
 *
 * @see ime_context_get_layout()
 * @see ime_context_get_layout_variation()
 * @see ime_context_get_cursor_position()
 * @see ime_context_get_autocapital_type()
 * @see ime_context_get_return_key_type()
 * @see ime_context_get_return_key_state()
 * @see ime_context_get_prediction_mode()
 * @see ime_context_get_password_mode()
 * @see ime_context_get_input_hint()
 * @see ime_context_get_bidi_direction()
 * @see ime_context_get_language()
 */
typedef struct _ime_context *ime_context_h;

/**
 * @brief Handle of the device information of the key event.
 *
 * @details This is one of parameters of ime_process_key_event_cb() callback function. IME application
 * may distinguish the key event by using this if necessary.
 *
 * @since_tizen 2.4
 *
 * @see ime_process_key_event_cb()
 * @see ime_device_info_get_name()
 * @see ime_device_info_get_class()
 * @see ime_device_info_get_subclass()
 */
typedef struct _ime_device_info *ime_device_info_h;

/**
 * @brief Called when the input panel is created.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks This callback function is mandatory and must be registered using ime_run(). The
 * ime_get_main_window() can be used to get the created input panel window.
 *
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The ime_run() function calls this callback function.
 *
 * @see ime_run()
 * @see ime_set_size()
 * @see ime_get_main_window()
 */
typedef void (*ime_create_cb)(void *user_data);

/**
 * @brief Called when the input panel is terminated.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks This callback function is mandatory and must be registered using ime_run(). The
 * ime_get_main_window() can be used to get the created input panel window.
 *
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see ime_run()
 * @see ime_get_main_window()
 */
typedef void (*ime_terminate_cb)(void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to show itself.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks This callback function is mandatory and must be registered using ime_run().
 * IME application should configure its input panel with #ime_context_h structure information.
 * The ime_get_main_window() can be used to get the created input panel window.
 * @a context should not be released.
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] context The input context information handle
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see ime_run()
 * @see ime_get_main_window()
 * @see ime_context_get_layout()
 * @see ime_context_get_layout_variation()
 * @see ime_context_get_cursor_position()
 * @see ime_context_get_autocapital_type()
 * @see ime_context_get_return_key_type()
 * @see ime_context_get_return_key_state()
 * @see ime_context_get_prediction_mode()
 * @see ime_context_get_password_mode()
 * @see ime_context_get_input_hint()
 * @see ime_context_get_bidi_direction()
 * @see ime_context_get_language()
 */
typedef void (*ime_show_cb)(int context_id, ime_context_h context, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to hide itself.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks This callback function is mandatory and must be registered using ime_run(). The
 * ime_get_main_window() can be used to get the created input panel window.
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see ime_run()
 * @see ime_get_main_window()
 */
typedef void (*ime_hide_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control has focus.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_focus_in_cb() function.
 *
 * @see ime_event_set_focus_in_cb()
 */
typedef void (*ime_focus_in_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control loses focus.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_focus_out_cb() function.
 *
 * @see ime_event_set_focus_out_cb()
 */
typedef void (*ime_focus_out_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control responds to a request with the surrounding text.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_request_surrounding_text() must be called to invoke this callback function, asynchronously.
 * @remarks @a text can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] text The UTF-8 string requested
 * @param[in] cursor_pos The cursor position
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_surrounding_text_updated_cb() function.
 *
 * @see ime_event_set_surrounding_text_updated_cb()
 * @see ime_request_surrounding_text()
 */
typedef void (*ime_surrounding_text_updated_cb)(int context_id, const char *text, int cursor_pos, void *user_data);

/**
 * @brief Called to reset the input context of an associated text input UI control.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_input_context_reset_cb() function.
 *
 * @see ime_event_set_input_context_reset_cb()
 */
typedef void (*ime_input_context_reset_cb)(void *user_data);

/**
 * @brief Called when the position of the cursor in an associated text input UI control changes.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] cursor_pos The cursor position
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_cursor_position_updated_cb() function.
 *
 * @see ime_event_set_cursor_position_updated_cb()
 */
typedef void (*ime_cursor_position_updated_cb)(int cursor_pos, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the language from the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The allocated @a lang_code will be released internally.
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] lang_code Input panel's current input language code (e.g., &quot;en_US&quot;)
 *
 * @pre The callback can be registered using ime_event_set_language_requested_cb() function.
 *
 * @see ime_event_set_language_requested_cb()
 */
typedef void (*ime_language_requested_cb)(void *user_data, char **lang_code);

/**
 * @brief Called to set the preferred language to the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a language information is already set to the input panel when it is shown
 * through #ime_context_h. This callback function will be only called when the client
 * application changes the edit field's language attribute after the input panel is shown.
 *
 * @param[in] language The preferred language that the client application wants
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_language_set_cb() function.
 *
 * @see ime_event_set_language_set_cb()
 */
typedef void (*ime_language_set_cb)(Ecore_IMF_Input_Panel_Lang language, void *user_data);

/**
 * @brief Called to set the application specific data to deliver to the input panel.
 *
 * @details This function is used by the applications to deliver the specific data to the input panel.
 * The data format MUST be negotiated by both application and input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a data should not be released.
 *
 * @param[in] data The specific data to be set to the input panel
 * @param[in] data_length The length of data, in bytes, to send to the input panel
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_imdata_set_cb() function.
 *
 * @see ime_event_set_imdata_set_cb()
 */
typedef void (*ime_imdata_set_cb)(void *data, unsigned int data_length, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the application specific data from the input panel.
 *
 * @details This function is used by the applications to request the specific data from the input panel.
 * The data format MUST be negotiated by both application and input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The allocated @a data and @a data_length will be released internally.
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] data Input panel's data to be set to the application
 * @param[out] data_length The length of data, in bytes, to send to the application
 *
 * @pre The callback can be registered using ime_event_set_imdata_requested_cb() function.
 *
 * @see ime_event_set_imdata_requested_cb()
 */
typedef void (*ime_imdata_requested_cb)(void *user_data, void **data, unsigned int *data_length);

/**
 * @brief Called when an associated text input UI control requests the input panel to set its layout.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a layout information is already set to the input panel when it is shown
 * through #ime_context_h. This callback function will be only called when the client
 * application changes the edit field's layout attribute after the input panel is shown.
 *
 * @param[in] layout The input panel layout
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_layout_set_cb() function.
 *
 * @see ime_event_set_layout_set_cb()
 */
typedef void (*ime_layout_set_cb)(Ecore_IMF_Input_Panel_Layout layout, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to set input hint.
 *
 * @since_tizen 6.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a input_hint information is already set to the input panel when it is shown
 * through #ime_context_h. This callback function will be only called when the client
 * application changes the edit field's input hint attribute after the input panel is shown.
 *
 * @param[in] input_hint The input hint
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_input_hint_set_cb() function.
 *
 * @see ime_event_set_input_hint_set_cb()
 */
typedef void (*ime_input_hint_set_cb)(Ecore_IMF_Input_Hints input_hint, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to set the @c Return key label.
 * The input panel can show text or image on the @c Return button according to the @c Return key action.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a type information is already set to the input panel when it is shown
 * through #ime_context_h. This callback function will be only called when the client
 * application changes the edit field's @c Return key type attribute after the input panel
 * is shown.
 *
 * @param[in] type The type of @c Return key on the input panel
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_return_key_type_set_cb() function.
 *
 * @see ime_event_set_return_key_type_set_cb()
 */
typedef void (*ime_return_key_type_set_cb)(Ecore_IMF_Input_Panel_Return_Key_Type type, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to enable
 * or disable the @c Return key state.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a disabled information is already set to the input panel when it is shown
 * through #ime_context_h. This callback function will be only called when the client
 * application changes the edit field's @c Return key disable attribute after the input panel
 * is shown.
 *
 * @param[in] disabled The Boolean state to disable @c Return key. The @c Return key is enabled by default
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_return_key_state_set_cb() function.
 *
 * @see ime_event_set_return_key_state_set_cb()
 */
typedef void (*ime_return_key_state_set_cb)(bool disabled, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the position and size from the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a x, @a y, @a w, and @a h should not be released.
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] x The x position in screen
 * @param[out] y The y position in screen
 * @param[out] w The window width
 * @param[out] h The window height
 *
 * @pre The callback can be registered using ime_event_set_geometry_requested_cb() function.
 *
 * @see ime_event_set_geometry_requested_cb()
 */
typedef void (*ime_geometry_requested_cb)(void *user_data, int *x, int *y, int *w, int *h);

/**
 * @brief Called when a key event is received from external devices or ime_send_key_event().
 *
 * @details This function processes the key event before an associated text input UI control does.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks If the key event is from the external device, @a dev_info will have its name, class and subclass information.
 * @a dev_info should not be released by the application. The platform manages the handle; the handle is released when ime_process_key_event_cb() exits.
 *
 * @param[in] key_code The key code to be sent
 * @param[in] key_mask The modifier key mask
 * @param[in] dev_info The device information handle
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @return @c true if the event was processed, otherwise the event was not processed and was forwarded to the client application.
 *
 * @pre The callback should be registered using ime_event_set_process_key_event_cb() function.
 *
 * @see ime_event_set_process_key_event_cb()
 * @see ime_device_info_get_name()
 * @see ime_device_info_get_class()
 * @see ime_device_info_get_subclass()
 * @see ime_send_key_event()
 * @see ime_commit_string()
 * @see ime_show_preedit_string()
 * @see ime_hide_preedit_string()
 * @see ime_update_preedit_string()
 */
typedef bool (*ime_process_key_event_cb)(ime_key_code_e key_code, ime_key_mask_e key_mask, ime_device_info_h dev_info, void *user_data);

/**
 * @brief Called when a key event is received with a keycode from external devices or ime_send_key_event().
 *
 * @details This function processes a key event with a keycode before an associated UI control for the text input deals with the key event.
 *
 * @since_tizen 5.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks If the key event is from the external device, @a dev_info will have its name, class and subclass information.
 * @a dev_info should not be released by the application. The platform manages the handle; the handle is released when ime_process_key_event_with_keycode_cb() exits.
 *
 * @param[in] key_code The X11 key code to be sent
 * @param[in] key_sym The key symbol to be sent
 * @param[in] key_mask The modifier key mask
 * @param[in] dev_info The device information handle
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @return @c true if the event was processed, otherwise @c false. When @c false returns, the event was not processed and was forwarded to the client application.
 *
 * @pre The callback should be registered using ime_event_set_process_key_event_with_keycode_cb() function.
 *
 * @see ime_event_set_process_key_event_with_keycode_cb()
 * @see ime_device_info_get_name()
 * @see ime_device_info_get_class()
 * @see ime_device_info_get_subclass()
 * @see ime_send_key_event()
 * @see ime_commit_string()
 * @see ime_show_preedit_string()
 * @see ime_hide_preedit_string()
 * @see ime_update_preedit_string()
 */
typedef bool (*ime_process_key_event_with_keycode_cb)(unsigned int key_code, ime_key_code_e key_sym, ime_key_mask_e key_mask, ime_device_info_h dev_info, void *user_data);

/**
 * @brief Called when the system display language is changed.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] language The language code
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_display_language_changed_cb() function.
 *
 * @see ime_event_set_display_language_changed_cb()
 */
typedef void (*ime_display_language_changed_cb)(const char *language, void *user_data);

/**
 * @brief Called when the device is rotated.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] degree The rotation degree
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_rotation_degree_changed_cb() function.
 *
 * @see ime_event_set_rotation_degree_changed_cb()
 */
typedef void (*ime_rotation_degree_changed_cb)(int degree, void *user_data);

/**
 * @brief Called when Accessibility in Settings application is on or off.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] state Accessibility option state
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_accessibility_state_changed_cb() function.
 *
 * @see ime_event_set_accessibility_state_changed_cb()
 */
typedef void (*ime_accessibility_state_changed_cb)(bool state, void *user_data);

/**
 * @brief Called to create the option window.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks if Input panel requests to open the option window, @a type will be #IME_OPTION_WINDOW_TYPE_KEYBOARD.
 * And if Settings application requests to open it, @a type will be #IME_OPTION_WINDOW_TYPE_SETTING_APPLICATION.
 * The @a window should not be released. The @a window can be used until ime_option_window_destroyed_cb() will be called.
 *
 * @param[in] window The created window object
 * @param[in] type The type of option window
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using ime_event_set_option_window_created_cb() function. The
 * ime_create_option_window() calls this callback function or Settings application can call this callback function.
 *
 * @see ime_event_set_option_window_created_cb()
 * @see ime_create_option_window()
 */
typedef void (*ime_option_window_created_cb)(Evas_Object *window, ime_option_window_type_e type, void *user_data);

/**
 * @brief Called to destroy the option window.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The @a window should be released using evas_object_del().
 *
 * @param[in] window The window object to destroy
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using ime_event_set_option_window_destroyed_cb() function.
 * ime_destroy_option_window() calls this callback function.
 *
 * @see ime_event_set_option_window_destroyed_cb()
 */
typedef void (*ime_option_window_destroyed_cb)(Evas_Object *window, void *user_data);

/**
 * @brief Called to set the prediction hint string to deliver to the input panel.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a prediction_hint is valid only in the callback. To use outside the callback, make a copy.
 * This function is used by the applications to deliver the prediction hint message to the input panel.
 *
 * @param[in] prediction_hint The prediction hint to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using ime_event_set_prediction_hint_set_cb() function.
 *
 * @see ime_event_set_prediction_hint_set_cb()
 */
typedef void (*ime_prediction_hint_set_cb)(const char *prediction_hint, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the text entry to set the MIME type.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a mime_type is valid only in the callback. To use outside the callback, make a copy.
 * This function is used by the applications to deliver the MIME type to the input panel.
 *
 * @param[in] mime_type The MIME type to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using ime_event_set_mime_type_set_request_cb() function.
 *
 * @see ime_event_set_mime_type_set_request_cb()
 */
typedef void (*ime_mime_type_set_request_cb)(const char *mime_type, void *user_data);

/**
 * @brief Called to set key-value pairs of predicting messages to deliver to the input panel.
 *
 * @since_tizen 5.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a key and @a value is valid only in the callback. To use outside the callback, make a copy.
 * This function is used by applications to deliver predicted hint messages to the input panel.
 *
 * @param[in] key The prediction hint key to be set to the input panel
 * @param[in] value The prediction hint value to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using ime_event_set_prediction_hint_data_set_cb() function.
 *
 * @see ime_event_set_prediction_hint_data_set_cb()
 */
typedef void (*ime_prediction_hint_data_set_cb)(const char *key, const char *value, void *user_data);

/**
 * @brief Called when an associated text input UI control sets the position align of the input panel.
 *
 * @since_tizen 9.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks Regardless of the rotation degree, the @a x, @a y values of the top-left corner on the screen are based on 0, 0.
 * This function is used to adjust the position of the input panel when its size changes in floating mode.
 *
 * @param[in] x The x coordinate of the @a align
 * @param[in] y The y coordinate of the @a align
 * @param[in] align The position alignment that the client application wants
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using ime_event_set_position_align_set_cb() function.
 *
 * @see ime_event_set_position_align_set_cb()
 */
typedef void (*ime_position_align_set_cb)(int x, int y, Ecore_IMF_Input_Panel_Align align, void *user_data);

/**
 * @brief The structure type to contain the set of the essential callback functions for IME application lifecycle and appearance.
 *
 * @since_tizen 2.4
 *
 * @remarks These four callback functions are mandatory for IME application.
 *
 * @see ime_run()
 */
typedef struct {
	ime_create_cb create;       /**< Called when the input panel is created */
	ime_terminate_cb terminate; /**< Called when the input panel is terminated */
	ime_show_cb show;           /**< Called when the input panel is requested to show itself */
	ime_hide_cb hide;           /**< Called when the input panel is requested to hide itself */
} ime_callback_s;

/**
 * @brief Runs the main loop of IME application.
 *
 * @details This function starts to run IME application's main loop. The ime_create_cb()
 * callback function is called to initialize IME application before the main loop starts up. And
 * the ime_terminate_cb() callback function is called when IME application is terminated.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks IME application MUST implement ime_app_main() function which is the main
 * entry point of IME application. In ime_app_main() function, the ime_run()
 * function MUST be called with the necessary callback functions; ime_create_cb(),
 * ime_terminate_cb(), ime_show_cb(), and ime_hide_cb() callback functions
 * are mandatory for IME application.
 *
 * @param[in] basic_cb The structure pointer of the essential callback functions
 * @param[in] user_data User data to be passed to the callback functions
 *
 * @return 0 if IME application ends successfully, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The ime_event_set_***() functions can be called to set the event handling callback functions.
 *
 * @code
 static void inputmethod_create_cb(void *user_data);
 static void inputmethod_terminate_cb(void *user_data);
 static void inputmethod_show_cb(int context_id, ime_context_h context, void *user_data);
 static void inputmethod_hide_cb(int context_id, void *user_data);
 static void inputmethod_focus_in_cb(int context_id, void *user_data);
 static void inputmethod_focus_out_cb(int context_id, void *user_data);
 static void inputmethod_cursor_position_updated_cb(int cursor_pos, void *user_data);

 static void inputmethod_create_cb(void *user_data)
 {
	 Evas_Object *ime_win = NULL;

	 ime_set_size(480, 400, 800, 400);
	 ime_win = ime_get_main_window();
	 if (ime_win) {
		 // Prepare before showing IME window.
	 }
 }

 static void inputmethod_show_cb(int context_id, ime_context_h context, void *user_data)
 {
	 Ecore_IMF_Input_Panel_Layout layout;
	 ime_layout_variation_e layout_variation;
	 Evas_Object *ime_win;

	 ime_context_get_layout(context, &layout);
	 ime_context_get_layout_variation(context, &layout_variation);

	 ime_win = ime_get_main_window();
	 if (ime_win) {
		 // Compose IME UI properly with the context information and show.

		 evas_object_show(ime_win);
	 }
 }

 static void inputmethod_hide_cb(int context_id, void *user_data)
 {
	 Evas_Object *ime_win = ime_get_main_window();
	 if (ime_win) {
		 evas_object_hide(ime_win);
	 }
 }

 void ime_app_main(int argc, char **argv)
 {
	 ime_callback_s basic_callback = {
		 inputmethod_create_cb,
		 inputmethod_terminate_cb,
		 inputmethod_show_cb,
		 inputmethod_hide_cb,
	 };

	 ime_event_set_focus_in_cb(inputmethod_focus_in_cb, NULL);
	 ime_event_set_focus_out_cb(inputmethod_focus_out_cb, NULL);
	 ime_event_set_cursor_position_updated_cb(inputmethod_cursor_position_updated_cb, NULL);

	 ime_run(&basic_callback, NULL);
 }
 * @endcode
 *
 * @see #ime_callback_s
 * @see ime_event_set_focus_in_cb()
 * @see ime_event_set_focus_out_cb()
 * @see ime_event_set_surrounding_text_updated_cb()
 * @see ime_event_set_input_context_reset_cb()
 * @see ime_event_set_cursor_position_updated_cb()
 * @see ime_event_set_language_requested_cb()
 * @see ime_event_set_language_set_cb()
 * @see ime_event_set_imdata_set_cb()
 * @see ime_event_set_layout_set_cb()
 * @see ime_event_set_return_key_type_set_cb()
 * @see ime_event_set_return_key_state_set_cb()
 * @see ime_event_set_geometry_requested_cb()
 * @see ime_event_set_display_language_changed_cb()
 * @see ime_event_set_rotation_degree_changed_cb()
 * @see ime_event_set_accessibility_state_changed_cb()
 * @see ime_event_set_option_window_created_cb()
 * @see ime_event_set_option_window_destroyed_cb()
 * @see ime_event_set_prediction_hint_set_cb()
 */
int ime_run(ime_callback_s *basic_cb, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control has focus.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_focus_in_cb() callback function is called when an associated text input
 * UI control has focus.
 *
 * @param[in] callback_func @c focus_in event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_focus_in_cb()
 * @see ime_run()
 */
int ime_event_set_focus_in_cb(ime_focus_in_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control loses focus.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_focus_out_cb() callback function is called when an associated text input
 * UI control loses focus.
 *
 * @param[in] callback_func @c focus_out event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_focus_out_cb()
 * @see ime_run()
 */
int ime_event_set_focus_out_cb(ime_focus_out_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control responds to a request with the surrounding text.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_surrounding_text_updated_cb() callback function is called when an
 * associated text input UI control responds to a request with the surrounding text.
 *
 * @param[in] callback_func @c surrounding_text_updated event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_surrounding_text_updated_cb()
 * @see ime_run()
 */
int ime_event_set_surrounding_text_updated_cb(ime_surrounding_text_updated_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called to reset the input context of an associated text input UI control.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_input_context_reset_cb() callback function is called to reset the input
 * context of an associated text input UI control.
 *
 * @param[in] callback_func @c input_context_reset event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_input_context_reset_cb()
 * @see ime_run()
 */
int ime_event_set_input_context_reset_cb(ime_input_context_reset_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when the position of the cursor in an associated text input UI control changes.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_cursor_position_updated_cb() callback function is called when the position
 * of the cursor in an associated text input UI control changes.
 *
 * @param[in] callback_func @c cursor_position_updated event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
  * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_cursor_position_updated_cb()
 * @see ime_run()
 */
int ime_event_set_cursor_position_updated_cb(ime_cursor_position_updated_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the language from the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_language_requested_cb() callback function is called when an associated
 * text input UI control requests the language from the input panel.
 *
 * @param[in] callback_func @c language_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_language_requested_cb()
 * @see ime_run()
 */
int ime_event_set_language_requested_cb(ime_language_requested_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called to set the preferred language to the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_language_set_cb() callback function is called to set the preferred
 * language to the input panel.
 *
 * @param[in] callback_func @c language_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_language_set_cb()
 * @see ime_run()
 */
int ime_event_set_language_set_cb(ime_language_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called to set the application specific data to deliver to the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_imdata_set_cb() callback function is called to set the application
 * specific data to deliver to the input panel.
 *
 * @param[in] callback_func @c imdata_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_imdata_set_cb()
 * @see ime_event_set_imdata_requested_cb()
 * @see ime_run()
 */
int ime_event_set_imdata_set_cb(ime_imdata_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the application specific data from the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_imdata_requested_cb() callback function is called when an associated
 * text input UI control requests the application specific data from the input panel.
 *
 * @param[in] callback_func @c imdata_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_imdata_requested_cb()
 * @see ime_event_set_imdata_set_cb()
 * @see ime_run()
 */
int ime_event_set_imdata_requested_cb(ime_imdata_requested_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to set its layout.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_layout_set_cb() callback function is called when an associated text input
 * UI control requests the input panel to set its layout.
 *
 * @param[in] callback_func @c layout_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_layout_set_cb()
 * @see ime_run()
 */
int ime_event_set_layout_set_cb(ime_layout_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to set the @c Return key label.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_return_key_type_set_cb() callback function is called when an associated
 * text input UI control requests the input panel to set the @c Return key label.
 *
 * @param[in] callback_func @c return_key_type_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_return_key_type_set_cb()
 * @see ime_run()
 */
int ime_event_set_return_key_type_set_cb(ime_return_key_type_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to control the @c Return key state.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_return_key_state_set_cb() callback function is called when an associated
 * text input UI control requests the input panel to enable or disable the @c Return key state.
 *
 * @param[in] callback_func @c return_key_state_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_return_key_state_set_cb()
 * @see ime_run()
 */
int ime_event_set_return_key_state_set_cb(ime_return_key_state_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the position and size from the input panel.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_geometry_requested_cb() callback function is called when an associated
 * text input UI control requests the position and size from the input panel.
 *
 * @param[in] callback_func @c geometry_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_geometry_requested_cb()
 * @see ime_run()
 */
int ime_event_set_geometry_requested_cb(ime_geometry_requested_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a key event is received from external devices or @c ime_send_key_event().
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_process_key_event_cb() callback function is called when the key event
 * is received from the external keyboard devices or ime_send_key_event() function.
 *
 * @param[in] callback_func @c process_key_event event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @code
 static void inputmethod_create_cb(void *user_data);
 static void inputmethod_terminate_cb(void *user_data);
 static void inputmethod_show_cb(int context_id, ime_context_h context, void *user_data);
 static void inputmethod_hide_cb(int context_id, void *user_data);

 static bool inputmethod_process_key_event_cb(ime_key_code_e keycode, ime_key_mask_e keymask, void *user_data);
 {
	 if (keymask & IME_KEY_MASK_CONTROL) {
		 return false; // e.g., Control+C key event would be forwarded to UI control of the client application
	 }
	 if (keymask & IME_KEY_MASK_ALT) {
		 return false;
	 }

	 if (!(keymask & IME_KEY_MASK_RELEASED)) { // The key is pressed
		 if (keycode == IME_KEY_1) {
			 ime_update_preedit_string("1"); // Show "1" preedit string
			 return true;
		 }
		 else if (keycode == IME_KEY_2) {
			 ime_commit_string("12"); // Input "12" string
			 return true;
		 }
	 }

	 return false;
 }

 void ime_app_main(int argc, char **argv)
 {
	 ime_callback_s basic_callback = {
		 inputmethod_create_cb,
		 inputmethod_terminate_cb,
		 inputmethod_show_cb,
		 inputmethod_hide_cb,
	 };

	 ime_event_set_process_key_event_cb(inputmethod_process_key_event_cb, NULL);

	 ime_run(&basic_callback, NULL);
 }
 * @endcode
 *
 * @see ime_process_key_event_cb()
 * @see ime_run()
 */
int ime_event_set_process_key_event_cb(ime_process_key_event_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when a key event is received with a keycode from external devices or @c ime_send_key_event().
 *
 * @since_tizen 5.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_process_key_event_with_keycode_cb() callback function is called when the key event
 * is received from external keyboard devices or ime_send_key_event().
 *
 * @param[in] callback_func @c process_key_event_with_keycode() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_process_key_event_with_keycode_cb()
 * @see ime_run()
 */
int ime_event_set_process_key_event_with_keycode_cb(ime_process_key_event_with_keycode_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when the system display language is changed.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_display_language_changed_cb() callback function is called when the system
 * display language is changed.
 *
 * @param[in] callback_func @c display_language_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_display_language_changed_cb()
 * @see ime_run()
 */
int ime_event_set_display_language_changed_cb(ime_display_language_changed_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when the device is rotated.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_rotation_degree_changed_cb() callback function is called when the device
 * is rotated.
 *
 * @param[in] callback_func @c rotation_degree_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_rotation_degree_changed_cb()
 * @see ime_run()
 */
int ime_event_set_rotation_degree_changed_cb(ime_rotation_degree_changed_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when Accessibility in Settings application is on or off.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_accessibility_state_changed_cb() callback function is called when
 * Accessibility in Settings application is on or off.
 *
 * @param[in] callback_func @c accessibility_state_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_accessibility_state_changed_cb()
 * @see ime_run()
 */
int ime_event_set_accessibility_state_changed_cb(ime_accessibility_state_changed_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called to create the option window.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_option_window_created_cb() callback function is called to create the option window.
 *
 * @param[in] callback_func @c option_window_created event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_option_window_created_cb()
 * @see ime_run()
 */
int ime_event_set_option_window_created_cb(ime_option_window_created_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called to destroy the option window.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_option_window_destroyed_cb() callback function is called to destroy the option window.
 *
 * @param[in] callback_func @c option_window_destroyed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_option_window_destroyed_cb()
 * @see ime_run()
 */
int ime_event_set_option_window_destroyed_cb(ime_option_window_destroyed_cb callback_func, void *user_data);

/**
 * @brief Sends a key event to the associated text input UI control.
 *
 * @details This function sends key down or up event with key mask to the client application.
 * If @a forward_key is @c true, this key event goes to the edit field directly. And if @a forward_key
 * is @c false, the ime_process_key_event_cb() callback function receives the key event before the edit field.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] keycode The key code to be sent
 * @param[in] keymask The modifier key mask
 * @param[in] forward_key The flag to send the key event directly to the edit field
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post If @a forward_key is @c false, the ime_process_key_event_cb() callback function can compose the text with the key events.
 *
 * @see ime_key_code_e
 * @see ime_key_mask_e
 * @see ime_process_key_event_cb()
 */
int ime_send_key_event(ime_key_code_e keycode, ime_key_mask_e keymask, bool forward_key);

/**
 * @brief Sends the text to the associated text input UI control.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] str The UTF-8 string to be committed
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_show_preedit_string()
 * @see ime_hide_preedit_string()
 * @see ime_update_preedit_string()
 */
int ime_commit_string(const char *str);

/**
 * @brief Requests to show preedit string.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_commit_string()
 * @see ime_hide_preedit_string()
 * @see ime_update_preedit_string()
 */
int ime_show_preedit_string(void);

/**
 * @brief Requests to hide preedit string.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_commit_string()
 * @see ime_show_preedit_string()
 * @see ime_update_preedit_string()
 */
int ime_hide_preedit_string(void);

/**
 * @brief Updates a new preedit string.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] str The UTF-8 string to be updated in preedit
 * @param[in] attrs The Eina_List which has #ime_preedit_attribute lists; @a str can be composed of multiple
 * string attributes: underline, highlight color and reversal color. The @a attrs will be released internally
 * on success and it can be NULL if no attributes to set
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post This function is supposed to be followed by the ime_show_preedit_string() function.
 *
 * @code
 {
	 int ret;
	 Eina_List *list = NULL;

	 ime_preedit_attribute *attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 0;
	 attr->length = 1;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_UNDERLINE;
	 list = eina_list_append(list, attr);

	 attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 1;
	 attr->length = 1;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_HIGHLIGHT;
	 list = eina_list_append(list, attr);

	 attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 2;
	 attr->length = 1;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_REVERSAL;
	 list = eina_list_append(list, attr);

	 ret = ime_update_preedit_string("abcd", list);
	 if (ret != IME_ERROR_NONE) {
		 EINA_LIST_FREE(list, attr)
			 free(attr);
	 }
 }
 * @endcode
 *
 * @see #ime_preedit_attribute
 * @see ime_commit_string()
 * @see ime_show_preedit_string()
 * @see ime_hide_preedit_string()
 * @see ime_update_preedit_cursor()
 */
int ime_update_preedit_string(const char *str, Eina_List *attrs);

/**
 * @brief Updates the cursor position in the preedit string.
 *
 * @since_tizen 6.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] pos The cursor position in the preedit string
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post This function is supposed to be followed by the ime_update_preedit_string() function.
 *
 * @code
 {
	 int ret;
	 Eina_List *list = NULL;

	 ime_preedit_attribute *attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 0;
	 attr->length = 1;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_UNDERLINE;
	 list = eina_list_append(list, attr);

	 attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 1;
	 attr->length = 3;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_REVERSAL;
	 list = eina_list_append(list, attr);

	 ret = ime_update_preedit_string("abcd", list);
	 if (ret != IME_ERROR_NONE) {
		 EINA_LIST_FREE(list, attr)
			 free(attr);
	 }
	 ret = ime_update_preedit_cursor(1);
 }
 * @endcode
 *
 * @see ime_show_preedit_string()
 * @see ime_hide_preedit_string()
 * @see ime_update_preedit_string()
 */
int ime_update_preedit_cursor(unsigned int pos);

/**
 * @brief Requests the surrounding text from the position of the cursor, asynchronously.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] maxlen_before The maximum length of string to be retrieved before the cursor; -1 means unlimited
 * @param[in] maxlen_after The maximum length of string to be retrieved after the cursor; -1 means unlimited
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @pre The ime_surrounding_text_updated_cb() callback function MUST be set by ime_event_set_surrounding_text_updated_cb().
 *
 * @post The requested surrounding text can be received using the ime_surrounding_text_updated_cb() callback function.
 *
 * @see ime_delete_surrounding_text()
 * @see ime_event_set_surrounding_text_updated_cb()
 * @see ime_surrounding_text_updated_cb()
 */
int ime_request_surrounding_text(int maxlen_before, int maxlen_after);

/**
 * @brief Requests to delete surrounding text.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] offset The offset value from the cursor position (in characters not bytes). For example, please use -3 if you want to remove 3 characters in front of current cursor position.
 * @param[in] len The length of the text to delete (in characters not bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_request_surrounding_text()
 */
int ime_delete_surrounding_text(int offset, int len);

/**
 * @brief Gets the surrounding text from the position of the cursor, synchronously.
 *
 * @since_tizen 3.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a text must be released using free().
 *
 * @param[in] maxlen_before The maximum length of string to be retrieved before the cursor; -1 means unlimited
 * @param[in] maxlen_after The maximum length of string to be retrieved after the cursor; -1 means unlimited
 * @param[out] text The surrounding text
 * @param[out] cursor_pos The cursor position
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 * @retval #IME_ERROR_OUT_OF_MEMORY Failed to obtain text due to out of memory
 *
 * @see ime_delete_surrounding_text()
 */
int ime_get_surrounding_text(int maxlen_before, int maxlen_after, char **text, int *cursor_pos);

/**
 * @brief Requests to set selection.
 *
 * @since_tizen 3.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] start The start cursor position in text (in characters not bytes)
 * @param[in] end The end cursor position in text (in characters not bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 */
int ime_set_selection(int start, int end);

/**
 * @brief Gets the selected text synchronously.
 *
 * @details If multi-line text is selected, the result will contain '\n' for each newline character.
 * And if the selected text is empty, the result will be an empty string.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a text must be released using free().
 *
 * @param[out] text The selected text
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 */
int ime_get_selected_text(char **text);

/**
 * @brief Gets the pointer of input panel main window.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The specific error code can be obtained using the get_last_result() method if this function returns NULL.
 * @remarks The returned value should not be released. The returned value is managed by the platform and will be released when terminating this process.
 *
 * @return The input panel main window object on success, otherwise NULL
 *
 * @exception #IME_ERROR_NONE Successful
 * @exception #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @exception #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 * @exception #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @see get_last_result()
 * @see ime_create_cb()
 * @see ime_terminate_cb()
 * @see ime_show_cb()
 * @see ime_hide_cb()
 */
Evas_Object* ime_get_main_window(void);

/**
 * @brief Updates the input panel window's size information.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
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
int ime_set_size(int portrait_width, int portrait_height, int landscape_width, int landscape_height);

/**
 * @brief Requests to create an option window from the input panel.
 *
 * @details The input panel can call this function to open the option window. This
 * function calls ime_option_window_created_cb() callback function with
 * #IME_OPTION_WINDOW_TYPE_KEYBOARD parameter.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The ime_option_window_created_cb() and ime_option_window_destroyed_cb()
 * callback functions MUST be set by ime_event_set_option_window_created_cb() and
 * ime_event_set_option_window_destroyed_cb() respectively.
 *
 * @post This function calls ime_option_window_created_cb() callback function to
 * create the option window. And ime_destroy_option_window() function can be called
 * to close the option window.
 *
 * @see ime_event_set_option_window_created_cb()
 * @see ime_option_window_created_cb()
 * @see ime_destroy_option_window()
 */
int ime_create_option_window(void);

/**
 * @brief Requests to destroy an option window.
 *
 * @details The input panel can call this function to close the option window which
 * is created from either the input panel or Settings application.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] window The option window to destroy
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @pre The ime_option_window_created_cb() and ime_option_window_destroyed_cb()
 * callback functions MUST be set by ime_event_set_option_window_created_cb() and
 * ime_event_set_option_window_destroyed_cb() respectively.
 *
 * @post This function calls ime_option_window_destroyed_cb() callback function
 * to destroy the option window.
 *
 * @see ime_event_set_option_window_destroyed_cb()
 * @see ime_option_window_destroyed_cb()
 * @see ime_create_option_window()
 */
int ime_destroy_option_window(Evas_Object *window);

/**
 * @brief Gets the layout information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the layout information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] layout Layout information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 * @see ime_layout_set_cb()
 */
int ime_context_get_layout(ime_context_h context, Ecore_IMF_Input_Panel_Layout *layout);

/**
 * @brief Gets the layout variation information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the layout variation information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] layout_variation Layout variation information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 * @see #ime_layout_variation_e
 */
int ime_context_get_layout_variation(ime_context_h context, ime_layout_variation_e *layout_variation);

/**
 * @brief Gets the cursor position information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the cursor position information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] cursor_pos Cursor position information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 * @see ime_cursor_position_updated_cb()
 */
int ime_context_get_cursor_position(ime_context_h context, int *cursor_pos);

/**
 * @brief Gets the autocapital type information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the autocapital type information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] autocapital_type Autocapital type information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 */
int ime_context_get_autocapital_type(ime_context_h context, Ecore_IMF_Autocapital_Type *autocapital_type);

/**
 * @brief Gets the @c Return key label type information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the @c Return key label type information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] return_key_type The @c Return key label type information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 * @see ime_return_key_type_set_cb()
 */
int ime_context_get_return_key_type(ime_context_h context, Ecore_IMF_Input_Panel_Return_Key_Type *return_key_type);

/**
 * @brief Gets the @c Return key state information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the @c Return key state information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] return_key_state The @c Return key state information \n @c true to enable @c Return key
 * button, @c false to disable @c Return key button
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 * @see ime_return_key_state_set_cb()
 */
int ime_context_get_return_key_state(ime_context_h context, bool *return_key_state);

/**
 * @brief Gets the prediction mode information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the prediction mode information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] prediction_mode Prediction mode information \n @c true to allow the predictive
 * text feature if available, @c false to disable the predictive text feature
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 */
int ime_context_get_prediction_mode(ime_context_h context, bool *prediction_mode);

/**
 * @brief Gets the password mode information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the password mode information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks If @a password_mode is @c true, the input panel is advised not to support the predictive text.
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] password_mode Password mode information \n @c true to indicate that a password being inputted,
 * @c false to indicate non-password edit field.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 */
int ime_context_get_password_mode(ime_context_h context, bool *password_mode);

/**
 * @brief Gets the input hint information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the input hint information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a input_hint is a bit-wise value which recommends the input panel provide
 * an auto completion and so on if it is capable of supporting such features.
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] input_hint Input hint information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 */
int ime_context_get_input_hint(ime_context_h context, Ecore_IMF_Input_Hints *input_hint);

/**
 * @brief Gets the text bidirectional information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the bidirectional information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] bidi Text bidirectional information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 */
int ime_context_get_bidi_direction(ime_context_h context, Ecore_IMF_BiDi_Direction *bidi);

/**
 * @brief Gets the preferred language information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the preferred language information in ime_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] language Preferred language information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see ime_show_cb()
 */
int ime_context_get_language(ime_context_h context, Ecore_IMF_Input_Panel_Lang *language);

/**
 * @brief Gets the device name of the key event.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a dev_name must be released using free().
 *
 * @param[in] dev_info The device information from the key event
 * @param[out] dev_name The name of key input device. This can be an empty string if the device name is not available
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_process_key_event_cb()
 * @see ime_device_info_get_class()
 * @see ime_device_info_get_subclass()
 */
int ime_device_info_get_name(ime_device_info_h dev_info, char **dev_name);

/**
 * @brief Gets the device class of the key event.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] dev_info The device information from the key event
 * @param[out] dev_class The class of key input device. This can be #ECORE_IMF_DEVICE_CLASS_NONE if the device class is not available
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_process_key_event_cb()
 * @see ime_device_info_get_name()
 * @see ime_device_info_get_subclass()
 */
int ime_device_info_get_class(ime_device_info_h dev_info, Ecore_IMF_Device_Class *dev_class);
/**
 * @brief Gets the device subclass of the key event.
 *
 * @since_tizen 2.4
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] dev_info The device information from the key event
 * @param[out] dev_subclass The subclass of key input device. This can be #ECORE_IMF_DEVICE_SUBCLASS_NONE if the device subclass is not available
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_process_key_event_cb()
 * @see ime_device_info_get_name()
 * @see ime_device_info_get_class()
 */
int ime_device_info_get_subclass(ime_device_info_h dev_info, Ecore_IMF_Device_Subclass *dev_subclass);

/**
 * @brief Sets the event callback function that is called to set the prediction hint string to deliver to the input panel.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_prediction_hint_set_cb() callback function is called to set the prediction
 * hint string to deliver to the input panel.
 *
 * @param[in] callback_func The prediction hint event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_prediction_hint_set_cb()
 * @see ime_run()
 */
int ime_event_set_prediction_hint_set_cb(ime_prediction_hint_set_cb callback_func, void *user_data);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the text entry to set the MIME type.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_mime_type_set_request_cb() callback function is called when an associated text input
 * UI control requests the text entry to set the MIME type.
 *
 * @param[in] callback_func MIME type event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_mime_type_set_request_cb()
 * @see ime_run()
 */
int ime_event_set_mime_type_set_request_cb(ime_mime_type_set_request_cb callback_func, void *user_data);

/**
 * @brief Sends a private command to the associated text input UI control.
 *
 * @details This can be used by IME to deliver specific data to an application.
 * The data format MUST be negotiated by both application and IME.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] command The UTF-8 string to be sent
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 */
int ime_send_private_command(const char *command);

/**
 * @brief Commits contents such as image to the associated text input UI control.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] content The content URI to be sent
 * @param[in] description The content description
 * @param[in] mime_type The MIME type received from the ime_mime_type_set_request_cb()
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_mime_type_set_request_cb()
 * @see ime_event_set_mime_type_set_request_cb()
 */
int ime_commit_content(const char *content, const char *description, const char *mime_type);

/**
 * @brief Sets the floating mode or not.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] floating_mode @c true - floating mode on, @c false - floating mode off
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see ime_set_floating_drag_start()
 * @see ime_set_floating_drag_end()
 */
int ime_set_floating_mode(bool floating_mode);

/**
 * @brief Allows the floating input panel window to move along with the mouse pointer when the mouse is pressed.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks This function can be used in floating mode. If the floating mode is deactivated, calling this function has no effect.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @pre The floating mode was turned on with ime_set_floating_mode().
 *
 * @see ime_set_floating_mode()
 * @see ime_set_floating_drag_end()
 */
int ime_set_floating_drag_start(void);

/**
 * @brief Disallows the movement of the floating input panel window with the mouse pointer when the mouse is pressed.
 *
 * @details This function must be called after invoking ime_set_floating_drag_start(). Otherwise the call is ignored.
 *
 * @since_tizen 4.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks This function can be used in floating mode. If the floating mode is deactivated, calling this function has no effect.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @pre The floating mode was turned on with ime_set_floating_mode().
 * @pre ime_set_floating_drag_start() was called before.
 *
 * @see ime_set_floating_mode()
 * @see ime_set_floating_drag_start()
 */
int ime_set_floating_drag_end(void);

/**
 * @brief Sets the event callback function that is called to set key-value pairs of predicting messages to deliver to the input panel.
 *
 * @since_tizen 5.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_prediction_hint_data_set_cb() callback function is called to provide the prediction
 * hint key and value which can be delivered to the input panel.
 *
 * @param[in] callback_func The callback function to give hints
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_prediction_hint_data_set_cb()
 * @see ime_run()
 */
int ime_event_set_prediction_hint_data_set_cb(ime_prediction_hint_data_set_cb callback_func, void *user_data);

/**
 * @brief Sends the request to hide IME.
 *
 * @since_tizen 5.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 */
int ime_request_hide(void);

/**
 * @brief Updates the state of input panel event.
 *
 * @since_tizen 5.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] type The input panel event type
 * @param[in] value The value of event type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @see #ime_event_type_e
 */
int ime_update_input_panel_event(ime_event_type_e type, unsigned int value);

/**
 * @brief Enables whether candidate strings show or not.
 *
 * @since_tizen 5.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] visible @c true if candidate strings show, @c false otherwise.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 */
int ime_set_candidate_visibility_state(bool visible);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to set input hint.
 *
 * @since_tizen 6.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_input_hint_set_cb() callback function is called to set the input hint
 * to deliver to the input panel.
 *
 * @param[in] callback_func @c input_hint_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_input_hint_set_cb()
 * @see ime_run()
 */
int ime_event_set_input_hint_set_cb(ime_input_hint_set_cb callback_func, void *user_data);

/**
 * @brief Unsets @c input_hint_set event callback function.
 *
 * @since_tizen 6.5
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_input_hint_set_cb() callback function is called to set the input hint
 * to deliver to the input panel.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 *
 * @see ime_input_hint_set_cb()
 * @see ime_event_set_input_hint_set_cb()
 */
int ime_event_unset_input_hint_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control sets the position align of the input panel.
 *
 * @since_tizen 9.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_position_align_set_cb() callback function is called when an associated text input
 * UI control sets the position align of the input panel.
 *
 * @param[in] callback_func ime_position_align_set_cb() event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @post The ime_run() function should be called to start the IME application's main loop.
 *
 * @see ime_position_align_set_cb()
 * @see ime_run()
 */
int ime_event_set_position_align_set_cb(ime_position_align_set_cb callback_func, void *user_data);

/**
 * @brief Unsets ime_position_align_set_cb() event callback function.
 *
 * @since_tizen 9.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The ime_position_align_set_cb() callback function is called when an associated text input
 * UI control sets the position align of the input panel.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function
 *
 * @see ime_position_align_set_cb()
 * @see ime_event_set_position_align_set_cb()
 */
int ime_event_unset_position_align_set_cb(void);

/**
 * @brief Moves and resizes the floating input panel window.
 *
 * @details This function must be called after the ime_focus_in_cb() callback is called. Otherwise the call will be ignored.
 *
 * @since_tizen 9.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks Regardless of the rotation degree, the @a x, @a y values of the top-left corner on the screen are based on 0, 0.
 * This function can be used in floating mode. If the floating mode is deactivated, calling this function has no effect.
 *
 * @param[in] x The top-left x coordinate of the input panel
 * @param[in] y The top-left y coordinate of the input panel
 * @param[in] w The new width of the input panel
 * @param[in] h The new height of the input panel
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @pre The floating mode was turned on with ime_set_floating_mode().
 *
 * @see ime_set_floating_mode()
 */
int ime_move_resize_floating_window(int x, int y, int w, int h);

/**
 * @brief Updates a new preedit string with GList attributes.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] str The UTF-8 string to be updated in preedit
 * @param[in] attrs The GList which has #ime_preedit_attribute lists; @a str can be composed of multiple
 * string attributes: underline, highlight color and reversal color. The @a attrs will be released internally
 * on success and it can be NULL if no attributes to set
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #IME_ERROR_NONE No error
 * @retval #IME_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 * @retval #IME_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #IME_ERROR_NOT_RUNNING IME main loop is not started yet
 *
 * @post This function is supposed to be followed by the ime_show_preedit_string() function.
 *
 * @code
 {
	 int ret;
	 GList *list = NULL;
	 void *data;

	 ime_preedit_attribute *attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 0;
	 attr->length = 1;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_UNDERLINE;
	 list = g_list_append(list, attr);

	 attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 1;
	 attr->length = 1;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_HIGHLIGHT;
	 list = g_list_append(list, attr);

	 attr = calloc(1, sizeof(ime_preedit_attribute));
	 attr->start = 2;
	 attr->length = 1;
	 attr->type = IME_ATTR_FONTSTYLE;
	 attr->value = IME_ATTR_FONTSTYLE_REVERSAL;
	 list = g_list_append(list, attr);

	 ret = ime_update_preedit_string_with_glist("abcd", list);
	 if (ret != IME_ERROR_NONE) {
		 while (list) {
		 	 data = list->data;
			 attr = (ime_preedit_attribute *)data;
			 if (attr)
			  	free(attr);

			 list = g_list_next(list);
		 }
		 g_list_free(list);
	 }
 }
 * @endcode
 *
 * @see #ime_preedit_attribute
 * @see ime_commit_string()
 * @see ime_show_preedit_string()
 * @see ime_hide_preedit_string()
 * @see ime_update_preedit_cursor()
 */
int ime_update_preedit_string_with_glist(const char *str, GList *attrs);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_INPUTMETHOD_H__ */
