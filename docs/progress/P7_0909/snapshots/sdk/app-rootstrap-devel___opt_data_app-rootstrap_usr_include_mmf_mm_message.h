/*
 * libmm-common
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jonghyuk Choi <jhchoi.choi@samsung.com>
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
 *
 */


#ifndef __MM_MESSAGE_H__
#define	__MM_MESSAGE_H__

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif


/**
 *         @addtogroup COMMON
 *         @{
		 * @file		mm_message.h
		 * @brief		This file declares the messages type of Multimedia framework.
		 * @version		1.0
		 *
		 * This file declares the messages type of Multimedia framework.
 */

/**
 * Enumerations of messages type.
*/
enum MMMessageType {
	/* COMMON */
	MM_MESSAGE_UNKNOWN = 0x00,         /**< Unknown message type */
	MM_MESSAGE_ERROR,                  /**< Error message type */
	MM_MESSAGE_WARNING,                /**< Warning message type */
	MM_MESSAGE_STATE_CHANGED,          /**< State change message type */
	MM_MESSAGE_STATE_INTERRUPTED,      /**< State change by interrupt */
	MM_MESSAGE_READY_TO_RESUME,        /**< Ready to resume message type */

	/* PLAYER */
	MM_MESSAGE_CONNECTING = 0x100,		/**< Connecting message type */
	MM_MESSAGE_CONNECTED,				/**< Rtspsrc has succeeded to connecting to server */
	MM_MESSAGE_CONNECTION_TIMEOUT,		/**< Connection timeout message type */
	MM_MESSAGE_BUFFERING,				/**< Buffering message type */
	MM_MESSAGE_BEGIN_OF_STREAM,			/**< Streaming begin message type */
	MM_MESSAGE_END_OF_STREAM,			/**< End of stream message type */
	MM_MESSAGE_RESUMED_BY_REW,			/**< Resumed by fast rewind message type */
	MM_MESSAGE_UPDATE_SUBTITLE,			/**< Update subtitle type */
	MM_MESSAGE_RTP_SENDER_REPORT,		/**< RTP sender report */
	MM_MESSAGE_RTP_RECEIVER_REPORT,		/**< RTP receiver report */
	MM_MESSAGE_RTP_SESSION_STATUS,		/**< RTP session status */
	MM_MESSAGE_FILE_NOT_SUPPORTED,		/**< Not supported file */
	MM_MESSAGE_FILE_NOT_FOUND,			/**< Not founded file */
	MM_MESSAGE_DRM_NOT_AUTHORIZED,		/**< Not authorized to play the content */
	MM_MESSAGE_VIDEO_CAPTURED,			/**< Video frame has been captured */
	MM_MESSAGE_VIDEO_NOT_CAPTURED,		/**< video capture failed */
	MM_MESSAGE_SEEK_COMPLETED,			/**< Seek completed */
	MM_MESSAGE_IMAGE_BUFFER,			/**< hls image buffer message type */
	MM_MESSAGE_DRM_NO_LICENSE,			/**< No license */
	MM_MESSAGE_DRM_EXPIRED,				/**< Expired license */
	MM_MESSAGE_DRM_FUTURE_USE,			/**< License for future use */
	MM_MESSAGE_VIDEO_BIN_CREATED,		/**< Video bin is created */
	MM_MESSAGE_FLUSH_BUFFER,			/**< Flush exported buffer */
	MM_MESSAGE_PLAY_POSITION,			/**< Play position */
	MM_MESSAGE_VIDEO_STREAM_CHANGED,	/**< Video stream is changed */
	MM_MESSAGE_PUSH_BUFFER_STATUS,		/**< Push buffer status message type */
	MM_MESSAGE_PUSH_BUFFER_SEEK_DATA, 	/**< Push buffer seek data message type */
	MM_MESSAGE_INTERRUPT_STARTED,		/**< Interrupt started */

