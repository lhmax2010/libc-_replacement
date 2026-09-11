/*
 * Copyright (c) 2018-2019 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_UIFW_MULTI_ASSISTANT_SERVICE_H__
#define __TIZEN_UIFW_MULTI_ASSISTANT_SERVICE_H__

#include <multi_assistant_common.h>
#include <multi_assistant_internal.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Definitions for multi-assistant service related modules */

typedef enum {
	MAS_SPEECH_STREAMING_EVENT_FAIL = -1,		/**< Failed */
	MAS_SPEECH_STREAMING_EVENT_START = 1,		/**< Start event */
	MAS_SPEECH_STREAMING_EVENT_CONTINUE = 2,	/**< Continue event */
	MAS_SPEECH_STREAMING_EVENT_FINISH = 3		/**< Finish event */
} mas_speech_streaming_event_e;

typedef enum {
	MAS_SPEECH_STATUS_NONE = -1, 					/**< None */
	MAS_SPEECH_STATUS_BEGINNING_POINT_DETECTED = 1,	/**< Beginning point of speech is detected */
	MAS_SPEECH_STATUS_END_POINT_DETECTED = 2 		/**< End point of speech is detected */
} mas_speech_status_e;

typedef enum {
	MAS_PLUGIN_EVENT_VOICE_KEY_PRESSED = 0,
	MAS_PLUGIN_EVENT_VOICE_KEY_RELEASED,
	MAS_PLUGIN_EVENT_VOICE_KEY_RELEASED_AFTER_PUSH = MAS_PLUGIN_EVENT_VOICE_KEY_RELEASED,
	MAS_PLUGIN_EVENT_VOICE_KEY_RELEASED_AFTER_TAP,
} mas_plugin_event_e;

typedef enum {
	MAS_RECORDING_SESSION_WAKE_WORD,
	MAS_RECORDING_SESSION_UTTERANCE,
	MAS_RECORDING_SESSION_FOLLOW_UP,
} mas_recording_session_e;

typedef enum {
	MAS_WAKEUP_ENGINE_COMMAND_TARGET_ALL_ASSISTANTS = 0,	/**< All assistants - name not required */
	MAS_WAKEUP_ENGINE_COMMAND_TARGET_SPECIFIC_ASSISTANT,	/**< Specific assistant - name required */
	MAS_WAKEUP_ENGINE_COMMAND_TARGET_DEPENDENCY_MODULE,		/**< Dependency module - name not required */
} mas_wakeup_engine_command_target_e;

typedef struct {
	const char *wakeup_appid;
	const char *wakeup_word;
	const char *wakeup_language;
	const char *wakeup_voice_id;
	const char *wakeup_engine;
	float wakeup_confidence_score;

	long wakeup_start_time;
	long wakeup_end_time;
	bool wakeup_time_valid;

	const void *extra_data;
	int extra_data_length;
	const char *extra_data_description;
} mas_wakeup_event_info;

typedef struct {
	mas_speech_streaming_event_e event;
	void* buffer;
	int len;
} mas_speech_data;

typedef void (*mas_error_cb)(int error, const char* err_msg, void* user_data);

typedef int (*mas_dependency_plugin_proxy_process_event)(mas_plugin_event_e event, void* data, int len);
typedef int (*mas_dependency_plugin_proxy_feed_audio_data)(mas_speech_streaming_event_e event, void* buffer, int len);
typedef int (*mas_dependency_plugin_proxy_send_command)(const char* engine_name, const char* command);
typedef int (*mas_dependency_plugin_proxy_wakeup_assistant)(const mas_wakeup_event_info* info);

typedef struct {
	mas_dependency_plugin_proxy_process_event		process_event;
	mas_dependency_plugin_proxy_feed_audio_data		feed_audio_data;
	mas_dependency_plugin_proxy_send_command		send_command;
	mas_dependency_plugin_proxy_wakeup_assistant	wakeup_assistant;
} mas_dependency_plugin_proxy_interface;

typedef void (*mas_engine_wakeup_event_cb)(mas_wakeup_event_info wakeup_event, void* user_data);
typedef void (*mas_engine_speech_status_cb)(mas_speech_status_e status, void *user_data);
typedef void (*mas_engine_audio_data_require_status_cb)(bool require, void* user_data);
typedef void (*mas_engine_wakeup_engine_command_cb)(mas_wakeup_engine_command_target_e target, const char* name, const char* command, void* user_data);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIFW_MULTI_ASSISTANT_SERVICE_H__ */

