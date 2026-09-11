/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_EDITOR_H__
#define __TIZEN_MEDIA_EDITOR_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_editor.h
 * @brief This file contains the media editor API, related structures and enumerations.
 * @since_tizen 7.0
 */

/**
 * @addtogroup CAPI_MEDIA_EDITOR_MODULE
 * @{
 */

#ifndef TIZEN_ERROR_MEDIA_EDITOR
#define TIZEN_ERROR_MEDIA_EDITOR        -0x030D0000 /* It'll be removed at end of ACR process */
#endif

/**
 * @brief Enumeration for the error codes of media editor.
 * @since_tizen 7.0
 */
typedef enum {
  MEDIAEDITOR_ERROR_NONE              = TIZEN_ERROR_NONE,                /**< Successful */
  MEDIAEDITOR_ERROR_NOT_SUPPORTED     = TIZEN_ERROR_NOT_SUPPORTED,       /**< Not supported */
  MEDIAEDITOR_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,   /**< Internal error */
  MEDIAEDITOR_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
  MEDIAEDITOR_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,   /**< The access to the resources can not be granted*/
  MEDIAEDITOR_ERROR_INVALID_STATE     = TIZEN_ERROR_MEDIA_EDITOR | 0x01, /**< Invalid state */
  MEDIAEDITOR_ERROR_RESOURCE_CONFLICT = TIZEN_ERROR_MEDIA_EDITOR | 0x02, /**< Blocked by resource conflicted */
  MEDIAEDITOR_ERROR_RESOURCE_FAILED   = TIZEN_ERROR_MEDIA_EDITOR | 0x03, /**< Blocked by resource failed */
} mediaeditor_error_e;

/**
 * @brief Enumeration for the media editor state.
 * @since_tizen 7.0
 */
typedef enum {
  MEDIAEDITOR_STATE_IDLE,      /**< Created, but not started to render */
  MEDIAEDITOR_STATE_RENDERING, /**< Start to rendering and saving */
  MEDIAEDITOR_STATE_PREVIEW    /**< Start to preview without saving */
} mediaeditor_state_e;

/**
 * @brief Enumeration for the media editor display type.
 * @since_tizen 7.0
 */
typedef enum {
  MEDIAEDITOR_DISPLAY_TYPE_OVERLAY, /**< Overlay surface display */
  MEDIAEDITOR_DISPLAY_TYPE_EVAS,    /**< Evas object surface display */
  MEDIAEDITOR_DISPLAY_TYPE_ECORE,   /**< Ecore object surface display */
  MEDIAEDITOR_DISPLAY_TYPE_NONE     /**< This disposes of buffers */
} mediaeditor_display_type_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_EDITOR_EFFECT_MODULE
 * @{
 */

/**
 * @brief Enumeration for the transition type.
 * @since_tizen 7.0
 */
typedef enum {
  MEDIAEDITOR_TRANSITION_TYPE_NONE = 0,                /**< Transition none */
  MEDIAEDITOR_TRANSITION_TYPE_BAR_WIPE_LR = 1,         /**< A bar moves from left to right */
  MEDIAEDITOR_TRANSITION_TYPE_BAR_WIPE_TB = 2,         /**< A bar moves from top to bottom */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_TL = 3,         /**< A box expands from the upper-left corner to the lower-right corner */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_TR = 4,         /**< A box expands from the upper-right corner to the lower-left corner */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_BR = 5,         /**< A box expands from the lower-right corner to the upper-left corner */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_BL = 6,         /**< A box expands from the lower-left corner to the upper-right corner */
  MEDIAEDITOR_TRANSITION_TYPE_FOUR_BOX_WIPE_CI = 7,    /**< A box shape expands from each of the four corners toward the center */
  MEDIAEDITOR_TRANSITION_TYPE_FOUR_BOX_WIPE_CO = 8,    /**< A box shape expands from the center of each quadrant toward the corners of each quadrant */
  MEDIAEDITOR_TRANSITION_TYPE_BARNDOOR_V = 21,         /**< A central, vertical line splits and expands toward the left and right edges */
  MEDIAEDITOR_TRANSITION_TYPE_BARNDOOR_H = 22,         /**< A central, horizontal line splits and expands toward the top and bottom edges */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_TC = 23,        /**< A box expands from the top edge's midpoint to the bottom corners */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_RC = 24,        /**< A box expands from the right edge's midpoint to the left corners */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_BC = 25,        /**< A box expands from the bottom edge's midpoint to the top corners */
  MEDIAEDITOR_TRANSITION_TYPE_BOX_WIPE_LC = 26,        /**< A box expands from the left edge's midpoint to the right corners */
  MEDIAEDITOR_TRANSITION_TYPE_DIAGONAL_TL = 41,        /**< A diagonal line moves from the upper-left corner to the lower-right corner */
  MEDIAEDITOR_TRANSITION_TYPE_DIAGONAL_TR = 42,        /**< A diagonal line moves from the upper right corner to the lower-left corner */
  MEDIAEDITOR_TRANSITION_TYPE_BOWTIE_V = 43,           /**< Two wedge shapes slide in from the top and bottom edges toward the center */
  MEDIAEDITOR_TRANSITION_TYPE_BOWTIE_H = 44,           /**< Two wedge shapes slide in from the left and right edges toward the center */
  MEDIAEDITOR_TRANSITION_TYPE_BARNDOOR_DBL = 45,       /**< A diagonal line from the lower-left to upper-right corners splits and expands toward the opposite corners */
  MEDIAEDITOR_TRANSITION_TYPE_BARNDOOR_DTL = 46,       /**< A diagonal line from upper-left to lower-right corners splits and expands toward the opposite corners */
  MEDIAEDITOR_TRANSITION_TYPE_MISC_DIAGONAL_DBD = 47,  /**< Four wedge shapes split from the center and retract toward the four edges */
  MEDIAEDITOR_TRANSITION_TYPE_MISC_DIAGONAL_DD = 48,   /**< A diamond connecting the four edge midpoints simultaneously contracts toward the center and expands toward the edges */
  MEDIAEDITOR_TRANSITION_TYPE_VEE_D = 61,              /**< A wedge shape moves from top to bottom */
  MEDIAEDITOR_TRANSITION_TYPE_VEE_L = 62,              /**< A wedge shape moves from right to left */
  MEDIAEDITOR_TRANSITION_TYPE_VEE_U = 63,              /**< A wedge shape moves from bottom to top */
  MEDIAEDITOR_TRANSITION_TYPE_VEE_R = 64,              /**< A wedge shape moves from left to right */
  MEDIAEDITOR_TRANSITION_TYPE_BARNVEE_D = 65,          /**< A 'V' shape extending from the bottom edge's midpoint to the opposite corners contracts toward the center and expands toward the edges */
  MEDIAEDITOR_TRANSITION_TYPE_BARNVEE_L = 66,          /**< A 'V' shape extending from the left edge's midpoint to the opposite corners contracts toward the center and expands toward the edges */
  MEDIAEDITOR_TRANSITION_TYPE_BARNVEE_U = 67,          /**< A 'V' shape extending from the top edge's midpoint to the opposite corners contracts toward the center and expands toward the edges */
  MEDIAEDITOR_TRANSITION_TYPE_BARNVEE_R = 68,          /**< A 'V' shape extending from the right edge's midpoint to the opposite corners contracts toward the center and expands toward the edges */
  MEDIAEDITOR_TRANSITION_TYPE_IRIS_RECT = 101,         /**< A rectangle expands from the center. */
  MEDIAEDITOR_TRANSITION_TYPE_CLOCK_CW12 = 201,        /**< A radial hand sweeps clockwise from the twelve o'clock position */
  MEDIAEDITOR_TRANSITION_TYPE_CLOCK_CW3 = 202,         /**< A radial hand sweeps clockwise from the three o'clock position */
  MEDIAEDITOR_TRANSITION_TYPE_CLOCK_CW6 = 203,         /**< A radial hand sweeps clockwise from the six o'clock position */
  MEDIAEDITOR_TRANSITION_TYPE_CLOCK_CW9 = 204,         /**< A radial hand sweeps clockwise from the nine o'clock position */
  MEDIAEDITOR_TRANSITION_TYPE_PINWHEEL_TBV = 205,      /**< Two radial hands sweep clockwise from the twelve and six o'clock positions */
  MEDIAEDITOR_TRANSITION_TYPE_PINWHEEL_TBH = 206,      /**< Two radial hands sweep clockwise from the nine and three o'clock positions */
  MEDIAEDITOR_TRANSITION_TYPE_PINWHEEL_FB = 207,       /**< Four radial hands sweep clockwise */
  MEDIAEDITOR_TRANSITION_TYPE_FAN_CT = 211,            /**< A fan unfolds from the top edge, the fan axis at the center */
  MEDIAEDITOR_TRANSITION_TYPE_FAN_CR = 212,            /**< A fan unfolds from the right edge, the fan axis at the center */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLEFAN_FOV = 213,     /**< Two fans, their axes at the center, unfold from the top and bottom */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLEFAN_FOH = 214,     /**< Two fans, their axes at the center, unfold from the left and right */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CWT = 221,   /**< A radial hand sweeps clockwise from the top edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CWR = 222,   /**< A radial hand sweeps clockwise from the right edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CWB = 223,   /**< A radial hand sweeps clockwise from the bottom edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CWL = 224,   /**< A radial hand sweeps clockwise from the left edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLESWEEP_PV = 225,    /**< Two radial hands sweep clockwise and counter-clockwise from the top and bottom edges' midpoints */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLESWEEP_PD = 226,    /**< Two radial hands sweep clockwise and counter-clockwise from the left and right edges' midpoints */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLESWEEP_OV = 227,    /**< Two radial hands attached at the top and bottom edges' midpoints sweep from right to left */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLESWEEP_OH = 228,    /**< Two radial hands attached at the left and right edges' midpoints sweep from top to bottom */
  MEDIAEDITOR_TRANSITION_TYPE_FAN_T = 231,             /**< A fan unfolds from the bottom, the fan axis at the top edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_FAN_R = 232,             /**< A fan unfolds from the left, the fan axis at the right edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_FAN_B = 233,             /**< A fan unfolds from the top, the fan axis at the bottom edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_FAN_L = 234,             /**< A fan unfolds from the right, the fan axis at the left edge's midpoint */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLEFAN_FIV = 235,     /**< Two fans, their axes at the top and bottom, unfold from the center */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLEFAN_FIH = 236,     /**< Two fans, their axes at the left and right, unfold from the center */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CWTL = 241,  /**< A radial hand sweeps clockwise from the upper-left corner */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CCWBL = 242, /**< A radial hand sweeps counter-clockwise from the lower-left corner */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CWBR = 243,  /**< A radial hand sweeps clockwise from the lower-right corner */
  MEDIAEDITOR_TRANSITION_TYPE_SINGLESWEEP_CCWTR = 244, /**< A radial hand sweeps counter-clockwise from the upper-right corner */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLESWEEP_PDTL = 245,  /**< Two radial hands attached at the upper-left and lower-right corners sweep down and up */
  MEDIAEDITOR_TRANSITION_TYPE_DOUBLESWEEP_PDBL = 246,  /**< Two radial hands attached at the lower-left and upper-right corners sweep down and up */
  MEDIAEDITOR_TRANSITION_TYPE_SALOONDOOR_T = 251,      /**< Two radial hands attached at the upper-left and upper-right corners sweep down */
  MEDIAEDITOR_TRANSITION_TYPE_SALOONDOOR_L = 252,      /**< Two radial hands attached at the upper-left and lower-left corners sweep to the right */
  MEDIAEDITOR_TRANSITION_TYPE_SALOONDOOR_B = 253,      /**< Two radial hands attached at the lower-left and lower-right corners sweep up */
  MEDIAEDITOR_TRANSITION_TYPE_SALOONDOOR_R = 254,      /**< Two radial hands attached at the upper-right and lower-right corners sweep to the left */
  MEDIAEDITOR_TRANSITION_TYPE_WINDSHIELD_R = 261,      /**< Two radial hands attached at the midpoints of the top and bottom halves sweep from right to left */
  MEDIAEDITOR_TRANSITION_TYPE_WINDSHIELD_U = 262,      /**< Two radial hands attached at the midpoints of the left and right halves sweep from top to bottom */
  MEDIAEDITOR_TRANSITION_TYPE_WINDSHIELD_V = 263,      /**< Two sets of radial hands attached at the midpoints of the top and bottom halves sweep from top to bottom and bottom to top */
  MEDIAEDITOR_TRANSITION_TYPE_WINDSHIELD_H = 264,      /**< Two sets of radial hands attached at the midpoints of the left and right halves sweep from left to right and right to left */
  MEDIAEDITOR_TRANSITION_TYPE_CROSSFADE = 512          /**< Crossfade */
} mediaeditor_transition_type_e;

/**
 * @brief Enumeration for the effect type.
 * @since_tizen 7.0
 * @remarks #MEDIAEDITOR_EFFECT_AUDIO_TYPE_FADE_IN and #MEDIAEDITOR_EFFECT_AUDIO_TYPE_FADE_OUT Can not be rolled back *
 */
typedef enum {
  MEDIAEDITOR_EFFECT_TYPE_NONE,               /**< None */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_EDGETV,       /**< Applies edge detect on video */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_AGINGTV,      /**< Adds age to video input using scratches and dust */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_DICETV,       /**< Dices the screen up into many small squares */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_WARPTV,       /**< Realtime goo'ing of the video input */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_SHAGADELICTV, /**< Makes images shagadelic */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_VERTIGOTV,    /**< A loopback alpha blending effector with rotating and scaling */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_REVTV,        /**< A video waveform monitor for each line of video processed */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_QUARKTV,      /**< Motion dissolver */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_OPTV,         /**< Optical art meets real-time video effect */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_RADIOACTV,    /**< Detects a difference from previous frame and blurs it */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_STREAKTV,     /**< Makes after images of moving objects */
  MEDIAEDITOR_EFFECT_VIDEO_TYPE_RIPPLETV,     /**< Makes ripple mark effect on the video input */
  MEDIAEDITOR_EFFECT_AUDIO_TYPE_FADE_IN,      /**< Audio fade in */
  MEDIAEDITOR_EFFECT_AUDIO_TYPE_FADE_OUT,     /**< Audio fade out */
  MEDIAEDITOR_EFFECT_AUDIO_TYPE_ECHO          /**< Adds an echo or reverb effect to an audio stream */
} mediaeditor_effect_type_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_EDITOR_MODULE
 * @{
 */


/**
 * @brief The media editor handle.
 * @since_tizen 7.0
 */
typedef void *mediaeditor_h;

/**
 * @brief The media editor display handle type.
 * @since_tizen 7.0
 */
typedef void *mediaeditor_display_h;

/**
 * @brief Called when an error occurs.
 * @details The following error codes can be received:\n
 *          #MEDIAEDITOR_ERROR_INVALID_OPERATION\n
 *          #MEDIAEDITOR_ERROR_INVALID_STATE\n
 *          #MEDIAEDITOR_ERROR_RESOURCE_CONFLICT\n
 *          #MEDIAEDITOR_ERROR_RESOURCE_FAILED
 *
 * @since_tizen 7.0
 * @param[in] error     The error code
 * @param[in] state     The state when error was occurred
 * @param[in] user_data The user data passed from the callback registration function
 * @see mediaeditor_set_error_cb()
 * @see mediaeditor_unset_error_cb()
 */
typedef void (*mediaeditor_error_cb)(mediaeditor_error_e error, mediaeditor_state_e state, void *user_data);

/**
 * @brief Called when the state of media editor is changed.
 * @since_tizen 7.0
 * @param[in] previous  The previous state
 * @param[in] current   The current state
 * @param[in] user_data The user data passed from the callback registration function
 * @see mediaeditor_set_state_changed_cb()
 * @see mediaeditor_unset_state_changed_cb()
 */
typedef void (*mediaeditor_state_changed_cb)(mediaeditor_state_e previous, mediaeditor_state_e current, void *user_data);

/**
 * @brief Called when rendering output is completed.
 * @since_tizen 7.0
 * @param[in] user_data The user data passed from the callback registration function
 * @see mediaeditor_start_render()
 */
typedef void (*mediaeditor_render_completed_cb)(void *user_data);

/**
 * @brief Creates a new media editor handle.
 * @since_tizen 7.0
 * @remarks The @a editor must be released using mediaeditor_destroy().\n
 *          The timeline which is composed of a set of layers and clips will be created.
 * @param[out] editor A newly returned handle to the media editor
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_RESOURCE_FAILED   Resource manager initialization error
 * @post @a editor state will be #MEDIAEDITOR_STATE_IDLE.
 *
 * @see mediaeditor_destroy()
 */
int mediaeditor_create(mediaeditor_h *editor);

/**
 * @brief Destroys a media editor handle and releases all its resources.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_RESOURCE_FAILED   Resource manager deinitialization error
 *
 * @see mediaeditor_create()
 */
int mediaeditor_destroy(mediaeditor_h editor);

/**
 * @brief Sets a display for preview.
 * @since_tizen 7.0
 * @param[in] editor  The media editor handle
 * @param[in] type    The display type
 * @param[in] display The display handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_NOT_SUPPORTED     The feature is not supported
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 *
 * @see mediaeditor_start_preview()
 */
int mediaeditor_set_display(mediaeditor_h editor, mediaeditor_display_type_e type, mediaeditor_display_h display);

/**
 * @brief Gets the current state.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @param[out] state The media editor state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #mediaeditor_state_e
 */
int mediaeditor_get_state(mediaeditor_h editor, mediaeditor_state_e *state);

/**
 * @brief Starts to render with given path, asynchronously.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediastorage
 * @privilege %http://tizen.org/privilege/externalstorage
 * @remarks If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage.\n
 *          Or if you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage.\n
 *          If you want to access both storage, you must add both privileges.
 * @param[in] editor    The media editor handle
 * @param[in] path      The path to save rendered output
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre User must add clip by calling mediaeditor_add_clip().
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post @a editor state will be #MEDIAEDITOR_STATE_RENDERING.
 * @post mediaeditor_render_completed_cb() will be invoked.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_cancel_render()
 */
int mediaeditor_start_render(mediaeditor_h editor, const char *path, mediaeditor_render_completed_cb callback, void *user_data);

/**
 * @brief Cancels rendering.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_RENDERING.
 * @post @a editor state will be #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_start_render()
 */
int mediaeditor_cancel_render(mediaeditor_h editor);

/**
 * @brief Starts to preview.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_NOT_SUPPORTED     The feature is not supported
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre The display handle must be set by calling mediaeditor_set_display()
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post @a editor state will be #MEDIAEDITOR_STATE_PREVIEW.
 * @see mediaeditor_set_display()
 */
int mediaeditor_start_preview(mediaeditor_h editor);

/**
 * @brief Stops to preview.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_NOT_SUPPORTED     The feature is not supported
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_PREVIEW.
 * @post @a editor state will be #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_start_preview()
 */
int mediaeditor_stop_preview(mediaeditor_h editor);

/**
 * @brief Sets a callback function to be invoked when an asynchronous operation error occurs.
 * @since_tizen 7.0
 * @param[in] editor    The media editor handle
 * @param[in] callback  Callback function pointer
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE    Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post mediaeditor_error_cb() will be invoked.
 * @see mediaeditor_unset_error_cb()
 * @see mediaeditor_error_cb()
 */
int mediaeditor_set_error_cb(mediaeditor_h editor, mediaeditor_error_cb callback, void *user_data);

/**
 * @brief Unsets an error callback function.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE    Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_set_error_cb()
 */
int mediaeditor_unset_error_cb(mediaeditor_h editor);

/**
 * @brief Sets a callback function to be invoked when the media editor state is changed.
 * @since_tizen 7.0
 * @param[in] editor    The media editor handle
 * @param[in] callback  Callback function pointer
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE    Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post mediaeditor_state_changed_cb() will be invoked.
 * @see mediaeditor_unset_state_changed_cb()
 * @see mediaeditor_state_changed_cb()
 */
int mediaeditor_set_state_changed_cb(mediaeditor_h editor, mediaeditor_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets a state changed callback function.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE    Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_set_state_changed_cb()
 */
int mediaeditor_unset_state_changed_cb(mediaeditor_h editor);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_EDITOR_LAYER_MODULE
 * @{
 */

/**
 * @brief The structure type of the layer information.
 * @since_tizen 7.0
 */
typedef struct {
  unsigned int id;       /**< The layer ID */
  unsigned int priority; /**< The layer priority */
} mediaeditor_layer_info_s;

/**
 * @brief Called when the priority of layers is changed.
 * @since_tizen 7.0
 * @remarks @a layer_info should be released using free().
 * @param[in] layer_infos The layer information including layer id and its priority. It's array.
 * @param[in] size        The number of layer_infos array
 * @param[in] user_data   The user data passed from the callback registration function
 * @see mediaeditor_set_layer_priority_changed_cb()
 * @see mediaeditor_unset_layer_priority_changed_cb()
 */
typedef void (*mediaeditor_layer_priority_changed_cb)(mediaeditor_layer_info_s *layer_infos, unsigned int size, void *user_data);

/**
 * @brief Adds a layer to timeline.
 * @since_tizen 7.0
 * @remarks Layers are responsible for ordering of contained clips. The order is determined by layers' priorities.\n
 *          The layers are stacked in a hierarchical structure.\n
 *          e.g. If we have 3 layers, it will have the following hierarchy.\n
 * <pre>
 *          Top    : layer ID 1, layer priority 0 (The highest priority)
 *                   layer ID 2, layer priority 1
 *          Bottom : layer ID 3, layer priority 2 (The lowest priority)
 * </pre>
 *          Priorities are always a continuous sequence, with no numbers missing in-between.\n
 *          For example, priorities 0, 1, 3, 4 are not possible.\n
 *          But, layer Id could be a discontinuous sequence. Please refer to examples of mediaeditor_remove_layer().\n
 *          The @a layer_priority of newly added layer will be lowest priority.
 * @param[in] editor          The media editor handle
 * @param[out] layer_id       The layer ID. It'll be used when you want to control this layer.
 * @param[out] layer_priority The layer priority represents the hierarchical ordering of contained clips.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_remove_layer()
 * @see mediaeditor_move_layer()
 * @see mediaeditor_activate_layer()
 * @see mediaeditor_deactivate_layer()
 */
int mediaeditor_add_layer(mediaeditor_h editor, unsigned int *layer_id, unsigned int *layer_priority);

/**
 * @brief Removes a layer from timeline.
 * @since_tizen 7.0
 * @remarks The other layer's layer ID is not changed after removing @a layer_id.\n
 *          If @a layer_id is not bottom layer, layer priority will be rearranged.\n
 *          e.g. There're 3 layers like the followings.\n
 * \n
 *          case 1 : remove layer ID 3.\n
 *                   The layer ID 3 is removed and other layers' priorities are not rearranged.\n
 * <pre>
 *                   | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          Before   |          layer ID 2, layer priority 1 (including 3 clips)
 *          removing | bottom : layer ID 3, layer priority 2 (including 5 clips)
 *          --------------------------------------------------------------------
 *          After    | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          removing | bottom : layer ID 2, layer priority 1 (including 3 clips)
 * </pre>
 * \n
 *          case 2 : remove layer ID 1.\n
 * <pre>
 *                   | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          Before   |          layer ID 2, layer priority 1 (including 3 clips)
 *          removing | bottom : layer ID 3, layer priority 2 (including 5 clips)
 *          --------------------------------------------------------------------
 *          After    | top    : layer ID 2, layer priority 0 (including 3 clips)
 *          removing | bottom : layer ID 3, layer priority 1 (including 5 clips)
 * </pre>
 * \n
 * *        case 3 : remove layer ID 2 and add a new layer.\n
 * <pre>
 *                   | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          Before   |          layer ID 2, layer priority 1 (including 3 clips)
 *          removing | bottom : layer ID 3, layer priority 2 (including 5 clips)
 *          --------------------------------------------------------------------
 *          After    | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          removing | bottom : layer ID 3, layer priority 1 (including 5 clips)
 *          --------------------------------------------------------------------
 *          After    | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          adding   |        : layer ID 3, layer priority 1 (including 5 clips)
 *          a layer  | bottom : layer ID 4, layer priority 2 (including 7 clips)
 * </pre>
 * @param[in] editor   The media editor handle
 * @param[in] layer_id The layer ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post mediaeditor_layer_priority_changed_cb() will be invoked if it's set.
 * @see mediaeditor_add_layer()
 * @see mediaeditor_set_layer_priority_changed_cb()
 * @see mediaeditor_unset_layer_priority_changed_cb()
 */
int mediaeditor_remove_layer(mediaeditor_h editor, unsigned int layer_id);

/**
 * @brief Moves a @a layer_id layer to @a layer_priority position.
 * @since_tizen 7.0
 * @remarks e.g. There're 3 layers.\n
 *          case 1 : move layer ID 1 to layer priority 0.\n
 *                   Nothing happens.\n
 * <pre>
 *                 | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          Before |          layer ID 2, layer priority 1 (including 3 clips)
 *          moving | bottom : layer ID 3, layer priority 2 (including 5 clips)
 *          ------------------------------------------------------------------
 *                 | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          After  |          layer ID 2, layer priority 1 (including 3 clips)
 *          moving | bottom : layer ID 3, layer priority 2 (including 5 clips)
 * </pre>
 * \n
 *          case 2 : move layer ID 3 to layer priority 1.\n
 * <pre>
 *                 | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          Before |          layer ID 2, layer priority 1 (including 3 clips)
 *          moving | bottom : layer ID 3, layer priority 2 (including 5 clips)
 *          ------------------------------------------------------------------
 *                 | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          After  |          layer ID 3, layer priority 1 (including 5 clips)
 *          moving | bottom : layer ID 2, layer priority 2 (including 3 clips)
 * </pre>
 * \n
 *          case 3 : move layer ID 1 to layer priority 3.\n
 *                   (Currently, there's no priority 3 layer.)\n
 * <pre>
 *                 | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          Before |          layer ID 2, layer priority 1 (including 3 clips)
 *          moving | bottom : layer ID 3, layer priority 2 (including 5 clips)
 *          ------------------------------------------------------------------
 *                 | top    : layer ID 2, layer priority 0 (including 3 clips)
 *          After  |          layer ID 3, layer priority 1 (including 5 clips)
 *          moving | bottom : layer ID 1, layer priority 2 (including 1 clips)
 * </pre>
 * \n
 *          case 4 : move layer ID 1 to layer priority 5.\n
 *                   (Currently, there's no priority 5 layer and it's not continuous sequence.)\n
 * <pre>
 *                 | top    : layer ID 1, layer priority 0 (including 1 clips)
 *          Before |          layer ID 2, layer priority 1 (including 3 clips)
 *          moving | bottom : layer ID 3, layer priority 2 (including 5 clips)
 *          ------------------------------------------------------------------
 *                 | top    : layer ID 2, layer priority 0 (including 3 clips)
 *          After  |          layer ID 3, layer priority 1 (including 5 clips)
 *          moving | bottom : layer ID 1, layer priority 2 (including 1 clips)
 * </pre>
 * \n
 *          If you can move layer to the lowest priority position, you can get the lowest priority using mediaeditor_get_layer_lowest_priority()
 * @param[in] editor         The media editor handle
 * @param[in] layer_id       The layer ID
 * @param[in] layer_priority The target layer priority that @a layer_id is moved
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post mediaeditor_layer_priority_changed_cb() will be invoked if it's set.
 * @see mediaeditor_add_layer()
 * @see mediaeditor_get_layer_lowest_priority()
 * @see mediaeditor_set_layer_priority_changed_cb()
 * @see mediaeditor_unset_layer_priority_changed_cb()
 */
int mediaeditor_move_layer(mediaeditor_h editor, unsigned int layer_id, unsigned int layer_priority);

/**
 * @brief Activates given layer on timeline. The layer will be included when it's rendered.
 * @since_tizen 7.0
 * @remarks Note that the newly created layer will be activated by default.
 * @param[in] editor   The media editor handle
 * @param[in] layer_id The layer ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_deactivate_layer()
 */
int mediaeditor_activate_layer(mediaeditor_h editor, unsigned int layer_id);

/**
 * @brief Deactivates given layer on timeline.
 * @since_tizen 7.0
 * @remarks The layer is not removed actually but just excluded when it's rendered.
 * @param[in] editor   The media editor handle
 * @param[in] layer_id The layer ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_activate_layer()
 */
int mediaeditor_deactivate_layer(mediaeditor_h editor, unsigned int layer_id);

/**
 * @brief Gets the priority of @a layer_id layer.
 * @since_tizen 7.0
 * @param[in] editor          The media editor handle
 * @param[in] layer_id        The layer ID
 * @param[out] layer_priority The layer priority
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @see mediaeditor_add_layer()
 * @see mediaeditor_get_layer_lowest_priority()
 * @see mediaeditor_get_layer_id()
 */
int mediaeditor_get_layer_priority(mediaeditor_h editor, unsigned int layer_id, unsigned int *layer_priority);

/**
 * @brief Gets the lowest priority of all layers.
 * @since_tizen 7.0
 * @param[in] editor          The media editor handle
 * @param[out] layer_priority The lowest layer priority
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @see mediaeditor_add_layer()
 * @see mediaeditor_get_layer_priority()
 * @see mediaeditor_get_layer_id()
 */
int mediaeditor_get_layer_lowest_priority(mediaeditor_h editor, unsigned int *layer_priority);

/**
 * @brief Gets the layer ID of @a layer_priority layer.
 * @since_tizen 7.0
 * @param[in] editor         The media editor handle
 * @param[in] layer_priority The layer priority
 * @param[out] layer_id      The layer ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @see mediaeditor_add_layer()
 * @see mediaeditor_get_layer_priority()
 * @see mediaeditor_get_layer_lowest_priority()
 */
int mediaeditor_get_layer_id(mediaeditor_h editor, unsigned int layer_priority, unsigned int *layer_id);

/**
 * @brief Sets a callback function to be invoked when a layer priority is changed.
 * @since_tizen 7.0
 * @param[in] editor    The media editor handle
 * @param[in] callback  Callback function pointer
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE    Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post mediaeditor_layer_priority_changed_cb() will be invoked.
 * @see mediaeditor_unset_layer_priority_changed_cb()
 * @see mediaeditor_layer_priority_changed_cb()
 */
int mediaeditor_set_layer_priority_changed_cb(mediaeditor_h editor, mediaeditor_layer_priority_changed_cb callback, void *user_data);

/**
 * @brief Unsets a layer priority changed callback function.
 * @since_tizen 7.0
 * @param[in] editor The media editor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE    Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_set_layer_priority_changed_cb()
 */
int mediaeditor_unset_layer_priority_changed_cb(mediaeditor_h editor);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_EDITOR_CLIP_MODULE
 * @{
 */

/**
 * @brief Adds a clip to timeline.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediastorage
 * @privilege %http://tizen.org/privilege/externalstorage
 * @remarks If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage.\n
 *          Or if you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage.\n
 *          If you want to access both storage, you must add both privileges.
 * @param[in] editor   The media editor handle
 * @param[in] path     The content location to add
 * @param[in] layer_id The layer ID to add clip
 * @param[in] start    The start time of @a path clip which is placed in timeline (in milliseconds)\n
 *                     If this is less than 0, clip will be added to the end of layer. i.e. it will be set to layer's duration
 * @param[in] duration The duration that the clip is in effect for in the timeline (in milliseconds)\n
 *                     It should be lesser than or equal to clip's duration. If not, error will be occurred in runtime.\n
 *                     The clip should have enough internal content greater than @a duration.
 * @param[in] in_point The initial offset of @a path clip to use internally when outputting content (in milliseconds)
 * @param[out] clip_id The ID of added clip. It'll be used when you want to delete this clip.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre The layer must be added by calling mediaeditor_add_layer().
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_layer()
 * @see mediaeditor_remove_clip()
 */
int mediaeditor_add_clip(mediaeditor_h editor, const char *path, unsigned int layer_id,
  int start, unsigned int duration, unsigned int in_point, unsigned int *clip_id);

/**
 * @brief Removes a clip from timeline.
 * @since_tizen 7.0
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID that will be removed.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 */
int mediaeditor_remove_clip(mediaeditor_h editor, unsigned int clip_id);

/**
 * @brief Splits a clip.
 * @since_tizen 7.0
 * @remarks After splitting the clip into two clips, the source clip's duration will be changed.
 * @param[in] editor       The media editor handle
 * @param[in] src_clip_id  The current clip ID that will be split.
 * @param[in] position     The time position at which the source clip will be split. (in milliseconds)
 * @param[out] new_clip_id The newly created clip ID.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 */
int mediaeditor_split_clip(mediaeditor_h editor, unsigned int src_clip_id, unsigned int position,
  unsigned int *new_clip_id);

/**
 * @brief Groups clips.
 * @since_tizen 7.0
 * @param[in] editor    The media editor handle
 * @param[in] clip_ids  The clip IDs to be grouped. Array.
 * @param[in] size      The number of clips
 * @param[out] group_id The grouped clip ID.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_ungroup_clip()
 */
int mediaeditor_group_clip(mediaeditor_h editor, unsigned int *clip_ids, unsigned int size, unsigned int *group_id);

/**
 * @brief Ungroups a clip.
 * @since_tizen 7.0
 * @remarks Layer priorities of ungrouped clips are the same as before grouping.\n
 *          If there's no matched @a group_id, #MEDIAEDITOR_ERROR_INVALID_PARAMETER will be returned.\n
 *          @a clip_ids should be released using free().
 * @param[in] editor    The media editor handle
 * @param[in] group_id  The grouped clip ID
 * @param[out] clip_ids The ungrouped clip IDs. Array
 * @param[out] size     The size of ungrouped clips
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_group_clip()
 */
int mediaeditor_ungroup_clip(mediaeditor_h editor, unsigned int group_id, unsigned int **clip_ids, unsigned int *size);

/**
 * @brief Moves a clip to different layer.
 * @since_tizen 7.0
 * @param[in] editor         The media editor handle
 * @param[in] clip_id        The clip ID that will be moved
 * @param[in] layer_priority The destination layer priority
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 */
int mediaeditor_move_clip_layer(mediaeditor_h editor, unsigned int clip_id, unsigned int layer_priority);

/**
 * @brief Gets the start position of clip.
 * @since_tizen 7.0
 * @remarks @a start will return value set by mediaeditor_add_clip() or mediaeditor_set_clip_start().
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID
 * @param[out] start  The current start position of clip (in milliseconds)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @see mediaeditor_add_clip()
 * @see mediaeditor_set_clip_start()
 * @see mediaeditor_get_clip_duration()
 * @see mediaeditor_set_clip_duration()
 * @see mediaeditor_get_clip_in_point()
 * @see mediaeditor_set_clip_in_point()
 */
int mediaeditor_get_clip_start(mediaeditor_h editor, unsigned int clip_id, unsigned int *start);

/**
 * @brief Sets the start position of clip.
 * @since_tizen 7.0
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID that will be changed
 * @param[in] start   The new start position (in milliseconds)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_start()
 * @see mediaeditor_get_clip_duration()
 * @see mediaeditor_set_clip_duration()
 * @see mediaeditor_get_clip_in_point()
 * @see mediaeditor_set_clip_in_point()
 */
int mediaeditor_set_clip_start(mediaeditor_h editor, unsigned int clip_id, unsigned int start);

/**
 * @brief Gets the duration of clip.
 * @since_tizen 7.0
 * @remarks @a duration will return value set by mediaeditor_add_clip() or mediaeditor_set_clip_duration().
 * @param[in] editor    The media editor handle
 * @param[in] clip_id   The clip ID
 * @param[out] duration The current time duration of clip (in milliseconds)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_start()
 * @see mediaeditor_set_clip_start()
 * @see mediaeditor_set_clip_duration()
 * @see mediaeditor_get_clip_in_point()
 * @see mediaeditor_set_clip_in_point()
 */
int mediaeditor_get_clip_duration(mediaeditor_h editor, unsigned int clip_id, unsigned int *duration);

/**
 * @brief Sets the duration of clip.
 * @since_tizen 7.0
 * @param[in] editor   The media editor handle
 * @param[in] clip_id  The clip ID that will be changed
 * @param[in] duration The new time duration (in milliseconds)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_start()
 * @see mediaeditor_set_clip_start()
 * @see mediaeditor_get_clip_duration()
 * @see mediaeditor_get_clip_in_point()
 * @see mediaeditor_set_clip_in_point()
 */
int mediaeditor_set_clip_duration(mediaeditor_h editor, unsigned int clip_id, unsigned int duration);

/**
 * @brief Gets the offset of clip.
 * @since_tizen 7.0
 * @remarks @a in_point will return value set by mediaeditor_add_clip() or mediaeditor_set_clip_in_point().
 * @param[in] editor    The media editor handle
 * @param[in] clip_id   The clip ID that will be changed
 * @param[out] in_point The offset of clip to use internally when outputting content (in milliseconds)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_start()
 * @see mediaeditor_set_clip_start()
 * @see mediaeditor_get_clip_duration()
 * @see mediaeditor_set_clip_duration()
 * @see mediaeditor_set_clip_in_point()
 */
int mediaeditor_get_clip_in_point(mediaeditor_h editor, unsigned int clip_id, unsigned int *in_point);

/**
 * @brief Sets the offset of clip.
 * @since_tizen 7.0
 * @param[in] editor    The media editor handle
 * @param[in] clip_id   The clip ID
 * @param[in] in_point The offset of clip to use internally when outputting content (in milliseconds)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_start()
 * @see mediaeditor_set_clip_start()
 * @see mediaeditor_get_clip_duration()
 * @see mediaeditor_set_clip_duration()
 * @see mediaeditor_get_clip_in_point()
 */
int mediaeditor_set_clip_in_point(mediaeditor_h editor, unsigned int clip_id, unsigned int in_point);

/**
 * @brief Gets the resolution of clip.
 * @since_tizen 7.0
 * @remarks If the clip doesn't have video, #MEDIAEDITOR_ERROR_INVALID_OPERATION will be returned.\n
 *          @a width and @a height will return its original size if it's not changed by mediaeditor_set_clip_resolution().
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID
 * @param[out] width  The width
 * @param[out] height The height
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @see mediaeditor_add_clip()
 * @see mediaeditor_set_clip_resolution()
 */
int mediaeditor_get_clip_resolution(mediaeditor_h editor, unsigned int clip_id, unsigned int *width, unsigned int *height);

/**
 * @brief Sets the resolution of clip.
 * @since_tizen 7.0
 * @remarks If the clip doesn't have video, #MEDIAEDITOR_ERROR_INVALID_OPERATION will be returned.
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID
 * @param[in] width   The width
 * @param[in] height  The height
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_resolution()
 */
int mediaeditor_set_clip_resolution(mediaeditor_h editor, unsigned int clip_id, unsigned int width, unsigned int height);

/**
 * @brief Gets the position of clip.
 * @since_tizen 7.0
 * @remarks If the clip doesn't have video, #MEDIAEDITOR_ERROR_INVALID_OPERATION will be returned.\n
 *          The default position is the top-left corner(0, 0).
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID
 * @param[out] pos_x  The horizontal position
 * @param[out] pos_y  The vertical position
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @see mediaeditor_add_clip()
 * @see mediaeditor_set_clip_position()
 */
int mediaeditor_get_clip_position(mediaeditor_h editor, unsigned int clip_id, unsigned int *pos_x, unsigned int *pos_y);

/**
 * @brief Sets the position of clip.
 * @since_tizen 7.0
 * @remarks If the clip doesn't have video, #MEDIAEDITOR_ERROR_INVALID_OPERATION will be returned.
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID
 * @param[in] pos_x   The horizontal position
 * @param[in] pos_y   The vertical position
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_position()
 */
int mediaeditor_set_clip_position(mediaeditor_h editor, unsigned int clip_id, unsigned int pos_x, unsigned int pos_y);

/**
 * @brief Gets the volume of clip.
 * @since_tizen 7.0
 * @remarks If the clip doesn't have audio, #MEDIAEDITOR_ERROR_INVALID_OPERATION will be returned.
 * @param[in] editor   The media editor handle
 * @param[in] clip_id  The clip ID
 * @param[out] volume  The current audio volume (0.0 ~ 10.0)
 *                     The default value is 1.0 (100%)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @see mediaeditor_add_clip()
 * @see mediaeditor_set_clip_volume()
 */
int mediaeditor_get_clip_volume(mediaeditor_h editor, unsigned int clip_id, double *volume);

/**
 * @brief Sets the volume of clip.
 * @since_tizen 7.0
 * @remarks If the clip doesn't have audio, #MEDIAEDITOR_ERROR_INVALID_OPERATION will be returned.
 * @param[in] editor  The media editor handle
 * @param[in] clip_id The clip ID
 * @param[in] volume  The new audio volume (0.0 ~ 10.0)\n
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_clip()
 * @see mediaeditor_get_clip_volume()
 */
int mediaeditor_set_clip_volume(mediaeditor_h editor, unsigned int clip_id, double volume);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_EDITOR_EFFECT_MODULE
 * @{
 */

/**
 * @brief Adds transition effect to the overlapped clip section.
 * @since_tizen 7.0
 * @remarks Note that clips should be already overlapped. If not, error will be occurred.\n
 *          Only one transition effect can be applied in the overlapped section.\n
 *          For each overlapped section, this function should be called to apply transition effect.
 * @param[in] editor   The media editor handle
 * @param[in] type     The transition type
 * @param[in] layer_id The layer ID
 * @param[in] start    The start position of overlapped clip area to be applied transition effect
 * @param[in] duration The duration of overlapped clip area to be applied transition effect
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_layer()
 * @see mediaeditor_add_clip()
 */
int mediaeditor_add_transition(mediaeditor_h editor, mediaeditor_transition_type_e type, unsigned int layer_id,
  unsigned int start, unsigned int duration);

/**
 * @brief Adds a new effect.
 * @since_tizen 7.0
 * @remarks #MEDIAEDITOR_EFFECT_AUDIO_TYPE_FADE_IN and #MEDIAEDITOR_EFFECT_AUDIO_TYPE_FADE_OUT can not be rolled back using
 *          mediaeditor_remove_effect().
 * @param[in] editor     The media editor handle
 * @param[in] type       The effect type
 * @param[in] layer_id   The layer priority
 * @param[in] start      The start time of effect which is placed in timeline (in milliseconds)
 * @param[in] duration   The duration of effect (in milliseconds)
 * @param[out] effect_id The effect ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_remove_effect()
 */
int mediaeditor_add_effect(mediaeditor_h editor, mediaeditor_effect_type_e type, unsigned int layer_id,
    unsigned int start, unsigned int duration, unsigned int *effect_id);

/**
 * @brief Removes an effect from timeline.
 * @since_tizen 7.0
 * @param[in] editor    The media editor handle
 * @param[in] effect_id The effect ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_add_effect()
 */
int mediaeditor_remove_effect(mediaeditor_h editor, unsigned int effect_id);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_EDITOR_PROJECT_MODULE
 * @{
 */

/**
 * @brief Called when a timeline of the project is loaded.
 * @since_tizen 7.0
 * @param[in] user_data The user data passed from the callback registration function
 * @see mediaeditor_load_project()
 */
typedef void (*mediaeditor_project_loaded_cb)(void *user_data);

/**
 * @brief Creates a project to @a path.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediastorage
 * @privilege %http://tizen.org/privilege/externalstorage
 * @remarks If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage.\n
 *          Or if you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage.\n
 *          If you want to access both storage, you must add both privileges.
 * @param[in] editor The media editor handle
 * @param[in] path   The path to create project
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_save_project()
 */
int mediaeditor_create_project(mediaeditor_h editor, const char *path);

/**
 * @brief Loads a project from @a path, asynchronously.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mediastorage
 * @privilege %http://tizen.org/privilege/externalstorage
 * @remarks If you want to access only internal storage by using this function, you should add privilege %http://tizen.org/privilege/mediastorage.\n
 *          Or if you want to access only external storage by using this function, you should add privilege %http://tizen.org/privilege/externalstorage.\n
 *          If you want to access both storage, you must add both privileges.
 * @param[in] editor The media editor handle
 * @param[in] path   The path to the saved project
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @post mediaeditor_project_loaded_cb() will be invoked.
 * @see mediaeditor_save_project()
 */
int mediaeditor_load_project(mediaeditor_h editor, const char *path, mediaeditor_project_loaded_cb callback, void *user_data);

/**
 * @brief Saves the current editing information.
 * @since_tizen 7.0
 * @remarks The project will be saved to the project's path, which was set during creation or loading.
 * @param[in] editor The media editor handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE     Invalid state
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @pre The project must be created or loaded.
 * @see mediaeditor_create_project()
 * @see mediaeditor_load_project()
 */
int mediaeditor_save_project(mediaeditor_h editor);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_EDITOR_H__ */
