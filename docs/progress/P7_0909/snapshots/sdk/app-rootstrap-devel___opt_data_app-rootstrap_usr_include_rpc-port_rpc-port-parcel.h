/*
 * Copyright (c) 2018 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_RPC_PORT_PARCEL_INCLUDE_H__
#define __TIZEN_APPFW_RPC_PORT_PARCEL_INCLUDE_H__

#include <stdbool.h>
#include <sys/time.h>

#include <bundle.h>
#include <rpc-port.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_RPC_PORT_PARCEL_MODULE
 * @{
 */

/**
 * @brief The rpc port parcel handle.
 * @since_tizen 4.0
 */
typedef void *rpc_port_parcel_h;

/**
 * @brief The header handle of the rpc port parcel.
 * @since_tizen 6.5
 */
typedef void *rpc_port_parcel_header_h;

/**
 * @brief The interface for converting data to/from a parcel.
 * @since_tizen 4.0
 */
typedef struct __rpc_port_parcelable {
	void (*to)(rpc_port_parcel_h h, void *data);  /**< The function pointer to read from parcel */
	void (*from)(rpc_port_parcel_h h, void *data);  /**< The function pointer to write to parcel */
} rpc_port_parcelable_t;

/**
 * @brief Creates a rpc port parcel handle.
 * @since_tizen 4.0
 * @remarks You must release @a h using rpc_port_parcel_destroy().
 * @param[out] h The rpc port parcel handle that is newly created
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see rpc_port_parcel_destroy()
 */
int rpc_port_parcel_create(rpc_port_parcel_h *h);

/**
 * @brief Creates a rpc port parcel handle from port.
 * @details Creates a rpc port parcel handle using read data from the port.
 *          It calls rpc_port_read() internally.
 * @since_tizen 4.0
 * @remarks You must release @a h using rpc_port_parcel_destroy().
 * @param[out] h The rpc port parcel handle that is newly created
 * @param[in] port The rpc port handle for creating handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a port is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR Internal I/O error
 * @see rpc_port_parcel_destroy()
 * @see rpc_port_parcel_send()
 */
int rpc_port_parcel_create_from_port(rpc_port_parcel_h *h, rpc_port_h port);

/**
 * @brief Sends parcel data through the port.
 * @details Sends parcel data through the port. It calls rpc_port_write()
 *          internally.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle that is newly created
 * @param[in] port The rpc port handle for writing data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR Internal I/O error
 * @see rpc_port_parcel_create_from_port()
 */
int rpc_port_parcel_send(rpc_port_parcel_h h, rpc_port_h port);

/**
 * @brief Destroys a rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_create()
 */
int rpc_port_parcel_destroy(rpc_port_parcel_h h);

/**
 * @brief Writes a byte value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] b Byte data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_byte()
 */
int rpc_port_parcel_write_byte(rpc_port_parcel_h h, char b);

/**
 * @brief Writes a short value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] i short data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_int16()
 */
int rpc_port_parcel_write_int16(rpc_port_parcel_h h, short i);

/**
 * @brief Writes a integer value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] i int data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_int32()
 */
int rpc_port_parcel_write_int32(rpc_port_parcel_h h, int i);

/**
 * @brief Writes a long long integer value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] i long long data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_int64()
 */
int rpc_port_parcel_write_int64(rpc_port_parcel_h h, long long i);

/**
 * @brief Writes a floating point value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] f float data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_float()
 */
int rpc_port_parcel_write_float(rpc_port_parcel_h h, float f);

/**
 * @brief Writes a double precision floating point value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] d double data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_double()
 */
int rpc_port_parcel_write_double(rpc_port_parcel_h h, double d);

/**
 * @brief Writes a string value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] str string data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_string()
 */
int rpc_port_parcel_write_string(rpc_port_parcel_h h, const char *str);

/**
 * @brief Writes a boolean value into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] b boolean data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_bool()
 */
int rpc_port_parcel_write_bool(rpc_port_parcel_h h, bool b);

/**
 * @brief Writes a bundle data into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] b Bundle data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_bundle()
 */
int rpc_port_parcel_write_bundle(rpc_port_parcel_h h, bundle *b);

/**
 * @brief Writes a count for array into rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] count Array count
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read_array_count()
 */
int rpc_port_parcel_write_array_count(rpc_port_parcel_h h, int count);

/**
 * @brief Writes the data into parcel handle using @a parcelable.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] parcelable The interface to write the data into parcel handle
 * @param[in] data Data which write into parcel
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_read()
 */
int rpc_port_parcel_write(rpc_port_parcel_h h, rpc_port_parcelable_t *parcelable, void *data);

/**
 * @brief Reads a byte value from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] b Byte data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_byte()
 */
int rpc_port_parcel_read_byte(rpc_port_parcel_h h, char *b);

/**
 * @brief Reads a short value from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] i short data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_int16()
 */
int rpc_port_parcel_read_int16(rpc_port_parcel_h h, short *i);

/**
 * @brief Reads a integer value from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] i int data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_int32()
 */
int rpc_port_parcel_read_int32(rpc_port_parcel_h h, int *i);

/**
 * @brief Reads a long long integer value from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] i long long data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_int64()
 */
int rpc_port_parcel_read_int64(rpc_port_parcel_h h, long long *i);

/**
 * @brief Reads a floating point value from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] f float data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_float()
 */
int rpc_port_parcel_read_float(rpc_port_parcel_h h, float *f);

/**
 * @brief Reads a double precision floating point value from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] d double data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_double()
 */
int rpc_port_parcel_read_double(rpc_port_parcel_h h, double *d);

/**
 * @brief Reads a string value from rpc port parcel handle.
 * @since_tizen 4.0
 * @remarks The @a str should be released using free().
 * @param[in] h The rpc port parcel handle
 * @param[out] str string data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_string()
 */
int rpc_port_parcel_read_string(rpc_port_parcel_h h, char **str);

/**
 * @brief Reads a boolean value from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] b boolean data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_bool()
 */
int rpc_port_parcel_read_bool(rpc_port_parcel_h h, bool *b);

/**
 * @brief Reads a bundle data from rpc port parcel handle.
 * @since_tizen 4.0
 * @remarks The @a b should be released using bundle_free().
 * @param[in] h The rpc port parcel handle
 * @param[out] b Bundle data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_bundle()
 */
int rpc_port_parcel_read_bundle(rpc_port_parcel_h h, bundle **b);

/**
 * @brief Reads a count for array from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] count Array count
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_array_count()
 */
int rpc_port_parcel_read_array_count(rpc_port_parcel_h h, int *count);

/**
 * @brief Reads a parcel from the data using @a parcelable.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] parcelable The interface to get data from parcel handle
 * @param[in] data Data which get from parcel
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write()
 */
int rpc_port_parcel_read(rpc_port_parcel_h h, rpc_port_parcelable_t *parcelable, void *data);

/**
 * @brief Reads bytes from rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[out] buf The array buffer to read
 * @param[in] size Bytes to read
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_bool()
 */
int rpc_port_parcel_burst_read(rpc_port_parcel_h h, unsigned char *buf, unsigned int size);

/**
 * @brief Writes bytes to rpc port parcel handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port parcel handle
 * @param[in] buf The array buffer to write
 * @param[in] size Bytes to write
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_parcel_write_bool()
 */
int rpc_port_parcel_burst_write(rpc_port_parcel_h h, const unsigned char *buf, unsigned int size);

/**
 * @brief Gets the header handle of the rpc port parcel.
 * @since_tizen 6.5
 * @remarks The @a header is managed by the platform and will be released when rpc_port_parcel_destroy() is called.
 * @param[in] h The rpc port parcel handle
 * @param[out] header The header handle of the rpc port parcel
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_parcel_get_header(rpc_port_parcel_h h, rpc_port_parcel_header_h *header);

/**
 * @brief Sets the tag to the header handle of the rpc port parcel.
 * @since_tizen 6.5
 * @param[in] header The header handle of the rpc port parcel
 * @param[in] tag The tag
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see rpc_port_parcel_header_get_tag()
 */
int rpc_port_parcel_header_set_tag(rpc_port_parcel_header_h header, const char *tag);

/**
 * @brief Gets the tag from the header handle of the rpc port parcel.
 * @since_tizen 6.5
 * @remarks The @a tag should be released using free().
 * @param[in] header The header handle of the rpc port parcel
 * @param[out] tag The tag
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_OUT_OF_MEMORY Out of memory
 * @see rpc_port_parcel_header_set_tag()
 */
int rpc_port_parcel_header_get_tag(rpc_port_parcel_header_h header, char **tag);

/**
 * @brief Sets the sequence number to the header handle of the rpc port parcel.
 * @since_tizen 6.5
 * @param[in] header The header handle of the rpc port parcel
 * @param[in] seq_num The sequence number
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see rpc_port_parcel_header_get_seq_num()
 */
int rpc_port_parcel_header_set_seq_num(rpc_port_parcel_header_h header, int seq_num);

/**
 * @brief Gets the sequence number from the header handle of the rpc port parcel.
 * @since_tizen 6.5
 * @param[in] header The header handle of the rpc port parcel
 * @param[out] seq_num The sequence number
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see rpc_port_parcel_header_set_seq_num()
 */
int rpc_port_parcel_header_get_seq_num(rpc_port_parcel_header_h header, int *seq_num);

/**
 * @brief Gets the timestamp from the header handle of the rpc port parcel.
 * @since_tizen 6.5
 * @remarks The @a timestamp represents monotonic time since some unspecified starting point.
 *          To get elapsed time properly, you have to get the timestamp using the clock_gettime() with CLOCK_MONITONIC_RAW.
 * @param[in] header The header handle of the rpc port parcel
 * @param[out] timestamp The timestamp
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_parcel_header_get_timestamp(rpc_port_parcel_header_h header, struct timespec *timestamp);

/**
 * @brief Gets the raw data of the rpc port parcel handle.
 * @since_tizen 6.5
 * @remarks You MUST NOT release @a raw using free(). It's managed by platform.
 * @param[in] h The rpc port parcel handle
 * @param[out] raw The raw data
 * @param[out] size The size of the raw data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_parcel_get_raw(rpc_port_parcel_h h, void **raw, unsigned int *size);

/**
 * @brief Creates the rpc port parcel handle with given the raw data.
 * @since_tizen 6.5
 * @remarks You must release @a h using rpc_port_parcel_destroy().
 * @param[out] h The rpc port parcel handle
 * @param[in] raw The raw data
 * @param[in] size The size of the raw data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_OUT_OF_MEMORY Out of memory
 * @see rpc_port_parcel_destroy()
 */
int rpc_port_parcel_create_from_raw(rpc_port_parcel_h *h, const void *raw, unsigned int size);

/**
 * @brief Creates the rpc port parcel handle without the header.
 * @since_tizen 8.0
 * @remarks You must release @a h using rpc_port_parcel_destroy().
 * @param[out] h The rpc port parcel handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_OUT_OF_MEMORY Out of memory
 * @see rpc_port_parcel_destroy()
 */
int rpc_port_parcel_create_without_header(rpc_port_parcel_h *h);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_APPFW_RPC_PORT_PARCEL_INCLUDE_H__
