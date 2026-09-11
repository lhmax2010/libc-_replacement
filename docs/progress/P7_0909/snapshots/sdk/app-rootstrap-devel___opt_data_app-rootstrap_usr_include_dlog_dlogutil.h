/* MIT License
 *
 * Copyright (c) 2012-2020 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_SYSTEM_DLOGUTIL_H__
#define __TIZEN_SYSTEM_DLOGUTIL_H__

#include <time.h>
#include <tizen_type.h>
#include <dlog.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SYSTEM_DLOGUTIL
 * @{
 */

/**
 * @brief Enumeration for timestamp-based log sorting orderings
 * @details Log messages get up to 4 different timestamps which don't necessarily produce the same ordering.
 * This enum lets you specify which ordering you want in given context.
 * @since_tizen 6.0
 * @remarks For information on the timestamp documentation, refer to @ref CAPI_SYSTEM_DLOGUTIL_OVERVIEW.
 */
typedef enum {
	DLOGUTIL_SORT_SENT_MONO = 0, /**< Monotonic timestamp applied by the sender */
	DLOGUTIL_SORT_SENT_REAL,     /**< Real-time timestamp applied by the sender */
	DLOGUTIL_SORT_RECV_MONO,     /**< Monotonic timestamp applied by the receiver */
	DLOGUTIL_SORT_RECV_REAL,     /**< Real-time timestamp applied by the receiver */
	DLOGUTIL_SORT_DEFAULT,       /**< The default timestamp of the buffer
	                                  @see dlogutil_buffer_get_default_ts_type() */
} dlogutil_sorting_order_e;

/**
 * @brief A struct containing the metadata and contents for a single dlog entry.
 * @details An individual log, received after all filters were applied.
 * @bug Currently missing a dlogutil_entry_destroy() function. Use regular libc free().
 * @since_tizen 6.0
 */
typedef struct dlogutil_entry dlogutil_entry_s;

/**
 * @brief Destroys the #dlogutil_entry_s struct and cleans up its memory.
 * @since_tizen 9.0
 * @param[in] entry A handle to the entry struct
 * @see dlogutil_get_log()
 */
void dlogutil_entry_destroy(dlogutil_entry_s *entry);

/**
 * @brief Retrieves the TID (thread identificator) of the log sender.
 * @details Retrieves the TID who sent a log, useful for debugging multithreaded programs.
 * @since_tizen 6.0
 * @remarks If LOG_ID_KMSG is used as the buffer, this function will always return
 *          TIZEN_ERROR_NO_DATA. This is because the KMSG buffer contains no TID information.
 * @param[in] entry Log entry
 * @param[out] tid TID of the log sender
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_NO_DATA TID missing or not applicable
 */
int dlogutil_entry_get_tid(const dlogutil_entry_s *entry, pid_t *tid);

/**
 * @brief Retrieves the PID (process identificator) of the log sender.
 * @details Retrieves the PID who sent a log, useful for debugging in a multi-program environment.
 * @since_tizen 6.0
 * @remarks If LOG_ID_KMSG is used as the buffer, this function will always return
 *          TIZEN_ERROR_NO_DATA. This is because the KMSG buffer contains no PID information.
 * @param[in] entry Log entry
 * @param[out] pid PID of the log sender
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_NO_DATA PID missing or not applicable
 */
int dlogutil_entry_get_pid(const dlogutil_entry_s *entry, pid_t *pid);

/**
 * @brief Retrieves the tag (arbitrary label) of the log entry.
 * @details Gets the log's tag, which has two main purposes.
 * One is to identify multiple instances of the same program.
 * The other is to identify subcomponents of a program, e.g. libraries.
 * @since_tizen 6.0
 * @remarks The tag is owned by the entry so do not free nor modify it.
 *       In some rare cases the entry may be malformed and the message may turn out to be unavailable.
 *       In such cases, an empty string may be returned instead.
 * @param[in] entry Log entry
 * @param[out] tag Log tag
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_NO_DATA The tag is missing
 */
int dlogutil_entry_get_tag(const dlogutil_entry_s *entry, const char **tag);

