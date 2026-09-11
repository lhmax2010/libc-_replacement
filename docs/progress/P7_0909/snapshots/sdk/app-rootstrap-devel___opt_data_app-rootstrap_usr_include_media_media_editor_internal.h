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

#ifndef __TIZEN_MEDIA_EDITOR_INTERNAL_H__
#define __TIZEN_MEDIA_EDITOR_INTERNAL_H__

#include <media_editor.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @internal
 * @brief Sets the ecore wayland video display.
 * @since_tizen 7.0
 * @remarks This function must be called in main thread of the application.
 *          Otherwise, it will return #MEDIAEDITOR_ERROR_INVALID_OPERATION by internal restriction.
 *          To avoid #MEDIAEDITOR_ERROR_INVALID_OPERATION in sub thread, ecore_thread_main_loop_begin() and
 *          ecore_thread_main_loop_end() can be used, but deadlock can occur if the main thread is busy.
 *          So, it's not recommended to use them.
 * @param[in] editor The handle to the mediaeditor
 * @param[in] ecore_wl_window The ecore wayland window handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIAEDITOR_ERROR_NONE Successful
 * @retval #MEDIAEDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIAEDITOR_ERROR_INVALID_STATE Invalid state
 * @retval #MEDIAEDITOR_ERROR_INVALID_OPERATION Invalid operation
 * @pre @a editor state must be set to #MEDIAEDITOR_STATE_IDLE.
 * @see mediaeditor_start_preview()
 * @see ecore_thread_main_loop_begin()
 * @see ecore_thread_main_loop_end()
 */
int mediaeditor_set_ecore_wl_display(mediaeditor_h editor, void *ecore_wl_window);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_EDITOR_INTERNAL_H__ */
