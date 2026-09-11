/* MIT License
 *
 * Copyright (c) 2022 Samsung Electronics Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. */

#ifndef __TIZEN_SYSTEM_CPU_BOOSTING_H__
#define __TIZEN_SYSTEM_CPU_BOOSTING_H__

#include "cpu-boosting-type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @partner
 * @brief Sets cpu boosting for the target process (pid/tids) to the desired level for as long as required.
 *
 * @details Another set request on the boosted target overrides the previous one.
 * @since_tizen 6.5
 *
 * @privlevel partner
 * @privilege http://tizen.org/privilege/internal/default/partner
 * @remarks An internal API, so it can be used only by partner level developers. \n
 *          In addition, the developers cannot arbitrary choose whether to enforce this privilege or not.
 * @param[in] pid The target process pid/tids \n
 *                If pid.pid is nonzero, pid.tid and pid.tid_count are ignored, \n
 *                                       and cpu boosting is applied for all the threads of pid.pid. \n
 *                If pid.pid is zero and pid.tid is not NULL, all tids in pid.tid are used. \n
 *                If pid.pid is zero and pid.tid is NULL, tid of the calling thread is used. \n
 *                The caller must (de)allocate the buffer in the pid.tid pointer.
 * @param[in] level The cpu boosting level
 * @param[in] flags The cpu boosting flag bits
 *                 If #CPU_BOOSTING_RESET_ON_FORK is set, child processes or threads created by fork() or pthread_create()
 *                 do not inherit boosted CPU schedulder and priority from the parent.
 *                 If #CPU_BOOSTING_FORCE_RESET_ON_SET is set,
 *                 reset CPU boosting forcely on resource_set_cpu_boosting() regardless of duplicate CPU boosting sets.
 * @param[in] timeout_msec The timeout in milliseconds, -1 to apply boosting permanently
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #RESOURCE_ERROR_NONE                         Successful
 * @retval #RESOURCE_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @see    resource_clear_cpu_boosting()
 * @see    resource_get_cpu_boosting_level()
 * @code
 *      resource_pid_t pid;
 *      cpu_boosting_level_e cpu_boosting_level = CPU_BOOSTING_LEVEL_STRONG;
 *      cpu_boosting_flag_e flags = 0;
 *      int timeout_msec = -1;
 *      int ret;
 *
 *      pid.pid = 0;
 *      pid.tid = NULL;
 *      pid.tid_count = 0;
 *
 *      // Set CPU boosting for the target process
 *      ret = resource_set_cpu_boosting(pid, cpu_boosting_level, flags, timeout_msec);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return ret;
 * @endcode
 */
int resource_set_cpu_boosting (resource_pid_t pid, cpu_boosting_level_e level, cpu_boosting_flag_e flags, int timeout_msec);

/**
 * @partner
 * @brief Clears cpu boosting for the boosted process (pid/tids).
 *
 * @details The default non-boosting configuration (SCHED_OTHER and nice=0) is applied.
 * @since_tizen 6.5
 *
 * @privlevel partner
 * @privilege http://tizen.org/privilege/internal/default/partner
 * @remarks An internal API, so it can be used only by partner level developers. \n
 *          In addition, the developers cannot arbitrary choose whether to enforce this privilege or not.
 * @param[in] pid The target process pid/tids \n
 *                If pid.pid is nonzero, pid.tid and pid.tid_count are ignored, \n
 *                                       and cpu boosting is cleared for all the threads of pid.pid. \n
 *                If pid.pid is zero and pid.tid is not NULL, all tids in pid.tid are used. \n
 *                If pid.pid is zero and pid.tid is NULL, tid of the calling thread is used. \n
 *                The caller must (de)allocate the buffer in the pid.tid pointer.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #RESOURCE_ERROR_NONE                         Successful
 * @retval #RESOURCE_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @see    resource_set_cpu_boosting()
 * @see    resource_get_cpu_boosting_level()
 * @code
 *      resource_pid_t pid;
 *      cpu_boosting_level_e cpu_boosting_level = CPU_BOOSTING_LEVEL_STRONG;
 *      cpu_boosting_flag_e flags = 0;
 *      int timeout_msec = -1;
 *      int ret;
 *
 *      pid.pid = 0;
 *      pid.tid = NULL;
 *      pid.tid_count = 0;
 *
 *      // Set CPU boosting for the target process
 *      ret = resource_set_cpu_boosting(pid, cpu_boosting_level, flags, timeout_msec);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return ret;
 *
 *      // Clear CPU boosting for the target process
 *      ret = resource_clear_cpu_boosting(pid);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return ret;
 * @endcode
 */
