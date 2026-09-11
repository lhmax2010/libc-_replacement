/*  MIT License
 *
 * Copyright (c) 2012-2020 Samsung Electronics Co., Ltd
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


/**
 * @file dlog.h
 * @version	0.4
 * @brief This file is the header file of the interface of Dlog.
 * @details Interfaces for DLog, the default system-wide logger for Tizen.
 */


#ifndef	_DLOG_H_
#define	_DLOG_H_

#include <stdarg.h>
#include <string.h>
#include <tizen_error.h>
/*
 * This is for checking dlog_print format string.
 */
#pragma GCC diagnostic warning "-Wformat"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * This is the local tag used for the following simplified
 * logging macros. You can change this preprocessor definition
 * before using the other macros to change the tag.
 */
#ifndef LOG_TAG
#define LOG_TAG NULL
#endif


/**
 * @addtogroup CAPI_SYSTEM_DLOG
 * @{
 *
 */


/**
 * @brief Enumeration for Dlog Errors, returned by API calls.
 * @details Some of DLog API calls can return these to denote what happened.
 * @since_tizen 2.3
 */
typedef enum {
	DLOG_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	DLOG_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	DLOG_ERROR_NOT_PERMITTED = TIZEN_ERROR_NOT_PERMITTED /**< Operation not permitted */
} dlog_error_e;


/**
 * @}
 */


/**
 * @addtogroup CAPI_SYSTEM_DLOG
 * @{
 */


/**
 * @brief Enumeration for log priority values in ascending priority order.
 * @details Priority level controls how important a message is. This is both
 * a way to a developer to gauge it and for tools to filter for it.
 * @since_tizen 2.3
 */
typedef enum {
/** @cond */
	DLOG_UNKNOWN = 0,
	DLOG_DEFAULT, // doesn't seem actually used or translated into any other level
/** @endcond */
	DLOG_VERBOSE, /**< Verbose, use to let users track what's going on with extra detail. Usually you'll want to gate this behind a `--verbose` flag or similar. */
	DLOG_DEBUG,   /**< Debug, meant for tracking execution details during development. Usually gets compiled out for non-debug builds. */
	DLOG_INFO,    /**< Info, for when something remarkable happens during normal operation. */
	DLOG_WARN,    /**< Warning, for suspicious activity or something that may warrant attention that isn't yet an error. Default filtering passes this level and above through. */
	DLOG_ERROR,   /**< Error, use when something fails. */
	DLOG_FATAL,   /**< Fatal, when an error is hard enough not to be recoverable. Depending on system configuration, dlog can automatically kill the program. */
	DLOG_SILENT,  /**< Silent, for use in filtering tools to "silence" logs, by being higher than the highest "real" level. */
/** @cond */
	DLOG_PRIO_MAX
/** @endcond */
} log_priority;

/* TODO: developers notoriously misuse ERROR as a generic "high priority" tag,
 * even for things that aren't actually errors. Consider adding some sort of
 * `DLOG_INFO_HIGH` or something that would retain the benefits of `DLOG_ERROR`,
 * i.e. be high enough to pass default filters and (green?) coloring in dlogutil. */

/**
 * @}
 */


/**
 * @addtogroup CAPI_SYSTEM_DLOG
 * @{
 */


/**
 * @brief Sends a log message with given priority and tag, using printf formatting
 * @details Produces a log and sends it to be stored on the DLog backend for later retrieval via dlogutil CLI tool.
 * @since_tizen 2.3
 * @param[in] prio priority level of type #log_priority
 * @param[in] tag tag - a null-terminated string
 * @param[in] fmt format string - same as printf
 * @return On success, the function returns the number of bytes written.
 *         On error, a negative errno-style error code
 * @retval #DLOG_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DLOG_ERROR_NOT_PERMITTED Operation not permitted
 * @pre none
 * @post none
 * @see dlog_vprint
 *
 * @code
#include<dlog.h>
int main(void)
{
    int integer = 21;
    char string[] = "test dlog";

	dlog_print(DLOG_INFO, "USR_TAG", "test dlog");
	dlog_print(DLOG_INFO, "USR_TAG", "%s, %d", string, integer);
    return 0;
}
 * @endcode
 */
int dlog_print(log_priority prio, const char *tag, const char *fmt, ...)  __attribute__((format(printf, 3, 4)));


/**
 * @brief Sends a log message with given priority and tag label, using variadic args
 * @details Produces a log and sends it to be stored on the DLog backend for later retrieval via dlogutil CLI tool.
 * @since_tizen 2.3
 * @param[in] prio priority level of type #log_priority
 * @param[in] tag tag - a null-terminated string
 * @param[in] fmt format string - same as printf
 * @param[in] ap va_list
 * @return On success, the function returns the number of bytes written.
 *         On error, a negative errno-style error code
 * @retval #DLOG_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DLOG_ERROR_NOT_PERMITTED Operation not permitted
 * @pre none
 * @post none
 * @see dlog_print
 *
 * @code
#include<dlog.h>
void my_debug_print(char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    dlog_vprint(DLOG_INFO, "USR_TAG", format, ap);
    va_end(ap);
}

int main(void)
{
    my_debug_print("%s", "test dlog");
    my_debug_print("%s, %d", "test dlog", 21);
    return 0;
}
 * @endcode
 */
int dlog_vprint(log_priority prio, const char *tag, const char *fmt, va_list ap);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _DLOG_H_*/
