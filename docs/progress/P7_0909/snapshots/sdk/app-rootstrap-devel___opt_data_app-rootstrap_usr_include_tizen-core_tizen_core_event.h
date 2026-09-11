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

#ifndef __TIZEN_BASE_TIZEN_CORE_EVENT_H__
#define __TIZEN_BASE_TIZEN_CORE_EVENT_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file tizen_core_event.h
 */

/**
 * @addtogroup CAPI_TIZEN_CORE_EVENT_MODULE
 * @{
 */

/**
 * @brief The tizen core event handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_event_h;

/**
 * @brief The tizen core event object handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_event_object_h;

/**
 * @brief The tizen core event handler handle.
 * @since_tizen 9.0
 */
typedef void *tizen_core_event_handler_h;

/**
 * @brief Called when the event is emitted.
 * @since_tizen 9.0
 * @remarks The @a object must not be deallocated by the application.
 *
 * @param[in] object The tizen core event object handle
 * @param[in] user_data The user data passed from the callback registration function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @see tizen_core_event_add_handler()
 */
typedef bool (*tizen_core_event_handler_cb)(tizen_core_event_object_h object,
                                            void *user_data);

/**
 * @brief Called when the event object is destroyed.
 * @since_tizen 9.0
 * @remarks The @a event_data should be released using release function if it's needed.
 *
 * @param[in] event_data The event data of the event object
 * @param[in] user_data The user data passed from the callback registration function
 * @see tizen_core_event_object_set_destroy_cb()
 * @see tizen_core_event_object_destroy()
 */
typedef void (*tizen_core_event_object_destroy_cb)(void *event_data,
                                                   void *user_data);

/**
 * @brief Creates the tizen core event handle.
 * @since_tizen 9.0
 * @remarks The @a event should be released using tizen_core_event_destroy().
 *
 * @param[out] event The tizen core event handle
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
  tizen_core_event_h event = NULL;
  int ret;

  ret = tizen_core_event_create(&event);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create event");
    return;
  }

  tizen_core_event_destroy(event);
}
 * @endcode
 *
 * @see tizen_core_event_destroy()
 */
int tizen_core_event_create(tizen_core_event_h *event);

/**
 * @brief Destroys the tizen core event handle.
 * @since_tizen 9.0
 *
 * @param[in] event The tizen core event handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_event_h event = NULL;
  int ret;

  tizen_core_event_create(&event);

  ret = tizen_core_event_destroy(event);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to destroy event");
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_event_create()
 */
int tizen_core_event_destroy(tizen_core_event_h event);

/**
 * @brief Adds the event handler to the tizen core event.
 * @details The @a event_handler is added at the back of the handler list of the tizen core event.
 *          When tizen_core_emit_event() is called, the @a callback function of the @a event_handler is called first.
 * @since_tizen 9.0
 * @remarks The @a event_handler should be released using tizen_core_event_remove_handler().
 *
 * @param[in] event The tizen core event handle
 * @param[in] callback The callback function to be invoked when the event is emitted
 * @param[in] user_data The user data to be passed to the callback function
 * @param[out] event_handler The tizen core event handler handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
static void event_handler_cb(tizen_core_event_object_h object, void *user_data)
{
  int event_id = 0;
  chat *event_data = NULL;

  tizen_core_event_object_get_id(object, &event_id);
  tizen_core_event_object_get_data(object, &event_data);

  dlog_print(DLOG_INFO, LOG_TAG, "id=%d, data=%s", event_id, event_data);
}

static void add_event_handler(tizen_core_event_h event)
{
  tizen_core_event_handler_h event_handler = NULL;
  int ret;

  ret = tizen_core_event_add_handler(event, event_handler_cb, NULL, &event_handler);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add event handler");
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_event_emit()
 * @see tizen_core_emit_event()
 * @see tizen_core_event_remove_handler()
 */
int tizen_core_event_add_handler(
    tizen_core_event_h event, tizen_core_event_handler_cb callback,
    void *user_data, tizen_core_event_handler_h *event_handler);