/**
 * @brief Retrieves the message (without any metadata) of the log entry.
 * @details Gets the raw message itself, which is the essence of a log.
 * @since_tizen 6.0
 * @remarks The message is owned by the entry so do not free nor modify it.
 *       In some rare cases the entry may be malformed and the message may turn out to be unavailable.
 *       In such cases, an empty string may be returned instead.
 * @param[in] entry Log entry
 * @param[out] msg Log message
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_NO_DATA The message is missing
 */
int dlogutil_entry_get_message(const dlogutil_entry_s *entry, const char **msg);

/**
 * @brief Retrieves the timestamp of given type from the log entry.
 * @details Gets the timestamp of a log, which lets you know when it was sent.
 * @since_tizen 6.0
 * @remarks The information about timestamp availability can be retrieved using the dlogutil_buffer_check_ts_type_available() function.
 *          For information on the timespec struct, refer to the clock_gettime(2) manpage.
 * @param[in] entry Log entry
 * @param[in] order Which timestamp to extract. #DLOGUTIL_SORT_DEFAULT is NOT allowed currently
 * @param[out] ts Timestamp of the entry
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid value of order
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_NO_DATA The timestamp is missing
 * @see dlogutil_buffer_check_ts_type_available()
 */
int dlogutil_entry_get_timestamp(const dlogutil_entry_s *entry, dlogutil_sorting_order_e order, struct timespec *ts);

/**
 * @brief Retrieves the priority level metadata of the log entry.
 * @details Checks how important the log was. Note that you only receive logs
 * that pass any priority filters you set, so you don't need to worry about chaff.
 * @since_tizen 6.0
 * @param[in] entry Log entry
 * @param[out] prio Log priority
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_NO_DATA The priority is missing
 */
int dlogutil_entry_get_priority(const dlogutil_entry_s *entry, log_priority *prio);

/**
 * @brief A struct containing libdlogutil initialisation configuration.
 * @details This is the handle to a one-time setup you have to do before sending a log retrieval request.
 * @since_tizen 6.0
 */
typedef struct dlogutil_config dlogutil_config_s;

/**
 * @brief Creates a new #dlogutil_config_s struct to be filled with configuration.
 * @details Creates a config to be filled in, this is the first function you should call.
 * @since_tizen 6.0
 * @remarks Needs to be freed using dlogutil_config_destroy().
 * @return A handle to the new struct, or NULL if out of memory
 * @see dlogutil_config_destroy()
 */
dlogutil_config_s *dlogutil_config_create(void);

/**
 * @brief Destroys the #dlogutil_config_s struct and cleans up its memory.
 * @details Frees resources of a config. Call it after you've connected and received a dlogutil state.
 * @since_tizen 6.0
 * @param[in] config A handle to the settings struct
 * @see dlogutil_config_create()
 */
void dlogutil_config_destroy(dlogutil_config_s *config);

/**
 * @brief Enables retrieving only those logs that are logged by the thread with the given TID.
 * @details Filters by TID. Useful for dumping the logs of a specific thread in a multithreaded process.
 * @since_tizen 6.0
 * @param[in] config A handle to the settings struct
 * @param[in] tid The TID value
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @retval TIZEN_ERROR_OUT_OF_MEMORY Out of memory
 */
int dlogutil_config_filter_tid(dlogutil_config_s *config, pid_t tid);

/**
 * @brief Enables retrieving only those logs that are logged by the process with the given PID.
 * @details Filters by PID. Useful for dumping the logs of a specific process.
 * @since_tizen 6.0
 * @param[in] config A handle to the settings struct.
 * @param[in] pid The PID value.
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @retval TIZEN_ERROR_OUT_OF_MEMORY Out of memory
 */
int dlogutil_config_filter_pid(dlogutil_config_s *config, pid_t pid);

/**
 * @brief Enables retrieving only those logs that match a given filter.
 * @details Filters by tag and priority. Useful for dumping logs of a specific program or subcomponent, or of sufficient importance.
 * @since_tizen 6.0
 * @param[in] config A handle to the settings struct.
 * @param[in] query The filter query. For syntax, see dlogutil's --help or @ref CAPI_SYSTEM_DLOG_UTIL documentation.
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid syntax of the filterspec
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_OUT_OF_MEMORY Out of memory
 */
