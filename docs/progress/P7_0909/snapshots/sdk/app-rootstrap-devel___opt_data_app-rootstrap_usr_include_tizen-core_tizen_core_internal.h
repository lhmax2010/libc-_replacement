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

#ifndef __TIZEN_BASE_TIZEN_CORE_INTERNAL_H__
#define __TIZEN_BASE_TIZEN_CORE_INTERNAL_H__

#include <tizen_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Internal priority for UI sources.
 * @since_tizen 11.0
 */
#define TIZEN_CORE_PRIORITY_UI \
  ((tizen_core_priority_e)(TIZEN_CORE_PRIORITY_HIGH + 20))

/**
 * @brief Enumeration for the tizen core cpu boosting level.
 * @since_tizen 9.0
 */
typedef enum {
  TIZEN_CORE_CPU_BOOSTING_LEVEL_NONE,    /**< None level */
  TIZEN_CORE_CPU_BOOSTING_LEVEL_STRONG,  /**< Strong level */
  TIZEN_CORE_CPU_BOOSTING_LEVEL_MEDIUM,  /**< Medium level */
  TIZEN_CORE_CPU_BOOSTING_LEVEL_WEAK,    /**< Weak level */
} tizen_core_cpu_boosting_level_e;

/**
 * @brief Gets the glib context from the tizen core handle.
 * @since_tizen 9.0
 *
 * @param[in] core The tizen core handle
 * @return @c the glib context on success,
 *         otherwise a nullptr
 */
void *tizen_core_get_glib_context(tizen_core_h core);

/**
 * @brief Sets cpu boosting of the tizen core.
 * @since_tizen 9.0
 *
 * @param[in] core The tizen core handle
 * @param[in] level The cpu boosting level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT The core is not running
 *
 * @code
#include <tizen_core_internal.h>
{
  tizen_core_h core = NULL;
  int ret;

  ret = tizen_core_find("main", &core);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    return -1;
  }

  ret = tizen_core_set_cpu_boosting(core, TIZEN_CORE_CPU_BOOSTING_LEVEL_STRONG);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    return -1;
  }

  return 0;
}
 * @endcode
 * @see tizen_core_clear_cpu_boosting()
 */
int tizen_core_set_cpu_boosting(tizen_core_h core,
                                tizen_core_cpu_boosting_level_e level);

/**
 * @brief Clears cpu boosting of the tizen core.
 * @since_tizen 9.0
 *
 * @param[in] core The tizen core handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_INVALID_CONTEXT The core is not running
 *
 * @code
#include <tizen_core_internal.h>
{
  tizen_core_h core = NULL;
  int ret;

  ret = tizen_core_find("main", &core);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    return -1;
  }

  ret = tizen_core_clear_cpu_boosting(core);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    return -1;
  }

  return 0;
}
 * @endcode
 * @see tizen_core_set_cpu_boosting()
 */
int tizen_core_clear_cpu_boosting(tizen_core_h core);

/**
 * @brief Adds an idle job with a priority to the tizen core.
 * @details The callback function will be called whenever there are
 *          no higher priority events pending to the tizen core.
 *          The given callback function is called repeatedly until it returns false,
 *          at which point the idler @a source is automatically destroyed and
 *          the callback function will not be called again.
 * @since_tizen 10.0
 * @remarks The @a source should be released using tizen_core_remove_source().
 *
 * @param[in] core The tizen core handle
 * @param[in] priority The priority of the idle source
 * @param[in] callback The callback function to be invoked when the idle job is emitted
 * @param[in] user_data The user data to be passed to the callback function
 * @param[out] source The tizen core source handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #TIZEN_CORE_ERROR_NONE Successful
 * @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @code
#include <tizen_core.h>
static bool idle_cb(void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "idler is invoked");
  return true;
}

static tizen_core_source_h add_idle_job(void)
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_add_idle_job_with_priority(core,
      TIZEN_CORE_PRIORITY_DEFAULT_IDLE, idle_cb, NULL, &source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add idle job");
    return NULL;
  }

  return source;
}
 * @endcode
 *
 * @see tizen_core_remove_source()
 * @see tizen_core_task_cb()
 */
int tizen_core_add_idle_job_with_priority(tizen_core_h core,
                                          tizen_core_priority_e priority,
                                          tizen_core_task_cb callback,
                                          void *user_data,
                                          tizen_core_source_h *source);

/**
* @brief Adds a timer with a priority to the tizen core.
* @details The callback function will be called at regular intervals.
*          The given callback function is called repeatedly until it returns false,
*          at which point the timer @a source is automatically destroyed and
*          the callback function will not be called again.
* @since_tizen 10.0
* @remarks The @a source should be released using tizen_core_remove_source().
*
* @param[in] core The tizen core handle
* @param[in] priority The priority of the timer source
* @param[in] interval The interval of the timer in milliseconds
* @param[in] callback The callback function to be invoked when the timer is expired
* @param[in] user_data The user data to be passed to the callback function
* @param[out] source The tizen core source handle
* @return @c 0 on success,
*         otherwise a negative error value
* @retval #TIZEN_CORE_ERROR_NONE Successful
* @retval #TIZEN_CORE_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #TIZEN_CORE_ERROR_OUT_OF_MEMORY Out of memory
*
* @code
#include <tizen_core.h>

static bool timeout_cb(void *user_data)
{
  dlog_print(DLOG_INFO, LOG_TAG, "timeout cb is invoked");
  return true;
}

static tizen_core_source_h add_timer(unsigned int interval)
{
  tizen_core_source_h source = NULL;
  tizen_core_h core = NULL;
  int ret;

  tizen_core_find("main", &core);
  ret = tizen_core_add_timer_with_priority(core, TIZEN_CORE_PRIORITY_DEFAULT,
      interval, timeout_cb, NULL, &source);
  if (ret != TIZEN_CORE_ERROR_NONE) {
    dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add timer");
    return NULL;
  }

  return source;
}
* @endcode
*
* @see tizen_core_remove_source()
* @see tizen_core_task_cb()
*/
int tizen_core_add_timer_with_priority(
    tizen_core_h core, tizen_core_priority_e priority, unsigned int interval,
    tizen_core_task_cb callback, void *user_data, tizen_core_source_h *source);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_BASE_TIZEN_CORE_INTERNAL_H__ */
