/*
 * Copyright (c) 2011-2018 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __VOICE_CONTROL_MANAGER_INTERNAL_H__
#define __VOICE_CONTROL_MANAGER_INTERNAL_H__

#include <voice_control_command.h>
#include <voice_control_command_expand.h>
#include <voice_control_common.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
	VC_SYSTEM_VOLUME_EVENT_CHANGE_FOR_NEARFIELD,
	VC_SYSTEM_VOLUME_EVENT_CHANGE_FOR_FARFIELD
} vc_system_volume_event_e;

#define VC_SERVICE_STATE_UPDATING 5 /**< 'Updating' state */

/**
 * @brief Sets demandable client list.
 *
 * @param[in] rule demandable client list rule path
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED    Operation failure
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_get_demandable_client_rule()
 */
int vc_mgr_set_demandable_client_rule(const char* rule);

/**
 * @brief Gets demandable client list.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_mgr_set_demandable_client_rule()
 */
int vc_mgr_unset_demandable_client_rule(void);

/**
 * @brief Sets domain such as agent or device type.
 * @since_tizen 5.0
 *
 * @param[in] domain Available agent or device type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE                Successful
 * @retval #VC_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE       Invalid state
 * @retval #VC_ERROR_PERMISSION_DENIED   Permission denied
 * @retval #VC_ERROR_NOT_SUPPORTED       VC not supported
 *
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 */
int vc_mgr_set_domain(const char* domain);

/**
 * @brief Changes system volume.
 * @since_tizen 5.0
 *
 */
int vc_mgr_change_system_volume(vc_system_volume_event_e volume_event);

/**
 * @brief Recover system volume.
 * @since_tizen 5.0
 *
 */
int vc_mgr_recover_system_volume();

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __VOICE_CONTROL_MANAGER_INTERNAL_H__ */

