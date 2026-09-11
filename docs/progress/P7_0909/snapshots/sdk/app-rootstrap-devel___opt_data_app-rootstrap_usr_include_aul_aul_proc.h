/*
 * Copyright (c) 2000 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __AUL_PROC_H__
#define __AUL_PROC_H__

#include <sys/types.h>
#include <unistd.h>

#include <aul.h>
#include <bundle.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the user ID of the process from the proc filesystem.
 * @since_tizen 6.5
 * @param[in]   pid             The process ID
 * @param[out]  uid             The user ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This function is only for App Framework internally.
 */
int aul_proc_get_uid(pid_t pid, uid_t *uid);

/**
 * @brief Gets the cmdline of the process from the proc filesystem.
 * @since_tizen 6.5
 * @param[in]   pid             The process ID
 * @param[in]   buf             The buffer
 * @param[in]   buf_size        The size of the buffer
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This function is only for App Framework internally.
 */
int aul_proc_get_cmdline(pid_t pid, char *buf, size_t buf_size);

/**
 * @brief Registers the process information,
 * @details This function registers the process information to the application manager daemon.
 *          After calling this function, the other process can get the process information
 *          using aul_proc_get_name() or aul_proc_get_extra().
 * @since_tizen 6.5
 * @param[in]   name            The process name
 * @param[in]   extra           The extra data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This function is only for App Framework internally.
 */
int aul_proc_register(const char *name, bundle *extra);

/**
 * @brief Deregister the process information.
 * @since_tizen 6.5
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This function is only for App Framework internally.
 */
int aul_proc_deregister(void);

/**
 * @brief Gets the process name.
 * @since_tizen 6.5
 * @remarks The @a name MUST be released using free().
 * @param[in]   pid             The process ID
 * @param[out]  name            The process name
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This function is only for App Framework internally.
 */
int aul_proc_get_name(pid_t pid, char **name);

/**
 * @brief Gets the extra data of the process.
 * @since_tizen 6.5
 * @reamrks The @a extra MUST be released using bundle_free().
 * @param[in]   pid             The process ID
 * @param[out]  extra           The extra data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This function is only for App Framework internally.
 */
int aul_proc_get_extra(pid_t pid, bundle **extra);

#ifdef __cplusplus
}
#endif

#endif /* __AUL_PROC_H__ */
