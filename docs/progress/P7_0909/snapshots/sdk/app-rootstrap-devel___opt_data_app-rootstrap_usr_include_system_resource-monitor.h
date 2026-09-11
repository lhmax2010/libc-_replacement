/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_SYSTEM_MONITOR_RESOURCE_MONITOR_H__
#define __TIZEN_SYSTEM_MONITOR_RESOURCE_MONITOR_H__

#include <tizen.h>

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file resource-monitor.h
 * @brief Provide resource C API for Resource Monitor which monitors physical
 * and logical resource. Each resource provides various attribute for resource
 * monitoring because each resource has the different characteristics.
 */

/**
 * @addtogroup CAPI_SYSTEM_RESOURCE_MONITOR_MODULE
 * @{
 */

/**
 * @brief Enumeration for resource monitor error value to catch the accurate
 * error reason.
 * @since_tizen 7.0
 */
typedef enum {
	RESOURCE_MONITOR_ERROR_NONE			= TIZEN_ERROR_NONE,			/**< Successful */
	RESOURCE_MONITOR_ERROR_PERMISSION_DENIED	= TIZEN_ERROR_PERMISSION_DENIED,	/**< Permission Denied */
	RESOURCE_MONITOR_ERROR_INVALID_PARAMETER	= TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid Parameter */
	RESOURCE_MONITOR_ERROR_NO_DATA			= TIZEN_ERROR_NO_DATA,			/**< Empty Data */
	RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY		= TIZEN_ERROR_OUT_OF_MEMORY,		/**< Out of Memory */
	RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_RESOURCE	= TIZEN_ERROR_RESOURCE_MONITOR | 0x01,	/**< Unavailable Resource Type */
	RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE	= TIZEN_ERROR_RESOURCE_MONITOR | 0x02,	/**< Unavailable Attribute */
} resource_monitor_error_e;

/**
 * @brief Enumeration for resource type which indicate physical and logical resource.
 * @since_tizen 7.0
 */
typedef enum {
	RESOURCE_MONITOR_TYPE_UNKNOWN			= 0,					/**< Unknown Resource Type */
	RESOURCE_MONITOR_TYPE_CPU			= 1,					/**< CPU Resource Type */
	RESOURCE_MONITOR_TYPE_BUS			= 2,					/**< BUS Resource Type */
	RESOURCE_MONITOR_TYPE_GPU			= 3,					/**< GPU Resource Type */
	RESOURCE_MONITOR_TYPE_MEMORY			= 4,					/**< Memory Resource Type */
	RESOURCE_MONITOR_TYPE_BATTERY			= 5,					/**< Battery Resource Type */
	RESOURCE_MONITOR_TYPE_DISPLAY			= 7,					/**< Display Resource Type */
	RESOURCE_MONITOR_TYPE_SYSTEM			= 8,					/**< System Resource Type */
	RESOURCE_MONITOR_TYPE_DISK			= 10,					/**< Disk Resource Type */
} resource_monitor_type_e;

/**
 * @brief Enumeration for resource attribute ID which indicates the attributes
 * of each physical and logical resource.
 * @since_tizen 7.0
 */
