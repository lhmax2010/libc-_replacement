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
 * @file utils_i18n_udatepg.h
 * @version 0.1
 * @brief utils_i18n_udatepg
 */

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UDATEPG_MODULE Udatepg
 * @brief The Udatepg module provides flexible generation of date format patterns, like "yy-MM-dd". The user can build up the generator by adding successive patterns.
 *
 * @section CAPI_BASE_UTILS_I18N_UDATEPG_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UDATEPG_MODULE_OVERVIEW Overview
 * @details The Udatepg module provides flexible generation of date format patterns,
 * like "yy-MM-dd". The user can build up the generator by adding successive patterns.
 * Once that is done, a query can be made using a "skeleton", which is a pattern that
 * just includes the desired fields and lengths(Since Tizen 3.0 constants for skeletons are available -
 * see #I18N_UDATE_YEAR and many, many others). The generator will return the
 * "best fit" pattern corresponding to that skeleton.\n
 * The main method people will use is i18n_udatepg_get_best_pattern(), since normally
 * #i18n_udatepg_h is pre-built with data from a particular locale.
 * However, generators can be built directly from other data as well.
 *
 * All input handlers must not be @c NULL.
 *
 * @section CAPI_BASE_UTILS_I18N_UDATEPG_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Gets the best pattern according to a given locale and formats a current date and time using an i18n_udate_format_h
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

	// creates new i18n_udate_format to format dates and times
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

	// formats a date using i18n_udate_format
	i18n_udate_format_date(formatter_KR, date, formatted, 64, NULL, &formattedLength);
	i18n_ustring_copy_au_n(result, formatted, 64);
	//ex) KOREA/Seoul - Current date : Wednesday, June 18, 2014 1:34:54 PM GMT+09:00
	dlog_print(DLOG_INFO, LOG_TAG, "KOREA/Seoul - Current date : %s\n",result);

	// formats a date using i18n_udate_format
	i18n_udate_format_date(formatter_LA, date, formatted, 64, NULL, &formattedLength);
	i18n_ustring_copy_au_n(result, formatted, 64);
	//ex) America/LOS Angeles - Current date : Tuesday, June 17, 2014 9:34:54 PM Pacific Daylight Time
	dlog_print(DLOG_INFO, LOG_TAG, "America/LOS Angeles - Current date : %s\n",result);

	// formats a date using i18n_udate_format
	i18n_udate_format_date(formatter_SaoPaulo, date, formatted, 64, NULL, &formattedLength);
	i18n_ustring_copy_au_n(result, formatted, 64);
	//ex) Brazil/Sao Paulo - Current date : 6 18, 2014 AD, 1:34:54 PM GMT-2
	dlog_print(DLOG_INFO, LOG_TAG, "Brazil/Sao Paulo - Current date : %s\n",result);

	dlog_print(DLOG_INFO, LOG_TAG, "i18n_udate_destroy\n");
	// destroy an #i18n_udate_format_h
	i18n_udate_destroy(formatter_KR);
	i18n_udate_destroy(formatter_LA);
	i18n_udate_destroy(formatter_SaoPaulo);
 * @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UDATEPG_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opens a generator according to a given locale.
 * @remarks Must release @a dtpg using i18n_udatepg_destroy().
 * @since_tizen 2.3
 *
 * @param[in] locale	If @c NULL - default locale will be used.
 * @param[out] dtpg		A pointer to #i18n_udatepg_h. Must not be @c NULL.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_udatepg_create(const char *locale, i18n_udatepg_h *dtpg);

/**
 * @brief Destroys a generator.
 * @since_tizen 2.3
 *
 * @param[in] dtpg	   A pointer to #i18n_udatepg_h. Must not be @c NULL.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_udatepg_destroy(i18n_udatepg_h dtpg);

/**
 * @brief Gets the best pattern matching the input skeleton.
 * @details It is guaranteed to have all of the fields in the skeleton.
 * @remarks This function uses a non-const #i18n_udatepg_h:
 *			It uses a stateful pattern parser which is set up for each generator object,
 *			rather than creating one for each function call.
 *			Consecutive calls to this function do not affect each other,
 *			but this function cannot be used concurrently on a single generator object.
 * @since_tizen 2.3
 *
 * @param[in] dtpg					A pointer to #i18n_udatepg_h. Must not be @c NULL.
 * @param[in] skeleton				The skeleton is a pattern containing only the variable fields.\n
 *									For example, "MMMdd" and "mmhh" are skeletons. Must not be @c NULL.
 * @param[in] len					The length of the @a skeleton, >= 0.
 * @param[out] best_pattern			The best pattern found from the given @a skeleton.
 * @param[in] capacity				The capacity of @a best_pattern, >= 0
 * @param[out] best_pattern_len		The length of @a best_pattern.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW A result would not fit in the supplied buffer
 */
int i18n_udatepg_get_best_pattern(i18n_udatepg_h dtpg, const i18n_uchar *skeleton, int32_t len, i18n_uchar *best_pattern, int32_t capacity, int32_t *best_pattern_len);


/* Newly Added APIs */


/**
 * @brief Creates an empty generator, to be constructed with i18n_udatepg_add_pattern() etc.
 * @since_tizen 2.3.1
 * @remarks The @a dtpg should be released using i18n_udatepg_destroy().
 *
 * @param[out] dtpg		A pointer to the #i18n_udatepg_h handle.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_udatepg_h dtpg = NULL;
 * 	error_code = i18n_udatepg_create_empty(&dtpg);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udatepg_create_empty: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_udatepg_destroy(dtpg);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_udatepg_create_empty(i18n_udatepg_h *dtpg);

/**
 * @brief Creates a copy of a generator.
 * @since_tizen 2.3.1
 * @remarks The @a dtpg should be released using i18n_udatepg_destroy().
 *
 * @param[in] dtpg			An #i18n_udatepg_h handle to be copied. Must not be @c NULL.
 * @param[out] dtpg_clone	A pointer to clone of @a dtpg handle.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 *  i18n_udatepg_h dtpg = NULL;
 *  i18n_udatepg_h dtpg_clone = NULL;
 * 	error_code = i18n_udatepg_create_empty(&dtpg);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udatepg_create_empty: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	error_code = i18n_udatepg_clone(dtpg, &dtpg_clone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udatepg_clone: %d\n", error_code);
 * 		i18n_udatepg_destroy(dtpg);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_udatepg_destroy(dtpg_clone);
 * 	i18n_udatepg_destroy(dtpg);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_udatepg_clone(const i18n_udatepg_h dtpg, i18n_udatepg_h *dtpg_clone);

/**
 * @brief Gets the best pattern matching the input @a skeleton.
 * @details It is guaranteed to have all of the fields in the @a skeleton.
 *
 *			Note that this function uses a non-const #i18n_udatepg_h:
 *			It uses a stateful pattern parser which is set up for each generator object,
 *			rather than creating one for each function call.
 *			Consecutive calls to this function do not affect each other,
 *			but this function cannot be used concurrently on a single generator object.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				An #i18n_udatepg_h handle. Must not be @c NULL.
 * @param[in] skeleton			The @a skeleton is a pattern containing only the variable fields;
 *								for example, "MMMdd" and "mmhh" are skeletons. Must not be @c NULL.
 * @param[in] length			The length of @a skeleton, >= 0.
 * @param[in] options			Options for forcing the length of specified fields in the
 *								returned pattern to match those in the @a skeleton (when this
 *								would not happen otherwise). For default behavior, use
 *								#I18N_UDATEPG_MATCH_NO_OPTIONS.
 * @param[out] best_pattern		The best pattern found from the given @a skeleton.
 * @param[in] capacity			The capacity of @a best_pattern, >= 0.
 *
 * @return The length of @a best_pattern.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_udatepg_get_best_pattern_with_options(i18n_udatepg_h dtpg, const i18n_uchar *skeleton, int32_t length,
												   i18n_udatepg_date_time_pattern_match_options_e options, i18n_uchar *best_pattern, int32_t capacity);

/**
 * @brief Gets a unique skeleton from a given pattern. For example, both "MMM-dd" and "dd/MMM" produce the skeleton "MMMdd".
 * @details Note that this function uses a non-const #i18n_udatepg_h:
 *			It uses a stateful pattern parser which is set up for each generator object,
 *			rather than creating one for each function call.
 *			Consecutive calls to this function do not affect each other,
 *			but this function cannot be used concurrently on a single generator object.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg		  An #i18n_udatepg_h handle. Must not be @c NULL.
 * @param[in] pattern	  Input pattern, such as "dd/MMM". Must not be @c NULL.
 * @param[in] length	  The length of @c pattern, >= 0.
 * @param[out] skeleton	  Such as "MMMdd".
 * @param[in] capacity	  The capacity of @a skeleton, >= 0.
 *
 * @return The length of @a skeleton.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_udatepg_get_skeleton(i18n_udatepg_h dtpg, const i18n_uchar *pattern, int32_t length, i18n_uchar *skeleton, int32_t capacity);

/**
 * @brief Gets a unique base skeleton from a given pattern.
 * @details This is the same as the skeleton, except that differences in length are minimized so
 *			as to only preserve the difference between string and numeric form. So
 *			for example, both "MMM-dd" and "d/MMM" produce the skeleton "MMMd"(notice the single d).<br>
 *			Note that this function uses a non-const #i18n_udatepg_h :
 *			It uses a stateful pattern parser which is set up for each generator object,
 *			rather than creating one for each function call.
 *			Consecutive calls to this function do not affect each other,
 *			but this function cannot be used concurrently on a single generator object.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				An #i18n_udatepg_h handle. Must not be @c NULL.
 * @param[in] pattern			Input pattern, such as "dd/MMM". Must not be @c NULL.
 * @param[in] length			The length of @c pattern, >= 0.
 * @param[out] base_skeleton	Such as "Md".
 * @param[in] capacity			The capacity of @a base_skeleton, >= 0.
 *
 * @return The length of @c base_skeleton.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_udatepg_get_base_skeleton(i18n_udatepg_h dtpg, const i18n_uchar *pattern, int32_t length, i18n_uchar *base_skeleton, int32_t capacity);

/**
 * @brief Adds a pattern to the generator.
 * @details If the pattern has the same skeleton as an existing pattern,
 *			and the override parameter is set, then the previous
 *			value is overridden. Otherwise, the previous value is retained.
 *			In either case, the conflicting status is set and previous value is stored in conflicting pattern. <br>
 *			Note that single-field patterns (like "MMM") are automatically added, and don't need to be added explicitly!
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg							An #i18n_udatepg_h handle. Must not be @c NULL.
 * @param[in] pattern						Input pattern, such as "dd/MMM". Must not be @c NULL.
 * @param[in] pattern_length				The length of @c pattern, >= 0.
 * @param[in] override						when existing values are to be overridden use true,
 *											otherwise use false.
 * @param[out] conflicting_pattern			Previous pattern with the same skeleton.
 * @param[in]  capacity						The capacity of @c conflicting_pattern.
 * @param[out] conflict_status A pointer to the conflicting status
 *		   The value could be #I18N_UDATEPG_NO_CONFLICT, #I18N_UDATEPG_BASE_CONFLICT or #I18N_UDATEPG_CONFLICT.
 *
 * @return	Length of @c conflicting_pattern. -1 if @c conflict_status is #I18N_UDATEPG_NO_CONFLICT
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t	 i18n_udatepg_add_pattern(i18n_udatepg_h dtpg, const i18n_uchar *pattern, int32_t pattern_length, i18n_ubool override, i18n_uchar *conflicting_pattern, int32_t capacity, i18n_udatepg_date_time_pattern_conflict_e *conflict_status);

/**
 * @brief An append_item_format is a pattern used to append a field if there is no good match.
 * @details For example, suppose that the input skeleton is "GyyyyMMMd",
 *			and there is no matching pattern internally, but there is a pattern
 *			matching "yyyyMMMd", say "d-MM-yyyy". Then that pattern is used, plus the G.
 *			The way these two are conjoined is by using the append_item_format for G (era).
 *			So if that value is, say "{0}, {1}" then the final resulting
 *			pattern is "d-MM-yyyy, G". <br>
 *			There are actually three available variables : {0} is the pattern so far,
 *			{1} is the element we are adding, and {2} is the name of the element. <br>
 *			This reflects the way that the CLDR data is organized.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg		An #i18n_udatepg_h handle. Must not be @c NULL.
 * @param[in] field		One of #i18n_udatepg_date_time_pattern_field_e, such as #I18N_UDATEPG_ERA_FIELD.
 * @param[in] value		Pattern, such as "{0}, {1}". Must not be @c NULL.
 * @param[in] length	The length of @c value, >= 0.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_get_append_item_format()
 */
int i18n_udatepg_set_append_item_format(i18n_udatepg_h dtpg, i18n_udatepg_date_time_pattern_field_e field, const i18n_uchar *value, int32_t length);

/**
 * @brief Getter corresponding to i18n_udatepg_set_append_item_format().
 * @details Values below 0 or at or above #I18N_UDATEPG_FIELD_COUNT are illegal arguments.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				An #i18n_udatepg_h handle. Must not be @c NULL.
 * @param[in] field				One of #i18n_udatepg_date_time_pattern_field_e, such as #I18N_UDATEPG_ERA_FIELD.
 * @param[out] pattern_length	A pointer that will receive the length of append item format @a value.
 *
 * @return The append_item_format for @a field
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_set_append_item_format()
 */
const i18n_uchar *i18n_udatepg_get_append_item_format(const i18n_udatepg_h dtpg, i18n_udatepg_date_time_pattern_field_e field, int32_t *pattern_length);

/**
 * @brief Sets the name of field, e.g. "era" in English for ERA.
 * @details These are only used if the corresponding append_item_format is used, and if it contains a {2} variable.
 *			This reflects the way that the CLDR data is organized.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg		An #i18n_udatepg_h handle. Must not be @c NULL.
 * @param[in] field		#i18n_udatepg_date_time_pattern_field_e, such as #I18N_UDATEPG_ERA_FIELD.
 * @param[in] value		Name for the @c field. Must not be @c NULL.
 * @param[in] length	The length of @c value, >= 0.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_get_append_item_name()
 */
int i18n_udatepg_set_append_item_name(i18n_udatepg_h dtpg, i18n_udatepg_date_time_pattern_field_e field, const i18n_uchar *value,
									  int32_t length);

/**
 * @brief Getter corresponding to i18n_udatepg_set_append_item_name().
 * @details Values below 0 or at or above #I18N_UDATEPG_FIELD_COUNT are illegal arguments.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				The #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[in] field				#i18n_udatepg_date_time_pattern_field_e, such as #I18N_UDATEPG_ERA_FIELD.
 * @param[out] pattern_length	A pointer that will receive the length of the name for @c field.
 *
 * @return The name for @c field
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_set_append_item_name()
 */
const i18n_uchar *i18n_udatepg_get_append_item_name(const i18n_udatepg_h dtpg, i18n_udatepg_date_time_pattern_field_e field,
													int32_t *pattern_length);

/**
 * @brief The date time format is a message format pattern used to compose date and time patterns.
 * @brief The default value is "{0} {1}", where {0} will be replaced by the date pattern and {1} will be replaced by the time pattern.
 *		  This is used when the input skeleton contains both date and time fields,
 *		  but there is not a close match among the added patterns.
 *		  For example, suppose that this object was created by adding "dd-MMM" and "hh:mm",
 *		  and its date time format is the default "{0} {1}". Then if the input skeleton is "MMMdhmm",
 *		  there is not an exact match, so the input skeleton is broken up into two components "MMMd" and "hmm".
 *		  There are close matches for those two skeletons, so the result is put together with this pattern, resulting in "d-MMM h:mm".
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				An #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[in] date_time_format	A message format pattern, here {0} will be replaced by the date pattern
 *								and {1} will be replaced by the time pattern. Must not be @c NULL.
 * @param[in] length			The length of @c date_time_format, >= 0.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_get_date_time_format()
 */
int i18n_udatepg_set_date_time_format(const i18n_udatepg_h dtpg, const i18n_uchar *date_time_format, int32_t length);

/**
 * @brief Getter corresponding to i18n_udatepg_set_date_time_format().
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				An #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[out] pattern_length	A pointer that will receive the length of the @a date_time_format.
 *
 * @return A date_time_format
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_set_date_time_format()
 */
const i18n_uchar *i18n_udatepg_get_date_time_format(const i18n_udatepg_h dtpg, int32_t *pattern_length);

/**
 * @brief The decimal value is used in formatting fractions of seconds.
 * @details If the skeleton contains fractional seconds, then this is used with the fractional seconds.
 *			For example, suppose that the input pattern is "hhmmssSSSS",
 *			and the best matching pattern internally is "H:mm:ss", and the decimal string is ",".
 *			Then the resulting pattern is modified to be "H:mm:ss,SSSS"
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg		The #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[in] decimal	Decimal. Must not be @c NULL.
 * @param[in] length	The length of @c decimal, >= 0.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_get_decimal()
 */
int i18n_udatepg_set_decimal(i18n_udatepg_h dtpg, const i18n_uchar *decimal, int32_t length);

/**
 * @brief Getter corresponding to i18n_udatepg_set_decimal().
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				The #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[out] pattern_length	A pointer that will receive the length of the @a decimal string.
 *
 * @return Corresponding to the decimal point.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_udatepg_set_decimal()
 */
const i18n_uchar *i18n_udatepg_get_decimal(const i18n_udatepg_h dtpg, int32_t *pattern_length);

/**
 * @brief Adjusts the field types (width and subtype) of a @a pattern to match what is in a @a skeleton.
 * @details That is, if you supply a @a pattern like "d-M H:m", and a @a skeleton of "MMMMddhhmm",
 *			then the input pattern is adjusted to be "dd-MMMM hh:mm".
 *			This is used internally to get the best match for the input @a skeleton, but can also be used externally.<br>
 *			Note that this function uses a non-const #i18n_udatepg_h:
 *			It uses a stateful pattern parser which is set up for each generator object,
 *			rather than creating one for each function call. Consecutive calls to this function do not affect each other,
 *			but this function cannot be used concurrently on a single generator object.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				The #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[in] pattern			Input pattern. Must not be @c NULL.
 * @param[in] pattern_length	The length of input @a pattern, >= 0.
 * @param[in] skeleton			The skeleton. Must not be @c NULL.
 * @param[in] skeleton_length	The length of input @a skeleton, >= 0.
 * @param[out] dest				Pattern adjusted to match the @a skeleton fields widths and subtypes.
 * @param[in] dest_capacity		The capacity of @a dest, >= 0.
 *
 * @return The length of @a dest.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_udatepg_replace_field_types(i18n_udatepg_h dtpg, const i18n_uchar *pattern, int32_t pattern_length,
										 const i18n_uchar *skeleton, int32_t skeleton_length, i18n_uchar *dest, int32_t dest_capacity);

/**
 * @brief Adjusts the field types (width and subtype) of a pattern to match what is in a @a skeleton.
 * @details That is, if you supply a @a pattern like "d-M H:m", and a @a skeleton of "MMMMddhhmm",
 *			then the input @a pattern is adjusted to be "dd-MMMM hh:mm".
 *			This is used internally to get the best match for the input @a skeleton, but can also be used externally.<br>
 *			Note that this function uses a non-const #i18n_udatepg_h:
 *			It uses a stateful pattern parser which is set up for each generator object,
 *			rather than creating one for each function call. Consecutive calls to this function do not affect each other,
 *			but this function cannot be used concurrently on a single generator object.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				The #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[in] pattern			Input pattern. Must not be @c NULL.
 * @param[in] pattern_length	The length of input @a pattern, >= 0.
 * @param[in] skeleton			The skeleton. Must not be @c NULL.
 * @param[in] skeleton_length	The length of input @a skeleton, >= 0.
 * @param[in] options			Options controlling whether the length of specified
 *								fields in the @a pattern are adjusted to match those in the @a skeleton
 *								(when this would not happen otherwise).
 *								For default behavior, use #I18N_UDATEPG_MATCH_NO_OPTIONS.
 * @param[out] dest				Pattern adjusted to match the @a skeleton fields widths and subtypes.
 * @param[in] dest_capacity		The capacity of @a dest, >= 0.
 *
 * @return The length of @a dest.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_udatepg_replace_field_types_with_options(i18n_udatepg_h dtpg, const i18n_uchar *pattern, int32_t pattern_length,
													  const i18n_uchar *skeleton, int32_t skeleton_length, i18n_udatepg_date_time_pattern_match_options_e options,
													  i18n_uchar *dest, int32_t dest_capacity);

/**
 * @brief Creates an #i18n_uenumeration_h for list of all the skeletons in canonical form.
 * @details Call i18n_udatepg_get_pattern_for_skeleton() to get the corresponding pattern.
 * @since_tizen 2.3.1
 * @remarks The @a enumeration should be released using i18n_uenumeration_destroy().
 *
 * @param[in] dtpg		An #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[out] enumeration A pointer to the #i18n_uenumeration_h for list of all the skeletons. The caller must destroy the object.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h enumeration = NULL;
 * 	i18n_udatepg_h dtpg = NULL;
 * 	error_code = i18n_udatepg_create_empty(&dtpg);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udatepg_create_empty: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	error_code = i18n_udatepg_skeletons_create(dtpg, &enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udatepg_skeletons_create: %d\n", error_code);
 * 		i18n_udatepg_destroy(dtpg);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(enumeration);
 * 	i18n_udatepg_destroy(dtpg);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_udatepg_skeletons_create(const i18n_udatepg_h dtpg, i18n_uenumeration_h *enumeration);

/**
 * @brief Creates an #i18n_uenumeration_h for list of all the base skeletons in canonical form.
 * @since_tizen 2.3.1
 * @remarks The @a enumeration should be released using i18n_uenumeration_destroy().
 *
 * @param[in] dtpg		An #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[out] enumeration A pointer to the #i18n_uenumeration_h for list of all the base skeletons. The caller must destroy the object.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_error_code_e error_code;
 * 	i18n_uenumeration_h enumeration = NULL;
 * 	i18n_udatepg_h dtpg = NULL;
 * 	error_code = i18n_udatepg_create_empty(&dtpg);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udatepg_create_empty: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	error_code = i18n_udatepg_base_skeletons_create(dtpg, &enumeration);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_udatepg_base_skeletons_create: %d\n", error_code);
 * 		i18n_udatepg_destroy(dtpg);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_uenumeration_destroy(enumeration);
 * 	i18n_udatepg_destroy(dtpg);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_udatepg_base_skeletons_create(const i18n_udatepg_h dtpg, i18n_uenumeration_h *enumeration);

/**
 * @brief Gets the pattern corresponding to a given skeleton.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] dtpg				The #i18n_udate_format_h handle. Must not be @c NULL.
 * @param[in] skeleton			The skeleton. Must not be @c NULL.
 * @param[in] skeleton_length	The length of @a skeleton, >= 0.
 * @param[out] pattern_length	The pointer to the length of return pattern
 *
 * @return Pattern corresponding to a given skeleton
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
const i18n_uchar *i18n_udatepg_get_pattern_for_skeleton(const i18n_udatepg_h dtpg, const i18n_uchar *skeleton, int32_t skeleton_length, int32_t *pattern_length);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */
