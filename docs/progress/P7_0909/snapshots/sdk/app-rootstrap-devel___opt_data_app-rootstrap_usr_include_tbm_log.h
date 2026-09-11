/**************************************************************************

libtbm

Copyright 2018 Samsung Electronics co., Ltd. All Rights Reserved.

Contact: SooChan Lim <sc1.lim@samsung.com>, Sangjin Lee <lsj119@samsung.com>
Boram Park <boram1288.park@samsung.com>, Changyeon Lee <cyeon.lee@samsung.com>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

#ifndef _TBM_LOG_H_
#define _TBM_LOG_H_

#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file tbm_log.h
 * @brief The header file to print logs in frontend and backend modules
 * @details
 */

enum {
	TBM_LOG_LEVEL_NONE,
	TBM_LOG_LEVEL_ERR,
	TBM_LOG_LEVEL_WRN,
	TBM_LOG_LEVEL_INFO,
	TBM_LOG_LEVEL_DBG,
};

void tbm_log_enable_color(unsigned int enable);
void tbm_log_enable_dlog(unsigned int enable);
void tbm_log_set_debug_level(int level);
void tbm_log_set_assert_level(int level);
void tbm_log_set_path(const char *path);
void tbm_log_print(int level, const char *fmt, ...);
void tbm_log_print_stdout(int level, const char *fmt, ...);

#define TBM_DBG(fmt, args...) \
	do { \
		struct timespec ts; \
		clock_gettime(CLOCK_MONOTONIC, &ts); \
		tbm_log_print(TBM_LOG_LEVEL_DBG, "[%5d.%06d][%d][%s %d]" fmt, \
					  (int)ts.tv_sec, (int)ts.tv_nsec / 1000, \
					  (int)syscall(SYS_gettid), __FUNCTION__, __LINE__, ##args);  \
	} while (0)
#define TBM_INFO(fmt, args...) \
	do { \
		struct timespec ts; \
		clock_gettime(CLOCK_MONOTONIC, &ts); \
		tbm_log_print(TBM_LOG_LEVEL_INFO, "[%5d.%06d][%d][%s %d]" fmt, \
					  (int)ts.tv_sec, (int)ts.tv_nsec / 1000, \
					  (int)syscall(SYS_gettid), __FUNCTION__, __LINE__, ##args);  \
	} while (0)
#define TBM_WRN(fmt, args...) \
	do { \
		struct timespec ts; \
		clock_gettime(CLOCK_MONOTONIC, &ts); \
		tbm_log_print(TBM_LOG_LEVEL_WRN, "[%5d.%06d][%d][%s %d]" fmt, \
					  (int)ts.tv_sec, (int)ts.tv_nsec / 1000, \
					  (int)syscall(SYS_gettid), __FUNCTION__, __LINE__, ##args);  \
	} while (0)
#define TBM_ERR(fmt, args...) \
	do { \
		struct timespec ts; \
		clock_gettime(CLOCK_MONOTONIC, &ts); \
		tbm_log_print(TBM_LOG_LEVEL_ERR, "[%5d.%06d][%d][%s %d]" fmt, \
					  (int)ts.tv_sec, (int)ts.tv_nsec / 1000, \
					  (int)syscall(SYS_gettid), __FUNCTION__, __LINE__, ##args);  \
	} while (0)

#define TBM_STDOUT_INFO(fmt, args...) \
	do { \
		struct timespec ts; \
		clock_gettime(CLOCK_MONOTONIC, &ts); \
		tbm_log_print_stdout(TBM_LOG_LEVEL_INFO, "[%5d.%06d][%d][%s %d]" fmt, \
					  (int)ts.tv_sec, (int)ts.tv_nsec / 1000, \
					  (int)syscall(SYS_gettid), __FUNCTION__, __LINE__, ##args);  \
	} while (0)

#define TBM_NEVER_GET_HERE() TBM_WRN("** NEVER GET HERE **")
#define TBM_DEPRECATED(str) \
	do { \
		if (str) \
			TBM_WRN("** DEPRECATED: %s **", str); \
		else \
			TBM_WRN("** DEPRECATED **"); \
	} while(0)

/* check condition */
#define TBM_RETURN_IF_FAIL(cond) {\
	if (!(cond)) {\
		TBM_ERR("'%s' failed.", #cond);\
		return;\
	} \
}
#define TBM_RETURN_VAL_IF_FAIL(cond, val) {\
	if (!(cond)) {\
		TBM_ERR("'%s' failed.", #cond);\
		return val;\
	} \
}
#define TBM_GOTO_VAL_IF_FAIL(cond, val) {\
	if (!(cond)) {\
		TBM_ERR("'%s' failed.", #cond);\
		goto val;\
	} \
}

#define TBM_RETURN_SET_ERR_IF_FAIL(cond, error, error_type) {\
	if (!(cond)) {\
		TBM_ERR("'%s' failed.", #cond);\
		error = error_type;\
		return;\
	} \
}

#define TBM_RETURN_VAL_SET_ERR_IF_FAIL(cond, val, error, error_type) {\
	if (!(cond)) {\
		TBM_ERR("'%s' failed.", #cond);\
		error = error_type;\
		return val;\
	} \
}

#define TBM_GOTO_VAL_SET_ERR_IF_FAIL(cond, val, error, error_type) {\
	if (!(cond)) {\
		TBM_ERR("'%s' failed.", #cond);\
		error = error_type;\
		goto val;\
	} \
}

#ifdef __cplusplus
}
#endif

#endif /* _TBM_LOG_H_ */