int dlogutil_config_filter_filterspec(dlogutil_config_s *config, const char *query);

/**
 * @brief Disables log sorting for given log retrieval request.
 * @details Logs are still received in some order that is usually largely sorted,
 *          but if sorting is disabled logutil-side there may be cases where a log
 *          with a later timestamp is in front of a log with an earlier one.
 *          The use case here is performance, since the failure case above is rare.
 * @since_tizen 6.0
 * @param[in] config A handle to the settings struct
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @see dlogutil_config_sorting_enable()
 * @see dlogutil_config_sorting_enable_with_size()
 */
int dlogutil_config_sorting_disable(dlogutil_config_s *config);

/**
 * @brief Enables log sorting for given log retrieval request.
 * @since_tizen 6.0
 * @details Makes sure logs are sorted. This is the default
 *          but has a modest performance cost.
 * @param[in] config A handle to the settings struct
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @see dlogutil_config_sorting_disable()
 * @see dlogutil_config_sorting_enable_with_size()
 */
int dlogutil_config_sorting_enable(dlogutil_config_s *config);

/**
 * @brief Enables sorting, choosing the sort buffer size manually.
 * @since_tizen 6.0
 * @remarks The count parameter influences the quality of sorting, but also memory usage.
 *       This version is a somewhat lower level version of dlogutil_config_sorting_enable().
 *       For more information on sorting quality, see @ref CAPI_SYSTEM_DLOGUTIL_OVERVIEW.
 * @param[in] config A handle to the settings struct
 * @param[in] entry_count How many logs to keep at a given time. At least 1
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Zero size
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @see dlogutil_config_sorting_disable()
 * @see dlogutil_config_sorting_enable()
 */
int dlogutil_config_sorting_enable_with_size(dlogutil_config_s *config, unsigned int entry_count);

/**
 * @brief Chooses a timestamp type by which returned logs are sorted by
 * @since_tizen 6.0
 * @details If the chosen timestamp is missing in the logs, currently they will not be sorted at all.
 *       This should, however, still become a reasonable order, since logs are usually stored
 *       sorted by one of timestamps.
 *       If only some logs are missing the chosen timestamp, ordering of the logs is undefined.
 *       See dlogutil_buffer_get_default_ts_type() for the default.
 * @param[in] config A handle to the settings struct
 * @param[in] sort_by Which timestamp type to sort by. #DLOGUTIL_SORT_DEFAULT is allowed
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 */
int dlogutil_config_order_set(dlogutil_config_s *config, dlogutil_sorting_order_e sort_by);

/**
 * @brief Adds a buffer whence logs will be taken to a request.
 * @details Specifies which buffer to retrieve logs from. Usually this will be main or apps.
 * @since_tizen 6.0
 * @param[in] config A handle to the settings struct
 * @param[in] buf The buffer ID to add to the handled set
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 */
int dlogutil_config_buffer_add(dlogutil_config_s *config, log_id_t buf);

/**
 * @brief Enables the full dump mode which prints every stored log
 * @details Pass this constant when you want to dump everything and not just N most recent logs.
 * @since_tizen 6.0
 * @see dlogutil_config_mode_set_dump()
 */
#define DLOGUTIL_MAX_DUMP_SIZE UINT_MAX

/**
 * @brief A struct containing the state of a log handling request
 * @details Represents a connection to dlog backend. You can't change config
 * anymore at this point, but can start retrieving logs.
 * @since_tizen 6.0
 */
typedef struct dlogutil_state dlogutil_state_s;

/**
 * @brief Destroys the #dlogutil_state_s struct and frees its memory
 * @details Use after you're done retrieving logs and are cleaning up.
 * @since_tizen 6.0
 * @param[in] state A handle to the state struct
 * @see dlogutil_config_connect()
 */
void dlogutil_state_destroy(dlogutil_state_s *state);

