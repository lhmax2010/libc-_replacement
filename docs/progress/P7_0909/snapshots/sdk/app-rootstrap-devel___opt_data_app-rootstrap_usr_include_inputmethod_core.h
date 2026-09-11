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

#ifndef __TIZEN_UIX_INPUTMETHOD_CORE_H__
#define __TIZEN_UIX_INPUTMETHOD_CORE_H__

/**
 * @file inputmethod_core.h
 * @brief This file contains input method APIs and related enumeration.
 */

#include <stdbool.h>
#include <tizen.h>
#include <glib.h>
#include <tizen_core_imf.h>
#include <inputmethod_core_keydef.h>
#include <inputmethod_core_device_event.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for input method function error.
 *
 * @since_tizen 11.0
 */
typedef enum {
    INPUTMETHOD_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
    INPUTMETHOD_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
    INPUTMETHOD_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
    INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION = TIZEN_ERROR_IME | 0x0001, /**< Necessary callback function is not set */
    INPUTMETHOD_ERROR_OPERATION_FAILED = TIZEN_ERROR_IME | 0x0002, /**< Operation failed */
    INPUTMETHOD_ERROR_NOT_INITIALIZED = TIZEN_ERROR_IME | 0x0003, /**< Not initialized */
    INPUTMETHOD_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
} inputmethod_error_e;

/**
 * @brief Enumeration of layout variation.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_context_get_layout_variation()
 */
typedef enum {
    INPUTMETHOD_LAYOUT_NORMAL_VARIATION_NORMAL = 0, /**< The plain normal layout */
    INPUTMETHOD_LAYOUT_NORMAL_VARIATION_FILENAME, /**< Filename layout; symbols such as '/', '*', '\', '|', '&lt;', '&gt;', '?', '&quot;' and ':' should be disabled */
    INPUTMETHOD_LAYOUT_NORMAL_VARIATION_PERSON_NAME, /**< The name of a person */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_NORMAL = 0, /**< The plain normal number layout */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_SIGNED, /**< The number layout to allow a negative sign */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_DECIMAL, /**< The number layout to allow decimal point to provide fractional value */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_SIGNED_AND_DECIMAL, /**< The number layout to allow decimal point and negative sign */
    INPUTMETHOD_LAYOUT_PASSWORD_VARIATION_NORMAL = 0, /**< The normal password layout */
    INPUTMETHOD_LAYOUT_PASSWORD_VARIATION_NUMBERONLY, /**< The password layout to allow only number */
} inputmethod_layout_variation_e;

/**
 * @brief Enumeration of string attribute type.
 *
 * @since_tizen 11.0
 *
 * @remarks Currently, a font style is available to use.
 *
 * @see #inputmethod_preedit_attribute
 * @see inputmethod_update_preedit_string()
 */
typedef enum {
    INPUTMETHOD_ATTR_NONE, /**< No attribute */
    INPUTMETHOD_ATTR_FONTSTYLE, /**< A font style attribute, e.g., underline, etc. */
} inputmethod_attribute_type;

/**
 * @brief Enumeration containing input panel events.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_update_input_panel_event()
 */
typedef enum {
    INPUTMETHOD_EVENT_TYPE_LANGUAGE = 1, /**< The language of the input panel */
    INPUTMETHOD_EVENT_TYPE_SHIFT_MODE, /**< The shift key state of the input panel */
    INPUTMETHOD_EVENT_TYPE_GEOMETRY, /**< The size of the input panel */
} inputmethod_event_type_e;

/**
 * @brief Value for #INPUTMETHOD_ATTR_FONTSTYLE. Draw a line under the text.
 * @since_tizen 11.0
 */
#define INPUTMETHOD_ATTR_FONTSTYLE_UNDERLINE    1

/**
 * @brief Value for #INPUTMETHOD_ATTR_FONTSTYLE. Draw text in highlighted color.
 * @since_tizen 11.0
 */
#define INPUTMETHOD_ATTR_FONTSTYLE_HIGHLIGHT    2

/**
 * @brief Value for #INPUTMETHOD_ATTR_FONTSTYLE. Draw text in reversal color.
 * @since_tizen 11.0
 */
#define INPUTMETHOD_ATTR_FONTSTYLE_REVERSAL     4

/**
 * @brief The structure type to contain the attributes for preedit string.
 *
 * @since_tizen 11.0
 *
 * @remarks A preedit string may have one or more different attributes. This structure describes each attribute of the string.
 *
 * @see inputmethod_update_preedit_string()
 * @see #inputmethod_attribute_type
 */
typedef struct {
    unsigned int start; /**< The start position in the string of this attribute */
    unsigned int length; /**< The character length of this attribute, the range is [start, start+length] */
    inputmethod_attribute_type type; /**< The type of this attribute */
    unsigned int value; /**< The value of this attribute */
} inputmethod_preedit_attribute;

/**
 * @brief Handle of an associated text input UI control's input context.
 *
 * @details This is one of parameters of inputmethod_show_cb() callback function. IME application
 * should configure its input panel with this structure information.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_context_get_layout()
 * @see inputmethod_context_get_layout_variation()
 * @see inputmethod_context_get_cursor_position()
 * @see inputmethod_context_get_autocapital_type()
 * @see inputmethod_context_get_return_key_type()
 * @see inputmethod_context_get_return_key_state()
 * @see inputmethod_context_get_prediction_mode()
 * @see inputmethod_context_get_password_mode()
 * @see inputmethod_context_get_input_hint()
 * @see inputmethod_context_get_bidi_direction()
 * @see inputmethod_context_get_caps_mode()
 */
