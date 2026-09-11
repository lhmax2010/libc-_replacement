/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <utils_i18n_types.h>

/**
 * @file utils_i18n_utmscale.h
 * @version 0.1
 * @brief utils_i18n_utmscale
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE Utmscale
 * @brief Universal Time Scale
 * @section CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE_HEADER Required Header
 *			\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE_OVERVIEW Overview
 * @details There are quite a few different conventions for binary datetime,
 *			depending on different platforms and protocols. Some of these have severe drawbacks.
 *			For example, people using Unix time (seconds since Jan 1, 1970) think that they are safe until near the year 2038.
 *			But cases can and do arise where arithmetic manipulations causes serious problems.
 *			Consider the computation of the average of two datetimes,
 *			for example: if one calculates them with average_time = (time1 + time2)/2,
 *			there will be overflow even with dates around the present.
 *			Moreover, even if these problems don't occur, there is the issue of conversion back and forth between different systems.
 *
 *			<p>
 *			Binary datetimes differ in a number of ways: the datatype, the unit,
 *			and the epoch (origin). ICU refers to these as time scales. For example:
 *
 *			<table border="1" cellspacing="0" cellpadding="4">
 *			 <caption>Table 1: Binary Time Scales</caption>
 *			 <tr>
 *			  <th align="left">Source</th>
 *			  <th align="left">Datatype</th>
 *			  <th align="left">Unit</th>
 *			  <th align="left">Epoch</th>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_JAVA_TIME</td>
 *			  <td>int64_t</td>
 *			  <td>milliseconds</td>
 *			  <td>Jan 1, 1970</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_UNIX_TIME</td>
 *			  <td>int32_t or int64_t</td>
 *			  <td>seconds</td>
 *			  <td>Jan 1, 1970</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_ICU4C_TIME</td>
 *			  <td>double</td>
 *			  <td>milliseconds</td>
 *			  <td>Jan 1, 1970</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_WINDOWS_FILE_TIME</td>
 *			  <td>int64_t</td>
 *			  <td>ticks (100 nanoseconds)</td>
 *			  <td>Jan 1, 1601</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_DOTNET_DATE_TIME</td>
 *			  <td>int64_t</td>
 *			  <td>ticks (100 nanoseconds)</td>
 *			  <td>Jan 1, 0001</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_MAC_OLD_TIME</td>
 *			  <td>int32_t or int64_t</td>
 *			  <td>seconds</td>
 *			  <td>Jan 1, 1904</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_MAC_TIME</td>
 *			  <td>double</td>
 *			  <td>seconds</td>
 *			  <td>Jan 1, 2001</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_EXCEL_TIME</td>
 *			  <td>?</td>
 *			  <td>days</td>
 *			  <td>Dec 31, 1899</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_DB2_TIME</td>
 *			  <td>?</td>
 *			  <td>days</td>
 *			  <td>Dec 31, 1899</td>
 *			 </tr>
 *			 <tr>
 *			  <td>UDTS_UNIX_MICROSECONDS_TIME</td>
 *			  <td>int64_t</td>
 *			  <td>microseconds</td>
 *			  <td>Jan 1, 1970</td>
 *			 </tr>
 *			</table>
 *
 *			<p>
 *			All of the epochs start at 00:00 am (the earliest possible time on the day in question),
 *			and are assumed to be UTC.
 *
 *			<p>
 *			The ranges for different datatypes are given in the following table (all values in years).
 *			The range of years includes the entire range expressible with positive and negative
 *			values of the datatype. The range of years for double is the range that would be allowed
 *			without losing precision to the corresponding unit.
 *
 *			<table border="1" cellspacing="0" cellpadding="4">
 *			<tr>
 *			 <th align="left">Units</th>
 *			 <th align="left">int64_t</th>
 *			 <th align="left">double</th>
 *			 <th align="left">int32_t</th>
 *			</tr>
 *			<tr>
 *			 <td>1 sec</td>
 *			 <td align="right">5.84542x10<sup>11</sup></td>
 *			 <td align="right">285,420,920.94</td>
 *			 <td align="right">136.10</td>
 *			</tr>
 *			<tr>
 *			 <td>1 millisecond</td>
 *			 <td align="right">584,542,046.09</td>
 *			 <td align="right">285,420.92</td>
 *			 <td align="right">0.14</td>
 *			</tr>
 *			<tr>
 *			 <td>1 microsecond</td>
 *			 <td align="right">584,542.05</td>
 *			 <td align="right">285.42</td>
 *			 <td align="right">0.00</td>
 *			</tr>
 *			<tr>
 *			 <td>100 nanoseconds (tick)</td>
 *			 <td align="right">58,454.20</td>
 *			 <td align="right">28.54</td>
 *			 <td align="right">0.00</td>
 *			</tr>
 *			<tr>
 *			 <td>1 nanosecond</td>
 *			 <td align="right">584.5420461</td>
 *			 <td align="right">0.2854</td>
 *			 <td align="right">0.00</td>
 *			</tr>
 *		   </table>
 *
 *		   <p>
 *		   These functions implement a universal time scale which can be used as a 'pivot',
 *		   and provide conversion functions to and from all other major time scales.
 *		   This datetimes to be converted to the pivot time, safely manipulated,
 *		   and converted back to any other datetime time scale.
 *
 *		   <p>
 *		   So what to use for this pivot? Java time has plenty of range, but cannot represent
 *		   .NET <code>System.DateTime</code> values without severe loss of precision. ICU4C time addresses this by using a
 *		   <code>double</code> that is otherwise equivalent to the Java time. However, there are disadvantages
 *		   with <code>doubles</code>. They provide for much more graceful degradation in arithmetic operations.
 *		   But they only have 53 bits of accuracy, which means that they will lose precision when
 *		   converting back and forth to ticks. What would really be nice would be a
 *		   <code>long double</code> (80 bits -- 64 bit mantissa), but that is not supported on most systems.
 *
 *		   <p>
 *		   The Unix extended time uses a structure with two components: time in seconds and a
 *		   fractional field (microseconds). However, this is clumsy, slow, and
 *		   prone to error (you always have to keep track of overflow and underflow in the
 *		   fractional field). <code>BigDecimal</code> would allow for arbitrary precision and arbitrary range,
 *		   but the ICU team did not want to use this as the normal type, because it is slow and does not
 *		   have a fixed size.
 *
 *		   <p>
 *		   Because of these issues, the ICU team ended up concluding that the .NET framework's
 *		   <code>System.DateTime</code> would be the best pivot. However, ICU uses the full range
 *		   allowed by the datatype, allowing for datetimes back to 29,000 BC and up to 29,000 AD.
 *		   This time scale is very fine grained, does not lose precision, and covers a range that
 *		   will meet almost all requirements. It will not handle the range that Java times do,
 *		   but frankly, being able to handle dates before 29,000 BC or after 29,000 AD is of very limited interest.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE
 * @{
 */

/**
 * @brief Gets a value associated with a particular time scale.
 * @since_tizen 3.0
 *
 * @param[in] time_scale The time scale
 * @param[in] utmscale_val A constant representing the value to get
 * @param[out] value The obtained value
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_utmscale_get_time_scale_value(i18n_utmscale_scale_e time_scale, i18n_utmscale_value_e utmscale_val, int64_t *value);

/**
 * @brief Converts the time given as an int64_t to Universal Time Scale
 * @since_tizen 3.0
 *
 * @param[in] other_time The int64_t datetime
 * @param[in] time_scale The time scale to convert from
 * @param[out] universal_time The datetime converted to the universal time scale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER The conversion is out of range.
 */
int i18n_utmscale_from_int64(int64_t other_time, i18n_utmscale_scale_e time_scale, int64_t *universal_time);

/**
 * @brief Converts the time given as Universal Time Scale to an int64_t.
 * @since_tizen 3.0
 *
 * @param[in] universal_time The datetime in the Universal Time Scale
 * @param[in] time_scale The time scale to convert to
 * @param[out] other_time The datetime converted to the given time scale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER The conversion is out of range.
 */
int i18n_utmscale_to_int64(int64_t universal_time, i18n_utmscale_scale_e time_scale, int64_t *other_time);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
