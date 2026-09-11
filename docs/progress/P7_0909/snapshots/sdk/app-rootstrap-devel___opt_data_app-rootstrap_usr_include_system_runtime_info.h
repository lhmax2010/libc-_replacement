/*
 * Copyright (c) 2011 - 2015 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_SYSTEM_RUNTIME_INFO_H__
#define __TIZEN_SYSTEM_RUNTIME_INFO_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @addtogroup  CAPI_SYSTEM_RUNTIME_INFO_MODULE
 * @{
 */


/**
 * @brief        Enumeration for error codes for runtime information.
 * @since_tizen  2.3
 */
typedef enum {
	RUNTIME_INFO_ERROR_NONE = TIZEN_ERROR_NONE,                           /**< Successful */
	RUNTIME_INFO_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	RUNTIME_INFO_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,         /**< Out of memory */
	RUNTIME_INFO_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR,                   /**< An input/output error occurred when read value from system */
	RUNTIME_INFO_ERROR_REMOTE_IO = TIZEN_ERROR_REMOTE_IO,                 /**< Remote I/O error occurred */
	RUNTIME_INFO_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< No permission to use the api */
	RUNTIME_INFO_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,         /**< Not supported parameter */
	RUNTIME_INFO_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA                      /**< No data available (Since 3.0) */
} runtime_info_error_e;


/**
 * @brief        Enumeration for keys for runtime information.
 * @since_tizen  2.3
 * @remarks      ~10000 : public key \n
 *               10001~ : product specific key
 */
typedef enum {
	RUNTIME_INFO_KEY_BLUETOOTH_ENABLED = 2,                 /**<Indicates whether Bluetooth is enabled. */
	RUNTIME_INFO_KEY_WIFI_HOTSPOT_ENABLED = 3,              /**<Indicates whether Wi-Fi hotspot is enabled. */
	RUNTIME_INFO_KEY_BLUETOOTH_TETHERING_ENABLED = 4,       /**<Indicates whether Bluetooth tethering is enabled. */
	RUNTIME_INFO_KEY_USB_TETHERING_ENABLED = 5,             /**<Indicates whether USB tethering is enabled. */
	RUNTIME_INFO_KEY_PACKET_DATA_ENABLED = 9,               /**<Indicates Whether the packet data through 3G network is enabled. */
	RUNTIME_INFO_KEY_DATA_ROAMING_ENABLED = 10,             /**<Indicates whether data roaming is enabled. */
	RUNTIME_INFO_KEY_VIBRATION_ENABLED = 12,                /**<Indicates whether vibration is enabled. */
	RUNTIME_INFO_KEY_AUDIO_JACK_CONNECTED = 17,             /**<Indicates whether audio jack is connected. */
	RUNTIME_INFO_KEY_GPS_STATUS = 18,                       /**<Indicates the current status of GPS. */
	RUNTIME_INFO_KEY_BATTERY_IS_CHARGING = 19,              /**<Indicates the battery is currently charging. */
	RUNTIME_INFO_KEY_TV_OUT_CONNECTED = 20,                 /**<Indicates whether TV out is connected. */
	RUNTIME_INFO_KEY_AUDIO_JACK_STATUS = 21,                /**<Indicates the current status of audio jack. */
	RUNTIME_INFO_KEY_USB_CONNECTED = 23,                    /**<Indicates whether USB is connected. */
	RUNTIME_INFO_KEY_CHARGER_CONNECTED = 24,                /**<Indicates whether charger is connected. */
	RUNTIME_INFO_KEY_AUTO_ROTATION_ENABLED = 26,            /**<Indicates whether auto rotation is enabled. */
} runtime_info_key_e;


/**
 * @brief        Enumeration for Wi-Fi status.
 * @since_tizen  2.3
 */
typedef enum {
	RUNTIME_INFO_WIFI_STATUS_DISABLED,    /**< Wi-Fi is disabled. */
	RUNTIME_INFO_WIFI_STATUS_UNCONNECTED, /**< Wi-Fi is enabled and network connection is not established. */
	RUNTIME_INFO_WIFI_STATUS_CONNECTED,   /**< Network connection is established in Wi-Fi network. */
} runtime_info_wifi_status_e;


/**
 * @brief        Enumeration for GPS status.
 * @since_tizen  2.3
 */
typedef enum {
	RUNTIME_INFO_GPS_STATUS_DISABLED,  /**< GPS is disabled. */
	RUNTIME_INFO_GPS_STATUS_SEARCHING, /**< GPS is searching for satellites. */
	RUNTIME_INFO_GPS_STATUS_CONNECTED, /**< GPS connection is established. */
} runtime_info_gps_status_e;

