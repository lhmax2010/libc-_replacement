/*
* Copyright (c) 2011-2015 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef VOICE_CONTROL_ELEMENTARY_PRIVATE_H_
#define VOICE_CONTROL_ELEMENTARY_PRIVATE_H_

#include <stdbool.h>
#include <tizen.h>

/**
* @internal
* @addtogroup VOICE_CONTROL_ELEMENTARY_PRIVATE_MODULE
* @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
* @deprecated Deprecated since 10.0
* @brief Set auto command-register mode and click method for recognized object.
* @since_tizen 3.0
*
* @param[in] mode The mode of auto register
* @param[in] click_method The click method for recognized object
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ELM_ERROR_NONE Successful
* @retval #VC_ELM_ERROR_OPERATION_FAILED Operation failed
* @retval #VC_ELM_ERROR_INVALID_STATE Invalid state
* @retval #VC_ELM_ERROR_NOT_SUPPORTED Not supported
*
* @pre vc_elm_initialize() should be successful.
*
* @see vc_elm_get_auto_register_mode()
*/
int vc_elm_set_auto_register_mode(int mode, int click_method) TIZEN_DEPRECATED_API;

/**
* @deprecated Deprecated since 10.0
* @brief Get auto command-register mode and click method for recognized object.
* @since_tizen 3.0
*
* @param[out] mode The mode of auto register
* @param[out] click_method The click method for recognized object
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ELM_ERROR_NONE Successful
* @retval #VC_ELM_ERROR_OPERATION_FAILED Operation failed
* @retval #VC_ELM_ERROR_INVALID_STATE Invalid state
* @retval #VC_ELM_ERROR_NOT_SUPPORTED Not supported
*
* @pre vc_elm_initialize() should be successful.
*
* @see vc_elm_set_auto_register_mode()
*/
int vc_elm_get_auto_register_mode(int* mode, int* click_method) TIZEN_DEPRECATED_API;

/**
* @deprecated Deprecated since 10.0
* @brief Add allowed text part for auto command register.
* @since_tizen 3.0
*
* @param[in] text_part The text part of elementary widget
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ELM_ERROR_NONE Successful
* @retval #VC_ELM_ERROR_OPERATION_FAILED Operation failed
* @retval #VC_ELM_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_ELM_ERROR_INVALID_STATE Invalid state
* @retval #VC_ELM_ERROR_NOT_SUPPORTED Not supported
*
* @pre vc_elm_initialize() should be successful.
*
* @see vc_elm_remove_allowed_text_part()
*/
int vc_elm_add_allowed_text_part(const char* text_part) TIZEN_DEPRECATED_API;

/**
* @deprecated Deprecated since 10.0
* @brief Remove the list of additional allowed text part for auto command register.
* @since_tizen 3.0
*
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ELM_ERROR_NONE Successful
* @retval #VC_ELM_ERROR_OPERATION_FAILED Operation failed
* @retval #VC_ELM_ERROR_INVALID_STATE Invalid state
* @retval #VC_ELM_ERROR_NOT_SUPPORTED Not supported
*
* @pre vc_elm_initialize() should be successful.
*
* @see vc_elm_add_allowed_text_part()
*/
int vc_elm_remove_allowed_text_part(void) TIZEN_DEPRECATED_API;

int vc_elm_set_geometry_info(int x, int y, int w, int h) TIZEN_DEPRECATED_API;

int vc_elm_unset_geometry_info() TIZEN_DEPRECATED_API;

int vc_elm_set_click_time(float time) TIZEN_DEPRECATED_API;

int vc_elm_unset_click_time() TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Function that check whether voice touch is set as auto mode or not
 * @param[out] is_vt_automode a parameter for checking whether voice touch is set as auto mode or not
 */
int vc_elm_is_supported_vt_auto(int* is_vt_automode) TIZEN_DEPRECATED_API;

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* VOICE_CONTROL_ELEMENTARY_PRIVATE_H_ */
