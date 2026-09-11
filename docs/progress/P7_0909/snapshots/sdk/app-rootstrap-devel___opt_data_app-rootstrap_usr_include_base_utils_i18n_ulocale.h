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
 *
 * Copyright (C) 1997-2013, International Business Machines
 * Corporation and others.	All Rights Reserved.
 */
#pragma once

#include <utils_i18n_types.h>

/**
 * @file utils_i18n_ulocale.h
 * @version 0.1
 * @brief utils_i18n_ulocale
 */

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_ULOCALE_MODULE Ulocale
 * @brief A Locale represents a specific geographical, political, or cultural region.
 * @section CAPI_BASE_UTILS_I18N_ULOCALE_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_ULOCALE_MODULE_OVERVIEW Overview
 * @details An operation that requires a Locale to perform its task is called locale-sensitive
 *			and uses the Locale to tailor information for the user. For example, displaying
 *			a number is a locale-sensitive operation. The number should be formatted according
 *			to the customs/conventions of the user's native country, region, or culture.
 *			A locale is simply a const char string.\n
 *			You create a Locale with one of the three options listed below. Each of the component is
 *			separated by '_' in the locale string.
 *			<code>
 *				newLanguage
 *				newLanguage + newCountry
 *				newLanguage + newCountry + newVariant
 *			</code>
 *			The first option is a valid ISO Language Code. These codes are the lower-case two-letter
 *			codes as defined by ISO-639.
 *			The second option includes an additional ISO Country Code. These codes are the
 *			upper-case two-letter codes as defined by ISO-3166. You can find a full list of these
 *			codes at a number of sites, such as: http://www.chemie.fu-berlin.de/diverse/doc/ISO_3166.html
 *			The third option requires another additional information–the Variant. The Variant codes
 *			are vendor and browser-specific. For example, use WIN for Windows, MAC for Macintosh,
 *			and POSIX for POSIX. Where there are two variants, separate them with an underscore,
 *			and put the most important one first. For example, a Traditional Spanish collation
 *			might be referenced, with "ES", "ES", "Traditional_WIN".
 *			Because a Locale is just an identifier for a region, no validity check is performed
 *			when you specify a Locale. If you want to see whether particular resources are available
 *			for the Locale you asked for, you must query those resources. For example,
 *			to create #i18n_ucollator_h object use i18n_ucollator_get_available() function for
 *			supported locales. Note: When you ask for a resource for a particular locale,
 *			you get back the best available match, not necessarily precisely what you asked for.\n
 *			The Locale provides a number of convenient constants that you can use to specify the
 *			commonly used locales. For example, the following refers to a locale for the United States:
 *			<code>#I18N_ULOCALE_US</code>\n
 *			Once you've specified a locale you can query it for information about itself.
 *			Use i18n_ulocale_get_country() to get the ISO Country Code and i18n_ulocale_get_language()
 *			to get the ISO Language Code. You can use i18n_ulocale_get_display_country() to get
 *			the name of the country suitable for displaying to the user. Similarly, you can use
 *			i18n_ulocale_get_display_language() to get the name of the language suitable for
 *			displaying to the user. Interestingly, the i18n_ulocale_get_display_xxx() methods are
 *			themselves locale-sensitive and have two versions: one that uses the default locale
 *			and one that takes a locale as an argument and displays the name or country in a language
 *			appropriate to that locale.\n
 *			The ICU provides a number of services that perform locale-sensitive operations.
 *			For example, the i18n_unumber_xxx functions format numbers, currency, or percentages
 *			in a locale-sensitive manner. Each of these functions has two variants;
 *			one with an explicit locale and one without:\n
 *			<code>
 *			i18n_unumber_format_h num_format = NULL;
 *			int error_code = i18n_unumber_create(I18N_UNUMBER_CURRENCY, NULL, -1, "fr_FR", NULL, &num_format);
 *			</code>\n
 *			the latter using the default locale:\n
 *			<code>
 *			i18n_unumber_format_h num_format = NULL;
 *			int error_code = i18n_unumber_create(I18N_UNUMBER_CURRENCY, NULL, -1, NULL, NULL, &num_format);
 *			</code>\n
 *			A Locale is the mechanism for identifying the kind of services that you would like
 *			to get. The locale is just a mechanism for identifying these services.\n
 *			Each international service that performs locale-sensitive operations allows you to get
 *			all the available objects of that type. You can sift through these objects by language,
 *			country, or variant, and use the display names to present a menu to the user.
 *			For example, you can create a menu of all the collation objects suitable for a given
 *			language. Such modules implement these two functions: i18n_xxx_get_available(),
 *			i18n_xxx_count_available(). Some of them has also i18n_xxx_get_display_name().\n
 *			Concerning POSIX/RFC1766 Locale IDs, the
 *			i18n_locale_get_language()/i18n_ulocale_get_country()/i18n_ulocale_get_variant()/i18n_ulocale_get_name()
 *			functions do understand the POSIX type form of language_COUNTRY.ENCODING\@VARIANT and
 *			if there is not an ICU-stype variant, i18n_ulocale_get_variant() for example will return
 *			the one listed after the \@at sign. As well, the hyphen "-" is recognized as
 *			a country/variant separator similarly to RFC1766. So for example, "en-us" will be
 *			interpreted as en_US. As a result, i18n_ulocale_get_name() is far from a no-op,
 *			and will have the effect of converting POSIX/RFC1766 IDs into ICU form, although it
 *			does NOT map any of the actual codes (i.e. russian->ru) in any way. Applications should
 *			call i18n_ulocale_get_name() at the point where a locale ID is coming from an external
 *			source (user entry, OS, web browser) and pass the resulting string to other ICU
 *			functions. For example, don't use de-de\@EURO as an argument to resourcebundle.
 *
 * @section CAPI_BASE_UTILS_I18N_ULOCALE_MODULE_SAMPLE_CODE_1 Sample Code 1
 * @brief Gets a default locale and a full name for the locale
 * @code
	const char *locale;
	const char *in_locale_id = "en_US";
	char language[64] = {0,};
	i18n_uchar result_w[64] = {0,};
	char result[64] = {0,};
	int language_capacity = 64;
	int buf_size_language;
	int buf_size_display_name;
	int ret = I18N_ERROR_NONE;

	// Sets default locale
	ret = i18n_ulocale_set_default(getenv("LC_TIME"));

	// Gets default locale
	ret = i18n_ulocale_get_default(&locale);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "i18n_ulocale_get_default() failed!!! \n");
	}
	dlog_print(DLOG_INFO, LOG_TAG, "default locale : %s\n", locale);	// default locale : en_GB.UTF-8

	// Gets the language code for the specified locale
	ret = i18n_ulocale_get_language(locale, language, language_capacity, &buf_size_language);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "i18n_ulocale_get_language() failed!!! \n");
	}
	dlog_print(DLOG_INFO, LOG_TAG, "language code for the locale : %s\n", language);  // language code for the locale : en

	// Gets the full name suitable for display for the specified locale
	ret = i18n_ulocale_get_display_name(locale, in_locale_id, result_w, 64, &buf_size_display_name);
	if (ret != I18N_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "i18n_ulocale_get_display_name() failed!!! \n");
	}
	i18n_ustring_copy_au(result, result_w);
	dlog_print(DLOG_INFO, LOG_TAG, "full name suitable for the locale : %s\n", result); // full name suitable for the locale : English (United Kingdom)
 * @endcode
 *
 * @section CAPI_BASE_UTILS_I18N_ULOCALE_MODULE_SAMPLE_CODE_2 Sample Code 2
 * @brief See all available locales
 * @code
	int i = 0;
	int32_t count = i18n_ulocale_count_available();
	for(i = 0; i < count; i++)
	{
		dlog_print(DLOG_INFO, LOG_TAG, "Available locale %d : %s " ,i, i18n_ulocale_get_available(i));
		//	 ...
		//Available locale 5 : en_GB
		//Available locale 6 : en_US
		//Available locale 7 : en_US_POSIX
		//	 ...
	}
 * @endcode
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_ULOCALE_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets I18N's default locale.
 * @details The returned string is a snapshot in time, and will remain valid
 *			and unchanged even when i18n_ulocale_set_default() is called.
 *			The returned storage is owned by I18N, and must not be altered or deleted by the caller.
 * @since_tizen 2.3
 * @remarks The @a locale should not be released.
 *
 * @param[out] locale	The I18N default locale
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_get_default(const char **locale);

