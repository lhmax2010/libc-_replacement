/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_WEBRTC_H__
#define __TIZEN_MEDIA_WEBRTC_H__

#include <tizen.h>
#include <media_format.h>
#include <media_packet.h>
#include <sound_manager.h>
#include <bundle.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
* @file webrtc.h
* @brief This file contains the WebRTC API.
*/

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MODULE
* @{
*/

/**
 * @brief WebRTC handle type.
 * @since_tizen 6.5
 */
typedef void *webrtc_h;

/**
 * @brief Enumeration for WebRTC error.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_ERROR_NONE = TIZEN_ERROR_NONE,                                /**< Successful */
	WEBRTC_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,              /**< Not supported */
	WEBRTC_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,      /**< Permission denied */
	WEBRTC_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,      /**< Invalid parameter */
	WEBRTC_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,      /**< Invalid operation */
	WEBRTC_ERROR_INVALID_STATE = TIZEN_ERROR_MEDIA_WEBRTC | 0x01,        /**< Invalid state */
	WEBRTC_ERROR_CONNECTION_FAILED = TIZEN_ERROR_MEDIA_WEBRTC | 0x02,    /**< Connection failed */
	WEBRTC_ERROR_STREAM_FAILED = TIZEN_ERROR_MEDIA_WEBRTC | 0x03,        /**< Stream failed */
	WEBRTC_ERROR_RESOURCE_FAILED = TIZEN_ERROR_MEDIA_WEBRTC | 0x04,      /**< Resource failed */
	WEBRTC_ERROR_RESOURCE_CONFLICT = TIZEN_ERROR_MEDIA_WEBRTC | 0x05,    /**< Resource conflict */
	WEBRTC_ERROR_NETWORK_RESOURCE_FAILED = TIZEN_ERROR_MEDIA_WEBRTC | 0x06,  /**< Network resource failed (Since 8.0) */
} webrtc_error_e;

/**
 * @brief Enumeration for WebRTC state.
 * @details See \ref CAPI_MEDIA_WEBRTC_LIFE_CYCLE_STATE_TRANSITIONS.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_STATE_IDLE,        /**<  Created but not started */
	WEBRTC_STATE_NEGOTIATING, /**<  Started but negotiation stage */
	WEBRTC_STATE_PLAYING,     /**<  Started all streams */
} webrtc_state_e;

/**
 * @brief Enumeration for WebRTC peer connection state.
 * @since_tizen 6.5
 * @remarks It corresponds with the RTCPeerConnectionState enum described in https://www.w3.org/TR/webrtc/#rtcpeerconnectionstate-enum.
 */
typedef enum {
	WEBRTC_PEER_CONNECTION_STATE_NEW,          /**<  New */
	WEBRTC_PEER_CONNECTION_STATE_CONNECTING,   /**<  Connecting */
	WEBRTC_PEER_CONNECTION_STATE_CONNECTED,    /**<  Connected */
	WEBRTC_PEER_CONNECTION_STATE_DISCONNECTED, /**<  Disconnected */
	WEBRTC_PEER_CONNECTION_STATE_FAILED,       /**<  Failed */
	WEBRTC_PEER_CONNECTION_STATE_CLOSED,       /**<  Closed */
} webrtc_peer_connection_state_e;

/**
 * @brief Enumeration for WebRTC signaling state.
 * @since_tizen 6.5
 * @remarks It corresponds with the RTCSignalingState enum described in https://www.w3.org/TR/webrtc/#rtcsignalingstate-enum.
 */
typedef enum {
	WEBRTC_SIGNALING_STATE_STABLE,               /**<  Stable */
	WEBRTC_SIGNALING_STATE_HAVE_LOCAL_OFFER,     /**<  Have local offer */
	WEBRTC_SIGNALING_STATE_HAVE_REMOTE_OFFER,    /**<  Have remote offer */
	WEBRTC_SIGNALING_STATE_HAVE_LOCAL_PRANSWER,  /**<  Have local provisional answer */
	WEBRTC_SIGNALING_STATE_HAVE_REMOTE_PRANSWER, /**<  Have remote provisional answer */
	WEBRTC_SIGNALING_STATE_CLOSED,               /**<  Closed */
} webrtc_signaling_state_e;

/**
 * @brief Enumeration for WebRTC ICE gathering state.
 * @since_tizen 6.5
 * @remarks It corresponds with the RTCIceGatheringState enum described in https://www.w3.org/TR/webrtc/#rtcicegatheringstate-enum.
 */
typedef enum {
	WEBRTC_ICE_GATHERING_STATE_NEW,        /**<  New */
	WEBRTC_ICE_GATHERING_STATE_GATHERING,  /**<  Gathering */
	WEBRTC_ICE_GATHERING_STATE_COMPLETE,   /**<  Complete */
} webrtc_ice_gathering_state_e;

/**
 * @brief Enumeration for WebRTC ICE connection state.
 * @since_tizen 6.5
 * @remarks It corresponds with the RTCIceConnectionState enum described in https://www.w3.org/TR/webrtc/#rtciceconnectionstate-enum.
 */
typedef enum {
	WEBRTC_ICE_CONNECTION_STATE_NEW,           /**<  New */
	WEBRTC_ICE_CONNECTION_STATE_CHECKING,      /**<  Checking */
	WEBRTC_ICE_CONNECTION_STATE_CONNECTED,     /**<  Connected */
	WEBRTC_ICE_CONNECTION_STATE_COMPLETED,     /**<  Completed */
	WEBRTC_ICE_CONNECTION_STATE_FAILED,        /**<  Failed */
	WEBRTC_ICE_CONNECTION_STATE_DISCONNECTED,  /**<  Disconnected */
	WEBRTC_ICE_CONNECTION_STATE_CLOSED,        /**<  Closed */
} webrtc_ice_connection_state_e;

/**
 * @brief Enumeration for WebRTC bundle policy.
 * @since_tizen 7.0
 * @remarks It corresponds with the RTCBundlePolicy enum described in https://www.w3.org/TR/webrtc/#rtcbundlepolicy-enum.
 */
typedef enum {
	WEBRTC_BUNDLE_POLICY_NONE,                 /**<  None */
	WEBRTC_BUNDLE_POLICY_MAX_BUNDLE,           /**<  Max-bundle */
	WEBRTC_BUNDLE_POLICY_MAX_COMPAT,           /**<  Max-compat (Since 9.0) */
} webrtc_bundle_policy_e;

/**
 * @brief Enumeration for WebRTC ICE transport policy.
 * @since_tizen 6.5
 * @remarks It corresponds with the RTCIceTransportPolicy enum described in https://www.w3.org/TR/webrtc/#rtcicetransportpolicy-enum.
 */
typedef enum {
	WEBRTC_ICE_TRANSPORT_POLICY_ALL,           /**<  All */
	WEBRTC_ICE_TRANSPORT_POLICY_RELAY,         /**<  Relay */
} webrtc_ice_transport_policy_e;

/**
 * @brief Enumeration for WebRTC media type.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_MEDIA_TYPE_AUDIO,   /**< Audio */
	WEBRTC_MEDIA_TYPE_VIDEO,   /**< Video */
} webrtc_media_type_e;

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MEDIA_SOURCE_MODULE
* @{
*/

/**
 * @brief Enumeration for WebRTC media source type.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_MEDIA_SOURCE_TYPE_AUDIOTEST,    /**<  Audio test */
	WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST,    /**<  Video test */
	WEBRTC_MEDIA_SOURCE_TYPE_MIC,          /**<  Audio from microphone */
	WEBRTC_MEDIA_SOURCE_TYPE_CAMERA,       /**<  Camera preview */
	WEBRTC_MEDIA_SOURCE_TYPE_SCREEN,       /**<  Screen capture */
	WEBRTC_MEDIA_SOURCE_TYPE_FILE,         /**<  Media file */
	WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET, /**<  Media packet */
	WEBRTC_MEDIA_SOURCE_TYPE_NULL          /**<  Null (Since 7.0) */
} webrtc_media_source_type_e;

/**
 * @brief Enumeration for buffer state type of media packet source.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_MEDIA_PACKET_SOURCE_BUFFER_STATE_UNDERFLOW,     /**< Buffer underflow */
	WEBRTC_MEDIA_PACKET_SOURCE_BUFFER_STATE_OVERFLOW,      /**< Buffer overflow */
} webrtc_media_packet_source_buffer_state_e;

/**
 * @brief Enumeration for WebRTC transceiver direction type.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_TRANSCEIVER_DIRECTION_SENDONLY,    /**< Send only */
	WEBRTC_TRANSCEIVER_DIRECTION_RECVONLY,    /**< Receive only */
	WEBRTC_TRANSCEIVER_DIRECTION_SENDRECV,    /**< Send and receive */
} webrtc_transceiver_direction_e;

/**
 * @brief Enumeration for WebRTC transceiver codec.
 * @since_tizen 7.0
 */
typedef enum {
	WEBRTC_TRANSCEIVER_CODEC_PCMU = 0x00000100 | 0x01,   /**< PCMU audio codec */
	WEBRTC_TRANSCEIVER_CODEC_PCMA = 0x00000100 | 0x02,   /**< PCMA audio codec */
	WEBRTC_TRANSCEIVER_CODEC_OPUS = 0x00000100 | 0x03,   /**< OPUS audio codec */
	WEBRTC_TRANSCEIVER_CODEC_VP8  = 0x00000200 | 0x01,   /**< VP8 video codec */
	WEBRTC_TRANSCEIVER_CODEC_VP9  = 0x00000200 | 0x02,   /**< VP9 video codec */
	WEBRTC_TRANSCEIVER_CODEC_H264 = 0x00000200 | 0x03,   /**< H264 video codec */
} webrtc_transceiver_codec_e;

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MEDIA_RENDER_MODULE
* @{
*/

/**
 * @brief WebRTC display handle type.
 * @since_tizen 6.5
 * @remarks The pointer of this handle can be obtained from EFL UI API.\n
 *          For example, in case of #WEBRTC_DISPLAY_TYPE_OVERLAY, elm_win_add() can be used to get the window handle.\n
 *          In case of #WEBRTC_DISPLAY_TYPE_EVAS, evas_object_image_add() can be used to get the renderable image handle.\n
 *          For more details, please refer to https://docs.tizen.org/application/native/guides/ui/efl/.
 */
typedef void *webrtc_display_h;

/**
 * @brief Enumeration for WebRTC display type.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_DISPLAY_TYPE_OVERLAY,      /**< Overlay */
	WEBRTC_DISPLAY_TYPE_EVAS,         /**< Evas image object */
} webrtc_display_type_e;

