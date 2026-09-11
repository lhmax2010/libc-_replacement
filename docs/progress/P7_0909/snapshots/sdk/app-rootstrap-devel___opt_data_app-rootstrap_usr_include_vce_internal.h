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


#ifndef __VCE_INTERNAL_H__
#define __VCE_INTERNAL_H__

#include <tizen.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A structure for the VC engine functions.
 * @details This structure contains essential callback functions for operating VC engine.
 * @since_tizen 4.0
 * @remarks These functions are optional for operating VC engine.
 */
typedef struct {
	/* Optional callbacks */
	vce_request_tts_cb					request_tts;
	void*								request_tts_user_data;
	vce_cancel_tts_cb					cancel_tts;
	void*								cancel_tts_user_data;
	vce_tts_audio_format_request_cb		get_tts_audio_format;
	void*								get_tts_audio_format_user_data;

	vce_private_data_set_cb				private_data_set;
	vce_private_data_requested_cb		private_data_request;
	vce_nlu_base_info_requested_cb		nlu_base_info_request;
	vce_specific_engine_request_cb		specific_engine_request;
} vce_internal_request_callback_s;

/**
 * @brief Enumeration for TTS feedback events.
 * @since_tizen 5.0
 */
typedef enum {
	VCE_UPDATE_EVENT_FAIL = -1,    /**< Failed */
	VCE_UPDATE_EVENT_START = 1,    /**< Start event */
	VCE_UPDATE_EVENT_FINISH = 2    /**< Finish event */
} vce_update_event_e;

int vce_send_update_status(vce_update_event_e update_event, const char* msg);


#ifdef __cplusplus
}
#endif

/**
* @}
*/

#endif /* __VCE_INTERNAL_H__ */
