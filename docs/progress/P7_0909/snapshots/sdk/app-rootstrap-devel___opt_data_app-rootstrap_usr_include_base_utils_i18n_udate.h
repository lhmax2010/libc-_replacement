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
 * @file utils_i18n_udate.h
 * @version 0.1
 * @brief utils_i18n_udate
 */

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UDATE_MODULE Udate
 * @brief The Udate module consists of functions that convert dates and time from their
 * internal representations to textual form and back again in a language-independent
 * manner.
 *
 * @section CAPI_BASE_UTILS_I18N_UDATE_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UDATE_MODULE_OVERVIEW Overview
 * @details The Udate module consists of functions that convert dates and time from their
 * internal representations to textual form and back again in a language-independent
 * manner. Converting from the internal representation (milliseconds since midnight,
 * January 1, 1970) to text is known as "formatting," and converting from text to
 * milliseconds is known as "parsing". We currently define only one concrete handle
 * #i18n_udate_format_h, which can handle pretty much all normal date formatting and parsing
 * actions.\n
 * The Udate module helps you format and parse dates for any locale. Your code can be
 * completely independent of the locale conventions for months, days of the week,
 * or even the calendar format: lunar vs. solar.
 *
 * @section CAPI_BASE_UTILS_I18N_UDATE_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Gets the best pattern according to a given locale and formats a current date and time using a locale_udate_format_h
 * @code
	i18n_udatepg_h pattern_generator = NULL;
	char *locale = I18N_ULOCALE_US;

	dlog_print(DLOG_INFO, LOG_TAG, "pattern_generator\n");

	if(!pattern_generator) {
		// create a pattern generator according to a given locale
		i18n_udatepg_create(locale, &pattern_generator);
	}

	if(!pattern_generator) {
		dlog_print(DLOG_INFO, LOG_TAG, "i18n_udatepg_create fail");
		return ;
	}

	i18n_uchar bestPattern[64] = {0,};
	char bestPatternString[64] = {0,};
	int bestPatternLength, len;
	const char *custom_format = "yyyy.MM.dd G 'at' HH:mm:ss zzz";
	i18n_uchar uch_custom_format[64];
	int ret = I18N_ERROR_NONE;

	dlog_print(DLOG_INFO, LOG_TAG, "getBestPattern\n");

	i18n_ustring_copy_ua(uch_custom_format, custom_format);
	len = i18n_ustring_get_length(uch_custom_format);

	// gets the best pattern that matches the given custom_format
	i18n_udatepg_get_best_pattern(pattern_generator, uch_custom_format, len, bestPattern, 64, &bestPatternLength);

	i18n_ustring_copy_au_n(bestPatternString, bestPattern, 64);
	// gets "MM/dd/yyyy G h:mm:ss a zzz" as the best pattern
	dlog_print(DLOG_INFO, LOG_TAG, "getBestPattern(char[]) : %s \n", bestPatternString);

	// closes a generator
	i18n_udatepg_destroy(pattern_generator);

	i18n_udate_format_h formatter_KR = NULL;
	i18n_udate_format_h formatter_LA = NULL;
	i18n_udate_format_h formatter_SaoPaulo = NULL;
	i18n_uchar formatted[64] = {0,};
	char result[64] = {0,};
	int formattedLength;
	i18n_udate date;
	const char *timezone_KR = "GMT+9:00";	// TimeZone for Korea/Seoul
	const char *timezone_LA = "America/Los_Angeles";
	const char *timezone_SaoPaulo = "America/Sao_Paulo";	// Brazil/East
	i18n_uchar utf16_timezone_KR[64] = {0,};
	i18n_uchar utf16_timezone_LA[64] = {0,};
	i18n_uchar utf16_timezone_SaoPaulo[64] = {0,};

	i18n_ustring_copy_ua_n(utf16_timezone_KR, timezone_KR, strlen(timezone_KR));
	i18n_ustring_copy_ua_n(utf16_timezone_LA, timezone_LA, strlen(timezone_LA));
	i18n_ustring_copy_ua_n(utf16_timezone_SaoPaulo, timezone_SaoPaulo, strlen(timezone_SaoPaulo));

	// creates new i18n_udate_format_h to format dates and times
	ret = i18n_udate_create(I18N_UDATE_FULL , I18N_UDATE_FULL , locale, utf16_timezone_KR, -1, bestPattern, -1, &formatter_KR);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "i18n_udate_create failed !!! \n");
	}
	if (!formatter_KR) {
		dlog_print(DLOG_INFO, LOG_TAG, "formatter is NULL\n");
	}
	ret = i18n_udate_create(I18N_UDATE_FULL , I18N_UDATE_FULL , locale, utf16_timezone_LA, -1, bestPattern, -1, &formatter_LA);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "i18n_udate_create failed !!! \n");
	}
	if (!formatter_LA) {
		dlog_print(DLOG_INFO, LOG_TAG, "formatter is NULL\n");
	}
	ret = i18n_udate_create(I18N_UDATE_PATTERN , I18N_UDATE_PATTERN , locale, utf16_timezone_SaoPaulo, -1, bestPattern, -1, &formatter_SaoPaulo);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "i18n_udate_create failed !!! \n");
	}
	if (!formatter_SaoPaulo) {
		dlog_print(DLOG_INFO, LOG_TAG, "formatter is NULL\n");
	}

	dlog_print(DLOG_INFO, LOG_TAG, "i18n_udate_format_date\n");

	// gets the current date and time
	i18n_ucalendar_get_now(&date);

	// formats a date using i18n_udate_format_h
	i18n_udate_format_date(formatter_KR, date, formatted, 64, NULL, &formattedLength);
	i18n_ustring_copy_au_n(result, formatted, 64);
	//ex) KOREA/Seoul - Current date : Wednesday, June 18, 2014 1:34:54 PM GMT+09:00
	dlog_print(DLOG_INFO, LOG_TAG, "KOREA/Seoul - Current date : %s\n",result);

	// formats a date using i18n_udate_format
	i18n_udate_format_date(formatter_LA, date, formatted, 64, NULL, &formattedLength);
	i18n_ustring_copy_au_n(result, formatted, 64);
	//ex) America/LOS Angeles - Current date : Tuesday, June 17, 2014 9:34:54 PM Pacific Daylight Time
	dlog_print(DLOG_INFO, LOG_TAG, "America/LOS Angeles - Current date : %s\n",result);

	// formats a date using i18n_udate_format_h
	i18n_udate_format_date(formatter_SaoPaulo, date, formatted, 64, NULL, &formattedLength);
	i18n_ustring_copy_au_n(result, formatted, 64);
	//ex) Brazil/Sao Paulo - Current date : 6 18, 2014 AD, 1:34:54 PM GMT-2
	dlog_print(DLOG_INFO, LOG_TAG, "Brazil/Sao Paulo - Current date : %s\n",result);

	dlog_print(DLOG_INFO, LOG_TAG, "i18n_udate_destroy\n");
	// destroy an i18n_udate_format_h
	i18n_udate_destroy(formatter_SaoPaulo);
	i18n_udate_destroy(formatter_LA);
	i18n_udate_destroy(formatter_KR);
 * @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UDATE_MODULE
 * @{
 */

/**
 * @brief Creates a new #i18n_udate_format_h for formatting and parsing dates and times.
 * @details A #i18n_udate_format_h may be used to format dates in calls to {@link i18n_udate_create()}.
 * @since_tizen 2.3
 * @remarks Must release @a format using i18n_udate_destroy().\n
 *			If @a pattern parameter is used, pass in #I18N_UDATE_PATTERN for @a time_style and @a date_style. \n
 *			#I18N_UDATE_PATTERN can only be used in this case, and has to be set for both @a time_style and @a date_style.
 *
 * @param[in] time_style The style used to format times\n One of #I18N_UDATE_FULL, #I18N_UDATE_LONG,
 * #I18N_UDATE_MEDIUM, #I18N_UDATE_SHORT, #I18N_UDATE_DEFAULT, #I18N_UDATE_PATTERN (see remarks) or #I18N_UDATE_NONE (relative time styles
 * are not currently supported).
 * @param[in] date_style The style used to format dates\n One of #I18N_UDATE_FULL, #I18N_UDATE_LONG,
 * #I18N_UDATE_MEDIUM, #I18N_UDATE_SHORT, #I18N_UDATE_DEFAULT, #I18N_UDATE_RELATIVE, #I18N_UDATE_LONG_RELATIVE,
 * #I18N_UDATE_MEDIUM_RELATIVE, #I18N_UDATE_SHORT_RELATIVE, #I18N_UDATE_PATTERN (see remarks) or #I18N_UDATE_NONE
 * @param[in] locale The locale specifying the formatting conventions.
 * @param[in] tz_id A timezone ID specifying the timezone to use\n	If @c 0, use the default timezone.
 * @param[in] tz_id_len The length of @a tz_id, otherwise @c -1 if NULL-terminated.
 * @param[in] pattern A pattern specifying the format to use. The pattern is generated by Udatepg module.\n
 *					  When the pattern parameter is used, pass in #I18N_UDATE_PATTERN for both time_style and date_style.\n
 *					  See remarks for additional conditions.
 * @param[in] pattern_len The number of characters in the pattern, or otherwise @c -1 if NULL-terminated.
 * @param[out] format A pointer to an #i18n_udate_format_h to use for formatting dates and times, otherwise @c 0 if an error occurs.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_udate_create(i18n_udate_format_style_e time_style, i18n_udate_format_style_e date_style, const char *locale, const i18n_uchar *tz_id, int32_t tz_id_len, const i18n_uchar *pattern, int pattern_len, i18n_udate_format_h *format);

/**
 * @brief Destroys an #i18n_udate_format_h.
 * @details Once destroyed, an #i18n_udate_format_h may no longer be used.
 * @since_tizen 2.3
 *
 * @param[in] format The formatter to destroy.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_udate_destroy(i18n_udate_format_h format);

/**
 * @brief Formats a date using an #i18n_udate_format_h.
 * @details The date will be formatted using the conventions specified in {@link i18n_udate_create()}
 * @since_tizen 2.3
 *
 * @param[in] format The formatter to use.
 * @param[in] date_to_format The date to format.
 * @param[out] result A pointer to a buffer to receive the formatted number.
 * @param[in] result_len The maximum size of the result.
 * @param[in] pos A pointer to an i18n_ufield_position\n
 * On input, position->field is read\n
 * On output, position->beginIndex and position->endIndex indicate
 * the beginning and ending indices of field number position->field, if such a field exists\n
 * This parameter may be @c NULL, in which case no field
 * position data is returned.
 * @param[out] buf_size_needed The total buffer size needed\n
 * If greater than @a result_len, the output was truncated.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_udate_format_date(const i18n_udate_format_h format, i18n_udate date_to_format, i18n_uchar *result, int32_t result_len, i18n_ufield_position_h pos, int32_t *buf_size_needed);


/* Newly Added APIs */

/**
 * @brief Maps from an #i18n_udate_format_h to the corresponding #i18n_ucalendar_date_fields_e.
 * @details Note: since the mapping is many-to-one, there is no inverse mapping.
 * @since_tizen 2.3.1
 *
 * @param[in] field The #i18n_udate_format_h to map.
 *	   #I18N_UDATE_FORMAT_TIMEZONE_LOCALIZED_GMT_OFFSET_FIELD, #I18N_UDATE_FORMAT_TIMEZONE_ISO_FIELD,
 *	   #I18N_UDATE_FORMAT_TIMEZONE_ISO_LOCAL_FIELD and #I18N_UDATE_FORMAT_FIELD_COUNT are not supported.
 * @param[out] date_field_type A pointer to the #i18n_ucalendar_date_fields_e.
 *
 * @return Error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_udate_to_calendar_date_field(i18n_udate_format_field_e field, i18n_ucalendar_date_fields_e *date_field_type);

/**
 * @brief Creates a copy of an #i18n_udate_format_h.
 * @details This function performs a deep copy.
 * @since_tizen 2.3.1
 * @remarks The @a format_clone should be released using i18n_udate_destroy().
 *
 * @param[in] format	 The format to copy.
 * @param[out] format_clone A pointer to clone of @a format.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_udate_format_h format = NULL;
 * 	i18n_udate_format_style_e time_style = I18N_UDATE_DEFAULT, date_style = I18N_UDATE_DEFAULT;
 * 	const char *locale = I18N_ULOCALE_US;
 * 	const i18n_uchar *tz_id = 0; // use the default timezone
 * 	int32_t tz_id_length = -1;
 * 	const i18n_uchar *pattern = NULL;
 * 	int32_t pattern_length = 0;
 * 	i18n_udate_format_h format_clone = NULL;
 *
 * 	error_code = i18n_udate_create(time_style, date_style, locale, tz_id, tz_id_length, pattern, pattern_length, &format);
 * 	if(error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udate_create: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_udate_clone(format, &format_clone);
 * 	if(error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udate_clone : %d\n", error_code);
 * 		i18n_udate_destroy(format);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_udate_destroy(format_clone);
 * 	i18n_udate_destroy(format);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_udate_clone(const i18n_udate_format_h format, i18n_udate_format_h *format_clone);

/**
 * @brief Parses a string into an date/time using an #i18n_udate_format_h.
 * @details The date will be parsed using the conventions specified in {@link i18n_udate_create()}.<br>
 *			Note that the normal date formats associated with some calendars - such as the Chinese lunar calendar - do not specify enough fields to enable dates to be parsed unambiguously.
 *			In the case of the Chinese lunar calendar, while the year within the current 60-year cycle is specified,
 *			the number of such cycles since the start date of the calendar (in the #I18N_UCALENDAR_ERA field of the i18n_ucalendar_h) is not normally part of the format,
 *			and parsing may assume the wrong era.
 *			For such cases it is recommended that clients parse using i18n_udate_parse_calendar() with the calendar passed in set to the current date,
 *			or to a date within the era/cycle that should be assumed if absent in the format.
 * @since_tizen 2.3.1
 *
 * @param[in] format	  The formatter to use.
 * @param[in] text		  The text to parse.
 * @param[in] text_length The length of text, or -1 if NULL-terminated.
 * @param[in] parse_pos	  If not 0, on input a pointer to an integer specifying the offset at which to begin parsing.
 *						  If not 0, on output the offset at which parsing ended.
 * @param[out] parsed_date A pointer to the value of the parsed date/time.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_format_date()
 */
int i18n_udate_parse(const i18n_udate_format_h format, const i18n_uchar *text, int32_t text_length, int32_t *parse_pos, i18n_udate *parsed_date);

/**
 * @brief Parses a string into an date/time using an #i18n_udate_format_h.
 * @details The date will be parsed using the conventions specified in {@link i18n_udate_create()}.
 * @since_tizen 2.3.1
 *
 * @param[in] format		 The formatter to use.
 * @param[in,out] calendar	 A pointer to calendar set on input to the date and time to be used for missing values in the date/time string being parsed,
 *							 and set on output to the parsed date/time.
 *							 When the calendar type is different from the internal calendar held by the #i18n_udate_format_h instance,
 *							 the internal calendar will be cloned to a work calendar set to the same milliseconds and time zone as this calendar parameter,
 *							 field values will be parsed based on the work calendar, then the result (milliseconds and time zone) will be set in this calendar.
 * @param[in] text			 The text to parse.
 * @param[in] text_length	 The length of text, or -1 if NULL-terminated.
 * @param[in] parse_pos		 If not 0, on input a pointer to an integer specifying the offset at which to begin parsing.
 *							 If not 0, on output the offset at which parsing ended.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_format_date()
 */
int i18n_udate_parse_calendar(const i18n_udate_format_h format, i18n_ucalendar_h *calendar, const i18n_uchar *text,
							  int32_t text_length, int32_t *parse_pos);

/**
 * @brief Determines if an #i18n_udate_format_h will perform lenient parsing.
 * @details With lenient parsing, the parser may use heuristics to interpret inputs that do not precisely match the pattern.
 *			With strict parsing, inputs must match the pattern.
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] format	 The #i18n_udate_format_h to query.
 *
 * @return true if format is set to perform lenient parsing, false otherwise.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_set_lenient()
 */
i18n_ubool i18n_udate_is_lenient(const i18n_udate_format_h format);

/**
 * @brief Specifies whether an #i18n_udate_format_h will perform lenient parsing.
 * @details With lenient parsing, the parser may use heuristics to interpret inputs that do not precisely match the pattern.
 *			With strict parsing, inputs must match the pattern.
 * @since_tizen 2.3.1
 *
 * @param[in] format	  The #i18n_udate_format_h to set.
 * @param[in] is_lenient true if fmt should perform lenient parsing, false otherwise.
 *
 * @return Error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_is_lenient()
 */
int i18n_udate_set_lenient(i18n_udate_format_h format, i18n_ubool is_lenient);

/**
 * @brief Gets the #i18n_ucalendar_h associated with an #i18n_udate_format_h.
 * @details An #i18n_udate_format_h uses an #i18n_ucalendar_h to convert a raw value to, for example, the day of the week.
 * @since_tizen 2.3.1
 * @remarks The @a calendar should not be released.
 *
 * @param[in] format		The #i18n_udate_format_h to query.
 * @param[out] calendar		A pointer to the #i18n_ucalendar_h used by format.
 *
 * @return Error code.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_set_calendar()
 */
int i18n_udate_get_calendar(const i18n_udate_format_h format, i18n_ucalendar_h *calendar);

/**
 * @brief Sets the #i18n_ucalendar_h associated with an #i18n_udate_format_h.
 * @details An #i18n_udate_format_h uses an #i18n_ucalendar_h to convert a raw value to, for example, the day of the week.
 * @since_tizen 2.3.1
 *
 * @param[in] format			 The #i18n_udate_format_h.
 * @param[in] calendar_to_set	 An #i18n_ucalendar_h to be used by the format.
 *
 * @return Error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_get_calendar()
 */
int i18n_udate_set_calendar(i18n_udate_format_h format, const i18n_ucalendar_h calendar_to_set);

/**
 * @brief Gets the #i18n_unumber_format_h associated with an #i18n_udate_format_h.
 * @details An #i18n_udate_format_h uses an #i18n_unumber_format_h to format numbers within a date, for example the day number.
 * @since_tizen 2.3.1
 * @remarks The @a number_format should not be released.
 *
 * @param[in] format			The formatter to query.
 * @param[out] number_format	A pointer to the #i18n_unumber_format_h used by @a format to format numbers.
 *
 * @return Error code.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_set_number_format()
 */
int i18n_udate_get_number_format(const i18n_udate_format_h format, i18n_unumber_format_h *number_format);

/**
 * @brief Sets the #i18n_unumber_format_h associated with an #i18n_udate_format_h.
 * @details An #i18n_udate_format_h uses an #i18n_unumber_format_h to format numbers within a date, for example the day number.
 * @since_tizen 2.3.1
 *
 * @param[in] format				 The #i18n_udate_format_h to set.
 * @param[in] number_format_to_set	 An #i18n_unumber_format_h to be used by @a format to format numbers.
 *
 * @return Error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_get_number_format()
 */
int i18n_udate_set_number_format(i18n_udate_format_h format, const i18n_unumber_format_h number_format_to_set);

/**
 * @brief Gets a locale for which date/time formatting patterns are available.
 * @details An #i18n_udate_format_h in a locale returned by this function will perform the correct formatting and parsing for the locale.
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_index	   The index of the desired locale.
 *
 * @return A locale for which date/time formatting patterns are available, or 0 if none.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter

 * @see i18n_udate_count_available()
 */
const char *i18n_udate_get_available(int32_t locale_index);

/**
 * @brief Determines how many locales have date/time formatting patterns available.
 * @details This function is the most useful for determining the loop ending condition for calls to {@link #i18n_udate_get_available()}.
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @return The number of locales for which date/time formatting patterns are available.
 *
 * @exception #I18N_ERROR_NONE Successful
 *
 * @see i18n_udate_get_available()
 */
int32_t i18n_udate_count_available(void);

/**
 * @brief Gets the year relative to which all 2-digit years are interpreted.
 * @details For example, if the 2-digit start year is 2100, the year 99 will be interpreted as 2199.
 *
 * @since_tizen 2.3.1
 *
 * @param[in] format	 The #i18n_udate_format_h to get.
 * @param[out] year A pointer to the year relative to which all 2-digit years are interpreted.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_set_2digit_year_start()
 */
int i18n_udate_get_2digit_year_start(const i18n_udate_format_h format, i18n_udate *year);

/**
 * @brief Sets the year relative to which all 2-digit years will be interpreted.
 * @details For example, if the 2-digit start year is 2100, the year 99 will be interpreted as 2199.
 *
 * @since_tizen 2.3.1
 *
 * @param[in] format  The #i18n_udate_format_h to map.
 * @param[in] date	  The year relative to which all 2-digit years will be interpreted.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_get_2digit_year_start()
 */
int i18n_udate_set_2digit_year_start(i18n_udate_format_h format, i18n_udate date);

/**
 * @brief Extracts the pattern from an #i18n_udate_format_h.
 * @details The pattern will follow the pattern syntax rules.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] format		 The #i18n_udate_format_h to query.
 * @param[in] localized		 true if the pattern should be localized, false otherwise.
 * @param[out] result		 A pointer to a buffer to receive the pattern.
 * @param[in] result_length	 The maximum size of result.
 *
 * @return The total buffer size needed; if greater than result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_apply_pattern()
 */
int32_t i18n_udate_to_pattern(const i18n_udate_format_h format, i18n_ubool localized, i18n_uchar *result,
							  int32_t result_length);

/**
 * @brief Sets the pattern used by an #i18n_udate_format_h.
 * @details The pattern should follow the pattern syntax rules.
 * @since_tizen 2.3.1
 *
 * @param[in] format		 The #i18n_udate_format_h to set.
 * @param[in] localized		 true if the pattern is localized, false otherwise.
 * @param[in] pattern		 The new pattern.
 * @param[in] pattern_length The length of pattern, or -1 if NULL-terminated.
 *
 * @return Error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_to_pattern()
 */
int i18n_udate_apply_pattern(i18n_udate_format_h format, i18n_ubool localized, const i18n_uchar *pattern,
							 int32_t pattern_length);

/**
 * @brief Gets the symbols associated with an #i18n_udate_format_h.
 * @details The symbols are what an #i18n_udate_format_h uses to represent locale-specific data, for example month or day names.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] format		The #i18n_udate_format_h to query.
 * @param[in] type			The type of symbols to get.
 *							All the types defined in the #i18n_udate_format_symbol_type_e enumeration are supported.
 * @param[in] symbol_index	The desired symbol of type type.
 * @param[out] result		A pointer to a buffer to receive the pattern.
 * @param[in] result_length The maximum size of the result buffer.
 *
 * @return The total buffer size needed; if greater than result_length, the output was truncated.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_count_symbols()
 * @see #i18n_udate_set_symbols()
 */
int32_t i18n_udate_get_symbols(const i18n_udate_format_h format, i18n_udate_format_symbol_type_e type, int32_t symbol_index,
							   i18n_uchar *result, int32_t result_length);

/**
 * @brief Counts the number of particular symbols for an #i18n_udate_format_h.
 * @details This function is most useful for determining the loop termination condition for calls to {@link #i18n_udate_get_symbols()}.
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] format The #i18n_udate_format_h to query.
 * @param[in] type	 The type of symbols to count.
 *					 If wrong type is passed, 0 will be returned.
 *
 * @return The number of symbols of type @a type.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_get_symbols()
 * @see #i18n_udate_set_symbols()
 */
int32_t i18n_udate_count_symbols(const i18n_udate_format_h format, i18n_udate_format_symbol_type_e type);

/**
 * @brief Sets the symbols associated with an #i18n_udate_format_h.
 * @details The symbols are what an #i18n_udate_format_h uses to represent locale-specific data, for example month or day names.
 *
 * @since_tizen 2.3.1
 *
 * @param[in] format	   The #i18n_udate_format_h to set.
 * @param[in] type		   The type of symbols to set.
 *						   All the types defined in the #i18n_udate_format_symbol_type_e enumeration are supported.
 *						   If a type not defined in the enumeration is passed, then the #I18N_ERROR_NOT_SUPPORTED error is returned.
 * @param[in] symbol_index The index of the symbol to set of type type.
 * @param[in] value		   The new value.
 * @param[in] value_length The length of @a value, or @c -1 if NULL-terminated.
 *
 * @return Error code. Error codes not listed below are described in the #i18n_error_code_e enumeration.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udate_count_symbols()
 * @see #i18n_udate_get_symbols()
 */
int i18n_udate_set_symbols(i18n_udate_format_h format, i18n_udate_format_symbol_type_e type, int32_t symbol_index,
						   i18n_uchar *value, int32_t value_length);

/**
 * @brief Gets the locale for this date format object.
 * @details You can choose between valid and actual locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] format  The #i18n_udate_format_h to get the locale from.
 * @param[in] type	  The type of the locale we're looking for (valid or actual).
 *
 * @return The locale name.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
const char *i18n_udate_get_locale_by_type(const i18n_udate_format_h format, i18n_ulocale_data_locale_type_e type);

/**
 * @brief Sets a particular #i18n_udisplay_context_e value in the formatter, such as #I18N_UDISPLAY_CONTEXT_CAPITALIZATION_FOR_STANDALONE.
 * @remarks I18N_UDISPLAY_CONTEXT_STANDARD_NAMES and I18N_UDISPLAY_CONTEXT_DIALECT_NAMES are not supported.
 *
 * @since_tizen 2.3.1
 *
 * @param[in]  format The #i18n_udate_format_h to set a #i18n_udisplay_context_e value.
 * @param[in]  value  The #i18n_udisplay_context_e value to set.
 *
 * @return Error code. Error codes not listed below are described in #i18n_error_code_e
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_udate_set_context(i18n_udate_format_h format, i18n_udisplay_context_e value);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