/**
 * @brief        Enumeration for audio jack status.
 * @since_tizen  2.3
 */
typedef enum {
	RUNTIME_INFO_AUDIO_JACK_STATUS_UNCONNECTED,     /**< Audio jack is not connected */
	RUNTIME_INFO_AUDIO_JACK_STATUS_CONNECTED_3WIRE, /**< 3-conductor wire is connected. */
	RUNTIME_INFO_AUDIO_JACK_STATUS_CONNECTED_4WIRE, /**< 4-conductor wire is connected. */
} runtime_info_audio_jack_status_e;


/**
 * @brief        Called when the runtime information changes.
 * @since_tizen  2.3
 *
 * @param[in] key        The type of notification
 * @param[in] user_data  The user data passed from the callback registration function
 *
 * @pre  runtime_info_set_changed_cb() will invoke this callback function.
 * @see  runtime_info_set_changed_cb()
 * @see  runtime_info_unset_changed_cb()
 */
typedef void (*runtime_info_changed_cb)(runtime_info_key_e key, void *user_data);


/**
 * @brief        Gets the integer value of the runtime information.
 * @details      This function gets current state of the given key which represents specific runtime information.
 * @since_tizen  2.3
 *
 * @param[in]  key    The runtime information status key from which data should be read
 * @param[out] value  The current value of the given key
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An input/output error occurred when read value from system
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  No permission to use the api
 * @retval #RUNTIME_INFO_ERROR_NOT_SUPPORTED      Not supported parameter
 */
int runtime_info_get_value_int(runtime_info_key_e key, int *value);


/**
 * @brief        Gets the boolean value from the runtime information.
 * @details      This function gets current state of the given key which represents specific runtime information.
 * @since_tizen  2.3
 *
 * @param[in]  key    The runtime information key from which data should be read
 * @param[out] value  The current value of the given key
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An input/output error occurred when read value from system
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  No permission to use the api
 * @retval #RUNTIME_INFO_ERROR_NOT_SUPPORTED      Not supported parameter
 */
int runtime_info_get_value_bool(runtime_info_key_e key, bool *value);


/**
 * @brief        Gets the double value from the runtime information.
 * @details      This function gets current state of the given key which represents specific runtime information.
 * @since_tizen  2.3
 *
 * @param[in]  key    The runtime information key from which data should be read
 * @param[out] value  The current value of the given key
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An input/output error occurred when read value from system
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  No permission to use the api
 * @retval #RUNTIME_INFO_ERROR_NOT_SUPPORTED      Not supported parameter
 */
int runtime_info_get_value_double(runtime_info_key_e key, double *value);


/**
 * @brief        Gets the string value for specified runtime information.
 * @details      This function gets current state of the given key which represents specific runtime information.
 * @since_tizen  2.3
 *
 * @remarks  You must release @a value using free().
 *
 * @param[in]  key    The runtime information key from which data should be read
 * @param[out] value  The current value of the given key
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An input/output error occurred when read value from system
 * @retval #RUNTIME_INFO_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  No permission to use the api
 * @retval #RUNTIME_INFO_ERROR_NOT_SUPPORTED      Not supported parameter
 */
int runtime_info_get_value_string(runtime_info_key_e key, char **value);


/**
 * @brief        Registers a change event callback for given runtime information key.
 * @details      Only the last registered callback will be operational if it is called multiple
 *               times on a same key. That is, it replaces previously registered one, if any.
 * @since_tizen  2.3
 * @remarks  You must release @a callback using runtime_info_unset_changed_cb().
 *
 * @param[in] key        The runtime information type
 * @param[in] callback   The callback function to invoke
 * @param[in] user_data  The user data to be passed to the callback function
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  No permission to use the api
 * @retval #RUNTIME_INFO_ERROR_NOT_SUPPORTED      Not supported parameter
 *
 * @post  runtime_info_changed_cb() will be invoked.
 * @see   runtime_info_unset_changed_cb()
 * @see   runtime_info_changed_cb()
*/
int runtime_info_set_changed_cb(runtime_info_key_e key, runtime_info_changed_cb callback, void *user_data);


/**
 * @brief        Unregisters the callback function registered by runtime_info_set_changed_cb().
 * @details      Unregisters the callback function registered by runtime_info_set_changed_cb() on the key.
 * @since_tizen  2.3
 *
 * @param[in] key  The runtime information type
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see  runtime_info_set_changed_cb()
 */