/**
 * @brief Sets I18N's default locale.
 * @details By default (without calling this function),
 *			I18N's default locale will be based on information obtained from the underlying system environment.
 *
 *			Changes to I18N's default locale do not propagate back to the system environment.
 *
 *			Changes to I18N's default locale to not affect any services that may already be open based on the previous default locale value.
 * @since_tizen 2.3
 *
 * @param[in] locale_id		The new I18N default locale.\n
 *							A value of @c NULL will try to get the system's default locale.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_set_default(const char *locale_id);

/**
 * @brief Gets the language code for the specified locale.
 * @since_tizen 2.3
 *
 * @param[in] locale_id				The locale to get the ISO language code with.\n
 *									@c NULL may be used to specify the default.
 * @param[out] language				The language code for @a locale_id
 * @param[in] language_capacity		The size of the @a language buffer to store the language code with
 * @param[out] buf_size_language	The actual buffer size needed for the language code.\n
 *									If it's greater than @a language_capacity, the returned language code will be truncated.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_get_language(const char *locale_id, char *language, int32_t language_capacity, int32_t *buf_size_language);

/**
 * @brief Gets the country code for the specified locale.
 * @since_tizen 2.3
 *
 * @param[in]  locale_id			The locale to get the country code with
 * @param[out] country				The country code for @a locale_id
 * @param[in]  country_capacity		The size of the @a country buffer to store the country code with
 * @param[out] error				Error information if retrieving the country code failed
 *
 * @return The actual buffer size needed for the country code.\n
 *		   If it's greater than @a country_capacity, the returned country code will be truncated.
 */