typedef enum {
	/* CPU Resource Attribute ID */
	RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ		= (1ULL << 0),				/**< Current CPU frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_CPU_ATTR_MIN_FREQ		= (1ULL << 1),				/**< Current CPU minimum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_CPU_ATTR_MAX_FREQ		= (1ULL << 2),				/**< Current CPU maximum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_CPU_ATTR_AVAILABLE_MIN_FREQ	= (1ULL << 3),				/**< Available CPU minimum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_CPU_ATTR_AVAILABLE_MAX_FREQ	= (1ULL << 4),				/**< Available CPU maximum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_CPU_ATTR_CUR_GOVERNOR		= (1ULL << 5),				/**< Current CPU frequency governor name (data type: DATA_TYPE_STRING) */
	RESOURCE_MONITOR_CPU_ATTR_NAME			= (1ULL << 6),				/**< CPU cluster name (data type: DATA_TYPE_STRING) */

	/* Bus Resource Attribute ID */
	RESOURCE_MONITOR_BUS_ATTR_CUR_FREQ		= (1ULL << 0),				/**< Current bus frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_BUS_ATTR_MIN_FREQ		= (1ULL << 1),				/**< Current bus minimum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_BUS_ATTR_MAX_FREQ		= (1ULL << 2),				/**< Current bus maximum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_BUS_ATTR_AVAILABLE_MIN_FREQ	= (1ULL << 3),				/**< Available bus minimum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_BUS_ATTR_AVAILABLE_MAX_FREQ	= (1ULL << 4),				/**< Available bus maximum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_BUS_ATTR_CUR_GOVERNOR		= (1ULL << 5),				/**< Current bus frequency governor name (data type: DATA_TYPE_STRING) */
	RESOURCE_MONITOR_BUS_ATTR_NAME			= (1ULL << 6),				/**< Bus device name (data type: DATA_TYPE_STRING) */

	/* GPU Resource Attribute ID */
	RESOURCE_MONITOR_GPU_ATTR_CUR_FREQ		= (1ULL << 0),				/**< Current GPU frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_GPU_ATTR_MIN_FREQ		= (1ULL << 1),				/**< Current GPU minimum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_GPU_ATTR_MAX_FREQ		= (1ULL << 2),				/**< Current GPU maximum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_GPU_ATTR_AVAILABLE_MIN_FREQ	= (1ULL << 3),				/**< Available GPU minimum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_GPU_ATTR_AVAILABLE_MAX_FREQ	= (1ULL << 4),				/**< Available GPU maximum frequency (data type: DATA_TYPE_INT, unit: kHz) */
	RESOURCE_MONITOR_GPU_ATTR_CUR_GOVERNOR		= (1ULL << 5),				/**< Current GPU frequency governor name (data type: DATA_TYPE_STRING) */
	RESOURCE_MONITOR_GPU_ATTR_NAME			= (1ULL << 6),				/**< GPU device name (data type: DATA_TYPE_STRING) */

	/* Memory Resource Attribute ID */
	RESOURCE_MONITOR_MEMORY_ATTR_TOTAL		= (1ULL << 0),				/**< Memory total size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_AVAILABLE		= (1ULL << 1),				/**< Memory available size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_FREE		= (1ULL << 2),				/**< Memory free size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_BUFFER		= (1ULL << 3),				/**< Memory buffer size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_CACHED		= (1ULL << 4),				/**< Memory cached size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_CMA_TOTAL		= (1ULL << 5),				/**< CMA memory total size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_CMA_FREE		= (1ULL << 6),				/**< CMA memory free size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_SWAP_TOTAL		= (1ULL << 7),				/**< Swap memory total size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_MEMORY_ATTR_SWAP_FREE		= (1ULL << 8),				/**< Swap memory free size (data type: DATA_TYPE_UINT64, unit: kB) */

	/* Battery Resource Attribute ID */
	RESOURCE_MONITOR_BATTERY_ATTR_CAPACITY		= (1ULL << 0),				/**< Battery capacity (data type: DATA_TYPE_INT, unit: %) */
	RESOURCE_MONITOR_BATTERY_ATTR_STATUS		= (1ULL << 1),				/**< Battery status (data type: DATA_TYPE_STRING) */
	RESOURCE_MONITOR_BATTERY_ATTR_TEMPERATURE	= (1ULL << 2),				/**< Battery temperature (data type: DATA_TYPE_INT) */
	RESOURCE_MONITOR_BATTERY_ATTR_VOLTAGE_NOW	= (1ULL << 3),				/**< Battery voltage value (data type: DATA_TYPE_INT, unit: uV) */
	RESOURCE_MONITOR_BATTERY_ATTR_CURRENT_NOW	= (1ULL << 4),				/**< Battery current value (data type: DATA_TYPE_INT, unit: uA) */
	RESOURCE_MONITOR_BATTERY_ATTR_PRESENT		= (1ULL << 5),				/**< Battery connected status (data type: DATA_TYPE_INT) */
	RESOURCE_MONITOR_BATTERY_ATTR_ONLINE		= (1ULL << 6),				/**< Battery Charger connector status (data type: DATA_TYPE_INT) */

	/* Display Resource Attribute ID */
	RESOURCE_MONITOR_DISPLAY_ATTR_FPS		= (1ULL << 0),				/**< Frame per second (data type: DATA_TYPE_DOUBLE) */
	RESOURCE_MONITOR_DISPLAY_ATTR_NAME		= (1ULL << 1),				/**< Display device name (data type: DATA_TYPE_STRING, unit: fps) */

	/* System Resource Attribute ID */
	RESOURCE_MONITOR_SYSTEM_ATTR_CPU_UTIL		= (1ULL << 0),				/**< CPU average utilization (data type: DATA_TYPE_DOUBLE, unit: %) */
	RESOURCE_MONITOR_SYSTEM_ATTR_CPU_USER_UTIL	= (1ULL << 1),				/**< CPU average utilization on user (data type: DATA_TYPE_DOUBLE, unit: %) */
	RESOURCE_MONITOR_SYSTEM_ATTR_CPU_SYS_UTIL	= (1ULL << 2),				/**< CPU average utilization on system (data type: DATA_TYPE_DOUBLE, unit: %) */
	RESOURCE_MONITOR_SYSTEM_ATTR_PER_CPU_UTIL	= (1ULL << 3),				/**< Per-CPU utilization (data type: DATA_TYPE_ARRAY_DOUBLE, unit: %) */
	RESOURCE_MONITOR_SYSTEM_ATTR_PER_CPU_USER_UTIL	= (1ULL << 4),				/**< Per-CPU utilization on user (data type: DATA_TYPE_ARRAY_DOUBLE, unit: %) */
	RESOURCE_MONITOR_SYSTEM_ATTR_PER_CPU_SYS_UTIL	= (1ULL << 5),				/**< Per-CPU utilization on system (data type: DATA_TYPE_ARRAY_DOUBLE, unit: %) */
	RESOURCE_MONITOR_SYSTEM_ATTR_POSSIBLE_CPU	= (1ULL << 6),				/**< Number of possible CPU (data type: DATA_TYPE_INT, unit: ea) */
	RESOURCE_MONITOR_SYSTEM_ATTR_ONLINE_CPU		= (1ULL << 7),				/**< Number of online CPU (data type: DATA_TYPE_INT, unit: ea) */

	/* Disk Resource Attribute ID */
	RESOURCE_MONITOR_DISK_ATTR_NAME			= (1ULL << 0),				/**< Disk device name (data type: DATA_TYPE_STRING) */
	RESOURCE_MONITOR_DISK_ATTR_READ_PER_SEC		= (1ULL << 1),				/**< Disk read per second (data type: DATA_TYPE_DOUBLE, unit: kB/s) */
	RESOURCE_MONITOR_DISK_ATTR_WRITE_PER_SEC	= (1ULL << 2),				/**< Disk write per second (data type: DATA_TYPE_DOUBLE, unit: kB/s) */
	RESOURCE_MONITOR_DISK_ATTR_READ_TOTAL		= (1ULL << 3),				/**< Disk read total size (data type: DATA_TYPE_UINT64, unit: kB) */
	RESOURCE_MONITOR_DISK_ATTR_WRITE_TOTAL		= (1ULL << 4),				/**< Disk write total size (data type: DATA_TYPE_UINT64, unit: kB) */
} resource_monitor_attr_id_e;

