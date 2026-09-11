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

#ifndef __UTILS_I18N_H__
#define __UTILS_I18N_H__

#include <utils_i18n_ucalendar.h>
#include <utils_i18n_udate.h>
#include <utils_i18n_udatepg.h>
#include <utils_i18n_ulocale.h>
#include <utils_i18n_unumber.h>
#include <utils_i18n_uchar.h>
#include <utils_i18n_ucollator.h>
#include <utils_i18n_unormalization.h>
#include <utils_i18n_usearch.h>
#include <utils_i18n_ustring.h>
#include <utils_i18n_timezone.h>
#include <utils_i18n_types.h>
#include <utils_i18n_uenumeration.h>
#include <utils_i18n_uset.h>
#include <utils_i18n_ubrk.h>
#include <utils_i18n_alpha_idx.h>
#include <utils_i18n_formattable.h>
#include <utils_i18n_measure_unit.h>
#include <utils_i18n_measure.h>
#include <utils_i18n_format.h>
#include <utils_i18n_measure_format.h>
#include <utils_i18n_field_position.h>
#include <utils_i18n_parse_position.h>
#include <utils_i18n_ushape.h>
#include <utils_i18n_utmscale.h>
#include <utils_i18n_ubidi.h>
#include <utils_i18n_uversion.h>
#include <utils_i18n_uchar_iter.h>
#include <utils_i18n_plural_rules.h>
#include <utils_i18n_plural_format.h>
#include <utils_i18n_immutable_idx.h>
#include <utils_i18n_date_interval.h>
#include <utils_i18n_date_interval_format.h>
#include <utils_i18n_simple_date_format.h>
#include <utils_i18n_loc_disp_names.h>
#include <utils_i18n_unumsys.h>
#include <utils_i18n_utext.h>
#include <utils_i18n_uscript.h>
#include <utils_i18n_uidna.h>
#include <utils_i18n_ucnv.h>
#include <utils_i18n_ucnvsel.h>
#include <utils_i18n_ucsdet.h>
#include <utils_i18n_ures.h>