int32_t i18n_ulocale_get_country(const char *locale_id, char *country, int32_t country_capacity, int *error);

/**
 * @brief Gets the full name suitable for display for the specified locale.
 * @since_tizen 2.3
 *
 * @param[in] locale_id					The locale to get the displayable name with.\n
 *										@c NULL may be used to specify the default.
 * @param[in] in_locale_id				The locale to be used to display the name.\n
 *										@c NULL may be used to specify the default.
 * @param[out] result					The displayable name for @a locale_id
 * @param[in] max_result_size			The size of the name buffer to store the displayable full name with
 * @param[out] buf_size_display_name	The actual buffer size needed for the displayable name.\n
 *										If it's greater than @a max_result_size, the returned displayable name will be truncated.
 *
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_get_display_name(const char *locale_id, const char *in_locale_id, i18n_uchar *result, int32_t max_result_size, int32_t *buf_size_display_name);

/**
 * @brief Gets the specified locale from a list of all available locales.
 * @details The return value is a pointer to an item of a locale name array.
 *			Both this array and the pointers it contains are owned by I18N
 *			and should not be deleted or written through by the caller.
 *			The locale name is terminated by a null pointer.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @param[in] n		The specific locale name index of the available locale list
 *
 * @return A specified locale name of all available locales
 * @exception #I18N_ERROR_NONE Success
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
const char *i18n_ulocale_get_available(int32_t n);

/**
 * @brief Gets the size of the all available locale list.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3
 *
 * @return The size of the locale list
 * @exception #I18N_ERROR_NONE Success
 */
int32_t i18n_ulocale_count_available(void);

/* Newly Added APIs */

/**
 * @brief Gets the script code for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			The locale to get the ISO script code with
 * @param[out] script			The script code for @a locale_id. Must not be @c NULL.
 * @param[in] script_capacity	The size of the @a script buffer to store the script code with
 *
 * @return The actual buffer size needed for the script code. If it's greater than @a script_capacity,
 *		   the returned script code will be truncated. If the @a script buffer is @c NULL
 *		   or the @a script_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_script(const char *locale_id, char *script, int32_t script_capacity);

/**
 * @brief Gets the variant code for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			The locale to get the variant code with
 * @param[out] variant			The variant code for @a locale_id
 * @param[in] variant_capacity	The size of the @a variant buffer to store the variant code with
 *
 * @return The actual buffer size needed for the variant code. If it's greater than @a variant_capacity,
 *		   the returned variant code will be truncated. If the @a variant buffer is @c NULL
 *		   or the @a variant_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_variant(const char *locale_id, char *variant, int32_t variant_capacity);

/**
 * @brief Gets the full name for the specified locale.
 * @details Note : This has the effect of 'canonicalizing' the I18N locale ID to
 *			a certain extent. Upper and lower case are set as needed.
 *			It does NOT map aliased names in any way.
 *			See the top of this header file.
 *			This API supports preflighting.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			The locale to get the full name with
 * @param[out] name				Fill in buffer for the name without keywords.
 * @param[in] name_capacity		Capacity of the fill in buffer.
 *
 * @return The actual buffer size needed for the full name. If it's greater than @a name_capacity,
 *		   the returned full name will be truncated.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_BUFFER_OVERFLOW A result would not fit in the supplied buffer
 */
int32_t i18n_ulocale_get_name(const char *locale_id, char *name, int32_t name_capacity);

