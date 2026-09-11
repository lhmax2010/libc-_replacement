/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_BASE_TIZEN_CORE_CHANNEL_H__
#define __TIZEN_BASE_TIZEN_CORE_CHANNEL_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file tizen_core_channel.h
 */

/**
 * @addtogroup CAPI_TIZEN_CORE_CHANNEL_MODULE
 * @{
 */

/**
 * @brief The tizen core channel sender handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_channel_sender_h;

/**
 * @brief The tizen core channel receiver handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_channel_receiver_h;

/**
 * @brief The tizen core channel object handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_channel_object_h;

/**
 * @brief Makes a pair of channel sender and receiver.
 * @since_tizen 9.0
 * @remarks The @a sender should be released using tizen_core_channel_sender_destroy().
 * @remarks The @a receiver should be released using tizen_core_channel_receiver_destroy().
 *
 * @param[out] sender The tizen core channel sender handle
 * @param[out] receiver The tizen core channel receiver handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_sender_h sender = NULL;
  tizen_core_channel_receiver_h receiver = NULL;
  int ret;

  ret = tizen_core_channel_make_pair(&sender, &receiver);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to make channel pair");
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_channel_sender_destroy()
 * @see tizen_core_channel_receiver_destroy()
 */
int tizen_core_channel_make_pair(tizen_core_channel_sender_h *sender,
                                 tizen_core_channel_receiver_h *receiver);

/**
 * @brief Sends the channel object to the receiver.
 * @since_tizen 9.0
 *
 * @param[in] sender The tizen core channel sender handle
 * @param[in] object The tizen core channel object handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_sender_h sender = NULL;
  tizen_core_channel_receiver_h receiver = NULL;
  tizen_core_channel_object_h object = NULL;
  int ret;

  tizen_core_channel_make_pair(&sender, &receiver);
  tizen_core_channel_object_create(&object);
  tizen_core_channel_object_set_id(object, 22);
  tizen_core_channel_object_set_data(object, strdup("22"));

  int ret = tizen_core_channel_sender_send(sender, object);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to send object");

  tizen_core_channel_object_destroy(object);
}
 * @endcode
 *
 * @see tizen_core_channel_object_create()
 */
int tizen_core_channel_sender_send(tizen_core_channel_sender_h sender,
                                   tizen_core_channel_object_h object);

/**
 * @brief Destroys the channel sender handle.
 * @since_tizen 9.0
 *
 * @param[in] sender The tizen core channel sender handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_sender_h sender = NULL;
  tizen_core_channel_receiver_h receiver = NULL;
  int ret;

  tizen_core_channel_make_pair(&sender, &receiver);
  tizen_core_channel_receiver_destroy(receiver);

  ret = tizen_core_channel_sender_destroy(sender);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to destroy channel sender");
}
 * @endcode
 *
 * @see tizen_core_channel_make_pair()
 * @see tizen_core_channel_sender_clone()
 */
int tizen_core_channel_sender_destroy(tizen_core_channel_sender_h sender);

/**
 * @brief Creates and returns a copy of the given the @a sender handle.
 * @since_tizen 9.0
 * @remarks The @a cloned_sender should be released using tizen_core_channel_sender_destroy().
 *
 * @param[in] sender The tizen core channel sender handle
 * @param[out] cloned_sender If successful, a newly created tizen core channel sender handle will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_sender_h sender = NULL;
  tizen_core_channel_sender_h cloned_sender = NULL;
  tizen_core_channel_receiver_h receiver = NULL;
  int ret;

  tizen_core_channel_make_pair(&sender, &receiver);

  ret = tizen_core_channel_sender_clone(sender, &cloned_sender);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to clone channel sender");
}
 * @endcode
 *
 * @see tizen_core_channel_sender_destroy()
 */
int tizen_core_channel_sender_clone(tizen_core_channel_sender_h sender,
                                    tizen_core_channel_sender_h *cloned_sender);

/**
 * @brief Receives the channel object from the sender.
 * @since_tizen 9.0
 * @remarks The @a object should be released using tizen_core_channel_object_destroy().
 *
 * @param[in] receiver The tizen core channel receiver handle
 * @param[out] object The tizen core channel object handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_sender_h sender = NULL;
  tizen_core_channel_receiver_h receiver = NULL;
  tizen_core_channel_object_h object = NULL;
  int id = -1;
  char *data = NULL;
  int ret;

  tizen_core_channel_make_pair(&sender, &receiver);
  tizen_core_channel_object_create(&object);
  tizen_core_channel_object_set_id(object, 22);
  tizen_core_channel_object_set_data(object, strdup("22"));

  tizen_core_channel_sender_send(sender, object);
  tizen_core_channel_object_destroy(object);
  object = NULL;

  ret = tizen_core_channel_receiver_receive(receiver, &object);
  if (ret == TIZEN_CORE_ERROR_NONE) {
    tizen_core_channel_object_get_id(object, &id);
    tizen_core_channel_object_get_data(object, &data);
    dlog_print(DLOG_INFO, LOG_TAG, "id=%d, data=%s", id, data);
    free(data);
    tizen_core_channel_object_destroy(object);
  }

  tizen_core_channel_sender_destroy(sender);
  tizen_core_channel_receiver_destroy(receiver);
}
 * @endcode
 *
 * @see tizen_core_channel_object_destroy()
 */
int tizen_core_channel_receiver_receive(tizen_core_channel_receiver_h receiver,
                                        tizen_core_channel_object_h *object);

