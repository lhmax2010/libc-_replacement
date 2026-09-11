/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __SCL_CSHARP_NUI_H__
#define __SCL_CSHARP_NUI_H__

#include <tizen.h>
#include <cscl-ui-controller.h>

#define MAX_FONT_NAME_LEN 32
#define DEGREE_TO_SCLROTATION(degree) \
    (degree == 90 ? ROTATION_90_CW : \
    (degree == 180 ? ROTATION_180 : \
    (degree == 270 ? ROTATION_90_CCW : \
    ROTATION_0)))

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _LabelAlignment {
    LABEL_ALIGN_LEFT_TOP = 0,
    LABEL_ALIGN_CENTER_TOP,
    LABEL_ALIGN_RIGHT_TOP,
    LABEL_ALIGN_LEFT_MIDDLE,
    LABEL_ALIGN_CENTER_MIDDLE,
    LABEL_ALIGN_RIGHT_MIDDLE,
    LABEL_ALIGN_LEFT_BOTTOM,
    LABEL_ALIGN_CENTER_BOTTOM,
    LABEL_ALIGN_RIGHT_BOTTOM,
    MAX_LABEL_ALIGNMENT
} LabelAlignment;

typedef enum _WindowType {
    WINDOW_KEYPAD,
    WINDOW_MAGNIFIER,
    WINDOW_DIM,
    WINDOW_POPUP,
    WINDOW_AUTO_POPUP,
}WindowType;

/* graphics backend */
typedef void (*scl_nui_draw_text_cb)(const char *font_name, short font_size, bool is_italic, bool is_bold, int r, int g, int b, int a,
                                     const char *str, int pos_x, int pos_y, int w, int h,
                                     LabelAlignment align, int padding_x, int padding_y, int inner_width, int inner_height, WindowType type, void* user_data);

typedef void (*scl_nui_draw_image_cb)(const char *image_path, int dest_x, int dest_y, int dest_weight, int dest_height, int src_x, int src_y, int src_width, int src_height, WindowType type, void* user_data);
typedef void (*scl_nui_draw_rectangle_cb)(int pos_x, int pos_y, int width, int height, bool fill, int fill_color_r, int fill_color_g, int fill_color_b, int fill_color_a, SCLWindowType type, void* user_data);

/* window backend */
typedef void (*scl_nui_update_window_cb)(SCLWindowType type, int x, int y, int width, int height, void* user_data);
typedef int (*scl_nui_key_click_event_cb)(const char *key_value, unsigned long key_event, int key_type, int key_modifier);
typedef int (*scl_nui_drag_state_changed_cb)(const char *key_value, unsigned long key_event, int key_type, int event_type, int current_x, int current_y);
typedef int (*scl_nui_event_notification_cb)(SCLUINotiType noti_type, SclNotiDesc *etc_info);
typedef void (*scl_nui_update_window_position_cb)(SCLWindowType type, int pos_x, int pos_y, int rot_x, int rot_y, void* user_data);

int scl_nui_init(Ecore_Wl2_Window *window, const char *entry_filepath);
int scl_nui_fini();
int scl_nui_show();
int scl_nui_hide();

int scl_nui_get_input_mode(const char **input_mode);
int scl_nui_get_input_mode_size(const char*input_mode, int display_mode, int *width, int *height);
int scl_nui_set_input_mode(const char *input_mode);
int scl_nui_set_update_pending(bool pend);
int scl_nui_set_rotation(int degree);

int scl_nui_set_draw_text_cb(scl_nui_draw_text_cb callback, void *user_data);
int scl_nui_set_draw_image_cb(scl_nui_draw_image_cb callback, void *user_data);
int scl_nui_set_draw_rectangle_cb(scl_nui_draw_rectangle_cb callback, void *user_data);

int scl_nui_set_cur_sublayout(const char *sub_layout_name);
int scl_nui_set_string_substitution(const char *original, const char *substitute);
int scl_nui_unset_string_substitution(const char *original);

int scl_nui_set_update_window_cb(scl_nui_update_window_cb callback, void *user_data);

int scl_nui_set_key_click_event_cb(scl_nui_key_click_event_cb callback);
int scl_nui_set_drag_state_changed_cb(scl_nui_drag_state_changed_cb callback);
int scl_nui_set_event_notification_cb(scl_nui_event_notification_cb callback);
int scl_nui_set_update_window_position_cb(scl_nui_update_window_position_cb callback, void *user_data);

int scl_nui_enable_magnifier(bool enabled);
int scl_nui_enable_sound(bool enabled);
int scl_nui_enable_vibration(bool enabled);
int scl_nui_set_private_key(const char* custom_id, char* label, char* imagelabel[SCL_BUTTON_STATE_MAX], char* imagebg[SCL_BUTTON_STATE_MAX], unsigned long key_event, char *key_value, bool redraw);
int scl_nui_unset_private_key(const char* custom_id);
int scl_nui_enable_button(const char* custom_id, bool enabled);
int scl_nui_get_shift_state(unsigned int *state);
int scl_nui_set_shift_state(unsigned int state);
int scl_nui_set_autocapital_shift_state(bool state);
int scl_nui_disable_input_event(bool disable);
int scl_nui_set_custom_starting_coordinates(int x, int y);

#ifdef __cplusplus
}
#endif

#endif /* __SCL_CSHARP_NUI_H__ */
