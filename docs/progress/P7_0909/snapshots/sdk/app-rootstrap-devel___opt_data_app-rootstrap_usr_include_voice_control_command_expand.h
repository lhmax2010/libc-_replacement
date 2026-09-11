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


#ifndef __VOICE_CONTROL_COMMAND_EXPAND_H__
#define __VOICE_CONTROL_COMMAND_EXPAND_H__

#include <time.h>
#include <tizen.h>
#include <voice_control_command.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Enumerations of command format.
 * @since_tizen 3.0
 */
typedef enum {
	VC_CMD_FORMAT_FIXED = 0,		/**< Fixed command only */
	VC_CMD_FORMAT_FIXED_AND_VFIXED,		/**< Fixed + variable fixed command */
	VC_CMD_FORMAT_VFIXED_AND_FIXED,		/**< Variable fixed + fixed command */
	VC_CMD_FORMAT_FIXED_AND_NONFIXED,	/**< Fixed + non fixed command */
	VC_CMD_FORMAT_NONFIXED_AND_FIXED,	/**< Non fixed + fixed command */
	VC_CMD_FORMAT_ACTION,			/**< Action command */
	VC_CMD_FORMAT_PARTIAL			/**< Partial matched command */
} vc_cmd_format_e;

#define VC_SEARCH_NONE_LEVEL 0
#define VC_SEARCH_TEXT_LEVEL 1
#define VC_SEARCH_WORD_LEVEL 2
#define VC_SEARCH_CHAR_LEVEL 3
#define VC_SEARCH_PRON_LEVEL 4

/**
* @brief Sets command domain
* @since_tizen 2.4
*
* @param[in] vc_command The command handle
* @param[in] domain The domain
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ERROR_NONE Successful
* @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_ERROR_PERMISSION_DENIED Permission denied
* @retval #VC_ERROR_NOT_SUPPORTED Not supported feature
*
* @see vc_cmd_get_domain()
*/
int vc_cmd_set_domain(vc_cmd_h vc_command, int domain);

/**
* @brief Gets command domain.
* @since_tizen 2.4
*
* @param[in] vc_command The command handle
* @param[out] domain The domain
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ERROR_NONE Successful
* @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_ERROR_PERMISSION_DENIED Permission denied
* @retval #VC_ERROR_NOT_SUPPORTED Not supported feature
*
* @see vc_cmd_set_domain()
*/
int vc_cmd_get_domain(vc_cmd_h vc_command, int* domain);

/**
* @brief Remove all commands from command list.
* @since_tizen 2.4
*
* @param[in] vc_cmd_list The command list handle
* @param[in] free_command The command free option @c true = release each commands in list,
*			@c false = remove command from list
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_CMD_ERROR_NONE Successful
* @retval #VC_CMD_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_CMD_ERROR_PERMISSION_DENIED Permission denied
* @retval #VC_CMD_ERROR_NOT_SUPPORTED Not supported feature
*
* @see vc_cmd_list_add()
*/
int vc_cmd_list_remove_all(vc_cmd_list_h vc_cmd_list, bool free_command);

/**
* @brief Gets the commands list which consists of specific type of commands such as foreground or background.
* @since_tizen 3.0
* @privilege %http://tizen.org/privilege/recorder
* @param[in] original The original command list handle
* @param[in] type The command type wants to get
* @param[out] filtered The filtered command list handle
* @return @c 0 on success,
*		  otherwise a negative error value
* @retval #VC_ERROR_NONE Successful
* @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_ERROR_OPERATION_FAILED Operation failure
* @retval #VC_ERROR_PERMISSION_DENIED Permission denied
* @retval #VC_ERROR_NOT_SUPPORTED Not supported
*/
int vc_cmd_list_filter_by_type(vc_cmd_list_h original, int type, vc_cmd_list_h* filtered);

/**
* @brief Sets pid.
* @since_tizen 3.0
*
* @param[in] vc_command The command handle
* @param[in] pid Process id
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_CMD_ERROR_NONE Successful
* @retval #VC_CMD_ERROR_INVALID_PARAMETER Invalid parameter
*
* @see vc_cmd_get_pid()
*/
int vc_cmd_set_pid(vc_cmd_h vc_command, int pid);

/**
* @brief Sets command domain
* @since_tizen 3.0
*
* @param[in] vc_command The command handle
* @param[out] pid Process id
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_CMD_ERROR_NONE Successful
* @retval #VC_CMD_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_CMD_ERROR_PERMISSION_DENIED Permission denied
* @retval #VC_CMD_ERROR_NOT_SUPPORTED Not supported feature
*
* @see vc_cmd_set_pid()
*/
int vc_cmd_get_pid(vc_cmd_h vc_command, int* pid);

/**
* @brief Sets unfixed command.
* @since_tizen 3.0
*
* @param[in] vc_command The command handle
* @param[in] command The unfixed command
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_CMD_ERROR_NONE Successful
* @retval #VC_CMD_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_CMD_ERROR_NOT_SUPPORTED Not supported feature
*
* @see vc_cmd_get_unfixed_command()
*/
int vc_cmd_set_unfixed_command(vc_cmd_h vc_command, const char* command);

/**
* @brief Gets nlu json data.
* @since_tizen 3.0
*
* @param[in] vc_command The command handle
* @param[out] json The nlu json data
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ERROR_NONE Successful
* @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #VC_ERROR_NOT_SUPPORTED Not supported feature
*
*/
int vc_cmd_get_nlu_json(vc_cmd_h vc_cmd, char** json);

/**
* @brief Gets the datetime value from the sentence.
* @since_tizen 3.0
*
* @param[in] text The sentence to analyze
* @param[out] result The datetime value in the sentence
* @param[out] remain Remained text except time
*
* @remark If the function succeeds, @a remain must be released with free() by you when you no longer need it.
*	If there is no time value in @a text or the function does not work correctly,
*	@a result is -1. Otherwise @a result has the time value in @a text.
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_CMD_ERROR_NONE Successful
* @retval #VC_ERROR_OPERATION_FAILED operation failure
* @retval #VC_ERROR_OUT_OF_MEMORY Not enough memory
* @retval #VC_CMD_ERROR_INVALID_PARAMETER Invalid parameter
*
* @see vc_cmd_set_datetime_lang()
*/
int vc_cmd_get_datetime(const char *text, time_t *result, char **remain);

/**
* @brief Gets the commands list of commands similar to the text
* @since_tizen 4.0
*
* @param[in] command The text to find the similar commands
* @param[in] src_list The command list handle of all registered foreground and widget type commands
* @param[out] dst_list The commands list handle to similar commands
* @param[in] search_level The search level selecting depth of algorithm
*
* @remark This function can modify @a src_list by the result, So if you want to keep the content of @a src_list,
*	make a copy of @a src_list before running this function. And also, you need to create the list handle of
*	@a dst_list before running this function, because this function does not create the handle internally.
*
* @return 0 on success, otherwise a negative error value
* @retval #VC_ERROR_NONE Successful
* @retval #VC_ERROR_OPERATION_FAILED operation failure
* @retval #VC_ERROR_OUT_OF_MEMORY Not enough memory
* @retval #VC_ERROR_INVALID_PARAMETER Invalid parameter
*/
int vc_cmd_get_partially_matched_cmd_list(const char *command, vc_cmd_list_h src_list, vc_cmd_list_h dst_list, int search_level);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __VOICE_CONTROL_COMMAND_EXPAND_H__ */