int runtime_info_unset_changed_cb(runtime_info_key_e key);


/**
 * @brief        Structure for memory information.
 * @since_tizen  2.4
 * @details      This structure provides comprehensive memory usage information collected from /proc/meminfo.
 *               The memory values are reported in kibibytes (KiB) where 1 KiB = 1024 bytes.
 *
 *               The memory metrics:
 *               - @c total: Total physical memory (MemTotal field from /proc/meminfo)
 *               - @c used: Actually used memory, calculated as (total - available) when MemAvailable is available,
 *                 otherwise (total - free - cache) when MemAvailable is not available
 *               - @c free: Completely unused memory that can be allocated immediately (MemFree field)
 *               - @c cache: Memory used for file system cache and buffers (Cached field)
 *               - @c swap: Used swap space on disk, calculated as (SwapTotal - SwapFree)
 */
typedef struct {
	int total; /**< Total memory (KiB) */
	int used;  /**< Used memory (KiB) */
	int free;  /**< Free memory (KiB) */
	int cache; /**< Cache memory (KiB) */
	int swap;  /**< Swap memory (KiB) */
} runtime_memory_info_s;


/**
 * @brief        Gets system memory information in the form of #runtime_memory_info_s.
 * @details      The information is collected from /proc/meminfo.
 * @since_tizen  2.4
 *
 * @param[out] info  The system memory information structure
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An Input/Output error occurred while reading from system
 *
 * @see  runtime_info_get_process_memory_info()
 */
int runtime_info_get_system_memory_info(runtime_memory_info_s *info);


/**
 * @brief        Structure for memory information per process.
 * @since_tizen  2.4
 * @details      This structure provides detailed memory usage information for specific processes.
 *               The information is collected from /proc/[pid]/stat and /proc/[pid]/smaps files.
 *               All values are reported in kibibytes (KiB).
 *
 *               Memory metrics:
 *               - @c vsz (Virtual Memory Size): Total virtual memory used by process, collected from
 *                 /proc/[pid]/stat field 23 (virtual memory size in bytes) and converted to KiB
 *               - @c rss (Resident Set Size): Non-swapped physical memory used by process, collected
 *                 from /proc/[pid]/smaps RSS sum
 *               - @c pss (Proportional Set Size): RSS adjusted for shared pages (each shared page's
 *                 size is divided by the number of processes sharing it), collected from /proc/[pid]/smaps PSS sum
 *               - @c shared_clean: Memory mapped to files that hasn't been modified and shared
 *                 with other processes (e.g., shared libraries), collected from /proc/[pid]/smaps Shared_Clean sum
 *               - @c shared_dirty: Memory mapped to files that has been modified and shared
 *                 with other processes, collected from /proc/[pid]/smaps Shared_Dirty sum
 *               - @c private_clean: Process-private memory mapped to files that hasn't been modified,
 *                 collected from /proc/[pid]/smaps Private_Clean sum
 *               - @c private_dirty: Process-private anonymous memory that has been modified
 *                 (heap, stack, dynamically allocated memory), collected from /proc/[pid]/smaps Private_Dirty sum
 */
typedef struct {
	int vsz;           /**< Virtual memory size (KiB) */
	int rss;           /**< Resident set size (KiB) */
	int pss;           /**< Proportional set size (KiB) */
	int shared_clean;  /**< Not modified and mapped by other processes (KiB) */
	int shared_dirty;  /**< Modified and mapped by other processes (KiB) */
	int private_clean; /**< Not modified and available only to that process (KiB) */
	int private_dirty; /**< Modified and available only to that process (KiB) */
} process_memory_info_s;


/**
 * @brief        Gets memory information per process in the form of #process_memory_info_s.
 * @details      The information is collected from /proc/[pid]/stat and /proc/[pid]/smaps.
 * @since_tizen  2.4
 *
 * @privlevel  public
 * @privilege  %http://tizen.org/privilege/systemmonitor
 * @remarks    You must release @a s value using free(). \n
 *             The size of @a s is the same with @a size.
 *
 * @param[in]  pid   The process unique id array
 * @param[in]  size  The size of pid array
 * @param[out] info  The memory information structure array of the processes
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_OUT_OF_MEMORY      Not able to allocate memory (for output param/other operations)
 * @retval #RUNTIME_INFO_ERROR_REMOTE_IO          Call to resource daemon failed (dbus errors/resource daemon errors)
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error during dbus message operations
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  Process not authorized to request process usage info
 *
 * @see  runtime_info_get_system_memory_info()
 */