/**
 * @brief Prepends the event handler to the tizen core event.
 * @details The @a event_handler is added to the front of the handler list of the tizen core event.
 *          When tizen_core_emit_event() is called, the @a callback function of the @a event_handler is called first.
 * @since_tizen 9.0
 * @remarks The @a event_handler should be released using tizen_core_event_remove_handler().
 *
 * @param[in] event The tizen core event handle
 * @param[in] callback The callback function to be invoked when the event is emitted
 * @param[in] user_data The user data to be passed to the callback function
 * @param[out] event_handler The tizen core event handler handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
static void event_handler_cb(tizen_core_event_object_h object, void *user_data)
{
  int event_id = 0;
  chat *event_data = NULL;

  tizen_core_event_object_get_id(object, &event_id);
  tizen_core_event_object_get_data(object, &event_data);

  dlog_print(DLOG_INFO, LOG_TAG, "id=%d, data=%s", event_id, data);
}

static void prepend_event_handler(tizen_core_event_h event)
{
  tizen_core_event_handler_h event_handler = NULL;
  int ret;

  ret = tizen_core_event_prepend_handler(event, event_handler_cb, NULL, &event_handler);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to prepend event handler");
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_event_emit()
 * @see tizen_core_emit_event()
 * @see tizen_core_event_remove_handler()
 */
int tizen_core_event_prepend_handler(
    tizen_core_event_h event, tizen_core_event_handler_cb callback,
    void *user_data, tizen_core_event_handler_h *event_handler);

/**
 * @brief Removes the event handler from the tizen core event.
 * @since_tizen 9.0
 *
 * @param[in] event The tizen core event handle
 * @param[in] event_handler The tizen core event handler handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static void remove_event_handler(tizen_core_event_h event, tizen_core_event_handler_h handler)
{
  int ret;

  ret = tizen_core_event_remove_handler(event, handler);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to remove event handler");
}
 * @endcode
 *
 * @see tizen_core_event_add_handler()
 */
int tizen_core_event_remove_handler(
    tizen_core_event_h event, tizen_core_event_handler_h event_handler);

/**
 * @brief Emits the event to the tizen core event.
 * @details The event is emitted to the tizen core event.
 * @since_tizen 9.0
 * @remarks The @a object will be released automatically.
 *          You MUST NOT release the @a object using tizen_core_event_object_destroy()
 *          when calling the function is successful.
 *          If this function returns an error, the @a object should be released
 *          using tizen_core_event_object_destroy().
 *
 * @param[in] event The tizen core event handle
 * @param[in] object The tizen core event object handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static void object_destroy_cb(void *event_data, void *user_data)
{
  char* value = (char *)event_data;

  if (value)
    free(value);
}

static void emit_event(tizen_core_event_h event)
{
  tizen_core_event_object_h object = NULL;
  char buf[12];
  int event_id = 99;
  int ret;

  snprintf(buf, sizeof(buf), "%d", event_id);
  tizen_core_event_object_create(&object, event_id, strdup(buf));
  tizen_core_event_object_set_destroy_cb(object, object_destroy_cb, NULL);

  ret = tizen_core_event_emit(event, object);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to emit event");
    tizen_core_event_object_destroy(object);
    return;
  }
}
 * @endcode
 *
 * @see tizen_core_event_add_handler()
 */
int tizen_core_event_emit(tizen_core_event_h event,
                          tizen_core_event_object_h object);

/**
 * @brief Creates the tizen core event object handle.
 * @details The @a data can be nullptr.
 *          If the @a data is memory allocated, the @a data can be released
 *          using the tizen_core_event_object_set_destroy_cb() function.
 *          When tizen_core_event_object_destroy() calls, the callback function
 *          set in tizen_core_event_object_set_destroy_cb() is called.
 * @since_tizen 9.0
 * @remarks The @a object should be released using tizen_core_event_object_destroy().
 * @remarks The @a object should not be released when the @a object is emitted
 *          using tizen_core_event_emit().
 *
 * @param[out] object The tizen core event object handle
 * @param[in] id The event ID
 * @param[in] data The event data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT Invalid context
 *
 * @code
#include <tizen_core.h>

static void object_destroy_cb(void *event_data, void *user_data)
{
  char *value = (char *)event_data;

  if (value)
    free(value);
}

static tizen_core_event_object_h create_event_object(void)
{
  tizen_core_event_object_h object = NULL;
  char buf[12];
  int event_id = 99;
  int ret;

  snprintf(buf, sizeof(buf), "%d", event_id);
  ret = tizen_core_event_object_create(&object, event_id, strdup(buf));
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create event object");
    return NULL;
  }

  tizen_core_event_object_set_destroy_cb(object, object_destroy_cb, NULL);

  return object;
}
 * @endcode
 *
 * @see tizen_core_event_object_set_destroy_cb()
 * @see tizen_core_event_object_destroy()
 * @see tizen_core_event_emit()
 */
