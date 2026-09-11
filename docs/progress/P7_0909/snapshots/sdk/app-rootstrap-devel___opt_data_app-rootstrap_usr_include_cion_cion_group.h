/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_APPFW_CION_GROUP_H__
#define __TIZEN_APPFW_CION_GROUP_H__

#include <cion_payload.h>
#include <cion_peer_info.h>
#include <cion_security.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_GROUP_MODULE
 * @{
 */

/**
 * @brief The Cion group handle.
 * @since_tizen 6.5
 */
typedef void *cion_group_h;

/**
 * @brief Called when the payload is received.
 * @since_tizen 6.5
 * @param[in] topic_name The name of topic \n
 *            The @a topic_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] payload The received payload \n
 *            The @a payload can be used only in the callback. \n
 *            The @a payload should not be released.
 * @param[in] user_data The user data
 * @see cion_group_add_payload_received_cb()
 * @see #cion_peer_info_h
 * @see #cion_payload_h
 */
typedef void (*cion_group_payload_received_cb)(const char *topic_name,
    const cion_peer_info_h peer_info, cion_payload_h payload, void *user_data);

/**
 * @brief Called when a peer joins a topic.
 * @since_tizen 6.5
 * @param[in] topic_name The name of topic \n
 *            The @a topic_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_group_add_joined_cb()
 * @see #cion_peer_info_h
 */
typedef void (*cion_group_joined_cb)(const char *topic_name,
    const cion_peer_info_h peer_info, void *user_data);

/**
 * @brief Called when a peer leaves a topic.
 * @since_tizen 6.5
 * @param[in] topic_name The name of topic \n
 *            The @a topic_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_group_add_left_cb()
 * @see #cion_peer_info_h
 */
typedef void (*cion_group_left_cb)(const char *topic_name,
    const cion_peer_info_h peer_info, void *user_data);

/**
 * @brief Creates a Cion group handle.
 * @details The Cion group is the group to share data. \n
 *          The users can subscribe the group that named topic and publish the data to share. \n
 * @since_tizen 6.5
 * @remarks @a group must be released using cion_group_destroy().
 * @remarks Max length of @a topic_name including the null terminator is 512.
 * @param[out] group The Cion group handle
 * @param[in] topic_name The name of topic
 * @param[in] security The Cion security handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_NOT_SUPPORTED Not supported
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see cion_group_destroy()
 * @see cion_security_create()
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;
	cion_group_h group = NULL;

	ret = cion_group_create(&group, "mytopic", NULL);
}
 * @endcode
 */
int cion_group_create(cion_group_h *group, const char *topic_name,
    cion_security_h security);

/**
 * @brief Destroys the group handle.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_group_create()
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_group_destroy(group);
}
 * @endcode
 */
int cion_group_destroy(cion_group_h group);

/**
 * @brief Subscribes to a topic that the group handle has.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/d2d.datasharing \n
 *            %http://tizen.org/privilege/internet
 * @param[in] group The Cion group handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_group_subscribe(group);
}
 * @endcode
 */
int cion_group_subscribe(cion_group_h group);

/**
 * @brief Unsubscribes from topic that the group handle has.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_group_unsubscribe(group);
}
 * @endcode
 */
int cion_group_unsubscribe(cion_group_h group);

/**
 * @brief Publishes data to the group.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @param[in] data The data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_INVALID_OPERATION Invalid operation
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_group_publish(group, payload);
}
 * @endcode
 */
int cion_group_publish(cion_group_h group, cion_payload_h data);

/**
 * @brief Adds callback function to receive payload.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_payload_received_cb()
 * @see cion_group_remove_payload_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_group_add_payload_received_cb(group,
            _cion_group_payload_received_cb, NULL);
}
 * @endcode
 */
int cion_group_add_payload_received_cb(cion_group_h group,
    cion_group_payload_received_cb cb, void *user_data);

/**
 * @brief Removes callback function to receive payload.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_payload_received_cb()
 * @see cion_group_add_payload_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_group_remove_payload_received_cb(group,
            _cion_group_payload_received_cb);
}
 * @endcode
 */
int cion_group_remove_payload_received_cb(cion_group_h group,
    cion_group_payload_received_cb cb);

/**
 * @brief Adds callback function for join event.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_group_joined_cb()
 * @see cion_group_remove_joined_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_group_add_joined_cb(group, _cion_group_joined_cb, NULL);
}
 * @endcode
 */
int cion_group_add_joined_cb(cion_group_h group,
    cion_group_joined_cb cb, void *user_data);

/**
 * @brief Removes callback function for join event.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_group_joined_cb()
 * @see cion_group_add_joined_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_group_remove_joined_cb(group, _cion_group_joined_cb);
}
 * @endcode
 */
int cion_group_remove_joined_cb(cion_group_h group, cion_group_joined_cb cb);

/**
 * @brief Adds callback function to get peer leaving information.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @param[in] cb The callback function
 * @param[in] user_data The user_data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_group_left_cb()
 * @see cion_group_remove_left_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_group_add_left_cb(group, _cion_group_left_cb, NULL);
}
 * @endcode
 */
int cion_group_add_left_cb(cion_group_h group,
    cion_group_left_cb cb, void *user_data);

/**
 * @brief Removes callback function to get peer leaving information.
 * @since_tizen 6.5
 * @param[in] group The Cion group handle
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_group_left_cb()
 * @see cion_group_add_left_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_group_remove_left_cb(group, _cion_group_left_cb);
}
 * @endcode
 */
int cion_group_remove_left_cb(cion_group_h group, cion_group_left_cb cb);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_APPFW_CION_GROUP_H__ */