/**
 * @brief Set log retrieval mode to retrieving all the logs since the start of the system without an end.
 * @details Essentially a dump, followed by monitoring. See those modes.
 * @since_tizen 6.0
 * @remarks This is similar to dlogutil in a default mode.
 * @param[in,out] config The configuration struct
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Config was NULL
 * @see dlogutil_config_mode_set_monitor()
 * @see dlogutil_config_mode_set_dump()
 * @see dlogutil_config_mode_set_compressed_memory_dump()
 */
int dlogutil_config_mode_set_continuous(dlogutil_config_s *config);

/**
 * @brief Set log retrieval mode to retrieving all the logs since the call without an end.
 * @details Will only retrieve new logs. Any logs present before the connection are ignored. Good for live debugging.
 * @since_tizen 6.0
 * @remarks This is similar to dlogutil -m.
 * @param[in,out] config The configuration struct
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Config was NULL
 * @see dlogutil_config_mode_set_continuous()
 * @see dlogutil_config_mode_set_dump()
 * @see dlogutil_config_mode_set_compressed_memory_dump()
 */
int dlogutil_config_mode_set_monitor(dlogutil_config_s *config);

/**
 * @brief Set log retrieval mode to dumping all the logs since the start of the system until the call
 *        (possibly a specified amount of the most recent of them instead).
 * @details Will only retrieve existing logs. Any logs incoming after the connection are ignored. Good for avoiding floods.
 * @since_tizen 6.0
 * @remarks This is similar to dlogutil -d. After dumping all the logs, dlogutil_get_log() will signal this by returning TIZEN_ERROR_NO_DATA.
 * @param[in,out] config The configuration struct
 * @param[in] entry_count Number of logs to be returned.
 *                        It can be #DLOGUTIL_MAX_DUMP_SIZE, in which case all the logs will be dumped
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Config was NULL
 * @see dlogutil_config_mode_set_continuous()
 * @see dlogutil_config_mode_set_monitor()
 * @see dlogutil_config_mode_set_compressed_memory_dump()
 */
int dlogutil_config_mode_set_dump(dlogutil_config_s *config, unsigned int entry_count);

/**
 * @brief Set log retrieval mode to dumping compressed historical logs
 * @details Similar to the dump mode, but works on a named compressed buffer.
 * @since_tizen 7.0
 * @remarks This is similar to `cat /var/log/dlog/xyz`. After dumping all the logs, dlogutil_get_log() will signal this by returning TIZEN_ERROR_NO_DATA.
 * @param[in,out] config The configuration struct
 * @param[in] compression_buffer The name of the compression storage entry
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_OUT_OF_MEMORY Not enough memory. Parameters left unchanged
 * @retval TIZEN_ERROR_INVALID_PARAMETER Config or compression_buffer was NULL
 * @see dlogutil_config_mode_set_continuous()
 * @see dlogutil_config_mode_set_monitor()
 * @see dlogutil_config_mode_set_dump()
 */
int dlogutil_config_mode_set_compressed_memory_dump(dlogutil_config_s *config, const char *compression_buffer);


/**
 * @brief Finalizes the config into a state struct by connecting to buffers
 * @details Gives you a state, letting you start getting logs. You can get rid of the config at this point.
 * @since_tizen 6.0
 * @remarks An application having platform privilege level can read platform log data
 * by declaring %http://tizen.org/privilege/log, which has been added since 6.5.
 * @param[in] config A handle to the settings struct
 * @param[out] state_out A handle to the state struct, which can be used in dlogutil_get_log()
 *                       and has to be destroyed by you
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Config or state_out was NULL
 * @retval TIZEN_ERROR_INVALID_PARAMETER No buffers selected
 * @retval TIZEN_ERROR_NOT_SUPPORTED Unsupported buffer set (KMSG + non-KMSG)
 * @retval TIZEN_ERROR_NOT_SUPPORTED Unsupported backend (zero-copy)
 * @retval TIZEN_ERROR_NO_DATA No buffers were opened (incl. due to null backend)
 * @retval TIZEN_ERROR_IO_ERROR Couldn't read config file
 * @retval TIZEN_ERROR_IO_ERROR Couldn't contact log backend
 * @retval TIZEN_ERROR_OUT_OF_MEMORY There's not enough memory
 */
int dlogutil_config_connect(dlogutil_config_s *config, dlogutil_state_s **state_out);