int resource_clear_cpu_boosting (resource_pid_t pid);

/**
 * @partner
 * @brief Gets the cpu boosting level for the target process (pid/tids).
 *
 * @details Obtains a list of cpu boosting level and the number of threads.
 * @since_tizen 6.5
 *
 * @privlevel partner
 * @privilege http://tizen.org/privilege/internal/default/partner
 * @remarks An internal API, so it can be used only by partner level developers. \n
 *          In addition, the developers cannot arbitrary choose whether to enforce this privilege or not.
 * @param[in] pid The target process pid/tids \n
 *                If pid.pid is nonzero, pid.tid and pid.tid_count are ignored, \n
 *                                       and all the threads of pid.pid are checked. \n
 *                If pid.pid is zero and pid.tid is not NULL, all tids in pid.tid are checked. \n
 *                If pid.pid is zero and pid.tid is NULL, the calling thread is checked. \n
 *                The caller must (de)allocate the buffer in the pid.tid pointer.
 * @param[out] level The boosting level for the target process (pid/tids) \n
 *                   level->tid_level will be allocated and filled with #cpu_boosting_level_e for tids requested by the pid argument. \n
 *                   The caller must deallocate the buffer in the level->tid_level pointer after using it.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #RESOURCE_ERROR_NONE                         Successful
 * @retval #RESOURCE_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @see    resource_set_cpu_boosting()
 * @see    resource_clear_cpu_boosting()
 * @code
 *      resource_pid_t pid;
 *      cpu_boosting_level_info_t cur_level_info;
 *      int ret;
 *
 *      pid.pid = 0;
 *      pid.tid = NULL;
 *      pid.tid_count = 0;
 *
 *      // Get CPU boosting level of the target process
 *      ret = resource_get_cpu_boosting_level(pid, &cur_level_info);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return ret;
 *
 *      // Free the cpu boosting level list of threads
 *      if (cur_level_info.tid_level)
 *              free(cur_level_info.tid_level);
 * @endcode
 */
int resource_get_cpu_boosting_level (resource_pid_t pid, cpu_boosting_level_info_t *level);

/**
 * @partner
 * @brief Sets cpu resource inheritance from the source tid to the destination process (pid/tids).
 *
 * @details The cpu boosting level of the destination process becomes equal to the source cpu boosting level.
 * @since_tizen 6.5
 *
 * @privlevel partner
 * @privilege http://tizen.org/privilege/internal/default/partner
 * @remarks An internal API, so it can be used only by partner level developers. \n
 *          In addition, the developers cannot arbitrary choose whether to enforce this privilege or not. \n
 *          It should be called from source after
 *          calling the resource_register_cpu_inheritance_destination() function in the destination.
 * @param[in] source_tid The caller thread tid
 * @param[in] dest_process The name of destination process
 * @param[in] timeout_msec The timeout in milliseconds, -1 to apply inheritance permanently
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #RESOURCE_ERROR_NONE                         Successful
 * @retval #RESOURCE_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @see    resource_clear_cpu_inheritance()
 * @see    resource_register_cpu_inheritance_destination()
 * @see    resource_unregister_cpu_inheritance_destination()
 * @code
 *      // Executed in the source side
 *      // Set cpu resource inheritance
 *      int ret = resource_set_cpu_inheritance(gettid(), "test-service", -1);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return ret;
 * @endcode
 */
int resource_set_cpu_inheritance (pid_t source_tid, const char *dest_process, int timeout_msec);

