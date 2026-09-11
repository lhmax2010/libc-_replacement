/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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




#ifndef __TIZEN_MEDIA_TONE_PLAYER_H__
#define __TIZEN_MEDIA_TONE_PLAYER_H__

#include <tizen.h>
#include <sound_manager.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define TONE_PLAYER_ERROR_CLASS          TIZEN_ERROR_MULTIMEDIA_CLASS | 0x60

/**
 * @file tone_player.h
 * @brief This file contains the tone player API
 */

/**
 * @addtogroup CAPI_MEDIA_TONE_PLAYER_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration of error codes for tone player.
 * @since_tizen 2.3
 */
typedef enum {
	TONE_PLAYER_ERROR_NONE               = TIZEN_ERROR_NONE,                    /**< Successful */
	TONE_PLAYER_ERROR_INVALID_PARAMETER  = TIZEN_ERROR_INVALID_PARAMETER,       /**< Invalid parameter */
	TONE_PLAYER_ERROR_INVALID_OPERATION  = TIZEN_ERROR_INVALID_OPERATION,       /**< Invalid operation */
	TONE_PLAYER_ERROR_NOT_SUPPORTED_TYPE = TIZEN_ERROR_TONE_PLAYER | 0x01       /**< Not supported (Since 3.0) */
} tone_player_error_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration of tone.
 * @since_tizen 2.3
 */
