/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_RUNTIME_INFO_INTERNAL_H__
#define __TIZEN_SYSTEM_RUNTIME_INFO_INTERNAL_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    RUNTIME_INFO_PROC_MEMORY_VSZ           = 100,  /**< Virtual memory size (KiB) */
    RUNTIME_INFO_PROC_MEMORY_RSS           = 200,  /**< Resident set size (KiB) */
    RUNTIME_INFO_PROC_MEMORY_PSS           = 300,  /**< Proportional set size (KiB) */
    RUNTIME_INFO_PROC_MEMORY_SHARED_CLEAN  = 400,  /**< Not modified and mapped by other processes (KiB) */
    RUNTIME_INFO_PROC_MEMORY_SHARED_DIRTY  = 500,  /**< Modified and mapped by other processes (KiB) */
    RUNTIME_INFO_PROC_MEMORY_PRIVATE_CLEAN = 600,  /**< Not modified and available only to that process (KiB) */
    RUNTIME_INFO_PROC_MEMORY_PRIVATE_DIRTY = 700,  /**< Modified and available only to that process (KiB) */
    RUNTIME_INFO_PROC_MEMORY_SWAP          = 800,  /**< SWAP memory size (KiB) */
    RUNTIME_INFO_PROC_MEMORY_GPU           = 900,  /**< GPU memory size (KiB) */
    RUNTIME_INFO_PROC_MEMORY_GEM_RSS       = 1000, /**< Resident set size in graphic execution manager (KiB) */

} process_memory_info_key_e;

int runtime_info_get_process_memory_value_int(int *pid, int size, process_memory_info_key_e key, int **info);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_RUNTIME_INFO_INTERNAL_H__ */
