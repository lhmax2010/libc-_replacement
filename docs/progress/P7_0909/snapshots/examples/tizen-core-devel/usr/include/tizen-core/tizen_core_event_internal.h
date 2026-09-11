/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_BASE_TIZEN_CORE_EVENT_INTERNAL_H__
#define __TIZEN_BASE_TIZEN_CORE_EVENT_INTERNAL_H__

#include <tizen_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generates the event ID.
 * @since_tizen 9.0
 *
 * @return @c the generated event ID
 *
 * @code
#include <tizen_core.h>
{
  tizen_core_event_object_h object = NULL;
  int id;

  id = tizen_core_event_generate_id();
  tizen_core_event_object_create(&object, id, NULL);
}
 * @endcode
 */
int tizen_core_event_generate_id(void);

/**
 * @brief Called when an event object needs to be filtered.
 * @since_tizen 11.0
 * @details This callback is invoked for each event before it is passed to
handlers.
 *          Return true to allow the event to be processed by handlers,
 *          or false to block the event from being processed.
 *
 * @param[in] obj The event object to be filtered
 * @param[in] data User data passed to tizen_core_event_add_filter()
 *
 * @return @c true to allow the event to be processed, @c false to block it
 *
 * @code
#include <tizen_core.h>
{
  bool my_filter(tizen_core_event_object_h obj, void* data) {
    int id;
    tizen_core_event_object_get_id(obj, &id);

    // Only process events with ID 100 or higher
    return id >= 100;
  }

  tizen_core_event_filter_h filter;
  tizen_core_event_add_filter(event, my_filter, NULL, &filter);
}
 * @endcode
 */
typedef bool (*tizen_core_event_filter_cb)(tizen_core_event_object_h obj,
                                           void *data);

/**
 * @brief The event filter handle
 * @since_tizen 11.0
 * @details This handle is returned by tizen_core_event_add_filter() and used
 *          with tizen_core_event_remove_filter() to remove the filter.
 */
typedef void *tizen_core_event_filter_h;

/**
 * @brief Creates an event filter for the specified event.
 * @since_tizen 11.0
 * @details This function creates an event filter that is invoked before any
 *          handler processes the event. The filter callback can examine the
 *          event object and decide whether to allow or block the event.
 *          Multiple filters can be added to an event, and they are invoked
 *          in the order they were added. If any filter returns false, the
 *          event is not processed by any handler.
 *
 * @param[in] event The event handle to add the filter to
 * @param[in] filter The filter callback function
 * @param[in] data User data to pass to the filter callback
 * @param[out] handle The newly created filter handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
{
  bool my_filter(tizen_core_event_object_h obj, void* data) {
    return true;  // Allow all events
  }

  tizen_core_event_filter_h filter;
  int ret = tizen_core_event_add_filter(event, my_filter, NULL, &filter);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    // Handle error
  }
}
 * @endcode
 */
int tizen_core_event_add_filter(tizen_core_event_h event,
                                tizen_core_event_filter_cb filter, void *data,
                                tizen_core_event_filter_h *handle);

/**
 * @brief Removes an event filter.
 * @since_tizen 11.0
 * @details This function removes the specified event filter from the event.
 *          After removal, the filter handle becomes invalid and should
 *          not be used anymore.
 *
 * @param[in] event The event handle the filter belongs to
 * @param[in] handler The filter handle to remove
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
#include <tizen_core.h>
{
  int ret = tizen_core_event_remove_filter(event, filter);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    // Handle error
  }
}
 * @endcode
 */
int tizen_core_event_remove_filter(tizen_core_event_h event,
                                   tizen_core_event_filter_h handler);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_BASE_TIZEN_CORE_EVENT_INTERNAL_H__ */