int tizen_core_event_object_create(tizen_core_event_object_h *object,
                                   int id, void *data);

/**
 * @brief Destroys the tizen core event object handle.
 * @since_tizen 9.0
 *
 * @param[in] object The tizen core event object handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_event_object_h object = NULL;
  int event_id = 99;
  int ret;

  tizen_core_event_object_create(&object, event_id, NULL);

  ret = tizen_core_event_object_destroy(object);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to destroy event object");
}
 * @endcode
 *
 * @see tizen_core_event_object_create()
 * @see tizen_core_event_object_set_destroy_cb()
 */
int tizen_core_event_object_destroy(tizen_core_event_object_h object);

/**
 * @brief Sets the destroy callback function to invoke when the event object is destroyed.
 * @since_tizen 9.0
 *
 * @param[in] object The tizen core event object handle
 * @param[in] callback The callback function to be invoked when the event object is destroyed
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static void object_destroy_cb(void *data, void *user_data)
{
  char *str = (char *)data;

  if (str)
    free(str);
}

static tizen_core_event_object_h create_event_object(int id, char *str)
{
  tizen_core_event_object_h object = NULL;
  int ret;

  tizen_core_event_object_create(&object, id, str);

  ret = tizen_core_event_object_set_destroy_cb(object, object_destroy_cb, NULL);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to set on_destroy callback");
    tizen_core_event_object_destroy(object);
    return NULL;
  }

  return object;
}
 * @endcode
 *
 * @see tizen_core_event_object_destroy()
 */
int tizen_core_event_object_set_destroy_cb(
    tizen_core_event_object_h object,
    tizen_core_event_object_destroy_cb callback, void *user_data);

/**
 * @brief Gets the event ID from the tizen core event object handle.
 * @since_tizen 9.0
 *
 * @param[in] object The tizen core event object handle
 * @param[out] id The event ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_event_object_h object = NULL;
  int event_id;
  int ret;

  tizen_core_event_object_create(&object, 22, NULL);

  ret = tizen_core_event_object_get_id(object, &event_id);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get event id");

  tizen_core_event_object_destroy(object);
}
 * @endcode
 *
 * @see tizen_core_event_object_create()
 */
int tizen_core_event_object_get_id(tizen_core_event_object_h object, int *id);

/**
 * @brief Gets the event data from the tizen core event object handle.
 * @since_tizen 9.0
 * @remarks The @a data should not be released if the @a object is emitted
 *          using tizen_core_event_emit().
 * @remarks The @a data is available until the @a object is released.
 *
 * @param[in] object The tizen core event object handle
 * @param[out] data The event data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
static void event_object_destroy_cb(void *event_data, void *user_data)
{
  char *value = (char *)event_data;

  if (value)
    free(value);
}

static void create_and_destroy_event_object(void)
{
  tizen_core_event_object_h object = NULL;
  char *event_data = strdup("22");
  char *value = NULL;
  int ret;

  tizen_core_event_object_create(&object, 22, event_data);
  tizen_core_event_object_set_destroy_cb(object, event_object_destroy_cb, NULL);

  ret = tizen_core_event_object_get_data(object, &value);
  if (ret != TIZEN_CORE_ERROR_NONE)
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get event data");

  tizen_core_event_object_destroy(object);
}
 * @endcode
 */
int tizen_core_event_object_get_data(tizen_core_event_object_h object,
                                     void **data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_BASE_TIZEN_CORE_EVENT_H__ */