int runtime_info_get_process_memory_info(int *pid, int size, process_memory_info_s **info);


/**
 * @brief        Structure for CPU usage.
 * @since_tizen  2.4
 * @details      This structure provides CPU usage distribution across different states.
 *               The values are percentages calculated since system boot and represent
 *               the time CPU spent in each state. The information is collected from /proc/stat.
 *
 *               Each field is calculated as a percentage using the formula:
 *               percentage = (field_value / total_uptime) × 100
 *               where total_uptime = user + nice + system + idle + iowait + irq + softirq
 *               (values collected from /proc/stat cpu line fields 1-7 in clock ticks)
 *
 *               CPU usage metrics:
 *               - @c user: Percentage of time spent running user-mode processes (applications),
 *                 calculated as (user_value / total_uptime) × 100, where user_value is collected from
 *                 /proc/stat field 1 (user mode time in clock ticks)
 *               - @c system: Percentage of time spent running kernel-mode processes (system calls, drivers),
 *                 calculated as (system_value / total_uptime) × 100, where system_value is collected from
 *                 /proc/stat field 3 (system mode time in clock ticks)
 *               - @c nice: Percentage of time spent running user-mode processes with lowered priority,
 *                 calculated as (nice_value / total_uptime) × 100, where nice_value is collected from
 *                 /proc/stat field 2 (nice priority user mode time in clock ticks)
 *               - @c iowait: Percentage of time spent waiting for I/O operations to complete (disk, network),
 *                 calculated as (iowait_value / total_uptime) × 100, where iowait_value is collected from
 *                 /proc/stat field 5 (I/O wait time in clock ticks)
 */
typedef struct {
	double user;   /**< Time running un-niced user processes (Percent) */
	double system; /**< Time running kernel processes (Percent) */
	double nice;   /**< Time running niced user processes (Percent) */
	double iowait; /**< Time waiting for I/O completion (Percent) */
} runtime_cpu_usage_s;


/**
 * @brief        Gets CPU usgae information in the form of #runtime_cpu_usage_s.
 * @details      The information is collected from /proc/stat.
 * @since_tizen  2.4
 *
 * @param[out] usage  The CPU usage structure
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An input/output error occurred while reading from system
 *
 * @see  runtime_info_get_process_cpu_usage()
 */
int runtime_info_get_cpu_usage(runtime_cpu_usage_s *usage);


/**
 * @brief        Structure for CPU usage per process.
 * @since_tizen  2.4
 * @details      This structure provides CPU usage information for specific processes.
 *               The information is collected from /proc/[pid]/stat file.
 *               All values are reported in clock ticks.
 *
 *               CPU usage metrics:
 *               - @c utime: Amount of time that this process has been scheduled in user mode,
 *                 collected from /proc/[pid]/stat field 14 (user mode time in clock ticks)
 *               - @c stime: Amount of time that this process has been scheduled in kernel mode,
 *                 collected from /proc/[pid]/stat field 15 (kernel mode time in clock ticks)
 */
typedef struct {
	int utime; /**< Amount of time that this process has been scheduled in user mode (clock ticks) */
	int stime; /**< Amount of time that this process has been scheduled in kernel mode (clock ticks) */
} process_cpu_usage_s;


/**
 * @brief        Gets CPU usage in the form of #process_cpu_usage_s.
 * @details      The information is collected from /proc/[pid]/stat.
 * @since_tizen  2.4
 *
 * @privlevel  public
 * @privilege  %http://tizen.org/privilege/systemmonitor
 * @remarks    You must release @a s value using free(). \n
 *             The size of @a s is the same with @a size.
 *
 * @param[in]  pid   The process unique id array
 * @param[in]  size  The size of pid array
 * @param[out] usage The CPU usage structure array of the processes
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_OUT_OF_MEMORY      Not able to allocate memory (for output param/other operations)
 * @retval #RUNTIME_INFO_ERROR_REMOTE_IO          Call to resource daemon failed (dbus errors/resource daemon errors)
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error occurred (during dbus message operations/other IO operations)
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  Process not authorized to request process usage info
 *
 * @see  runtime_info_get_cpu_usage()
 */
int runtime_info_get_process_cpu_usage(int *pid, int size, process_cpu_usage_s **usage);


/**
 * @brief        Gets the total number of processors including both online and offline.
 * @details      The information is collected from /sys/devices/system/cpu/possible.
 * @since_tizen  3.0
 *
 * @param[out] num_core  The number of whole processors
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error occurred (during file open operation)
 */