/**
 * @brief Retrieves a single log according to a dump request
 * @details Gets a log from the backend. See the API for dlogutil entry.
 * @since_tizen 6.0
 * @remarks If the calling process doesn't have CAP_SYSLOG and is not in the log group, you will only get
 *          some of the logs. Also, you must set the mode (dlogutil_config_mode_set_*).
 * @param[in] state A handle to the state struct
 * @param[in] timeout How many milliseconds to wait for the log.
 *                    The actual runtime of the call can obviously be slightly longer than this argument.
 *                    0 means don't wait, -1 means wait indefinitely
 * @param[out] entry_out A returned log. It is owned by you and must be freed using dlogutil_entry_destroy function.
 *                       In case of an error, NULL is returned
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_TIMED_OUT Timeout exceeded
 * @retval TIZEN_ERROR_NO_DATA In dump mode, no more logs remaining
 * @retval TIZEN_ERROR_INVALID_PARAMETER One of the pointers was NULL
 * @retval TIZEN_ERROR_INVALID_PARAMETER State not in log-getting mode
 * @retval TIZEN_ERROR_OUT_OF_MEMORY There's not enough memory
 * @retval TIZEN_ERROR_IO_ERROR Couldn't fulfill request
 * @see dlogutil_entry_destroy()
 */
int dlogutil_get_log(dlogutil_state_s *state, int timeout, dlogutil_entry_s **entry_out);

/**
 * @brief Irreversibly clears a log buffer from any logs inside
 * @details Tells the backend to get rid of any stored logs. Good for chaff management.
 * @since_tizen 6.0
 * @remarks You can't use one of the log-getting modes (dlogutil_config_mode_set_*).
 * @param[in] state A handle to the state struct
 * @param[in] buffer A buffer to be cleared
 * @remarks Either CAP_SYSLOG or being in the log group is required.
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER State pointer was null
 * @retval TIZEN_ERROR_INVALID_PARAMETER State in log-getting mode
 * @retval TIZEN_ERROR_IO_ERROR Couldn't fulfill request
 * @retval Other An arbitrary Tizen error code
 */
int dlogutil_buffer_clear(dlogutil_state_s *state, log_id_t buffer);

/**
 * @brief Gets the human-readable, constant name of a buffer
 * @details Gets the name of a buffer according to dlog. Mostly for convenience.
 * @since_tizen 6.0
 * @remarks The returned string will have the static lifetime.
 * @param[in] buffer A buffer to be inspected
 * @param[out] name The name of the passed buffer
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 */
int dlogutil_buffer_get_name(log_id_t buffer, const char **name);

/**
 * @brief Gets the data storage capacity of a log buffer in bytes
 * @details Returns how many bytes of logs can fit in given buffer on the backend.
 * @since_tizen 6.0
 * @remarks Either CAP_SYSLOG or being in the log group is required.
 *          Also, you can't use one of the log-getting modes (dlogutil_config_mode_set_*).
 * @bug This is currently evaluated using the config file, which can change at runtime. In particular,
 *      this can desync from the value of the server if the config file changes. Note that this is mostly
 *      a theoretical concern, since the file is not intended to change during the runtime (the daemon asks
 *      to reboot the system in such a case).
 * @param[in] state A handle to the state struct
 * @param[in] buffer A buffer to be inspected
 * @param[out] capacity The buffer's maximum capacity, in bytes
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER The state pointer was NULL
 * @retval TIZEN_ERROR_INVALID_PARAMETER The capacity pointer was NULL
 * @retval TIZEN_ERROR_INVALID_PARAMETER State in log-getting mode
 * @retval TIZEN_ERROR_IO_ERROR Couldn't fulfill request
 * @retval Other An arbitrary Tizen error code
 */
int dlogutil_buffer_get_capacity(dlogutil_state_s *state, log_id_t buffer, unsigned int *capacity);