/**
 * @brief Gets the full name for the specified locale.
 * @details Note : This has the effect of 'canonicalizing' the string to
 *			a certain extent. Upper and lower case are set as needed,
 *			and if the components were in 'POSIX' format they are changed to
 *			I18N format. It does NOT map aliased names in any way.
 *			See the top of this header file.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			The locale to get the full name with
 * @param[out] name				The full name for @a locale_id
 * @param[in] name_capacity		The size of the @a name buffer to store the full name with
 *
 * @return The actual buffer size needed for the full name. If it's greater than @a name_capacity,
 *		   the returned full name will be truncated.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_BUFFER_OVERFLOW A result would not fit in the supplied buffer
 */
int32_t i18n_ulocale_canonicalize(const char *locale_id, char *name, int32_t name_capacity);

/**
 * @brief Gets the ISO language code for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id		The locale to get the ISO language code with
 *
 * @return A language the ISO language code for @a locale_id
 *
 * @exception #I18N_ERROR_NONE Successful
 */
const char *i18n_ulocale_get_iso3_language(const char *locale_id);

/**
 * @brief Gets the ISO country code for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id		The locale to get the ISO country code with
 *
 * @return A country the ISO country code for @a locale_id
 *
 * @exception #I18N_ERROR_NONE Successful
 */
const char *i18n_ulocale_get_iso3_country(const char *locale_id);

/**
 * @brief Gets the Win32 LCID value for the specified locale.
 * @details If the I18N locale is not recognized by Windows, 0 will be returned.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id		The locale to get the Win32 LCID value with
 *
 * @return A country the Win32 LCID for @a locale_id
 *
 * @exception #I18N_ERROR_NONE Successful
 */
uint32_t i18n_ulocale_get_lcid(const char *locale_id);

/**
 * @brief Gets the language name suitable for display for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale				The locale to get the ISO language code with
 * @param[in] display_locale		The locale to be used to display the name.\n
 *									@c NULL may be used to specify the default.
 * @param[out] language				The displayable language code for @a locale.
 * @param[in] language_capacity		The size of the @a language buffer to store the displayable language code with
 *
 * @return The actual buffer size needed for the displayable language code. If it's greater than
 *		   @a language_capacity, the returned language code will be truncated. If the @a language buffer is @c NULL
 *		   or the @a language_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c 0 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_display_language(const char *locale, const char *display_locale, i18n_uchar *language, int32_t language_capacity);

/**
 * @brief Gets the script name suitable for display for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale			The locale to get the displayable script code with. @c NULL may be used to specify the default.
 * @param[in] display_locale	The locale to be used to display the name. @c NULL may be used to specify the default.
 * @param[out] script			The displayable country code for @a locale
 * @param[in] script_capacity	The size of the script buffer to store the displayable script code with
 *
 * @return The actual buffer size needed for the displayable script code. If it's greater than @a script_capacity,
 *		   the returned displayable script code will be truncated. If the @a script buffer is @c NULL
 *		   or the @a script_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c 0 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_display_script(const char *locale, const char *display_locale, i18n_uchar *script, int32_t script_capacity);

/**
 * @brief Gets the country name suitable for display for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale			The locale to get the displayable country code with. @c NULL may be used to specify the default.
 * @param[in] display_locale	The locale to be used to display the name. @c NULL may be used to specify the default.
 * @param[out] country			The displayable country code for @a locale
 * @param[in] country_capacity	The size of the @a country buffer to store the displayable country code with
 *
 * @return The actual buffer size needed for the displayable country code. If it's greater than @a country_capacity,
 *		   the returned displayable country code will be truncated. If the @a country buffer is @c NULL
 *		   or the @a country_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c 0 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_display_country(const char *locale, const char *display_locale, i18n_uchar *country, int32_t country_capacity);

/**
 * @brief Gets the variant name suitable for display for the specified locale.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale			The locale to get the displayable variant code with. @c NULL may be used to specify the default.
 * @param[in] display_locale	The locale to be used to display the name. @c NULL may be used to specify the default.
 * @param[out] variant			The displayable variant code for @a locale
 * @param[in] variant_capacity	The size of the @a variant buffer to store the displayable variant code with
 *
 * @return The actual buffer size needed for the displayable variant code. If it's greater than @a variant_capacity,
 *		   the returned displayable variant code will be truncated. If the @a variant buffer is @c NULL
 *		   or the @a variant_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c 0 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_display_variant(const char *locale, const char *display_locale, i18n_uchar *variant, int32_t variant_capacity);

/**
 * @brief Gets the keyword name suitable for display for the specified locale.
 * @details E.g : for the locale string de_DE\@collation=PHONEBOOK, this API gets the display string for the keyword collation.
 *			Usage :
 * @code
 *	  i18n_error_code_e status = I18N_ERROR_NONE;
 *	  const char* keyword = NULL;
 *	  int32_t keyword_len = 0;
 *	  int32_t keyword_count = 0;
 *	  i18n_uchar display_keyword[256];
 *	  int32_t display_keyword_len = 0;
 *	  i18n_uenumeration_h keyword_enum;
 *	  i18n_ulocale_keywords_create("en_US@collation=PHONEBOOK;calendar=TRADITIONAL", &keyword_enum);
 *
 *	  for(keyword_count = i18n_uenumeration_count(keyword_enum); keyword_count > 0; keyword_count--){
 *			status = get_last_result();
 *			if(status > 0){
 *				// something went wrong so handle the error
 *				break;
 *			}
 *			// the uenum_next returns NULL-terminated string
 *			keyword = i18n_uenumeration_next(keyword_enum, &keyword_len);
 *			display_keyword_len = i18n_ulocale_get_display_keyword(keyword, "en_US", display_keyword, 256);
 *			// do something interesting
 *	  }
 *	  i18n_uenumeration_destroy(keyword_enum);
 * @endcode
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] keyword			The keyword whose display string needs to be returned.
 * @param[in] display_locale	The locale to be used to display the name. @c NULL may be used to specify the default.
 * @param[out] dest				The buffer to which the displayable keyword should be written.
 * @param[in] dest_capacity		The size of the buffer (number of #i18n_uchar characters). If it is 0, then
 *								@a dest may be @c NULL and the function will only return the length of the result without
 *								writing any of the result string (pre-flighting).
 *
 * @return The actual buffer size needed for the displayable variant code. If the @a dest buffer is @c NULL
 *		   or the @a dest_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c 0 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ulocale_keywords_create()
 * @see i18n_uenumeration_count()
 * @see i18n_uenumeration_next()
 * @see i18n_uenumeration_destroy()
 */