/**
 * @brief Destroys the tizen core channel receiver handle.
 * @since_tizen 9.0
 *
 * @param[in] receiver The tizen core channel receiver handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_sender_h sender = NULL;
  tizen_core_channel_receiver_h receiver = NULL;
  int ret;

  tizen_core_channel_make_pair(&sender, &receiver);
  tizen_core_channel_sender_destroy(sender);

  ret = tizen_core_channel_receiver_destroy(receiver);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to destroy channel receiver");
}
 * @endcode
 *
 * @see tizen_core_channel_make_pair()
 */
int tizen_core_channel_receiver_destroy(tizen_core_channel_receiver_h receiver);

/**
 * @brief Creates the tizen core channel object handle.
 * @since_tizen 9.0
 * @remarks The @a object should be released using tizen_core_channel_object_destroy().
 *
 * @param[out] object The tizen core channel object handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_object_h object = NULL;
  int ret;

  ret = tizen_core_channel_object_create(&object);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create channel object");
    return;
  }

  tizen_core_channel_object_destroy(object);
}
 * @endcode
 *
 * @see tizen_core_channel_object_destroy()
 */
int tizen_core_channel_object_create(tizen_core_channel_object_h *object);

/**
 * @brief Destroys the tizen core channel object handle.
 * @since_tizen 9.0
 *
 * @param[in] object The tizen core channel object handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_object_h object = NULL;
  int ret;

  tizen_core_channel_object_create(&object);

  ret = tizen_core_channel_object_destroy(object);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to destroy channel object");
}
 * @endcode
 *
 * @see tizen_core_channel_object_create()
 */
int tizen_core_channel_object_destroy(tizen_core_channel_object_h object);

/**
 * @brief Sets the ID to the tizen core channel object handle.
 * @since_tizen 9.0
 *
 * @param[in] object The tizen core channel object handle
 * @param[in] id  The channel ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_object_h object = NULL;
  int ret;

  tizen_core_channel_object_create(&object);

  ret = tizen_core_channel_object_set_id(object, 22);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set channel object id");
    tizen_core_channel_object_destroy(object);
    return;
  }

  tizen_core_channel_object_destroy(object);
}
 * @endcode
 *
 * @see tizen_core_channel_object_get_id()
 */
int tizen_core_channel_object_set_id(tizen_core_channel_object_h object,
                                     int id);

/**
 * @brief Gets the ID from the tizen core channel object handle.
 * @since_tizen 9.0
 *
 * @param[in] object The tizen core channel object handle
 * @param[out] id The channel ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_object_h object = NULL;
  int id = 0;
  int ret;

  tizen_core_channel_object_create(&object);
  tizen_core_channel_object_set_id(object, 22);

  ret = tizen_core_channel_object_get_id(object, &id);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get channel object id");
    return;
  }

  tizen_core_channel_object_destroy(object);
}
 * @endcode
 *
 * @see tizen_core_channel_object_set_id()
 */
int tizen_core_channel_object_get_id(tizen_core_channel_object_h object,
                                     int *id);

/**
 * @brief Sets the data to the tizen core channel object handle.
 * @since_tizen 9.0
 * @remarks The @a data should be released using release function
 *          if the @a data is created by the memory allocation.
 *
 * @param[in] object The tizen core channel object handle
 * @param[in] data The channel data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_object_h object = NULL;
  char *data = NULL;
  int ret;

  tizen_core_channel_object_create(&object);

  ret = tizen_core_channel_object_set_data(object, strdup("22"));
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set channel data");
    tizen_core_channel_object_destroy(object);
    return;
  }

  tizen_core_channel_object_get_data(object, &data);
  free(data);
  tizen_core_channel_object_destroy(object);
}
 * @endcode
 *
 * @see tizen_core_channel_object_get_data()
 */
int tizen_core_channel_object_set_data(tizen_core_channel_object_h object,
                                       void *data);

/**
 * @brief Gets the data from the tizen core channel object handle.
 * @since_tizen 9.0
 * @remarks The @a data should be released using release function.
 *
 * @param[in] object The tizen core channel object handle
 * @param[out] data The channel data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_channel_object_h object = NULL;
  char *data = NULL;
  int ret;

  tizen_core_channel_object_create(&object);
  tizen_core_channel_object_set_data(object, strdup("22"));

  ret = tizen_core_channel_object_get_data(object, &data);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get channel data");
    tizen_core_channel_object_destroy(object);
    return;
  }

  dlog_print(DLOG_INFO, LOG_TAG, "data=%s", data);
  free(data);
  tizen_core_channel_object_destroy(object);
}
 * @endcode
 *
 * @see tizen_core_channel_object_set_data()
 */
int tizen_core_channel_object_get_data(tizen_core_channel_object_h object,
                                       void **data);

/**
 * @brief Gets the sender task name from the tizen core channel object handle.
 * @since_tizen 9.0
 * @remarks The @a task_name must not be deallocated by the application.
 *
 * @param[in] object The tizen core channel object handle
 * @param[out] task_name The sender task name
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>

static void print_task_name(tizen_core_channel_object_h object)
{
  const char *task_name = NULL;
  int ret;

  ret = tizen_core_channel_object_get_sender_task_name(object, &task_name);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get sender task name");
    return;
  }

  dlog_print(DLOG_INFO, LOG_TAG, "sender task=%s", task_name");
}
 * @endcode
 *
 * @see tizen_core_channel_sender_send()
 * @see tizen_core_channel_receiver_receive()
 */
int tizen_core_channel_object_get_sender_task_name(
    tizen_core_channel_object_h object, const char **task_name);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_BASE_TIZEN_CORE_CHANNEL_H__ */