	/* CAMCORDER */
	MM_MESSAGE_CAMCORDER_STATE_CHANGED = 0x200,	/**< State changed.*/
	MM_MESSAGE_CAMCORDER_STATE_CHANGED_BY_ASM,	/**< State change by audio session manager message type */
	MM_MESSAGE_CAMCORDER_MAX_SIZE,			/**< Maximum size, camcorder waits for user's order (cam_commit/cancel).*/
	MM_MESSAGE_CAMCORDER_NO_FREE_SPACE,		/**< No free space, camcorder waits for user's order (cam_commit/cancel).*/
	MM_MESSAGE_CAMCORDER_TIME_LIMIT,		/**< Time limit, camcorder waits for user's order (cam_commit/cancel)*/
	MM_MESSAGE_CAMCORDER_CAPTURED,			/**< Image captured */
	MM_MESSAGE_CAMCORDER_ERROR,			/**< Error occurred.*/
	MM_MESSAGE_CAMCORDER_FOCUS_CHANGED,		/**< Focus changed */
	MM_MESSAGE_CAMCORDER_CURRENT_VOLUME,		/**< Current volume level of real input stream */
	MM_MESSAGE_CAMCORDER_FIRMWARE_UPDATE,		/**< Firmware update */
	MM_MESSAGE_CAMCORDER_RECORDING_STATUS,		/**< Recording status. (Elapsed time, total time, file size(KB))*/
	MM_MESSAGE_CAMCORDER_STATE_CHANGED_BY_SECURITY, /**< State change by system security(ex:MDM) message type */
	MM_MESSAGE_CAMCORDER_VIDEO_SNAPSHOT_CAPTURED,	/**< Image captured while video recording */
	MM_MESSAGE_CAMCORDER_HDR_PROGRESS,		/**< Progressing percent of HDR capture */
	MM_MESSAGE_CAMCORDER_FACE_DETECT_INFO,		/**< Face detection information */
	MM_MESSAGE_CAMCORDER_VIDEO_CAPTURED,		/**< Video captured */
	MM_MESSAGE_CAMCORDER_AUDIO_CAPTURED,		/**< Audio captured */
	MM_MESSAGE_CAMCORDER_LOW_LIGHT_STATE,		/**< Low light state */
	MM_MESSAGE_CAMCORDER_CAPTURE_SOUND,		/**< Noti to play capture sound - only single capture available */
	MM_MESSAGE_CAMCORDER_CAPTURE_SOUND_COMPLETED,	/**< Noti to play capture sound completed - only single capture available */
	MM_MESSAGE_CAMCORDER_STATE_CHANGED_BY_RM,	/**< State change by resource manager.*/
	MM_MESSAGE_CAMCORDER_STATE_CHANGE_STARTED_BY_ASM,		/**< State change is started by sound policy */
	MM_MESSAGE_CAMCORDER_STATE_CHANGE_STARTED_BY_SECURITY,	/**< State change is started by system security */
	MM_MESSAGE_CAMCORDER_STATE_CHANGE_STARTED_BY_RM,	/**< State change is started by resource manager */

	/* RADIO */
	MM_MESSAGE_RADIO_SCAN_START = 0x300,		/**< Radio frequency scanning initiated */
	MM_MESSAGE_RADIO_SCAN_INFO,			/**< Founded radio frequency report. check message parameters  */
	MM_MESSAGE_RADIO_SCAN_FINISH,			/**< Radio frequency scanning has finished */
	MM_MESSAGE_RADIO_SCAN_STOP,			/**< Radio frequency scanning has stopped */
	MM_MESSAGE_RADIO_SEEK_START,			/**< Radio seeking has established */
	MM_MESSAGE_RADIO_SEEK_FINISH,			/**< Radio seeking has finished */
	MM_MESSAGE_RADIO_SET_FREQUENCY,			/**< Radio set frequency async*/
	MM_MESSAGE_RADIO_RDS_PS,			/**< Radio RDS Program Service Data has arrived> */
	MM_MESSAGE_RADIO_RDS_RT,			/**< Radio RDS Radio Text Data has arrived> */


	/* MEDIA CALL */
	MM_MESSAGE_MEDIACALL_RESERVED = 0x400,		/**< Reserved message for Media Call */

	/* IMAGE */
	MM_MESSAGE_IMAGE_RESERVED = 0x500,		/**< Reserved message for Image */

	/* SOUND */
	MM_MESSAGE_SOUND_PCM_CAPTURE_RESTRICTED = 0x600,	/**< Sound pcm capture is restricted by system */
	MM_MESSAGE_SOUND_PCM_INTERRUPTED,			/**< Sound pcm is interrupted by audio system */

	/* FILE INFO */
	MM_MESSAGE_FILEINFO_RESERVED = 0x700,		/**< Reserved message for File Info */

	/* STREAM RECORDER */
	MM_MESSAGE_STREAMRECORDER_ERROR	= 0x800,
	MM_MESSAGE_STREAMRECORDER_CONSUME_COMPLETE,
	MM_MESSAGE_STREAMRECORDER_STATE_CHANGED,
	MM_MESSAGE_STREAMRECORDER_MAX_SIZE,		/**< Maximum size, streamcorder waits for user's order (commit/cancel) */
	MM_MESSAGE_STREAMRECORDER_NO_FREE_SPACE,	/**< No free space,streamcorder waits for user's order (commit/cancel) */
	MM_MESSAGE_STREAMRECORDER_TIME_LIMIT,		/**< Time limit, streamcorder waits for user's order (commit/cancel) */
	MM_MESSAGE_STREAMRECORDER_RECORDING_STATUS,	/**< Recording status. (Elapsed time, total time, file size(KB)) */
	MM_MESSAGE_STREAMRECORDER_VIDEO_CAPTURED,	/**< Video captured done. This message will be called when video capture is finished (eos) */
	MM_MESSAGE_STREAMRECORDER_AUDIO_CAPTURED,	/**< Audio captured done. This message will be called when audio capture is finished (eos) */