typedef enum {
	TONE_TYPE_DEFAULT = 0,			/**< The default tone */
	TONE_TYPE_DTMF_0 = 0,				/**< Predefined DTMF 0 */
	TONE_TYPE_DTMF_1,				/**< Predefined DTMF 1 */
	TONE_TYPE_DTMF_2,				/**< Predefined DTMF 2 */
	TONE_TYPE_DTMF_3,				/**< Predefined DTMF 3 */
	TONE_TYPE_DTMF_4,				/**< Predefined DTMF 4 */
	TONE_TYPE_DTMF_5,				/**< Predefined DTMF 5 */
	TONE_TYPE_DTMF_6,				/**< Predefined DTMF 6 */
	TONE_TYPE_DTMF_7,				/**< Predefined DTMF 7 */
	TONE_TYPE_DTMF_8,				/**< Predefined DTMF 8 */
	TONE_TYPE_DTMF_9,				/**< Predefined DTMF 9 */
	TONE_TYPE_DTMF_S,			   /**< Predefined DTMF Star - Asterisk (*) */
	TONE_TYPE_DTMF_P,				/**< Predefined DTMF sharP (#) */
	TONE_TYPE_DTMF_A,				/**< Predefined DTMF A (A) */
	TONE_TYPE_DTMF_B,				/**< Predefined DTMF B (B) */
	TONE_TYPE_DTMF_C,				/**< Predefined DTMF C (C) */
	TONE_TYPE_DTMF_D,				/**< Predefined DTMF D (D) */
	TONE_TYPE_SUP_DIAL,     /**< Call supervisory tone, Dial tone: CEPT: 425Hz, continuous */
	TONE_TYPE_ANSI_DIAL,    /**< Call supervisory tone, Dial tone: ANSI (IS-95): 350Hz+440Hz, continuous */
	TONE_TYPE_JAPAN_DIAL,    /**< Call supervisory tone, Dial tone: JAPAN: 400Hz, continuous*/
	TONE_TYPE_SUP_BUSY,    /**< Call supervisory tone, Busy: CEPT: 425Hz, 500ms ON, 500ms OFF... */
	TONE_TYPE_ANSI_BUSY,     /**< Call supervisory tone, Busy: ANSI (IS-95): 480Hz+620Hz, 500ms ON, 500ms OFF... */
	TONE_TYPE_JAPAN_BUSY,     /**< Call supervisory tone, Busy: JAPAN: 400Hz, 500ms ON, 500ms OFF...*/
	TONE_TYPE_SUP_CONGESTION,   /**< Call supervisory tone, Congestion: CEPT, JAPAN: 425Hz, 200ms ON, 200ms OFF */
	TONE_TYPE_ANSI_CONGESTION,  /**< Call supervisory tone, Congestion: ANSI (IS-95): 480Hz+620Hz, 250ms ON, 250ms OFF... */
	TONE_TYPE_SUP_RADIO_ACK,   /**< Call supervisory tone, Radio path acknowledgment : CEPT, ANSI: 425Hz, 200ms ON  */
	TONE_TYPE_JAPAN_RADIO_ACK,  /**< Call supervisory tone, Radio path acknowledgment : JAPAN: 400Hz, 1s ON, 2s OFF...*/
	TONE_TYPE_SUP_RADIO_NOTAVAIL,  /**< Call supervisory tone, Radio path not available: 425Hz, 200ms ON, 200 OFF 3 bursts */
	TONE_TYPE_SUP_ERROR,    /**< Call supervisory tone, Error/Special info: 950Hz+1400Hz+1800Hz, 330ms ON, 1s OFF... */
	TONE_TYPE_SUP_CALL_WAITING,  /**< Call supervisory tone, Call Waiting: CEPT, JAPAN: 425Hz, 200ms ON, 600ms OFF, 200ms ON, 3s OFF...  */
	TONE_TYPE_ANSI_CALL_WAITING,  /**< Call supervisory tone, Call Waiting: ANSI (IS-95): 440 Hz, 300 ms ON, 9.7 s OFF, (100 ms ON, 100 ms OFF, 100 ms ON, 9.7s OFF ...) */
	TONE_TYPE_SUP_RINGTONE,   /**< Call supervisory tone, Ring Tone: CEPT, JAPAN: 425Hz, 1s ON, 4s OFF... */
	TONE_TYPE_ANSI_RINGTONE,   /**< Call supervisory tone, Ring Tone: ANSI (IS-95): 440Hz + 480Hz, 2s ON, 4s OFF... */
	TONE_TYPE_PROP_BEEP,    /**< General beep: 400Hz+1200Hz, 35ms ON */
	TONE_TYPE_PROP_ACK,     /**< Proprietary tone, positive acknowledgment: 1200Hz, 100ms ON, 100ms OFF 2 bursts */
	TONE_TYPE_PROP_NACK,     /**< Proprietary tone, negative acknowledgment: 300Hz+400Hz+500Hz, 400ms ON */
	TONE_TYPE_PROP_PROMPT,    /**< Proprietary tone, prompt tone: 400Hz+1200Hz, 200ms ON  */
	TONE_TYPE_PROP_BEEP2,     /**< Proprietary tone, general double beep: twice 400Hz+1200Hz, 35ms ON, 200ms OFF, 35ms ON */
	TONE_TYPE_SUP_INTERCEPT,       /**< Call supervisory tone (IS-95), intercept tone: alternating 440 Hz and 620 Hz tones, each on for 250 ms */
	TONE_TYPE_SUP_INTERCEPT_ABBREV,    /**< Call supervisory tone (IS-95), abbreviated intercept: intercept tone limited to 4 seconds */
	TONE_TYPE_SUP_CONGESTION_ABBREV,     /**< Call supervisory tone (IS-95), abbreviated congestion: congestion tone limited to 4 seconds */
	TONE_TYPE_SUP_CONFIRM,       /**< Call supervisory tone (IS-95), confirm tone: a 350 Hz tone added to a 440 Hz tone repeated 3 times in a 100 ms on, 100 ms off cycle */
	TONE_TYPE_SUP_PIP,        /**< Call supervisory tone (IS-95), pip tone: four bursts of 480 Hz tone (0.1 s on, 0.1 s off). */
	TONE_TYPE_CDMA_DIAL_TONE_LITE,     /**< 425Hz continuous */
	TONE_TYPE_CDMA_NETWORK_USA_RINGBACK,   /**< CDMA USA Ringback: 440Hz+480Hz 2s ON, 4000 OFF ...*/
	TONE_TYPE_CDMA_INTERCEPT,      /**< CDMA Intercept tone: 440Hz 250ms ON, 620Hz 250ms ON ...*/
	TONE_TYPE_CDMA_ABBR_INTERCEPT,     /**< CDMA Abbr Intercept tone: 440Hz 250ms ON, 620Hz 250ms ON */
	TONE_TYPE_CDMA_REORDER,       /**< CDMA Reorder tone: 480Hz+620Hz 250ms ON, 250ms OFF... */
	TONE_TYPE_CDMA_ABBR_REORDER,     /**< CDMA Abbr Reorder tone: 480Hz+620Hz 250ms ON, 250ms OFF repeated for 8 times */
	TONE_TYPE_CDMA_NETWORK_BUSY,     /**< CDMA Network Busy tone: 480Hz+620Hz 500ms ON, 500ms OFF continuous */
	TONE_TYPE_CDMA_CONFIRM,       /**< CDMA Confirm tone: 350Hz+440Hz 100ms ON, 100ms OFF repeated for 3 times */
	TONE_TYPE_CDMA_ANSWER,       /**< CDMA answer tone: silent tone - definition Frequency 0, 0ms ON, 0ms OFF */
	TONE_TYPE_CDMA_NETWORK_CALLWAITING,    /**< CDMA Network Callwaiting tone: 440Hz 300ms ON */
	TONE_TYPE_CDMA_PIP,        /**< CDMA PIP tone: 480Hz 100ms ON, 100ms OFF repeated for 4 times */
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_NORMAL,   /**< ISDN Call Signal Normal tone: {2091Hz 32ms ON, 2556 64ms ON} 20 times, 2091 32ms ON, 2556 48ms ON, 4s OFF */
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_INTERGROUP,  /**< ISDN Call Signal Intergroup tone: {2091Hz 32ms ON, 2556 64ms ON} 8 times, 2091Hz 32ms ON, 400ms OFF, {2091Hz 32ms ON, 2556Hz 64ms ON} 8times, 2091Hz 32ms ON, 4s OFF.*/
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_SP_PRI,   /**< ISDN Call Signal SP PRI tone:{2091Hz 32ms ON, 2556 64ms ON} 4 times 2091Hz 16ms ON, 200ms OFF, {2091Hz 32ms ON, 2556Hz 64ms ON} 4 times, 2091Hz 16ms ON, 200ms OFF */
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_PAT3,   /**< SDN Call sign PAT3 tone: silent tone */
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_PING_RING,  /**< ISDN Ping Ring tone: {2091Hz 32ms ON, 2556Hz 64ms ON} 5 times 2091Hz 20ms ON */
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_PAT5,   /**< ISDN Pat5 tone: silent tone */
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_PAT6,   /**< ISDN Pat6 tone: silent tone */
	TONE_TYPE_CDMA_CALL_SIGNAL_ISDN_PAT7,   /**< ISDN Pat7 tone: silent tone */
	TONE_TYPE_CDMA_HIGH_L,       /**< TONE_CDMA_HIGH_L tone: {3700Hz 25ms, 4000Hz 25ms} 40 times 4000ms OFF, Repeat .... */
	TONE_TYPE_CDMA_MED_L,       /**< TONE_CDMA_MED_L tone: {2600Hz 25ms, 2900Hz 25ms} 40 times 4000ms OFF, Repeat .... */
	TONE_TYPE_CDMA_LOW_L,       /**< TONE_CDMA_LOW_L tone: {1300Hz 25ms, 1450Hz 25ms} 40 times, 4000ms OFF, Repeat .... */
	TONE_TYPE_CDMA_HIGH_SS,       /**< CDMA HIGH SS tone: {3700Hz 25ms, 4000Hz 25ms} repeat 16 times, 400ms OFF, repeat .... */
	TONE_TYPE_CDMA_MED_SS,       /**< CDMA MED SS tone: {2600Hz 25ms, 2900Hz 25ms} repeat 16 times, 400ms OFF, repeat .... */
	TONE_TYPE_CDMA_LOW_SS,       /**< CDMA LOW SS tone: {1300z 25ms, 1450Hz 25ms} repeat 16 times, 400ms OFF, repeat .... */
	TONE_TYPE_CDMA_HIGH_SSL,       /**< CDMA HIGH SSL tone: {3700Hz 25ms, 4000Hz 25ms} 8 times, 200ms OFF, {3700Hz 25ms, 4000Hz 25ms} repeat 8 times, 200ms OFF, {3700Hz 25ms, 4000Hz 25ms} repeat 16 times, 4000ms OFF, repeat ... */
	TONE_TYPE_CDMA_MED_SSL,       /**< CDMA MED SSL tone: {2600Hz 25ms, 2900Hz 25ms} 8 times, 200ms OFF, {2600Hz 25ms, 2900Hz 25ms} repeat 8 times, 200ms OFF, {2600Hz 25ms, 2900Hz 25ms} repeat 16 times, 4000ms OFF, repeat ... */
	TONE_TYPE_CDMA_LOW_SSL,       /**< CDMA LOW SSL tone: {1300Hz 25ms, 1450Hz 25ms} 8 times, 200ms OFF, {1300Hz 25ms, 1450Hz 25ms} repeat 8 times, 200ms OFF, {1300Hz 25ms, 1450Hz 25ms} repeat 16 times, 4000ms OFF, repeat ... */
	TONE_TYPE_CDMA_HIGH_SS_2,      /**< CDMA HIGH SS2 tone: {3700Hz 25ms, 4000Hz 25ms} 20 times, 1000ms OFF, {3700Hz 25ms, 4000Hz 25ms} 20 times, 3000ms OFF, repeat .... */
	TONE_TYPE_CDMA_MED_SS_2,       /**< CDMA MED SS2 tone: {2600Hz 25ms, 2900Hz 25ms} 20 times, 1000ms OFF, {2600Hz 25ms, 2900Hz 25ms} 20 times, 3000ms OFF, repeat .... */
	TONE_TYPE_CDMA_LOW_SS_2,       /**< CDMA LOW SS2 tone: {1300Hz 25ms, 1450Hz 25ms} 20 times, 1000ms OFF, {1300Hz 25ms, 1450Hz 25ms} 20 times, 3000ms OFF, repeat .... */
	TONE_TYPE_CDMA_HIGH_SLS,       /**< CDMA HIGH SLS tone: {3700Hz 25ms, 4000Hz 25ms} 10 times, 500ms OFF, {3700Hz 25ms, 4000Hz 25ms} 20 times, 500ms OFF, {3700Hz 25ms, 4000Hz 25ms} 10 times, 3000ms OFF, REPEAT */
	TONE_TYPE_CDMA_MED_SLS,       /**< CDMA MED SLS tone: {2600Hz 25ms, 2900Hz 25ms} 10 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 20 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 10 times, 3000ms OFF, REPEAT */
	TONE_TYPE_CDMA_LOW_SLS,       /**< CDMA LOW SLS tone: {1300Hz 25ms, 1450Hz 25ms} 10 times, 500ms OFF, {1300Hz 25ms, 1450Hz 25ms} 20 times, 500ms OFF, {1300Hz 25ms, 1450Hz 25ms} 10 times, 3000ms OFF, REPEAT */
	TONE_TYPE_CDMA_HIGH_S_X4,      /**< CDMA HIGH S X4 tone: {3700Hz 25ms, 4000Hz 25ms} 10 times, 500ms OFF, {3700Hz 25ms, 4000Hz 25ms} 10 times, 500ms OFF, {3700Hz 25ms, 4000Hz 25ms} 10 times, 500ms OFF, {3700Hz 25ms, 4000Hz 25ms} 10 times, 2500ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_MED_S_X4,       /**< CDMA MED S X4 tone: {2600Hz 25ms, 2900Hz 25ms} 10 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 10 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 10 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 10 times, 2500ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_LOW_S_X4,       /**< CDMA LOW S X4 tone: {2600Hz 25ms, 2900Hz 25ms} 10 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 10 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 10 times, 500ms OFF, {2600Hz 25ms, 2900Hz 25ms} 10 times, 2500ms OFF, REPEAT....*/
	TONE_TYPE_CDMA_HIGH_PBX_L,      /**< CDMA HIGH PBX L: {3700Hz 25ms, 4000Hz 25ms}20 times, 2000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_MED_PBX_L,      /**< CDMA MED PBX L: {2600Hz 25ms, 2900Hz 25ms}20 times, 2000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_LOW_PBX_L,      /**< CDMA LOW PBX L: {1300Hz 25ms,1450Hz 25ms}20 times, 2000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_HIGH_PBX_SS,      /**< CDMA HIGH PBX SS tone: {3700Hz 25ms, 4000Hz 25ms} 8 times 200 ms OFF, {3700Hz 25ms 4000Hz 25ms}8 times, 2000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_MED_PBX_SS,      /**< CDMA MED PBX SS tone: {2600Hz 25ms, 2900Hz 25ms} 8 times 200 ms OFF, {2600Hz 25ms 2900Hz 25ms}8 times, 2000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_LOW_PBX_SS,      /**< CDMA LOW PBX SS tone: {1300Hz 25ms, 1450Hz 25ms} 8 times 200 ms OFF, {1300Hz 25ms 1450Hz 25ms}8 times, 2000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_HIGH_PBX_SSL,      /**< CDMA HIGH PBX SSL tone:{3700Hz 25ms, 4000Hz 25ms} 8 times 200ms OFF, {3700Hz 25ms, 4000Hz 25ms} 8 times, 200ms OFF, {3700Hz 25ms, 4000Hz 25ms} 16 times, 1000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_MED_PBX_SSL,      /**< CDMA MED PBX SSL tone:{2600Hz 25ms, 2900Hz 25ms} 8 times 200ms OFF, {2600Hz 25ms, 2900Hz 25ms} 8 times, 200ms OFF, {2600Hz 25ms, 2900Hz 25ms} 16 times, 1000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_LOW_PBX_SSL,      /**< CDMA LOW PBX SSL tone:{1300Hz 25ms, 1450Hz 25ms} 8 times 200ms OFF, {1300Hz 25ms, 1450Hz 25ms} 8 times, 200ms OFF, {1300Hz 25ms, 1450Hz 25ms} 16 times, 1000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_HIGH_PBX_SLS,      /**< CDMA HIGH PBX SLS tone:{3700Hz 25ms, 4000Hz 25ms} 8 times 200ms OFF, {3700Hz 25ms, 4000Hz 25ms} 16 times, 200ms OFF, {3700Hz 25ms, 4000Hz 25ms} 8 times, 1000ms OFF, REPEAT....  */
	TONE_TYPE_CDMA_MED_PBX_SLS,      /**< CDMA MED PBX SLS tone:{2600Hz 25ms, 2900Hz 25ms} 8 times 200ms OFF, {2600Hz 25ms, 2900Hz 25ms} 16 times, 200ms OFF, {2600Hz 25ms, 2900Hz 25ms} 8 times, 1000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_LOW_PBX_SLS,      /**< CDMA LOW PBX SLS tone:{1300Hz 25ms, 1450Hz 25ms} 8 times 200ms OFF, {1300Hz 25ms, 1450Hz 25ms} 16 times, 200ms OFF, {1300Hz 25ms, 1450Hz 25ms} 8 times, 1000ms OFF, REPEAT.... */
	TONE_TYPE_CDMA_HIGH_PBX_S_X4,     /**< CDMA HIGH PBX X S4 tone: {3700Hz 25ms 4000Hz 25ms} 8 times, 200ms OFF, {3700Hz 25ms 4000Hz 25ms} 8 times, 200ms OFF, {3700Hz 25ms 4000Hz 25ms} 8 times, 200ms OFF, {3700Hz 25ms 4000Hz 25ms} 8 times, 800ms OFF, REPEAT... */
	TONE_TYPE_CDMA_MED_PBX_S_X4,      /**< CDMA MED PBX X S4 tone: {2600Hz 25ms 2900Hz 25ms} 8 times, 200ms OFF, {2600Hz 25ms 2900Hz 25ms} 8 times, 200ms OFF, {2600Hz 25ms 2900Hz 25ms} 8 times, 200ms OFF, {2600Hz 25ms 2900Hz 25ms} 8 times, 800ms OFF, REPEAT... */
	TONE_TYPE_CDMA_LOW_PBX_S_X4,      /**< CDMA LOW PBX X S4 tone: {1300Hz 25ms 1450Hz 25ms} 8 times, 200ms OFF, {1300Hz 25ms 1450Hz 25ms} 8 times, 200ms OFF, {1300Hz 25ms 1450Hz 25ms} 8 times, 200ms OFF, {1300Hz 25ms 1450Hz 25ms} 8 times, 800ms OFF, REPEAT... */
	TONE_TYPE_CDMA_ALERT_NETWORK_LITE,    /**< CDMA Alert Network Lite tone: 1109Hz 62ms ON, 784Hz 62ms ON, 740Hz 62ms ON 622Hz 62ms ON, 1109Hz 62ms ON */
	TONE_TYPE_CDMA_ALERT_AUTOREDIAL_LITE,   /**< CDMA Alert Auto Redial tone: {1245Hz 62ms ON, 659Hz 62ms ON} 3 times, 1245 62ms ON */
	TONE_TYPE_CDMA_ONE_MIN_BEEP,      /**< CDMA One Min Beep tone: 1150Hz+770Hz 400ms ON */
	TONE_TYPE_CDMA_KEYPAD_VOLUME_KEY_LITE,   /**< CDMA KEYPAD Volume key lite tone: 941Hz+1477Hz 120ms ON */
	TONE_TYPE_CDMA_PRESSHOLDKEY_LITE,    /**< CDMA PRESSHOLDKEY LITE tone: 587Hz 375ms ON, 1175Hz 125ms ON */
	TONE_TYPE_CDMA_ALERT_INCALL_LITE,     /**< CDMA ALERT INCALL LITE tone: 587Hz 62ms, 784 62ms, 831Hz 62ms, 784Hz 62ms, 1109 62ms, 784Hz 62ms, 831Hz 62ms, 784Hz 62ms*/
	TONE_TYPE_CDMA_EMERGENCY_RINGBACK,    /**< CDMA EMERGENCY RINGBACK tone: {941Hz 125ms ON, 10ms OFF} 3times 4990ms OFF, REPEAT... */
	TONE_TYPE_CDMA_ALERT_CALL_GUARD,    /**< CDMA ALERT CALL GUARD tone: {1319Hz 125ms ON, 125ms OFF} 3 times */
	TONE_TYPE_CDMA_SOFT_ERROR_LITE,     /**< CDMA SOFT ERROR LITE tone: 1047Hz 125ms ON, 370Hz 125ms */
	TONE_TYPE_CDMA_CALLDROP_LITE,     /**< CDMA CALLDROP LITE tone: 1480Hz 125ms, 1397Hz 125ms, 784Hz 125ms */
	TONE_TYPE_CDMA_NETWORK_BUSY_ONE_SHOT,   /**< CDMA_NETWORK_BUSY_ONE_SHOT tone: 425Hz 500ms ON, 500ms OFF. */
	TONE_TYPE_CDMA_ABBR_ALERT,      /**< CDMA_ABBR_ALERT tone: 1150Hz+770Hz 400ms ON */
	TONE_TYPE_CDMA_SIGNAL_OFF,    /**< CDMA_SIGNAL_OFF - silent tone */
	TONE_TYPE_USER_DEFINED_LOW_FRE,                    /**< User Defined Tone: 100Hz continuous */
	TONE_TYPE_USER_DEFINED_MED_FRE,                    /**< User Defined Tone: 200Hz continuous */
	TONE_TYPE_USER_DEFINED_HIGH_FRE                   /**< User Defined Tone: 300Hz continuous */
} tone_type_e;


/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_TONE_PLAYER_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 11.0.
 * @brief Plays a tone with stream information of sound-manager.
 *
 * @since_tizen 3.0
 *
 * @remarks Voice Recognition stream type is not supported in this API.
 *
 * @param[in] tone	The tone type to play
 * @param[in] stream_info	The sound stream information handle
 * @param[in] duration_ms	The tone duration in milliseconds \n
 *		                    @c -1 indicates an infinite duration.
 * @param[out] id	The tone player ID ( can be set to @c NULL )
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TONE_PLAYER_ERROR_NONE Successful
 * @retval #TONE_PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TONE_PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #TONE_PLAYER_ERROR_NOT_SUPPORTED_TYPE Not supported stream type
 *
 * @see tone_player_stop()
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int tone_player_start_new(tone_type_e tone, sound_stream_info_h stream_info, int duration_ms, int *id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Stops playing the tone.
 *
 * @since_tizen 2.3
 *
 * @param[in] id	The tone player ID to stop
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #TONE_PLAYER_ERROR_NONE Successful
 * @retval #TONE_PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TONE_PLAYER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see tone_player_start_new()
 */
int tone_player_stop(int id) TIZEN_DEPRECATED_API;




/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_TONE_PLAYER_H__ */