typedef struct _inputmethod_context *inputmethod_context_h;

/**
 * @brief Handle of the device information of the key event.
 *
 * @details This is one of parameters of inputmethod_process_key_event_cb() callback function. IME application
 * may distinguish the key event by using this if necessary.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_device_info_get_name()
 * @see inputmethod_device_info_get_class()
 * @see inputmethod_device_info_get_subclass()
 */
typedef struct _inputmethod_device_info *inputmethod_device_info_h;

/**
 * @brief Handle of the key information of the key event.
 *
 * @details This is one of parameters of inputmethod_process_key_event_cb() callback function. IME application
 * may distinguish the key event by using this if necessary.
 *
 * @since_tizen 11.0
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_key_info_get_code()
 * @see inputmethod_key_info_get_symbol()
 * @see inputmethod_key_info_get_mask()
 */
typedef struct _inputmethod_key_info *inputmethod_key_info_h;

/**
 * @brief Called when an associated text input UI control requests the input panel to show itself.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks IME application should configure its input panel with #inputmethod_context_h structure information.
 * @a context should not be released.
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] context The input context information handle
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_show_cb() function.
 *
 * @see inputmethod_event_set_show_cb()
 * @see inputmethod_context_get_layout()
 * @see inputmethod_context_get_layout_variation()
 * @see inputmethod_context_get_cursor_position()
 * @see inputmethod_context_get_autocapital_type()
 * @see inputmethod_context_get_return_key_type()
 * @see inputmethod_context_get_return_key_state()
 * @see inputmethod_context_get_prediction_mode()
 * @see inputmethod_context_get_password_mode()
 * @see inputmethod_context_get_input_hint()
 * @see inputmethod_context_get_bidi_direction()
 * @see inputmethod_context_get_caps_mode()
 */
