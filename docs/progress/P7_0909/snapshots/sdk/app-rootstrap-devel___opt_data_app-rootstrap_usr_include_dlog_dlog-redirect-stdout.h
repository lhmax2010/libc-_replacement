/* MIT License
 *
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Open an unstructured connection to a DLog logging sink
 * @details Opens a connection to which you can stream data.
 *          DLog will automatically convert it to individual logs appropriately.
 * @remarks Useful for redirecting stdout to DLog
 * @param[in] buffer Target DLog buffer
 * @param[in] fileno File descriptor to use for the connection
 * @param[in] tag DLog tag for the resulting logs
 * @param[in] prio Priority for the resulting logs
 * @return An error code (negative errno)
 * @retval 0 Success
 * @retval -EINVAL Buffer invalid or does not allow connections
 * @retval -EPERM Buffer disabled via dlog config
 * @retval -ENOKEY Buffer not configured via dlog config
 * @retval -ENOTSUP Unsupported DLog backend
 * @retval -EINVAL Tag empty or unspecified
 * @retval -EINVAL Priority invalid
 * @retval -EINVAL Fileno invalid
 * @retval Other Any errno returnable by dup2, open, or ioctl syscalls
 */
int dlog_connect_fd(int buffer, int fileno, const char *tag, int prio);

/**
 * @brief Check whether a file descriptor has been opened via DLog
 * @details Use for low-level code that does bulk operations on FDs (for example, close them all)
 *          but still wants to be able to use DLog interfaces afterwards.
 * @bug DLog devices opened manually (i.e. not via library) are still returned as positives.
 *      On the other hand, /dev/null is always returned as a negative even if opened via DLog.
 * @param[in] fd File descriptor number
 * @return bool Whether the descriptor refers to a DLog file
 */
#ifdef __cplusplus
bool
#else
_Bool
#endif
dlog_is_log_fd(int fd);

#ifdef __cplusplus
}
#endif