int32_t i18n_ulocale_get_display_keyword(const char *keyword, const char *display_locale, i18n_uchar *dest, int32_t dest_capacity);

/**
 * @brief Gets the value of the keyword suitable for display for the specified locale.
 * @details E.g : for the locale string de_DE\@collation=PHONEBOOK, this API gets the display string for PHONEBOOK,
 * in the display locale, when "collation" is specified as the keyword.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale			The locale to get the displayable variant code with. @c NULL may be used to specify the default.
 * @param[in] keyword			The keyword for whose value should be used.
 * @param[in] display_locale	The locale to be used to display the name. @c NULL may be used to specify the default.
 * @param[out] dest				The buffer to which the displayable keyword should be written.
 * @param[in] dest_capacity		The size of the buffer (number of #i18n_uchar characters). If it is 0, then
 *								@a dest may be @c NULL and the function will only return the length of the result
 *								without writing any of the result string (pre-flighting).
 *
 * @return The actual buffer size needed for the displayable variant code. If the @a dest buffer is @c NULL
 *		   or the @a dest_capacity is lower than @c 0 , then the #I18N_ERROR_INVALID_PARAMETER error will be set
 *		   and @c 0 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_display_keyword_value(const char *locale, const char *keyword, const char *display_locale, i18n_uchar *dest, int32_t dest_capacity);

/**
 * @brief Gets a list of all available 2-letter language codes defined in ISO 639,
 *		  plus additional 3-letter codes determined to be useful for locale generation as
 *		  defined by Unicode CLDR.
 * @details This is a pointer to an array of pointers to arrays of char. All of these pointers are owned
 *			by I18N - do not delete them, and do not write through them.
 *			The array is terminated with a @c NULL pointer.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @return A list of all available language codes
 *
 * @exception #I18N_ERROR_NONE Successful
 */
const char *const *i18n_ulocale_get_iso_languages(void);

/**
 *
 * @brief Gets a list of all available 2-letter country codes defined in ISO 639.
 * @details This is a pointer to an array of pointers to arrays of char. All of these pointers are
 *			owned by I18N - do not delete them, and do not write through them.
 *			The array is terminated with a @c NULL pointer.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section.
 * @since_tizen 2.3.1
 *
 * @return A list of all available country codes
 *
 * @exception #I18N_ERROR_NONE Successful
 */
const char *const *i18n_ulocale_get_iso_countries(void);