/**
 * @brief Enumeration for resource control ID. The device board have the
 * multiple resources with same resource type. The resource control ID
 * indicates the identification of each resource among same resource type.
 * @since_tizen 7.0
 */
typedef enum {
	/* CPU Resource Control ID */
	RESOURCE_MONITOR_CPU_CTRL_CLUSTER_ID		= (1ULL << 0),		/**< CPU cluster resource control ID */

	/* Bus Resource Control ID */
	RESOURCE_MONITOR_BUS_CTRL_DEVICE_ID		= (1ULL << 0),		/**< Bus device resource control ID */

	/* GPU Resource Control ID */
	RESOURCE_MONITOR_GPU_CTRL_DEVICE_ID		= (1ULL << 0),		/**< GPU device resource control ID */

	/* Display Resource Control ID */
	RESOURCE_MONITOR_DISPLAY_CTRL_DEVICE_ID		= (1ULL << 0),		/**< Display device resource control ID */

	/* Disk Resource Control ID */
	RESOURCE_MONITOR_DISK_CTRL_DEVICE_ID		= (1ULL << 0),		/**< Disk device resource control ID */
} resource_monitor_ctrl_id_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_RESOURCE_MONITOR_MODULE
 * @{
 */

/**
 * @brief Initializes the resource monitor which monitors the physical and logical resource.
 * @details A unique ID is required to use the resource monitor. A unique ID is
 * required to use all functions in the resource monitor.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @c return value should be released using resource_monitor_exit().
 * @return @c positive integer as resource monitor ID on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @see    resource_monitor_exit()
 * @code
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Set control value to indicate the specific CPU device.
 *         // It assumes CPU cluster is only one for this example.
 *         int ret = resource_monitor_set_resource_ctrl(monitor_id, resource_id, 0);
 *
 *         // Set interested resource attributes to monitor the CPU current/minimum frequency.
 *         u_int64_t attr_mask = RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ | RESOURCE_MONITOR_CPU_ATTR_MIN_FREQ;
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, attr_mask);
 *
 *         // Update resource attribute data of CPU current /minimum frequency..
 *         ret = resource_monitor_update(monitor_id);
 *
 *         // Get the monitored resource attribute data of CPU current/minimum frequency.
 *         int cpu_cur_freq;
 *         int cpu_min_freq;
 *         ret = resource_monitor_get_value_int(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ, &cpu_cur_freq);
 *         ret = resource_monitor_get_value_int(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_MIN_FREQ, &cpu_min_freq);
 *
 *         // Delete monitor_id
 *         resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_init(void);

/**
 * @brief Exits the resource monitor to finish the monitoring of physical and logical resource.
 * @details The resource monitor is terminated using the unique ID returned from
 * the resource_monitor_init() function. The unique ID is no longer valid.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     ID of resource monitor which be returned by resource_monitor_init()
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @see    resource_monitor_init()
 * @code
 *         int ret;
 *         int monitor_id = resource_monitor_init();
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_exit(int monitor_id);

/**
 * @brief Gets the count of supported resources according to resource type.
 * @details Returns the number of resources when one hardware board contains
 * multiple resources of the same type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitorID
 * @param[in] resource_type  Resource type
 * @param[out] resource_count  Resource count retrieved from resource monitor
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_RESOURCE       Unavailable Resource Type
 * @see    resource_monitor_init()
 * @code
 *         int count;
 *         int ret;
 *         int monitor_id = resource_monitor_init();
 *
 *         // Get the count of the CPU resource
 *         ret = resource_monitor_get_resource_count(monitor_id, RESOURCE_MONITOR_TYPE_CPU, &count);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_resource_count(int monitor_id, resource_monitor_type_e resource_type, int *resource_count);

/**
 * @brief Creates resource for given resource_type which indicates the one of
 * various resource type.
 * @details The various kinds of resources can be monitored using unique IDs
 * generated using the resource_monitor_init function. Create the resource id
 * required to monitor the resource.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @c return value should be released using resource_delete_resource().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_type  Resource type
 * @return @c positive integer as resource ID on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_RESOURCE       Unavailable Resource Type
 * @see    resource_monitor_init()
 * @see    resource_monitor_delete_resource()
 * @code
 *         int ret;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_create_resource(int monitor_id, resource_monitor_type_e resource_type);

/**
 * @brief Deletes resource of given resource ID which indicates the resource
 * instance.
 * @details Deletes the resource_id when monitoring of a specific resource
 * is no longer performed using the resource_monitor_create_resource function.
 * Resource_id is no longer valid.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @code
 *         int ret;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_delete_resource(int monitor_id, int resource_id);

/**
 * @brief Sets the resource control with value which is different according to resource control id.
 * @details There may be multiple resources of the same type. A specific resource
 * must be set to point to a specific resource by setting ctrl_id prior to the monitoring request.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] ctrl_id        Resource control ID
 * @param[in] value          Value for resource control ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @code
 *         int ret;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Set resource control
 *         ret = resource_monitor_set_resource_ctrl(monitor_id, resource_id,
 *                                   RESOURCE_MONITOR_CPU_CTRL_CLUSTER_ID, 0);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_set_resource_ctrl(int monitor_id, int resource_id, resource_monitor_ctrl_id_e ctrl_id, int value);

/**
 * @brief Sets the resource attributes of interest for monitoring.
 * @details The data that can be monitored for each individual resource is different.
 * Each monitorable data is represented by a resource_monitor_attr_id_e.
 * You must set the interest for the resource attribute you want to monitor before the monitoring request.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_mask      Attribute mask including the various attributes
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_unset_resource_attr()
 * @code
 *         int ret;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Set resource monitor
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_set_resource_attr(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_mask);

/**
 * @brief Unsets the resource attributes of interest for monitoring.
 * @details The data that can be monitored for each individual resource is different.
 * Each monitorable data is represented by a resource_monitor_attr_id_e.
 * The interest must be removed for the resource attribute you want to remove from the monitoring target.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_mask      Attribute mask including the various attributes
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @code
 *         int ret;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Set and unsert resource monitor
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ);
 *         ret = resource_monitor_unset_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_unset_resource_attr(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_mask);

/**
 * @brief Checks whether a resource attribute is supported or not.
 * @details Each monitorable data is represented by a resource_monitor_attr_id_e.
 * You can check if monitoring of a specific resource attribute is supported.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] supported     Resource attributes is either supported or not (@c true  == supported, @c false = not supported)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @code
 *         int ret;
 *         bool is_supported;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Check whether the corresponding resource is supported
 *         ret = resource_monitor_is_resource_attr_supported(monitor_id, resource_id,
 *                               RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ, &is_supported);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_is_resource_attr_supported(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, bool *supported);

/**
 * @brief Updates the value of attributes of interest for all created resource.
 * @details Requests monitoring of all resource types generated using unique IDs
 * generated using the resource_monitor_init function.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @code
 *         int ret;
 *         // Create monitor_id
 *         int monitor_id = resource_monitor_init();
 *
 *         // Update all resource of monitor_id
 *         ret = resource_monitor_update(monitor_id);
 *
 *         // Delete monitor_id
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_update(int monitor_id);

/**
 * @brief Updates the value of attributes of interest for a resource.
 * @details Requests monitoring of specific resource instances generated using
 * unique IDs generated using the resource_monitor_init function.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @code
 *         int ret;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Set resource control and attribute
 *         ret = resource_monitor_set_resource_ctrl(monitor_id, resource_id,
 *                                   RESOURCE_MONITOR_CPU_CTRL_CLUSTER_ID, 0);
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ);
 *
 *         // Update the corresponding resource of monitor_id
 *         ret = resource_monitor_update_resource(monitor_id, resource_id);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_update_resource(int monitor_id, int resource_id);

/**
 * @brief Gets the int value of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an integer 32-bit data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] value         Value retrieved from resource attribute
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         int32_t value;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_int(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_CUR_FREQ, &value);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_value_int(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, int32_t *value);

/**
 * @brief Gets the int64 value of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an integer 64-bit data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] value         Value retrieved from resource attribute
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         int64_t value;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, ...);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, ...);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_int64(monitor_id, resource_id, ..., &value);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_value_int64(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, int64_t *value);

/**
 * @brief Gets the uint32 value of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an unsigned integer 32-bit data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] value         Value retrieved from resource attribute
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         uint32_t value;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, ...);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, ...);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_uint32(monitor_id, resource_id, ..., &value);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_value_uint32(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, u_int32_t *value);

/**
 * @brief Gets the uint64 value of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an unsigned integer 64-bit data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] value         Value retrieved from resource attribute
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         uint64_t value;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_MEMORY);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_MEMORY_ATTR_TOTAL);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_uint64(monitor_id, resource_id, RESOURCE_MONITOR_MEMORY_ATTR_TOTAL, &value);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_value_uint64(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, u_int64_t *value);

/**
 * @brief Gets the double value of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an double data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] value         Value retrieved from resource attribute
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         double value;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_DISPLAY);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_DISPLAY_ATTR_FPS);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_double(monitor_id, resource_id, RESOURCE_MONITOR_DISPLAY_ATTR_FPS, &value);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_value_double(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, double *value);

/**
 * @brief Gets the string value of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an double data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @a value should be released using free().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] value         Value retrieved from resource attribute
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         char *value;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_CPU);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_NAME);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_string(monitor_id, resource_id, RESOURCE_MONITOR_CPU_ATTR_NAME, &value);
 *         free(value);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_value_string(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, char **value);

/**
 * @brief Gets the int array of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an int-32bit array data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @a array should be released using free().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] array         Array retrieved from resource attribute
 * @param[out] length        Length of array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         int32_t *array = NULL;
 *         int length;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, ...);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, ...);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_double(monitor_id, resource_id, ..., &array, &length);
 *         free(array);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_array_int(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, int32_t **array, int *length);

/**
 * @brief Gets the int64 array of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an int-64bit array data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @a array should be released using free().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] array         Array retrieved from resource attribute
 * @param[out] length        Length of array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         int64_t *array = NULL;
 *         int length;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, ...);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, ...);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_double(monitor_id, resource_id, ..., &array, &length);
 *         free(array);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_array_int64(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, int64_t **array, int *length);

/**
 * @brief Gets the uint32 array of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an unsigned int-32bit array data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @a array should be released using free().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] array         Array retrieved from resource attribute
 * @param[out] length        Length of array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         uint32_t *array = NULL;
 *         int length;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, ...);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, ...);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_double(monitor_id, resource_id, ..., &array, &length);
 *         free(array);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_array_uint32(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, u_int32_t **array, int *length);

/**
 * @brief Gets the uint64 array of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an unsigned int-64bit array data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @a array should be released using free().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] array         Array retrieved from resource attribute
 * @param[out] length        Length of array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         uint64_t *array = NULL;
 *         int length;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, ...);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, ...);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_double(monitor_id, resource_id, ..., &array, &length);
 *         free(array);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_array_uint64(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, u_int64_t **array, int *length);

/**
 * @brief Gets the double array of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an double array data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @a array should be released using free().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] array         Array retrieved from resource attribute
 * @param[out] length        Length of array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         double *array = NULL;
 *         int length;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, RESOURCE_MONITOR_TYPE_SYSTEM);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, RESOURCE_MONITOR_SYSTEM_ATTR_PER_CPU_UTIL);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_double(monitor_id, resource_id, RESOURCE_MONITOR_SYSTEM_ATTR_PER_CPU_UTIL, &array, &length);
 *         free(array);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_array_double(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, double **array, int *length);

/**
 * @brief Gets the string array of resource attribute which indicates one characteristics of resource.
 * @details Obtain a resource attribute value having an string array data type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/systemmonitor
 * @remarks The @a array and each data in the array should be released using free().
 * @param[in] monitor_id     Resource monitor ID which be returned by resource_monitor_init()
 * @param[in] resource_id    Resource ID which be returned by resource_monitor_create_resource()
 * @param[in] attr_id        Resource attribute ID
 * @param[out] array         Array retrieved from resource attribute
 * @param[out] length        Length of array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #RESOURCE_MONITOR_ERROR_NONE                         Successful
 * @retval #RESOURCE_MONITOR_ERROR_PERMISSION_DENIED            Permission Denied
 * @retval #RESOURCE_MONITOR_ERROR_INVALID_PARAMETER            Invalid Parameter
 * @retval #RESOURCE_MONITOR_ERROR_NO_DATA                      Empty Data
 * @retval #RESOURCE_MONITOR_ERROR_OUT_OF_MEMORY                Out of Memory
 * @retval #RESOURCE_MONITOR_ERROR_NOT_SUPPORTED_ATTRIBUTE      Unavailable Attribute
 * @see    resource_monitor_init()
 * @see    resource_monitor_create_resource()
 * @see    resource_monitor_set_resource_attr()
 * @see    resource_monitor_set_resource_ctrl()
 * @see    resource_monitor_update()
 * @see    resource_monitor_update_resource()
 * @code
 *         int ret;
 *         char **array = NULL;
 *         int length;
 *         // Create monitor_id and resource_id for CPU device.
 *         int monitor_id = resource_monitor_init();
 *         int resource_id = resource_monitor_create_resource(monitor_id, ...);
 *
 *         // Set resource attribute
 *         ret = resource_monitor_set_resource_attr(monitor_id, resource_id, ...);
 *
 *         // Get resource attribute value
 *         ret = resource_monitor_get_value_double(monitor_id, resource_id, ..., &array, &length);
 *         for (int i = 0; i < length; i++)
 *                   free(array[i]);
 *
 *         free(array);
 *
 *         // Delete monitor_id and resource_id for CPU device.
 *         ret = resource_monitor_delete_resource(monitor_id, resource_id);
 *         ret = resource_monitor_exit(monitor_id);
 * @endcode
 */
int resource_monitor_get_array_string(int monitor_id, int resource_id, resource_monitor_attr_id_e attr_id, char ***array, int *length);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_MONITOR_RESOURCE_MONITOR_H__ */
