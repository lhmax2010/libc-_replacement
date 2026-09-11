/**
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


#ifndef __VOICE_CONTROL_INTERNAL_H__
#define __VOICE_CONTROL_INTERNAL_H__

#include <tizen.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Called when client gets the asr recognition result from vc-daemon.
 *
 * @param[in] event The result event
 * @param[in] result ASR text
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @return @c true when asr result is consumed \n @c false to propagate.
 *
 * @pre An application registers callback function using vc_mgr_set_pre_result_cb().
 *
 * @see vc_widget_set_asr_result_cb()
 * @see vc_widget_unset_asr_result_cb()
 */
typedef bool (*vc_asr_result_cb)(vc_result_event_e event, const char* result, void *user_data);

/**
 * @brief Sets command list from file.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 *
 * @remarks The command type is valid for #VC_COMMAND_TYPE_FOREGROUND and #VC_COMMAND_TYPE_BACKGROUND.
 * Therefore, @a type is either #VC_COMMAND_TYPE_FOREGROUND or #VC_COMMAND_TYPE_BACKGROUND.
 * In the file corresponding to @a file_path, there must be commands and command types.
 *
 * @param[in] file_path The command file path (absolute or relative)
 * @param[in] type Command type
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre The state should be #VC_STATE_READY.
 *
 * @see vc_unset_command_list()
 */
int vc_set_command_list_from_file(const char* file_path, int type);

/**
 * @brief Requests to set app id which is to want to ask the server dialogue.
 * @details Using this function, the developer can request registering the application on vc framework.
 *          If developer requests to register @a app_id with @a credential which is valid, the application will be set on vc framework.
 *          and then, when the developer requests the dialogue using vc_request_dialog(), dialog from specific engine server will be played by vc framework.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks If @a app_id is @c NULL, the API tries to get app ID using app manager framework.
 *          However, getting app ID may be failed.
 * @param[in] app_id App id which is to want to ask server dialog.
 * @param[in] credential Credential key.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 * @see vc_unset_server_dialog()
 */
int vc_set_server_dialog(const char* app_id, const char* credential);

/**
 * @brief Requests to unset app id which is to not want to ask the server dialogue.
 * @details Using this function, the developer can disable function to ask dialog based on server.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @remarks If @a app_id is @c NULL, the API tries to get app ID using app manager framework.
 *          However, getting app ID may be failed.
 * @param[in] app_id App id which is to not want to ask server dialog.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @pre The service state should be #VC_SERVICE_STATE_READY.
 * @see vc_set_server_dialog()
 */
int vc_unset_server_dialog(const char* app_id);

/**
 * @brief Connects the voice control service synchronously.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/recorder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #VC_ERROR_NONE Successful
 * @retval #VC_ERROR_INVALID_STATE Invalid state
 * @retval #VC_ERROR_OPERATION_FAILED Operation failure
 * @retval #VC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #VC_ERROR_NOT_SUPPORTED Not supported
 * @pre The state should be #VC_STATE_INITIALIZED.
 * @post If this function is called, the state will be #VC_STATE_READY.
 * @see vc_unprepare()
 */
int vc_prepare_sync(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __VOICE_CONTROL_INTERNAL_H__ */