/**
 * @brief Truncates the locale ID string to get the parent locale ID.
 * @details Copies the part of the string before the last underscore.
 *			The parent locale ID will be an empty string if there is no
 *			underscore, or if there is only one underscore at @a locale_id[0].
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			Input locale ID string.
 * @param[out] parent			Output string buffer for the parent locale ID. Must not be @c NULL.
 * @param[in] parent_capacity	Size of the output buffer. If it's lower than the number of characters
 *								stored in the @a locale_id between the first character and the last occurrence
 *								of the underscore ("_") character, than the error code will be set to
 *								#I18N_ERROR_BUFFER_OVERFLOW.
 *
 * @return The length of the parent locale ID. If the @a parent buffer is @c NULL or the @a parent_capacity is lower than @c 0,
 *		   then the #I18N_ERROR_INVALID_PARAMETER error will be set and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @exception #I18N_ERROR_BUFFER_OVERFLOW If the capacity of the @a parent is lower than the number of characters
 *										  in the @a locale_id from index 0 to the index of the last occurrence of
 *										  the underscore ("_") symbol.
 */
int32_t i18n_ulocale_get_parent(const char *locale_id, char *parent, int32_t parent_capacity);

/**
 * @brief Gets the full name for the specified locale, like i18n_ulocale_get_name(), but without keywords.
 * @details Note : This has the effect of 'canonicalizing' the string to a certain extent.
 *			Upper and lower case are set as needed,
 *			and if the components were in 'POSIX' format they are changed to I18N format.
 *			It does NOT map aliased names in any way.
 *			See the top of this header file.
 *
 *			This API strips off the keyword part, so "de_DE\@collation=phonebook" will become "de_DE".
 *			This API supports preflighting.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			The locale to get the full name with
 * @param[out] name				Fill in buffer for the name without keywords.
 * @param[in] name_capacity		Capacity of the fill in buffer.
 *
 * @return The actual buffer size needed for the full name. If it's greater than @a name_capacity,
 *		   the returned full name will be truncated.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_BUFFER_OVERFLOW A result would not fit in the supplied buffer
 */
int32_t i18n_ulocale_get_base_name(const char *locale_id, char *name, int32_t name_capacity);

/**
 * @brief Gets an enumeration of keywords for the specified locale.
 * @details Enumeration must get disposed of by the client using i18n_uenumeration_destroy() function.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id		The locale to get the variant code with
 *
 * @param enumeration A pointer to the enumeration of keywords or @c NULL if there are no keywords.
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_keywords_create(const char *locale_id, i18n_uenumeration_h *enumeration);

/**
 * @brief Gets the value for a keyword.
 * @details Locale name does not need to be normalized.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			A locale name containing the keyword ("de_DE@currency=EURO;collation=PHONEBOOK")
 * @param[in] keyword_name		The name of the keyword for which we want the value. Case insensitive.
 * @param[out] buffer			Receiving buffer
 * @param[in] buffer_capacity	Capacity of receiving @a buffer
 *
 * @return The length of keyword value. If the @a keyword_name or @a buffer is @c NULL or the @a buffer_capacity is lower than @c 0,
 *		   then the #I18N_ERROR_INVALID_PARAMETER error will be set and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_keyword_value(const char *locale_id, const char *keyword_name, char *buffer, int32_t buffer_capacity);

/**
 * @brief Sets or removes the value of the specified keyword.
 * @details For removing all keywords(retrieving only locale base name associated with the keywords),
 * use i18n_ulocale_get_base_name().
 *
 *			NOTE : Unlike almost every other I18N function which takes a
 *			buffer, this function will NOT truncate the output text. If a
 *			#I18N_ERROR_BUFFER_OVERFLOW is received, it means that the original
 *			buffer is untouched. This is done to prevent incorrect or possibly
 *			even malformed locales from being generated and used.
 *
 * Below code prints following logs in SDK:
 *
 * Locale and keywords: en_US\@abc=12;def=34 \n
 * keyword1: abc \n
 * keyword2: def \n
 * Locale: en_US \n
 * Keyword1 value: 12
 *
 * @code
 *	  int32_t buff_size = 50;
 *	  int32_t sub_buff_size = 45;
 *	  char *buff = malloc(sizeof(char)*buff_size);
 *	  char *sub_buff = malloc(sizeof(char)*sub_buff_size);
 *	  char *locale = "en_US";
 *
 *	  snprintf(buff, buff_size, "%s%s",locale, sub_buff);
 *
 *	  i18n_ulocale_set_keyword_value("abc", "cba", buff, buff_size);
 *	  i18n_ulocale_set_keyword_value("def", "34", buff, buff_size);
 *
 *	  i18n_uenumeration_h keyword_enum;
 *	  i18n_ulocale_keywords_create(buff, &keyword_enum);
 *
 *	  int32_t res_len = 0;
 *	  const char *keyw = i18n_uenumeration_next(keyword_enum, &res_len);
 *	  const char *keyw2 = i18n_uenumeration_next(keyword_enum, &res_len);
 *
 *	  char *only_locale = malloc(sizeof(char)*10);
 *	  i18n_ulocale_get_base_name(buff, only_locale, 10);
 *
 *	  int32_t buff_size_get = 50;
 *	  char *buff_out = malloc(sizeof(char)*buff_size_get);
 *	  i18n_ulocale_get_keyword_value(buff, "abc", buff_out, buff_size_get);
 *
 *	  dlog_print(DLOG_DEBUG, "test", "Locale and keywords: %s", buff);
 *	  dlog_print(DLOG_DEBUG, "test", "keyword1: %s", keyw);
 *	  dlog_print(DLOG_DEBUG, "test", "keyword2: %s", keyw2);
 *	  dlog_print(DLOG_DEBUG, "test", "Locale: %s", only_locale);
 *	  dlog_print(DLOG_DEBUG, "test", "Keyword1 value: %s", buff_out);
 *
 *	  free(buff_out);
 *	  free(only_locale);
 *	  i18n_uenumeration_destroy(keyword_enum)
 *	  free(sub_buff);
 *	  free(buff);
 * @endcode
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] keyword_name		A name of the keyword to be set. Case insensitive.
 * @param[in] keyword_value		A value of the keyword to be set. If 0-length or
 *								@c NULL, will result in the keyword being removed.
 *								No error is given if that keyword does not exist.
 * @param[out] buffer			Input buffer containing locale to be modified.
 * @param[in] buffer_capacity	Capacity of receiving @a buffer
 *
 * @return The length needed for the @a buffer. If the @a keyword_name or @a buffer is @c NULL or the @a buffer_capacity is lower than @c 0,
 *		   then the #I18N_ERROR_INVALID_PARAMETER error will be set and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 * @see i18n_ulocale_get_keyword_value()
 */
