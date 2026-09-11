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

#ifndef __TIZEN_UI_EFL_UTIL_SCREENSHOT_EXTENSION_H__
#define __TIZEN_UI_EFL_UTIL_SCREENSHOT_EXTENSION_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
# if __GNUC__ >= 4
#  ifndef API
#   define API __attribute__ ((visibility("default")))
#  endif
# endif
#endif

/**
 * @file efl_util_screenshot_extension.h
 */

/**
 * @addtogroup CAPI_EFL_UTIL_MODULE
 * @{
 */

/**
 * @platform
 * @brief Set auto rotation value for screenshot.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenshot  efl_util_screenshot_h handle
 * @param[in]  set  auto rotation value (1:use auto rotation, 0:don't use auto rotation)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenshot
 * @see efl_util_screenshot_get_auto_rotation()
 */
API int efl_util_screenshot_set_auto_rotation(efl_util_screenshot_h screenshot, int set);

/**
 * @platform
 * @brief Get current auto rotation value of screenshot.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenshot  efl_util_screenshot_h handle
 * @param[out]  set  The current auto rotation value of screenshot
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenshot
 * @see efl_util_screenshot_set_auto_rotation()
 */
API int efl_util_screenshot_get_auto_rotation(efl_util_screenshot_h screenshot, int *set);

/**
 * @platform
 * @brief Takes a screenshot of the specified area and returns the tbm surface.
 * @since_tizen 10.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenshot  #efl_util_screenshot_h handle
 * @param[in]  x  The x coordinate of the area to capture (top-left corner)
 * @param[in]  y  The y coordinate of the area to capture (top-left corner)
 * @param[in]  w  The width of the area to capture
 * @param[in]  h  The height of the area to capture
 * @return #tbm_surface_h on success, otherwise @c NULL
 * @retval #tbm_surface_h The TBM surface handle
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenshot
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_SCREENSHOT_EXECUTION_FAIL Execution failure
 * @see efl_util_screenshot_initialize()
 * @see efl_util_screenshot_deinitialize()
 */
API tbm_surface_h efl_util_screenshot_take_area_tbm_surface(efl_util_screenshot_h screenshot, int x, int y, int w, int h);

/**
 * @platform
 * @brief Definition for the screenmirror handle.
 * @since_tizen 7.0
 */
typedef struct _efl_util_screenmirror_h * efl_util_screenmirror_h;

/**
 * @brief The screenmirror handler
 * @details This handler will be called when mirror buffer is received.
 * @since_tizen 7.0
 */
typedef void (*efl_util_screenmirror_handler)(efl_util_screenmirror_h screenmirror,
                                              tbm_surface_h t_surface, void *user_data);

/**
 * @platform
 * @brief Initializes a screenmirror.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @remarks The specific error code can be obtained using the get_last_result()
 * method. Error codes are described in Exception section.
 * @param[in] width width of the screenmirror surface
 * @param[in] height height of the screenmirror surface
 * @return #efl_util_screenmirror_h on success, otherwise @c NULL
 * @retval #efl_util_screenmirror_h The screenmirror handle
 * @exception #EFL_UTIL_ERROR_NONE Successful
 * @exception #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #EFL_UTIL_ERROR_OUT_OF_MEMORY Memory allocation failure
 * @exception #EFL_UTIL_ERROR_SCREENSHOT_INIT_FAIL Initialization failure
 * @exception #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenmirror
 * @see efl_util_screenmirror_deinitialize()
 */
API efl_util_screenmirror_h efl_util_screenmirror_initialize(int width, int height);

/**
 * @platform
 * @brief Deinitializes the screenmirror.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenmirror  #efl_util_screenmirror_h handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenmirror
 * @see efl_util_screenmirror_initialize()
 */
API int efl_util_screenmirror_deinitialize(efl_util_screenmirror_h screenmirror);

/**
 * @platform
 * @brief Set handler to get screenmirror buffer.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenmirror  #efl_util_screenmirror_h handle
 * @param[in]  func  mirror buffer get handler
 * @param[in]  data  user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenmirror
 * @see efl_util_screenmirror_initialize()
 */
API int efl_util_screenmirror_set_handler(efl_util_screenmirror_h screenmirror,
                                          efl_util_screenmirror_handler func, void *data);

/**
 * @platform
 * @brief Start the screenmirror.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenmirror  #efl_util_screenmirror_h handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_SCREENSHOT_EXECUTION_FAIL Execution failure
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenmirror
 * @see efl_util_screenmirror_initialize()
 */
API int efl_util_screenmirror_start(efl_util_screenmirror_h screenmirror);

/**
 * @platform
 * @brief Stop the screenmirror.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/screenshot
 * @param[in]  screenmirror  #efl_util_screenmirror_h handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #EFL_UTIL_ERROR_NONE Successful
 * @retval #EFL_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #EFL_UTIL_ERROR_SCREENSHOT_EXECUTION_FAIL Execution failure
 * @retval #EFL_UTIL_ERROR_PERMISSION_DENIED No permission for screenmirror
 * @see efl_util_screenmirror_initialize()
 * @see efl_util_screenmirror_start();
 */
API int efl_util_screenmirror_stop(efl_util_screenmirror_h screenmirror);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_UI_EFL_UTIL_SCREENSHOT_EXTENSION_H__ */