typedef void (*inputmethod_show_cb)(int context_id, inputmethod_context_h context, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to hide itself.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_hide_cb() function.
 *
 * @see inputmethod_event_set_hide_cb()
 */
typedef void (*inputmethod_hide_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control has focus.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_focus_in_cb() function.
 *
 * @see inputmethod_event_set_focus_in_cb()
 */
typedef void (*inputmethod_focus_in_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control loses focus.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_focus_out_cb() function.
 *
 * @see inputmethod_event_set_focus_out_cb()
 */
typedef void (*inputmethod_focus_out_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control responds to a request with the surrounding text.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] text The UTF-8 string requested
 * @param[in] cursor_pos The cursor position
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_surrounding_text_updated_cb()
 */
typedef void (*inputmethod_surrounding_text_updated_cb)(int context_id, const char *text, int cursor_pos, void *user_data);

/**
 * @brief Called to reset the input context of an associated text input UI control.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_input_context_reset_cb()
 */
typedef void (*inputmethod_input_context_reset_cb)(void *user_data);

/**
 * @brief Called when the position of the cursor in an associated text input UI control changes.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] cursor_pos The cursor position
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_cursor_position_updated_cb()
 */
typedef void (*inputmethod_cursor_position_updated_cb)(int cursor_pos, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the language from the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The allocated @a lang_code will be released internally.
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] lang_code Input panel's current input language code (e.g., "en_US")
 *
 * @see inputmethod_event_set_language_requested_cb()
 */
typedef void (*inputmethod_language_requested_cb)(void *user_data, char **lang_code);

/**
 * @brief Called to set the preferred language to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] lang_code The preferred language code that the client application wants (e.g., "en_US")
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_language_set_cb()
 */
typedef void (*inputmethod_language_set_cb)(char *lang_code, void *user_data);

/**
 * @brief Called to set the application specific data to deliver to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] data The specific data to be set to the input panel
 * @param[in] data_length The length of data, in bytes
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_imdata_set_cb()
 */
typedef void (*inputmethod_imdata_set_cb)(void *data, unsigned int data_length, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the application specific data from the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] data Input panel's data to be set to the application
 * @param[out] data_length The length of data, in bytes
 *
 * @see inputmethod_event_set_imdata_requested_cb()
 */
typedef void (*inputmethod_imdata_requested_cb)(void *user_data, void **data, unsigned int *data_length);

/**
 * @brief Called when an associated text input UI control requests the input panel to set its layout.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] layout The input panel layout
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_layout_set_cb()
 */
typedef void (*inputmethod_layout_set_cb)(tizen_core_imf_input_panel_layout_e layout, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to set input hint.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] input_hint The input hint
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_input_hint_set_cb()
 */
typedef void (*inputmethod_input_hint_set_cb)(tizen_core_imf_input_hints_e input_hint, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to set the Return key label.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] type The type of Return key on the input panel
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_return_key_type_set_cb()
 */
typedef void (*inputmethod_return_key_type_set_cb)(tizen_core_imf_input_panel_return_key_type_e type, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to enable or disable the Return key state.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] disabled The Boolean state to disable Return key
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_return_key_state_set_cb()
 */
typedef void (*inputmethod_return_key_state_set_cb)(bool disabled, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the position and size from the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] x The x position in screen
 * @param[out] y The y position in screen
 * @param[out] w The window width
 * @param[out] h The window height
 *
 * @see inputmethod_event_set_geometry_requested_cb()
 */
typedef void (*inputmethod_geometry_requested_cb)(void *user_data, int *x, int *y, int *w, int *h);

/**
 * @brief Called when a key event is received from external devices or inputmethod_send_key_event().
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a key_info and @a dev_info should not be released by the application.
 *
 * @param[in] key_info The key information handle
 * @param[in] dev_info The device information handle
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @return @c true if the event was processed, otherwise the event was forwarded to the client application.
 *
 * @see inputmethod_event_set_process_key_event_cb()
 * @see inputmethod_key_info_get_code()
 * @see inputmethod_key_info_get_symbol()
 * @see inputmethod_key_info_get_mask()
 */
typedef bool (*inputmethod_process_key_event_cb)(inputmethod_key_info_h key_info, inputmethod_device_info_h dev_info, void *user_data);

/**
 * @brief Called when the system display language is changed.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] language The language code
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_display_language_changed_cb()
 */
typedef void (*inputmethod_display_language_changed_cb)(const char *language, void *user_data);

/**
 * @brief Called when the device is rotated.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] degree The rotation degree
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_rotation_degree_changed_cb()
 */
typedef void (*inputmethod_rotation_degree_changed_cb)(int degree, void *user_data);

/**
 * @brief Called when Accessibility in Settings application is on or off.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] state Accessibility option state
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_event_set_accessibility_state_changed_cb()
 */
typedef void (*inputmethod_accessibility_state_changed_cb)(bool state, void *user_data);

/**
 * @brief Called to set the prediction hint string to deliver to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] prediction_hint The prediction hint to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 *
 * @see inputmethod_event_set_prediction_hint_set_cb()
 */
typedef void (*inputmethod_prediction_hint_set_cb)(const char *prediction_hint, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the text entry to set the MIME type.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] mime_type The MIME type to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 *
 * @see inputmethod_event_set_mime_type_set_request_cb()
 */
typedef void (*inputmethod_mime_type_set_request_cb)(const char *mime_type, void *user_data);

/**
 * @brief Called to set key-value pairs of predicting messages to deliver to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key The prediction hint key
 * @param[in] value The prediction hint value
 * @param[in] user_data User data to be passed to the callback function
 *
 * @see inputmethod_event_set_prediction_hint_data_set_cb()
 */
typedef void (*inputmethod_prediction_hint_data_set_cb)(const char *key, const char *value, void *user_data);

/**
 * @brief Called when an associated text input UI control sets the position align of the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] x The x coordinate of the align
 * @param[in] y The y coordinate of the align
 * @param[in] align The position alignment that the client application wants
 * @param[in] user_data User data to be passed to the callback function
 *
 * @see inputmethod_event_set_position_align_set_cb()
 */
typedef void (*inputmethod_position_align_set_cb)(int x, int y, tizen_core_imf_input_panel_align_e align, void *user_data);

/**
 * @brief Called when a caps mode is changed.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] mode Caps mode
 * @param[in] user_data User data to be passed from the callback registration function
 */
typedef void (*inputmethod_caps_mode_changed_cb)(int mode, void *user_data);

/**
 * @brief Called when an autocapital type is set.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] type The autocapital type to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_autocapital_type_set_cb)(tizen_core_imf_autocapital_type_e type, void *user_data);

/**
 * @brief Called when a prediction allow is set.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] prediction_allow The prediction allow to be set to the input panel
 * @param[in] user_data User data to be passed to the callback function
 */
typedef void (*inputmethod_prediction_allow_set_cb)(uint32_t prediction_allow, void *user_data);

/* ======================== Library initialization functions ======================== */

/**
 * @brief Initializes the Inputmethod library.
 *
 * @details This function initializes the Inputmethod library. If this API is not called, all other APIs
 * will fail and return INPUTMETHOD_ERROR_NOT_INITIALIZED error.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_PERMISSION_DENIED The application does not have the privilege to call this function.
 *
 * @see inputmethod_shutdown()
 */
inputmethod_error_e inputmethod_initialize(void);

/**
 * @brief Shuts down the Inputmethod library.
 *
 * @details This function shuts down the Inputmethod library. It should be called when IME application no longer
 * needs to use the Inputmethod library.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_initialize()
 */
inputmethod_error_e inputmethod_shutdown(void);

/* ======================== Event set/unset functions ======================== */

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to show itself.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_show_cb() callback function is called when an associated text input
 * UI control requests the input panel to show itself.
 *
 * @param[in] callback_func @c show event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 * @see inputmethod_event_unset_show_cb()
 */
inputmethod_error_e inputmethod_event_set_show_cb(inputmethod_show_cb callback_func, void *user_data);

/**
 * @brief Unsets the show event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_show_cb()
 */
inputmethod_error_e inputmethod_event_unset_show_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to hide itself.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_hide_cb() callback function is called when an associated text input
 * UI control requests the input panel to hide itself.
 *
 * @param[in] callback_func @c hide event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_hide_cb()
 * @see inputmethod_event_unset_hide_cb()
 */
inputmethod_error_e inputmethod_event_set_hide_cb(inputmethod_hide_cb callback_func, void *user_data);

/**
 * @brief Unsets the hide event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_hide_cb()
 */
inputmethod_error_e inputmethod_event_unset_hide_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control has focus.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_focus_in_cb() callback function is called when an associated text input
 * UI control has focus.
 *
 * @param[in] callback_func @c focus_in event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_focus_in_cb()
 * @see inputmethod_event_unset_focus_in_cb()
 */
inputmethod_error_e inputmethod_event_set_focus_in_cb(inputmethod_focus_in_cb callback_func, void *user_data);

/**
 * @brief Unsets the focus_in event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_focus_in_cb()
 */
inputmethod_error_e inputmethod_event_unset_focus_in_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control loses focus.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_focus_out_cb() callback function is called when an associated text input
 * UI control loses focus.
 *
 * @param[in] callback_func @c focus_out event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_focus_out_cb()
 * @see inputmethod_event_unset_focus_out_cb()
 */
inputmethod_error_e inputmethod_event_set_focus_out_cb(inputmethod_focus_out_cb callback_func, void *user_data);

/**
 * @brief Unsets the focus_out event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_focus_out_cb()
 */
inputmethod_error_e inputmethod_event_unset_focus_out_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control responds to a request with the surrounding text.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_surrounding_text_updated_cb() callback function is called when an
 * associated text input UI control responds to a request with the surrounding text.
 *
 * @param[in] callback_func @c surrounding_text_updated event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_surrounding_text_updated_cb()
 * @see inputmethod_event_unset_surrounding_text_updated_cb()
 * @see inputmethod_request_surrounding_text()
 */
inputmethod_error_e inputmethod_event_set_surrounding_text_updated_cb(inputmethod_surrounding_text_updated_cb callback_func, void *user_data);

/**
 * @brief Unsets the surrounding_text_updated event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_surrounding_text_updated_cb()
 */
inputmethod_error_e inputmethod_event_unset_surrounding_text_updated_cb(void);

/**
 * @brief Sets the event callback function that is called to reset the input context of an associated text input UI control.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_input_context_reset_cb() callback function is called to reset the input
 * context of an associated text input UI control.
 *
 * @param[in] callback_func @c input_context_reset event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_input_context_reset_cb()
 * @see inputmethod_event_unset_input_context_reset_cb()
 */
inputmethod_error_e inputmethod_event_set_input_context_reset_cb(inputmethod_input_context_reset_cb callback_func, void *user_data);

/**
 * @brief Unsets the input_context_reset event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_input_context_reset_cb()
 */
inputmethod_error_e inputmethod_event_unset_input_context_reset_cb(void);

/**
 * @brief Sets the event callback function that is called when the position of the cursor in an associated text input UI control changes.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_cursor_position_updated_cb() callback function is called when the position
 * of the cursor in an associated text input UI control changes.
 *
 * @param[in] callback_func @c cursor_position_updated event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_cursor_position_updated_cb()
 * @see inputmethod_event_unset_cursor_position_updated_cb()
 */
inputmethod_error_e inputmethod_event_set_cursor_position_updated_cb(inputmethod_cursor_position_updated_cb callback_func, void *user_data);

/**
 * @brief Unsets the cursor_position_updated event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_cursor_position_updated_cb()
 */
inputmethod_error_e inputmethod_event_unset_cursor_position_updated_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the language from the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_language_requested_cb() callback function is called when an associated
 * text input UI control requests the language from the input panel.
 *
 * @param[in] callback_func @c language_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_language_requested_cb()
 * @see inputmethod_event_unset_language_requested_cb()
 */
inputmethod_error_e inputmethod_event_set_language_requested_cb(inputmethod_language_requested_cb callback_func, void *user_data);

/**
 * @brief Unsets the language_requested event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_language_requested_cb()
 */
inputmethod_error_e inputmethod_event_unset_language_requested_cb(void);

/**
 * @brief Sets the event callback function that is called to set the preferred language to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_language_set_cb() callback function is called when the client application
 * changes the preferred language after the input panel is shown.
 *
 * @param[in] callback_func @c language_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_language_set_cb()
 * @see inputmethod_event_unset_language_set_cb()
 */
inputmethod_error_e inputmethod_event_set_language_set_cb(inputmethod_language_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the language_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_language_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_language_set_cb(void);

/**
 * @brief Sets the event callback function that is called to set the application specific data to deliver to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_imdata_set_cb() callback function is called to set the application
 * specific data to deliver to the input panel.
 *
 * @param[in] callback_func @c imdata_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_imdata_set_cb()
 * @see inputmethod_event_unset_imdata_set_cb()
 * @see inputmethod_event_set_imdata_requested_cb()
 */
inputmethod_error_e inputmethod_event_set_imdata_set_cb(inputmethod_imdata_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the imdata_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_imdata_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_imdata_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the application specific data from the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_imdata_requested_cb() callback function is called when an associated
 * text input UI control requests the application specific data from the input panel.
 *
 * @param[in] callback_func @c imdata_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_imdata_requested_cb()
 * @see inputmethod_event_unset_imdata_requested_cb()
 * @see inputmethod_event_set_imdata_set_cb()
 */
inputmethod_error_e inputmethod_event_set_imdata_requested_cb(inputmethod_imdata_requested_cb callback_func, void *user_data);

/**
 * @brief Unsets the imdata_requested event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_imdata_requested_cb()
 */
inputmethod_error_e inputmethod_event_unset_imdata_requested_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to set its layout.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_layout_set_cb() callback function is called when an associated text input
 * UI control requests the input panel to set its layout.
 * @a layout information is already set to the input panel when it is shown through #inputmethod_context_h.
 * This callback function will be only called when the client application changes the edit field's layout
 * attribute after the input panel is shown.
 *
 * @param[in] callback_func @c layout_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_layout_set_cb()
 * @see inputmethod_event_unset_layout_set_cb()
 */
inputmethod_error_e inputmethod_event_set_layout_set_cb(inputmethod_layout_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the layout_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_layout_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_layout_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to set input hint.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_input_hint_set_cb() callback function is called when an associated text input
 * UI control requests the input panel to set input hint.
 * @a input_hint information is already set to the input panel when it is shown through #inputmethod_context_h.
 * This callback function will be only called when the client application changes the edit field's input hint
 * attribute after the input panel is shown.
 *
 * @param[in] callback_func @c input_hint_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_input_hint_set_cb()
 * @see inputmethod_event_unset_input_hint_set_cb()
 */
inputmethod_error_e inputmethod_event_set_input_hint_set_cb(inputmethod_input_hint_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the input_hint_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_input_hint_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_input_hint_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to set the @c Return key label.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_return_key_type_set_cb() callback function is called when an associated
 * text input UI control requests the input panel to set the @c Return key label.
 * @a type information is already set to the input panel when it is shown through #inputmethod_context_h.
 * This callback function will be only called when the client application changes the edit field's
 * @c Return key type attribute after the input panel is shown.
 *
 * @param[in] callback_func @c return_key_type_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_return_key_type_set_cb()
 * @see inputmethod_event_unset_return_key_type_set_cb()
 */
inputmethod_error_e inputmethod_event_set_return_key_type_set_cb(inputmethod_return_key_type_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the return_key_type_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_return_key_type_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_return_key_type_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the input panel to control the @c Return key state.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_return_key_state_set_cb() callback function is called when an associated
 * text input UI control requests the input panel to enable or disable the @c Return key state.
 * @a disabled information is already set to the input panel when it is shown through #inputmethod_context_h.
 * This callback function will be only called when the client application changes the edit field's
 * @c Return key disable attribute after the input panel is shown.
 *
 * @param[in] callback_func @c return_key_state_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_return_key_state_set_cb()
 * @see inputmethod_event_unset_return_key_state_set_cb()
 */
inputmethod_error_e inputmethod_event_set_return_key_state_set_cb(inputmethod_return_key_state_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the return_key_state_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_return_key_state_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_return_key_state_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the position and size from the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_geometry_requested_cb() callback function is called when an associated
 * text input UI control requests the position and size from the input panel.
 *
 * @param[in] callback_func @c geometry_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_geometry_requested_cb()
 * @see inputmethod_event_unset_geometry_requested_cb()
 */
inputmethod_error_e inputmethod_event_set_geometry_requested_cb(inputmethod_geometry_requested_cb callback_func, void *user_data);

/**
 * @brief Unsets the geometry_requested event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_geometry_requested_cb()
 */
inputmethod_error_e inputmethod_event_unset_geometry_requested_cb(void);

/**
 * @brief Sets the event callback function that is called when a key event is received from external devices or inputmethod_send_key_event().
 *
 * @details This function processes the key event before an associated text input UI control does.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_process_key_event_cb() callback function is called when the key event
 * is received from external keyboard devices or inputmethod_send_key_event().
 * If the key event is from the external device, @a dev_info will have its name, class and subclass information.
 *
 * @param[in] callback_func @c process_key_event event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_event_unset_process_key_event_cb()
 * @see inputmethod_send_key_event()
 */
inputmethod_error_e inputmethod_event_set_process_key_event_cb(inputmethod_process_key_event_cb callback_func, void *user_data);

/**
 * @brief Unsets the process_key_event event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_process_key_event_cb()
 */
inputmethod_error_e inputmethod_event_unset_process_key_event_cb(void);

/**
 * @brief Sets the event callback function that is called when the system display language is changed.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_display_language_changed_cb() callback function is called when the system
 * display language is changed.
 *
 * @param[in] callback_func @c display_language_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_display_language_changed_cb()
 * @see inputmethod_event_unset_display_language_changed_cb()
 */
inputmethod_error_e inputmethod_event_set_display_language_changed_cb(inputmethod_display_language_changed_cb callback_func, void *user_data);

/**
 * @brief Unsets the display_language_changed event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_display_language_changed_cb()
 */
inputmethod_error_e inputmethod_event_unset_display_language_changed_cb(void);

/**
 * @brief Sets the event callback function that is called when the device is rotated.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_rotation_degree_changed_cb() callback function is called when the device
 * is rotated.
 *
 * @param[in] callback_func @c rotation_degree_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_rotation_degree_changed_cb()
 * @see inputmethod_event_unset_rotation_degree_changed_cb()
 */
inputmethod_error_e inputmethod_event_set_rotation_degree_changed_cb(inputmethod_rotation_degree_changed_cb callback_func, void *user_data);

/**
 * @brief Unsets the rotation_degree_changed event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_rotation_degree_changed_cb()
 */
inputmethod_error_e inputmethod_event_unset_rotation_degree_changed_cb(void);

/**
 * @brief Sets the event callback function that is called when Accessibility in Settings application is on or off.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_accessibility_state_changed_cb() callback function is called when
 * Accessibility in Settings application is on or off.
 *
 * @param[in] callback_func @c accessibility_state_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_accessibility_state_changed_cb()
 * @see inputmethod_event_unset_accessibility_state_changed_cb()
 */
inputmethod_error_e inputmethod_event_set_accessibility_state_changed_cb(inputmethod_accessibility_state_changed_cb callback_func, void *user_data);

/**
 * @brief Unsets the accessibility_state_changed event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_accessibility_state_changed_cb()
 */
inputmethod_error_e inputmethod_event_unset_accessibility_state_changed_cb(void);

/**
 * @brief Sets the event callback function that is called to set the prediction hint string to deliver to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_prediction_hint_set_cb() callback function is called when an application
 * delivers a prediction hint message to the input panel.
 *
 * @param[in] callback_func @c prediction_hint_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_prediction_hint_set_cb()
 * @see inputmethod_event_unset_prediction_hint_set_cb()
 */
inputmethod_error_e inputmethod_event_set_prediction_hint_set_cb(inputmethod_prediction_hint_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the prediction_hint_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_prediction_hint_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_prediction_hint_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control requests the text entry to set the MIME type.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_mime_type_set_request_cb() callback function is called when an application
 * delivers the MIME type to the input panel.
 *
 * @param[in] callback_func @c mime_type_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_mime_type_set_request_cb()
 * @see inputmethod_event_unset_mime_type_set_request_cb()
 */
inputmethod_error_e inputmethod_event_set_mime_type_set_request_cb(inputmethod_mime_type_set_request_cb callback_func, void *user_data);

/**
 * @brief Unsets the mime_type_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_mime_type_set_request_cb()
 */
inputmethod_error_e inputmethod_event_unset_mime_type_set_request_cb(void);

/**
 * @brief Sets the event callback function that is called to set key-value pairs of predicting messages to deliver to the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_prediction_hint_data_set_cb() callback function is called when an application
 * delivers key-value prediction hint messages to the input panel.
 *
 * @param[in] callback_func @c prediction_hint_data_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_prediction_hint_data_set_cb()
 * @see inputmethod_event_unset_prediction_hint_data_set_cb()
 */
inputmethod_error_e inputmethod_event_set_prediction_hint_data_set_cb(inputmethod_prediction_hint_data_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the prediction_hint_data_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_prediction_hint_data_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_prediction_hint_data_set_cb(void);

/**
 * @brief Sets the event callback function that is called when an associated text input UI control sets the position align of the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_position_align_set_cb() callback function is called when a client application
 * sets the position alignment of the input panel.
 * Regardless of the rotation degree, the @a x, @a y values of the top-left corner on the screen are based on 0, 0.
 * This function is used to adjust the position of the input panel when its size changes in floating mode.
 *
 * @param[in] callback_func @c position_align_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_position_align_set_cb()
 * @see inputmethod_event_unset_position_align_set_cb()
 */
inputmethod_error_e inputmethod_event_set_position_align_set_cb(inputmethod_position_align_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the position_align_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_position_align_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_position_align_set_cb(void);

/**
 * @brief Sets the event callback function that is called when a caps mode is changed.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_caps_mode_changed_cb() callback function is called when an associated text input
 * UI control sends the change of caps mode.
 *
 * @param[in] callback_func @c caps_mode_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_caps_mode_changed_cb()
 */
inputmethod_error_e inputmethod_event_set_caps_mode_changed_cb(inputmethod_caps_mode_changed_cb callback_func, void *user_data);

/**
 * @brief Unsets the caps_mode_changed event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_caps_mode_changed_cb()
 */
inputmethod_error_e inputmethod_event_unset_caps_mode_changed_cb(void);

/**
 * @brief Sets the event callback function that is called when an autocapital type is set.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c autocapital_type_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_autocapital_type_set_cb()
 */
inputmethod_error_e inputmethod_event_set_autocapital_type_set_cb(inputmethod_autocapital_type_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the autocapital_type_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_autocapital_type_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_autocapital_type_set_cb(void);

/**
 * @brief Sets the event callback function that is called when a prediction allow is set.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] callback_func @c prediction_allow_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_prediction_allow_set_cb()
 */
inputmethod_error_e inputmethod_event_set_prediction_allow_set_cb(inputmethod_prediction_allow_set_cb callback_func, void *user_data);

/**
 * @brief Unsets the prediction_allow_set event callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_event_set_prediction_allow_set_cb()
 */
inputmethod_error_e inputmethod_event_unset_prediction_allow_set_cb(void);

/* ======================== Key info functions ======================== */

/**
 * @brief Gets the key code from the key info handle.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key_info The key information handle
 * @param[out] key_code The hardware key code of the key event
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_key_info_get_symbol()
 * @see inputmethod_key_info_get_mask()
 */
inputmethod_error_e inputmethod_key_info_get_code(inputmethod_key_info_h key_info, unsigned int *key_code);

/**
 * @brief Gets the key symbol from the key info handle.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key_info The key information handle
 * @param[out] key_symbol The key symbol of the key event
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_key_info_get_code()
 * @see inputmethod_key_info_get_mask()
 */
inputmethod_error_e inputmethod_key_info_get_symbol(inputmethod_key_info_h key_info, inputmethod_key_symbol_e *key_symbol);

/**
 * @brief Gets the key mask from the key info handle.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key_info The key information handle
 * @param[out] key_mask The modifier key mask of the key event
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_key_info_get_code()
 * @see inputmethod_key_info_get_symbol()
 */
inputmethod_error_e inputmethod_key_info_get_mask(inputmethod_key_info_h key_info, inputmethod_key_mask_e *key_mask);

/* ======================== Action functions ======================== */

/**
 * @brief Sends a key event to the associated text input UI control.
 *
 * @details This function sends key down or up event with key mask to the client application.
 * If @a forward_key is @c true, this key event goes to the edit field directly. And if @a forward_key
 * is @c false, the inputmethod_process_key_event_cb() callback function receives the key event before the edit field.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] key_symbol The key symbol to be sent
 * @param[in] key_mask The modifier key mask
 * @param[in] forward_key The flag to send the key event directly to the edit field
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_key_symbol_e
 * @see inputmethod_key_mask_e
 * @see inputmethod_process_key_event_cb()
 */
inputmethod_error_e inputmethod_send_key_event(inputmethod_key_symbol_e key_symbol, inputmethod_key_mask_e key_mask, bool forward_key);

/**
 * @brief Sends the text to the associated text input UI control.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] str The UTF-8 string to be committed
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_update_preedit_string()
 */
inputmethod_error_e inputmethod_commit_string(const char *str);

/**
 * @brief Updates a new preedit string and cursor position.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a attrs is a GList which has #inputmethod_preedit_attribute elements; @a str can be composed
 * of multiple string attributes: underline, highlight color and reversal color. The @a attrs will be
 * released internally on success and it can be NULL if no attributes to set.
 *
 * @param[in] str The UTF-8 string to be updated in preedit
 * @param[in] attrs The GList of #inputmethod_preedit_attribute for @a str; can be NULL
 * @param[in] pos The cursor position in the preedit string
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #inputmethod_preedit_attribute
 * @see inputmethod_commit_string()
 *
 * @code
 {
	 int ret;
	 GList *list = NULL;
	 void *data;

	 inputmethod_preedit_attribute *attr = calloc(1, sizeof(inputmethod_preedit_attribute));
	 attr->start = 0;
	 attr->length = 1;
	 attr->type = INPUTMETHOD_ATTR_FONTSTYLE;
	 attr->value = INPUTMETHOD_ATTR_FONTSTYLE_UNDERLINE;
	 list = g_list_append(list, attr);

	 attr = calloc(1, sizeof(inputmethod_preedit_attribute));
	 attr->start = 1;
	 attr->length = 1;
	 attr->type = INPUTMETHOD_ATTR_FONTSTYLE;
	 attr->value = INPUTMETHOD_ATTR_FONTSTYLE_HIGHLIGHT;
	 list = g_list_append(list, attr);

	 attr = calloc(1, sizeof(inputmethod_preedit_attribute));
	 attr->start = 2;
	 attr->length = 1;
	 attr->type = INPUTMETHOD_ATTR_FONTSTYLE;
	 attr->value = INPUTMETHOD_ATTR_FONTSTYLE_REVERSAL;
	 list = g_list_append(list, attr);

	 ret = inputmethod_update_preedit_string("abcd", list, 1);
	 if (ret != INPUTMETHOD_ERROR_NONE) {
		 while (list) {
		 	 data = list->data;
			 attr = (inputmethod_preedit_attribute *)data;
			 if (attr)
			  	free(attr);

			 list = g_list_next(list);
		 }
		 g_list_free(list);
	 }
 }
 * @endcode
 */
inputmethod_error_e inputmethod_update_preedit_string(const char *str, GList *attrs, unsigned int pos);

/**
 * @brief Requests the surrounding text from the position of the cursor, asynchronously.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks The inputmethod_surrounding_text_updated_cb() callback function must be set before calling
 * this function, otherwise #INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION is returned.
 *
 * @param[in] maxlen_before The maximum length of string to be retrieved before the cursor; -1 means unlimited
 * @param[in] maxlen_after The maximum length of string to be retrieved after the cursor; -1 means unlimited
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 *
 * @see inputmethod_event_set_surrounding_text_updated_cb()
 * @see inputmethod_delete_surrounding_text()
 */
inputmethod_error_e inputmethod_request_surrounding_text(int maxlen_before, int maxlen_after);

/**
 * @brief Requests to delete surrounding text.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] offset The offset value from the cursor position
 * @param[in] len The length of the text to be deleted; must be greater than 0
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_request_surrounding_text()
 */
inputmethod_error_e inputmethod_delete_surrounding_text(int offset, int len);

/**
 * @brief Gets the surrounding text from the position of the cursor, synchronously.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a text must be released using free() when it is no longer needed.
 *
 * @param[in] maxlen_before The maximum length of string to be retrieved before the cursor; -1 means unlimited
 * @param[in] maxlen_after The maximum length of string to be retrieved after the cursor; -1 means unlimited
 * @param[out] text The surrounding text
 * @param[out] cursor_pos The cursor position
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see inputmethod_request_surrounding_text()
 * @see inputmethod_delete_surrounding_text()
 */
inputmethod_error_e inputmethod_get_surrounding_text(int maxlen_before, int maxlen_after, char **text, int *cursor_pos);

/**
 * @brief Sets the selection range in the associated text input UI control.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] start The start cursor position in text (must be >= 0)
 * @param[in] end The end cursor position in text
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_get_selected_text()
 */
inputmethod_error_e inputmethod_set_selection(int start, int end);

/**
 * @brief Gets the currently selected text from the associated text input UI control.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a text must be released using free() when it is no longer needed.
 *
 * @param[out] text The selected text
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_set_selection()
 */
inputmethod_error_e inputmethod_get_selected_text(char **text);

/**
 * @brief Sends a private command to the associated text input UI control.
 *
 * @details This can be used by applications to provide specific information to the input panel.
 * The data format MUST be negotiated by both the application and the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] command The private command to be sent to the client application
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_send_private_command(const char *command);

/**
 * @brief Commits content such as an image to the associated text input UI control.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] content The content URI to be committed (e.g., "file:///path/to/image.png")
 * @param[in] description The content description
 * @param[in] mime_type The MIME type of the content (e.g., "image/png")
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 */
inputmethod_error_e inputmethod_commit_content(const char *content, const char *description, const char *mime_type);

/**
 * @brief Sets the floating mode of the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] floating_mode @c true to enable floating mode, @c false to disable
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_set_floating_drag_start()
 * @see inputmethod_set_floating_drag_end()
 * @see inputmethod_move_resize_floating_window()
 */
inputmethod_error_e inputmethod_set_floating_mode(bool floating_mode);

/**
 * @brief Enables drag of the floating input panel.
 *
 * @details This function must be called when the user starts dragging the floating input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_set_floating_mode()
 * @see inputmethod_set_floating_drag_end()
 */
inputmethod_error_e inputmethod_set_floating_drag_start(void);

/**
 * @brief Disables drag of the floating input panel.
 *
 * @details This function must be called when the user stops dragging the floating input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 *
 * @see inputmethod_set_floating_mode()
 * @see inputmethod_set_floating_drag_start()
 */
inputmethod_error_e inputmethod_set_floating_drag_end(void);

/**
 * @brief Requests to hide the input panel.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_request_hide(void);

/**
 * @brief Notifies the client application of a change in the input panel's state.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] type The type of input panel event
 * @param[in] value The value associated with the event type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #inputmethod_event_type_e
 */
inputmethod_error_e inputmethod_update_input_panel_event(inputmethod_event_type_e type, unsigned int value);

/**
 * @brief Sets the visibility state of the candidate string window.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] visible @c true to show the candidate string window, @c false to hide
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 */
inputmethod_error_e inputmethod_set_candidate_visibility_state(bool visible);

/**
 * @brief Moves and resizes the floating input panel window.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] x The x position of the window (must be >= 0)
 * @param[in] y The y position of the window (must be >= 0)
 * @param[in] w The width of the window (must be >= 0)
 * @param[in] h The height of the window (must be >= 0)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_set_floating_mode()
 */
inputmethod_error_e inputmethod_move_resize_floating_window(int x, int y, int w, int h);

/* ======================== Context getter functions ======================== */

/**
 * @brief Gets the layout information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] layout The input panel layout
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 * @see inputmethod_context_get_layout_variation()
 */
inputmethod_error_e inputmethod_context_get_layout(inputmethod_context_h context, tizen_core_imf_input_panel_layout_e *layout);

/**
 * @brief Gets the layout variation information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] layout_variation The input panel layout variation
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 * @see inputmethod_context_get_layout()
 * @see #inputmethod_layout_variation_e
 */
inputmethod_error_e inputmethod_context_get_layout_variation(inputmethod_context_h context, inputmethod_layout_variation_e *layout_variation);

/**
 * @brief Gets the cursor position information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] cursor_pos The cursor position in the edit field
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 */
inputmethod_error_e inputmethod_context_get_cursor_position(inputmethod_context_h context, int *cursor_pos);

/**
 * @brief Gets the autocapital type information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] autocapital_type The auto capital type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 */
inputmethod_error_e inputmethod_context_get_autocapital_type(inputmethod_context_h context, tizen_core_imf_autocapital_type_e *autocapital_type);

/**
 * @brief Gets the Return key type information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] return_key_type The type of Return key on the input panel
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 * @see inputmethod_context_get_return_key_state()
 */
inputmethod_error_e inputmethod_context_get_return_key_type(inputmethod_context_h context, tizen_core_imf_input_panel_return_key_type_e *return_key_type);

/**
 * @brief Gets the Return key state information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] return_key_state @c true if the Return key is disabled, @c false if enabled
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 * @see inputmethod_context_get_return_key_type()
 */
inputmethod_error_e inputmethod_context_get_return_key_state(inputmethod_context_h context, bool *return_key_state);

/**
 * @brief Gets the prediction mode information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] prediction_mode @c true if prediction is allowed, @c false otherwise
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 */
inputmethod_error_e inputmethod_context_get_prediction_mode(inputmethod_context_h context, bool *prediction_mode);

/**
 * @brief Gets the password mode information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] password_mode @c true if the edit field is in password mode, @c false otherwise
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 */
inputmethod_error_e inputmethod_context_get_password_mode(inputmethod_context_h context, bool *password_mode);

/**
 * @brief Gets the input hint information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] input_hint The input hint of the edit field
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 */
inputmethod_error_e inputmethod_context_get_input_hint(inputmethod_context_h context, tizen_core_imf_input_hints_e *input_hint);

/**
 * @brief Gets the bidirectional mode information from the input context.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] bidi The bidirectional mode of the edit field
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 */
inputmethod_error_e inputmethod_context_get_bidi_direction(inputmethod_context_h context, tizen_core_imf_bidi_direction_e *bidi);

/**
 * @brief Gets the caps mode information from the input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the caps mode information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] context The input context handle
 * @param[out] caps_mode Caps mode information; @c true to turn on shift mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_show_cb()
 */
inputmethod_error_e inputmethod_context_get_caps_mode(inputmethod_context_h context, bool *caps_mode);

/* ======================== Device info getter functions ======================== */

/**
 * @brief Gets the device name from the device info handle.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @remarks @a dev_name must be released using free() when it is no longer needed.
 *
 * @param[in] dev_info The device information handle
 * @param[out] dev_name The name of the input device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_device_info_get_class()
 * @see inputmethod_device_info_get_subclass()
 */
inputmethod_error_e inputmethod_device_info_get_name(inputmethod_device_info_h dev_info, char **dev_name);

/**
 * @brief Gets the device class from the device info handle.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] dev_info The device information handle
 * @param[out] dev_class The class of the input device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_device_info_get_name()
 * @see inputmethod_device_info_get_subclass()
 */
inputmethod_error_e inputmethod_device_info_get_class(inputmethod_device_info_h dev_info, tizen_core_imf_device_class_e *dev_class);

/**
 * @brief Gets the device subclass from the device info handle.
 *
 * @since_tizen 11.0
 *
 * @privlevel public
 *
 * @privilege %http://tizen.org/privilege/ime
 *
 * @param[in] dev_info The device information handle
 * @param[out] dev_subclass The subclass of the input device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_INITIALIZED Not initialized
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see inputmethod_process_key_event_cb()
 * @see inputmethod_device_info_get_name()
 * @see inputmethod_device_info_get_class()
 */
inputmethod_error_e inputmethod_device_info_get_subclass(inputmethod_device_info_h dev_info, tizen_core_imf_device_subclass_e *dev_subclass);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_INPUTMETHOD_CORE_H__ */
