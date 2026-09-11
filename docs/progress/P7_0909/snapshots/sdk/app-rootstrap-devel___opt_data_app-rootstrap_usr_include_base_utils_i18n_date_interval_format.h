/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_date_interval_format.h
 * @version 0.1
 * @brief utils_i18n_date_interval_format
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE DateIntervalFormat
 * @brief Date Interval Format module formats date intervals in a language-independent manner.
 * @section CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE_HEADER Required Header
 *          \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE_OVERVIEW Overview
 * @details Date Interval Format module formats a Date Interval into text as compactly as possible.
 *          For example, the date interval format from "Jan 11, 2008" to "Jan 18,. 2008" is
 *          "Jan 11-18, 2008" for English.
 *
 *          There is no structural information in date time patterns. For any punctuations and
 *          string literals inside a date time pattern, we do not know whether it is just a separator,
 *          or a prefix, or a suffix. Without such information, so, it is difficult to generate
 *          a sub-pattern (or super-pattern) by algorithm. So, formatting a Date Interval is pattern-driven.
 *
 *          A skeleton
 *          * only keeps the field pattern letter and ignores all other parts in a pattern, such as space,
 *            punctuations, and string literals.
 *          * hides the order of fields.
 *          * might hide a field's pattern letter length.
 *
 *          For those non-digit calendar fields, the pattern letter length is important, such
 *          as MMM, MMMM, and MMMMM; EEE and EEEE, and the field's pattern letter length is honored.
 *
 *          The calendar fields we support for interval formatting are: year, month, date, day-of-week,
 *          am-pm, hour, hour-of-day, minute, and second (though we do not currently have specific
 *          interval format date for skeletons with seconds). Those calendar fields can be defined in
 *          the following order: year > month > date > hour (in day) > minute > second.
 *
 *          The largest different calendar fields between 2 calendars is the first different calendar
 *          field in above order.
 *
 *          For example: the largest different calendar fields between "Jan 10, 2007" and "Feb 20,
 *          2008" is year.
 *
 *          For other calendar fields, the compact interval formatting is not supported. And the interval
 *          format will be fall back to fall-back patterns, which is mostly "{date0} - {date1}".
 *
 *          There is a set of pre-defined static skeleton strings. There are pre-defined interval patterns
 *          for those pre-defined skeletons in locales' resource files. For example, for a skeleton
 *          #I18N_UDATE_YEAR_ABBR_MONTH_DAY, which is "yMMMd", in en_US, if the largest different calendar
 *          field between date1 and date2 is "year", the date interval pattern is "MMM d, yyyy - MMM
 *          d, yyyy", such as "Jan 10, 2007 - Jan 10, 2008". If the largest different calendar field between
 *          date1 and date2 is "month", the date interval pattern is "MMM d - MMM d, yyyy", such as
 *          "Jan 10 - Feb 10, 2007". If the largest different calendar field between date1 and date2 is "day",
 *          the date interval pattern is "MMM d-d, yyyy", such as "Jan 10-20, 2007".
 *
 *          For date skeleton, the interval patterns when year, or month, or date is different are defined
 *          in resource files. For time skeleton, the interval patterns when am/pm, or hour, or minute
 *          is different are defined in resource files.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE
 * @{
 */

/**
 * @brief Creates a date interval format object with a non-specified locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *            i18n_date_interval_fmt_destroy() function.
 *
 * @param[in]    skeleton            The skeleton on which interval format based
 * @param[out]   date_interval_fmt   The created date interval format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_date_interval_fmt_create(const char *skeleton, i18n_date_interval_fmt_h *date_interval_fmt);

/**
 * @brief Creates a date interval format object for the given locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *            i18n_date_interval_fmt_destroy() function.
 *
 * @param[in]   skeleton            The skeleton on which interval format based
 * @param[in]   locale              The locale value
 * @param[out]  date_interval_fmt   The created date interval format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_date_interval_fmt_create_from_locale(const char *skeleton, const char *locale,
                                              i18n_date_interval_fmt_h *date_interval_fmt);

/**
 * @brief Creates a clone of the given @a date_interval_fmt object.
 * @since_tizen 5.0
 * @remarks The @a clone object should be released by the caller with the
 *            i18n_date_interval_fmt_destroy() function.
 *
 * @param[in]    date_interval_fmt   The date interval format object to be cloned
 * @param[out]   clone               The created date interval format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_date_interval_fmt_clone(i18n_date_interval_fmt_h date_interval_fmt,
                                 i18n_date_interval_fmt_h *clone);

/**
 * @brief Destroys the date interval format object.
 * @since_tizen 5.0
 *
 * @param[in]  date_interval_fmt  The date interval format object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_fmt_destroy(i18n_date_interval_fmt_h date_interval_fmt);

/**
 * @brief Checks whether two date interval format objects are equal.
 * @since_tizen 5.0
 *
 * @param[in]   date_interval_fmt_1   The first date interval format object
 * @param[in]   date_interval_fmt_2   The second date interval format object
 * @param[out]  equals               @c true if @a date_interval_fmt1 is equal
 *                                   to @c date_interval_fmt2, @c false otherwise
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_fmt_equals(i18n_date_interval_fmt_h date_interval_fmt_1,
                                  i18n_date_interval_fmt_h date_interval_fmt_2,
                                  bool *equals);

/**
 * @brief Sets the time zone for the calendar used by the #i18n_date_interval_fmt_h object.
 * @since_tizen 5.0
 *
 * @param[in]  date_interval_fmt    The date interval format object
 * @param[in]  timezone             The new time zone
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_fmt_set_time_zone(i18n_date_interval_fmt_h date_interval_fmt,
                                         i18n_timezone_h timezone);

/**
 * @brief Gets the time zone used by the #i18n_date_interval_fmt_h object.
 * @since_tizen 5.0
 * @remarks The obtained timezone should be released by the caller with the
 *          i18n_timezone_destroy() function.
 *
 * @param[in]   date_interval_fmt    The date interval format object
 * @param[out]  timezone             The time zone used by @a date_interval_fmt object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_fmt_get_time_zone(i18n_date_interval_fmt_h date_interval_fmt,
                                         i18n_timezone_h *timezone);

/**
 * @brief Adopts the time zone for the calendar used by the #i18n_date_interval_fmt_h object.
 * @details The caller no longer owns the timezone object and should not delete it
 *          after this call.
 * @since_tizen 5.0
 *
 * @param[in]   date_interval_fmt    The date interval format object
 * @param[out]  timezone             The time zone used by @a date_interval_fmt object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_date_interval_fmt_adopt_time_zone(i18n_date_interval_fmt_h date_interval_fmt,
                                           i18n_timezone_h timezone);

/**
 * @brief Formats a date interval to produce a string.
 * @since_tizen 5.0
 *
 * @param[in]      date_interval_fmt    The date interval format object
 * @param[in]      date_interval        Date interval to be formatted
 * @param[in,out]  append_to            Input/output parameter to receive the result.
 *                                      The result is appended to the existing contents.
 * @param[in]      field_position       The #i18n_field_position_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW      Buffer overflow
 */
int i18n_date_interval_fmt_format_with_date_interval(i18n_date_interval_fmt_h date_interval_fmt,
                                                     i18n_date_interval_h date_interval, char **append_to,
                                                     i18n_field_position_h field_position);

/**
 * @brief Formats two calendars to produce a string.
 * @since_tizen 5.0
 *
 * @param[in]      date_interval_fmt    The date interval format object
 * @param[in]      from_cal             Calendar set to the 'from date' in date interval to be formatted
 *                                      into date interval string
 * @param[in]      to_cal               Calendar set to the 'to date' in date interval to be formatted into
 *                                      date interval string
 * @param[in,out]  append_to            Input/output parameter to receive the result.
 *                                      The result is appended to the existing contents.
 * @param[in]      field_position       The #i18n_field_position_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE                 Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER    Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW      Buffer overflow
 */
int i18n_date_interval_fmt_format_with_calendar(i18n_date_interval_fmt_h date_interval_fmt,
                                                i18n_ucalendar_h from_cal, i18n_ucalendar_h to_cal,
                                                char **append_to, i18n_field_position_h field_position);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