/**
 * @brief Enumeration for WebRTC display mode.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_DISPLAY_MODE_LETTER_BOX,   /**< Letter box */
	WEBRTC_DISPLAY_MODE_ORIGIN_SIZE,  /**< Origin size */
	WEBRTC_DISPLAY_MODE_FULL,         /**< Full screen */
} webrtc_display_mode_e;

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_DATA_CHANNEL_MODULE
* @{
*/

/**
 * @brief WebRTC data channel handle type.
 * @since_tizen 6.5
 */
typedef void *webrtc_data_channel_h;

/**
 * @brief WebRTC bytes data handle type.
 * @since_tizen 6.5
 */
typedef void *webrtc_bytes_data_h;

/**
 * @brief Enumeration for WebRTC data channel type.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_DATA_CHANNEL_TYPE_STRING,  /**<  String data */
	WEBRTC_DATA_CHANNEL_TYPE_BYTES,   /**<  Bytes data */
} webrtc_data_channel_type_e;

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_STATISTICS_MODULE
* @{
*/

/**
 * @brief Enumeration for WebRTC statistics type.
 * @since_tizen 7.0
 */
typedef enum {
	WEBRTC_STATS_TYPE_CODEC                = 0x0001,   /**< Codec */
	WEBRTC_STATS_TYPE_INBOUND_RTP          = 0x0002,   /**< Inbound RTP */
	WEBRTC_STATS_TYPE_OUTBOUND_RTP         = 0x0004,   /**< Outbound RTP */
	WEBRTC_STATS_TYPE_REMOTE_INBOUND_RTP   = 0x0008,   /**< Remote inbound RTP */
	WEBRTC_STATS_TYPE_REMOTE_OUTBOUND_RTP  = 0x0010,   /**< Remote outbound RTP */
	WEBRTC_STATS_TYPE_CANDIDATE_PAIR       = 0x0020,   /**< Candidate pair (Since 8.0) */
	WEBRTC_STATS_TYPE_LOCAL_CANDIDATE      = 0x0040,   /**< Local candidate (Since 8.0) */
	WEBRTC_STATS_TYPE_REMOTE_CANDIDATE     = 0x0080,   /**< Remote candidate (Since 8.0) */
} webrtc_stats_type_e;

/**
 * @brief Definition for all the supported statistics types mask.
 * @since_tizen 7.0
 * @see webrtc_foreach_stats()
 */
#define WEBRTC_STATS_TYPE_ALL    0

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_COMMON    0x00000100

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC codec stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtccodecstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_CODEC    0x00000200

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC RTP stream stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcrtpstreamstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_RTP_STREAM    0x00000400

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC received RTP stream stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcreceivedrtpstreamstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_RECEIVED_RTP_STREAM    0x00000800

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC inbound RTP stream stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcinboundrtpstreamstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_INBOUND_RTP_STREAM    0x00001000

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC sent RTP stream stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcsentrtpstreamstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_SENT_RTP_STREAM    0x00002000

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC outbound RTP stream stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcoutboundrtpstreamstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_OUTBOUND_RTP_STREAM    0x00004000

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC remote inbound RTP stream stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcremoteinboundrtpstreamstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_REMOTE_INBOUND_RTP_STREAM    0x00008000

/**
 * @deprecated Deprecated since 8.0.
 * @brief Definition for mask value used by #webrtc_stats_prop_e that represents properties of RTC remote outbound RTP stream stats.
 * @since_tizen 7.0
 * @remarks It corresponds with the values described in https://www.w3.org/TR/webrtc-stats/#dom-rtcremoteoutboundrtpstreamstats.
 * @see webrtc_stats_prop_e
 */
#define WEBRTC_STATS_REMOTE_OUTBOUND_RTP_STREAM    0x00010000

/**
 * @brief Enumeration for WebRTC statistics property.
 * @since_tizen 7.0
 */
typedef enum {
	WEBRTC_STATS_PROP_TIMESTAMP = 1,        /**< Timestamp */
	WEBRTC_STATS_PROP_ID,                   /**< Id */
	WEBRTC_STATS_PROP_PAYLOAD_TYPE,         /**< Payload type */
	WEBRTC_STATS_PROP_CLOCK_RATE,           /**< Clock rate */
	WEBRTC_STATS_PROP_CHANNELS,             /**< Channels */
	WEBRTC_STATS_PROP_MIME_TYPE,            /**< MIME type */
	WEBRTC_STATS_PROP_CODEC_TYPE,           /**< Codec type */
	WEBRTC_STATS_PROP_SDP_FMTP_LINE,        /**< SDP FMTP line */
	WEBRTC_STATS_PROP_SSRC,                 /**< SSRC */
	WEBRTC_STATS_PROP_TRANSPORT_ID,         /**< Transport id */
	WEBRTC_STATS_PROP_CODEC_ID,             /**< Codec id */
	WEBRTC_STATS_PROP_PACKETS_RECEIVED,     /**< Packets received */
	WEBRTC_STATS_PROP_PACKETS_LOST,         /**< Packets lost */
	WEBRTC_STATS_PROP_PACKETS_DISCARDED,    /**< Packets discarded */
	WEBRTC_STATS_PROP_JITTER,               /**< Jitter */
	WEBRTC_STATS_PROP_BYTES_RECEIVED,       /**< Bytes received */
	WEBRTC_STATS_PROP_PACKETS_DUPLICATED,   /**< Packets duplicated */
	WEBRTC_STATS_PROP_BYTES_SENT,           /**< Bytes sent */
	WEBRTC_STATS_PROP_PACKETS_SENT,         /**< Packets sent */
	WEBRTC_STATS_PROP_REMOTE_ID,            /**< Remote id */
	WEBRTC_STATS_PROP_FIR_COUNT,            /**< FIR packets */
	WEBRTC_STATS_PROP_PLI_COUNT,            /**< PLI packets */
	WEBRTC_STATS_PROP_NACK_COUNT,           /**< NACK packets */
	WEBRTC_STATS_PROP_ROUND_TRIP_TIME,      /**< Round trip time */
	WEBRTC_STATS_PROP_FRACTION_LOST,        /**< Fraction lost */
	WEBRTC_STATS_PROP_REMOTE_TIMESTAMP,     /**< Remote timestamp */
	WEBRTC_STATS_PROP_LOCAL_ID,             /**< Local id */
	WEBRTC_STATS_PROP_KIND,                 /**< Kind (Since 8.0) */
	WEBRTC_STATS_PROP_ADDRESS,              /**< Address of the candidate (Since 8.0) */
	WEBRTC_STATS_PROP_PORT,                 /**< Port number of the candidate (Since 8.0) */
	WEBRTC_STATS_PROP_CANDIDATE_TYPE,       /**< Candidate type (Since 8.0) */
	WEBRTC_STATS_PROP_PRIORITY,             /**< Priority (Since 8.0) */
	WEBRTC_STATS_PROP_PROTOCOL,             /**< Protocol, 'udp' or 'tcp' (Since 8.0) */
	WEBRTC_STATS_PROP_RELAY_PROTOCOL,       /**< Relay protocol (Since 8.0) */
	WEBRTC_STATS_PROP_URL,                  /**< URL (Since 8.0) */
	WEBRTC_STATS_PROP_LOCAL_CANDIDATE_ID,   /**< Local candidate id associated with the candidate pair (Since 8.0) */
	WEBRTC_STATS_PROP_REMOTE_CANDIDATE_ID,  /**< Remote candidate id associated with the candidate pair (Since 8.0) */
} webrtc_stats_prop_e;

/**
 * @brief Enumeration for statistics property data type.
 * @since_tizen 7.0
 */
typedef enum {
	WEBRTC_STATS_PROP_TYPE_BOOL,    /**< Boolean type */
	WEBRTC_STATS_PROP_TYPE_INT,     /**< Signed integer type */
	WEBRTC_STATS_PROP_TYPE_UINT,    /**< Unsigned integer type */
	WEBRTC_STATS_PROP_TYPE_INT64,   /**< 64-bit signed integer type */
	WEBRTC_STATS_PROP_TYPE_UINT64,  /**< 64-bit unsigned integer type */
	WEBRTC_STATS_PROP_TYPE_FLOAT,   /**< Float type */
	WEBRTC_STATS_PROP_TYPE_DOUBLE,  /**< Double type */
	WEBRTC_STATS_PROP_TYPE_STRING,  /**< String type */
} webrtc_stats_prop_type_e;

/**
 * @brief The structure type for statistics property information.
 * @since_tizen 7.0
 */
typedef struct {
	const char *name;               /**< The property name */
	webrtc_stats_prop_e prop;       /**< The property enum */
	webrtc_stats_prop_type_e type;  /**< The property data type */
	/** The property value as per the data type above */
	union {
		bool v_bool;                /**< For boolean */
		int v_int;                  /**< For signed integer */
		unsigned int v_uint;        /**< For unsigned integer */
		int64_t v_int64;            /**< For 64-bit signed integer */
		uint64_t v_uint64;          /**< For 64-bit unsigned integer */
		float v_float;              /**< For float */
		double v_double;            /**< For double */
		const char *v_string;       /**< For string */
	};
} webrtc_stats_prop_info_s;

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MODULE
* @{
*/

/**
 * @brief Called when an error occurs.
 * @details The following error codes can be received:\n
 *          #WEBRTC_ERROR_INVALID_OPERATION\n
 *          #WEBRTC_ERROR_CONNECTION_FAILED\n
 *          #WEBRTC_ERROR_STREAM_FAILED\n
 *          #WEBRTC_ERROR_RESOURCE_FAILED\n
 *          #WEBRTC_ERROR_RESOURCE_CONFLICT\n
 *          #WEBRTC_ERROR_NETWORK_RESOURCE_FAILED (Since 8.0)
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] error      The error code
 * @param[in] state      The current state of the WebRTC handle
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_error_cb()
 * @see webrtc_unset_error_cb()
 */
typedef void (*webrtc_error_cb)(webrtc_h webrtc, webrtc_error_e error, webrtc_state_e state, void *user_data);

/**
 * @brief Called when the WebRTC state is changed.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] previous   The previous state of the WebRTC handle
 * @param[in] current    The current state of the WebRTC handle
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_state_changed_cb()
 * @see webrtc_unset_state_changed_cb()
 */
typedef void (*webrtc_state_changed_cb)(webrtc_h webrtc, webrtc_state_e previous, webrtc_state_e current, void *user_data);

/**
 * @brief Called when the session description is created.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.\n
 *          @a description is a JSON string.\n
 *          It will be {"sdp":{"type":"offer or answer","sdp":"..."}}.\n
 *          The @a description can be used only in the callback. To use outside, make a copy.
 * @param[in] webrtc       WebRTC handle
 * @param[in] description  The session description
 * @param[in] user_data    The user data passed from the callback registration function
 * @post @a description must be set as a local description by calling webrtc_set_local_description()
 * @post @a description must be sent to the remote peer via the signaling channel.
 * @see webrtc_create_offer_async()
 * @see webrtc_create_answer_async()
 * @see webrtc_set_local_description()
 */
typedef void (*webrtc_session_description_created_cb)(webrtc_h webrtc, const char *description, void *user_data);

/**
 * @brief Called when the WebRTC peer connection state is changed.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] state      The peer connection state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_peer_connection_state_change_cb()
 * @see webrtc_unset_peer_connection_state_change_cb()
 */
typedef void (*webrtc_peer_connection_state_change_cb)(webrtc_h webrtc, webrtc_peer_connection_state_e state, void *user_data);

/**
 * @brief Called when the WebRTC signaling state is changed.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] state      The signaling state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_signaling_state_change_cb()
 * @see webrtc_unset_signaling_state_change_cb()
 */
typedef void (*webrtc_signaling_state_change_cb)(webrtc_h webrtc, webrtc_signaling_state_e state, void *user_data);

/**
 * @brief Called when the WebRTC ICE gathering state is changed.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] state      The ICE gathering state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_ice_gathering_state_change_cb()
 * @see webrtc_unset_ice_gathering_state_change_cb()
 */
typedef void (*webrtc_ice_gathering_state_change_cb)(webrtc_h webrtc, webrtc_ice_gathering_state_e state, void *user_data);

/**
 * @brief Called when the WebRTC ICE connection state is changed.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] state      The ICE connection state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_ice_connection_state_change_cb()
 * @see webrtc_unset_ice_connection_state_change_cb()
 */
typedef void (*webrtc_ice_connection_state_change_cb)(webrtc_h webrtc, webrtc_ice_connection_state_e state, void *user_data);

/**
 * @brief Called iteratively to report all the TURN server URLs.
 * @since_tizen 6.5
 * @remarks The @a turn_server can be used only in the callback. To use outside, make a copy.
 * @param[in] turn_server  The TURN server URL
 * @param[in] user_data    The user data passed from the callback registration function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @see webrtc_foreach_turn_server()
 */
typedef bool (*webrtc_turn_server_cb)(const char *turn_server, void *user_data);

/**
 * @brief Called when the WebRTC needs session negotiation.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_negotiation_needed_cb()
 * @see webrtc_unset_negotiation_needed_cb()
 */
typedef void (*webrtc_negotiation_needed_cb)(webrtc_h webrtc, void *user_data);

/**
 * @brief Called when the WebRTC needs to send the ICE candidate message to the remote peer through the signaling channel.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.\n
 *          The @a candidate can be used only in the callback. To use outside, make a copy.\n
 *          It will be in the form: {"ice":{"candidate":"...", "sdpMLineIndex":0 or 1 or ...}}. \n\n
 *          Since 10.0, at the end of ICE gathering, an empty candidate may be delivered in the form:
 *          {"ice":{"candidate":"","sdpMLineIndex":0 or 1, ...}}.\n
 *          This indicates the end-of-candidates marker, meaning no further candidates will be provided.
 * @param[in] webrtc     WebRTC handle
 * @param[in] candidate  The ICE candidate message
 * @param[in] user_data  The user data passed from the callback registration function
 * @post @a candidate must be sent to the remote peer via the signaling channel.
 * @see webrtc_set_ice_candidate_cb()
 * @see webrtc_unset_ice_candidate_cb()
 */
typedef void (*webrtc_ice_candidate_cb)(webrtc_h webrtc, const char *candidate, void *user_data);

/**
 * @brief Called when a new track has been added to the WebRTC pipeline after the negotiation.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] type       The media type
 * @param[in] track_id   The track id
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_track_added_cb()
 * @see webrtc_unset_track_added_cb()
 */
typedef void (*webrtc_track_added_cb)(webrtc_h webrtc, webrtc_media_type_e type, unsigned int track_id, void *user_data);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MEDIA_SOURCE_MODULE
* @{
*/

/**
 * @brief Called when the buffer state of media packet source is changed.
 * @since_tizen 6.5
 * @param[in] source_id  The media source id
 * @param[in] state      The buffer state (underflow or overflow)
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_media_packet_source_set_buffer_state_changed_cb()
 * @see webrtc_media_packet_source_unset_buffer_state_changed_cb()
 */
typedef void (*webrtc_media_packet_source_buffer_state_changed_cb)(unsigned int source_id, webrtc_media_packet_source_buffer_state_e state, void *user_data);


/**
 * @brief Called iteratively to report all the supported transceiver codecs.
 * @since_tizen 7.0
 * @param[in] codec        The supported codec
 * @param[in] user_data    The user data passed from the callback registration function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @see webrtc_media_source_foreach_supported_transceiver_codec()
 */
typedef bool (*webrtc_media_source_supported_transceiver_codec_cb)(webrtc_transceiver_codec_e codec, void *user_data);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MEDIA_RENDER_MODULE
* @{
*/

/**
 * @brief Called when each audio or video frame is ready to be rendered via the WebRTC pipeline after the negotiation.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.\n
 *          Use media_packet_get_buffer_data_ptr() with @a packet to get the Gstreamer buffer pointer.\n
 *          The @a packet should be released using media_packet_destroy().
 * @param[in] webrtc     WebRTC handle
 * @param[in] type       The media type
 * @param[in] track_id   The track id
 * @param[in] packet     The media packet which has a frame data
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_set_encoded_audio_frame_cb()
 * @see webrtc_unset_encoded_audio_frame_cb()
 * @see webrtc_set_encoded_video_frame_cb()
 * @see webrtc_unset_encoded_video_frame_cb()
 * @see media_packet_get_buffer_data_ptr()
 */
typedef void (*webrtc_encoded_frame_cb)(webrtc_h webrtc, webrtc_media_type_e type, unsigned int track_id, media_packet_h packet, void *user_data);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_DATA_CHANNEL_MODULE
* @{
*/

/**
 * @brief Called when the data channel is created to the connection by the remote peer.
 * @since_tizen 6.5
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.\n
 *          The @a channel should not be released.
 * @param[in] webrtc     WebRTC handle
 * @param[in] channel    WebRTC data channel handle
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_data_channel_set_open_cb()
 * @see webrtc_data_channel_unset_open_cb()
 * @see webrtc_data_channel_set_message_cb()
 * @see webrtc_data_channel_unset_message_cb()
 * @see webrtc_data_channel_set_error_cb()
 * @see webrtc_data_channel_unset_error_cb()
 * @see webrtc_data_channel_set_close_cb()
 * @see webrtc_data_channel_unset_close_cb()
 */
typedef void (*webrtc_data_channel_cb)(webrtc_h webrtc, webrtc_data_channel_h channel, void *user_data);

/**
 * @brief Called when the data channel's underlying data transport is established.
 * @since_tizen 6.5
 * @remarks The @a channel is the same object for which the callback was set.\n
 *          The @a channel should not be released.
 * @param[in] channel    WebRTC data channel handle
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_create_data_channel()
 * @see webrtc_destroy_data_channel()
 * @see webrtc_data_channel_set_open_cb()
 * @see webrtc_data_channel_unset_open_cb()
 */
typedef void (*webrtc_data_channel_open_cb)(webrtc_data_channel_h channel, void *user_data);

/**
 * @brief Called when a message is received from other peer via the data channel.
 * @since_tizen 6.5
 * @remarks The @a channel is the same object for which the callback was set.\n
 *          The @a channel should not be released.\n
 *          When @a type is #WEBRTC_DATA_CHANNEL_TYPE_STRING, @a message should be casted to char pointer.\n
 *          When @a type is #WEBRTC_DATA_CHANNEL_TYPE_BYTES, @a message should be casted to #webrtc_bytes_data_h.
 *          In this case, webrtc_get_data() can be used to get the data and its size inside of this callback.\n
 *          The @a message should not be released.
 * @param[in] channel    WebRTC data channel handle
 * @param[in] type       The data type
 * @param[in] message    The message from the remote peer
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_create_data_channel()
 * @see webrtc_destroy_data_channel()
 * @see webrtc_data_channel_set_message_cb()
 * @see webrtc_data_channel_unset_message_cb()
 * @see webrtc_get_data()
 */
typedef void (*webrtc_data_channel_message_cb)(webrtc_data_channel_h channel, webrtc_data_channel_type_e type, void *message, void *user_data);

/**
 * @brief Called when an error occurs on the data channel.
 * @details The following error codes can be received:\n
 *          #WEBRTC_ERROR_INVALID_OPERATION\n
 *          #WEBRTC_ERROR_STREAM_FAILED\n
 *          #WEBRTC_ERROR_RESOURCE_FAILED
 * @since_tizen 6.5
 * @remarks The @a channel is the same object for which the callback was set.\n
 *          The @a channel should not be released.
 * @param[in] channel    WebRTC data channel handle
 * @param[in] error      The error code
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_create_data_channel()
 * @see webrtc_destroy_data_channel()
 * @see webrtc_data_channel_set_error_cb()
 * @see webrtc_data_channel_unset_error_cb()
 */
typedef void (*webrtc_data_channel_error_cb)(webrtc_data_channel_h channel, webrtc_error_e error, void *user_data);

/**
 * @brief Called when the data channel has closed down.
 * @since_tizen 6.5
 * @remarks The @a channel is the same object for which the callback was set.\n
 *          The @a channel should not be released.
 * @param[in] channel    WebRTC data channel handle
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_create_data_channel()
 * @see webrtc_destroy_data_channel()
 * @see webrtc_data_channel_set_close_cb()
 * @see webrtc_data_channel_unset_close_cb()
 */
typedef void (*webrtc_data_channel_close_cb)(webrtc_data_channel_h channel, void *user_data);

/**
 * @brief Called when the number of bytes of data currently queued to be sent over the data channel falls below the threshold.
 * @since_tizen 7.0
 * @remarks The @a channel is the same object for which the callback was set.\n
 *          The @a channel should not be released.
 * @param[in] channel    WebRTC data channel handle
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_create_data_channel()
 * @see webrtc_destroy_data_channel()
 * @see webrtc_data_channel_set_buffered_amount_low_cb()
 * @see webrtc_data_channel_unset_buffered_amount_low_cb()
 */
typedef void (*webrtc_data_channel_buffered_amount_low_cb)(webrtc_data_channel_h channel, void *user_data);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_STATISTICS_MODULE
* @{
*/

/**
 * @brief Called iteratively to report all the statistics properties.
 * @since_tizen 7.0
 * @remarks The @a prop_info can be used only in the callback. To use outside, make a copy.
 * @param[in] type       The statistics type
 * @param[in] prop_info  The statistics property information
 * @param[in] user_data  The user data passed from the callback registration function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @see webrtc_foreach_stats()
 * @par Example
 * @code
	bool __stats_cb(webrtc_stats_type_e type, const webrtc_stats_prop_info_s *prop_info, void *user_data)
	{
		switch (prop_info->type) {
		case WEBRTC_STATS_PROP_TYPE_BOOL:
			printf("type[0x%x] prop[%s, 0x%08x, value:%d]\n", type, prop_info->name, prop_info->prop, prop_info->v_bool);
			break;
		case WEBRTC_STATS_PROP_TYPE_INT:
			printf("type[0x%x] prop[%s, 0x%08x, value:%d]\n", type, prop_info->name, prop_info->prop, prop_info->v_int);
			break;
		case WEBRTC_STATS_PROP_TYPE_INT64:
			printf("type[0x%x] prop[%s, 0x%08x, value:%"PRId64"]\n", type, prop_info->name, prop_info->prop, prop_info->v_int64);
			break;
		case WEBRTC_STATS_PROP_TYPE_UINT:
			printf("type[0x%x] prop[%s, 0x%08x, value:%u]\n", type, prop_info->name, prop_info->prop, prop_info->v_uint);
			break;
		case WEBRTC_STATS_PROP_TYPE_UINT64:
			printf("type[0x%x] prop[%s, 0x%08x, value:%"PRIu64"]\n", type, prop_info->name, prop_info->prop, prop_info->v_uint64);
			break;
		case WEBRTC_STATS_PROP_TYPE_FLOAT:
			printf("type[0x%x] prop[%s, 0x%08x, value:%f]\n", type, prop_info->name, prop_info->prop, prop_info->v_float);
			break;
		case WEBRTC_STATS_PROP_TYPE_DOUBLE:
			printf("type[0x%x] prop[%s, 0x%08x, value:%lf]\n", type, prop_info->name, prop_info->prop, prop_info->v_double);
			break;
		case WEBRTC_STATS_PROP_TYPE_STRING:
			printf("type[0x%x] prop[%s, 0x%08x, value:%s]\n", type, prop_info->name, prop_info->prop, prop_info->v_string);
			break;
		}
		return true;
	}
 * @endcode
 */
typedef bool (*webrtc_stats_cb)(webrtc_stats_type_e type, const webrtc_stats_prop_info_s *prop_info, void *user_data);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MODULE
* @{
*/

/**
 * @brief Sets a callback function to be invoked when an asynchronous operation error occurs.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @post webrtc_error_cb() will be invoked.
 * @see webrtc_unset_error_cb()
 * @see webrtc_error_cb()
 */
int webrtc_set_error_cb(webrtc_h webrtc, webrtc_error_cb callback, void *user_data);

/**
 * @brief Unsets the error callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_set_error_cb()
 */
int webrtc_unset_error_cb(webrtc_h webrtc);

/**
 * @brief Creates an instance of WebRTC.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks A signaling channel not addressed in this API should be established to send SDP or ICE candidate messages to each other.\n
 *          The @a webrtc should be released using webrtc_destroy().
 * @param[out] webrtc    WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @post @a webrtc state will be #WEBRTC_STATE_IDLE.
 * @see webrtc_destroy()
 */
int webrtc_create(webrtc_h *webrtc);

/**
 * @brief Starts the WebRTC.
 * @since_tizen 6.5
 * @param[in] webrtc    WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @retval #WEBRTC_ERROR_RESOURCE_FAILED Resource failed
 * @pre webrtc_ice_candidate_cb() must be set by calling webrtc_set_ice_candidate_cb().
 * @pre webrtc_media_packet_source_set_format() must be called if @a webrtc has a media packet source.
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_state_changed_cb() will be invoked.
 * @post @a webrtc state will be #WEBRTC_STATE_NEGOTIATING.
 * @see webrtc_create()
 * @see webrtc_stop()
 * @see webrtc_start_sync()
 */
int webrtc_start(webrtc_h webrtc);

/**
 * @brief Starts the WebRTC synchronously.
 * @since_tizen 9.0
 * @remarks The main difference with webrtc_start() is that this returns after changing state to #WEBRTC_STATE_NEGOTIATING.\n
 *          webrtc_state_changed_cb() will not be invoked.
 * @param[in] webrtc    WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @retval #WEBRTC_ERROR_RESOURCE_FAILED Resource failed
 * @pre webrtc_ice_candidate_cb() must be set by calling webrtc_set_ice_candidate_cb().
 * @pre webrtc_media_packet_source_set_format() must be called if @a webrtc has a media packet source.
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post @a webrtc state will be #WEBRTC_STATE_NEGOTIATING.
 * @see webrtc_create()
 * @see webrtc_start()
 * @see webrtc_stop()
 */
int webrtc_start_sync(webrtc_h webrtc);

/**
 * @brief Stops the WebRTC.
 * @since_tizen 6.5
 * @param[in] webrtc    WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @retval #WEBRTC_ERROR_RESOURCE_FAILED Resource failed
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @post @a webrtc state will be #WEBRTC_STATE_IDLE.
 * @see webrtc_create()
 * @see webrtc_start()
 */
int webrtc_stop(webrtc_h webrtc);

/**
 * @brief Destroys the WebRTC.
 * @since_tizen 6.5
 * @param[in] webrtc    WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_create()
 */
int webrtc_destroy(webrtc_h webrtc);

/**
 * @brief Sets a callback function to be invoked when the WebRTC state is changed.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in the main thread.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @post webrtc_state_changed_cb() will be invoked.
 * @see webrtc_unset_state_changed_cb()
 * @see webrtc_state_changed_cb()
 * @see webrtc_get_state()
 * @see webrtc_create_offer()
 * @see webrtc_create_offer_async()
 */
int webrtc_set_state_changed_cb(webrtc_h webrtc, webrtc_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the state changed callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_set_state_changed_cb()
 */
int webrtc_unset_state_changed_cb(webrtc_h webrtc);

/**
 * @brief Gets the WebRTC state.
 * @since_tizen 6.5
 * @param[in] webrtc    WebRTC handle
 * @param[out] state    WebRTC state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_set_state_changed_cb()
 * @see webrtc_unset_state_changed_cb()
 */
int webrtc_get_state(webrtc_h webrtc, webrtc_state_e *state);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MEDIA_SOURCE_MODULE
* @{
*/

/**
 * @brief Adds a media source.
 * @since_tizen 6.5
 * @remarks The camera privilege(%http://tizen.org/privilege/camera) should be added if @a type is #WEBRTC_MEDIA_SOURCE_TYPE_CAMERA.\n
 *          The recorder privilege(%http://tizen.org/privilege/recorder) should be added if @a type is #WEBRTC_MEDIA_SOURCE_TYPE_MIC.\n
 *          With @a type of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, you can configure a transceiver only for receiving audio or video stream. (Since 7.0)\n
 *          @a type of #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN is not allowed to be used by third-party applications due to the security reasons. (Since 7.0)\n
 *          The display feature(%http://tizen.org/feature/display) is required if @a type is #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN. (Since 8.0)\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] type        The media source type to be added
 * @param[out] source_id  The media source id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_remove_media_source()
 * @see webrtc_media_source_set_transceiver_direction()
 * @see webrtc_media_source_get_transceiver_direction()
 * @see webrtc_media_source_foreach_supported_transceiver_codec()
 * @see webrtc_media_source_set_transceiver_codec()
 * @see webrtc_media_source_get_transceiver_codec()
 * @see webrtc_media_source_set_pause()
 * @see webrtc_media_source_get_pause()
 * @see webrtc_media_source_set_mute()
 * @see webrtc_media_source_get_mute()
 * @see webrtc_media_source_set_encoder_bitrate()
 * @see webrtc_media_source_get_encoder_bitrate()
 * @see webrtc_media_source_set_video_resolution()
 * @see webrtc_media_source_get_video_resolution()
 * @see webrtc_media_source_set_video_framerate()
 * @see webrtc_media_source_get_video_framerate()
 * @see webrtc_media_source_set_audio_loopback()
 * @see webrtc_media_source_unset_audio_loopback()
 * @see webrtc_media_source_set_video_loopback()
 * @see webrtc_media_source_unset_video_loopback()
 * @see webrtc_mic_source_set_sound_stream_info()
 * @see webrtc_camera_source_set_device_id()
 * @see webrtc_camera_source_get_device_id()
 * @see webrtc_file_source_set_path()
 * @see webrtc_file_source_set_looping()
 * @see webrtc_file_source_get_looping()
 * @see webrtc_media_packet_source_set_buffer_state_changed_cb()
 * @see webrtc_media_packet_source_unset_buffer_state_changed_cb()
 * @see webrtc_media_packet_source_set_format()
 * @see webrtc_media_packet_source_push_packet()
 */
int webrtc_add_media_source(webrtc_h webrtc, webrtc_media_source_type_e type, unsigned int *source_id);

/**
 * @brief Removes the media source.
 * @since_tizen 6.5
 * @remarks #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id to be removed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_add_media_source()
 */
int webrtc_remove_media_source(webrtc_h webrtc, unsigned int source_id);

/**
 * @brief Sets the transceiver direction to the media source with specified media type.
 * @since_tizen 6.5
 * @remarks If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, it only allows #WEBRTC_TRANSCEIVER_DIRECTION_RECVONLY. (Since 7.0)\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[in] direction   The transceiver direction to set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @pre webrtc_media_packet_source_set_format() must be called if @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET.
 * @see webrtc_media_source_get_transceiver_direction()
 */
int webrtc_media_source_set_transceiver_direction(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, webrtc_transceiver_direction_e direction);

/**
 * @brief Gets the transceiver direction of the media source with specified media type.
 * @since_tizen 6.5
 * @remarks The default value is #WEBRTC_TRANSCEIVER_DIRECTION_SENDRECV except for a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL.\n
 *          If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, the default value is #WEBRTC_TRANSCEIVER_DIRECTION_RECVONLY. (Since 7.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[out] direction  Current transceiver direction
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @pre webrtc_media_packet_source_set_format() must be called if @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET.
 * @see webrtc_media_source_set_transceiver_direction()
 */
int webrtc_media_source_get_transceiver_direction(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, webrtc_transceiver_direction_e *direction);

/**
 * @brief Retrieves all the supported transceiver codecs.
 * @since_tizen 7.0
 * @remarks If @a source_type is #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET,
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_type The media source type
 * @param[in] media_type  The media type
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_media_source_supported_transceiver_codec_cb()
 * @see webrtc_media_source_set_transceiver_codec()
 * @see webrtc_media_source_get_transceiver_codec()
 */
int webrtc_media_source_foreach_supported_transceiver_codec(webrtc_h webrtc, webrtc_media_source_type_e source_type, webrtc_media_type_e media_type, webrtc_media_source_supported_transceiver_codec_cb callback, void *user_data);

/**
 * @brief Sets the transceiver codec to the media source.
 * @since_tizen 7.0
 * @remarks If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET,
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[in] codec       The transceiver codec to set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_get_transceiver_codec()
 * @see webrtc_media_source_foreach_supported_transceiver_codec()
 */
int webrtc_media_source_set_transceiver_codec(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, webrtc_transceiver_codec_e codec);

/**
 * @brief Gets the transceiver codec of the media source.
 * @since_tizen 7.0
 * @remarks If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET,
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[out] codec      Current transceiver codec
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_set_transceiver_codec()
 * @see webrtc_media_source_foreach_supported_transceiver_codec()
 */
int webrtc_media_source_get_transceiver_codec(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, webrtc_transceiver_codec_e *codec);

/**
 * @brief Sets the RTP payload type to the media source.
 * @since_tizen 9.0
 * @remarks This function would be used to set a specific payload type of a codec complying with a remote peer's offer description.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] codec       The transceiver codec
 * @param[in] pt          The RTP payload type\n
 *                        Valid range is 96 ~ 127.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_get_payload_type()
 * @see webrtc_media_source_get_transceiver_codec()
 * @see webrtc_media_source_set_transceiver_codec()
 * @see webrtc_media_source_foreach_supported_transceiver_codec()
 */
int webrtc_media_source_set_payload_type(webrtc_h webrtc, unsigned int source_id, webrtc_transceiver_codec_e codec, unsigned int pt);

/**
 * @brief Gets the RTP payload type of the media source.
 * @since_tizen 9.0
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] codec       The transceiver codec
 * @param[out] pt         The RTP payload type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_set_payload_type()
 * @see webrtc_media_source_get_transceiver_codec()
 * @see webrtc_media_source_set_transceiver_codec()
 * @see webrtc_media_source_foreach_supported_transceiver_codec()
 */
int webrtc_media_source_get_payload_type(webrtc_h webrtc, unsigned int source_id, webrtc_transceiver_codec_e codec, unsigned int *pt);

/**
 * @brief Sets pause to the media source.
 * @since_tizen 6.5
 * @remarks If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, this function will return #WEBRTC_ERROR_INVALID_PARAMETER. (Since 7.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[in] pause       Pause or play (@c true = pause, @c false = play)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_media_source_get_pause()
 */
int webrtc_media_source_set_pause(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, bool pause);

/**
 * @brief Gets the pause state of the media source.
 * @since_tizen 6.5
 * @remarks The default value is @c false.\n
 *          If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, this function will return #WEBRTC_ERROR_INVALID_PARAMETER. (Since 7.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[out] paused     Paused or not (@c true = paused, @c false = playing)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_media_source_set_pause()
 */
int webrtc_media_source_get_pause(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, bool *paused);

 /**
 * @brief Sets mute to the media source.
 * @details If @a mute is set to @c true, black frame or silent sound will be transmitted to the remote peer according to the media type of the @a source_id. \n
 *          If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET or #WEBRTC_MEDIA_SOURCE_TYPE_NULL (Since 7.0),
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[in] mute        Mute or not (@c true = mute, @c false = not mute)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_media_source_get_mute()
 */
int webrtc_media_source_set_mute(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, bool mute);

 /**
 * @brief Gets the mute state of the media source.
 * @details If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET or #WEBRTC_MEDIA_SOURCE_TYPE_NULL (Since 7.0),
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @since_tizen 6.5
 * @remarks The default value is @c false.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[out] muted      Muted or not (@c true = muted, @c false = not muted)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_media_source_set_mute()
 */
int webrtc_media_source_get_mute(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, bool *muted);

/**
 * @brief Sets an encoder target bitrate(in bits/sec) to the media source.
 * @details If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET or #WEBRTC_MEDIA_SOURCE_TYPE_NULL,
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @since_tizen 7.0
 * @param[in] webrtc         WebRTC handle
 * @param[in] source_id      The media source id
 * @param[in] media_type     The media type
 * @param[in] target_bitrate The target bitrate of the encoder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_add_media_source()
 * @see webrtc_media_source_get_encoder_bitrate()
 */
int webrtc_media_source_set_encoder_bitrate(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, int target_bitrate);

/**
 * @brief Gets the encoder target bitrate(in bits/sec) of the media source.
 * @details If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_MEDIA_PACKET or #WEBRTC_MEDIA_SOURCE_TYPE_NULL,
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @since_tizen 7.0
 * @param[in] webrtc          WebRTC handle
 * @param[in] source_id       The media source id
 * @param[in] media_type      The media type
 * @param[out] target_bitrate The target bitrate of the encoder
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_add_media_source()
 * @see webrtc_media_source_set_encoder_bitrate()
 */
int webrtc_media_source_get_encoder_bitrate(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, int *target_bitrate);

/**
 * @brief Sets a video resolution to the media source.
 * @details The following media source types contain video:\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_CAMERA\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN
 * @since_tizen 6.5
 * @remarks If @a source_id does not support for the dynamic resolution change, #WEBRTC_ERROR_INVALID_OPERATION will be returned\n
 *          while @a webrtc state is #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 *          Since 7.0, this function supports dynamic resolution change regardless of state for all video sources mentioned in details.\n
 *          However, some sources cannot be changed to a greater value than the resolution set in the #WEBRTC_STATE_IDLE.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The video source id
 * @param[in] width       The video width
 * @param[in] height      The video height
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_get_video_resolution()
 */
int webrtc_media_source_set_video_resolution(webrtc_h webrtc, unsigned int source_id, int width, int height);

/**
 * @brief Gets the video resolution of the media source.
 * @details The following media source types contain video:\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_CAMERA\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The video source id
 * @param[out] width      The video width
 * @param[out] height     The video height
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_set_video_resolution()
 */
int webrtc_media_source_get_video_resolution(webrtc_h webrtc, unsigned int source_id, int *width, int *height);

/**
 * @brief Sets a video source frame rate, also known as FPS.
 * @details The following media source types contain video:\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_CAMERA\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN
 * @since_tizen 7.0
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The video source id
 * @param[in] framerate   The video frame rate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_add_media_source()
 * @see webrtc_media_source_get_video_framerate()
 */
int webrtc_media_source_set_video_framerate(webrtc_h webrtc, unsigned int source_id, int framerate);

/**
 * @brief Gets the video source frame rate, also known as FPS.
 * @details The following media source types contain video:\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_CAMERA\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN
 * @since_tizen 7.0
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The video source id
 * @param[out] framerate  The video frame rate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_add_media_source()
 * @see webrtc_media_source_set_video_framerate()
 */
int webrtc_media_source_get_video_framerate(webrtc_h webrtc, unsigned int source_id, int *framerate);

/**
 * @brief Sets the mic source's sound manager stream information.
 * @details If @a source_id is not a media source of #WEBRTC_MEDIA_SOURCE_TYPE_MIC, this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @since_tizen 6.5
 * @remarks You can set sound stream information including audio routing.\n
 *          The following sound stream types can be used to create the @a stream_info :\n
 *          #SOUND_STREAM_TYPE_MEDIA\n
 *          #SOUND_STREAM_TYPE_VOICE_RECOGNITION\n
 *          #SOUND_STREAM_TYPE_VOIP\n
 *          #SOUND_STREAM_TYPE_MEDIA_EXTERNAL_ONLY\n
 *          For more details, please refer to @ref CAPI_MEDIA_SOUND_MANAGER_MODULE .\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The mic source id
 * @param[in] stream_info The sound stream information
 * @return @c 0 on success, otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see #sound_stream_info_h
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int webrtc_mic_source_set_sound_stream_info(webrtc_h webrtc, unsigned int source_id, sound_stream_info_h stream_info);

/**
 * @brief Sets a camera device id to the camera source.
 * @since_tizen 7.0
 * @remarks #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The camera source id
 * @param[in] device_id   The camera device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add camera source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_add_media_source()
 * @see webrtc_camera_source_get_device_id()
 */
int webrtc_camera_source_set_device_id(webrtc_h webrtc, unsigned int source_id, unsigned int device_id);

/**
 * @brief Gets the camera device id of the camera source.
 * @since_tizen 7.0
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The camera source id
 * @param[out] device_id  The camera device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add camera source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_add_media_source()
 * @see webrtc_camera_source_set_device_id()
 */
int webrtc_camera_source_get_device_id(webrtc_h webrtc, unsigned int source_id, unsigned int *device_id);

/**
 * @brief Sets a callback function to be invoked when the buffer state of media packet source is changed.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media packet source id
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Add media packet source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @post webrtc_media_packet_source_buffer_state_changed_cb() will be invoked.
 * @see webrtc_media_packet_source_push_packet()
 * @see webrtc_media_packet_source_unset_buffer_state_changed_cb()
 * @see webrtc_media_packet_source_buffer_state_changed_cb()
 */
int webrtc_media_packet_source_set_buffer_state_changed_cb(webrtc_h webrtc, unsigned int source_id, webrtc_media_packet_source_buffer_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the buffer state changed callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media packet source id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media packet source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_packet_source_set_buffer_state_changed_cb()
 */
int webrtc_media_packet_source_unset_buffer_state_changed_cb(webrtc_h webrtc, unsigned int source_id);

/**
 * @brief Sets media format to the media packet source.
 * @since_tizen 6.5
 * @remarks The following media format mimetypes can be used to create the @a format :\n
 *          #MEDIA_FORMAT_VORBIS\n
 *          #MEDIA_FORMAT_OPUS\n
 *          #MEDIA_FORMAT_PCM_S16LE\n
 *          #MEDIA_FORMAT_PCMU\n
 *          #MEDIA_FORMAT_PCMA\n
 *          #MEDIA_FORMAT_H264_SP\n
 *          #MEDIA_FORMAT_H264_MP\n
 *          #MEDIA_FORMAT_H264_HP\n
 *          #MEDIA_FORMAT_MJPEG\n
 *          #MEDIA_FORMAT_VP8\n
 *          #MEDIA_FORMAT_VP9\n
 *          #MEDIA_FORMAT_I420\n
 *          #MEDIA_FORMAT_NV12\n
 *          For more details, please refer to @ref CAPI_MEDIA_TOOL_MEDIA_FORMAT_MODULE .\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media packet source id
 * @param[in] format      The media format
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media packet source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_packet_source_push_packet()
 * @see webrtc_media_packet_source_set_buffer_state_changed_cb()
 */
int webrtc_media_packet_source_set_format(webrtc_h webrtc, unsigned int source_id, media_format_h format);

/**
 * @brief Pushes media packet to the media packet source.
 * @since_tizen 6.5
 * @remarks This function takes ownership of the @a packet.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media packet source id
 * @param[in] packet      The media packet
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media packet source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @pre webrtc_media_packet_source_set_format() must be called before calling this function.
 * @pre webrtc_media_packet_source_buffer_state_changed_cb() must be set by calling webrtc_media_packet_source_set_buffer_state_changed_cb().
 * @see webrtc_media_packet_source_set_format()
 * @see webrtc_media_packet_source_set_buffer_state_changed_cb()
 */
int webrtc_media_packet_source_push_packet(webrtc_h webrtc, unsigned int source_id, media_packet_h packet);

/**
 * @brief Sets media path of the file source.
 * @since_tizen 7.0
 * @remarks %http://tizen.org/privilege/mediastorage is needed if path is relevant to internal media storage.\n
 *          %http://tizen.org/privilege/externalstorage is needed if path is relevant to external media storage.\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc       WebRTC handle
 * @param[in] source_id    The file source id
 * @param[in] path         The media path
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Add file source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 */
int webrtc_file_source_set_path(webrtc_h webrtc, unsigned int source_id, const char *path);

/**
 * @brief Sets looping mode of the file source.
 * @details If @a looping is @c true, the transfer starts again from the beginning of the file source after reaching the end of the file.
 *          If it is @c false, it won't. The default value is @c false.
 * @since_tizen 7.0
 * @param[in] webrtc       WebRTC handle
 * @param[in] source_id    The file source id
 * @param[in] looping      The new looping state: (@c true = looping, @c false = non-looping)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_file_source_get_looping()
 */
int webrtc_file_source_set_looping(webrtc_h webrtc, unsigned int source_id, bool looping);

/**
 * @brief Gets looping mode of the file source.
 * @since_tizen 7.0
 * @remarks The default value is @c false.
 * @param[in] webrtc       WebRTC handle
 * @param[in] source_id    The file source id
 * @param[out] looping     The looping state: (@c true = looping, @c false = non-looping)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_file_source_set_looping()
 */
int webrtc_file_source_get_looping(webrtc_h webrtc, unsigned int source_id, bool *looping);

/**
 * @platform
 * @brief Sets the crop coordinates of a screen source.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/internal/default/platform
 * @remarks The coordinates are in UI (screen) units, and the start coordinate refers to the upper left corner of the video display area on the screen.\n
 *          If we give a rectangle within that area (described by x, y, width, height) then this sub-area will be sent to peer and
 *          it changes the video resolution.
 *          The changed resolution can be checked through webrtc_media_source_get_video_resolution().\n
 *          Even when the crop area is changed again, the rectangle coordinates are still based on the original UI (screen) as in the first setting.\n
 *          If the video resolution is changed by webrtc_media_source_set_video_resolution(), then crop cannot be performed.\n
 *          Crop and resolution change cannot be done at the same time.
 * @param[in] webrtc     WebRTC handle
 * @param[in] source_id  The screen source id
 * @param[in] x          The start X coordinate of the UI for cropping
 * @param[in] y          The start Y coordinate of the UI for cropping
 * @param[in] width      Width of the UI for cropping
 * @param[in] height     Height of the UI for cropping
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre Add screen source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @see webrtc_screen_source_unset_crop()
 * @see webrtc_media_source_get_video_resolution()
 */
int webrtc_screen_source_set_crop(webrtc_h webrtc, unsigned int source_id, int x, int y, int width, int height);

/**
 * @platform
 * @brief Unsets the crop coordinates of a screen source.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/internal/default/platform
 * @param[in] webrtc       WebRTC handle
 * @param[in] source_id    The screen source id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre Add screen source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @see webrtc_screen_source_set_crop()
 */
int webrtc_screen_source_unset_crop(webrtc_h webrtc, unsigned int source_id);

/**
 * @brief Starts the media source.
 * @since_tizen 9.0
 * @remarks This function will be used after adding a media source during state of #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.\n
 *          If the state is #WEBRTC_STATE_IDLE, this function will have no effect. Rather webrtc_start() will start media sources later.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_add_media_source()
 * @see webrtc_remove_media_source()
 * @see webrtc_start()
 */
int webrtc_start_media_source(webrtc_h webrtc, unsigned int source_id);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MEDIA_RENDER_MODULE
* @{
*/

/**
 * @brief Sets a sound manager stream information to the audio track to be rendered.
 * @since_tizen 6.5
 * @remarks Call this function within webrtc_track_added_cb(), otherwise #WEBRTC_ERROR_INVALID_OPERATION will be returned.\n
 *          If webrtc_set_encoded_audio_frame_cb() has been called, it will return #WEBRTC_ERROR_INVALID_OPERATION.\n
 *          The following sound stream types can be used to create the @a stream_info :\n
 *          #SOUND_STREAM_TYPE_MEDIA\n
 *          #SOUND_STREAM_TYPE_VOIP\n
 *          #SOUND_STREAM_TYPE_MEDIA_EXTERNAL_ONLY\n
 *          For more details, please refer to @ref CAPI_MEDIA_SOUND_MANAGER_MODULE.
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] stream_info The sound stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @see webrtc_set_track_added_cb()
 * @see webrtc_unset_track_added_cb()
 */
int webrtc_set_sound_stream_info(webrtc_h webrtc, unsigned int track_id, sound_stream_info_h stream_info);

/**
 * @brief Sets mute to the audio track.
 * @since_tizen 8.0
 * @remarks If @a mute is set to @c true, playback of audio track received from a remote peer will be muted.
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] mute        Mute or not (@c true = mute, @c false = not mute)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @pre Call webrtc_set_sound_stream_info() before calling this function.
 * @see webrtc_get_audio_mute()
 */
int webrtc_set_audio_mute(webrtc_h webrtc, unsigned int track_id, bool mute);

/**
 * @brief Gets the mute state of the audio track.
 * @since_tizen 8.0
 * @remarks The default value is @c false.
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[out] muted      Muted or not (@c true = muted, @c false = not muted)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @pre Call webrtc_set_sound_stream_info() before calling this function.
 * @see webrtc_set_audio_mute()
 */
int webrtc_get_audio_mute(webrtc_h webrtc, unsigned int track_id, bool *muted);

/**
 * @brief Sets a display to the video track to be rendered.
 * @since_tizen 6.5
 * @remarks Call this function within webrtc_track_added_cb(), otherwise #WEBRTC_ERROR_INVALID_OPERATION will be returned.\n
 *          If webrtc_set_encoded_video_frame_cb() has been called, it will return #WEBRTC_ERROR_INVALID_OPERATION.\n
 *          The display feature(%http://tizen.org/feature/display) is required. (Since 8.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] type        The display type
 * @param[in] display     The display handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @see webrtc_set_track_added_cb()
 * @see webrtc_unset_track_added_cb()
 * @see webrtc_set_display_mode()
 * @see webrtc_get_display_mode()
 * @see webrtc_set_display_visible()
 * @see webrtc_get_display_visible()
 */
int webrtc_set_display(webrtc_h webrtc, unsigned int track_id, webrtc_display_type_e type, webrtc_display_h display);

/**
 * @brief Sets the display mode of the video track.
 * @since_tizen 6.5
 * @remarks The display feature(%http://tizen.org/feature/display) is required. (Since 8.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] mode        The display mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre For remote video track, webrtc_set_display() must be called with @a track_id from webrtc_track_added_cb().\n
 *      For loopback video track, webrtc_media_source_set_video_loopback() must be called to get @a track_id.
 * @see webrtc_get_display_mode()
 */
int webrtc_set_display_mode(webrtc_h webrtc, unsigned int track_id, webrtc_display_mode_e mode);

/**
 * @brief Gets the display mode of the video track.
 * @since_tizen 6.5
 * @remarks The default value is #WEBRTC_DISPLAY_MODE_LETTER_BOX.\n
 *          The display feature(%http://tizen.org/feature/display) is required. (Since 8.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[out] mode       The display mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre For remote video track, webrtc_set_display() must be called with @a track_id from webrtc_track_added_cb().\n
 *      For loopback video track, webrtc_media_source_set_video_loopback() must be called to get @a track_id.
 * @see webrtc_set_display_mode()
 */
int webrtc_get_display_mode(webrtc_h webrtc, unsigned int track_id, webrtc_display_mode_e *mode);

/**
 * @brief Sets the display visibleness of the video track.
 * @since_tizen 6.5
 * @remarks The display feature(%http://tizen.org/feature/display) is required. (Since 8.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] visible     The display visibleness
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre For remote video track, webrtc_set_display() must be called with @a track_id from webrtc_track_added_cb().\n
 *      For loopback video track, webrtc_media_source_set_video_loopback() must be called to get @a track_id.
 * @see webrtc_get_display_visible()
 */
int webrtc_set_display_visible(webrtc_h webrtc, unsigned int track_id, bool visible);

/**
 * @brief Gets the display visibleness of the video track.
 * @since_tizen 6.5
 * @remarks The default value is @c true.\n
 *          The display feature(%http://tizen.org/feature/display) is required. (Since 8.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[out] visible    The display visibleness
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre For remote video track, webrtc_set_display() must be called with @a track_id from webrtc_track_added_cb().\n
 *      For loopback video track, webrtc_media_source_set_video_loopback() must be called to get @a track_id.
 * @see webrtc_set_display_visible()
 */
int webrtc_get_display_visible(webrtc_h webrtc, unsigned int track_id, bool *visible);

/**
 * @brief Sets an encoded audio frame callback function to be invoked when each audio frame is ready to be rendered.
 * @since_tizen 6.5
 * @remarks If @a callback is set, audio data from the remote peer will be forwarded to @a callback without being rendered by itself.\n
 *          The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE or #WEBRTC_STATE_NEGOTIATING.
 * @post webrtc_encoded_frame_cb() will be invoked.
 * @see webrtc_unset_encoded_audio_frame_cb()
 * @see webrtc_encoded_frame_cb()
 */
int webrtc_set_encoded_audio_frame_cb(webrtc_h webrtc, webrtc_encoded_frame_cb callback, void *user_data);

/**
 * @brief Unsets the encoded audio frame callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE or #WEBRTC_STATE_NEGOTIATING.
 * @see webrtc_set_encoded_audio_frame_cb()
 */
int webrtc_unset_encoded_audio_frame_cb(webrtc_h webrtc);

/**
 * @brief Sets an encoded video frame callback function to be invoked when each video frame is ready to be rendered.
 * @since_tizen 6.5
 * @remarks If @a callback is set, video data from the remote peer will be forwarded to @a callback without being rendered by itself.\n
 *          The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE or #WEBRTC_STATE_NEGOTIATING.
 * @post webrtc_encoded_frame_cb() will be invoked.
 * @see webrtc_unset_encoded_video_frame_cb()
 * @see webrtc_encoded_frame_cb()
 */
int webrtc_set_encoded_video_frame_cb(webrtc_h webrtc, webrtc_encoded_frame_cb callback, void *user_data);

/**
 * @brief Unsets the encoded video frame callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE or #WEBRTC_STATE_NEGOTIATING.
 * @see webrtc_set_encoded_video_frame_cb()
 */
int webrtc_unset_encoded_video_frame_cb(webrtc_h webrtc);

/**
 * @brief Sets an audio loopback to render the audio frames of the media source.
 * @details The following media source types are available for this function:\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_AUDIOTEST\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_MIC\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_FILE
 * @since_tizen 6.5
 * @remarks The following sound stream types can be used to create the @a stream_info :\n
 *          #SOUND_STREAM_TYPE_MEDIA\n
 *          #SOUND_STREAM_TYPE_VOIP\n
 *          #SOUND_STREAM_TYPE_MEDIA_EXTERNAL_ONLY\n
 *          For more details, please refer to @ref CAPI_MEDIA_SOUND_MANAGER_MODULE.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The audio source id
 * @param[in] stream_info The sound stream information
 * @param[out] track_id   The track id (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_set_video_loopback()
 */
int webrtc_media_source_set_audio_loopback(webrtc_h webrtc, unsigned int source_id, sound_stream_info_h stream_info, unsigned int *track_id);

/**
 * @brief Unsets the audio loopback.
 * @since_tizen 7.0
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The audio source id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_set_audio_loopback()
 */
int webrtc_media_source_unset_audio_loopback(webrtc_h webrtc, unsigned int source_id);

/**
 * @brief Sets a video loopback to render the video frames of the media source.
 * @details The following media source types are available for this function:\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_CAMERA\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_FILE
 * @since_tizen 6.5
 * @remarks The display feature(%http://tizen.org/feature/display) is required. (Since 8.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The video source id
 * @param[in] type        The display type
 * @param[in] display     The display handle
 * @param[out] track_id   The track id (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_set_audio_loopback()
 * @see webrtc_set_display_mode()
 * @see webrtc_get_display_mode()
 * @see webrtc_set_display_visible()
 * @see webrtc_get_display_visible()
 */
int webrtc_media_source_set_video_loopback(webrtc_h webrtc, unsigned int source_id, webrtc_display_type_e type, webrtc_display_h display, unsigned int *track_id);

/**
 * @brief Unsets the video loopback.
 * @since_tizen 7.0
 * @remarks The display feature(%http://tizen.org/feature/display) is required. (Since 8.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The video source id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_set_video_loopback()
 */
int webrtc_media_source_unset_video_loopback(webrtc_h webrtc, unsigned int source_id);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MODULE
* @{
*/

/**
 * @brief Sets a STUN server URL.
 * @since_tizen 6.5
 * @remarks Regarding STUN, refer to the RFC7064(https://tools.ietf.org/html/rfc7064).\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc       WebRTC handle
 * @param[in] stun_server  The STUN server URL of the form stun://host:port
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 */
int webrtc_set_stun_server(webrtc_h webrtc, const char *stun_server);

/**
 * @brief Gets the STUN server URL.
 * @since_tizen 6.5
 * @remarks The @a stun_server should be released using free().
 * @param[in] webrtc       WebRTC handle
 * @param[out] stun_server  The STUN server URL
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 */
int webrtc_get_stun_server(webrtc_h webrtc, char **stun_server);

/**
 * @brief Adds a TURN server URL.
 * @since_tizen 6.5
 * @remarks Regarding TURN, refer to the RFC7065(https://tools.ietf.org/html/rfc7065).\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc       WebRTC handle
 * @param[in] turn_server  The TURN server URL of the form turn(s)://username:password@@host:port
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_foreach_turn_server()
 */
int webrtc_add_turn_server(webrtc_h webrtc, const char *turn_server);

/**
 * @brief Retrieves all the TURN server URLs.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_turn_server_cb()
 * @see webrtc_add_turn_server()
 */
int webrtc_foreach_turn_server(webrtc_h webrtc, webrtc_turn_server_cb callback, void *user_data);

/**
 * @brief Sets a bundle policy.
 * @since_tizen 7.0
 * @remarks #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc       WebRTC handle
 * @param[in] policy       The bundle policy
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_get_bundle_policy()
 */
int webrtc_set_bundle_policy(webrtc_h webrtc, webrtc_bundle_policy_e policy);

/**
 * @brief Gets the bundle policy.
 * @since_tizen 7.0
 * @remarks The default value is #WEBRTC_BUNDLE_POLICY_MAX_BUNDLE.
 * @param[in] webrtc       WebRTC handle
 * @param[out] policy      The bundle policy
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_set_bundle_policy()
 */
int webrtc_get_bundle_policy(webrtc_h webrtc, webrtc_bundle_policy_e *policy);

/**
 * @brief Sets a ICE transport policy that represents which candidates the ICE Agent is allowed to use.
 * @since_tizen 6.5
 * @remarks #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc       WebRTC handle
 * @param[in] policy       The ICE transport policy
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_get_ice_transport_policy()
 */
int webrtc_set_ice_transport_policy(webrtc_h webrtc, webrtc_ice_transport_policy_e policy);

/**
 * @brief Gets the ICE transport policy.
 * @since_tizen 6.5
 * @remarks The default value is #WEBRTC_ICE_TRANSPORT_POLICY_ALL.
 * @param[in] webrtc       WebRTC handle
 * @param[out] policy      The ICE transport policy
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_set_ice_transport_policy()
 */
int webrtc_get_ice_transport_policy(webrtc_h webrtc, webrtc_ice_transport_policy_e *policy);

/**
 * @brief Sets a callback function to be invoked when the WebRTC peer connection state is changed.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in the main thread.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_peer_connection_state_change_cb() will be invoked.
 * @see webrtc_unset_peer_connection_state_change_cb()
 * @see webrtc_peer_connection_state_change_cb()
 * @see webrtc_get_peer_connection_state()
 */
int webrtc_set_peer_connection_state_change_cb(webrtc_h webrtc, webrtc_peer_connection_state_change_cb callback, void *user_data);

/**
 * @brief Unsets the peer connection state change callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @see webrtc_set_peer_connection_state_change_cb()
 * @see webrtc_get_peer_connection_state()
 */
int webrtc_unset_peer_connection_state_change_cb(webrtc_h webrtc);

/**
 * @brief Gets the peer connection state.
 * @since_tizen 6.5
 * @remarks The default value is #WEBRTC_PEER_CONNECTION_STATE_NEW.
 * @param[in] webrtc      WebRTC handle
 * @param[out] state      The peer connection state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_set_peer_connection_state_change_cb()
 * @see webrtc_unset_peer_connection_state_change_cb()
 */
int webrtc_get_peer_connection_state(webrtc_h webrtc, webrtc_peer_connection_state_e *state);

/**
 * @brief Sets a callback function to be invoked when the WebRTC signaling state is changed.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in the main thread.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_signaling_state_change_cb() will be invoked.
 * @see webrtc_unset_signaling_state_change_cb()
 * @see webrtc_signaling_state_change_cb()
 * @see webrtc_get_signaling_state()
 */
int webrtc_set_signaling_state_change_cb(webrtc_h webrtc, webrtc_signaling_state_change_cb callback, void *user_data);

/**
 * @brief Unsets the signaling state change callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @see webrtc_set_signaling_state_change_cb()
 * @see webrtc_get_signaling_state()
 */
int webrtc_unset_signaling_state_change_cb(webrtc_h webrtc);

/**
 * @brief Gets the signaling state.
 * @since_tizen 6.5
 * @remarks The default value is #WEBRTC_SIGNALING_STATE_STABLE.
 * @param[in] webrtc      WebRTC handle
 * @param[out] state      The signaling state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_set_signaling_state_change_cb()
 * @see webrtc_unset_signaling_state_change_cb()
 */
int webrtc_get_signaling_state(webrtc_h webrtc, webrtc_signaling_state_e *state);

/**
 * @brief Sets a callback function to be invoked when the WebRTC ICE gathering state is changed.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in the main thread.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_ice_gathering_state_change_cb() will be invoked.
 * @see webrtc_unset_ice_gathering_state_change_cb()
 * @see webrtc_ice_gathering_state_change_cb()
 * @see webrtc_get_ice_gathering_state()
 */
int webrtc_set_ice_gathering_state_change_cb(webrtc_h webrtc, webrtc_ice_gathering_state_change_cb callback, void *user_data);

/**
 * @brief Unsets the ICE gathering state change callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @see webrtc_set_ice_gathering_state_change_cb()
 * @see webrtc_get_ice_gathering_state()
 */
int webrtc_unset_ice_gathering_state_change_cb(webrtc_h webrtc);

/**
 * @brief Gets the ICE gathering state.
 * @since_tizen 6.5
 * @remarks The default value is #WEBRTC_ICE_GATHERING_STATE_NEW.
 * @param[in] webrtc      WebRTC handle
 * @param[out] state      The ICE gathering state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_set_ice_gathering_state_change_cb()
 * @see webrtc_unset_ice_gathering_state_change_cb()
 */
int webrtc_get_ice_gathering_state(webrtc_h webrtc, webrtc_ice_gathering_state_e *state);

/**
 * @brief Sets a callback function to be invoked when the WebRTC ICE connection state is changed.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in the main thread.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_ice_connection_state_change_cb() will be invoked.
 * @see webrtc_unset_ice_connection_state_change_cb()
 * @see webrtc_ice_connection_state_change_cb()
 * @see webrtc_get_ice_connection_state()
 */
int webrtc_set_ice_connection_state_change_cb(webrtc_h webrtc, webrtc_ice_connection_state_change_cb callback, void *user_data);

/**
 * @brief Unsets the ICE connection state change callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @see webrtc_set_ice_connection_state_change_cb()
 * @see webrtc_get_ice_connection_state()
 */
int webrtc_unset_ice_connection_state_change_cb(webrtc_h webrtc);

/**
 * @brief Gets the ICE connection state.
 * @since_tizen 6.5
 * @remarks The default value is #WEBRTC_ICE_CONNECTION_STATE_NEW.
 * @param[in] webrtc      WebRTC handle
 * @param[out] state      The ICE connection state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_set_ice_connection_state_change_cb()
 * @see webrtc_unset_ice_connection_state_change_cb()
 */
int webrtc_get_ice_connection_state(webrtc_h webrtc, webrtc_ice_connection_state_e *state);

/**
 * @brief Sets a negotiation needed callback function to be invoked when a change has occurred which requires session negotiation.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_negotiation_needed_cb() will be invoked.
 * @see webrtc_unset_negotiation_needed_cb()
 * @see webrtc_negotiation_needed_cb()
 */
int webrtc_set_negotiation_needed_cb(webrtc_h webrtc, webrtc_negotiation_needed_cb callback, void *user_data);

/**
 * @brief Unsets the negotiation needed callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @see webrtc_set_negotiation_needed_cb()
 */
int webrtc_unset_negotiation_needed_cb(webrtc_h webrtc);

/**
 * @brief Sets an ICE candidate callback function to be invoked when the WebRTC needs to send the ICE candidate message to the remote peer.
 * @since_tizen 6.5
 * @remarks If all the ICE candidates have been gathered, ICE gathering state will be changed to #WEBRTC_ICE_GATHERING_STATE_COMPLETE.\n
 *          The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_ice_candidate_cb() will be invoked.
 * @see webrtc_unset_ice_candidate_cb()
 * @see webrtc_ice_candidate_cb()
 * @see webrtc_set_ice_gathering_state_change_cb()
 * @see webrtc_get_ice_gathering_state()
 */
int webrtc_set_ice_candidate_cb(webrtc_h webrtc, webrtc_ice_candidate_cb callback, void *user_data);

/**
 * @brief Unsets the ice candidate callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @see webrtc_set_ice_candidate_cb()
 */
int webrtc_unset_ice_candidate_cb(webrtc_h webrtc);

/**
 * @brief Creates SDP offer to start a new WebRTC connection to a remote peer.
 * @since_tizen 6.5
 * @remarks The @a offer should be released using free().\n
 *          The @a options currently has no effect.\n
 *          This function can be called in the state of #WEBRTC_STATE_PLAYING. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] options     Configuration options for the offer (optional, this can be NULL)
 * @param[out] offer      SDP offer
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @post @a offer must be set as a local description by calling webrtc_set_local_description()
 * @post @a offer must be sent to the remote peer via the signaling channel.
 * @see webrtc_state_changed_cb()
 * @see webrtc_negotiation_needed_cb()
 * @see webrtc_set_local_description()
 */
int webrtc_create_offer(webrtc_h webrtc, bundle *options, char **offer);

/**
 * @brief Creates SDP answer to an offer received from a remote peer during the negotiation of a WebRTC connection.
 * @since_tizen 6.5
 * @remarks The @a answer should be released using free().\n
 *          The @a options currently has no effect.\n
 *          This function can be called in the state of #WEBRTC_STATE_PLAYING. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] options     Configuration options for the answer (optional, this can be NULL)
 * @param[out] answer     SDP answer
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @pre The remote SDP offer must be set by calling webrtc_set_remote_description().
 * @pre The signaling state must be set to #WEBRTC_SIGNALING_STATE_HAVE_REMOTE_OFFER.
 * @post @a answer must be set as a local description by calling webrtc_set_local_description()
 * @post @a answer must be sent to the remote peer via the signaling channel.
 * @see webrtc_set_remote_description()
 * @see webrtc_set_local_description()
 * @see webrtc_get_signaling_state()
 * @see webrtc_set_signaling_state_change_cb()
 */
int webrtc_create_answer(webrtc_h webrtc, bundle *options, char **answer);

/**
 * @brief Creates SDP offer asynchronously to start a new WebRTC connection to a remote peer.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in the main thread.\n
 *          The @a options currently has no effect.\n
 *          This function can be called in the state of #WEBRTC_STATE_PLAYING. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] options     Configuration options for the offer (optional, this can be NULL)
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @post webrtc_session_description_created_cb() will be invoked.
 * @see webrtc_state_changed_cb()
 * @see webrtc_negotiation_needed_cb()
 * @see webrtc_set_local_description()
 * @see webrtc_session_description_created_cb()
 */
int webrtc_create_offer_async(webrtc_h webrtc, bundle *options, webrtc_session_description_created_cb callback, void *user_data);

/**
 * @brief Creates SDP answer asynchronously to an offer received from a remote peer during the negotiation of a WebRTC connection.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in the main thread.\n
 *          The @a options currently has no effect.\n
 *          This function can be called in the state of #WEBRTC_STATE_PLAYING. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] options     Configuration options for the answer (optional, this can be NULL)
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @pre The remote SDP offer must be set by calling webrtc_set_remote_description().
 * @pre The signaling state must be set to #WEBRTC_SIGNALING_STATE_HAVE_REMOTE_OFFER.
 * @post webrtc_session_description_created_cb() will be invoked.
 * @see webrtc_set_remote_description()
 * @see webrtc_set_local_description()
 * @see webrtc_get_signaling_state()
 * @see webrtc_set_signaling_state_change_cb()
 * @see webrtc_session_description_created_cb()
 */
int webrtc_create_answer_async(webrtc_h webrtc, bundle *options, webrtc_session_description_created_cb callback, void *user_data);

/**
 * @brief Sets the session description for a local peer associated with a WebRTC connection.
 * @since_tizen 6.5
 * @remarks @a description is a JSON string.\n
 *          It will be {"sdp":{"type":"offer or answer","sdp":"..."}}.\n
 *          This function can be called in the state of #WEBRTC_STATE_PLAYING. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] description The local session description
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @see webrtc_create_offer()
 * @see webrtc_create_offer_async()
 * @see webrtc_create_answer()
 * @see webrtc_create_answer_async()
 * @see webrtc_set_signaling_state_change_cb()
 * @see webrtc_get_signaling_state()
 * @see webrtc_get_local_description()
 */
int webrtc_set_local_description(webrtc_h webrtc, const char *description);

/**
 * @brief Gets the local session description.
 * @since_tizen 9.0
 * @remarks @a description is a JSON string.\n
 *          It will be {"sdp":{"type":"offer or answer","sdp":"..."}}.\n
 *          The @a description should be released using free() if the value is not NULL.
 * @param[in] webrtc       WebRTC handle
 * @param[out] description The local session description
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_set_local_description()
 * @par Example
 * @code
   #include <webrtc.h>
   ...
   webrtc_h webrtc;
   char *offer_desc = NULL;
   char *local_desc = NULL;
   ...
   webrtc_create(&webrtc);
   // after setting media and starting handle
   ...
   webrtc_create_offer(webrtc, NULL, &offer_desc);
   webrtc_set_local_description(webrtc, offer_desc);
   // process negotiation via signaling server
   ...
   // get local description for some reason
   webrtc_get_local_description(webrtc, &local_desc);
   ...
   // after use, free it
   if (local_desc)
       free(local_desc);
   ...
 * @endcode
 */
int webrtc_get_local_description(webrtc_h webrtc, char **description);

/**
 * @brief Sets the session description of the remote peer's current offer or answer.
 * @since_tizen 6.5
 * @remarks @a description is a JSON string.\n
 *          It should be {"sdp":{"type":"offer or answer","sdp":"..."}}.
 *          This function can be called in the state of #WEBRTC_STATE_PLAYING. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] description The remote session description
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @see webrtc_state_changed_cb()
 * @see webrtc_set_signaling_state_change_cb()
 * @see webrtc_get_signaling_state()
 * @see webrtc_get_remote_description()
 */
int webrtc_set_remote_description(webrtc_h webrtc, const char *description);

/**
 * @brief Gets the remote session description.
 * @since_tizen 9.0
 * @remarks @a description is a JSON string.\n
 *          It should be {"sdp":{"type":"offer or answer","sdp":"..."}}.\n
 *          The @a description should be released using free() if the value is not NULL.
 * @param[in] webrtc       WebRTC handle
 * @param[out] description The remote session description
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_set_remote_description()
 * @par Example
 * @code
   #include <webrtc.h>
   ...
   webrtc_h webrtc;
   char *remote_desc = NULL;
   ...
   webrtc_create(&webrtc);
   // after setting media and starting handle
   ...
   // process negotiation via signaling server
   // get remote (offer or answer) description from signaling server
   remote_desc = __get_remote_desc();
   webrtc_set_remote_description(webrtc, remote_desc);
   ...
   // get remote description for some reason
   webrtc_get_remote_description(webrtc, &remote_desc);
   ...
   // after use, free it
   if (remote_desc)
       free(remote_desc);
   ...
 * @endcode
 */
int webrtc_get_remote_description(webrtc_h webrtc, char **description);

/**
 * @brief Adds a new ICE candidate from the remote peer over its signaling channel.
 * @since_tizen 6.5
 * @remarks @a candidate is a JSON string.\n
 *          It should be {"ice":{"candidate":"...", "sdpMLineIndex":0 or 1 or ...}}.
 * @param[in] webrtc      WebRTC handle
 * @param[in] candidate   The ICE candidate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING.
 */
int webrtc_add_ice_candidate(webrtc_h webrtc, const char *candidate);

/**
 * @brief Sets a track added callback function to be invoked when a new track has been added to the WebRTC.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE or #WEBRTC_STATE_NEGOTIATING.
 * @post webrtc_track_added_cb() will be invoked.
 * @see webrtc_unset_track_added_cb()
 * @see webrtc_track_added_cb()
 */
int webrtc_set_track_added_cb(webrtc_h webrtc, webrtc_track_added_cb callback, void *user_data);

/**
 * @brief Unsets the track added callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_set_track_added_cb()
 */
int webrtc_unset_track_added_cb(webrtc_h webrtc);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_DATA_CHANNEL_MODULE
* @{
*/

/**
 * @brief Sets a data channel callback function to be invoked when the data channel is created to the connection by the remote peer.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] webrtc      WebRTC handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @post webrtc_data_channel_cb() will be invoked.
 * @see webrtc_unset_data_channel_cb()
 * @see webrtc_data_channel_cb()
 */
int webrtc_set_data_channel_cb(webrtc_h webrtc, webrtc_data_channel_cb callback, void *user_data);

/**
 * @brief Unsets the data channel callback function.
 * @since_tizen 6.5
 * @param[in] webrtc      WebRTC handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_IDLE.
 * @see webrtc_set_data_channel_cb()
 */
int webrtc_unset_data_channel_cb(webrtc_h webrtc);

/**
 * @brief Creates a new data channel which is linked with the remote peer.
 * @since_tizen 6.5
 * @remarks The @a channel should be released using webrtc_destroy().\n
 *          The @a options dictionary is similar format as the RTCDataChannelInit members outlined https://www.w3.org/TR/webrtc/#dom-rtcdatachannelinit.\n
 *          The following attributes can be set to @a options by using #bundle API:\n
 *          'ordered' of type bool            : Whether the channel will send data with guaranteed ordering. The default value is @c true.\n
 *          'max-packet-lifetime' of type int : The time in milliseconds to attempt transmitting unacknowledged data. -1 for unset. The default value is -1.\n
 *          'max-retransmits' of type int     : The number of times data will be attempted to be transmitted without acknowledgement before dropping. The default value is -1.\n
 *          'protocol' of type string         : The subprotocol used by this channel. The default value is NULL.\n
 *          'id' of type int                  : Override the default identifier selection of this channel. The default value is -1.\n
 *          'priority' of type int            : The priority to use for this channel(1:very low, 2:low, 3:medium, 4:high). The default value is 2.\n
 *          #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] label       Name for the channel
 * @param[in] options     Configuration options for creating the data channel (optional, this can be NULL)
 * @param[out] channel    Data channel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_destroy_data_channel()
 */
int webrtc_create_data_channel(webrtc_h webrtc, const char *label, bundle *options, webrtc_data_channel_h *channel);

/**
 * @brief Destroys the data channel.
 * @since_tizen 6.5
 * @param[in] channel    Data channel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_create_data_channel()
 */
int webrtc_destroy_data_channel(webrtc_data_channel_h channel);

/**
 * @brief Sets a data channel open callback function to be invoked when the data channel's underlying data transport is established.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] channel     Data channel handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @post webrtc_data_channel_open_cb() will be invoked.
 * @see webrtc_data_channel_unset_open_cb()
 * @see webrtc_data_channel_open_cb()
 */
int webrtc_data_channel_set_open_cb(webrtc_data_channel_h channel, webrtc_data_channel_open_cb callback, void *user_data);

/**
 * @brief Unsets the data channel open callback function.
 * @since_tizen 6.5
 * @param[in] channel     Data channel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_data_channel_set_open_cb()
 */
int webrtc_data_channel_unset_open_cb(webrtc_data_channel_h channel);

/**
 * @brief Sets a data channel message callback function to be invoked when a message is received from the remote peer.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] channel     Data channel handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @post webrtc_data_channel_message_cb() will be invoked.
 * @see webrtc_data_channel_unset_message_cb()
 * @see webrtc_data_channel_message_cb()
 */
int webrtc_data_channel_set_message_cb(webrtc_data_channel_h channel, webrtc_data_channel_message_cb callback, void *user_data);

/**
 * @brief Unsets the data channel message callback function.
 * @since_tizen 6.5
 * @param[in] channel     Data channel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_data_channel_set_message_cb()
 */
int webrtc_data_channel_unset_message_cb(webrtc_data_channel_h channel);

/**
 * @brief Sets a data channel error callback function to be invoked when an error occurs on the data channel.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] channel     Data channel handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @post webrtc_data_channel_error_cb() will be invoked.
 * @see webrtc_data_channel_unset_error_cb()
 * @see webrtc_data_channel_error_cb()
 */
int webrtc_data_channel_set_error_cb(webrtc_data_channel_h channel, webrtc_data_channel_error_cb callback, void *user_data);

/**
 * @brief Unsets the data channel error callback function.
 * @since_tizen 6.5
 * @param[in] channel     Data channel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_data_channel_set_error_cb()
 */
int webrtc_data_channel_unset_error_cb(webrtc_data_channel_h channel);

/**
 * @brief Sets a data channel close callback function to be invoked when the data channel has closed down.
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 * @param[in] channel     Data channel handle
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @post webrtc_data_channel_close_cb() will be invoked.
 * @see webrtc_data_channel_unset_close_cb()
 * @see webrtc_data_channel_close_cb()
 */
int webrtc_data_channel_set_close_cb(webrtc_data_channel_h channel, webrtc_data_channel_close_cb callback, void *user_data);

/**
 * @brief Unsets the data channel close callback function.
 * @since_tizen 6.5
 * @param[in] channel     Data channel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_data_channel_set_close_cb()
 */
int webrtc_data_channel_unset_close_cb(webrtc_data_channel_h channel);

/**
 * @brief Sends a string data across the data channel to the remote peer.
 * @since_tizen 6.5
 * @param[in] channel     Data channel handle
 * @param[in] string      String data to send
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a channel should be opened before calling this function.
 * @post webrtc_data_channel_error_cb() will be invoked if this function fails.
 * @see webrtc_data_channel_open_cb()
 * @see webrtc_data_channel_send_bytes()
 */
int webrtc_data_channel_send_string(webrtc_data_channel_h channel, const char *string);

/**
 * @brief Sends byte data across the data channel to the remote peer.
 * @since_tizen 6.5
 * @param[in] channel     Data channel handle
 * @param[in] data        Byte data to send
 * @param[in] size        Size of the data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a channel should be opened before calling this function.
 * @post webrtc_data_channel_error_cb() will be invoked if this function fails.
 * @see webrtc_data_channel_open_cb()
 * @see webrtc_data_channel_send_string()
 */
int webrtc_data_channel_send_bytes(webrtc_data_channel_h channel, const char *data, unsigned int size);

/**
 * @brief Gets the channel label.
 * @since_tizen 6.5
 * @remarks The @a label should be released using free().
 * @param[in] channel     Data channel handle
 * @param[out] label      The channel label
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_create_data_channel()
 */
int webrtc_data_channel_get_label(webrtc_data_channel_h channel, char **label);

/**
 * @brief Gets data pointer and its size.
 * @since_tizen 6.5
 * @remarks This function must be called inside of the webrtc_data_channel_message_cb().\n
 *          @a bytes and @a data are managed by the platform and will be released when after the webrtc_data_channel_message_cb() is ended.
 * @param[in] bytes       Bytes data handle
 * @param[out] data       Data pointer
 * @param[out] size       Size of the data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_data_channel_message_cb()
 */
int webrtc_get_data(webrtc_bytes_data_h bytes, const char **data, unsigned long *size);

/**
 * @brief Gets the number of bytes of data currently queued to be sent over the data channel.
 * @since_tizen 7.0
 * @param[in] channel             Data channel handle
 * @param[out] buffered_amount    The number of bytes of data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_create_data_channel()
 */
int webrtc_data_channel_get_buffered_amount(webrtc_data_channel_h channel, unsigned int *buffered_amount);

/**
 * @brief Sets the threshold at which the buffered amount is considered low and callback function.
 * @since_tizen 7.0
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.\n
            Callback function will be invoked when the number of bytes currently queued falls below the @a threshold.
 * @param[in] channel     Data channel handle
 * @param[in] threshold   The threshold
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @post webrtc_data_channel_buffered_amount_low_cb() will be invoked.
 * @see webrtc_data_channel_buffered_amount_low_cb()
 * @see webrtc_data_channel_get_buffered_amount_low_threshold()
 * @see webrtc_data_channel_unset_buffered_amount_low_cb()
 */
int webrtc_data_channel_set_buffered_amount_low_cb(webrtc_data_channel_h channel, unsigned int threshold, webrtc_data_channel_buffered_amount_low_cb callback, void *user_data);

/**
 * @brief Gets the threshold at which the buffered amount is considered low.
 * @since_tizen 7.0
 * @remarks The default value is 0.
 * @param[in] channel     Data channel handle
 * @param[out] threshold  The threshold
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_data_channel_set_buffered_amount_low_cb()
 */
int webrtc_data_channel_get_buffered_amount_low_threshold(webrtc_data_channel_h channel, unsigned int *threshold);

/**
 * @brief Unsets the data channel buffered amount low callback function.
 * @since_tizen 7.0
 * @param[in] channel     Data channel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_data_channel_set_buffered_amount_low_cb()
 */
int webrtc_data_channel_unset_buffered_amount_low_cb(webrtc_data_channel_h channel);

/**
 * @}
 */

/**
* @addtogroup CAPI_MEDIA_WEBRTC_STATISTICS_MODULE
* @{
*/

/**
 * @brief Retrieves all the statistics properties.
 * @since_tizen 7.0
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.
 *          This function can be called in the state of #WEBRTC_STATE_NEGOTIATING. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] type_mask   The mask values of #webrtc_stats_type_e combined with bitwise 'or'
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre @a webrtc state must be set to #WEBRTC_STATE_NEGOTIATING or #WEBRTC_STATE_PLAYING.
 * @post webrtc_stats_cb() will be invoked.
 * @par Example
 * @code
	if (webrtc_foreach_stats(webrtc, WEBRTC_STATS_TYPE_ALL, __stats_cb, user_data) != WEBRTC_ERROR_NONE)
		return error;
	... or ...
	if (webrtc_foreach_stats(webrtc, WEBRTC_STATS_TYPE_INBOUND_RTP | WEBRTC_STATS_TYPE_REMOTE_OUTBOUND_RTP,
		__stats_cb, user_data) != WEBRTC_ERROR_NONE)
		return error;
 * @endcode
 */
int webrtc_foreach_stats(webrtc_h webrtc, int type_mask, webrtc_stats_cb callback, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_WEBRTC_H__ */