	MM_MESSAGE_NUM,					/**< The number of the messages */
};


/**
 * Union type of 'MMMessageParamType' structure. Please add type when you make new union type of MMMessageParamType
 */
enum MMMessageUnionType {
	MM_MSG_UNION_CODE,		/**< Default */
	MM_MSG_UNION_STATE,
	MM_MSG_UNION_TIME,
	MM_MSG_UNION_CONNECTION,
	MM_MSG_UNION_FIRMWARE,
	MM_MSG_UNION_RTPSENDER_REPORT,
	MM_MSG_UNION_RTPRECEIVER_REPORT,
	MM_MSG_UNION_RTPSESSION_STATUS,
	MM_MSG_UNION_SUBTITLE,
	MM_MSG_UNION_RADIO_SCAN,
	MM_MSG_UNION_RECORDING_STATUS,
	MM_MSG_UNION_REC_VOLUME_DB,
	MM_MSG_UNION_CONSUME_RECORDER_BUFFER,
	MM_MSG_UNION_BUFFER_STATUS,
	MM_MSG_UNION_SEEK_DATA,
};

/*
 * Enumerations of code for MM_MESSAGE_STATE_INTERRUPTED messages type.
 */
enum MMMessageInterruptedCode {
	MM_MSG_CODE_INTERRUPTED_BY_MEDIA = 0,
	MM_MSG_CODE_INTERRUPTED_BY_CALL_START,
	MM_MSG_CODE_INTERRUPTED_BY_CALL_END,
	MM_MSG_CODE_INTERRUPTED_BY_EARJACK_UNPLUG,
	MM_MSG_CODE_INTERRUPTED_BY_RESOURCE_CONFLICT,
	MM_MSG_CODE_INTERRUPTED_BY_ALARM_START,
	MM_MSG_CODE_INTERRUPTED_BY_ALARM_END,
	MM_MSG_CODE_INTERRUPTED_BY_EMERGENCY_START,
	MM_MSG_CODE_INTERRUPTED_BY_EMERGENCY_END,
	MM_MSG_CODE_INTERRUPTED_BY_OTHER_PLAYER_APP,
	MM_MSG_CODE_INTERRUPTED_BY_NOTIFICATION_START,
	MM_MSG_CODE_INTERRUPTED_BY_NOTIFICATION_END,
};

/*
 * Enumerations of code for MM_MESSAGE_SOUND_PCM_CAPTURE_RESTRICTED messages type.
 */
enum MMMessagePcmCaptureCode {
	MM_MSG_CODE_PCM_CAPTURE_RESTRICTED = 0,
	MM_MSG_CODE_PCM_CAPTURE_ALLOWED,
};

/**
 * Message callback function type.
 */
typedef	int	(*MMMessageCallback) (int id, void *param, void *user_param);

/**
 * Message Parameter.
 */
