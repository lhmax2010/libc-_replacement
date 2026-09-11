/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __CAPI_TELEPHONY_CALL_H__
#define __CAPI_TELEPHONY_CALL_H__

/**
 * @deprecated Deprecated since 10.0
 * @file telephony_call.h
 * @brief This file contains call APIs and related enumeration.
 */

#include "telephony_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_TELEPHONY_INFORMATION_CALL
 * @{
 */

/**
 * @deprecated Deprecated since 10.0
 * @brief The call handle.
 * @since_tizen 2.4
 */
typedef struct telephony_call_info_s *telephony_call_h;


/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration for the call status.
 * @since_tizen 2.4
 */
typedef enum {
	TELEPHONY_CALL_STATUS_IDLE, /**< Idle status */
	TELEPHONY_CALL_STATUS_ACTIVE, /**< Active status */
	TELEPHONY_CALL_STATUS_HELD, /**< Held status */
	TELEPHONY_CALL_STATUS_DIALING, /**< Dialing status */
	TELEPHONY_CALL_STATUS_ALERTING, /**< Alerting status */
	TELEPHONY_CALL_STATUS_INCOMING, /**< Incoming status */
} telephony_call_status_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration for the preferred voice call subscription.
 * @since_tizen 2.4
 */
typedef enum {
	TELEPHONY_CALL_PREFERRED_VOICE_SUBS_UNKNOWN = -1, /**< Unknown status */
	TELEPHONY_CALL_PREFERRED_VOICE_SUBS_CURRENT_NETWORK = 0, /**< Current network */
	TELEPHONY_CALL_PREFERRED_VOICE_SUBS_ASK_ALWAYS, /**< ASK Always */
	TELEPHONY_CALL_PREFERRED_VOICE_SUBS_SIM1, /**< SIM 1 */
	TELEPHONY_CALL_PREFERRED_VOICE_SUBS_SIM2, /**<  SIM 2 */
} telephony_call_preferred_voice_subs_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration for the call type.
 * @since_tizen 2.4
 */
typedef enum {
	TELEPHONY_CALL_TYPE_VOICE, /**< Voice call */
	TELEPHONY_CALL_TYPE_VIDEO, /**< Video call */
	TELEPHONY_CALL_TYPE_E911, /**< Emergency call */
} telephony_call_type_e;

/**
 * @deprecated Deprecated since 10.0
 * @brief Enumeration for the call direction.
 * @since_tizen 2.4
 */
typedef enum {
	TELEPHONY_CALL_DIRECTION_MO, /**< MO(Mobile Originated) call */
	TELEPHONY_CALL_DIRECTION_MT, /**< MT(Mobile Terminated) call */
} telephony_call_direction_e;


/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the current value for the preferred voice call subscription.
 * @details This function is used to retrieve the current value for the preferred voice call subscription.
 *         It requires the handle obtained from the telephony_init() function as an argument and returns
 *         the currently set preferred call subscription value in the form of #telephony_call_preferred_voice_subs_e.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 *
 * @param[in] handle The handle from telephony_init()
 * @param[out] call_sub The currently set preferred call subscription value. (#telephony_call_preferred_voice_subs_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 * @retval #TELEPHONY_ERROR_OPERATION_FAILED  Operation failed
 */
int telephony_call_get_preferred_voice_subscription(telephony_h handle, telephony_call_preferred_voice_subs_e *call_sub) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the list of the current call.
 * @details This function is used to retrieve the list of active calls in progress.
 *          It takes the telephony handle as input and returns the number of existing
 *          calls along with their details. By calling this API, you can get an overview
 *          of all ongoing calls associated with your application's telephony session.
 *          However, it should be noted that the returned call list needs to be released
 *          by calling the 'telephony_call_release_call_list' function.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks You must release @c call_list using telephony_call_release_call_list().
 *
 * @param[in] handle The handle from telephony_init()
 * @param[out] count Count of the existing calls
 * @param[out] call_list List of call information for existing calls
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_PERMISSION_DENIED Permission denied
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 * @retval #TELEPHONY_ERROR_OPERATION_FAILED  Operation failed
 */
int telephony_call_get_call_list(telephony_h handle, unsigned int *count, telephony_call_h **call_list) TIZEN_DEPRECATED_API;


/**
* @deprecated Deprecated since 10.0
* @brief Releases the list allocated from telephony_call_get_call_list().
 * @details This function releases the memory occupied by the call list obtained through
 *          the telephony_call_get_call_list() API. By invoking this API, you ensure that
 *          the resources consumed by the call list are freed up, allowing them to be reused
 *          elsewhere within your application or system. Remember that before releasing the call list,
 *          it is crucial to validate its validity using the 'telephony_call_validate_call_list' function.
 *
 * @since_tizen 2.4
 *
 * @param[in] count The count of the calls from telephony_call_get_call_list()
 * @param[in] call_list The handle from telephony_call_get_call_list()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre The call list should be validated from telephony_call_get_call_list().
 */
int telephony_call_release_call_list(unsigned int count, telephony_call_h **call_list) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the call handle ID.
 * @details This function retrieves the unique identifier (ID) associated with a given call handle.
 *          By utilizing this API, you can access specific attributes or perform actions related to
 *          a particular call within your application's telephony session.
 *
 * @since_tizen 2.4
 *
 * @param[in] call_handle The handle from telephony_call_get_call_list()
 * @param[out] handle_id The ID of the call handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre The call handle should be obtained from telephony_call_get_call_list().
 * @post The call list should be released by using telephony_call_release_call_list().
 */
int telephony_call_get_handle_id(telephony_call_h call_handle, unsigned int *handle_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the call number.
 * @since_tizen 2.4
 * @details This function retrieves the phone number associated with a specified call handle.
 *          By providing the call handle obtained from the telephony_call_get_call_list() API as input,
 *          you can obtain the corresponding call number.
 *
 * @remarks You must release @c number using free() on success case.
 *
 * @param[in] call_handle The handle from telephony_call_get_call_list()
 * @param[out] number The number of the call
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre The call handle should be obtained from telephony_call_get_call_list().
 * @post The call list should be released by using telephony_call_release_call_list().
 */
int telephony_call_get_number(telephony_call_h call_handle, char **number) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the call type.
 * @details This function retrieves the type of a specified call based on its handle.
 *          Knowing the call type allows you to provide appropriate handling mechanisms
 *          according to different scenarios.
 *
 * @since_tizen 2.4
 *
 * @param[in] call_handle The handle from telephony_call_get_call_list()
 * @param[out] type The type of the call (#telephony_call_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre The call handle should be obtained from telephony_call_get_call_list().
 * @post The call list should be released by using telephony_call_release_call_list().
 */
int telephony_call_get_type(telephony_call_h call_handle, telephony_call_type_e *type) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets the call status.
 * @details This function retrieves the current status of a call based on its handle.
 *          Understanding the call status enables you to implement accurate response
 *          mechanisms tailored to each situation.
 *
 * @since_tizen 2.4
 *
 * @param[in] call_handle The handle from telephony_call_get_call_list()
 * @param[out] status The status of the call (#telephony_call_status_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre The call handle should be obtained from telephony_call_get_call_list().
 * @post The call list should be released by using telephony_call_release_call_list().
 */
int telephony_call_get_status(telephony_call_h call_handle, telephony_call_status_e *status) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets whether the call is MO(Mobile Originated) call or MT(Mobile Terminated).
 * @since_tizen 2.4
 * @details This function determines whether a call originated from the mobile device (MO)
 *          or was received by the mobile device (MT), based on the provided call handle.
 *
 * @param[in] call_handle The handle from telephony_call_get_call_list()
 * @param[out] direction The direction of the call (#telephony_call_direction_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre The call handle should be obtained from telephony_call_get_call_list().
 * @post The call list should be released by using telephony_call_release_call_list().
 */
int telephony_call_get_direction(telephony_call_h call_handle, telephony_call_direction_e *direction) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0
 * @brief Gets whether the call is conference call or not.
 * @details This function checks if a call is part of a conference call or a single individual
 *          call based on the provided call handle. By fetching the required call handle using
 *          the telephony_call_get_call_list() API, you can determine the nature of the call and
 *          adjust your application's functionality accordingly.
 *
 * @since_tizen 2.4
 *
 * @param[in] call_handle The handle from telephony_call_get_call_list()
 * @param[out] conference_status The value whether the call is conference call or not
 *                            (true: Conference call, false: Single call)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #TELEPHONY_ERROR_NONE              Successful
 * @retval #TELEPHONY_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TELEPHONY_ERROR_NOT_SUPPORTED     Not supported
 *
 * @pre The call handle should be obtained from telephony_call_get_call_list().
 * @post The call list should be released by using telephony_call_release_call_list().
 */
int telephony_call_get_conference_status(telephony_call_h call_handle, bool *conference_status) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_TELEPHONY_CALL_H__ */
