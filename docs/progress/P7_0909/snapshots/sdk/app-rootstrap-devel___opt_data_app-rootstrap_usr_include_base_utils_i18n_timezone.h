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
 * @file utils_i18n_timezone.h
 * @version 0.1
 * @brief utils_i18n_timezone
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE Timezone
 * @brief The Timezone module represents a time zone offset, and also figures out daylight savings.
 * @section CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE_OVERVIEW Overview
 * @details The Timezone module represents a time zone offset, and also figures out daylight savings.\n
 * Typically, you get an i18n_timezone_h using #i18n_timezone_create_default which creates a TimeZone based on the time zone
 * where the program is running. For example, for a program running in Japan,
 * #i18n_timezone_create_default creates an i18n_timezone_h based on Japanese Standard Time.\n
 * You can also get an i18n_timezone_h using #i18n_timezone_create along with a time zone ID.
 * For instance, the time zone ID for the US Pacific Time zone is "America/Los_Angeles".
 * So, you can get a Pacific Time i18n_timezone_h with:\n
 * <code>
 * i18n_timezone_h timezone;\n
 * i18n_timezone_create(&timezone, "America/Los_Angeles");\n
 * </code>
 * To create a new i18n_timezone_h, you call the factory function #i18n_timezone_create() and pass it a time zone ID.
 * You can use the int #i18n_timezone_foreach_timezone_id() function to obtain a list of all the time zone IDs recognized by #i18n_timezone_create().\n
 * You can also use #i18n_timezone_create_default() to create an i18n_timezone_h. This function uses platform-specific APIs to produce
 * an i18n_timezone_h for the time zone corresponding to the client's computer's physical location.
 * For example, if you're in Japan (assuming your machine is set up correctly), #i18n_timezone_create_default()
 * will return an i18n_timezone_h for Japanese Standard Time ("Asia/Tokyo").
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE
 * @{
 */

/**
 * @brief Returns the "unknown" time zone.
 * @details #i18n_timezone_create() returns a mutable clone of this time zone if the input ID is not recognized.
 * @since_tizen 2.3
 * @remarks The @a timezone should be released using i18n_timezone_destory().
 *
 * @param[out] timezone the "unknown" time zone.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @see i18n_timezone_create()
 * @see i18n_timezone_create_gmt()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 * int main() {
 * 	i18n_timezone_h timezone;
 * 	i18n_error_code_e error_code;
 * 	char *timezone_id = NULL;
 *
 * 	error_code = i18n_timezone_create_unknown(&timezone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating unknown timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_timezone_get_id(timezone, &timezone_id);
 * 	printf("Created unknown timezone: %s\n", timezone_id);
 *
 * 	free(timezone_id);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_create_unknown(i18n_timezone_h *timezone);

/**
 * @brief The GMT (=UTC) time zone has a raw offset of zero and does not use daylight savings time.
 * @details This is a commonly used time zone.\n
 * Note: For backward compatibility reason, the ID used by the time zone returned by this method is "GMT", although the I18N's canonical ID for the GMT time zone is "Etc/GMT".
 * @since_tizen 2.3
 * @remarks The @a timezone should be released using i18n_timezone_destory().
 *
 * @param[out] timezone the GMT/UTC time zone.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @see i18n_timezone_create_unknown()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_timezone_h timezone;
 * 	i18n_error_code_e error_code;
 * 	char *timezone_id = NULL;
 *
 * 	error_code = i18n_timezone_create_gmt(&timezone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating GMT timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_timezone_get_id(timezone, &timezone_id);
 * 	printf("Created gmt timezone: %s\n", timezone_id);
 *
 * 	free(timezone_id);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_create_gmt(i18n_timezone_h *timezone);

/**
 * @brief Creates an i18n_timezone_h for the given timezone_id.
 * @since_tizen 2.3
 * @remarks The @a timezone should be released using i18n_timezone_destory().
 *
 * @param[out] timezone the GMT/UTC time zone.
 * @param[in]  timezone_id the ID for an i18n_timezone_h, such as "America/Los_Angeles", or a custom ID such as "GMT-8:00".
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_timezone_h timezone;
 * 	i18n_error_code_e error_code;
 * 	char *timezone_id = NULL;
 *
 * 	error_code = i18n_timezone_create(&timezone, "Asia/Seoul");
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_timezone_get_id(timezone, &timezone_id);
 * 	printf("Created timezone: %s\n", timezone_id);
 *
 * 	free(timezone_id);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_create(i18n_timezone_h *timezone, const char *timezone_id);

/**
 * @brief Destroys an i18n_timezone_h.
 * @details Once destroyed, an i18n_timezone_h may no longer be used.
 * @since_tizen 2.3
 *
 * @param[in] timezone The i18n_timezone_h to destroy.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_destroy(i18n_timezone_h timezone);

/**
 * @brief Returns an enumeration over system time zone IDs with the given filter conditions.
 * @since_tizen 2.3
 *
 * @param[in] timezone_type The system time zone type.
 * @param[in] region The ISO 3166 two-letter country code or UN M.49 three-digit area code. When NULL, no filtering done by region.
 * @param[in] raw_offset An offset from GMT in milliseconds, ignoring the effect of daylight savings time, if any. When NULL, no filtering done by zone offset.
 * @param[in] cb The callback function to get an enumeration object, owned by the caller.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_foreach_timezone_id_by_region(i18n_system_timezone_type_e timezone_type, const char *region, const int32_t *raw_offset, i18n_timezone_id_cb cb, void *user_data);

/**
 * @brief Returns an enumeration over all recognized time zone IDs.
 * (i.e., all strings that #i18n_timezone_create() accepts)
 * @since_tizen 2.3
 *
 * @param[in] cb The callback function to get an enumeration object, owned by the caller.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_foreach_timezone_id(i18n_timezone_id_cb cb, void *user_data);

/**
 * @brief Returns an enumeration over time zone IDs with a given raw offset from GMT.
 * @details There may be several times zones with the same GMT offset that differ in the way they
 * handle daylight savings time. For example, the state of Arizona doesn't observe daylight
 * savings time. If you ask for the time zone IDs corresponding to GMT-7:00,
 * you'll get back an enumeration over two time zone IDs: "America/Denver,"
 * which corresponds to Mountain Standard Time in the winter and Mountain Daylight Time in the summer,
 * and "America/Phoenix", which corresponds to Mountain Standard Time year-round, even in the summer.
 *
 * @since_tizen 2.3
 *
 * @param[in] raw_offset an offset from GMT in milliseconds, ignoring the effect of daylight savings time, if any
 * @param[in] cb The callback function to get an enumeration object, owned by the caller.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_foreach_timezone_id_with_offset(int32_t raw_offset, i18n_timezone_id_cb cb, void *user_data);

/**
 * @brief Returns an enumeration over time zone IDs associated with the given country.
 * @details Some zones are affiliated with no country (e.g., "UTC"); these may also be retrieved, as a group.
 * @since_tizen 2.3
 *
 * @param[in] country The ISO 3166 two-letter country code, or NULL to retrieve zones not affiliated with any country.
 * @param[in] cb The callback function to get an enumeration object, owned by the caller.
 * @param[in] user_data The user data to be passed to the callback function.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_foreach_timezone_id_by_country(const char *country, i18n_timezone_id_cb cb, void *user_data);

/**
 * @brief Returns the number of IDs in the equivalency group that includes the given ID.
 * @details An equivalency group contains zones that have the same GMT offset and rules.\n
 * The returned count includes the given ID; it is always >= 1. The given ID must be a system time zone. If it is not, returns zero.
 * @since_tizen 2.3
 *
 * @param[in] timezone_id a system time zone ID
 * @param[out] count the number of zones in the equivalency group containing 'timezone_id', or zero if 'timezone_id' is not a valid system ID
 *
 * @retval #I18N_ERROR_NONE Successful
 * @see i18n_timezone_get_equivalent_id()
 */
int i18n_timezone_count_equivalent_ids(const char *timezone_id, int32_t *count);

/**
 * @brief Returns an ID in the equivalency group that includes the given ID.
 * @details An equivalency group contains zones that have the same GMT offset and rules.\n
 * The given index must be in the range 0..n-1, where n is the out parameter value
 * from i18n_timezone_count_equivalent_ids(timezone_id, &n).
 * For some value of 'index', the returned value will be equal to the given id.
 * If the given id is not a valid system time zone,
 * or if 'index' is out of range, then returns an empty string.
 * @since_tizen 2.3
 * @remarks The @a equivalent_timezone_id should be released using free().
 *
 * @param[in] timezone_id a system time zone ID
 * @param[in] index a value from 0 to n-1, where n is the out parameter value from i18n_timezone_count_equivalent_ids(timezone_id, &n)
 * @param[out] equivalent_timezone_id the ID of the index-th zone in the equivalency group containing 'timezone_id',
 *			   or an empty string if 'timezone_id' is not a valid system ID or 'index' is out of range
 *
 * @retval #I18N_ERROR_NONE Successful
 * @see i18n_timezone_count_equivalent_ids()
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	const char *timezone_id = "PST";
 * 	int32_t index = 0;
 * 	char *equivalent_timezone_id;
 * 	ret = i18n_timezone_get_equivalent_id(timezone_id, index, &equivalent_timezone_id);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	printf("equivalent timezone id: %s\n", equivalent_timezone_id);
 * 	free(equivalent_timezone_id);
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_get_equivalent_id(const char *timezone_id, int32_t index, char **equivalent_timezone_id);

/**
 * @brief Creates a new copy of the default i18n_timezone_h for this host.
 * @details Unless the default time zone has already been set using #i18n_timezone_set_default(),
 * the default is determined by querying the system using methods in TPlatformUtilities.
 * If the system routines fail, or if they specify an i18n_timezone_h or i18n_timezone_h offset
 * which is not recognized, the i18n_timezone_h indicated by the ID kLastResortID
 * is instantiated and made the default.
 *
 * This function determines the default timezone by querying the system once and storing the
 * obtained timezone as default until the application terminates.
 *
 * To query the system for the current timezone, use i18n_timezone_detect_host_timezone().
 *
 * @since_tizen 2.3
 * @remarks The @a timezone should be released using i18n_timezone_destory().
 *
 * @param[out] timezone A default i18n_timezone_h. Clients are responsible for deleting the time zone object returned.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_timezone_h timezone;
 * 	i18n_error_code_e error_code;
 * 	char *timezone_id = NULL;
 *
 * 	error_code = i18n_timezone_create_default(&timezone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating default timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 * 	i18n_timezone_get_id(timezone, &timezone_id);
 * 	printf("Created default timezone: %s\n", timezone_id);
 *
 * 	free(timezone_id);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_create_default(i18n_timezone_h *timezone);

/**
 * @brief Sets the default time zone (i.e., what's returned by #i18n_timezone_create_default()) to be the specified time zone.
 * @details If NULL is specified for the time zone, the default time zone is set to the default host time zone.
 * The caller remains responsible for deleting it. \n
 * This function is not thread safe. It is an error for multiple threads to concurrently attempt to set
 * the default time zone, or for any thread to attempt to reference the default zone while another thread is setting it.
 *
 * @since_tizen 2.3
 * @remarks Do not use unless you know what you are doing.
 *
 * @param[in] timezone The given timezone.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_set_default(i18n_timezone_h timezone);

/**
 * @brief Returns the timezone data version currently used by I18N.
 * @remarks The specific error code can be obtained using the get_last_result()
 *		method. Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3
 *
 * @return the version string, such as "2007f"
 * @exception #I18N_ERROR_NONE Successful
 */
const char *i18n_timezone_get_tzdata_version(void);

/**
 * @brief Gets the region code associated with the given system time zone ID.
 * @details The region code is either ISO 3166 2-letter country code or UN M.49 3-digit area code.
 * When the time zone is not associated with a specific location, for example - "Etc/UTC",
 * "EST5EDT", then this method returns "001" (UN M.49 area code for World).
 * @since_tizen 2.3
 *
 * @param[in]  timezone_id The system time zone ID.
 * @param[out] region Output buffer for receiving the region code.
 * @param[out] region_len The length of the region code.
 * @param[in]  region_capacity The size of the output buffer. If it is lower than required @a region buffer size,
 *							   then I18N_ERROR_BUFFER_OVERFLOW error is returned.
 *
 * @return the version string, such as "2007f"
 */
int i18n_timezone_get_region(const char *timezone_id, char *region, int32_t *region_len, int32_t region_capacity);

/**
 * @brief Returns the time zone raw and GMT offset for the given moment in time.
 * @details Upon return, local-millis = GMT-millis + rawOffset + dstOffset. All computations are performed
 * in the proleptic Gregorian calendar.
 * @since_tizen 2.3
 *
 * @param[in]  timezone The i18n_timezone_h to get an offset.
 * @param[in]  date moment in time for which to return offsets, in units of milliseconds from January 1, 1970 0:00 GMT, either GMT time or local wall time, depending on `local'.
 * @param[in]  local output if true, `date' is local wall time; otherwise it is in GMT time.
 * @param[out] raw_offset parameter to receive the raw offset, that is, the offset not including DST adjustments
 * @param[out] dst_offset output parameter to receive the DST offset, that is, the offset to be added to `raw_offset' to obtain the total offset between local and GMT time. If DST is not in effect, this value is zero; otherwise it is a positive value, typically one hour.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_get_offset_with_date(i18n_timezone_h timezone, i18n_udate date, i18n_ubool local, int32_t *raw_offset, int32_t *dst_offset);

/**
 * @brief Sets the i18n_timezone_h's raw GMT offset
 *	   (i.e., the number of milliseconds to add to GMT to get local time, before taking daylight savings time into account).
 * @since_tizen 2.3
 *
 * @param[in] timezone The i18n_timezone_h to set a raw offset.
 * @param[in] offset_milliseconds The new raw GMT offset for this time zone.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_set_raw_offset(i18n_timezone_h timezone, int32_t offset_milliseconds);

/**
 * @brief Gets the i18n_timezone_h's raw GMT offset (i.e., the number of milliseconds
 * to add to GMT to get local time, before taking daylight savings time into account).
 * @since_tizen 2.3
 *
 * @param[in] timezone The i18n_timezone_h to get a raw offset.
 * @param[out] offset_milliseconds The i18n_timezone_h's raw GMT offset.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_get_raw_offset(i18n_timezone_h timezone, int32_t *offset_milliseconds);

/**
 * @brief Fills in "timezone_id" with the i18n_timezone_h's ID.
 * @since_tizen 2.3
 * @remarks The @a timezone_id should be released using free().
 *
 * @param[in] timezone The i18n_timezone_h to get a timezone ID.
 * @param[out] timezone_id Receives this i18n_timezone_h's ID.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_timezone_h timezone;
 * 	i18n_error_code_e error_code;
 * 	char *timezone_id = NULL;
 *
 * 	error_code = i18n_timezone_create(&timezone, "Asia/Seoul");
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_timezone_get_id(timezone, &timezone_id);
 * 	printf("Created timezone: %s\n", timezone_id);
 *
 * 	free(timezone_id);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_get_id(i18n_timezone_h timezone, char **timezone_id);

/**
 * @brief Sets the i18n_timezone_h's ID to the specified value.
 * @details This doesn't affect any other fields. for example,\n
 * 	  \n<code>
 * 	  i18n_timezone_h timezone = NULL;\n
 * 	  i18n_timezone_create ( &timezone, "America/New_York" );\n
 * 	  i18n_timezone_set_id ( "America/Los_Angeles" );\n
 * 	  </code>\n
 * the timezone's GMT offset and daylight-savings rules don't change to those for Los Angeles.
 * They're still those for New York. Only the ID has changed.
 * @since_tizen 2.3
 *
 * @param[in] timezone The i18n_timezone_h to set a timezone ID.
 * @param[in] timezone_id The new time zone ID.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_set_id(i18n_timezone_h timezone, const char *timezone_id);

/**
 * @brief Returns a name of this time zone suitable for presentation to the user in the default locale.
 * @details This method returns the long name, not including daylight savings.
 * If the display name is not available for the locale, then this method returns a string in the localized GMT offset format such as GMT[+-]HH:mm.
 * @since_tizen 2.3
 * @remarks The @a display_name should be released using free().
 *
 * @param[in] timezone The i18n_timezone_h to get a display name.
 * @param[out] display_name The human-readable name of this time zone in the default locale.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main()
 * {
 * 	char *display_name;
 * 	i18n_timezone_h timezone = NULL;
 * 	i18n_error_code_e error_code;
 * 	char timezone_id[10];
 * 	int gmt = -12;
 * 	for(; gmt < 15; gmt++) {
 * 		snprintf(timezone_id, 10, (gmt<0)? "GMT%d:00" : "GMT+%d:00", gmt);
 * 		error_code = i18n_timezone_create(&timezone, timezone_id);
 * 		if (error_code != I18N_ERROR_NONE) {
 * 			printf("Error i18n_timezone_create: %d\n", error_code);
 * 			return EXIT_FAILURE;
 * 		}
 *
 * 		error_code = i18n_timezone_get_display_name(timezone, &display_name);
 * 		if (error_code != I18N_ERROR_NONE) {
 * 			printf("Error i18n_timezone_get_display_name: %d\n", error_code);
 * 			i18n_timezone_destroy(timezone);
 * 			return EXIT_FAILURE;
 * 		}
 * 		printf("Created %s timezone display_name: %s\n", timezone_id, display_name);
 * 		free(display_name);
 * 		i18n_timezone_destroy(timezone);
 * 	}
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_get_display_name(i18n_timezone_h timezone, char **display_name);

/**
 * @deprecated Deprecated since 5.0. Use i18n_timezone_get_display_name_with_locale_id() instead.
 * @brief Returns a name of this time zone suitable for presentation to the user in the default locale.
 * @details This method returns the long name, not including daylight savings.
 * If the display name is not available for the locale, then this method returns a string in the localized GMT offset format such as GMT[+-]HH:mm.
 * @since_tizen 2.3
 * @remarks The @a display_name should be released using free().
 *
 * @param[in] timezone The i18n_timezone_h to get a display name.
 * @param[in] language The language in which to supply the display name. This parameter can be NULL; if so, the locale is initialized to match the current default locale.
 * @param[in] country The country in which to supply the display name. This parameter can be NULL.
 * @param[out] display_name The human-readable name of this time zone in the default locale.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main()
 * {
 * 	const char *language = I18N_ULOCALE_ENGLISH;
 * 	const char *country = "US";
 * 	char *display_name;
 * 	i18n_timezone_h timezone = NULL;
 * 	i18n_error_code_e error_code;
 * 	char timezone_id[10];
 * 	int gmt = -12;
 * 	for(; gmt < 15; gmt++) {
 * 		snprintf(timezone_id, 10, (gmt<0)? "GMT%d:00" : "GMT+%d:00", gmt);
 * 		error_code = i18n_timezone_create(&timezone, timezone_id);
 * 		if (error_code != I18N_ERROR_NONE) {
 * 			printf("Error i18n_timezone_create: %d\n", error_code);
 * 			return EXIT_FAILURE;
 * 		}
 *
 * 		error_code = i18n_timezone_get_display_name_with_locale(timezone, language, country, &display_name);
 * 		if (error_code != I18N_ERROR_NONE) {
 * 			printf("Error i18n_timezone_get_display_name_with_locale: %d\n", error_code);
 * 			i18n_timezone_destroy(timezone);
 * 			return EXIT_FAILURE;
 * 		}
 * 		printf("Created %s timezone display_name: %s\n", timezone_id, display_name);
 * 		free(display_name);
 *
 * 		i18n_timezone_destroy(timezone);
 * 	}
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_get_display_name_with_locale(i18n_timezone_h timezone, const char *language, const char *country , char **display_name) TIZEN_DEPRECATED_API;

/**
 * @brief Returns a name of this time zone suitable for presentation to the user in the default locale.
 * @details If the display name is not available for the locale,
 * then this method returns a string in the localized GMT offset format such as GMT[+-]HH:mm.
 * @since_tizen 2.3
 * @remarks The @a display_name should be released using free().
 *
 * @param[in] timezone The i18n_timezone_h to get a display name.
 * @param[in] daylight If true, display_name is filled with the daylight savings name.
 * @param[in] style The style displayed on.
 * @param[out] display_name The human-readable name of this time zone in the default locale.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main()
 * {
 * 	i18n_timezone_h timezone = NULL;
 * 	i18n_ubool daylight = true;
 * 	i18n_timezone_display_type_e type = I18N_TIMEZONE_DISPLAY_TYPE_SHORT;
 * 	i18n_error_code_e error_code;
 * 	char *display_name;
 * 	char timezone_id[10];
 * 	int gmt = -12;
 * 	for(; gmt < 15; gmt++) {
 * 		type = I18N_TIMEZONE_DISPLAY_TYPE_SHORT;
 * 		snprintf(timezone_id, 10, (gmt<0)? "GMT%d:00" : "GMT+%d:00", gmt);
 * 		error_code = i18n_timezone_create(&timezone, timezone_id);
 * 		if (error_code != I18N_ERROR_NONE) {
 * 			printf("Error i18n_timezone_create: %d\n", error_code);
 * 			return EXIT_FAILURE;
 * 		}
 *
 * 		for(; type <= I18N_TIMEZONE_DISPLAY_TYPE_GENERIC_LOCATION; type++) {
 * 			error_code = i18n_timezone_get_display_name_with_type(timezone, daylight, type, &display_name);
 * 			if (error_code != I18N_ERROR_NONE) {
 * 				printf("Error i18n_timezone_get_display_name_with_type: %d\n", error_code);
 * 				i18n_timezone_destroy(timezone);
 * 				return EXIT_FAILURE;
 * 			}
 * 			printf("Created %s timezone display_name: %s\n", timezone_id, display_name);
 * 			free(display_name);
 * 		}
 *
 * 		error_code = i18n_timezone_destroy(timezone);
 * 	}
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_get_display_name_with_type(i18n_timezone_h timezone, i18n_ubool daylight, i18n_timezone_display_type_e style, char **display_name);

/**
 * @deprecated Deprecated since 5.0. Use i18n_timezone_get_display_name_with_type_locale_id() instead.
 * @brief Returns a name of this time zone suitable for presentation to the user in the default locale.
 * @details If the display name is not available for the locale,
 * then this method returns a string in the localized GMT offset format such as GMT[+-]HH:mm.
 * @since_tizen 2.3
 * @remarks The @a display_name should be released using free().
 *
 * @param[in] timezone The i18n_timezone_h to get a display name.
 * @param[in] daylight If true, display_name is filled with the daylight savings name.
 * @param[in] style The style displayed on.
 * @param[in] language The language in which to supply the display name. This parameter can be NULL; if so, the locale is initialized to match the current default locale.
 * @param[in] country The country in which to supply the display name. This parameter can be NULL.
 * @param[out] display_name The human-readable name of this time zone in the default locale.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_timezone_h timezone;
 * 	i18n_error_code_e error_code;
 * 	i18n_ubool daylight = true;
 * 	i18n_timezone_display_type_e style = I18N_TIMEZONE_DISPLAY_TYPE_LONG;
 * 	char *timezone_id = NULL;
 * 	const char *language = I18N_ULOCALE_ENGLISH;
 * 	const char *country = "US";
 * 	char *display_name;
 *
 * 	error_code = i18n_timezone_create_default(&timezone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating default timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_timezone_get_display_name_with_type_locale(timezone, daylight, style, language, country, &display_name);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_timezone_get_display_name_with_type_locale: %d\n", error_code);
 * 		i18n_timezone_destroy(timezone);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_timezone_get_id(timezone, &timezone_id);
 * 	printf("Created %s timezone display_name: %s\n", timezone_id, display_name);
 *
 * 	free(timezone_id);
 * 	free(display_name);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_get_display_name_with_type_locale(i18n_timezone_h timezone, i18n_ubool daylight, i18n_timezone_display_type_e style, const char *language, const char *country, char **display_name) TIZEN_DEPRECATED_API;

/**
 * @brief Queries if this time zone uses daylight savings time.
 * @since_tizen 2.3
 *
 * @param[in] timezone The i18n_timezone_h to know whether uses daylight savings time or not.
 * @param[out] daylight_time True if this time zone uses daylight savings time, False, otherwise.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_use_daylight_time(i18n_timezone_h timezone, i18n_ubool *daylight_time);

/**
 * @brief Returns true if this zone has the same rule and offset as another zone.
 * @details That is, if this zone differs only in ID, if at all.
 * @since_tizen 2.3
 *
 * @param[in] timezone The i18n_timezone_h to know whether has the same rule or not.
 * @param[in] other The i18n_timezone_h to be compared with.
 * @param[out] same_rule True if the given zone is the same as this one, with the possible exception of the ID.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_has_same_rule(i18n_timezone_h timezone, i18n_timezone_h other, i18n_ubool *same_rule);

/**
 * @brief Clones i18n_timezone_h polymorphically.
 * @details Clients are responsible for deleting the i18n_timezone_h cloned.
 * @since_tizen 2.3
 * @remarks The @a clone should be released using i18n_timezone_destroy().
 *
 * @param[in] timezone The i18n_timezone_h to clone.
 * @param[out] clone A new copy of this i18n_timezone_h.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_timezone_h timezone, clone;
 * 	i18n_error_code_e error_code;
 * 	char *timezone_id = NULL;
 *
 * 	error_code = i18n_timezone_create_default(&timezone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating default timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	error_code = i18n_timezone_clone(timezone, &clone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error i18n_timezone_get_display_name_with_type_locale: %d\n", error_code);
 * 		i18n_timezone_destroy(timezone);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_timezone_get_id(clone, &timezone_id);
 * 	printf("Created clone timezone: %s\n", timezone_id);
 *
 * 	free(timezone_id);
 * 	i18n_timezone_destroy(clone);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_clone(i18n_timezone_h timezone, i18n_timezone_h *clone);

/**
 * @brief Returns the amount of time to be added to local standard time to get local wall clock time.
 * @details The default implementation always returns 3600000 milliseconds (i.e., one hour) if this time zone observes Daylight Saving Time.
 * Otherwise, 0 (zero) is returned.\n
 * If an underlying TimeZone implementation subclass supports historical Daylight Saving Time changes,
 * this method returns the known latest daylight saving value.
 * @since_tizen 2.3
 *
 * @param[in] timezone The i18n_timezone_h to get DST savings.
 * @param[out] dst_savings The amount of saving time in milliseconds.
 *
 * @retval #I18N_ERROR_NONE Successful
 */
int i18n_timezone_get_dst_savings(i18n_timezone_h timezone, int32_t *dst_savings);

/**
 * @brief Creates an #i18n_timezone_h detected from the current host system configuration.
 * @details This function does not change the default time zone and does not update the current
 * ICU's default. It may return a different #i18n_timezone_h from the one returned by
 * i18n_timezone_create_default().
 * @since_tizen 3.0
 * @remarks The @a timezone should be released using i18n_timezone_destory().
 *
 * @param[out] timezone A new instance of #i18n_timezone_h detected from the current host system
 * configuration. Users are responsible for deleting the returned time zone object with
 * i18n_timezone_destroy().
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <utils_i18n.h>
 *
 * int main() {
 * 	i18n_timezone_h timezone;
 * 	i18n_error_code_e error_code;
 * 	char *timezone_id = NULL;
 *
 * 	error_code = i18n_timezone_detect_host_timezone(&timezone);
 * 	if (error_code != I18N_ERROR_NONE) {
 * 		printf("Error creating timezone: %d\n", error_code);
 * 		return EXIT_FAILURE;
 * 	}
 *
 * 	i18n_timezone_get_id(timezone, &timezone_id);
 * 	printf("Created timezone: %s\n", timezone_id);
 *
 * 	free(timezone_id);
 * 	i18n_timezone_destroy(timezone);
 *
 * 	return EXIT_SUCCESS;
 * }
 * @endcode
 */
int i18n_timezone_detect_host_timezone(i18n_timezone_h *timezone);

/**
 * @brief Returns a name of this time zone suitable for presentation to the user in the default locale.
 * @details This method returns the long name, not including daylight savings.
 * If the display name is not available for the locale, then this method returns a string in the localized GMT offset format such as GMT[+-]HH:mm.
 * @since_tizen 5.0
 * @remarks @a display_name is allocated on heap and should be released by the caller with the free() function.
 *
 * @param[in]  timezone        The i18n_timezone_h to get a display name
 * @param[in]  locale_id       The locale string containing language and country code
 * @param[out] display_name    The human-readable name of this time zone in the default locale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_timezone_get_display_name_with_locale_id(i18n_timezone_h timezone, const char *locale_id , char **display_name);

/**
 * @brief Returns a name of this time zone suitable for presentation to the user in the default locale.
 * @details If the display name is not available for the locale,
 * then this method returns a string in the localized GMT offset format such as GMT[+-]HH:mm.
 * @since_tizen 5.0
 * @remarks @a display_name is allocated on heap and should be released by the caller with the free() function.
 *
 * @param[in]  timezone        The i18n_timezone_h to get a display name
 * @param[in]  daylight        If true, display_name is filled with the daylight savings name
 * @param[in]  style           The style displayed on
 * @param[in]  locale_id       The locale string containing language and country code
 * @param[out] display_name    The human-readable name of this time zone in the default locale
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE              Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_timezone_get_display_name_with_type_locale_id(i18n_timezone_h timezone, i18n_ubool daylight, i18n_timezone_display_type_e style, const char *locale_id, char **display_name);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