int runtime_info_get_processor_count(int *num_core);


/**
 * @brief        Gets the current frequency of a specific processor.
 * @details      The information is collected from /sys/devices/system/cpu/cpu[idx]/cpufreq/scaling_cur_freq.
 * @since_tizen  3.0
 *
 * @param[in]  core_idx  The index (from 0) of CPU core that you want to know the frequency
 * @param[out] cpu_freq  The current frequency(MHz) of processor
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error occurred (during file open operation)
 * @retval #RUNTIME_INFO_ERROR_NO_DATA            No data available (Since 3.0)
 */
int runtime_info_get_processor_current_frequency(int core_idx, int *cpu_freq);


/**
 * @brief        Gets the maximum frequency of a specific processor.
 * @details      The information is collected from /sys/devices/system/cpu/cpu[idx]/cpufreq/scaling_max_freq.
 * @since_tizen  3.0
 *
 * @param[in]  core_idx  The index (from 0) of CPU core that you want to know the frequency
 * @param[out] cpu_freq  The max frequency(MHz) of processor
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error occurred (during file open operation)
 * @retval #RUNTIME_INFO_ERROR_NO_DATA            No data available (Since 3.0)
 */
int runtime_info_get_processor_max_frequency(int core_idx, int *cpu_freq);


/**
 * @brief        Gets the total physical memory size in kibibyte(KiB).
 * @details      The information is collected from /proc/zoneinfo.
 * @since_tizen  4.0
 *
 * @param[out] size  Physical memory size (KiB)
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error occurred (during file open operation)
 */
int runtime_info_get_physical_memory_size(int *size);


/**
 * @brief        Handle for app usage information.
 * @since_tizen  4.0
 */
typedef struct app_usages_s *app_usage_h;


/**
 * @brief        Destroy #app_usage_h that won't be used anymore.
 * @details      Destroy #app_usage_h acquired by functions runtime_info_get_all_apps_memory_usage()
 *               or runtime_info_get_all_apps_cpu_rate().
 * @since_tizen  4.0
 *
 * @param[in] handle  App usage handle to free
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see  runtime_info_get_all_apps_memory_usage()
 * @see  runtime_info_get_all_apps_cpu_rate()
 */
int runtime_info_app_usage_destroy(app_usage_h handle);


/**
 * @brief        Gets the number of applications that #app_usage_h has retrieved.
 * @details      Gets only applications that are managed by <a href="https://docs.tizen.org/application/native/guides/app-management/app-manager/">Application Manager</a>.
 * @since_tizen  4.0
 *
 * @param[in]  handle  The app usage handle
 * @param[out] count   The number of apps on the app list
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see  runtime_info_get_all_apps_memory_usage()
 * @see  runtime_info_get_all_apps_cpu_rate()
 */
int runtime_info_app_usage_get_count(app_usage_h handle, int *count);


/**
 * @brief        Gets an application ID by index from an #app_usage_h.
 * @details      Gets only applications that are managed by <a href="https://docs.tizen.org/application/native/guides/app-management/app-manager/">Application Manager</a>.
 * @since_tizen  4.0
 *
 * @remarks  You must release @a appid using free().
 *
 * @param[in]  handle  The app usage handle
 * @param[in]  index   The index in the app list; should be between 0 and @a count - 1 (inclusive), \n
 *                     where @a count is provided by runtime_info_app_usage_get_count()
 * @param[out] appid   The app ID
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see  runtime_info_get_all_apps_memory_usage()
 * @see  runtime_info_get_all_apps_cpu_rate()
 */
int runtime_info_app_usage_get_appid(app_usage_h handle, int index, char **appid);


/**
 * @brief        Gets resource usage of an application by index from an #app_usage_h.
 * @details      Counts memory that is exclusively used by an application. That is, it doesn't
 *               count shared memory of an application such as file or shared library, etc.
 *               Gets only applications that are managed by <a href="https://docs.tizen.org/application/native/guides/app-management/app-manager/">Application Manager</a>.
 * @since_tizen  4.0
 *
 * @remarks  The meaning of @a usage depends on which function was used to create @a handle.
 *
 * @param[in]  handle  The app usage handle
 * @param[in]  index   The index in the app list; should be between 0 and @a count - 1 (inclusive), \n
 *                     where @a count is provided by runtime_info_app_usage_get_count()
 * @param[out] usage   Resource usage
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see  runtime_info_get_all_apps_memory_usage()
 * @see  runtime_info_get_all_apps_cpu_rate()
 */
int runtime_info_app_usage_get_usage(app_usage_h handle, int index, unsigned int *usage);


/**
 * @brief        Gets memory usage of all applications in kilobyte into #app_usage_h.
 * @details      Counts memory that is exclusively used by an application. That is, it doesn't
 *               count shared memory of an application such as file or shared library, etc.
 *               Gets only applications that are managed by <a href="https://docs.tizen.org/application/native/guides/app-management/app-manager/">Application Manager</a>.
 * @since_tizen  4.0
 *
 * @privlevel  public
 * @privilege  %http://tizen.org/privilege/systemmonitor
 * @remarks    You must release @a usage using runtime_info_app_usage_destroy().
 *
 * @param[out] usage  An array of each app's memory usage (KB)
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_OUT_OF_MEMORY      Not able to allocate memory (for output param/other operations)
 * @retval #RUNTIME_INFO_ERROR_REMOTE_IO          Call to resource daemon failed (dbus errors/resource daemon errors)
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error during dbus message operations
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  Process not authorized to request app usage info
 * @retval #RUNTIME_INFO_ERROR_NO_DATA            No app running
 *
 * @code
 * #include <runtime_info.h>
 *
 * void print_memory_usage(void)
 * {
 *     int i;
 *     int count;
 *     app_usage_h mem_usage_handle;
 *     char *appid;
 *     unsigned int usage;
 *
 *     runtime_info_get_all_apps_memory_usage(&mem_usage_handle);
 *     runtime_info_app_usage_get_count(mem_usage_handle, &count);
 *
 *     for (i = 0; i < count; i++) {
 *         runtime_info_app_usage_get_appid(mem_usage_handle, i, &appid);
 *         runtime_info_app_usage_get_usage(mem_usage_handle, i, &usage);
 *         printf("appid = %s, usage = %u KB\n", appid, usage);
 *         free(appid);
 *     }
 *
 *     runtime_info_app_usage_destroy(mem_usage_handle);
 *
 * }
 * @endcode
 */
int runtime_info_get_all_apps_memory_usage(app_usage_h *usage);


/**
 * @brief        Gets CPU rate of all applications in kilobyte into #app_usage_h.
 * @details      The information is collected from /proc/[pid]/stat.
 *               Gets only applications that are managed by <a href="https://docs.tizen.org/application/native/guides/app-management/app-manager/">Application Manager</a>.
 * @since_tizen  4.0
 *
 * @privlevel  public
 * @privilege  %http://tizen.org/privilege/systemmonitor
 * @remarks    You must release @a rate using runtime_info_app_usage_destroy().
 *
 * @param[out] rate  An array of each app's CPU usage rate (%), the values are rounded down.
 *
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval #RUNTIME_INFO_ERROR_NONE               Successful
 * @retval #RUNTIME_INFO_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #RUNTIME_INFO_ERROR_OUT_OF_MEMORY      Not able to allocate memory (for output param/other operations)
 * @retval #RUNTIME_INFO_ERROR_REMOTE_IO          Call to resource daemon failed (dbus errors/resource daemon errors)
 * @retval #RUNTIME_INFO_ERROR_IO_ERROR           An I/O error during dbus message operations
 * @retval #RUNTIME_INFO_ERROR_PERMISSION_DENIED  Process not authorized to request app usage info
 * @retval #RUNTIME_INFO_ERROR_NO_DATA            No app running
 *
 * @code
 * #include <runtime_info.h>
 *
 * void print_cpu_usage(void)
 * {
 *     int i;
 *     int count;
 *     app_usage_h cpu_rate_handle;
 *     char *appid;
 *     unsigned int rate;
 *
 *     runtime_info_get_all_apps_cpu_rate(&cpu_rate_handle);
 *     runtime_info_app_usage_get_count(cpu_rate_handle, &count);
 *
 *     for (i = 0; i < count; i++) {
 *         runtime_info_app_usage_get_appid(cpu_rate_handle, i, &appid);
 *         runtime_info_app_usage_get_usage(cpu_rate_handle, i, &rate);
 *         printf("appid = %s, rate = %u %%\n", appid, rate);
 *         free(appid);
 *     }
 *
 *     runtime_info_app_usage_destroy(cpu_rate_handle);
 *
 * }
 * @endcode
 */
int runtime_info_get_all_apps_cpu_rate(app_usage_h *rate);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_RUNTIME_INFO_H__ */