typedef struct {
	void *data;                             /**< Allocated data */

/**
 * union.
 */
	int union_type;		/**< Type of union. Please refer MMMessageUnionType */

	union {
		int code;                       /**< Error or warning code */
/**
 * state.
 */
		struct {
			int previous;           /**< Previous state */
			int current;            /**< Current state */
			int code;               /**< Error or warning code */
		} state;
/**
 * time.
 */
		struct {
			int64_t elapsed;  /**< Elapsed time */
			int64_t total;    /**< Total time */
		} time;
/**
 * connection.
 */
		struct {
			unsigned int buffering; /**< Buffering percent */
		} connection;

/**
 * firmware.
 */
		struct {
			int state;              /**< Update state*/
			int progress;           /**< Update progress percentage */
		} firmware;
/**
 * RTPSenderReport.
 */
		struct {
			unsigned char is_valid;                 /**< Structure is valid or not */
			//int64 ntptime;                        /**< The NTP timestamp indicates the point of time measured in wall clock time when this report was sent. */		  unsigned int rtptime;			/**< The RTP timestamp resembles the same time as the NTP timestamp ,but is measured in the same units and with the same random offset as the RTP timestamps in data packets */
			unsigned long long ntptime;             /**< The NTP timestamp indicates the point of time measured in wall clock time when this report was sent. */		  unsigned int rtptime;			/**< The RTP timestamp resembles the same time as the NTP timestamp ,but is measured in the same units and with the same random offset as the RTP timestamps in data packets */
			unsigned int packet_count;              /**< The sender's packet count totals up the number of RTP data packets transmitted by the sender since joining the RTP session */
			unsigned int octet_count;               /**< The total number of payload octets (i.e., not including the header or any padding) transmitted in RTP data packets by the sender since starting up transmission */
		} RTPSenderReport;      /**< RTPSenderReport message parameter structure. */
/**
 * RTPReceiverReport.
 */
		struct {
			  unsigned char is_valid;               /**< Structure is valid or not */
			  unsigned int ssrc;                    /**< The SSRC identifier of the sender whose reception is reported in this block . who the report is from */
			  unsigned char fractionlost;           /**< The sender of the receiver report estimates the fraction of the RTP data packets from source SSRC_n that it assumes to be lost since it sent the previous SR or RR packet.*/
			  unsigned int packetslost;             /**< The sender of a receiver report blocks also tries to estimate the total number of RTP data packets from source SSRC_n that have been lost since the beginning of reception. Packets that arrive late are not counted as lost, and the loss may be negative if there are duplicates. */
			  unsigned int exthighestseq;           /**< The low 16 bits of the extended highest sequence number contain the highest sequence number received in an RTP data packet from source SSRC_n, and the most significant 16 bits extend that sequence number with the corresponding count of sequence number cycles. */
			  unsigned int jitter;                  /**< An estimate of the statistical variance of the RTP data packet inter-arrival time, measured in timestamp units and expressed as an unsigned intege */
			  unsigned int lsr;                     /**< Last SR Timestamp        */
			  unsigned int dlsr;                    /**< Delay since last SR Timestamp    */
		} RTPReceiverReport;    /**< RTPReceiverReport message parameter structure. */
/**
 * RTPSessionStatus.
 */
		struct {
			double bandwidth;                       /**< Bandwidth Modifier*/
			double sender_fraction;                 /**< Fraction of packet loss by sender*/
			double receiver_fraction;               /**< Fraction of packet loss from Receiver*/
			double rtcp_bandwidth;                  /**< The total bandwidth that will be used for RTCP packets by all members of this session, in octets per second */
			double min_interval;                    /**< Minimum Interval */
			unsigned int sender_sources;            /**< Identifies the synchronization source from sender*/
			unsigned int active_sources;            /**< Identifies the Active source from sender*/
			unsigned int avg_rtcp_packet_size;      /**< Average RTCP packet size */
			unsigned int bye_members;               /**< BYE Packet members */
		} RTPSessionStatus;     /**< RTPSessionStatus message parameter structure. */
/**
 * subtitle
 */
		struct {
			unsigned long duration;         /**< duration */
		} subtitle;

/**
 * Radio Scan Info
 */
		struct {
			int frequency;                  /**< detected active frequency with MM_MESSAGE_RADIO_SCAN_INFO */
		} radio_scan;

/**
 * Radio RDS PS and RT Info
 */
		struct {
			int frequency;                /**< The frequency at which the RDS data was obtained> */
			char* rt_ps;                  /**< the radio text obtained with MM_MESSAGE_RADIO_RDS_PS and MM_MESSAGE_RADIO_RDS_RT */
		} radio_rds_text;
/**
 * Recording status
 */
		struct {
			unsigned long long elapsed;          /**< Elapsed time (ms) */
			unsigned long long total;            /**< Total time */
			unsigned long long filesize;         /**< Recording File size (KB). An approximate value. */
			unsigned long long remained_time;    /**< Remained time (ms).
													This is available if time limit is not set or
													time limit is bigger than estimated remained time. */
		} recording_status;

/**
 * Recording volume level - dB
 */
		float rec_volume_dB;

		struct {
			void * consumed_buffer;

		} consumed_mediabuffer;

/**
 * Video frame capture
 */
		struct {
			unsigned int width;						/* width of captured image */
			unsigned int height;					/* height of captured image */
			unsigned int orientation;				/* orientation of captured image */
		} captured_frame;
/**
 * seek_data
 */
		struct {
			unsigned int stream_type;
			unsigned long long offset;
		} seek_data;
/**
 * buffer_status
 */
		struct {
			unsigned int stream_type;
			unsigned int status;
			unsigned long long bytes;
		} buffer_status;
	};

	int size;       /**< Allocated size of 'data' */

	int reserved;   /**< reserved field for future use */
} MMMessageParamType;

/**
 * @}
 */

#ifdef __cplusplus
	}
#endif


#endif	/* __MM_MESSAGE_H__ */