int32_t i18n_ulocale_set_keyword_value(const char *keyword_name, const char *keyword_value, char *buffer, int32_t buffer_capacity);

/**
 * @brief Gets the layout character orientation for the specified locale.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id		locale name
 * @param[out] layout_type A pointer to the enum indicating the layout orientation for characters.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_get_character_orientation(const char *locale_id, i18n_ulocale_layout_type_e *layout_type);

/**
 * @brief Gets the layout line orientation for the specified locale.
 * @remarks Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id		locale name
 * @param[out] layout_type A pointer to the enum indicating the layout orientation for lines.
 *
 * @return The obtained error code.
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_get_line_orientation(const char *locale_id, i18n_ulocale_layout_type_e *layout_type);

/**
 * @brief Gets the I18N locale ID for the specified Win32 LCID value.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] host_id			The Win32 LCID to translate
 * @param[out] locale			The output buffer for the I18N locale ID, which will be NULL-terminated if there is room.
 * @param[in] locale_capacity	The size of the output buffer
 *
 * @return The actual size of the locale ID, not including NULL-termination.
 *		   If the @a locale buffer is @c NULL or the @a locale_capacity is lower than @c 0,
 *		   then the #I18N_ERROR_INVALID_PARAMETER error will be set and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_get_locale_for_lcid(uint32_t host_id, char *locale, int32_t locale_capacity);

/**
 * @brief Adds the likely subtags for a provided locale ID, per the algorithm described
 *		  in the following CLDR technical report : http://www.unicode.org/reports/tr35/#Likely_Subtags
 * @details If @a locale_id is already in the maximal form, or there is no data available
 *			for maximization, it will be copied to the output buffer. For example,
 *			"und-Zzzz" cannot be maximized, since there is no reasonable maximization.
 *
 *			Examples :
 *
 *			"en" maximizes to "en_Latn_US"
 *
 *			"de" maximizes to "de_Latn_US"
 *
 *			"sr" maximizes to "sr_Cyrl_RS"
 *
 *			"sh" maximizes to "sr_Latn_RS" (Note this will not reverse.)
 *
 *			"zh_Hani" maximizes to "zh_Hans_CN" (Note this will not reverse.)
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id						The locale to maximize
 * @param[out] maximized_locale_id			The maximized locale
 * @param[in] maximized_locale_id_capacity	The capacity of the @a maximized_locale_id buffer
 *
 * @return The actual buffer size needed for the maximized locale. If it's
 *		   greater than @a maximized_lacale_id_capacity, the returned ID will be truncated.
 *		   On error, the return value is -1.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_add_likely_subtags(const char *locale_id, char *maximized_locale_id, int32_t maximized_locale_id_capacity);

/**
 * @brief Minimizes the subtags for a provided locale ID, per the algorithm described
 *		  in the following CLDR technical report: http://www.unicode.org/reports/tr35/#Likely_Subtags
 * @details If @a locale_id is already in the minimal form, or there is no data available
 *			for minimization, it will be copied to the output buffer.  Since the
 *			minimization algorithm relies on proper maximization, see the comments
 *			for i18n_ulocale_add_likely_subtags() for reasons why there might not be any data.
 *
 *			Examples :
 *
 *			"en_Latn_US" minimizes to "en"
 *
 *			"de_Latn_US" minimizes to "de"
 *
 *			"sr_Cyrl_RS" minimizes to "sr"
 *
 *			"zh_Hant_TW" minimizes to "zh_TW" (The region is preferred to the script, and minimizing to "zh" would imply "zh_Hans_CN".)
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id						The locale to minimize
 * @param[out] minimized_locale_id			The minimized locale
 * @param[in] minimized_locale_id_capacity	The capacity of the @a minimized_locale_id buffer
 *
 * @return The actual buffer size needed for the minimized locale. If it's
 *		   greater than @a minimized_locale_id_capacity, the returned ID will be truncated.
 *		   On error, the return value is -1.
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_minimize_subtags(const char *locale_id, char *minimized_locale_id, int32_t minimized_locale_id_capacity);

/**
 * @brief Returns a locale ID for the specified BCP47 language tag string.
 * @details If the specified language tag contains any ill-formed subtags,
 *			the first such subtag and all following subtags are ignored.<br>
 *
 *			This implements the 'Language-Tag' production of BCP47, and so
 *			supports grandfathered (regular and irregular) as well as private
 *			use language tags. Private use tags are represented as 'x-whatever',
 *			and grandfathered tags are converted to their canonical replacements
 *			where they exist. Note that a few grandfathered tags have no modern
 *			replacement, these will be converted using the fallback described in
 *			the first paragraph, so some information might be lost.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] langtag			  The input BCP47 language tag.
 * @param[out] locale_id		  The output buffer receiving a locale ID for the
 *								  specified BCP47 language tag.
 * @param[in] locale_id_capacity  The size of the @a locale_id output buffer.
 * @param[in] parsed_length		  If not @c NULL, successfully parsed length
 *								  for the input language tag is set.
 *
 * @return The length of the locale ID. If the @a langtag or @a locale_id buffer is @c NULL or the @a locale_id_capacity is lower than @c 0,
 *		   then the #I18N_ERROR_INVALID_PARAMETER error will be set and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_for_language_tag(const char *langtag, char *locale_id, int32_t locale_id_capacity, int32_t *parsed_length);

/**
 * @brief Returns a well-formed language tag for this locale ID.
 * @details Note : When @a strict is @c false, any locale
 *			fields which do not satisfy the BCP47 syntax requirement will
 *			be omitted from the result.	 When @a strict is
 *			@c true, this function sets #I18N_ERROR_INVALID_PARAMETER to the
 *			result if any locale fields do not satisfy the
 *			BCP47 syntax requirement.
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *			Error codes are described in Exceptions section and #i18n_error_code_e description.
 * @since_tizen 2.3.1
 *
 * @param[in] locale_id			The input locale ID
 * @param[out] langtag			The output buffer receiving BCP47 language
 *								tag for the locale ID.
 * @param[in] langtag_capacity	The size of the BCP47 language tag
 *								output buffer.
 * @param[in] strict			Boolean value indicating if the function returns
 *								an error for an ill-formed input locale ID.
 *
 * @return The length of the BCP47 language tag. If the @a locale_id  or @a langtag buffer is @c NULL or the @a langtag_capacity is lower than @c 0,
 *		   then the #I18N_ERROR_INVALID_PARAMETER error will be set and @c -1 will be returned.
 *
 * @exception #I18N_ERROR_NONE Successful
 * @exception #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int32_t i18n_ulocale_to_language_tag(const char *locale_id, char *langtag, int32_t langtag_capacity, i18n_ubool strict);

/**
 * @brief Gets the bogus state.
 * @details A locale string can be bogus if it does not identify an existing locale.
 * @since_tizen 4.0
 *
 * @param[in] locale_id			The input locale ID
 * @param[out] is_bogus			Boolean value indicating if the given locale is bogus.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ulocale_is_bogus(const char *locale_id, i18n_ubool *is_bogus);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
