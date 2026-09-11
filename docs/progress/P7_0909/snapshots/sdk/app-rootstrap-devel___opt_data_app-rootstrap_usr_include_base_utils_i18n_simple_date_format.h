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
 * @file utils_i18n_simple_date_format.h
 * @version 0.1
 * @brief utils_i18n_simple_date_format
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE Simple Date Format
 * @brief The Simple Date Format module provides API for formatting and parsing dates
 *        in language-independent manner.
 * @section CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE_HEADER Required Header
 *      \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE_OVERVIEW Overview
 * @details The Simple Date Format module provides API for formatting and parsing dates
 *          in language-independent manner.
 *
 *          It allows for formatting (millis -> text), parsing (text -> millis), and normalization.
 *          Formats/Parses a date or time, which is the standard milliseconds since 24:00 GMT, Jan 1, 1970.
 *
 *          Date and Time Patterns:
 *
 *          Date and time formats are specified by date and time pattern strings. Within date and time pattern
 *          strings, all unquoted ASCII letters [A-Za-z] are reserved as pattern letters representing calendar
 *          fields. Simple Date Format supports the date and time formatting algorithm and pattern letters
 *          defined by UTS#35 Unicode Locale Data Markup Language (LDML) and further documented for ICU in the
 *          ICU User Guide. The following pattern letters are currently available (note that the actual values
 *          depend on CLDR and may change from the examples shown here):
 *
 * <table  border="1">
 * <tr>
 *    <th>Field </th>
 *    <th style="text-align: center">Sym. </th>
 *    <th style="text-align: center">No. </th>
 *    <th>Example </th>
 *    <th>Description  </th>
 * </tr>
 * <tr>
 *    <th rowspan="3">era </th>
 *    <td style="text-align: center" rowspan="3">G </td>
 *    <td style="text-align: center">1..3 </td>
 *    <td>AD </td>
 *    <td rowspan="3">Era - Replaced with the Era string for the current date. One to three letters for the
 *                    abbreviated form, four letters for the long (wide) form, five for the narrow form.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>Anno Domini  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>A  </td>
 * </tr>
 * <tr>
 *    <th rowspan="6">year </th>
 *    <td style="text-align: center">y </td>
 *    <td style="text-align: center">1..n </td>
 *    <td>1996 </td>
 *    <td>
 *       Year. Normally the length specifies the padding, but for two letters it also specifies the maximum length. Example:
 *       <div align="center">
 *          <center>
 *             <table  border="1" cellpadding="2" cellspacing="0">
 *                <tr>
 *                   <th>Year </th>
 *                   <th style="text-align: right">y </th>
 *                   <th style="text-align: right">yy </th>
 *                   <th style="text-align: right">yyy </th>
 *                   <th style="text-align: right">yyyy </th>
 *                   <th style="text-align: right">yyyyy  </th>
 *                </tr>
 *                <tr>
 *                   <td>AD 1 </td>
 *                   <td style="text-align: right">1 </td>
 *                   <td style="text-align: right">01 </td>
 *                   <td style="text-align: right">001 </td>
 *                   <td style="text-align: right">0001 </td>
 *                   <td style="text-align: right">00001  </td>
 *                </tr>
 *                <tr>
 *                   <td>AD 12 </td>
 *                   <td style="text-align: right">12 </td>
 *                   <td style="text-align: right">12 </td>
 *                   <td style="text-align: right">012 </td>
 *                   <td style="text-align: right">0012 </td>
 *                   <td style="text-align: right">00012  </td>
 *                </tr>
 *                <tr>
 *                   <td>AD 123 </td>
 *                   <td style="text-align: right">123 </td>
 *                   <td style="text-align: right">23 </td>
 *                   <td style="text-align: right">123 </td>
 *                   <td style="text-align: right">0123 </td>
 *                   <td style="text-align: right">00123  </td>
 *                </tr>
 *                <tr>
 *                   <td>AD 1234 </td>
 *                   <td style="text-align: right">1234 </td>
 *                   <td style="text-align: right">34 </td>
 *                   <td style="text-align: right">1234 </td>
 *                   <td style="text-align: right">1234 </td>
 *                   <td style="text-align: right">01234  </td>
 *                </tr>
 *                <tr>
 *                   <td>AD 12345 </td>
 *                   <td style="text-align: right">12345 </td>
 *                   <td style="text-align: right">45 </td>
 *                   <td style="text-align: right">12345 </td>
 *                   <td style="text-align: right">12345 </td>
 *                   <td style="text-align: right">12345  </td>
 *                </tr>
 *             </table>
 *          </center>
 *       </div>
 *    </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">Y </td>
 *    <td style="text-align: center">1..n </td>
 *    <td>1997 </td>
 *    <td>Year (in "Week of Year" based calendars). Normally the length specifies the padding, but for two letters it also
 *        specifies the maximum length. This year designation is used in ISO year-week calendar as defined by ISO 8601, but can
 *        be used in non-Gregorian based calendar systems where week date processing is desired. May not always be the same value
 *        as calendar year.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">u </td>
 *    <td style="text-align: center">1..n </td>
 *    <td>4601 </td>
 *    <td>Extended year. This is a single number designating the year of this calendar system, encompassing all supra-year fields.
 *        For example, for the Julian calendar system, year numbers are positive, with an era of BCE or CE. An extended year value
 *        for the Julian calendar system assigns positive values to CE years and negative values to BCE years, with 1 BCE being year 0.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center" rowspan="3">U </td>
 *    <td style="text-align: center">1..3 </td>
 *    <td>&amp;#30002;&amp;#23376; </td>
 *    <td rowspan="3">Cyclic year name. Calendars such as the Chinese lunar calendar (and related calendars) and the Hindu calendars
 *                    use 60-year cycles of year names. Use one through three letters for the abbreviated name, four for the full (wide)
 *                    name, or five for the narrow name (currently the data only provides abbreviated names, which will be used for
 *                    all requested name widths). If the calendar does not provide cyclic year name data, or if the year value to be
 *                    formatted is out of the range of years for which cyclic name data is provided, then numeric formatting is used
 *                    (behaves like 'y').  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>(currently also &amp;#30002;&amp;#23376;)  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>(currently also &amp;#30002;&amp;#23376;)  </td>
 * </tr>
 * <tr>
 *    <th rowspan="6">quarter </th>
 *    <td rowspan="3" style="text-align: center">Q </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>02 </td>
 *    <td rowspan="3">Quarter - Use one or two for the numerical quarter, three for the abbreviation, or four for the full (wide)
 *                    name (five for the narrow name is not yet supported).  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>Q2  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>2nd quarter  </td>
 * </tr>
 * <tr>
 *    <td rowspan="3" style="text-align: center">q </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>02 </td>
 *    <td rowspan="3"><b>Stand-Alone</b> Quarter - Use one or two for the numerical quarter, three for the abbreviation, or four
 *                                       for the full name (five for the narrow name is not yet supported).  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>Q2  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>2nd quarter  </td>
 * </tr>
 * <tr>
 *    <th rowspan="8">month </th>
 *    <td rowspan="4" style="text-align: center">M </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>09 </td>
 *    <td rowspan="4">Month - Use one or two for the numerical month, three for the abbreviation, four for the full (wide) name,
 *                    or five for the narrow name. With two ("MM"), the month number is zero-padded if necessary (e.g. "08")  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>Sep  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>September  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>S  </td>
 * </tr>
 * <tr>
 *    <td rowspan="4" style="text-align: center">L </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>09 </td>
 *    <td rowspan="4"><b>Stand-Alone</b> Month - Use one or two for the numerical month, three for the abbreviation, four for the
 *                       full (wide) name, or 5 for the narrow name. With two ("LL"), the month number is zero-padded if
 *                       necessary (e.g. "08")  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>Sep  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>September  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>S  </td>
 * </tr>
 * <tr>
 *    <th rowspan="2">week </th>
 *    <td style="text-align: center">w </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>27 </td>
 *    <td>Week of Year. Use "w" to show the minimum number of digits, or "ww" to always show two digits (zero-padding
 *        if necessary, e.g. "08").  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">W </td>
 *    <td style="text-align: center">1 </td>
 *    <td>3 </td>
 *    <td>Week of Month  </td>
 * </tr>
 * <tr>
 *    <th rowspan="4">day </th>
 *    <td style="text-align: center">d </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>1 </td>
 *    <td>Date - Day of the month. Use "d" to show the minimum number of digits, or "dd" to always show two digits (zero-padding
 *        if necessary, e.g. "08").  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">D </td>
 *    <td style="text-align: center">1..3 </td>
 *    <td>345 </td>
 *    <td>Day of year  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">F </td>
 *    <td style="text-align: center">1 </td>
 *    <td>2 </td>
 *    <td>Day of Week in Month. The example is for the 2nd Wed in July  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">g </td>
 *    <td style="text-align: center">1..n </td>
 *    <td>2451334 </td>
 *    <td>Modified Julian day. This is different from the conventional Julian day number in two regards. First, it demarcates
 *        days at local zone midnight, rather than noon GMT. Second, it is a local number; that is, it depends on the local
 *        time zone. It can be thought of as a single number that encompasses all the date-related fields.  </td>
 * </tr>
 * <tr>
 *    <th rowspan="14">week<br />
 *       day
 *    </th>
 *    <td rowspan="4" style="text-align: center">E </td>
 *    <td style="text-align: center">1..3 </td>
 *    <td>Tue </td>
 *    <td rowspan="4">Day of week - Use one through three letters for the short day, four for the full (wide) name, five
 *                    for the narrow name, or six for the short name.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>Tuesday  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>T  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">6 </td>
 *    <td>Tu  </td>
 * </tr>
 * <tr>
 *    <td rowspan="5" style="text-align: center">e </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>2 </td>
 *    <td rowspan="5">Local day of week. Same as E except adds a numeric value that will depend on the local starting day
 *                    of the week, using one or two letters. For this example, Monday is the first day of the week.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>Tue  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>Tuesday  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>T  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">6 </td>
 *    <td>Tu  </td>
 * </tr>
 * <tr>
 *    <td rowspan="5" style="text-align: center">c </td>
 *    <td style="text-align: center">1 </td>
 *    <td>2 </td>
 *    <td rowspan="5"><b>Stand-Alone</b> local day of week - Use one letter for the local numeric value (same as 'e'),
 *                       three for the short day, four for the full (wide) name, five for the narrow name, or six for the short name.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>Tue  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>Tuesday  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>T  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">6 </td>
 *    <td>Tu  </td>
 * </tr>
 * <tr>
 *    <th>period </th>
 *    <td style="text-align: center">a </td>
 *    <td style="text-align: center">1 </td>
 *    <td>AM </td>
 *    <td>AM or PM  </td>
 * </tr>
 * <tr>
 *    <th rowspan="4">hour </th>
 *    <td style="text-align: center">h </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>11 </td>
 *    <td>Hour [1-12]. When used in skeleton data or in a skeleton passed in an API for flexible data pattern generation,
 *        it should match the 12-hour-cycle format preferred by the locale (h or K); it should not match a 24-hour-cycle
 *        format (H or k). Use hh for zero padding.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">H </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>13 </td>
 *    <td>Hour [0-23]. When used in skeleton data or in a skeleton passed in an API for flexible data pattern generation, it
 *        should match the 24-hour-cycle format preferred by the locale (H or k); it should not match a 12-hour-cycle format
 *        (h or K). Use HH for zero padding.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">K </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>0 </td>
 *    <td>Hour [0-11]. When used in a skeleton, only matches K or h, see above. Use KK for zero padding.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">k </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>24 </td>
 *    <td>Hour [1-24]. When used in a skeleton, only matches k or H, see above. Use kk for zero padding.  </td>
 * </tr>
 * <tr>
 *    <th>minute </th>
 *    <td style="text-align: center">m </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>59 </td>
 *    <td>Minute. Use "m" to show the minimum number of digits, or "mm" to always show two digits (zero-padding if necessary,
 *        e.g. "08").  </td>
 * </tr>
 * <tr>
 *    <th rowspan="3">second </th>
 *    <td style="text-align: center">s </td>
 *    <td style="text-align: center">1..2 </td>
 *    <td>12 </td>
 *    <td>Second. Use "s" to show the minimum number of digits, or "ss" to always show two digits (zero-padding if necessary,
 *        e.g. "08").  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">S </td>
 *    <td style="text-align: center">1..n </td>
 *    <td>3450 </td>
 *    <td>Fractional Second - truncates (like other time fields) to the count of letters when formatting. Appends zeros if more
 *        than 3 letters specified. Truncates at three significant digits when parsing. (example shows display using pattern
 *        SSSS for seconds value 12.34567)  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">A </td>
 *    <td style="text-align: center">1..n </td>
 *    <td>69540000 </td>
 *    <td>Milliseconds in day. This field behaves <em>exactly</em> like a composite of all time-related fields, not including
 *        the zone fields. As such, it also reflects discontinuities of those fields on DST transition days. On a day of DST
 *        onset, it will jump forward. On a day of DST cessation, it will jump backward. This reflects the fact that is must
 *        be combined with the offset field to obtain a unique local time value.  </td>
 * </tr>
 * <tr>
 *    <th rowspan="23">zone </th>
 *    <td rowspan="2" style="text-align: center">z </td>
 *    <td style="text-align: center">1..3 </td>
 *    <td>PDT </td>
 *    <td>The <em>short specific non-location format</em>. Where that is unavailable, falls back to the <em>short localized
 *        GMT format</em> ("O").  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>Pacific Daylight Time </td>
 *    <td>The <em>long specific non-location format</em>. Where that is unavailable, falls back to the <em>long localized
 *        GMT format</em> ("OOOO").  </td>
 * </tr>
 * <tr>
 *    <td rowspan="3" style="text-align: center">Z </td>
 *    <td style="text-align: center">1..3 </td>
 *    <td>-0800 </td>
 *    <td>The <em>ISO8601 basic format</em> with hours, minutes and optional seconds fields. The format is equivalent to
 *        RFC 822 zone format (when optional seconds field is absent). This is equivalent to the "xxxx" specifier.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>GMT-8:00 </td>
 *    <td>The <em>long localized GMT format</em>. This is equivalent to the "OOOO" specifier.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>-08:00<br />
 *       -07:52:58
 *    </td>
 *    <td>The <em>ISO8601 extended format</em> with hours, minutes and optional seconds fields. The ISO8601 UTC indicator "Z"
 *        is used when local time offset is 0. This is equivalent to the "XXXXX" specifier.  </td>
 * </tr>
 * <tr>
 *    <td rowspan="2" style="text-align: center">O </td>
 *    <td style="text-align: center">1 </td>
 *    <td>GMT-8 </td>
 *    <td>The <em>short localized GMT format</em>.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>GMT-08:00 </td>
 *    <td>The <em>long localized GMT format</em>.  </td>
 * </tr>
 * <tr>
 *    <td rowspan="2" style="text-align: center">v </td>
 *    <td style="text-align: center">1 </td>
 *    <td>PT </td>
 *    <td>The <em>short generic non-location format</em>. Where that is unavailable, falls back to the <em>generic location
 *        format</em> ("VVVV"), then the <em>short localized GMT format</em> as the final fallback.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>Pacific Time </td>
 *    <td>The <em>long generic non-location format</em>. Where that is unavailable, falls back to <em>generic location
 *        format</em> ("VVVV").  </td>
 * </tr>
 * <tr>
 *    <td rowspan="4" style="text-align: center">V </td>
 *    <td style="text-align: center">1 </td>
 *    <td>uslax </td>
 *    <td>The short time zone ID. Where that is unavailable, the special short time zone ID <em>unk</em> (Unknown Zone) is used.<br />
 *       <em><b>Note</b>: This specifier was originally used for a variant of the short specific non-location format, but it was
 *                      deprecated in the later version of the LDML specification. In CLDR 23/ICU 51, the definition of the
 *                      specifier was changed to designate a short time zone ID.</em>
 *    </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">2 </td>
 *    <td>America/Los_Angeles </td>
 *    <td>The long time zone ID.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>Los Angeles </td>
 *    <td>The exemplar city (location) for the time zone. Where that is unavailable, the localized exemplar city name for the
 *        special zone <em>Etc/Unknown</em> is used as the fallback (for example, "Unknown City").   </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>Los Angeles Time </td>
 *    <td>The <em>generic location format</em>. Where that is unavailable, falls back to the <em>long localized GMT
 *        format</em> ("OOOO"; Note: Fallback is only necessary with a GMT-style Time Zone ID, like Etc/GMT-830.)<br />
 *       This is especially useful when presenting possible timezone choices for user selection, since the naming is more
 *       uniform than the "v" format.
 *    </td>
 * </tr>
 * <tr>
 *    <td rowspan="5" style="text-align: center">X </td>
 *    <td style="text-align: center">1 </td>
 *    <td>-08<br />
 *       +0530<br />
 *       Z
 *    </td>
 *    <td>The <em>ISO8601 basic format</em> with hours field and optional minutes field. The ISO8601 UTC indicator "Z" is used
 *        when local time offset is 0.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">2 </td>
 *    <td>-0800<br />
 *       Z
 *    </td>
 *    <td>The <em>ISO8601 basic format</em> with hours and minutes fields. The ISO8601 UTC indicator "Z" is used when local
 *        time offset is 0.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>-08:00<br />
 *       Z
 *    </td>
 *    <td>The <em>ISO8601 extended format</em> with hours and minutes fields. The ISO8601 UTC indicator "Z" is used when
 *        local time offset is 0.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>-0800<br />
 *       -075258<br />
 *       Z
 *    </td>
 *    <td>The <em>ISO8601 basic format</em> with hours, minutes and optional seconds fields. (Note: The seconds field is not
 *        supported by the ISO8601 specification.) The ISO8601 UTC indicator "Z" is used when local time offset is 0.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>-08:00<br />
 *       -07:52:58<br />
 *       Z
 *    </td>
 *    <td>The <em>ISO8601 extended format</em> with hours, minutes and optional seconds fields. (Note: The seconds field is
 *        not supported by the ISO8601 specification.) The ISO8601 UTC indicator "Z" is used when local time offset is 0.  </td>
 * </tr>
 * <tr>
 *    <td rowspan="5" style="text-align: center">x </td>
 *    <td style="text-align: center">1 </td>
 *    <td>-08<br />
 *       +0530
 *    </td>
 *    <td>The <em>ISO8601 basic format</em> with hours field and optional minutes field.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">2 </td>
 *    <td>-0800 </td>
 *    <td>The <em>ISO8601 basic format</em> with hours and minutes fields.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">3 </td>
 *    <td>-08:00 </td>
 *    <td>The <em>ISO8601 extended format</em> with hours and minutes fields.  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">4 </td>
 *    <td>-0800<br />
 *       -075258
 *    </td>
 *    <td>The <em>ISO8601 basic format</em> with hours, minutes and optional seconds fields. (Note: The seconds field is
 *        not supported by the ISO8601 specification.)  </td>
 * </tr>
 * <tr>
 *    <td style="text-align: center">5 </td>
 *    <td>-08:00<br />
 *       -07:52:58
 *    </td>
 *    <td>The <em>ISO8601 extended format</em> with hours, minutes and optional seconds fields. (Note: The seconds field
 *        is not supported by the ISO8601 specification.)  </td>
 * </tr>
 * </table>
 *
 * Any characters in the pattern that are not in the ranges of ['a'..'z'] and ['A'..'Z'] will be treated as quoted text.
 * For instance, characters like ':', '.', ' ', '#' and '@' will appear in the resulting time text even they are not embraced
 * within single quotes.
 *
 * A pattern containing any invalid pattern letter will result in a failing UErrorCode result during formatting or parsing.
 *
 * When parsing a date string using the abbreviated year pattern ("y" or "yy"), Simple Date Format must interpret the abbreviated
 * year relative to some century. It does this by adjusting dates to be within 80 years before and 20 years after the time the
 * Simple Date Format instance is created. For example, using a pattern of "MM/dd/yy" and a Simple Date Format instance created
 * on Jan 1, 1997, the string "01/11/12" would be interpreted as Jan 11, 2012 while the string "05/04/64" would be interpreted
 * as May 4, 1964. During parsing, only strings consisting of exactly two digits will be parsed into the default century.
 * Any other numeric string, such as a one digit string, a three or more digit string, or a two digit string that isn't all digits
 * (for example, "-1"), is interpreted literally. So "01/02/3" or "01/02/003" are parsed (for the Gregorian calendar), using the
 * same pattern, as Jan 2, 3 AD. Likewise (but only in lenient parse mode, the default) "01/02/-3" is parsed as Jan 2, 4 BC.
 *
 * If the year pattern has more than two 'y' characters, the year is interpreted literally, regardless of the number of digits.
 * So using the pattern "MM/dd/yyyy", "01/11/12" parses to Jan 11, 12 A.D.
 *
 * When numeric fields abut one another directly, with no intervening delimiter characters, they constitute a run of abutting
 * numeric fields. Such runs are parsed specially. For example, the format "HHmmss" parses the input text "123456" to 12:34:56,
 * parses the input text "12345" to 1:23:45, and fails to parse "1234". In other words, the leftmost field of the run is flexible,
 * while the others keep a fixed width. If the parse fails anywhere in the run, then the leftmost field is shortened by one
 * character, and the entire run is parsed again. This is repeated until either the parse succeeds or the leftmost field is one
 * character in length. If the parse still fails at that point, the parse of the run fails.
 *
 * For time zones that have no names, Simple Date Format uses strings GMT+hours:minutes or GMT-hours:minutes.
 *
 * The calendar defines what is the first day of the week, the first week of the year, whether hours are zero based or not
 * (0 vs 12 or 24), and the timezone. There is one common number format to handle all the numbers; the digit count is handled
 * programmatically according to the pattern.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_SIMPLE_DATE_FORMAT_MODULE
 * @{
 */

/**
 * @brief Creates a simple date format object using the default pattern for the default locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_simple_date_fmt_destroy() function.
 *
 * @param[out]  simple_date_fmt   The created simple date format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_create(i18n_simple_date_fmt_h *simple_date_fmt);

/**
 * @brief Creates a simple date format object using the given pattern and the default locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_simple_date_fmt_destroy() function.
 *
 * @param[in]   pattern          The pattern for the simple date format
 * @param[out]  simple_date_fmt  The created simple date format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_create_from_pattern(const char *pattern,
                                             i18n_simple_date_fmt_h *simple_date_fmt);

/**
 * @brief Creates a simple date format object using the given pattern, numbering system override,
 *        and the default locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_simple_date_fmt_destroy() function.
 *
 * @param[in]   pattern          The pattern for the simple date format
 * @param[in]   override         The override string
 * @param[out]  simple_date_fmt  The created simple date format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_create_from_pattern_override(const char *pattern, const char *override,
                                                      i18n_simple_date_fmt_h *simple_date_fmt);

/**
 * @brief Creates a simple date format object using the given pattern and locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_simple_date_fmt_destroy() function.
 *
 * @param[in]   pattern          The pattern for the simple date format
 * @param[in]   locale           The given locale
 * @param[out]  simple_date_fmt  The created simple date format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_create_from_pattern_locale(const char *pattern, const char *locale,
                                                    i18n_simple_date_fmt_h *simple_date_fmt);

/**
 * @brief Creates a simple date format object  using the given pattern, numbering system
 *        override and locale.
 * @since_tizen 5.0
 * @remarks The created object should be released by the caller with the
 *          i18n_simple_date_fmt_destroy() function.
 *
 * @param[in]  pattern          The pattern for the simple date format
 * @param[in]  override         The numbering system override
 * @param[in]  locale           The given locale
 * @param[out] simple_date_fmt  The created simple date format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_create_from_pattern_override_locale(const char *pattern, const char *override,
                                                             const char *locale,
                                                             i18n_simple_date_fmt_h *simple_date_fmt);

/**
 * @brief Destroys the given simple date format object.
 * @since_tizen 5.0
 *
 * @param[in] simple_date_fmt  The simple date format object to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_destroy(i18n_simple_date_fmt_h simple_date_fmt);

/**
 * @brief Creates a clone of the given @a simple_date_fmt object.
 * @since_tizen 5.0
 * @remarks The @a clone object should be released by the caller with the
 *          i18n_simple_date_fmt_destroy() function.
 *
 * @param[in]   simple_date_fmt   The simple date format object to be cloned
 * @param[out]  clone             The created simple date format object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_clone(i18n_simple_date_fmt_h simple_date_fmt,
                               i18n_simple_date_fmt_h *clone);

/**
 * @brief Checks whether two simple date format objects are equal.
 * @since_tizen 5.0
 *
 * @param[in]   simple_date_fmt_1    The first simple date format object
 * @param[in]   simple_date_fmt_2    The second simple date format format object
 * @param[out]  equals               @c true if @a simple_date_fmt_1 is equal
 *                                   to @a simple_date_fmt_2, @c false otherwise
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_equals(i18n_simple_date_fmt_h simple_date_fmt_1,
                                i18n_simple_date_fmt_h simple_date_fmt_2,
                                bool *equals);

/**
 * @brief Formats a date or time (which is the standard millis since 24:00 GMT, Jan 1, 1970)
 *        using field position.
 * @since_tizen 5.0
 *
 * @param[in]      simple_date_fmt    The format object for which date or time will be formatted
 * @param[in]      calendar           Calendar set to the date and time to be formatted into a
 *                                    date/time string
 * @param[in,out]  append_to          Input/output parameter to receive the result.
 *                                    The result is appended to the existing contents.
 * @param[in]      field_position     The #i18n_field_position_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_format_with_field_position(i18n_simple_date_fmt_h simple_date_fmt,
                                                    i18n_ucalendar_h calendar, char **append_to,
                                                    i18n_field_position_h field_position);

/**
 * @brief Parses a date/time string beginning at the given parse position.
 * @details For example, a time text "07/10/96 4:5 pm, pdt" will be parsed into a date that is equivalent
 *        to date 837039928046. By default, parsing is lenient: if the input is not in the form used
 *        by this object's format method but can still be parsed as a date, then the parse succeeds.
 *        Clients may insist on strict adherence to the format by calling set_lenient(false).
 * @since_tizen 5.0
 *
 * @param[in]      simple_date_fmt   The simple date format object
 * @param[in]      source            The date/time string to be parsed
 * @param[in,out]  parse_position    On input, the position at which to start parsing; on output, the
 *                                   position at which parsing terminated, or the start position if the
 *                                   parse failed
 * @param[in,out]  result            A #i18n_ucalendar_h object set on input to the date and time to be used
 *                                   for missing values in the date/time string being parsed, and set on output
 *                                   to the parsed date/time. When the calendar type is different from the internal
 *                                   calendar held by this #i18n_simple_date_fmt_h object instance, the internal
 *                                   calendar will be cloned to a work calendar set to the same milliseconds and
 *                                   time zone as the cal parameter, field values will be parsed based on the
 *                                   work calendar, then the result (milliseconds and time zone) will be set in
 *                                   this calendar.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_parse(i18n_simple_date_fmt_h simple_date_fmt, const char *source,
                               i18n_parse_position_h parse_position, i18n_ucalendar_h *result);

/**
 * @brief Sets the start i18n_udate used to interpret two-digit year strings.
 * @details When dates are parsed having 2-digit year strings, they are placed within a assumed
 *          range of 100 years starting on the two digit start date. For example, the string "24-jan-17"
 *          may be in the year 1817, 1917, 2017, or some other year. Simple date format chooses a year
 *          so that the resultant date is on or after the two digit start date and within 100 years of the
 *          two digit start date. by default, the two digit start date is set to 80 years before the current
 *          time at which a #i18n_simple_date_fmt_h object is created.
 * @since_tizen 5.0
 *
 * @param[in]    simple_date_fmt   The simple date format object
 * @param[in]    udate             The start date of the given #i18n_simple_date_fmt_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_set_2digit_year_start(i18n_simple_date_fmt_h simple_date_fmt,
                                               i18n_udate udate);

/**
 * @brief Gets the start i18n_udate used to interpret two-digit year strings.
 * @details When dates are parsed having 2-digit year strings, they are placed within a assumed range
 *          of 100 years starting on the two digit start date. For example, the string "24-jan-17" may
 *          be in the year 1817, 1917, 2017, or some other year. Simple date format chooses a year so that
 *          the resultant date is on or after the two digit start date and within 100 years of the two digit
 *          start date. By default, the two digit start date is set to 80 years before the current time at
 *          which a #i18n_simple_date_fmt_h object is created.
 * @since_tizen 5.0
 *
 * @param[in]    simple_date_fmt   The simple date format object
 * @param[out]   udate             The start date of the given #i18n_simple_date_fmt_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_get_2digit_year_start(i18n_simple_date_fmt_h simple_date_fmt,
                                               i18n_udate *udate);

/**
 * @brief Returns a pattern string that describes given simple format date object.
 * @since_tizen 5.0
 * @remarks The returned pattern should be freed by the caller with free() function.
 *
 * @param[in]   simple_date_fmt   The simple date format object
 * @param[out]  pattern           The pattern from the given simple date object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY      Out of memory
 */
int i18n_simple_date_fmt_to_pattern(i18n_simple_date_fmt_h simple_date_fmt,
                                    char **pattern);

/**
 * @brief Returns a localized pattern string that describes given simple date format object.
 * @details In most cases, this will return the same thing as i18n_simple_date_fmt_to_pattern(),
 *          but a locale can specify characters to use in pattern descriptions in place of the ones
 *          described in this class's class documentation. Presumably, letters that would be more
 *          mnemonic in that locale's language. This function would produce a pattern using those
 *          letters.
 * @since_tizen 5.0
 * @remarks The returned localized pattern should be freed by the caller with free() function.
 *
 * @param[in]   simple_date_fmt        The simple date format object
 * @param[out]  localized_pattern      The localized pattern from the given simple date object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_to_localized_pattern(i18n_simple_date_fmt_h simple_date_fmt,
                                              char **localized_pattern);

/**
 * @brief Applies the given unlocalized pattern string to this date format.
 * @details After this call, this formatter will format dates according to the new pattern.
 * @since_tizen 5.0
 *
 * @param[in]  simple_date_fmt  The simple date format object
 * @param[in]  pattern          The pattern to be applied
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_apply_pattern(i18n_simple_date_fmt_h simple_date_fmt,
                                       const char *pattern);

/**
 * @brief Applies the given localized pattern string to this date format.
 * @since_tizen 5.0
 *
 * @param[in]  simple_date_fmt       The simple date format object
 * @param[in]  localized_pattern     The localized pattern to be applied
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_apply_localized_pattern(i18n_simple_date_fmt_h simple_date_fmt,
                                                 const char *localized_pattern);

/**
 * @brief Sets the calendar to be used by this date format.
 * @details Initially, the default calendar for the specified or default locale is used. The caller
 *          should not delete the #i18n_ucalendar_h object after it is adopted by this call. Adopting
 *          a new calendar will change to the default symbols.
 * @since_tizen 5.0
 *
 * @param[in]  simple_date_fmt   The simple date format object
 * @param[in]  ucalendar         The #i18n_ucalendar_h object to be adopted
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_adopt_calendar(i18n_simple_date_fmt_h simple_date_fmt,
                                        i18n_ucalendar_h ucalendar);

/**
 * @brief Sets a particular #i18n_udisplay_context_e value in the formatter, such as
 *        #I18N_UDISPLAY_CONTEXT_CAPITALIZATION_FOR_STANDALONE.
 * @since_tizen 5.0
 *
 * @param[in]  simple_date_fmt   The simple date format object
 * @param[in]  value             The i18n_udisplay_context_e value to set
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE               Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER  Invalid function parameter
 */
int i18n_simple_date_fmt_set_context(i18n_simple_date_fmt_h simple_date_fmt,
                                     i18n_udisplay_context_e value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
