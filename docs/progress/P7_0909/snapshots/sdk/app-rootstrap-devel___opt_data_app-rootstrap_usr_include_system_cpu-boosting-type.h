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

#ifndef __TIZEN_SYSTEM_CPU_BOOSTING_TYPE_H__
#define __TIZEN_SYSTEM_CPU_BOOSTING_TYPE_H__

#include <sys/types.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration for cpu boosting error.
 * @since_tizen 6.5
 */
typedef enum {
	RESOURCE_ERROR_NONE              = TIZEN_ERROR_NONE,               /**< Success */
	RESOURCE_ERROR_NOT_SUPPORTED     = TIZEN_ERROR_INVALID_OPERATION,  /**< Not supported */
	RESOURCE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,  /**< Invalid parameter */
	RESOURCE_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,  /**< Permission denied */
	RESOURCE_ERROR_OPERATION_FAILED  = TIZEN_ERROR_IO_ERROR,           /**< Operation failed */
	RESOURCE_ERROR_OUT_OF_MEMORY     = TIZEN_ERROR_OUT_OF_MEMORY,      /**< Out of memory */
	RESOURCE_ERROR_NO_SUCH_FILE      = TIZEN_ERROR_NO_SUCH_FILE        /**< No such file */
} resource_error_e;

/**
 * @brief Enumeration for cpu boosting flag.
 * @since_tizen 6.5
 */
typedef enum resource_cpu_boosting_flag {
	CPU_BOOSTING_RESET_ON_FORK = 0x01,              /**< CPU RESET ON FORK */
	CPU_BOOSTING_FORCE_RESET_ON_SET = 0x02,		/**< CPU FORCE RESET ON SET */
} cpu_boosting_flag_e;

/**
 * @brief Enumeration for cpu boosting level.
 * @since_tizen 6.5
 */
typedef enum resource_cpu_boosting_level {
	CPU_BOOSTING_LEVEL_NONE = 0,                    /**< CPU BOOSTING LEVEL NONE */
	CPU_BOOSTING_LEVEL_STRONG = 1,                  /**< CPU BOOSTING LEVEL STRONG */
	CPU_BOOSTING_LEVEL_MEDIUM,                      /**< CPU BOOSTING LEVEL MEDIUM */
	CPU_BOOSTING_LEVEL_WEAK,                        /**< CPU BOOSTING LEVEL WEAK */
	CPU_BOOSTING_LEVEL_COLDBOOT,			/**< CPU BOOSTING LEVEL COLDBOOT */
	CPU_BOOSTING_LEVEL_END,                         /**< CPU BOOSTING LEVEL END */
} cpu_boosting_level_e;

/**
 * @brief Structure for process and thread group.
 * @since_tizen 6.5
 */
typedef struct resource_pid_info {
	pid_t pid;                                      /**< process id */
	pid_t *tid;                                     /**< thread group */
	int tid_count;                                  /**< the number of thread */
} resource_pid_t;

/**
 * @brief Structure for level of thread group.
 * @since_tizen 6.5
 */
typedef struct resource_cpu_boosting_level_info {
	int *tid_level;                                 /**< levels of thread group */
	int tid_count;                                  /**< the number of thread */
} cpu_boosting_level_info_t;

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_SYSTEM_CPU_BOOSTING_TYPE_H__ */