/**
 * @partner
 * @brief Clears cpu resource inheritance from the source tid to the destination process (pid/tids).
 *
 * @details Terminates cpu boosting of the destination process, \n
 *          which has already been boosted due to cpu resource inheritance.
 * @since_tizen 6.5
 *
 * @privlevel partner
 * @privilege http://tizen.org/privilege/internal/default/partner
 * @remarks An internal API, so it can be used only by partner level developers. \n
 *          In addition, the developers cannot arbitrary choose whether to enforce this privilege or not. \n
 *          It should be called from source after
 *          calling the resource_register_cpu_inheritance_destination() function in the destination.
 * @param[in] source_tid The caller thread's tid
 * @param[in] dest_process The name of destination process
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #RESOURCE_ERROR_NONE                         Successful
 * @retval #RESOURCE_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @see    resource_set_cpu_inheritance()
 * @see    resource_register_cpu_inheritance_destination()
 * @see    resource_unregister_cpu_inheritance_destination()
 * @code
 *      // Executed in the source side
 *      // Set cpu resource inheritance
 *      int ret = resource_set_cpu_inheritance(gettid(), "test-service", -1);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return ret;
 *
 *      // Clear cpu resource inheritance
 *      ret = resource_clear_cpu_inheritance(gettid(), "test-service");
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return ret;
 * @endcode
 */
int resource_clear_cpu_inheritance (pid_t source_tid, const char *dest_process);

/**
 * @partner
 * @brief Registers a destination process (pid/tids) for cpu resource inheritance.
 *
 * @details Registers the target process for CPU boosting through CPU resource inheritance.
 * @since_tizen 6.5
 *
 * @privlevel partner
 * @privilege http://tizen.org/privilege/internal/default/partner
 * @remarks An internal API, so it can be used only by partner level developers. \n
 *          In addition, the developers cannot arbitrary choose whether to enforce this privilege or not.
 * @param[in] dest_process The name of destination process
 * @param[in] pid The destination process pid/tids \n
 *                If pid.pid is nonzero, pid.tid and pid.tid_count are ignored, \n
 *                                       and all the threads of pid.pid are registered. \n
 *                If pid.pid is zero and pid.tid is not NULL, all tids in pid.tid are registered. \n
 *                If pid.pid is zero and pid.tid is NULL, tid of the calling thread is registered. \n
 *                The caller must (de)allocate the buffer in the pid.tid pointer.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #RESOURCE_ERROR_NONE                         Successful
 * @retval #RESOURCE_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @see    resource_set_cpu_inheritance()
 * @see    resource_clear_cpu_inheritance()
 * @see    resource_unregister_cpu_inheritance_destination()
 * @code
 *      int ret;
 *      resource_pid_t pid;
 *      pid.pid = 0;
 *      pid.tid = NULL;
 *      pid.tid_count = 0;
 *
 *      // Executed in the destination side
 *      // Register destination process for cpu resource inheritance (called in destination)
 *      ret = resource_register_cpu_inheritance_destination("test-service", pid);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return;
 * @endcode
 */
int resource_register_cpu_inheritance_destination (const char *dest_process, resource_pid_t pid);

/**
 * @partner
 * @brief Unregisters a destination process for cpu resource inheritance.
 *
 * @details Prevents the destination process from receiving further cpu resource inheritance.
 * @since_tizen 6.5
 *
 * @privlevel partner
 * @privilege http://tizen.org/privilege/internal/default/partner
 * @remarks An internal API, so it can be used only by partner level developers. \n
 *          In addition, the developers cannot arbitrary choose whether to enforce this privilege or not.
 * @param[in] dest_process The name of destination process
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #RESOURCE_ERROR_NONE                         Successful
 * @retval #RESOURCE_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @see    resource_set_cpu_inheritance()
 * @see    resource_clear_cpu_inheritance()
 * @see    resource_register_cpu_inheritance_destination()
 * @code
 *      int ret;
 *      resource_pid_t pid;
 *      pid.pid = 0;
 *      pid.tid = NULL;
 *      pid.tid_count = 0;
 *
 *      // Executed in the destination side
 *      // Register destination process for cpu resource inheritance (called in destination)
 *      ret = resource_register_cpu_inheritance_destination("test-service", pid);
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return;
 *
 *      // Unregister destination process for cpu resource inheritance
 *      ret = resource_unregister_cpu_inheritance_destination("test-service");
 *      if (ret != RESOURCE_ERROR_NONE)
 *              return;
 * @endcode
 */
int resource_unregister_cpu_inheritance_destination (const char *dest_process);


#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_SYSTEM_CPU_BOOSTING_H__ */
