/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file utils_i18n_ucsdet.h
 * @version 0.1
 * @brief utils_i18n_ucsdet
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE Ucsdet
 * @brief C API: Charset Detection API.
 * @section CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE_HEADER Required Header
 *  \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE_OVERVIEW Overview
 * @details This function provides a facility for detecting the charset or encoding of character data in an unknown text format.
 *          The input data can be from an array of bytes.
 *
 *          Character set detection is at best an imprecise operation.
 *          The detection process will attempt to identify the charset that best matches the characteristics of the byte data,
 *          but the process is partly statistical in nature, and the results can not be guaranteed to always be correct.
 *
 *          For best accuracy in charset detection, the input data should be primarily in a single language,
 *          and a minimum of a few hundred bytes worth of plain text in the language are needed.
 *          The detection process will attempt to ignore html or xml style markup that could otherwise obscure the content.
 *
 *          An alternative to the ICU Charset Detector is the Compact Encoding Detector,
 *          https://github.com/google/compact_enc_det. It often gives more accurate results, especially with short input samples.
 *
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE
 * @{
 */

/**
 * @brief   Creates an #i18n_ucharset_detector_h.
 * @since_tizen 6.0
 * @remarks The @a ucsd should be released using #i18n_ucsdet_destroy().
 * @param[out] ucsd    The newly created charset detector.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_create(i18n_ucharset_detector_h *ucsd);

/**
 * @brief   Destroys a charset detector.
 * @details All storage and any other resources owned by this charset detector will be released.
 *          Failure to destroy a charset detector when finished with it can result in memory leaks in the application.
 * @since_tizen 6.0
 * @param[in] ucsd  The charset detector to be destroyed.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_destroy(i18n_ucharset_detector_h ucsd);

/**
 * @brief   Sets the input byte data whose charset is to detected.
 * @details Ownership of the input text byte array remains with the caller.
 *          The input string must not be altered or deleted until the charset detector is either destroyed or reset to refer to different input text.
 * @since_tizen 6.0
 * @param[in] ucsd     The charset detector to be used.
 * @param[in] text_in  The input text of unknown encoding.
 * @param[in] len      The length of the input text, or -1 if the text is NUL terminated.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_set_text(i18n_ucharset_detector_h ucsd, const char *text_in, int32_t len);

/**
 * @brief   Sets the declared encoding for charset detection.
 * @details The declared encoding of an input text is an encoding obtained by the user from an HTTP header
 *          or XML declaration or similar source that can be provided as an additional hint to the charset detector.
 *
 *          How and whether the declared encoding will be used during the detection process is TBD.
 * @since_tizen 6.0
 * @param[in] ucsd      The charset detector to be used.
 * @param[in] encoding  An encoding for the current data obtained from a header or declaration or other source outside of the byte data itself.
 * @param[in] length  The length of the encoding name, or -1 if the name string is NUL terminated.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_set_declared_encoding(i18n_ucharset_detector_h ucsd, const char *encoding, int32_t length);

/**
 * @brief   Gets the charset that best matches the supplied input data.
 * @details Note though, that because the detection only looks at the start of the input data,
 *          there is a possibility that the returned charset will fail to handle the full set of input data.
 *
 *          The returned match @a ucsm is owned by the detector @a ucsd.
 *          It will remain valid until the detector input is reset, or until the detector is destroyed.
 * @since_tizen 6.0
 * @remarks The @a ucsm is valid until @a ucsd is released.
 * @param[in] ucsd  The charset detector to be used.
 * @param[out] ucsm An #i18n_ucharset_match_h representing the best matching charset, or NULL if no charset matches the byte data.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_detect(i18n_ucharset_detector_h ucsd, i18n_ucharset_match_h *ucsm);

/**
 * @brief   Gets all charset matches that appear to be consistent with the input, returning an array of results.
 * @details The results are ordered with the best quality match first.
 *
 *          Because the detection only looks at a limited amount of the input byte data,
 *          some of the returned charsets may fail to handle the all of input data.
 * @since_tizen 6.0
 * @param[in] ucsd           The charset detector to be used.
 * @param[out] matches_found Pointer to a variable that will be set to the number of charsets
 *                           identified that are consistent with the input data.
 * @param[out] ucsm          A pointer to an array of pointers to #i18n_ucharset_match_h.
 *                           This array, and the #i18n_ucharset_match_h instances it contains, are owned by the @ref CAPI_BASE_UTILS_I18N_UCHARSET_DETECTION_MODULE,
 *                           and will remain valid until the detector @a ucsd is destroyed or modified.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_detect_all(i18n_ucharset_detector_h ucsd, int32_t *matches_found, i18n_ucharset_match_h **ucsm);

/**
 * @brief   Gets the name of the charset represented by an #i18n_ucharset_match_h.
 * @details The storage for the returned name string is owned by @a ucsm,
 *          and will remain valid while @a ucsm is valid.
 *
 *          The name returned is suitable for use with the ICU conversion APIs.
 * @since_tizen 6.0
 * @remarks The @a name should be released using free().
 * @param[in] ucsm  The charset match object.
 * @param[out] name The name of the matching charset.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_get_name(const i18n_ucharset_match_h ucsm, const char **name);

/**
 * @brief   Gets a confidence number for the quality of the match of the byte data with the charset.
 * @details Confidence numbers range from zero to 100, with 100 representing complete confidence and zero representing no confidence.
 *
 *          The confidence values are somewhat arbitrary.
 *          They define an an ordering within the results for any single detection operation
 *          but are not generally comparable between the results for different input.
 *
 *          A confidence value of ten does have a general meaning - it is used for charsets
 *          that can represent the input data, but for which there is no other indication that suggests that the charset is the correct one.
 *          Pure 7 bit ASCII data, for example, is compatible with a great many charsets,
 *          most of which will appear as possible matches with a confidence of 10.
 * @since_tizen 6.0
 * @param[in] ucsm     The charset match object.
 * @param[out] number  A confidence number for the charset match.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_get_confidence(const i18n_ucharset_match_h ucsm, int32_t *number);

/**
 * @brief   Gets the RFC 3066 code for the language of the input data.
 * @details The Charset Detection service is intended primarily for detecting charsets, not language.
 *          For some, but not all, charsets, a language is identified as a byproduct of the detection process,
 *          and that is what is returned by this function.
 *
 *          CAUTION:\n
 *          1. Language information is not available for input data encoded in all charsets. In particular, no language is identified for UTF-8 input data.\n
 *          2. Closely related languages may sometimes be confused.
 *          If more accurate language detection is required, a linguistic analysis package should be used.\n
 *
 *          The storage for the returned @a code is owned by @a ucsm, and will remain valid while @a ucsm is valid.
 * @since_tizen 6.0
 * @remarks The @a code should be released using free().
 * @param[in] ucsm      The charset match object.
 * @param[out] code     The RFC 3066 code for the language of the input data, or an empty string if the language could not be determined.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_get_language(const i18n_ucharset_match_h ucsm, const char **code);

/**
 * @brief   Gets the entire input text as an #i18n_uchar string, placing it into a caller-supplied buffer.
 * @details A terminating NUL character will be appended to the buffer if space is available.
 *
 *          The number of #i18n_uchar characters in the output string, not including the terminating NUL, is returned.
 *
 *          If the supplied buffer is smaller than required to hold the output,
 *          the contents of the buffer are undefined.
 *          The full output string length (the number of #i18n_uchar characters) is returned as always,
 *          and can be used to allocate a buffer of the correct size.
 * @since_tizen 6.0
 * @param[in] ucsm     The charset match object.
 * @param[in] buf      An #i18n_uchar buffer to be filled with the converted text data.
 * @param[in] cap      The capacity of the buffer in #i18n_uchar.
 * @param[out] number  The number of #i18n_uchar in the output string.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_get_uchars(const i18n_ucharset_match_h ucsm, i18n_uchar *buf, int32_t cap, int32_t *number);

/**
 * @brief   Gets an iterator over the set of all detectable charsets - over the charsets that are known to the charset detection service.
 * @details The returned @a iterator provides access to the names of the charsets.
 *
 *          The state of the Charset detector that is passed in does not affect the result of this function,
 *          but requiring a valid charset detector as a parameter insures
 *          that the charset detection service has been safely initialized and that the required detection data is available.
 *
 *          Note: Multiple different charset encodings in a same family may use a single shared name in this implementation.
 *          For example, this method returns an array including "ISO-8859-1" (ISO Latin 1),
 *          but not including "windows-1252" (Windows Latin 1).
 *          However, actual detection result could be "windows-1252"
 *          when the input data matches Latin 1 code points with any points only available in "windows-1252".
 * @since_tizen 6.0
 * @remarks The @a iterator should be released using #i18n_uenumeration_destroy().
 * @param[in] ucsd      A Charset detector.
 * @param[out] iterator An iterator providing access to the detectable charset names.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_get_all_detectable_charsets(i18n_ucharset_detector_h ucsd, i18n_uenumeration_h *iterator);

/**
 * @brief   Gets whether input filtering is enabled for this charset detector.
 * @details Input filtering removes text that appears to be HTML or XML markup
 *          from the input before applying the code page detection heuristics.
 * @since_tizen 6.0
 * @param[in] ucsd      The charset detector to check.
 * @param[out] result   TRUE if filtering is enabled.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_is_input_filter_enabled(i18n_ucharset_detector_h ucsd, i18n_ubool *result);

/**
 * @brief   Enables filtering of input text.
 * @details If filtering is enabled, text within angle brackets ("<" and ">")
 *          will be removed before detection, which will remove most HTML or XML markup.
 * @since_tizen 6.0
 * @param[in] ucsd              The charset detector to check.
 * @param[in] filter            True to enable input text filtering.
 * @param[out] previous_setting The previous setting.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 *
 */
int i18n_ucsdet_enable_input_filter(i18n_ucharset_detector_h ucsd, i18n_ubool filter, i18n_ubool *previous_setting);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */
