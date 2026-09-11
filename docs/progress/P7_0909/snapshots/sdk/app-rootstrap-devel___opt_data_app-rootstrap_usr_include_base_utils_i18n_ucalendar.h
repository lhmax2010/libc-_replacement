/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_ucalendar.h
 * @version 0.1
 * @brief utils_i18n_ucalendar
 */

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE Ucalendar
 * @brief The Ucalendar is used for converting between an udate module and a set of integer fields
 *		 such as #I18N_UCALENDAR_YEAR, #I18N_UCALENDAR_MONTH, #I18N_UCALENDAR_DATE, #I18N_UCALENDAR_HOUR, and so on.
 * @section CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 * @section CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE_OVERVIEW Overview
 * @details The Ucalendar is used for converting between an udate module and a set of integer fields
 *		 such as #I18N_UCALENDAR_YEAR, #I18N_UCALENDAR_MONTH, #I18N_UCALENDAR_DATE, #I18N_UCALENDAR_HOUR, and so on.
 *		 (An udate module represents a specific instant in time with millisecond precision. See udate for
 *		 information about the udate.)
 *
 * @section CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Converts the given date and time to the corresponding UTC time(number of seconds that have elapsed since January 1, 1970), considering the given time zone
 * @code

 #define ms2sec(ms) (long long int)(ms)/1000.0

  // get time in sec from input date and time
  long long int _time_convert_itol(char *tzid, int y, int mon, int d, int h, int min, int s)
 {
	 long long int lli;
	 i18n_ucalendar_h ucal;
	 i18n_udate date;
	 int ret = I18N_ERROR_NONE;
	 int year, month, day, hour, minute, second;
	 int len;

	 i18n_uchar *_tzid = NULL;

	 if (tzid == NULL) {
		 tzid = "Etc/GMT";
	 }
	 _tzid = (i18n_uchar*)calloc(strlen(tzid) + 1, sizeof(i18n_uchar));
	 if (_tzid == NULL) {
		 return -1;
	 }
	 // converts 'tzid' to unicode string
	 i18n_ustring_copy_ua(_tzid, tzid);

	 // gets length of '_tzid'
	 len = i18n_ustring_get_length(_tzid);
	 // creates i18n_ucalendar_h
	 ret = i18n_ucalendar_create(_tzid, len, "en_US", I18N_UCALENDAR_TRADITIONAL, &ucal);
	 if (ret) {
		 dlog_print(DLOG_INFO, LOG_TAG, "i18n_ucalendar_create failed.\n");
		 return -1;
	 }

	 // sets i18n_ucalendar_h's date
	 i18n_ucalendar_set_date_time(ucal, y, mon-1, d, h, min, s);

	 // gets the current value of a field from i18n_ucalendar_h
	 i18n_ucalendar_get(ucal, I18N_UCALENDAR_YEAR, &year);
	 i18n_ucalendar_get(ucal, I18N_UCALENDAR_MONTH, &month);
	 i18n_ucalendar_get(ucal, I18N_UCALENDAR_DATE, &day);
	 i18n_ucalendar_get(ucal, I18N_UCALENDAR_HOUR, &hour);
	 i18n_ucalendar_get(ucal, I18N_UCALENDAR_MINUTE, &minute);
	 i18n_ucalendar_get(ucal, I18N_UCALENDAR_SECOND, &second);
	 dlog_print(DLOG_INFO, LOG_TAG, "Date from ucal, year:%d month:%d day:%d hour:%d minute:%d second:%d.\n",year, month, day, hour, minute, second);

	 // gets i18n_ucalendar's current time and converts it from milliseconds to seconds
	 i18n_ucalendar_get_milliseconds(ucal, &date);
	 lli = ms2sec(date);
	 // destroys i18n_ucalendar_h
	 i18n_ucalendar_destroy(ucal);
	 if (_tzid) {
		 free(_tzid);
	 }

	 return lli;
 }
 * @endcode
 *
 * @section CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE_SAMPLE_CODE_2 Sample Code 2
 * @brief Describes an example that uses _time_convert_itol from 'Sample Code 2'
 * @code
	   // converts the given time to UTC time(number of seconds that have elapsed since January 1, 1970)
	 long long int time = _time_convert_itol("Etc/GMT", 2014, 5, 28, 15, 14, 0);
	 dlog_print(DLOG_INFO, LOG_TAG, "Time Zone: %s\t, %d/%d/%d/%d/%d/%d\n", "Etc/GMT", 2014, 5, 28, 15, 14, 0);
	 dlog_print(DLOG_INFO, LOG_TAG, "_time_convert_itol test : %lld\n", time);
 * @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE
 * @{
 */

/**
 * @brief Sets the default time zone.
 * @since_tizen 2.3
 *
 * @param[in] zone_id null-terminated time zone ID
 *
 * @return An #i18n_error_code_e error code
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_set_default_timezone(const i18n_uchar *zone_id);

/**
 * @brief Gets the current date and time.
 * @details The value returned is represented as milliseconds from the epoch.
 * @since_tizen 2.3
 *
 * @param[out] date The current date and time
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_get_now(i18n_udate *date);

/**
 * @brief Creates an #i18n_ucalendar_h.
 * An #i18n_ucalendar_h may be used to convert a millisecond value to a year,
 * month, and day.
 * @details Note: When an unknown TimeZone ID is specified, the #i18n_ucalendar_h returned
 * by the function is initialized with GMT ("Etc/GMT") without any
 * errors/warnings.
 * @since_tizen 2.3
 * @remarks Must release @a calendar using i18n_ucalendar_destroy().
 *
 * @param[in] zone_id The desired TimeZone ID \n
 *					  If @c 0, use the default time zone.
 * @param[in] len The length of the zone ID,
 *					  otherwise @c -1 if null-terminated
 * @param[in] locale The desired locale
 *					If @c NULL, the default locale will be used.
 * @param[in] type The type of #I18N_UCALENDAR_DEFAULT to create \n
 *					This can be #I18N_UCALENDAR_GREGORIAN to create the Gregorian
 * calendar for the locale, or #I18N_UCALENDAR_DEFAULT to create the default calendar for the locale (the
 * default calendar may also be Gregorian).
 * @param[out] calendar A pointer to an #i18n_ucalendar_h,
 *					  otherwise @c 0 if an error occurs
 *
 * @return An #i18n_error_code_e error code
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_ucalendar_create(const i18n_uchar *zone_id, int32_t len, const char *locale, i18n_ucalendar_type_e type, i18n_ucalendar_h *calendar);

/**
 * @brief Destroys an #i18n_ucalendar_h.
 * @details Once destroyed, an #i18n_ucalendar_h may no longer be used.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to destroy
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_destroy(i18n_ucalendar_h calendar);

/**
 * @brief Creates a copy of a #i18n_ucalendar_h.
 * This function performs a deep copy.
 * @since_tizen 2.3
 * @remarks Must release @a identical_to_cal using i18n_ucalendar_destroy().
 *
 * @param[in] cal The #i18n_ucalendar_h to copy
 * @param[out] identical_to_cal A pointer to a #i18n_ucalendar_h identical to @a cal.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_ucalendar_h cal = NULL;
 * 	i18n_ucalendar_h identical_to_cal = NULL;
 * 	i18n_error_code_e error_code;
 *
 * 	error_code = i18n_ucalendar_create(NULL, -1, I18N_ULOCALE_US, I18N_UCALENDAR_DEFAULT, &cal);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_ucalendar_set_date_time(cal, 2000, 1, 1, 1, 1, 1);
 *
 * 	error_code = i18n_ucalendar_clone(cal, &identical_to_cal);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_clone: %d\n", error_code);
 * 		i18n_ucalendar_destroy(cal);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_ucalendar_destroy(identical_to_cal);
 * 	i18n_ucalendar_destroy(cal);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucalendar_clone(const i18n_ucalendar_h cal, i18n_ucalendar_h *identical_to_cal);

/**
 * @brief Gets the display name for a calendar's TimeZone.
 * @details A display name is suitable for presentation to a user.
 * @since_tizen 2.3
 *
 * @param[in] calendar	   The #i18n_ucalendar_h to query
 * @param[in] type		   The desired display name format \n
 *						   One of #I18N_UCALENDAR_STANDARD, #I18N_UCALENDAR_SHORT_STANDARD, #I18N_UCALENDAR_DST, or #I18N_UCALENDAR_SHORT_DST
 * @param[in] locale	   The desired locale for the display name
 * @param[out] result	   A pointer to a buffer to receive the formatted number
 * @param[in] result_len The maximum size of the result
 * @param[out] buf_size_needed The total buffer size needed \n
 *				If greater than @a result_len, the output is truncated
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_get_timezone_displayname(const i18n_ucalendar_h calendar, i18n_ucalendar_displayname_type_e type, const char *locale, i18n_uchar *result, int32_t result_len, int32_t *buf_size_needed);

/**
 * @brief Determines if an #i18n_ucalendar_h is currently in daylight savings time.
 * @details Daylight savings time is not used in all parts of the world.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to query
 * @param[out] is_in   If @c true @a calendar is currently in daylight savings time,
 *					   otherwise @c false
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_is_in_daylight_time(const i18n_ucalendar_h calendar, i18n_ubool *is_in);

/**
 * @brief Sets the value of a field in a #i18n_ucalendar_h.
 * @details All fields are represented as 32-bit integers.
 * @since_tizen 2.3
 *
 * @param[in] cal The #i18n_ucalendar_h to modify
 * @param[in] field The field to set \n
 *				One of #I18N_UCALENDAR_ERA, #I18N_UCALENDAR_YEAR,
 * #I18N_UCALENDAR_MONTH, #I18N_UCALENDAR_WEEK_OF_YEAR, #I18N_UCALENDAR_WEEK_OF_MONTH,
 * #I18N_UCALENDAR_DATE, #I18N_UCALENDAR_DAY_OF_YEAR, #I18N_UCALENDAR_DAY_OF_WEEK,
 * #I18N_UCALENDAR_DAY_OF_WEEK_IN_MONTH, #I18N_UCALENDAR_AM_PM, #I18N_UCALENDAR_HOUR,
 * #I18N_UCALENDAR_HOUR_OF_DAY, #I18N_UCALENDAR_MINUTE, #I18N_UCALENDAR_SECOND,
 * #I18N_UCALENDAR_MILLISECOND, #I18N_UCALENDAR_ZONE_OFFSET, #I18N_UCALENDAR_DST_OFFSET.
 * @param[in] val The desired value of @a field.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_set(i18n_ucalendar_h cal, i18n_ucalendar_date_fields_e field, int32_t val);

/**
 * @brief Sets a numeric attribute associated with an #i18n_ucalendar_h.
 * @details Numeric attributes include the first day of the week, or the minimal number
 * of days in the first week of the month.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to modify
 * @param[in] attr The desired attribute \n
 *				One of #I18N_UCALENDAR_LENIENT, #I18N_UCALENDAR_FIRST_DAY_OF_WEEK,
 * or #I18N_UCALENDAR_MINIMAL_DAYS_IN_FIRST_WEEK.
 *
 * @param[in] val The new value of @a attr
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_set_attribute(i18n_ucalendar_h calendar, i18n_ucalendar_attribute_e attr, int32_t val);

/**
 * @brief Gets a numeric attribute associated with an i18n_ucalendar.
 * @details Numeric attributes include the first day of the week, or the minimal numbers of days
 * in the first week of the month.
 *
 * @since_tizen 2.3
 *
 * @param[in] calendar The i18n_ucalendar to query
 * @param[in] attr The desired attribute \n
 *				One of #I18N_UCALENDAR_LENIENT, #I18N_UCALENDAR_FIRST_DAY_OF_WEEK,
 * or #I18N_UCALENDAR_MINIMAL_DAYS_IN_FIRST_WEEK.
 *
 * @param[out] val The value of @a attr
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_get_attribute(i18n_ucalendar_h calendar, i18n_ucalendar_attribute_e attr, int32_t *val);

/**
 * @brief Gets a calendar's current time in milliseconds.
 * @details The time is represented as milliseconds from the epoch.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to query
 * @param[out] date The calendar's current time in milliseconds
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucalendar_set_milliseconds()
 * @see i18n_ucalendar_set_date_time()
 */
int i18n_ucalendar_get_milliseconds(const i18n_ucalendar_h calendar, i18n_udate *date);

/**
 * @brief Sets a calendar's current time in milliseconds.
 * @details The time is represented as milliseconds from the epoch.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to set
 * @param[in] milliseconds The desired date and time
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucalendar_get_milliseconds()
 * @see i18n_ucalendar_set_date_time()
 */
int i18n_ucalendar_set_milliseconds(i18n_ucalendar_h calendar, i18n_udate milliseconds);

/**
 * @brief Sets a calendar's current date.
 * @details The date is represented as a series of 32-bit integers.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to set
 * @param[in] year The desired year
 * @param[in] month The desired month\n
 *			One of #I18N_UCALENDAR_JANUARY, #I18N_UCALENDAR_FEBRUARY, #I18N_UCALENDAR_MARCH, #I18N_UCALENDAR_APRIL, #I18N_UCALENDAR_MAY,
 *			#I18N_UCALENDAR_JUNE, #I18N_UCALENDAR_JULY, #I18N_UCALENDAR_AUGUST, #I18N_UCALENDAR_SEPTEMBER, #I18N_UCALENDAR_OCTOBER, #I18N_UCALENDAR_NOVEMBER, or #I18N_UCALENDAR_DECEMBER
 * @param[in] date The desired day of the month
 * @param[in] hour The desired hour of the day
 * @param[in] min The desired minute
 * @param[in] sec The desired second
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucalendar_get_milliseconds()
 * @see i18n_ucalendar_set_milliseconds()
 */
int i18n_ucalendar_set_date_time(i18n_ucalendar_h calendar, int32_t year, int32_t month, int32_t date, int32_t hour, int32_t min, int32_t sec);

/**
 * @brief Returns @c true if two #i18n_ucalendar_h calendars are equivalent.
 * @details Equivalent #i18n_ucalendar_h calendars will behave identically, but they may be set to
 * different times.
 * @since_tizen 2.3
 *
 * @param[in] calendar1 The first of the calendars to compare
 * @param[in] calendar2 The second of the calendars to compare
 * @param[out] equiv If @c true @a cal1 and @a cal2 are equivalent, otherwise @c false
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_is_equivalent_to(const i18n_ucalendar_h calendar1, const i18n_ucalendar_h calendar2, i18n_ubool *equiv);

/**
 * @brief Adds a specified signed amount to a particular field in a #i18n_ucalendar_h.
 * @details This can modify more significant fields in the calendar.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to which to add
 * @param[in] field The field to which to add the signed value\n One of #I18N_UCALENDAR_ERA, #I18N_UCALENDAR_YEAR, #I18N_UCALENDAR_MONTH,
 * #I18N_UCALENDAR_WEEK_OF_YEAR, #I18N_UCALENDAR_WEEK_OF_MONTH, #I18N_UCALENDAR_DATE, #I18N_UCALENDAR_DAY_OF_YEAR, #I18N_UCALENDAR_DAY_OF_WEEK,
 * #I18N_UCALENDAR_DAY_OF_WEEK_IN_MONTH, #I18N_UCALENDAR_AM_PM, #I18N_UCALENDAR_HOUR, #I18N_UCALENDAR_HOUR_OF_DAY, #I18N_UCALENDAR_MINUTE, #I18N_UCALENDAR_SECOND,
 * #I18N_UCALENDAR_MILLISECOND.
 * @param[in] amount The signed amount to add to the field \n
 *				If the amount causes the value to exceed to maximum or minimum values for that field,
 *				other fields are modified to preserve the magnitude of the change.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter

 */
int i18n_ucalendar_add(i18n_ucalendar_h calendar, i18n_ucalendar_date_fields_e field, int32_t amount);

/**
 * @brief Gets the current value of a field from an #i18n_ucalendar_h.
 * @details All fields are represented as 32-bit integers.
 * @since_tizen 2.3
 *
 * @param[in] calendar The #i18n_ucalendar_h to query
 * @param[in] field The desired field\n One of I18N_UCALENDAR_ERA, I18N_UCALENDAR_YEAR, I18N_UCALENDAR_MONTH,
 * #I18N_UCALENDAR_WEEK_OF_YEAR, #I18N_UCALENDAR_WEEK_OF_MONTH, #I18N_UCALENDAR_DATE, #I18N_UCALENDAR_DAY_OF_YEAR, #I18N_UCALENDAR_DAY_OF_WEEK,
 * #I18N_UCALENDAR_DAY_OF_WEEK_IN_MONTH, #I18N_UCALENDAR_AM_PM, #I18N_UCALENDAR_HOUR, #I18N_UCALENDAR_HOUR_OF_DAY, #I18N_UCALENDAR_MINUTE, #I18N_UCALENDAR_SECOND,
 * #I18N_UCALENDAR_MILLISECOND, #I18N_UCALENDAR_ZONE_OFFSET, or #I18N_UCALENDAR_DST_OFFSET.
 * @param[out] val The value of the desired field.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_get(const i18n_ucalendar_h calendar, i18n_ucalendar_date_fields_e field, int32_t *val);


/* Newly Added APIs */



/**
 * @brief Returns the difference between the target time and the time this calendar object is currently set to.
 * @details If the target time is after the current calendar setting, the the returned value will be positive.
 *			The field parameter specifies the units of the return value.
 *			For example, if field is I18N_UCALENDAR_MONTH and i18n_ucalendar_get_field_difference returns 3,
 *			then the target time is 3 to less than 4 months after the current calendar setting. <br>
 *			As a side effect of this call, this calendar is advanced toward target by the given amount.
 *			That is, calling this function has the side effect of calling i18n_ucalendar_add on this calendar with the
 *			specified field and an amount equal to the return value from this function. <br>
 *			A typical way of using this function is to call it first with the largest field of interest, then with progressively smaller fields.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The i18n_ucalendar_h to compare and update.
 * @param[in] target   The target date to compare to the current calendar setting.
 * @param[in] field	   One of #I18N_UCALENDAR_ERA, #I18N_UCALENDAR_YEAR, #I18N_UCALENDAR_MONTH,
 *					   #I18N_UCALENDAR_WEEK_OF_YEAR, #I18N_UCALENDAR_WEEK_OF_MONTH, #I18N_UCALENDAR_DATE,
 *					   #I18N_UCALENDAR_DAY_OF_YEAR, #I18N_UCALENDAR_DAY_OF_WEEK, #I18N_UCALENDAR_DAY_OF_WEEK_IN_MONTH,
 *					   #I18N_UCALENDAR_AM_PM, #I18N_UCALENDAR_HOUR, #I18N_UCALENDAR_HOUR_OF_DAY, #I18N_UCALENDAR_MINUTE,
 *					   #I18N_UCALENDAR_SECOND, #I18N_UCALENDAR_MILLISECOND.
 *					   <strong>Please note</strong> that the returned value type is int32_t. In case of #I18N_UCALENDAR_MILLISECOND, maximal
 *					   difference between dates may be equal to the maximal value of the int32_t, which is 2147483647 (about one month difference).
 *					   If the difference is bigger, then the #I18N_ERROR_INVALID_PARAMETER error will be returned.
 * @param[out] status  A pointer to an i18n_error_code_e to receive any errors
 *
 * @return The date difference for the specified field.
 */
int32_t i18n_ucalendar_get_field_difference(i18n_ucalendar_h calendar, i18n_udate target, i18n_ucalendar_date_fields_e field, i18n_error_code_e *status);

/**
 * @brief Creates an enumeration over system time zone IDs with the given filter conditions.
 * @since_tizen 2.3.1
 * @remarks The @a enumeration should be released using i18n_uenumeration_destroy().
 *
 * @param[in] zone_type The system time zone type.
 * @param[in] region The ISO 3166 two-letter country code or UN M.49 three-digit
 *		area code. When @c NULL, no filtering done by region.
 * @param[in] raw_offset An offset from GMT in milliseconds, ignoring the effect
 *		of daylight savings time, if any. When @c NULL, no filtering done by zone offset.
 *
 * @param[out] enumeration A Pointer to the enumeration object that the caller must dispose of using
 *		i18n_uenumeration_destroy(), or@c  NULL upon failure.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_uenumeration_h enumeration = NULL;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_ucalendar_timezone_id_enumeration_create(I18N_UCALENDAR_ZONE_TYPE_ANY, "US", NULL, &enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_timezone_id_enumeration_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(enumeration);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucalendar_timezone_id_enumeration_create(i18n_system_timezone_type_e zone_type, const char *region, const int32_t *raw_offset, i18n_uenumeration_h *enumeration);

/**
 * @brief Creates an enumeration over all time zones.
 * @since_tizen 2.3.1
 * @remarks The @a enumeration should be released using i18n_uenumeration_destroy().
 *
 * @param[out] enumeration A pointer to the enumeration object that the caller must dispose of using
 *		i18n_uenumeration_destroy(), or @c NULL upon failure.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_uenumeration_h enumeration = NULL;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_ucalendar_timezones_create(&enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_timezones_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(enumeration);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucalendar_timezones_create(i18n_uenumeration_h *enumeration);

/**
 * @brief Creates an enumeration over all time zones associated with the given country.
 * @details Some zones are affiliated with no country (e.g., "UTC"); these may also be retrieved, as a group.
 * @since_tizen 2.3.1
 * @remarks The @a enumeration should be released using i18n_uenumeration_destroy().
 *
 * @param[in] country The ISO 3166 two-letter country code, or @c NULL to retrieve zones not affiliated with any country
 *
 * @param[out] enumeration A pointer to the enumeration object that the caller must dispose of using
 *		i18n_uenumeration_destroy(), or @c NULL upon failure.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_uenumeration_h enumeration = NULL;
 * 	i18n_error_code_e error_code;
 * 	error_code = i18n_ucalendar_country_timezones_create("US", &enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_country_timezones_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(enumeration);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_ucalendar_country_timezones_create(const char *country, i18n_uenumeration_h *enumeration);

/**
 * @brief Returns the default time zone (i.e., the one returned by the
 *		  i18n_timezone_create_default() function).
 * @details The default time zone is determined initially by querying the host operating
 *		system and storing the obtained time zone as default until the application terminates.
 *		Therefore, if the time zone is changed after the first call, subsequent calls will not
 *		reflect the change, i.e. the returned time zone will be the same as for the first function
 *		call.
 *
 *		To query the system for the current timezone, use i18n_timezone_detect_host_timezone()
 *		from the Timezone module.
 *
 *		The default time zone may be changed with i18n_ucalendar_set_default_timezone()
 *		or with the i18n Timezone API.
 *
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[out] result A buffer to receive the result, or @c NULL
 * @param[in]  result_capacity The capacity of the @a result buffer
 *
 * @return The @a result string length, not including the terminating @c NULL.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid parameter
 */
int32_t i18n_ucalendar_get_default_timezone(i18n_uchar *result, int32_t result_capacity);

/**
 * @brief Sets the TimeZone used by a #i18n_ucalendar_h.
 * @details A #i18n_ucalendar_h uses a timezone for converting from Greenwich time to local time.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to set.
 * @param[in] zone_id The desired TimeZone ID. If NULL, use the default time zone.
 * @param[in] length The length of @c zone_id, or -1 if NULL-terminated.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_set_timezone(i18n_ucalendar_h calendar, const i18n_uchar *zone_id, int32_t length);

/**
 * @brief Gets the ID of the calendar's time zone.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section and
 *		#i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 * @param[out] result Receives the calendar's time zone ID.
 * @param[in] result_length The maximum size of the @a result.
 *
 * @return The total buffer size needed; if greater than @a result_length, the output was truncated.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ucalendar_get_timezone_id(const i18n_ucalendar_h calendar, i18n_uchar *result, int32_t result_length);

/**
 * @brief Sets the Gregorian Calendar change date.
 * @details This is the point when the switch from Julian dates to Gregorian dates
 *		occurred. Default is 00:00:00 local time, October 15, 1582.
 *		Previous to this time and date will be Julian dates.
 *		This function works only for Gregorian calendars. If the #i18n_ucalendar_h
 *		is not an instance of a Gregorian calendar, then a
 *		#I18N_ERROR_NOT_SUPPORTED error code is set.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The calendar object.
 * @param[in] date The given Gregorian cutover date.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ucalendar_get_gregorian_change()
 */
int i18n_ucalendar_set_gregorian_change(i18n_ucalendar_h calendar, i18n_udate date);

/**
 * @brief Gets the Gregorian Calendar change date.
 * @details This is the point when the switch from Julian dates to Gregorian dates
 *		occurred. Default is 00:00:00 local time, October 15, 1582.
 *		Previous to this time and date will be Julian dates.
 *		This function works only for Gregorian calendars. If the #i18n_ucalendar_h
 *		is not an instance of a Gregorian calendar, then a
 *		#I18N_ERROR_NOT_SUPPORTED error code is set.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The calendar object.
 * @param[out] date A pointer to the Gregorian cutover time for this calendar.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ucalendar_set_gregorian_change()
 */
int i18n_ucalendar_get_gregorian_change(const i18n_ucalendar_h calendar, i18n_udate *date);

/**
 * @brief Gets a locale for which calendars are available.
 * @details A #i18n_ucalendar_h in a locale returned by this function will contain
 *		the correct day and month names for the locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_index The index of the desired locale.
 *
 * @return A locale for which calendars are available, or 0 if none.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid parameter
 * @see i18n_ucalendar_count_available()
 */
const char *i18n_ucalendar_get_available(int32_t locale_index);

/**
 * @brief Determines how many locales have calendars available.
 * @details This function is most useful as determining the loop ending condition for calls to i18n_ucalendar_get_available().
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @return The number of locales for which calendars are available.
 * @exception #I18N_ERROR_NONE Successful
 * @see i18n_ucalendar_get_available()
 */
int32_t i18n_ucalendar_count_available(void);

/**
 * @brief Sets a calendar's current date.
 * @details The date is represented as a series of 32-bit integers.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to set.
 * @param[in] year The desired year.
 * @param[in] month The desired month; one of #I18N_UCALENDAR_JANUARY, #I18N_UCALENDAR_FEBRUARY, #I18N_UCALENDAR_MARCH,
 *		#I18N_UCALENDAR_APRIL, #I18N_UCALENDAR_MAY, #I18N_UCALENDAR_JUNE, #I18N_UCALENDAR_JULY, #I18N_UCALENDAR_AUGUST,
 *		#I18N_UCALENDAR_SEPTEMBER, #I18N_UCALENDAR_OCTOBER, #I18N_UCALENDAR_NOVEMBER, #I18N_UCALENDAR_DECEMBER
 * @param[in] date The desired day of the month.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ucalendar_add()
 * @see i18n_ucalendar_set_milliseconds()
 * @see i18n_ucalendar_set_date_time()
 */
int i18n_ucalendar_set_date(i18n_ucalendar_h calendar, int32_t year, int32_t month, int32_t date);

/**
 * @brief Adds a specified signed amount to a particular field in a #i18n_ucalendar_h.
 * @details This will not modify more significant fields in the calendar.
 *		Rolling by a positive value always means moving forward in time
 *		(unless the limit of the field is reached, in which case it may pin or wrap),
 *		so for Gregorian calendar, starting with 100 BC and
 *		rolling the year by +1 results in 99 BC.
 *		When eras have a definite beginning and end (as in the Chinese calendar,
 *		or as in most eras in the Japanese calendar) then rolling the year past either
 *		limit of the era will cause the year to wrap around. When eras only have a limit at one end,
 *		then attempting to roll the year past that limit will result in pinning the year at that limit.
 *		Note that for most calendars in which era 0 years move forward in time (such as Buddhist, Hebrew, or Islamic),
 *		it is possible for add or roll to result in negative years for era 0 (that is the
 *		only way to represent years before the calendar epoch).
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to which to add.
 * @param[in] field The field to which to add the signed value; one of #I18N_UCALENDAR_ERA,
 *		#I18N_UCALENDAR_YEAR, #I18N_UCALENDAR_MONTH, #I18N_UCALENDAR_WEEK_OF_YEAR,
 *		#I18N_UCALENDAR_WEEK_OF_MONTH, #I18N_UCALENDAR_DATE, #I18N_UCALENDAR_DAY_OF_YEAR,
 *		#I18N_UCALENDAR_DAY_OF_WEEK, #I18N_UCALENDAR_DAY_OF_WEEK_IN_MONTH, #I18N_UCALENDAR_AM_PM,
 *		#I18N_UCALENDAR_HOUR, #I18N_UCALENDAR_HOUR_OF_DAY, #I18N_UCALENDAR_MINUTE,
 *		#I18N_UCALENDAR_SECOND, #I18N_UCALENDAR_MILLISECOND.
 * @param[in] amount The signed amount to add to the @a field. If the amount causes the
 *		value to exceed to maximum or minimum values for that field, the field is pinned
 *		to a permissible value.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @remarks #I18N_UCALENDAR_ZONE_OFFSET and #I18N_UCALENDAR_DST_OFFSET are not supported by this function.
 * @see i18n_ucalendar_add()
 */
int i18n_ucalendar_roll(i18n_ucalendar_h calendar, i18n_ucalendar_date_fields_e field, int32_t amount);

/**
 * @brief Determines if a field in a #i18n_ucalendar_h is set.
 * @details All fields are represented as 32-bit integers.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 * @param[in] field	   The desired field.
 *
 * @return @c true if field is set, @c false otherwise.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ucalendar_get()
 * @see i18n_ucalendar_set()
 * @see i18n_ucalendar_clear_field()
 * @see i18n_ucalendar_clear()
 *
 */
i18n_ubool i18n_ucalendar_is_set(const i18n_ucalendar_h calendar, i18n_ucalendar_date_fields_e field);

/**
 * @brief Clears a field in a #i18n_ucalendar_h.
 * @details All fields are represented as 32-bit integers.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h containing the field to clear.
 * @param[in] field	   The field to clear.
 *
 * @return Error code
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ucalendar_get()
 * @see i18n_ucalendar_set()
 * @see i18n_ucalendar_is_set()
 * @see i18n_ucalendar_clear()
 *
 */
int i18n_ucalendar_clear_field(i18n_ucalendar_h calendar, i18n_ucalendar_date_fields_e field);

/**
 * @brief Clears all fields in a #i18n_ucalendar_h.
 * @details All fields are represented as 32-bit integers.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar #i18n_ucalendar_h to clear.
 *
 * @return Error code
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ucalendar_is_set()
 * @see i18n_ucalendar_clear_field()
 *
 */
int i18n_ucalendar_clear(i18n_ucalendar_h calendar);

/**
 * @brief Determines a limit for a field in an #i18n_ucalendar_h.
 * @details A limit is a maximum or minimum value for a field.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 * @param[in] field	   The desired field; one of #I18N_UCALENDAR_ERA, #I18N_UCALENDAR_YEAR, #I18N_UCALENDAR_MONTH,
 *					   #I18N_UCALENDAR_WEEK_OF_YEAR, #I18N_UCALENDAR_WEEK_OF_MONTH, #I18N_UCALENDAR_DATE,
 *					   #I18N_UCALENDAR_DAY_OF_YEAR, #I18N_UCALENDAR_DAY_OF_WEEK, #I18N_UCALENDAR_DAY_OF_WEEK_IN_MONTH,
 *					   #I18N_UCALENDAR_AM_PM, #I18N_UCALENDAR_HOUR, #I18N_UCALENDAR_HOUR_OF_DAY,
 *					   #I18N_UCALENDAR_MINUTE, #I18N_UCALENDAR_SECOND, #I18N_UCALENDAR_MILLISECOND,
 *					   #I18N_UCALENDAR_ZONE_OFFSET, #I18N_UCALENDAR_DST_OFFSET, #I18N_UCALENDAR_YEAR_WOY,
 *					   #I18N_UCALENDAR_DOW_LOCAL, #I18N_UCALENDAR_EXTENDED_YEAR, #I18N_UCALENDAR_JULIAN_DAY,
 *					   #I18N_UCALENDAR_MILLISECONDS_IN_DAY, #I18N_UCALENDAR_IS_LEAP_MONTH.
 * @param[in] type	   The desired critical point; one of #I18N_UCALENDAR_MINIMUM, #I18N_UCALENDAR_MAXIMUM,
 *					   #I18N_UCALENDAR_GREATEST_MINIMUM, #I18N_UCALENDAR_LEAST_MAXIMUM,
 *					   #I18N_UCALENDAR_ACTUAL_MINIMUM, #I18N_UCALENDAR_ACTUAL_MAXIMUM.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @return The requested value.
 */
int32_t i18n_ucalendar_get_limit(const i18n_ucalendar_h calendar, i18n_ucalendar_date_fields_e field, i18n_ucalendar_limit_type_e type);

/**
 * @brief Gets the locale for this @a calendar object.
 * @details You can choose between valid and actual locale.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section and
 *		#i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The calendar object
 * @param[in] type Type of the locale we're looking for (valid or actual)
 *
 * @return The requested value.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
const char *i18n_ucalendar_get_locale_by_type(const i18n_ucalendar_h calendar, i18n_ulocale_data_locale_type_e type);

/**
 * @brief Returns the timezone data version currently used by ICU.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @return The version string, such as "2007f".
 * @exception #I18N_ERROR_NONE Successful
 */
const char *i18n_ucalendar_get_tz_data_version(void);

/**
 * @brief Returns the canonical system timezone ID or the normalized custom time zone ID for the given time zone ID.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section and
 *		#i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] id The input timezone ID to be canonicalized.
 * @param[in] length The length of the @a id, or @c -1 if NULL-terminated.
 * @param[out] result The buffer receives the canonical system timezone ID or the custom timezone ID in normalized format.
 * @param[in] result_capacity The capacity of the @a result buffer.
 * @param[out] is_system_id Receives if the given @a id is a known system timezone ID.
 *
 * @return The result string length, not including the terminating NULL.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ucalendar_get_canonical_timezone_id(const i18n_uchar *id, int32_t length, i18n_uchar *result, int32_t result_capacity,	 i18n_ubool *is_system_id);

/**
 * @brief Gets the resource keyword value string designating the calendar type for the #i18n_ucalendar_h.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section and
 *		#i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 *
 * @return The resource keyword value string.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
const char *i18n_ucalendar_get_type(const i18n_ucalendar_h calendar);

/**
 * @brief Given a key and a locale, returns an array of string values in a preferred order that would make a difference.
 * @details These are all and only those values where the open (creation) of the service with the
 *		locale formed from the input locale plus input keyword and that value
 *		has different behavior than creation with the input locale alone.
 * @since_tizen 2.3.1
 * @remarks The @a enumeration should be released using i18n_uenumeration_destroy().
 *
 * @param[in] key One of the keys supported by this service. For now, only "calendar" is supported.
 * @param[in] locale The locale
 * @param[in] commonly_used If set to @c true it will return only commonly used values with
 *		the given locale in preferred order. Otherwise, it will return all the available
 *		values for the locale.
 *
 * @param[out] enumeration A pointer to the string enumeration over keyword values for the given key and the locale.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_ucalendar_h cal = NULL;
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h enumeration = NULL;
 *
 * 	error_code = i18n_ucalendar_create(NULL, -1, I18N_ULOCALE_US, I18N_UCALENDAR_DEFAULT, &cal);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	const char *key = i18n_ucalendar_get_type(cal);
 * 	error_code = get_last_result();
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_create: %d\n", error_code);
 * 		i18n_ucalendar_destroy(cal);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	const char *locale = i18n_ucalendar_get_available(1);
 * 	error_code = i18n_ucalendar_get_keyword_values_for_locale(key, locale, 0, &enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_ucalendar_get_keyword_values_for_localee: %d\n", error_code);
 * 		i18n_ucalendar_destroy(cal);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_uenumeration_destroy(enumeration);
 * 	i18n_ucalendar_destroy(cal);
 * 	return EXIT_SUCCESS;
 * }
 *
 * @endcode
 */
int i18n_ucalendar_get_keyword_values_for_locale(const char *key, const char *locale, i18n_ubool commonly_used, i18n_uenumeration_h *enumeration);

/**
 * @brief Returns whether the given day of the week is a weekday, a weekend day,
 *		or a day that transitions from one to the other, for the locale and calendar system
 *		associated with this #i18n_ucalendar_h (the locale's region is often the most determinant factor).
 * @details If a transition occurs at midnight, then the days before and after the
 *		transition will have the type #I18N_UCALENDAR_WEEKDAY or #I18N_UCALENDAR_WEEKEND.
 *		If a transition occurs at a time other than midnight, then the day of the
 *		transition will have the type #I18N_UCALENDAR_WEEKEND_ONSET or #I18N_UCALENDAR_WEEKEND_CEASE.
 *		In this case, the function i18n_ucalendar_get_weekend_transition() will
 *		return the point of transition.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 * @param[in] day_of_week The day of the week whose type is desired (#I18N_UCALENDAR_SUNDAY..#I18N_UCALENDAR_SATURDAY).
 *
 * @param [out] weekday A pointer to the #i18n_ucalendar_weekday_type_e for the day of the week.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ucalendar_get_day_of_week_type(const i18n_ucalendar_h calendar, i18n_ucalendar_days_of_week_e day_of_week, i18n_ucalendar_weekday_type_e *weekday);

/**
 * @brief Returns the time during the day at which the weekend begins or ends in this calendar system.
 * @details If i18n_ucalendar_get_day_of_week_type() returns #I18N_UCALENDAR_WEEKEND_ONSET
 *		for the specified @a day_of_week, return the time at which the weekend begins. If
 *		i18n_ucalendar_get_day_of_week_type() returns #I18N_UCALENDAR_WEEKEND_CEASE for
 *		the specified @a day_of_week, return the time at which the weekend ends. If
 *		i18n_ucalendar_get_day_of_week_type() returns some other #i18n_ucalendar_weekday_type_e
 *		for the specified @a day_of_week, it is an error condition (#I18N_ERROR_INVALID_PARAMETER).
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section and
 *		#i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 * @param[in] day_of_week The day of the week whose type is desired (#I18N_UCALENDAR_SUNDAY..#I18N_UCALENDAR_SATURDAY).
 *
 * @return The milliseconds after midnight at which the weekend begins or ends.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t	 i18n_ucalendar_get_weekend_transition(const i18n_ucalendar_h calendar, i18n_ucalendar_days_of_week_e day_of_week);

/**
 * @brief Returns @c true if the given #i18n_udate is in the weekend in this calendar system.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section and
 *		#i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 * @param[in] date The #i18n_udate in question.
  *
 * @return @c true if the given #i18n_udate is in the weekend in this calendar system, @c false otherwise.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_ucalendar_is_weekend(i18n_ucalendar_h calendar, i18n_udate date);

/**
 * @brief Get the #i18n_udate for the next/previous time zone transition relative
 *		to the calendar's current date, in the time zone to which the calendar is currently set.
 * @details If there is no known time zone transition of the requested type relative
 *		to the calendar's date, the function returns @c false.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in Exceptions section and
 *		#i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] calendar The #i18n_ucalendar_h to query.
 * @param[in] type The type of transition desired.
 * @param[out] transition A pointer to a #i18n_udate to be set to the transition time.
 *		If the function returns @c false, the value set is unspecified.
 *
 * @return @c true if the given #i18n_udate is in the weekend in this calendar system, @c false otherwise.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
i18n_ubool i18n_ucalendar_get_timezone_transition_date(const i18n_ucalendar_h calendar, i18n_utimezone_transition_type_e type, i18n_udate *transition);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