/**
 * @file utils_i18n.h
 * @version 0.1
 * @brief utils_i18n
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_MODULE i18n
 * @brief The i18n module contains:
 *       - uchar
 *       - ucollator
 *       - unormalization
 *       - usearch
 *       - ustring
 *       - ucalendar
 *       - udate
 *       - udatepg
 *       - ulocale
 *       - unumber
 *       - alpha_idx
 *       - formattable
 *       - measure unit
 *       - measure
 *       - format
 *       - measure format
 *       - field position
 *       - parse position
 *       - ushape
 *       - utmscale
 *       - ubidi
 *       - uversion
 *       - uchar iter
 *       - plural rules
 *       - plural format
 *       - immutable_idx
 *       - date interval
 *       - date interval format
 *       - simple date format
 *       - locale display names
 *       - numbering system
 *       - utext
 *       - unicode Script Information
 *       - IDNA
 *       - Character conversion
 *       - Converter selector
 *       - Charset Detection
 *       - resource bundle
 *
 *	   This module provides flexible generation of number or date format patterns and helps you format and parse dates/number for any locale.
 * The i18n module provides various features based on data from ICU. The following table shows the version of ICU used in each Tizen platform.
 * <table>
 *	<tr>
 *	 <td>Tizen 2.3, 2.3.1, 2.4</td>
 *	 <td>ICU 51</td>
 *	 <td>CLDR 23</td>
 *	 <td>Unicode 6.2</td>
 *	</tr>
 *	<tr>
 *	 <td>Tizen 2.3.2, 3.0</td>
 *	 <td>ICU 57</td>
 *	 <td>CLDR 29</td>
 *	 <td>Unicode 8.0</td>
 *	</tr>
 * </table>
 * @section CAPI_BASE_UTILS_I18N_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 * @section CAPI_BASE_UTILS_I18N_MODULE_OVERVIEW Overview
 * <table>
 * <tr>
 *	  <th>API</th>
 *	  <th>Description</th>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>The Timezone module represents a time zone offset, and also figures out daylight savings.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>UEnumeration defines functions for handling String Enumeration.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>Ubrk module defines methods for finding the location of boundaries in text.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>Ucollator module performs locale-sensitive string comparison. It builds searching and sorting routines for natural language text and provides correct sorting orders for most locales.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>Uchar module provides low-level access to the Unicode Character Database.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *	  <td>Unormalization module provides Unicode normalization functionality for standard unicode normalization.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USEARCH_MODULE</td>
 *	  <td>Usearch module provides language-sensitive text searching based on the comparison rules defined in a ucollator data struct.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>Uset module allows to specify a subset of character used in strings.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>Ustring module provides general unicode string handling.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>Ucalendar is used for converting between a i18n_udate type and a set of integer fields
		 such as #I18N_UCALENDAR_YEAR, #I18N_UCALENDAR_MONTH, #I18N_UCALENDAR_DATE, #I18N_UCALENDAR_HOUR, and so on.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>Udate module consists of functions that convert dates and times from their
			 internal representations to textual form and back again in a language-independent manner.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td> Udatepg module provides flexible generation of date format patterns, like "yy-MM-dd". </td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>A ulocale represents a specific geographical, political, or cultural region. </td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>Unumber helps you format and parse numbers for any locale.</td>
 * </tr>
 *	<tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>Alpha_idx supports the creation of a UI index appropriate for a given language.</td>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>Formattable interconverts between the primitive numeric types (double, long, etc.) as well as the #i18n_udate and char string.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>The MeasureUnit module contains units such as length, mass, volume, currency, etc...</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_MODULE</td>
 *	  <td>The Measure module represents an amount of a specified unit, consisting of a number and a unit.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMAT_MODULE</td>
 *	  <td>The Format module represents the basic format object.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>The Measure Format module represents a formatter for measure objects.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>The Field Position module is used to identify fields in a formatted output.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>The Parse Position module used by Format and its subclasses to keep track of the current position during parsing.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USHAPE_MODULE</td>
 *	  <td>Ushape module provides Arabic shaping functionality.</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE</td>
 *	  <td>The Universal Time Scale</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>Ubidi module provides implementation of the Unicode Bidirectional Algorithm.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UVERSION_MODULE</td>
 *     <td>Uversion module provides API for accessing ICU version numbers.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *     <td>The Uchar Iterator module provides API for code unit iteration.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *     <td>The Plural rules module Defines rules for mapping non-negative numeric values onto a small set of keywords.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *     <td>The Plural format module supports the creation of internationalized messages with plural inflection.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE</td>
 *     <td>The Immutable_idx is immutable, thread-safe version of Alpha_idx.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE</td>
 *     <td>Date interval represents a date interval.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *     <td>Date interval format module formats date intervals in a language-independent manner.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *     <td>The Simple Date Format module provides API for formatting and parsing dates in language-independent manner.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *     <td>The locale display names module returns display names of Locales and components of Locales.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *     <td>Numbering System</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *     <td>Abstract Unicode Text API.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *     <td>Unicode Script Information</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *     <td>Internationalizing Domain Names in Applications (IDNA)</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *     <td>Character conversion</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE</td>
 *     <td>A converter selector is built with a set of encoding/charset names and given an input string returns the set of names of the corresponding converters which can convert the string.</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *     <td>Detecting the charset or encoding of character data in an unknown text format</td>
 * </tr>
 * <tr>
 *     <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *     <td>Resource Bundle.</td>
 * </tr>
 * </table>
 *
 * @section CAPI_BASE_UTILS_I18N_MODULE_MAPPING_TABLE Mapping Table
 * <table>
 * <tr>
 *	  <th>Module</th>
 *	  <th>Native API</th>
 *	  <th>ICU API</th>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_create_unknown</td>
 *	  <td>getUnknown</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_create_gmt</td>
 *	  <td>getGMT</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_create</td>
 *	  <td>createTimeZone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_destroy</td>
 *	  <td></td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_foreach_timezone_id_by_region</td>
 *	  <td>createTimeZoneIDEnumeration</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_foreach_timezone_id</td>
 *	  <td>createEnumeration</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_foreach_timezone_id_with_offset</td>
 *	  <td>createEnumeration</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_foreach_timezone_id_by_country</td>
 *	  <td>createEnumeration</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_count_equivalent_ids</td>
 *	  <td>countEquivalentIDs</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_equivalent_id</td>
 *	  <td>getEquivalentID</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_create_default</td>
 *	  <td>createDefault</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_set_default</td>
 *	  <td>setDefault</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_tzdata_version</td>
 *	  <td>getTZDataVersion</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_region</td>
 *	  <td>getRegion</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_offset_with_date</td>
 *	  <td>getOffset</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_set_raw_offset</td>
 *	  <td>setRawOffset</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_raw_offset</td>
 *	  <td>getRawOffset</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_id</td>
 *	  <td>getID</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_set_id</td>
 *	  <td>setID</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_display_name</td>
 *	  <td>getDisplayName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_display_name_with_type</td>
 *	  <td>getDisplayName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_use_daylight_time</td>
 *	  <td>useDaylightTime</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_has_same_rule</td>
 *	  <td>hasSameRules</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_clone</td>
 *	  <td>clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_dst_savings</td>
 *	  <td>getDSTSavings</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_detect_host_timezone</td>
 *	  <td>detectHostTimeZone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_display_name_with_locale_id</td>
 *	  <td>getDisplayName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_TIMEZONE_MODULE</td>
 *	  <td>#i18n_timezone_get_display_name_with_type_locale_id</td>
 *	  <td>getDisplayName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>#i18n_uenumeration_destroy</td>
 *	  <td>uenum_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>#i18n_uenumeration_count</td>
 *	  <td>uenum_count</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>#i18n_uenumeration_unext</td>
 *	  <td>uenum_unext</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>#i18n_uenumeration_next</td>
 *	  <td>uenum_next</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>#i18n_uenumeration_reset</td>
 *	  <td>uenum_reset</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>#i18n_uenumeration_uchar_strings_enumeration_create</td>
 *	  <td>uenum_openUCharStringsEnumeration</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UENUMERATION_MODULE</td>
 *	  <td>#i18n_uenumeration_char_strings_enumeration_create</td>
 *	  <td>uenum_openCharStringsEnumeration</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_create</td>
 *	  <td>ubrk_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_create_rules</td>
 *	  <td>ubrk_openRules</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_safe_clone</td>
 *	  <td>ubrk_safeClone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_destroy</td>
 *	  <td>ubrk_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_set_text</td>
 *	  <td>ubrk_setText</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_current</td>
 *	  <td>ubrk_current</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_next</td>
 *	  <td>ubrk_next</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_preceding</td>
 *	  <td>ubrk_preceding</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_previous</td>
 *	  <td>ubrk_previous</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_first</td>
 *	  <td>ubrk_first</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_last</td>
 *	  <td>ubrk_last</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_following</td>
 *	  <td>ubrk_following</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_get_available</td>
 *	  <td>ubrk_getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_count_available</td>
 *	  <td>ubrk_countAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_is_boundary</td>
 *	  <td>ubrk_isBoundary</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_get_rule_status</td>
 *	  <td>ubrk_getRuleStatus</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_get_rule_status_vec</td>
 *	  <td>ubrk_getRuleStatusVec</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBRK_MODULE</td>
 *	  <td>#i18n_ubrk_get_locale_by_type</td>
 *	  <td>ubrk_getLocaleByType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set_default_timezone</td>
 *	  <td>ucal_setDefaultTimeZone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_now</td>
 *	  <td>ucal_getNow</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_create</td>
 *	  <td>ucal_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_destroy</td>
 *	  <td>ucal_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_clone</td>
 *	  <td>ucal_clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_timezone_displayname</td>
 *	  <td>ucal_getTimeZoneDisplayName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_is_in_daylight_time</td>
 *	  <td>ucal_inDaylightTime</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set</td>
 *	  <td>ucal_set</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set_attribute</td>
 *	  <td>ucal_setAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_attribute</td>
 *	  <td>ucal_getAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_milliseconds</td>
 *	  <td>ucal_getMillis</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set_milliseconds</td>
 *	  <td>ucal_setMillis</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set_date_time</td>
 *	  <td>ucal_setDateTime</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_is_equivalent_to</td>
 *	  <td>ucal_equivalentTo</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_add</td>
 *	  <td>ucal_add</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get</td>
 *	  <td>ucal_get</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_clear</td>
 *	  <td>ucal_clear</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_clear_field</td>
 *	  <td>ucal_clearField</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_count_available</td>
 *	  <td>ucal_countAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_country_timezones_create</td>
 *	  <td>ucal_openCountryTimeZones</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_available</td>
 *	  <td>ucal_getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_canonical_timezone_id</td>
 *	  <td>ucal_getCanonicalTimeZoneID</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_day_of_week_type</td>
 *	  <td>ucal_getDayOfWeekType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_default_timezone</td>
 *	  <td>ucal_getDefaultTimeZone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_field_difference</td>
 *	  <td>ucal_getFieldDifference</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_gregorian_change</td>
 *	  <td>ucal_getGregorianChange</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_keyword_values_for_locale</td>
 *	  <td>ucal_getKeywordValuesForLocale</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_limit</td>
 *	  <td>ucal_getLimit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_locale_by_type</td>
 *	  <td>ucal_getLocaleByType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_timezone_id</td>
 *	  <td>ucal_getTimeZoneID</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_timezone_transition_date</td>
 *	  <td>ucal_getTimeZoneTransitionDate</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_type</td>
 *	  <td>ucal_getType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_tz_data_version</td>
 *	  <td>ucal_getTZDataVersion</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_get_weekend_transition</td>
 *	  <td>ucal_getWeekendTransition</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_is_set</td>
 *	  <td>ucal_isSet</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_is_weekend</td>
 *	  <td>ucal_isWeekend</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_roll</td>
 *	  <td>ucal_roll</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set_date</td>
 *	  <td>ucal_setDate</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set_gregorian_change</td>
 *	  <td>ucal_setGregorianChange</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_set_timezone</td>
 *	  <td>ucal_setTimeZone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_timezones_create</td>
 *	  <td>ucal_openTimeZones</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCALENDAR_MODULE</td>
 *	  <td>#i18n_ucalendar_timezone_id_enumeration_create</td>
 *	  <td>ucal_openTimeZoneIDEnumeration</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_int_property_value</td>
 *	  <td>u_getIntpropertyValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_ublock_code</td>
 *	  <td>ublock_getCode</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_has_binary_property</td>
 *	  <td>hasBinaryProperty</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_alphabetic</td>
 *	  <td>u_isUAlphabetic</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_lowercase</td>
 *	  <td>u_isULowercase</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_uppercase</td>
 *	  <td>u_isUUppercase</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_white_space</td>
 *	  <td>u_isUWhiteSpace</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_int_property_min_value</td>
 *	  <td>u_getIntPropertyMinValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_int_property_max_value</td>
 *	  <td>u_getIntPropertyMaxValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_numeric_value</td>
 *	  <td>u_getNumericValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_lower</td>
 *	  <td>u_islower</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_upper</td>
 *	  <td>u_isupper</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_title</td>
 *	  <td>u_istitle</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_digit</td>
 *	  <td>u_isdigit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_alpha</td>
 *	  <td>u_isalpha</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_alnum</td>
 *	  <td>u_isalnum</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_xdigit</td>
 *	  <td>u_isxdigit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_punct</td>
 *	  <td>u_ispunct</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_graph</td>
 *	  <td>u_isgraph</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_blank</td>
 *	  <td>u_isblank</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_defined</td>
 *	  <td>u_isdefined</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_space</td>
 *	  <td>u_isspace</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_java_space_char</td>
 *	  <td>u_isJavaSpaceChar</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_whitespace</td>
 *	  <td>u_isWhitespace</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_control</td>
 *	  <td>u_iscntrl</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_iso_control</td>
 *	  <td>u_isISOControl</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_printable</td>
 *	  <td>u_isprint</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_base</td>
 *	  <td>u_isbase</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_char_direction</td>
 *	  <td>u_charDirection</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_mirrored</td>
 *	  <td>u_isMirrored</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_char_mirror</td>
 *	  <td>u_charMirror</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_bidi_paired_bracket</td>
 *	  <td>bidi_paired_bracket</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_char_type</td>
 *	  <td>u_charType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_enum_char_types</td>
 *	  <td>u_enumCharTypes</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_combining_class</td>
 *	  <td>u_getCombiningClass</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_char_digit_value</td>
 *	  <td>u_charDigitValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_char_name</td>
 *	  <td>u_charName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_char_from_name</td>
 *	  <td>u_charFromName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_enum_char_names</td>
 *	  <td>u_enumCharNames</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_property_name</td>
 *	  <td>u_getPropertyName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_property_enum</td>
 *	  <td>u_getPropertyEnum</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_property_value_name</td>
 *	  <td>u_getPropertyValueName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_property_value_enum</td>
 *	  <td>u_getPropertyValueEnum</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_id_start</td>
 *	  <td>u_isIDStart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_id_part</td>
 *	  <td>u_isIDPart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_id_ignorable</td>
 *	  <td>u_isIDIgnorable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_java_id_start</td>
 *	  <td>u_isJavaIDStart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_is_java_id_part</td>
 *	  <td>u_isJavaIDPart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_to_lower</td>
 *	  <td>u_tolower</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_to_upper</td>
 *	  <td>u_toupper</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_to_title</td>
 *	  <td>u_totitle</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_fold_case</td>
 *	  <td>u_foldCase</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_digit</td>
 *	  <td>u_digit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_for_digit</td>
 *	  <td>u_forDigit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_char_age</td>
 *	  <td>u_charAge</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_unicode_version</td>
 *	  <td>u_digit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_MODULE</td>
 *	  <td>#i18n_uchar_get_fc_nfkc_closure</td>
 *	  <td>u_getFC_NFKC_Closure</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_create</td>
 *	  <td>ucol_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_destroy</td>
 *	  <td>ucol_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_str_collator</td>
 *	  <td>ucol_strcoll</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_equal</td>
 *	  <td>ucol_equal</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_set_strength</td>
 *	  <td>ucol_setStrength</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_set_attribute</td>
 *	  <td>ucol_setAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_create_rules</td>
 *	  <td>ucol_openRules</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_contractions_and_expansions</td>
 *	  <td>ucol_getContractionsAndExpansions</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_str_collator_utf8</td>
 *	  <td>ucol_strcollUTF8</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_greater</td>
 *	  <td>ucol_greater</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_greater_or_equal</td>
 *	  <td>ucol_greaterOrEqual</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_str_collator_iter</td>
 *	  <td>ucol_strcollIter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_strength</td>
 *	  <td>ucol_getStrength</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_reorder_codes</td>
 *	  <td>ucol_getReorderCodes</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_set_reorder_codes</td>
 *	  <td>ucol_setReorderCodes</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_equivalent_reorder_codes</td>
 *	  <td>ucol_getEquivalentReorderCodes</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_display_name</td>
 *	  <td>ucol_getDisplayName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_available</td>
 *	  <td>ucol_getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_count_available</td>
 *	  <td>ucol_countAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_create_available_locales</td>
 *	  <td>ucol_openAvailableLocales</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_keywords</td>
 *	  <td>ucol_getKeywords</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_keyword_values</td>
 *	  <td>ucol_getKeywordValues</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_keyword_values_for_locale</td>
 *	  <td>ucol_getKeywordValuesForLocale</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_functional_equivalent</td>
 *	  <td>ucol_getFunctionalEquivalent</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_rules</td>
 *	  <td>ucol_getRules</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_sort_key</td>
 *	  <td>ucol_getSortKey</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_next_sort_key_part</td>
 *	  <td>ucol_nextSortKeyPart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_bound</td>
 *	  <td>ucol_getBound</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_version</td>
 *	  <td>ucol_getVersion</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_uca_version</td>
 *	  <td>ucol_getUCAVersion</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_merge_sort_keys</td>
 *	  <td>ucol_mergeSortkeys</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_attribute</td>
 *	  <td>ucol_getAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_set_max_variable</td>
 *	  <td>ucol_setMaxVariable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_max_variable</td>
 *	  <td>ucol_getMaxVariable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_variable_top</td>
 *	  <td>ucol_getVariableTop</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_safe_clone</td>
 *	  <td>ucol_safeClone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_rules_ex</td>
 *	  <td>ucol_getRulesEx</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_locale_by_type</td>
 *	  <td>ucol_getLocaleByType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_get_tailored_set</td>
 *	  <td>ucol_getTailoredSet</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_clone_binary</td>
 *	  <td>ucol_cloneBinary</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCOLLATOR_MODULE</td>
 *	  <td>#i18n_ucollator_create_binary</td>
 *	  <td>ucol_openBinary</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_create</td>
 *	  <td>udat_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_destroy</td>
 *	  <td>udat_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_format_date</td>
 *	  <td>udat_format</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_apply_pattern</td>
 *	  <td>udat_applyPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_clone</td>
 *	  <td>udat_clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_count_available</td>
 *	  <td>udat_countAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_count_symbols</td>
 *	  <td>udat_countSymbols</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_get_2digit_year_start</td>
 *	  <td>udat_get2DigitYearStart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_get_available</td>
 *	  <td>udat_getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_get_calendar</td>
 *	  <td>udat_getCalendar</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_get_locale_by_type</td>
 *	  <td>udat_getLocaleByType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_get_number_format</td>
 *	  <td>udat_getNumberFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_get_symbols</td>
 *	  <td>udat_getSymbols</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_is_lenient</td>
 *	  <td>udat_isLenient</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_parse</td>
 *	  <td>udat_parse</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_parse_calendar</td>
 *	  <td>udat_parseCalendar</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_set_2digit_year_start</td>
 *	  <td>udat_set2DigitYearStart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_set_calendar</td>
 *	  <td>udat_setCalendar</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_set_context</td>
 *	  <td>udat_setContext</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_set_lenient</td>
 *	  <td>udat_setLenient</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_set_number_format</td>
 *	  <td>udat_setNumberFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_set_symbols</td>
 *	  <td>udat_setSymbols</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_to_calendar_date_field</td>
 *	  <td>udat_toCalendarDateField</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATE_MODULE</td>
 *	  <td>#i18n_udate_to_pattern</td>
 *	  <td>udat_toPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_create</td>
 *	  <td>udatpg_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_destroy</td>
 *	  <td>udatpg_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_best_pattern</td>
 *	  <td>udatpg_getBestPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_add_pattern</td>
 *	  <td>udatpg_addPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_base_skeletons_create</td>
 *	  <td>udatpg_openBaseSkeletons</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_clone</td>
 *	  <td>udatpg_clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_create_empty</td>
 *	  <td>udatpg_openEmpty</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_append_item_format</td>
 *	  <td>udatpg_getAppendItemFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_append_item_name</td>
 *	  <td>udatpg_getAppendItemName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_base_skeleton</td>
 *	  <td>udatpg_getBaseSkeleton</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_best_pattern_with_options</td>
 *	  <td>udatpg_getBestPatternWithOptions</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_date_time_format</td>
 *	  <td>udatpg_getDateTimeFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_decimal</td>
 *	  <td>udatpg_getDecimal</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_pattern_for_skeleton</td>
 *	  <td>udatpg_getPatternForSkeleton</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_get_skeleton</td>
 *	  <td>udatpg_getSkeleton</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_replace_field_types</td>
 *	  <td>udatpg_replaceFieldTypes</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_replace_field_types_with_options</td>
 *	  <td>udatpg_replaceFieldTypesWithOptions</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_set_append_item_format</td>
 *	  <td>udatpg_setAppendItemFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_set_append_item_name</td>
 *	  <td>udatpg_setAppendItemName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_set_date_time_format</td>
 *	  <td>udatpg_setDateTimeFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_set_decimal</td>
 *	  <td>udatpg_setDecimal</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UDATEPG_MODULE</td>
 *	  <td>#i18n_udatepg_skeletons_create</td>
 *	  <td>udatpg_openSkeletons</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_default</td>
 *	  <td>uloc_getDefault</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_set_default</td>
 *	  <td>uloc_setDefault</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_language</td>
 *	  <td>uloc_getLanguage</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_country</td>
 *	  <td>uloc_getCountry</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_display_name</td>
 *	  <td>uloc_getDisplayName</td>
 * </tr>
 *
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_add_likely_subtags</td>
 *	  <td>uloc_addLikelySubtags</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_canonicalize</td>
 *	  <td>uloc_canonicalize</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_for_language_tag</td>
 *	  <td>uloc_forLanguageTag</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_base_name</td>
 *	  <td>uloc_getBaseName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_character_orientation</td>
 *	  <td>uloc_getCharacterOrientation</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_display_country</td>
 *	  <td>uloc_getDisplayCountry</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_display_keyword</td>
 *	  <td>uloc_getDisplayKeyword</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_display_keyword_value</td>
 *	  <td>uloc_getDisplayKeywordValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_display_language</td>
 *	  <td>uloc_getDisplayLanguage</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_display_script</td>
 *	  <td>uloc_getDisplayScript</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_display_variant</td>
 *	  <td>uloc_getDisplayVariant</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_iso3_country</td>
 *	  <td>uloc_getISO3Country</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_iso3_language</td>
 *	  <td>uloc_getISO3Language</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_iso_countries</td>
 *	  <td>uloc_getISOCountries</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_iso_languages</td>
 *	  <td>uloc_getISOLanguages</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_keyword_value</td>
 *	  <td>uloc_getKeywordValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_lcid</td>
 *	  <td>uloc_getLCID</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_line_orientation</td>
 *	  <td>uloc_getLineOrientation</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_locale_for_lcid</td>
 *	  <td>uloc_getLocaleForLCID</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_name</td>
 *	  <td>uloc_getName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_parent</td>
 *	  <td>uloc_getParent</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_script</td>
 *	  <td>uloc_getScript</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_variant</td>
 *	  <td>uloc_getVariant</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_keywords_create</td>
 *	  <td>uloc_openKeywords</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_minimize_subtags</td>
 *	  <td>uloc_minimizeSubtags</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_set_keyword_value</td>
 *	  <td>uloc_setKeywordValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_to_language_tag</td>
 *	  <td>uloc_toLanguageTag</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_is_bogus</td>
 *	  <td>isBogus</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_get_available</td>
 *	  <td>uloc_getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ULOCALE_MODULE</td>
 *	  <td>#i18n_ulocale_count_available</td>
 *	  <td>uloc_countAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *	  <td>#i18n_unormalization_get_instance</td>
 *	  <td>unorm2_getInstance</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *	  <td>#i18n_unormalization_normalize</td>
 *	  <td>unorm2_normalize</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_nfc_instance</td>
 *    <td>unorm2_getNFCInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_nfd_instance</td>
 *    <td>unorm2_getNFDInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_nfkc_instance</td>
 *    <td>unorm2_getNFKCInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_nfkd_instance</td>
 *    <td>unorm2_getNFKDInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_nfkc_casefold_instance</td>
 *    <td>unorm2_getNFKCCasefoldInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_create_filtered</td>
 *    <td>unorm2_openFiltered</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_destroy</td>
 *    <td>unorm2_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_normalize_second_and_append</td>
 *    <td>unorm2_normalizeSecondAndAppend</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_append</td>
 *    <td>unorm2_append</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_decomposition</td>
 *    <td>unorm2_getDecomposition</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_raw_decomposition</td>
 *    <td>unorm2_getRawDecomposition</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_compose_pair</td>
 *    <td>unorm2_composePair</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_get_combining_class</td>
 *    <td>unorm2_getCombiningClass</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_is_normalized</td>
 *    <td>unorm2_isNormalized</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_quick_check</td>
 *    <td>unorm2_quickCheck</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_span_quick_check_yes</td>
 *    <td>unorm2_spanQuickCheckYes</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalizer_has_boundary_before</td>
 *    <td>unorm2_hasBoundaryBefore</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalizer_has_boundary_after</td>
 *    <td>unorm2_hasBoundaryAfter</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNORMALIZATION_MODULE</td>
 *    <td>#i18n_unormalization_is_inert</td>
 *    <td>unorm2_isInert</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_create</td>
 *	  <td>unum_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_destroy</td>
 *	  <td>unum_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_get_symbol</td>
 *	  <td>unum_getSymbol</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_apply_pattern</td>
 *	  <td>unum_applyPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_clone</td>
 *	  <td>unum_clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_count_available</td>
 *	  <td>unum_countAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_format</td>
 *	  <td>unum_format</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_format_decimal</td>
 *	  <td>unum_formatDecimal</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_format_double</td>
 *	  <td>unum_formatDouble</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_format_double_currency</td>
 *	  <td>unum_formatDoubleCurrency</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_format_int64</td>
 *	  <td>unum_formatInt64</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_get_attribute</td>
 *	  <td>unum_getAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_get_available</td>
 *	  <td>unum_getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_get_double_attribute</td>
 *	  <td>unum_getDoubleAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_get_locale_by_type</td>
 *	  <td>unum_getLocaleByType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_get_text_attribute</td>
 *	  <td>unum_getTextAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_parse</td>
 *	  <td>unum_parse</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_parse_decimal</td>
 *	  <td>unum_parseDecimal</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_parse_double</td>
 *	  <td>unum_parseDouble</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_parse_double_currency</td>
 *	  <td>unum_parseDoubleCurrency</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_parse_int64</td>
 *	  <td>unum_parseInt64</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_set_attribute</td>
 *	  <td>unum_setAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_set_double_attribute</td>
 *	  <td>unum_setDoubleAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_set_symbol</td>
 *	  <td>unum_setSymbol</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_set_text_attribute</td>
 *	  <td>unum_setTextAttribute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UNUMBER_MODULE</td>
 *	  <td>#i18n_unumber_to_pattern</td>
 *	  <td>unum_toPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USEARCH_MODULE</td>
 *	  <td>#i18n_usearch_create_new</td>
 *	  <td>usearch_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USEARCH_MODULE</td>
 *	  <td>#i18n_usearch_destroy</td>
 *	  <td>usearch_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USEARCH_MODULE</td>
 *	  <td>#i18n_usearch_get_matched_text</td>
 *	  <td>usearch_getMatchedText</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USEARCH_MODULE</td>
 *	  <td>#i18n_usearch_get_collator</td>
 *	  <td>usearch_getCollator</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USEARCH_MODULE</td>
 *	  <td>#i18n_usearch_first</td>
 *	  <td>usearch_first</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_create_empty</td>
 *	  <td>uset_openEmpty</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_create</td>
 *	  <td>uset_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_create_pattern</td>
 *	  <td>uset_openPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_create_pattern_options</td>
 *	  <td>uset_openPatternOptions</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_destroy</td>
 *	  <td>uset_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_clone</td>
 *	  <td>uset_clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_is_frozen</td>
 *	  <td>uset_isFrozen</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_freeze</td>
 *	  <td>uset_freeze</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_clone_as_thawed</td>
 *	  <td>uset_cloneAsThawed</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_set</td>
 *	  <td>uset_set</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_apply_pattern</td>
 *	  <td>uset_applyPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_apply_int_property_value</td>
 *	  <td>uset_applyIntPropertyValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_apply_property_alias</td>
 *	  <td>uset_applyPropertyAlias</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_resembles_pattern</td>
 *	  <td>uset_resemblesPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_to_pattern</td>
 *	  <td>uset_toPattern</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_add</td>
 *	  <td>uset_add</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_add_all</td>
 *	  <td>uset_addAll</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_add_range</td>
 *	  <td>uset_addRange</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_add_string</td>
 *	  <td>uset_addString</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_add_all_code_points</td>
 *	  <td>uset_addAllCodePoints</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_remove</td>
 *	  <td>uset_remove</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_remove_range</td>
 *	  <td>uset_removeRange</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_remove_string</td>
 *	  <td>uset_removeString</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_remove_all</td>
 *	  <td>uset_removeAll</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_retain</td>
 *	  <td>uset_retain</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_retain_all</td>
 *	  <td>uset_retainAll</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_compact</td>
 *	  <td>uset_compact</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_complement</td>
 *	  <td>uset_complement</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_complement_all</td>
 *	  <td>uset_complementAll</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_clear</td>
 *	  <td>uset_clear</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_destroy_over</td>
 *	  <td>uset_closeOver</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_remove_all_strings</td>
 *	  <td>uset_removeAllStrings</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_is_empty</td>
 *	  <td>uset_isEmpty</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_contains</td>
 *	  <td>uset_contains</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_contains_range</td>
 *	  <td>uset_containsRange</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_contains_string</td>
 *	  <td>uset_containsString</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_index_of</td>
 *	  <td>uset_indexOf</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_char_at</td>
 *	  <td>uset_charAt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_size</td>
 *	  <td>uset_size</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_get_item_count</td>
 *	  <td>uset_getItemCount</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_get_item</td>
 *	  <td>uset_getItem</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_contains_all</td>
 *	  <td>uset_containsAll</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_contains_all_code_points</td>
 *	  <td>uset_containsAllCodePoints</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_contains_none</td>
 *	  <td>uset_containsNone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_contains_some</td>
 *	  <td>uset_containsSome</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_span</td>
 *	  <td>uset_span</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_span_back</td>
 *	  <td>uset_spanBack</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_span_utf8</td>
 *	  <td>uset_spanUTF8</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_span_back_utf8</td>
 *	  <td>uset_spanBackUTF8</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_equals</td>
 *	  <td>uset_equals</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_serialize</td>
 *	  <td>uset_serialize</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_get_serialized_set</td>
 *	  <td>uset_getSerializedSet</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_set_serialized_to_one</td>
 *	  <td>uset_setSerializedToOne</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_serialized_contains</td>
 *	  <td>uset_serializedContains</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_get_serialized_range_count</td>
 *	  <td>uset_getSerializedRangeCount</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USET_MODULE</td>
 *	  <td>#i18n_uset_get_serialized_range</td>
 *	  <td>uset_getSerializedRange</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_get_length</td>
 *	  <td>u_strlen</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_count_char32</td>
 *	  <td>u_countChar32</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_has_more_char32_than</td>
 *	  <td>u_strHasMoreChar32Than</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_cat</td>
 *	  <td>u_strcat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_cat_n</td>
 *	  <td>u_strncat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_string</td>
 *	  <td>u_strstr</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_find_first</td>
 *	  <td>u_strFindFirst</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_char</td>
 *	  <td>u_strchr</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_char32</td>
 *	  <td>u_strchr32</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_r_string</td>
 *	  <td>u_strrstr</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_find_last</td>
 *	  <td>u_strFindLast</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_r_char</td>
 *	  <td>u_strrchr</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_r_char32</td>
 *	  <td>u_strrchr32</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_pbrk</td>
 *	  <td>u_strpbrk</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_cspn</td>
 *	  <td>u_strcspn</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_spn</td>
 *	  <td>u_strspn</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_tokenizer_r</td>
 *	  <td>u_strtok_r</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_compare</td>
 *	  <td>u_strcmp</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_compare_code_point_order</td>
 *	  <td>u_strcmpCodePointOrder</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_compare_binary_order</td>
 *	  <td>u_strCompare</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_case_compare_with_length</td>
 *	  <td>u_strCaseCompare</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_compare_n</td>
 *	  <td>u_strncmp</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_compare_n_code_point_order</td>
 *	  <td>u_strncmpCodePointOrder</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_case_compare</td>
 *	  <td>u_strcasecmp</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_case_compare_n</td>
 *	  <td>u_strncasecmp</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_case_compare</td>
 *	  <td>u_memcasecmp</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_copy</td>
 *	  <td>u_strcpy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_copy_n</td>
 *	  <td>u_strncpy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_copy_ua</td>
 *	  <td>u_uastrcpy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_copy_ua_n</td>
 *	  <td>u_uastrncpy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_copy_au</td>
 *	  <td>u_austrcpy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_copy_au_n</td>
 *	  <td>u_austrncpy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_copy</td>
 *	  <td>u_memcpy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_move</td>
 *	  <td>u_memmove</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_set</td>
 *	  <td>u_memset</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_compare</td>
 *	  <td>u_memcmp</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_compare_code_point_order</td>
 *	  <td>u_memcmpCodePointOrder</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_char</td>
 *	  <td>u_memchr</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_char32</td>
 *	  <td>u_memchr32</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_r_char</td>
 *	  <td>u_memrchr</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_mem_r_char32</td>
 *	  <td>u_memrchr32</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_unescape</td>
 *	  <td>u_unescape</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_unescape_at</td>
 *	  <td>u_unescapeAt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_upper</td>
 *	  <td>u_strToUpper</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_lower</td>
 *	  <td>u_strToLower</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_title_new</td>
 *	  <td>u_strToTitle</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_fold_case</td>
 *	  <td>u_strFoldCase</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_WCS</td>
 *	  <td>u_strToWCS</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_from_WCS</td>
 *	  <td>u_strFromWCS</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_UTF8</td>
 *	  <td>u_strToUTF8</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_from_UTF8</td>
 *	  <td>u_strFromUTF8</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_UTF8_with_sub</td>
 *	  <td>u_strToUTF8WithSub</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_from_UTF8_with_sub</td>
 *	  <td>u_strFromUTF8WithSub</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_from_UTF8_lenient</td>
 *	  <td>u_strFromUTF8Lenient</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_UTF32</td>
 *	  <td>u_strToUTF32</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_from_UTF32</td>
 *	  <td>u_strFromUTF32</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_to_UTF32_with_sub</td>
 *	  <td>u_strToUTF32WithSub</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USTRING_MODULE</td>
 *	  <td>#i18n_ustring_from_UTF32_with_sub</td>
 *	  <td>u_strFromUTF32WithSub</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_create</td>
 *	  <td>AlphabeticIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_destroy</td>
 *	  <td>~AlphabeticIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_add_labels</td>
 *	  <td>addLabels</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_add_record</td>
 *	  <td>addRecord</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_next_bucket</td>
 *	  <td>nextBucket</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_next_record</td>
 *	  <td>nextRecord</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_bucket_record_count</td>
 *	  <td>getBucketRecordCount</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_bucket_label</td>
 *	  <td>getBucketLabel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_record_data</td>
 *	  <td>getRecordData</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_inflow_label</td>
 *	  <td>getInflowLabel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_set_inflow_label</td>
 *	  <td>setInflowLabel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_overflow_label</td>
 *	  <td>getOverflowLabel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_set_overflow_label</td>
 *	  <td>setOverflowLabel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_underflow_label</td>
 *	  <td>getUnderflowLabel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_set_underflow_label</td>
 *	  <td>setUnderflowLabel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_max_label_count</td>
 *	  <td>getMaxLabelCount</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_set_max_label_count</td>
 *	  <td>setMaxLabelCount</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_clear_records</td>
 *	  <td>clearRecords</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_bucket_count</td>
 *	  <td>getBucketCount</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_record_count</td>
 *	  <td>getRecordCount</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_bucket_index</td>
 *	  <td>getBucketIndex(const UnicodeString &itemName, UErrorCode &status)</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_current_bucket_index</td>
 *	  <td>getBucketIndex()</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_bucket_label_type</td>
 *	  <td>getBucketLabelType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_get_record_name</td>
 *	  <td>getRecordName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_reset_bucket_iter</td>
 *	  <td>resetBucketIterator</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_ALPHA_IDX_MODULE</td>
 *	  <td>#i18n_alpha_idx_reset_record_iter</td>
 *	  <td>resetRecordIterator</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_create_default</td>
 *	  <td>Formattable()</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_create_with_udate</td>
 *	  <td>Formattable(UDate d, ISDATE flag)</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_create_with_double</td>
 *	  <td>Formattable(double d)</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_create_with_long</td>
 *	  <td>Formattable(int32_t l)</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_create_with_int64</td>
 *	  <td>Formattable(int64_t ll)</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_create_with_char_string</td>
 *	  <td>Formattable(const char* strToCopy)</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_create_with_formattable_array</td>
 *	  <td>Formattable(const Formattable* arrayToCopy, int32_t count)</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_destroy</td>
 *	  <td>delete</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_clone</td>
 *	  <td>Formattable::clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_get_array</td>
 *	  <td>Formattable::getArray</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_get_date</td>
 *	  <td>Formattable::getDate</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_get_double</td>
 *	  <td>Formattable::getDouble</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_get_int64</td>
 *	  <td>Formattable::getInt64</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_get_long</td>
 *	  <td>Formattable::getLong</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_get_string</td>
 *	  <td>Formattable::getString</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_get_type</td>
 *	  <td>Formattable::getType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_is_numeric</td>
 *	  <td>Formattable::isNumeric</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_not_equal</td>
 *	  <td>Formattable::operator!=</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_equal</td>
 *	  <td>Formattable::operator==</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_element_at</td>
 *	  <td>Formattable::operator[]</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_set_array</td>
 *	  <td>Formattable::setArray</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_set_date</td>
 *	  <td>Formattable::setDate</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_set_double</td>
 *	  <td>Formattable::setDouble</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_set_int64</td>
 *	  <td>Formattable::setInt64</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_set_long</td>
 *	  <td>Formattable::setLong</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMATTABLE_MODULE</td>
 *	  <td>#i18n_formattable_set_string</td>
 *	  <td>Formattable::setString</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create</td>
 *	  <td>MeasureUnit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_clone</td>
 *	  <td>clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_destroy</td>
 *	  <td>~MeasureUnit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_get_type</td>
 *	  <td>getType</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_get_subtype</td>
 *	  <td>getSubtype</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_get_available</td>
 *	  <td>getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_get_available_with_type</td>
 *	  <td>getAvailable</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_foreach_available_type</td>
 *	  <td>getAvailableTypes</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_g_force</td>
 *	  <td>createGForce</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_meter_per_second_squared</td>
 *	  <td>createMeterPerSecondSquared</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_arc_minute</td>
 *	  <td>createArcMinute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_arc_second</td>
 *	  <td>createArcSecond</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_degree</td>
 *	  <td>createDegree</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_radian</td>
 *	  <td>createRadian</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_acre</td>
 *	  <td>createAcre</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_hectare</td>
 *	  <td>createHectare</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_square_centimeter</td>
 *	  <td>createSquareCentimeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_square_foot</td>
 *	  <td>createSquareFoot</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_square_inch</td>
 *	  <td>createSquareInch</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_square_kilometer</td>
 *	  <td>createSquareKilometer</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_square_meter</td>
 *	  <td>createSquareMeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_square_mile</td>
 *	  <td>createSquareMile</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_square_yard</td>
 *	  <td>createSquareYard</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_karat</td>
 *	  <td>createKarat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_liter_per_kilometer</td>
 *	  <td>createLiterPerKilometer</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_mile_per_gallon</td>
 *	  <td>createMilePerGallon</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_bit</td>
 *	  <td>createBit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_byte</td>
 *	  <td>createByte</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_gigabit</td>
 *	  <td>createGigabit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_gigabyte</td>
 *	  <td>createGigabyte</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilobit</td>
 *	  <td>createKilobit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilobyte</td>
 *	  <td>createKilobyte</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_megabit</td>
 *	  <td>createMegabit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_megabyte</td>
 *	  <td>createMegabyte</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_terabit</td>
 *	  <td>createTerabit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_terabyte</td>
 *	  <td>createTerabyte</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_day</td>
 *	  <td>createDay</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_hour</td>
 *	  <td>createHour</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_microsecond</td>
 *	  <td>createMicrosecond</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_millisecond</td>
 *	  <td>createMillisecond</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_minute</td>
 *	  <td>createMinute</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_month</td>
 *	  <td>createMonth</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_nanosecond</td>
 *	  <td>createNanosecond</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_second</td>
 *	  <td>createSecond</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_week</td>
 *	  <td>createWeek</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_year</td>
 *	  <td>createYear</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_ampere</td>
 *	  <td>createAmpere</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_milliampere</td>
 *	  <td>createMilliampere</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_ohm</td>
 *	  <td>createOhm</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_volt</td>
 *	  <td>createVolt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_calorie</td>
 *	  <td>createCalorie</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_foodcalorie</td>
 *	  <td>createFoodcalorie</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_joule</td>
 *	  <td>createJoule</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilocalorie</td>
 *	  <td>createKilocalorie</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilojoule</td>
 *	  <td>createKilojoule</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilowatt_hour</td>
 *	  <td>createKilowattHour</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_gigahertz</td>
 *	  <td>createGigahertz</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_hertz</td>
 *	  <td>createHertz</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilohertz</td>
 *	  <td>createKilohertz</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_megahertz</td>
 *	  <td>createMegahertz</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_astronomical_unit</td>
 *	  <td>createAstronomicalUnit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_centimeter</td>
 *	  <td>createCentimeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_decimeter</td>
 *	  <td>createDecimeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_fathom</td>
 *	  <td>createFathom</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_foot</td>
 *	  <td>createFoot</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_furlong</td>
 *	  <td>createFurlong</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_inch</td>
 *	  <td>createInch</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilometer</td>
 *	  <td>createKilometer</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_light_year</td>
 *	  <td>createLightYear</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_meter</td>
 *	  <td>createMeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_micrometer</td>
 *	  <td>createMicrometer</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_mile</td>
 *	  <td>createMile</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_millimeter</td>
 *	  <td>createMillimeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_nanometer</td>
 *	  <td>createNanometer</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_nautical_mile</td>
 *	  <td>createNauticalMile</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_parsec</td>
 *	  <td>createParsec</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_picometer</td>
 *	  <td>createPicometer</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_yard</td>
 *	  <td>createYard</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_lux</td>
 *	  <td>createLux</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_carat</td>
 *	  <td>createCarat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_gram</td>
 *	  <td>createGram</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilogram</td>
 *	  <td>createKilogram</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_metric_ton</td>
 *	  <td>createMetricTon</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_microgram</td>
 *	  <td>createMicrogram</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_milligram</td>
 *	  <td>createMilligram</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_ounce</td>
 *	  <td>createOunce</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_ounce_troy</td>
 *	  <td>createOunceTroy</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_pound</td>
 *	  <td>createPound</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_stone</td>
 *	  <td>createStone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_ton</td>
 *	  <td>createTon</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_gigawatt</td>
 *	  <td>createGigawatt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_horsepower</td>
 *	  <td>createHorsepower</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilowatt</td>
 *	  <td>createKilowatt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_megawatt</td>
 *	  <td>createMegawatt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_milliwatt</td>
 *	  <td>createMilliwatt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_watt</td>
 *	  <td>createWatt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_hectopascal</td>
 *	  <td>createHectopascal</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_inch_hg</td>
 *	  <td>createInchHg</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_millibar</td>
 *	  <td>createMillibar</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_millimeter_of_mercury</td>
 *	  <td>createMillimeterOfMercury</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_pound_per_square_inch</td>
 *	  <td>createPoundPerSquareInch</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kilometer_per_hour</td>
 *	  <td>createKilometerPerHour</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_meter_per_second</td>
 *	  <td>createMeterPerSecond</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_mile_per_hour</td>
 *	  <td>createMilePerHour</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_celsius</td>
 *	  <td>createCelsius</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_fahrenheit</td>
 *	  <td>createFahrenheit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_kelvin</td>
 *	  <td>createKelvin</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_acre_foot</td>
 *	  <td>createAcreFoot</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_bushel</td>
 *	  <td>createBushel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_centiliter</td>
 *	  <td>createCentiliter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cubic_centimeter</td>
 *	  <td>createCubicCentimeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cubic_foot</td>
 *	  <td>createCubicFoot</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cubic_inch</td>
 *	  <td>createCubicInch</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cubic_kilometer</td>
 *	  <td>createCubicKilometer</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cubic_meter</td>
 *	  <td>createCubicMeter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cubic_mile</td>
 *	  <td>createCubicMile</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cubic_yard</td>
 *	  <td>createCubicYard</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_cup</td>
 *	  <td>createCup</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_deciliter</td>
 *	  <td>createDeciliter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_fluid_ounce</td>
 *	  <td>createFluidOunce</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_gallon</td>
 *	  <td>createGallon</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_hectoliter</td>
 *	  <td>createHectoliter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_liter</td>
 *	  <td>createLiter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_megaliter</td>
 *	  <td>createMegaliter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_milliliter</td>
 *	  <td>createMilliliter</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_pint</td>
 *	  <td>createPint</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_quart</td>
 *	  <td>createQuart</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_tablespoon</td>
 *	  <td>createTablespoon</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_UNIT_MODULE</td>
 *	  <td>#i18n_measure_unit_create_teaspoon</td>
 *	  <td>createTeaspoon</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_MODULE</td>
 *	  <td>#i18n_measure_create</td>
 *	  <td>Measure</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_MODULE</td>
 *	  <td>#i18n_measure_clone</td>
 *	  <td>clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_MODULE</td>
 *	  <td>#i18n_measure_destroy</td>
 *	  <td>~Measure</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_MODULE</td>
 *	  <td>#i18n_measure_get_number</td>
 *	  <td>getNumber</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_MODULE</td>
 *	  <td>#i18n_measure_get_unit</td>
 *	  <td>getUnit</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMAT_MODULE</td>
 *	  <td>#i18n_format_destroy</td>
 *	  <td>~Format</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMAT_MODULE</td>
 *	  <td>#i18n_format_clone</td>
 *	  <td>clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMAT_MODULE</td>
 *	  <td>#i18n_format_format</td>
 *	  <td>format</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMAT_MODULE</td>
 *	  <td>#i18n_format_parse_object</td>
 *	  <td>parseObject</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FORMAT_MODULE</td>
 *	  <td>#i18n_format_get_locale</td>
 *	  <td>getLocale</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_create</td>
 *	  <td>MeasureFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_destroy</td>
 *	  <td>~MeasureFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_clone</td>
 *	  <td>clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_format</td>
 *	  <td>format</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_parse_object</td>
 *	  <td>parseObject</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_create_currency_format_from_locale</td>
 *	  <td>createCurrencyFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_create_currency_format</td>
 *	  <td>createCurrencyFormat</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_format_buff</td>
 *	  <td>format</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_format_measures</td>
 *	  <td>formatMeasures</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	  <td>#i18n_measure_format_format_measure_per_unit</td>
 *	  <td>formatMeasurePerUnit</td>
 * </tr>
 * <tr>
 *	 <td>@ref CAPI_BASE_UTILS_I18N_MEASURE_FORMAT_MODULE</td>
 *	 <td>#i18n_measure_format_get_unit_display_name</td>
 *	 <td>getUnitDisplayName</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_create</td>
 *	  <td>FieldPosition</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_create_for_field</td>
 *	  <td>FieldPosition</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_destroy</td>
 *	  <td>~FieldPosition</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_clone</td>
 *	  <td>clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_get_field</td>
 *	  <td>getField</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_get_begin_index</td>
 *	  <td>getBeginIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_get_end_index</td>
 *	  <td>getEndIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_set_field</td>
 *	  <td>setField</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_set_begin_index</td>
 *	  <td>setBeginIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_FIELD_POSITION_MODULE</td>
 *	  <td>#i18n_field_position_set_end_index</td>
 *	  <td>setEndIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_create</td>
 *	  <td>ParsePosition</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_create_with_index</td>
 *	  <td>ParsePosition</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_destroy</td>
 *	  <td>~ParsePosition</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_clone</td>
 *	  <td>clone</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_get_index</td>
 *	  <td>getIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_set_index</td>
 *	  <td>setIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_set_error_index</td>
 *	  <td>setErrorIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_PARSE_POSITION_MODULE</td>
 *	  <td>#i18n_parse_position_get_error_index</td>
 *	  <td>getErrorIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_USHAPE_MODULE</td>
 *	  <td>#i18n_ushape_shape_arabic</td>
 *	  <td>u_shapeArabic</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE</td>
 *	  <td>#i18n_utmscale_get_time_scale_value</td>
 *	  <td>utmscale_getTimeScaleValue</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE</td>
 *	  <td>#i18n_utmscale_from_int64</td>
 *	  <td>utmscale_fromInt64</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UTMSCALE_MODULE</td>
 *	  <td>#i18n_utmscale_to_int64</td>
 *	  <td>utmscale_toInt64</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_destroy</td>
 *	  <td>ubidi_close</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_count_paragraphs</td>
 *	  <td>ubidi_countParagraphs</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_count_runs</td>
 *	  <td>ubidi_countRuns</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_base_direction</td>
 *	  <td>ubidi_getBaseDirection</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_class_cb</td>
 *	  <td>ubidi_getClassCallback</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_customized_class</td>
 *	  <td>ubidi_getCustomizedClass</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_direction</td>
 *	  <td>ubidi_getDirection</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_length</td>
 *	  <td>ubidi_getLength</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_level_at</td>
 *	  <td>ubidi_getLevelAt</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_levels</td>
 *	  <td>ubidi_getLevels</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_logical_index</td>
 *	  <td>ubidi_getLogicalIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_logical_map</td>
 *	  <td>ubidi_getLogicalMap</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_logical_run</td>
 *	  <td>ubidi_getLogicalRun</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_paragraph</td>
 *	  <td>ubidi_getParagraph</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_paragraph_by_index</td>
 *	  <td>ubidi_getParagraphByIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_para_level</td>
 *	  <td>ubidi_getParaLevel</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_processed_length</td>
 *	  <td>ubidi_getProcessedLength</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_reordering_mode</td>
 *	  <td>ubidi_getReorderingMode</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_reordering_options</td>
 *	  <td>ubidi_getReorderingOptions</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_result_length</td>
 *	  <td>ubidi_getResultLength</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_text</td>
 *	  <td>ubidi_getText</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_visual_index</td>
 *	  <td>ubidi_getVisualIndex</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_visual_map</td>
 *	  <td>ubidi_getVisualMap</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_get_visual_run</td>
 *	  <td>ubidi_getVisualRun</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_invert_map</td>
 *	  <td>ubidi_invertMap</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_is_inverse</td>
 *	  <td>ubidi_isInverse</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_is_order_paragraphs_ltr</td>
 *	  <td>ubidi_isOrderParagraphsLTR</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_create</td>
 *	  <td>ubidi_open</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_create_sized</td>
 *	  <td>ubidi_openSized</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_order_paragraphs_ltr</td>
 *	  <td>ubidi_orderParagraphsLTR</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_reorder_logical</td>
 *	  <td>ubidi_reorderLogical</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_reorder_visual</td>
 *	  <td>ubidi_reorderVisual</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_set_class_cb</td>
 *	  <td>ubidi_setClassCallback</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_set_context</td>
 *	  <td>ubidi_setContext</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_set_inverse</td>
 *	  <td>ubidi_setInverse</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_set_line</td>
 *	  <td>ubidi_setLine</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_set_para</td>
 *	  <td>ubidi_setPara</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_set_reordering_mode</td>
 *	  <td>ubidi_setReorderingMode</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_set_reordering_options</td>
 *	  <td>ubidi_setReorderingOptions</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_write_reordered</td>
 *	  <td>ubidi_writeReordered</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UBIDI_MODULE</td>
 *	  <td>#i18n_ubidi_write_reverse</td>
 *	  <td>ubidi_writeReverse</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UVERSION_MODULE</td>
 *    <td>#i18n_uversion_get_version</td>
 *    <td>u_getVersion</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UVERSION_MODULE</td>
 *    <td>#i18n_uversion_from_string</td>
 *    <td>u_versionFromString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UVERSION_MODULE</td>
 *    <td>#i18n_uversion_from_ustring</td>
 *    <td>u_versionFromUString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UVERSION_MODULE</td>
 *    <td>#i18n_uversion_to_string</td>
 *    <td>u_versionToString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_set_string</td>
 *    <td>uiter_setString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_set_utf16be</td>
 *    <td>uiter_setUTF16BE</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_set_utf8</td>
 *    <td>uiter_setUTF8</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_get_index</td>
 *    <td>UCharIteratorGetIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_move</td>
 *    <td>UCharIteratorMove</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_has_next</td>
 *    <td>UCharIteratorHasNext</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_has_previous</td>
 *    <td>UCharIteratorHasPrevious</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_current</td>
 *    <td>UCharIteratorCurrent</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_next</td>
 *    <td>UCharIteratorNext</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_previous</td>
 *    <td>UCharIteratorPrevious</td>
 * </tr>
  * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_get_state</td>
 *    <td>UCharIteratorGetState</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHAR_ITER_MODULE</td>
 *    <td>#i18n_uchar_iter_set_state</td>
 *    <td>UCharIteratorSetState</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_create</td>
 *    <td>PluralRules</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_destroy</td>
 *    <td>~PluralRules</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_clone</td>
 *    <td>clone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_create_rules_from_descr</td>
 *    <td>createRules</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_for_locale</td>
 *    <td>forLocale</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_select_int32</td>
 *    <td>select</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_select_double</td>
 *    <td>select</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_get_keywords</td>
 *    <td>getKeywords</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_get_samples</td>
 *    <td>getSamples</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_is_keyword</td>
 *    <td>isKeyword</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_RULES_MODULE</td>
 *    <td>#i18n_plural_rules_get_keyword_other</td>
 *    <td>getKeywordOther</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_create</td>
 *    <td>PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_create_from_locale</td>
 *    <td>PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_create_from_rules_pattern</td>
 *    <td>PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_create_from_locale_type_pattern</td>
 *    <td>PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_create_from_locale_type</td>
 *    <td>PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_create_from_locale_pattern</td>
 *    <td>PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_create_from_locale_rules_pattern</td>
 *    <td>PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_destroy</td>
 *    <td>~PluralFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_clone</td>
 *    <td>clone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_apply_pattern</td>
 *    <td>applyPattern</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_format_int32</td>
 *    <td>format</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_format_double</td>
 *    <td>format</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_format_formattable</td>
 *    <td>format</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_set_number_format</td>
 *    <td>setNumberFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_PLURAL_FORMAT_MODULE</td>
 *    <td>#i18n_plural_format_to_pattern</td>
 *    <td>toPattern</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE</td>
 *    <td>#i18n_immutable_idx_create</td>
 *    <td>buildImmutableIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE</td>
 *    <td>#i18n_immutable_idx_destroy</td>
 *    <td>~ImmutableIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE</td>
 *    <td>#i18n_immutable_idx_get_bucket_count</td>
 *    <td>getBucketCount</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE</td>
 *    <td>#i18n_immutable_idx_get_bucket_index</td>
 *    <td>getBucketIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE</td>
 *    <td>#i18n_immutable_idx_get_bucket_label</td>
 *    <td>getLabel</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_IMMUTABLE_IDX_MODULE</td>
 *    <td>#i18n_immutable_idx_get_bucket_label_type</td>
 *    <td>getLabelType</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE</td>
 *    <td>#i18n_date_interval_create</td>
 *    <td>DateInterval</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE</td>
 *    <td>#i18n_date_interval_destroy</td>
 *    <td>~DateInterval </td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE</td>
 *    <td>#i18n_date_interval_clone</td>
 *    <td>clone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE</td>
 *    <td>#i18n_date_interval_get_from_date</td>
 *    <td>getFromDate</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE</td>
 *    <td>#i18n_date_interval_get_to_date</td>
 *    <td>getToDate</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_MODULE</td>
 *    <td>#i18n_date_interval_equals</td>
 *    <td>operator==</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_create</td>
 *    <td>createInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_create_from_locale</td>
 *    <td>createInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_clone</td>
 *    <td>clone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_destroy</td>
 *    <td>~DateIntervalFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_equals</td>
 *    <td>operator==</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_set_time_zone</td>
 *    <td>setTimeZone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_get_time_zone</td>
 *    <td>getTimeZone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_adopt_time_zone</td>
 *    <td>adoptTimeZone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_format_with_date_interval</td>
 *    <td>format</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_DATE_INTERVAL_FORMAT_MODULE</td>
 *    <td>#i18n_date_interval_fmt_format_with_calendar</td>
 *    <td>format</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_create</td>
 *    <td>SimpleDateFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_create_from_pattern</td>
 *    <td>SimpleDateFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_create_from_pattern_override</td>
 *    <td>SimpleDateFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_create_from_pattern_locale</td>
 *    <td>SimpleDateFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_create_from_pattern_override_locale</td>
 *    <td>SimpleDateFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_destroy</td>
 *    <td>~SimpleDateFormat</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_clone</td>
 *    <td>clone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_equals</td>
 *    <td>operator==</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_format_with_field_position</td>
 *    <td>format</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_parse</td>
 *    <td>parse</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_set_2digit_year_start</td>
 *    <td>set2DigitYearStart</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_get_2digit_year_start</td>
 *    <td>get2DigitYearStart</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_to_pattern</td>
 *    <td>toPattern</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_to_localized_pattern</td>
 *    <td>toLocalizedPattern</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_apply_pattern</td>
 *    <td>applyPattern</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_apply_localized_pattern</td>
 *    <td>applyLocalizedPattern</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_adopt_calendar</td>
 *    <td>adoptCalendar</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE</td>
 *    <td>#i18n_simple_date_fmt_set_context</td>
 *    <td>setContext</td>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_create</td>
 *    <td>createInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_create_from_dialect</td>
 *    <td>createInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_create_from_context</td>
 *    <td>createInstance</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_destroy</td>
 *    <td>~LocaleDisplayNames</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_locale</td>
 *    <td>getLocale</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_dialect_handling</td>
 *    <td>getDialectHandling</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_context</td>
 *    <td>getContext</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_loc_disp_name</td>
 *    <td>localeDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_language_disp_name</td>
 *    <td>languageDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_script_disp_name</td>
 *    <td>scriptDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_script_disp_name_with_script_code</td>
 *    <td>scriptDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_region_disp_name</td>
 *    <td>regionDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_variant_disp_name</td>
 *    <td>variantDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_key_disp_name</td>
 *    <td>keyDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_LOCALE_DISPLAY_NAMES_MODULE</td>
 *    <td>#i18n_loc_disp_names_get_key_value_disp_name</td>
 *    <td>keyValueDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>#i18n_unumsys_create</td>
 *    <td>unumsys_open</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>#i18n_unumsys_create_by_name</td>
 *    <td>unumsys_openByName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>#i18n_unumsys_destroy</td>
 *    <td>unumsys_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>i18n_unumsys_get_available_names</td>
 *    <td>unumsys_openAvailableNames</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>#i18n_unumsys_get_name</td>
 *    <td>unumsys_getName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>#i18n_unumsys_is_algorithmic</td>
 *    <td>unumsys_isAlgorithmic</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>#i18n_unumsys_get_radix</td>
 *    <td>unumsys_getRadix</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UNUMSYS_MODULE</td>
 *    <td>#i18n_unumsys_get_description</td>
 *    <td>unumsys_getDescription</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *    <td>#i18n_utext_destroy</td>
 *    <td>utext_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_create_for_UTF8</td>
 *    <td>utext_openUTF8</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_create_for_uchars</td>
 *	  <td>utext_openUChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_clone</td>
 *    <td>utext_clone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_equals</td>
 *	  <td>utext_equals</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_native_length</td>
 *    <td>utext_nativeLength</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_is_length_expensive</td>
 *	  <td>utext_isLengthExpensive</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_char32_at</td>
 *    <td>utext_char32At</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_current32</td>
 *    <td>utext_current32</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_next32</td>
 *	  <td>utext_next32</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_previous32</td>
 *    <td>utext_previous32</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_next32_from</td>
 *	  <td>utext_next32From</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_previous32_from</td>
 *	  <td>utext_previous32From</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_get_native_index</td>
 *	  <td>utext_getNativeIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_set_native_index</td>
 *	  <td>utext_setNativeIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_move_index32</td>
 *	  <td>utext_moveIndex32</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_get_previous_native_index</td>
 *    <td>utext_getPreviousNativeIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_extract</td>
 *	  <td>utext_extract</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_is_writable</td>
 *	  <td>utext_isWritable</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_has_meta_data</td>
 *    <td>utext_hasMetaData</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_replace</td>
 *	 <td>utext_replace</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_copy</td>
 *	  <td>utext_copy</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UTEXT_MODULE</td>
 *	  <td>#i18n_utext_freeze</td>
 *	  <td>utext_freeze</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_get_codes</td>
 *    <td>uscript_getCode</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_get_name</td>
 *    <td>uscript_getName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_get_short_name</td>
 *    <td>uscript_getShortName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_get_script</td>
 *    <td>uscript_getScript</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_has_script</td>
 *    <td>uscript_hasScript</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_get_script_extensions</td>
 *    <td>uscript_getScriptExtensions</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_get_sample_string</td>
 *    <td>uscript_getSampleString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_get_usage</td>
 *    <td>uscript_getUsage</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_is_right_to_left</td>
 *    <td>uscript_isRightToLeft</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_breaks_between_letters</td>
 *    <td>uscript_breaksBetweenLetters</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_USCRIPT_MODULE</td>
 *    <td>#i18n_uscript_is_cased</td>
 *    <td>uscript_isCased</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_create</td>
 *    <td>uidna_openUTS46</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_destroy</td>
 *    <td>uidna_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_label_to_ascii</td>
 *    <td>uidna_labelToASCII</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_label_to_unicode</td>
 *    <td>uidna_labelToUnicode</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_name_to_ascii</td>
 *    <td>uidna_nameToASCII</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_name_to_unicode</td>
 *    <td>uidna_nameToUnicode</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_label_to_ascii_UTF8</td>
 *    <td>uidna_labelToASCII_UTF8</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_label_to_unicode_UTF8</td>
 *    <td>uidna_labelToUnicodeUTF8</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_name_to_ascii_UTF8</td>
 *    <td>uidna_nameToASCII_UTF8</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UIDNA_MODULE</td>
 *    <td>#i18n_uidna_name_to_unicode_UTF8</td>
 *    <td>uidna_nameToUnicodeUTF8</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *	  <td>#i18n_ucnv_compare_names</td>
 *    <td>ucnv_compareNames</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_create</td>
 *    <td>ucnv_open</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_create_unicode</td>
 *    <td>ucnv_openU</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_create_ccsid</td>
 *    <td>ucnv_openCCSID</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_create_package</td>
 *    <td>ucnv_openPackage</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_safe_clone</td>
 *    <td>ucnv_safeClone</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_destroy</td>
 *    <td>ucnv_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_subst_chars</td>
 *    <td>ucnv_getSubstChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_set_subst_chars</td>
 *    <td>ucnv_setSubstChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_set_subst_string</td>
 *    <td>ucnv_setSubstString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_invalid_chars</td>
 *    <td>ucnv_getInvalidChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_invalid_uchars</td>
 *    <td>ucnv_getInvalidUChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_reset</td>
 *    <td>ucnv_reset</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_reset_to_unicode</td>
 *    <td>ucnv_resetToUnicode</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_reset_from_unicode</td>
 *    <td>ucnv_resetFromUnicode</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_max_char_size</td>
 *    <td>ucnv_getMaxCharSize</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_min_char_size</td>
 *    <td>ucnv_getMinCharSize</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_display_name</td>
 *    <td>ucnv_getDisplayName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_name</td>
 *    <td>ucnv_getName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_ccsid</td>
 *    <td>ucnv_getCCSID</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_platform</td>
 *    <td>ucnv_getPlatform</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_type</td>
 *    <td>ucnv_getType</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_starters</td>
 *    <td>ucnv_getStarters</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_unicode_set</td>
 *    <td>ucnv_getUnicodeSet</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_from_unicode</td>
 *    <td>ucnv_fromUnicode</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_to_unicode</td>
 *    <td>ucnv_toUnicode</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_from_uchars</td>
 *    <td>ucnv_fromUChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_to_uchars</td>
 *    <td>ucnv_toUChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_next_uchar</td>
 *    <td>ucnv_getNextUChar</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_convert_ex</td>
 *    <td>ucnv_convertEx</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_convert</td>
 *    <td>ucnv_convert</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_to_algorithmic</td>
 *    <td>ucnv_toAlgorithmic</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_from_algorithmic</td>
 *    <td>ucnv_fromAlgorithmic</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_flush_cache</td>
 *    <td>ucnv_flushCache</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_count_available</td>
 *    <td>ucnv_countAvailable</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_available_name</td>
 *    <td>ucnv_getAvailableName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_create_all_names</td>
 *    <td>ucnv_openAllNames</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_count_aliases</td>
 *    <td>ucnv_countAliases</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_alias</td>
 *    <td>ucnv_getAlias</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_aliases</td>
 *    <td>ucnv_getAliases</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_create_standard_names</td>
 *    <td>ucnv_openStandardNames</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_count_standards</td>
 *    <td>ucnv_countStandards</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_standard</td>
 *    <td>ucnv_getStandard</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_standard_name</td>
 *    <td>ucnv_getStandardName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_canonical_name</td>
 *    <td>ucnv_getCanonicalName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_get_default_name</td>
 *    <td>ucnv_getDefaultName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_set_default_name</td>
 *    <td>ucnv_setDefaultName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_fix_file_separator</td>
 *    <td>ucnv_fixFileSeparator</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_is_ambiguous</td>
 *    <td>ucnv_isAmbiguous</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_set_fallback</td>
 *    <td>ucnv_setFallback</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_uses_fallback</td>
 *    <td>ucnv_usesFallback</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_detect_unicode_signature</td>
 *    <td>ucnv_detectUnicodeSignature</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_from_ucount_pending</td>
 *    <td>ucnv_fromUCountPending</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_to_ucount_pending</td>
 *    <td>ucnv_toUCountPending</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCNV_MODULE</td>
 *    <td>#i18n_ucnv_is_fixed_width</td>
 *    <td>ucnv_isFixedWidth</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE</td>
 *    <td>#i18n_ucnvsel_create</td>
 *    <td>ucnvsel_open</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE</td>
 *    <td>#i18n_ucnvsel_destroy</td>
 *    <td>ucnvsel_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE</td>
 *    <td>#i18n_ucnvsel_create_from_serialized</td>
 *    <td>ucnvsel_openFromSerialized</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE</td>
 *    <td>#i18n_ucnvsel_serialize</td>
 *    <td>ucnvsel_serialize</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE</td>
 *    <td>#i18n_ucnvsel_select_for_string</td>
 *    <td>ucnvsel_selectForString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCONVERTER_SELECTOR_MODULE</td>
 *    <td>#i18n_ucnvsel_select_for_utf8</td>
 *    <td>ucnvsel_selectForUTF8</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_create</td>
 *    <td>ucsdet_open</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_destroy</td>
 *    <td>ucsdet_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_set_text</td>
 *    <td>ucsdet_setText</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_set_declared_encoding</td>
 *    <td>ucsdet_setDeclaredEncoding</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_detect</td>
 *    <td>ucsdet_detect</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_detect_all</td>
 *    <td>ucsdet_detectAll</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_get_name</td>
 *    <td>ucsdet_getName</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_get_confidence</td>
 *    <td>ucsdet_getConfidence</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_get_language</td>
 *    <td>ucsdet_getLanguage</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_get_uchars</td>
 *    <td>ucsdet_getUChars</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_get_all_detectable_charsets</td>
 *    <td>ucsdet_getAllDetectableCharsets</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_is_input_filter_enabled</td>
 *    <td>ucsdet_isInputFilterEnabled</td>
 * </tr>
 * <tr>
 *	  <td>@ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE</td>
 *    <td>#i18n_ucsdet_enable_input_filter</td>
 *    <td>ucsdet_enableInputFilter</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_create</td>
 *    <td>ures_open</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_create_direct</td>
 *    <td>ures_openDirect</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_create_unicode</td>
 *    <td>ures_openU</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_destroy</td>
 *    <td>ures_close</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_version</td>
 *    <td>ures_getVersion</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_locale_by_type</td>
 *    <td>ures_getLocaleByType</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_string</td>
 *    <td>ures_getString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_UTF8_string</td>
 *    <td>ures_getUTF8String</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_binary</td>
 *    <td>ures_getBinary</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_int_vector</td>
 *    <td>ures_getIntVector</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_uint</td>
 *    <td>ures_getUInt</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_int</td>
 *    <td>ures_getInt</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_size</td>
 *    <td>ures_getSize</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_type</td>
 *    <td>ures_getType</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_key</td>
 *    <td>ures_getKey</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_reset_iterator</td>
 *    <td>ures_resetIterator</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_has_next</td>
 *    <td>ures_hasNext</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_next_resource</td>
 *    <td>ures_getNextResource</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_next_string</td>
 *    <td>ures_getNextString</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_by_index</td>
 *    <td>ures_getByIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_string_by_index</td>
 *    <td>ures_getStringByIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_UTF8_string_by_index</td>
 *    <td>ures_getUTF8StringByIndex</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_by_key</td>
 *    <td>ures_getByKey</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_string_by_key</td>
 *    <td>ures_getStringByKey</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_BASE_UTILS_I18N_URES_MODULE</td>
 *    <td>#i18n_ures_get_UTF8_string_by_key</td>
 *    <td>ures_getUTF8StringByKey</td>
 * </tr>
 * </table>
 */

#ifdef __cplusplus
}
#endif

#endif	  /* __UTILS_I18N_H__*/