/**
 * @brief Gets the storage data usage of a log buffer, in bytes
 * @details Returns how many bytes of logs are currently stored in given buffer on the backend.
 * @since_tizen 6.0
 * @remarks Either CAP_SYSLOG or being in the log group is required.
 *          Also, you can't use one of the log-getting modes (dlogutil_config_mode_set_*).
 * @bug This is currently evaluated using the config file, which can change at runtime. In particular,
 *      this can desync from the value of the server if the config file changes. Note that this is mostly
 *      a theoretical concern, since the file is not intended to change during the runtime (the daemon asks
 *      to reboot the system in such a case).
 * @param[in] state A handle to the state struct
 * @param[in] buffer A buffer to be inspected
 * @param[out] usage Buffer's current usage, in bytes
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER The state pointer was NULL
 * @retval TIZEN_ERROR_INVALID_PARAMETER The usage pointer was NULL
 * @retval TIZEN_ERROR_INVALID_PARAMETER State in log-getting mode
 * @retval TIZEN_ERROR_IO_ERROR Couldn't fulfill request
 * @retval Other An arbitrary Tizen error code
 */
int dlogutil_buffer_get_usage(dlogutil_state_s *state, log_id_t buffer, unsigned int *usage);

/**
 * @brief Gets the default sorting timestamp type of a buffer.
 * @since_tizen 6.0
 * @details This is the timestamp type that will be used for sorting by default.
 *          We assume that it is always available and the input is sorted by it.
 *          See @ref CAPI_SYSTEM_DLOGUTIL_OVERVIEW for more details.
 * @bug This is currently evaluated using the config file, which can change at runtime. In particular,
 *      this can desync from the value of the server if the config file changes. Note that this is mostly
 *      a theoretical concern, since the file is not intended to change during the runtime (the daemon asks
 *      to reboot the system in such a case).
 * @param[in] buffer A single buffer to be inspected
 * @param[out] type The default timestamp type of the passed buffer
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @retval TIZEN_ERROR_IO_ERROR Couldn't read config file
 * @retval Other An arbitrary Tizen error code
 */
int dlogutil_buffer_get_default_ts_type(log_id_t buffer, dlogutil_sorting_order_e *type);

/**
 * @brief Checks if a buffer contains timestamps of a given type.
 * @since_tizen 6.0
 * @details If false is returned, the timestamp may still be available in some of the logs.
 *          However, if true is returned, the timestamp will always be available.
 *          You can check the timestamp availability per log using the dlogutil_entry_get_timestamp() function.
 * @bug This is currently evaluated using the config file, which can change at runtime. In particular,
 *      this can desync from the value of the server if the config file changes. Note that this is mostly
 *      a theoretical concern, since the file is not intended to change during the runtime (the daemon asks
 *      to reboot the system in such a case).
 * @param[in] buffer A single buffer to be inspected
 * @param[in] type The timestamp type to be considered
 * @param[out] available Whether the given timestamp type is guaranteed to be available
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER More than one buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 * @retval TIZEN_ERROR_IO_ERROR Couldn't read config file
 */
int dlogutil_buffer_check_ts_type_available(log_id_t buffer, dlogutil_sorting_order_e type, bool *available);

/**
 * @brief Gets the buffer aliasing (same storage) information
 * @since_tizen 6.0
 * @details Sometimes, multiple buffers will be backed by a single log storage (for example, by the same
 *          kernel device). In such cases, the storage will only be opened once. This function allows you
 *          to see whether this is the case.
 * @bug This is currently evaluated using the config file, which can change at runtime. In particular,
 *      this can desync from the value of the server if the config file changes. Note that this is mostly
 *      a theoretical concern, since the file is not intended to change during the runtime (the daemon asks
 *      to reboot the system in such a case).
 * @param[in] state A handle to the state struct
 * @param[in] buffer A single buffer to be inspected
 * @param[out] real_buffer Buffer aliasing information: LOG_ID_INVALID if the buffer is disabled,
 *                         the log_id_t value corresponding to the real buffer otherwise
 * @return An error code
 * @retval TIZEN_ERROR_NONE Success
 * @retval TIZEN_ERROR_INVALID_PARAMETER Invalid buffer
 * @retval TIZEN_ERROR_INVALID_PARAMETER The pointer was NULL
 */
int dlogutil_buffer_get_alias(dlogutil_state_s *state, log_id_t buffer, log_id_t *real_buffer);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_DLOGUTIL_H__ */
