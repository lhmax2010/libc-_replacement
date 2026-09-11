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



#ifndef __MM_ERROR_H__
#define __MM_ERROR_H__

#ifdef __cplusplus
	extern "C" {
#endif


/**
	@addtogroup  COMMON
	@{
	 * @file		mm_error.h
	 * @brief		This file defines error codes for multimedia framework.
	 * @version		1.0
	 *
	 * This file defines error codes for multimedia framework.
*/

#define MM_ERROR_NONE				0x00000000		/**< No Error */
#define MM_ERROR_CLASS				0x80000000		/**< Definition of number describing error group */
#define MM_ERROR_COMMON_CLASS			0x80000100		/**< Category for describing common error group */
#define MM_ERROR_SOUND_CLASS			0x80000200		/**< Category for describing sound error group */
#define MM_ERROR_RADIO_CLASS			0x80000300		/**< Category for describing radio error group */
#define MM_ERROR_PLAYER_CLASS			0x80000400		/**< Category for describing player error group */
#define MM_ERROR_RESOURCE_CLASS			0x80000500		/**< Category for describing resource error group */
#define MM_ERROR_FILE_CLASS			0x80000600		/**< Category for describing file error group */
#define MM_ERROR_STREAMRECORDER_CLASS		0x80000700      /**< Category for describing streamrecorder error group */
#define MM_ERROR_CAMCORDER_CLASS		0x80000800		/**< Category for describing camcorder error group */
#define MM_ERROR_WFD_CLASS		0x80000900		/**< Category for describing wifi-display error group */
#define MM_ERROR_IMAGE_CLASS			0x80000a00		/**< Category for describing image error group */
#define MM_ERROR_POLICY_CLASS			0x80000b00		/**< Category for Policy */
#define MM_ERROR_TRANSCODE_CLASS		0x80000c00		/**< Error message for describing transcode error group */
#define MM_ERROR_EVASRENDER_CLASS		0x80000d00		/**< Error message for describing mm-evas-renderer error group */

/*
	MM_ERROR_CLASS
*/
#define MM_ERROR_UNKNOWN			(MM_ERROR_CLASS | 0x00)		/**< Unclassified error */
#define MM_ERROR_INVALID_ARGUMENT		(MM_ERROR_CLASS | 0x01)		/**< Invalid argument */
#define MM_ERROR_OUT_OF_MEMORY			(MM_ERROR_CLASS | 0x02)		/**< Out of memory */
#define MM_ERROR_OUT_OF_STORAGE			(MM_ERROR_CLASS | 0x03)		/**< Out of storage */
#define MM_ERROR_INVALID_HANDLE			(MM_ERROR_CLASS | 0x04)		/**< Invalid handle */
#define MM_ERROR_FILE_NOT_FOUND			(MM_ERROR_CLASS | 0x05)		/**< Cannot find file */
#define MM_ERROR_FILE_READ			(MM_ERROR_CLASS | 0x06)		/**< Fail to read data from file */
#define MM_ERROR_FILE_WRITE			(MM_ERROR_CLASS | 0x07)		/**< Fail to write data to file */
#define MM_ERROR_END_OF_FILE			(MM_ERROR_CLASS | 0x08)		/**< End of file */
#define MM_ERROR_NOT_SUPPORT_API		(MM_ERROR_CLASS | 0x09)		/**< Not supported API*/
#define MM_ERROR_NOT_IMPLEMENTED		(MM_ERROR_CLASS | 0x0a)		/**< Not implemented functions */

/*
	MM_ERROR_COMMON_CLASS
*/
#define MM_ERROR_COMMON_INVALID_ARGUMENT	(MM_ERROR_COMMON_CLASS | 0x01)		/**< Invalid argument */
#define MM_ERROR_COMMON_NO_FREE_SPACE		(MM_ERROR_COMMON_CLASS | 0x02)		/**< Out of storage */
#define MM_ERROR_COMMON_OUT_OF_MEMORY		(MM_ERROR_COMMON_CLASS | 0x03)		/**< Out of memory */
#define MM_ERROR_COMMON_UNKNOWN			(MM_ERROR_COMMON_CLASS | 0x04)		/**< Unknown error */
#define MM_ERROR_COMMON_INVALID_ATTRTYPE	(MM_ERROR_COMMON_CLASS | 0x05)		/**< Invalid argument */
#define MM_ERROR_COMMON_INVALID_PERMISSION	(MM_ERROR_COMMON_CLASS | 0x06)		/**< Invalid permission */
#define MM_ERROR_COMMON_OUT_OF_ARRAY		(MM_ERROR_COMMON_CLASS | 0x07)		/**< Out of array */
#define MM_ERROR_COMMON_OUT_OF_RANGE		(MM_ERROR_COMMON_CLASS | 0x08)		/**< Out of value range*/
#define MM_ERROR_COMMON_ATTR_NOT_EXIST		(MM_ERROR_COMMON_CLASS | 0x09)		/**< Attribute doesn't exist. */
#define MM_ERROR_COMMON_INTERNAL			(MM_ERROR_COMMON_CLASS | 0x0a)		/**< Internal errors */

/*
 *	MM_ERROR_SOUND_CLASS
 */
#define MM_ERROR_SOUND_NOT_INITIALIZED			(MM_ERROR_SOUND_CLASS | 0x01)		/**< Not initialized player */
#define MM_ERROR_SOUND_CODEC_NOT_FOUND			(MM_ERROR_SOUND_CLASS | 0x02)		/**< Not supported format */
#define MM_ERROR_SOUND_DECODE				(MM_ERROR_SOUND_CLASS | 0x03)		/**< Error while decoding data */
#define MM_ERROR_SOUND_NO_FREE_SPACE			(MM_ERROR_SOUND_CLASS | 0x04)		/**< Out of storage */
#define MM_ERROR_SOUND_FILE_NOT_FOUND			(MM_ERROR_SOUND_CLASS | 0x05)		/**< Cannot find file */
#define MM_ERROR_SOUND_STREAM_NOT_FOUND			(MM_ERROR_SOUND_CLASS | 0x06)		/**< Cannot find stream */
#define MM_ERROR_SOUND_END_OF_STREAM			(MM_ERROR_SOUND_CLASS | 0x07)		/**< End of stream */
#define MM_ERROR_SOUND_SEEK				(MM_ERROR_SOUND_CLASS | 0x08)		/**< Seek related errors */
#define MM_ERROR_SOUND_INVALID_POINTER			(MM_ERROR_SOUND_CLASS | 0x09)		/**< Invalid pointer */
#define MM_ERROR_SOUND_PERMISSION_DENIED		(MM_ERROR_SOUND_CLASS | 0x0a)		/**< Permission Denied */
#define MM_ERROR_SOUND_INVALID_OPERATION		(MM_ERROR_SOUND_CLASS | 0x10)		/**< Invalid operation */
#define MM_ERROR_SOUND_NO_DATA				(MM_ERROR_SOUND_CLASS | 0x11)		/**< No data */

#define MM_ERROR_SOUND_INVALID_FILE			(MM_ERROR_SOUND_CLASS | 0x21)		/**< Invalid file */
#define MM_ERROR_SOUND_INVALID_PATH			(MM_ERROR_SOUND_CLASS | 0x22)		/**< Invalid path */
#define MM_ERROR_SOUND_INVALID_VOLUME			(MM_ERROR_SOUND_CLASS | 0x23)		/**< Invalid volume */
#define MM_ERROR_SOUND_UNSUPPORTED_MEDIA_TYPE		(MM_ERROR_SOUND_CLASS | 0x24)		/**< Invalid media type */
#define MM_ERROR_SOUND_INTERNAL				(MM_ERROR_SOUND_CLASS | 0x25)		/**< Internal errors */
#define MM_ERROR_SOUND_INVALID_STATE			(MM_ERROR_SOUND_CLASS | 0x26)		/**< Invalid state */
#define MM_ERROR_SOUND_VOLUME_BLOCKED_BY_SAFETY	(MM_ERROR_SOUND_CLASS | 0x27) /**< Set volume fail dueto safety volume popup */

#define MM_ERROR_SOUND_VOLUME_NO_INSTANCE		(MM_ERROR_SOUND_CLASS | 0x31)		/**< No active instance */
#define MM_ERROR_SOUND_VOLUME_CAPTURE_ONLY		(MM_ERROR_SOUND_CLASS | 0x32)		/**< Capture instance only */

#define MM_ERROR_SOUND_DEVICE_NOT_OPENED		(MM_ERROR_SOUND_CLASS | 0x41)		/**< Cannot open sound device */
#define MM_ERROR_SOUND_DEVICE_INVALID_FORMAT		(MM_ERROR_SOUND_CLASS | 0x42)		/**< Invalid file format */
#define MM_ERROR_SOUND_DEVICE_INVALID_CHANNEL		(MM_ERROR_SOUND_CLASS | 0x43)		/**< Invalid channel type */
#define MM_ERROR_SOUND_DEVICE_INVALID_SAMPLERATE	(MM_ERROR_SOUND_CLASS | 0x44)		/**< Invalid samplerate */
#define MM_ERROR_SOUND_DEVICE_INVALID_MODE		(MM_ERROR_SOUND_CLASS | 0x45)		/**< Invalid mode */
#define MM_ERROR_SOUND_DEVICE_INVALID_VOLUME		(MM_ERROR_SOUND_CLASS | 0x46)		/**< Invalid volume */

#define MM_ERROR_SOUND_SERVER_DISCONNECTED		(MM_ERROR_SOUND_CLASS | 0x51)		/**< Server is disconnected */

#define MM_ERROR_SOUND_UNSUPPORTED_FORMAT		MM_ERROR_SOUND_UNSUPPORTED_MEDIA_TYPE	/**< Not supported media format */


/*
	MM_ERROR_RADIO_CLASS
 */
#define MM_ERROR_RADIO_NOT_INITIALIZED			(MM_ERROR_RADIO_CLASS | 0x01)		/**< Radio instance is not initialized */
#define MM_ERROR_RADIO_NO_FREE_SPACE			(MM_ERROR_RADIO_CLASS | 0x02)		/**< Not enough memory */
#define MM_ERROR_RADIO_INTERNAL				(MM_ERROR_RADIO_CLASS | 0x03)		/**< Radio internal error */
#define MM_ERROR_RADIO_RESPONSE_TIMEOUT			(MM_ERROR_RADIO_CLASS | 0x04)		/**< Response timeout error */
#define MM_ERROR_RADIO_INVALID_STATE			(MM_ERROR_RADIO_CLASS | 0x05)		/**< Radio is in invalid state */
#define MM_ERROR_RADIO_NO_OP				(MM_ERROR_RADIO_CLASS | 0x06)		/**< Radio is in desired state */
#define MM_ERROR_RADIO_DEVICE_NOT_OPENED		(MM_ERROR_RADIO_CLASS | 0x07)		/**< Failed to opne radio devide */
#define MM_ERROR_RADIO_DEVICE_NOT_FOUND			(MM_ERROR_RADIO_CLASS | 0x08)		/**< Failed to find  radio devide */
#define MM_ERROR_RADIO_PERMISSION_DENIED		(MM_ERROR_RADIO_CLASS | 0x09)		/**< Not supported permission */
#define MM_ERROR_RADIO_NO_ANTENNA		(MM_ERROR_RADIO_CLASS | 0x0a)		/**<  No antenna error */


/*
	MM_ERROR_PLAYER_CLASS
*/
#define MM_ERROR_PLAYER_NOT_INITIALIZED					(MM_ERROR_PLAYER_CLASS | 0x01)		/**< Not initialized player */
#define MM_ERROR_PLAYER_CODEC_NOT_FOUND					(MM_ERROR_PLAYER_CLASS | 0x02)		/**< Not supported format */
#define MM_ERROR_PLAYER_DECODE						(MM_ERROR_PLAYER_CLASS | 0x03)		/**< Error while decoding data */
#define MM_ERROR_PLAYER_NO_FREE_SPACE					(MM_ERROR_PLAYER_CLASS | 0x04)		/**< Out of storage */
#define MM_ERROR_PLAYER_FILE_NOT_FOUND					(MM_ERROR_PLAYER_CLASS | 0x05)		/**< Cannot find file */
#define MM_ERROR_PLAYER_SEEK						(MM_ERROR_PLAYER_CLASS | 0x06)		/**< Seek related errors */
#define MM_ERROR_PLAYER_INTERNAL					(MM_ERROR_PLAYER_CLASS | 0x07)		/**< Internal errors */
#define MM_ERROR_PLAYER_INVALID_STATE					(MM_ERROR_PLAYER_CLASS | 0x08)		/**< Invalid state */
#define MM_ERROR_PLAYER_INVALID_URI					(MM_ERROR_PLAYER_CLASS | 0x09)		/**< Invalid URI */
#define MM_ERROR_PLAYER_NO_OP						(MM_ERROR_PLAYER_CLASS | 0x0a)		/**< No operation */
#define MM_ERROR_PLAYER_NOT_SUPPORTED_FORMAT				(MM_ERROR_PLAYER_CLASS | 0x0b)		/**< Not supported format */
#define MM_ERROR_PLAYER_NOT_SUPPORTED_SUBTITLE				(MM_ERROR_PLAYER_CLASS | 0x0c)		/**< Not supported subtitle */
#define MM_ERROR_PLAYER_AUDIO_CODEC_NOT_FOUND				(MM_ERROR_PLAYER_CLASS | 0x0d)		/**< Not supported audio codec */
#define MM_ERROR_PLAYER_VIDEO_CODEC_NOT_FOUND				(MM_ERROR_PLAYER_CLASS | 0x0e)		/**< Not supported video codec */
#define MM_ERROR_PLAYER_STREAMING_FAIL					(MM_ERROR_PLAYER_CLASS | 0x0f)		/**< Fail to play streaming */
#define MM_ERROR_PLAYER_INVALID_STREAM					(MM_ERROR_PLAYER_CLASS | 0x10)		/**< Invalid stream */
#define MM_ERROR_PLAYER_DOING_SEEK					(MM_ERROR_PLAYER_CLASS | 0x11)		/**< Still seeking */
#define MM_ERROR_PLAYER_DRM_NO_LICENSE					(MM_ERROR_PLAYER_CLASS | 0x12)		/**< No license */
#define MM_ERROR_PLAYER_DRM_EXPIRED					(MM_ERROR_PLAYER_CLASS | 0x13)		/**< Expired license */
#define MM_ERROR_PLAYER_DRM_FUTURE_USE					(MM_ERROR_PLAYER_CLASS | 0x14)		/**< License for future use */
#define MM_ERROR_PLAYER_DRM_NOT_AUTHORIZED				(MM_ERROR_PLAYER_CLASS | 0x15)          /**< Not authorized */
#define MM_ERROR_PLAYER_DRM_OUTPUT_PROTECTION				(MM_ERROR_PLAYER_CLASS | 0x16)          /**< Output protected */
#define MM_ERROR_PLAYER_RESOURCE_LIMIT					(MM_ERROR_PLAYER_CLASS | 0x17)          /**< Resource limit */
#define MM_ERROR_PLAYER_PERMISSION_DENIED				(MM_ERROR_PLAYER_CLASS | 0x18)          /**< Permission denied */
#define MM_ERROR_PLAYER_BUFFER_SPACE					(MM_ERROR_PLAYER_CLASS | 0x19)          /**< No buffer space available */

#define MM_ERROR_PLAYER_STREAMING_UNSUPPORTED_AUDIO			(MM_ERROR_PLAYER_CLASS | 0x22)		/**< Unsupported audio  */
#define MM_ERROR_PLAYER_STREAMING_UNSUPPORTED_VIDEO			(MM_ERROR_PLAYER_CLASS | 0x23)		/**< Unsupported video  */
#define MM_ERROR_PLAYER_STREAMING_CONNECTION_FAIL			(MM_ERROR_PLAYER_CLASS | 0x24)		/**< Fail to connect to streaming server  */
#define MM_ERROR_PLAYER_STREAMING_DNS_FAIL				(MM_ERROR_PLAYER_CLASS | 0x25)		/**< Fail to get host using DNS  */
#define MM_ERROR_PLAYER_STREAMING_SERVER_DISCONNECTED			(MM_ERROR_PLAYER_CLASS | 0x26)		/**< Streaming server is disconnected  */
#define MM_ERROR_PLAYER_STREAMING_BAD_SERVER				(MM_ERROR_PLAYER_CLASS | 0x27)		/**< Streaming server's bad beahvior  */
#define MM_ERROR_PLAYER_STREAMING_INVALID_PROTOCOL			(MM_ERROR_PLAYER_CLASS | 0x28)		/**< Invalid protocol  */
#define MM_ERROR_PLAYER_STREAMING_INVALID_URL				(MM_ERROR_PLAYER_CLASS | 0x29)		/**< Invalid URL  */
#define MM_ERROR_PLAYER_STREAMING_UNEXPECTED_MSG			(MM_ERROR_PLAYER_CLASS | 0x2a)		/**< Unexpected message from streaming server  */
#define MM_ERROR_PLAYER_STREAMING_OUT_OF_MEMORIES			(MM_ERROR_PLAYER_CLASS | 0x2b)		/**< Out of memories  */
#define MM_ERROR_PLAYER_STREAMING_RTSP_TIMEOUT				(MM_ERROR_PLAYER_CLASS | 0x2c)		/**< RTSP timed out  */
#define MM_ERROR_PLAYER_STREAMING_BAD_REQUEST				(MM_ERROR_PLAYER_CLASS | 0x2d)		/**< Bad request  */
#define MM_ERROR_PLAYER_STREAMING_NOT_AUTHORIZED			(MM_ERROR_PLAYER_CLASS | 0x2e)		/**< Not authorized  */
#define MM_ERROR_PLAYER_STREAMING_PAYMENT_REQUIRED			(MM_ERROR_PLAYER_CLASS | 0x2f)		/**< Payment required  */
#define MM_ERROR_PLAYER_STREAMING_FORBIDDEN				(MM_ERROR_PLAYER_CLASS | 0x30)		/**< Forbidden by streaming server  */
#define MM_ERROR_PLAYER_STREAMING_CONTENT_NOT_FOUND			(MM_ERROR_PLAYER_CLASS | 0x31)		/**< Content could not be found  */
#define MM_ERROR_PLAYER_STREAMING_METHOD_NOT_ALLOWED			(MM_ERROR_PLAYER_CLASS | 0x32)		/**< Not allowed method by streaming server  */
#define MM_ERROR_PLAYER_STREAMING_NOT_ACCEPTABLE			(MM_ERROR_PLAYER_CLASS | 0x33)		/**< Not accepted by streaming server  */
#define MM_ERROR_PLAYER_STREAMING_PROXY_AUTHENTICATION_REQUIRED		(MM_ERROR_PLAYER_CLASS | 0x34)		/**< Proxy authentication is required  */
#define MM_ERROR_PLAYER_STREAMING_SERVER_TIMEOUT			(MM_ERROR_PLAYER_CLASS | 0x35)		/**< Streaming server timed out  */
#define MM_ERROR_PLAYER_STREAMING_GONE					(MM_ERROR_PLAYER_CLASS | 0x36)		/**< Gone */
#define MM_ERROR_PLAYER_STREAMING_LENGTH_REQUIRED			(MM_ERROR_PLAYER_CLASS | 0x37)		/**< Length required */
#define MM_ERROR_PLAYER_STREAMING_PRECONDITION_FAILED			(MM_ERROR_PLAYER_CLASS | 0x38)		/**< Precondition is failed */
#define MM_ERROR_PLAYER_STREAMING_REQUEST_ENTITY_TOO_LARGE		(MM_ERROR_PLAYER_CLASS | 0x39)		/**< Request entity is too large */
#define MM_ERROR_PLAYER_STREAMING_REQUEST_URI_TOO_LARGE			(MM_ERROR_PLAYER_CLASS | 0x3a)		/**< Request URL is too large */
#define MM_ERROR_PLAYER_STREAMING_UNSUPPORTED_MEDIA_TYPE		(MM_ERROR_PLAYER_CLASS | 0x3b)		/**< Unsupported media type */
#define MM_ERROR_PLAYER_STREAMING_PARAMETER_NOT_UNDERSTOOD		(MM_ERROR_PLAYER_CLASS | 0x3c)		/**< Parameter could not bu understood  */
#define MM_ERROR_PLAYER_STREAMING_CONFERENCE_NOT_FOUND			(MM_ERROR_PLAYER_CLASS | 0x3d)		/**< Conference could not be found */
#define MM_ERROR_PLAYER_STREAMING_NOT_ENOUGH_BANDWIDTH			(MM_ERROR_PLAYER_CLASS | 0x3e)		/**< Bandwidth is not enough */
#define MM_ERROR_PLAYER_STREAMING_NO_SESSION_ID				(MM_ERROR_PLAYER_CLASS | 0x3f)		/**< No session ID */
#define MM_ERROR_PLAYER_STREAMING_METHOD_NOT_VALID_IN_THIS_STATE	(MM_ERROR_PLAYER_CLASS | 0x40)		/**< Method is not valid in this state  */
#define MM_ERROR_PLAYER_STREAMING_HEADER_FIELD_NOT_VALID_FOR_SOURCE	(MM_ERROR_PLAYER_CLASS | 0x41)		/**< Header field is not valid for source */
#define MM_ERROR_PLAYER_STREAMING_INVALID_RANGE				(MM_ERROR_PLAYER_CLASS | 0x42)		/**< Invalid range */
#define MM_ERROR_PLAYER_STREAMING_PARAMETER_IS_READONLY			(MM_ERROR_PLAYER_CLASS | 0x43)		/**< Parameter is read only */
#define MM_ERROR_PLAYER_STREAMING_AGGREGATE_OP_NOT_ALLOWED		(MM_ERROR_PLAYER_CLASS | 0x44)		/**< Aggregation option is not allowed */
#define MM_ERROR_PLAYER_STREAMING_ONLY_AGGREGATE_OP_ALLOWED		(MM_ERROR_PLAYER_CLASS | 0x45)		/**< Only aggregation option is allowed  */
#define MM_ERROR_PLAYER_STREAMING_BAD_TRANSPORT				(MM_ERROR_PLAYER_CLASS | 0x46)		/**< Bad transport */
#define MM_ERROR_PLAYER_STREAMING_DESTINATION_UNREACHABLE		(MM_ERROR_PLAYER_CLASS | 0x47)		/**< Destination could not be reached */
#define MM_ERROR_PLAYER_STREAMING_INTERNAL_SERVER_ERROR			(MM_ERROR_PLAYER_CLASS | 0x48)		/**< Internal server error */
#define MM_ERROR_PLAYER_STREAMING_NOT_IMPLEMENTED			(MM_ERROR_PLAYER_CLASS | 0x49)		/**< Not implemented */
#define MM_ERROR_PLAYER_STREAMING_BAD_GATEWAY				(MM_ERROR_PLAYER_CLASS | 0x4a)		/**< Bad gateway */
#define MM_ERROR_PLAYER_STREAMING_SERVICE_UNAVAILABLE			(MM_ERROR_PLAYER_CLASS | 0x4b)		/**< Service is unavailable */
#define MM_ERROR_PLAYER_STREAMING_GATEWAY_TIME_OUT			(MM_ERROR_PLAYER_CLASS | 0x4c)		/**< Gateway timed out  */
#define MM_ERROR_PLAYER_STREAMING_RTSP_VERSION_NOT_SUPPORTED		(MM_ERROR_PLAYER_CLASS | 0x4d)		/**< RTSP version is not supported */
#define MM_ERROR_PLAYER_STREAMING_OPTION_NOT_SUPPORTED			(MM_ERROR_PLAYER_CLASS | 0x4e)		/**< Not supported option */

#define MM_ERROR_PLAYER_SOUND_EFFECT_NOT_SUPPORTED_FILTER		(MM_ERROR_PLAYER_CLASS | 0x51)		/**< Not supported filter */
#define MM_ERROR_PLAYER_SOUND_EFFECT_INVALID_STATUS			(MM_ERROR_PLAYER_CLASS | 0x52)		/**< Invalid status */


/*
	MM_ERROR_RESOURCE_CLASS
 */
#define MM_ERROR_RESOURCE_NOT_INITIALIZED			(MM_ERROR_RESOURCE_CLASS | 0x01)		/**< Not initialized */
#define MM_ERROR_RESOURCE_INTERNAL				(MM_ERROR_RESOURCE_CLASS | 0x02)		/**< Internal errors */
#define MM_ERROR_RESOURCE_INVALID_STATE				(MM_ERROR_RESOURCE_CLASS | 0x03)		/**< Invalid state */


/*
	MM_CAMCORDER_ERROR_CLASS
*/
#define MM_ERROR_CAMCORDER_DEVICE			(MM_ERROR_CAMCORDER_CLASS | 0x01)		/**< General error of camera device */
#define MM_ERROR_CAMCORDER_DEVICE_NOT_FOUND		(MM_ERROR_CAMCORDER_CLASS | 0x02)		/**< Cannot find camera device */
#define MM_ERROR_CAMCORDER_DEVICE_BUSY			(MM_ERROR_CAMCORDER_CLASS | 0x03)		/**< Busy camera device */
#define MM_ERROR_CAMCORDER_DEVICE_OPEN			(MM_ERROR_CAMCORDER_CLASS | 0x04)		/**< Fail to open camera device */
#define MM_ERROR_CAMCORDER_DEVICE_IO			(MM_ERROR_CAMCORDER_CLASS | 0x05)		/**< Fail to control camera device */
#define MM_ERROR_CAMCORDER_DEVICE_TIMEOUT		(MM_ERROR_CAMCORDER_CLASS | 0x06)		/**< Device Timeout */
#define MM_ERROR_CAMCORDER_DEVICE_REG_TROUBLE		(MM_ERROR_CAMCORDER_CLASS | 0x07)		/**< Register trouble of camear device */
#define MM_ERROR_CAMCORDER_DEVICE_WRONG_JPEG		(MM_ERROR_CAMCORDER_CLASS | 0x08)		/**< Captured data is abnormal */
#define MM_ERROR_CAMCORDER_DEVICE_LACK_BUFFER		(MM_ERROR_CAMCORDER_CLASS | 0x09)		/**< Buffer of device is not sufficient */

#define MM_ERROR_CAMCORDER_GST_CORE			(MM_ERROR_CAMCORDER_CLASS | 0x0a)		/**< Gst Core error */
#define MM_ERROR_CAMCORDER_GST_LIBRARY			(MM_ERROR_CAMCORDER_CLASS | 0x0b)		/**< Gst library error */
#define MM_ERROR_CAMCORDER_GST_RESOURCE			(MM_ERROR_CAMCORDER_CLASS | 0x0c)		/**< Gst resource error */
#define MM_ERROR_CAMCORDER_GST_STREAM			(MM_ERROR_CAMCORDER_CLASS | 0x0d)		/**< Gst stream error */
#define MM_ERROR_CAMCORDER_GST_STATECHANGE		(MM_ERROR_CAMCORDER_CLASS | 0x0e)		/**< Gst state change failed */
#define MM_ERROR_CAMCORDER_GST_NEGOTIATION		(MM_ERROR_CAMCORDER_CLASS | 0x0f)		/**< Gst negotiation failed */
#define MM_ERROR_CAMCORDER_GST_LINK			(MM_ERROR_CAMCORDER_CLASS | 0x10)		/**< Gst linking failed */
#define MM_ERROR_CAMCORDER_GST_FLOW_ERROR		(MM_ERROR_CAMCORDER_CLASS | 0x11)		/**< Gst linking failed */

#define MM_ERROR_CAMCORDER_ENCODER			(MM_ERROR_CAMCORDER_CLASS | 0x12)		/**< General error of Encoder */
#define MM_ERROR_CAMCORDER_ENCODER_BUFFER		(MM_ERROR_CAMCORDER_CLASS | 0x13)		/**< Encoder buffer error */
#define MM_ERROR_CAMCORDER_ENCODER_WRONG_TYPE		(MM_ERROR_CAMCORDER_CLASS | 0x14)		/**< Encoder wrong type (format or param) */
#define MM_ERROR_CAMCORDER_ENCODER_WORKING		(MM_ERROR_CAMCORDER_CLASS | 0x15)		/**< Encoding fail */

#define MM_ERROR_CAMCORDER_INVALID_CONDITION		(MM_ERROR_CAMCORDER_CLASS | 0x16)		/**< Invalid pre-condition */
#define MM_ERROR_CAMCORDER_INVALID_STATE		(MM_ERROR_CAMCORDER_CLASS | 0x17)		/**< Invalid state */
#define MM_ERROR_CAMCORDER_INVALID_ARGUMENT		(MM_ERROR_CAMCORDER_CLASS | 0x18)		/**< Invalid Argument */

#define MM_ERROR_CAMCORDER_INTERNAL			(MM_ERROR_CAMCORDER_CLASS | 0x19)		/**< Internal error */
#define MM_ERROR_CAMCORDER_NOT_SUPPORTED		(MM_ERROR_CAMCORDER_CLASS | 0x1a)		/**< Not supported by camera device */
#define MM_ERROR_CAMCORDER_RESPONSE_TIMEOUT		(MM_ERROR_CAMCORDER_CLASS | 0x1b)		/**< Not responsed in time */
#define MM_ERROR_CAMCORDER_NOT_INITIALIZED		(MM_ERROR_CAMCORDER_CLASS | 0x1c)		/**< Not initialized camcorder */
#define MM_ERROR_CAMCORDER_LOW_MEMORY			(MM_ERROR_CAMCORDER_CLASS | 0x1d)		/**< Memory alloc failed */
#define MM_ERROR_CAMCORDER_RESOURCE_CREATION		(MM_ERROR_CAMCORDER_CLASS | 0x1e)		/**< Creating resource of camcorder failed */
#define MM_ERROR_CAMCORDER_CMD_IS_RUNNING		(MM_ERROR_CAMCORDER_CLASS | 0x1f)		/**< Command is running now */
#define MM_ERROR_CAMCORDER_DSP_FAIL			(MM_ERROR_CAMCORDER_CLASS | 0x20)		/**< DSP fail */
#define MM_ERROR_CAMCORDER_AUDIO_EMPTY			(MM_ERROR_CAMCORDER_CLASS | 0x21)		/**< Audio data is empty */
#define MM_ERROR_CAMCORDER_CREATE_CONFIGURE		(MM_ERROR_CAMCORDER_CLASS | 0x22)		/**< Failed to create configure structure */
#define MM_ERROR_CAMCORDER_FILE_SIZE_OVER		(MM_ERROR_CAMCORDER_CLASS | 0x23)		/**< File size is greater than max size */
#define MM_ERROR_CAMCORDER_DISPLAY_DEVICE_OFF		(MM_ERROR_CAMCORDER_CLASS | 0x24)		/**< Display device is off. Requests could be ignored */

#define MM_ERROR_CAMCORDER_MNOTE_CREATION		(MM_ERROR_CAMCORDER_CLASS | 0x26)		/**< Exif makernote data construction failed */
#define MM_ERROR_CAMCORDER_MNOTE_MALLOC			(MM_ERROR_CAMCORDER_CLASS | 0x27)		/**< Memory allocation for makernote entries failed */
#define MM_ERROR_CAMCORDER_MNOTE_ADD_ENTRY		(MM_ERROR_CAMCORDER_CLASS | 0x28)		/**< Adding a makernote entry to list failed */

/*
	MM_ERROR_STREAMRECORDER_CLASS
*/
#define MM_ERROR_STREAMRECORDER_DEVICE			(MM_ERROR_STREAMRECORDER_CLASS | 0x01)		/**< General error of streamrecorder device */

#define MM_ERROR_STREAMRECORDER_GST_CORE		(MM_ERROR_STREAMRECORDER_CLASS | 0x0a)		/**< Gst Core error */
#define MM_ERROR_STREAMRECORDER_GST_LIBRARY		(MM_ERROR_STREAMRECORDER_CLASS | 0x0b)		/**< Gst library error */
#define MM_ERROR_STREAMRECORDER_GST_RESOURCE		(MM_ERROR_STREAMRECORDER_CLASS | 0x0c)		/**< Gst resource error */
#define MM_ERROR_STREAMRECORDER_GST_STREAM		(MM_ERROR_STREAMRECORDER_CLASS | 0x0d)		/**< Gst stream error */
#define MM_ERROR_STREAMRECORDER_GST_STATECHANGE		(MM_ERROR_STREAMRECORDER_CLASS | 0x0e)		/**< Gst state change failed */
#define MM_ERROR_STREAMRECORDER_GST_NEGOTIATION		(MM_ERROR_STREAMRECORDER_CLASS | 0x0f)		/**< Gst negotiation failed */
#define MM_ERROR_STREAMRECORDER_GST_LINK		(MM_ERROR_STREAMRECORDER_CLASS | 0x10)		/**< Gst linking failed */
#define MM_ERROR_STREAMRECORDER_GST_FLOW_ERROR		(MM_ERROR_STREAMRECORDER_CLASS | 0x11)		/**< Gst linking failed */

#define MM_ERROR_STREAMRECORDER_ENCODER			(MM_ERROR_STREAMRECORDER_CLASS | 0x12)		/**< General error of Encoder */
#define MM_ERROR_STREAMRECORDER_ENCODER_BUFFER		(MM_ERROR_STREAMRECORDER_CLASS | 0x13)		/**< Encoder buffer error */
#define MM_ERROR_STREAMRECORDER_ENCODER_WRONG_TYPE	(MM_ERROR_STREAMRECORDER_CLASS | 0x14)		/**< Encoder wrong type (format or param) */
#define MM_ERROR_STREAMRECORDER_ENCODER_WORKING		(MM_ERROR_STREAMRECORDER_CLASS | 0x15)		/**< Encoding fail */

#define MM_ERROR_STREAMRECORDER_INVALID_CONDITION	(MM_ERROR_STREAMRECORDER_CLASS | 0x16)		/**< Invalid pre-condition */

#define MM_ERROR_STREAMRECORDER_INVALID_ARGUMENT	(MM_ERROR_STREAMRECORDER_CLASS | 0x18)		/**< Invalid Argument */

#define MM_ERROR_STREAMRECORDER_INTERNAL		(MM_ERROR_STREAMRECORDER_CLASS | 0x19)		/**< Internal error */
#define MM_ERROR_STREAMRECORDER_RESPONSE_TIMEOUT	(MM_ERROR_STREAMRECORDER_CLASS | 0x1b)		/**< Not responsed in time */

#define MM_ERROR_STREAMRECORDER_NOT_INITIALIZED		(MM_ERROR_STREAMRECORDER_CLASS | 0x1c)		/**< Not initialized streamrecorder */
#define MM_ERROR_STREAMRECORDER_LOW_MEMORY		(MM_ERROR_STREAMRECORDER_CLASS | 0x1d)		/**< Memory alloc failed */
#define MM_ERROR_STREAMRECORDER_RESOURCE_CREATION	(MM_ERROR_STREAMRECORDER_CLASS | 0x1e)		/**< Creating resource of streamrecorder failed */
#define MM_ERROR_STREAMRECORDER_CMD_IS_RUNNING		(MM_ERROR_STREAMRECORDER_CLASS | 0x1f)		/**< Command is running now */

#define MM_ERROR_STREAMRECORDER_FILE_SIZE_OVER		(MM_ERROR_STREAMRECORDER_CLASS | 0x23)		/**< File size is greater than max size */

#define MM_ERROR_STREAMRECORDER_AUDIOBUFFER_PUSH	(MM_ERROR_STREAMRECORDER_CLASS | 0x26)		/**< Exif makernote data construction failed */
#define MM_ERROR_STREAMRECORDER_VIDEOBUFFER_PUSH	(MM_ERROR_STREAMRECORDER_CLASS | 0x27)		/**< Memory allocation for makernote entries failed */
#define MM_ERROR_STREAMRECORDER_BUFFER_CONVERT		(MM_ERROR_STREAMRECORDER_CLASS | 0x28)		/**< Adding a makernote entry to list failed */

/*
	MM_ERROR_WFD_CLASS
*/
#define MM_ERROR_WFD_INVALID_ARGUMENT			(MM_ERROR_WFD_CLASS | 0x01)		/**< Invalid Argument */
#define MM_ERROR_WFD_NO_FREE_SPACE			(MM_ERROR_WFD_CLASS | 0x02)		/**< Out of storage */
#define MM_ERROR_WFD_NOT_INITIALIZED			(MM_ERROR_WFD_CLASS | 0x03)		/**< Not initialized wifi-display */
#define MM_ERROR_WFD_NO_OP				(MM_ERROR_WFD_CLASS | 0x04)		/**< No operation */
#define MM_ERROR_WFD_INVALID_STATE			(MM_ERROR_WFD_CLASS | 0x05)		/**< Invalid state */
#define MM_ERROR_WFD_INTERNAL				(MM_ERROR_WFD_CLASS | 0x06)		/**< Internal errors */
#define MM_ERROR_WFD_NOHDCPKEY				(MM_ERROR_WFD_CLASS | 0x07)		/**< No HDCP key */


/*
	MM_IMAGE_ERROR_CLASS
*/
#define MM_ERROR_IMAGE_FILEOPEN				(MM_ERROR_IMAGE_CLASS | 1)		/**< Cannot open file */
#define MM_ERROR_IMAGE_INTERNAL				(MM_ERROR_IMAGE_CLASS | 2)		/**< Image module internal error */
#define MM_ERROR_IMAGEHANDLE_NOT_INITIALIZED		(MM_ERROR_IMAGE_CLASS | 3)		/**< Fail to open camera device */
#define MM_ERROR_NO_DECODED_DATA			(MM_ERROR_IMAGE_CLASS | 4)		/**< Fail to control camera device */
#define MM_ERROR_IMAGE_NO_FREE_SPACE			(MM_ERROR_IMAGE_CLASS | 5)		/**< Not enough memory */
#define MM_ERROR_IMAGE_INVALID_VALUE			(MM_ERROR_IMAGE_CLASS | 0x22)		/**< Invalid value */
#define MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT		(MM_ERROR_IMAGE_CLASS | 0x23)		/**< Not supported image format */
#define MM_ERROR_IMAGE_DEVICE_NOT_SUPPORT		(MM_ERROR_IMAGE_CLASS | 0x24)		/**< Not supported image device */

/*
	MM_ERROR_TRANSCODE_CLASS
*/
#define MM_ERROR_TRANSCODE_FILEOPEN					(MM_ERROR_TRANSCODE_CLASS | 1)		/**< Cannot open file */
#define MM_ERROR_TRANSCODE_INTERNAL					(MM_ERROR_TRANSCODE_CLASS | 2)		/**< Transcode internal error */
#define MM_ERROR_TRANSCODE_NOT_INITIALIZED			(MM_ERROR_TRANSCODE_CLASS | 3)		/**< Fail to open transcode device */
#define MM_ERROR_TRANSCODE_NO_DECODED_DATA			(MM_ERROR_TRANSCODE_CLASS | 4)		/**< Fail to control transcode device */
#define MM_ERROR_TRANSCODE_NO_FREE_SPACE			(MM_ERROR_TRANSCODE_CLASS | 5)		/**< Not enough memory */
#define MM_ERROR_TRANSCODE_INVALID_VALUE			(MM_ERROR_TRANSCODE_CLASS | 0x22)	/**< Invalid value */
#define MM_ERROR_TRANSCODE_NOT_SUPPORT_FORMAT		(MM_ERROR_TRANSCODE_CLASS | 0x23)	/**< Not supported transcode format */
#define MM_ERROR_TRANSCODE_DEVICE_NOT_SUPPORT		(MM_ERROR_TRANSCODE_CLASS | 0x24)	/**< Not supported transcode device */
#define MM_ERROR_TRANSCODE_FILE_NOT_FOUND			(MM_ERROR_TRANSCODE_CLASS | 0x25)	/**< Cannot find file */
#define MM_ERROR_TRANSCODE_RESOURCE_CREATION		(MM_ERROR_TRANSCODE_CLASS | 0x26)	/**< Creating resource of transcode failed */
#define MM_ERROR_TRANSCODE_SEEK_FAILED				(MM_ERROR_TRANSCODE_CLASS | 0x27)	/**< Transcode video seek  failed */

/*
	MM_ERROR_FILE_CLASS
 */
#define MM_ERROR_FILE_INTERNAL			(MM_ERROR_FILE_CLASS | 0x01)		/**< Internal error */
#define MM_ERROR_FILE_UNKNOWN_FORMAT		(MM_ERROR_FILE_CLASS | 0x02)		/**< Unknown file format */


/*
	 MM_ERROR_POLICY_CLASS
 */
#define MM_ERROR_POLICY_BLOCKED			(MM_ERROR_POLICY_CLASS | 0x01)		/**< Blocked by Audio system policy */
#define MM_ERROR_POLICY_INTERRUPTED		(MM_ERROR_POLICY_CLASS | 0x02)		/**< Interrupted by Audio system policy */
#define MM_ERROR_POLICY_INTERNAL		(MM_ERROR_POLICY_CLASS | 0x03)		/**< Internal errors */
#define MM_ERROR_POLICY_DUPLICATED		(MM_ERROR_POLICY_CLASS | 0x04)		/**< Duplicated Policy Instance */
#define MM_ERROR_POLICY_RESTRICTED		(MM_ERROR_POLICY_CLASS | 0x05)		/**< Restricted by security system policy */
#define MM_ERROR_POLICY_BLOCKED_BY_CALL		(MM_ERROR_POLICY_CLASS | 0x06)		/**< Blocked by Audio system policy - CALL */
#define MM_ERROR_POLICY_BLOCKED_BY_ALARM	(MM_ERROR_POLICY_CLASS | 0x07)		/**< Blocked by Audio system policy - ALARM */

/*
	MM_ERROR_EVASRENDER_CLASS
 */
#define MM_ERROR_EVASRENDER_NO_FREE_SPACE		(MM_ERROR_EVASRENDER_CLASS | 0x01)		/**< Not enough memory */
#define MM_ERROR_EVASRENDER_NOT_INITIALIZED		(MM_ERROR_EVASRENDER_CLASS | 0x02)		/**< Not initialized */
#define MM_ERROR_EVASRENDER_INTERNAL			(MM_ERROR_EVASRENDER_CLASS | 0x03)		/**< Internal error */
#define MM_ERROR_EVASRENDER_INVALID_OPERATION		(MM_ERROR_EVASRENDER_CLASS | 0x04)		/**< Invalid operation */
#define MM_ERROR_EVASRENDER_INVALID_ARGUMENT		(MM_ERROR_EVASRENDER_CLASS | 0x05)		/**< Invalid Argument */
/**
	@}
*/

#ifdef __cplusplus
	}
#endif

#endif	/* __MM_ERROR_H__ */
