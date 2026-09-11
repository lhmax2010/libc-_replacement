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
 *
 * Copyright (C) 1999-2013, International Business Machines Corporation
 * and others. All Rights Reserved.
 */
#pragma once

#include <utils_i18n_types.h>

/**
 * @file utils_i18n_ushape.h
 * @version 0.1
 * @brief utils_i18n_ushape
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_USHAPE_MODULE Ushape
 * @brief Ushape module provides Arabic shaping functionality.
 *
 * @section CAPI_BASE_UTILS_I18N_USHAPE_MODULE_HEADER Required Header
 *	\#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_USHAPE_MODULE_OVERVIEW Overview
 * @details Ushape module provides Arabic shaping functionality.
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_USHAPE_MODULE
 * @{
 */

/**
 * @brief Shapes Arabic text on a character basis.
 * @details This function performs basic operations for "shaping" Arabic text.
 *			It is most useful for use with legacy data formats and legacy display technology
 *			(simple terminals). All operations are performed on Unicode characters.
 *
 *			Text-based shaping means that some character code points in the text are replaced
 *			by others depending on the context. It transforms one kind of text into another.
 *			In comparison, modern displays for Arabic text select appropriate, context-dependent
 *			font glyphs for each text element, which means that they transform text into a glyph
 *			vector.
 *
 *			Text transformations are necessary when modern display technology is not available
 *			or when text needs to be transformed to or from legacy formats that use "shaped"
 *			characters. Since the Arabic script is cursive, connecting adjacent letters to each
 *			other, computers select images for each letter based on the surrounding letters.
 *			This usually results in four images per Arabic letter: initial, middle, final, and
 *			isolated forms. In Unicode, on the other hand, letters are normally stored abstract,
 *			and a display system is expected to select the necessary glyphs.
 *			(This makes searching and other text processing easier because the same letter has only
 *			one code.) It is possible to mimic this with text transformations because there are
 *			characters in Unicode that are rendered as letters with a specific shape (or cursive
 *			connectivity). They were included for interoperability with legacy systems and
 *			codepages, and for unsophisticated display systems.
 *
 *			A second kind of text transformations is supported for Arabic digits:
 *			For compatibility with legacy codepages that only include European digits, it is
 *			possible to replace one set of digits by another, changing the character code points.
 *			These operations can be performed for either Arabic-Indic Digits (U+0660...U+0669) or
 *			Eastern (Extended) Arabic-Indic digits (U+06f0...U+06f9).
 *
 *			Some replacements may result in more or fewer characters (code points). By default,
 *			this means that the destination buffer may receive text with a length different from
 *			the source length. Some legacy systems rely on the length of the text to be constant.
 *			They expect extra spaces to be added or consumed either next to the affected character
 *			or at the end of the text.
 *
 *			For details about the available operations, see the description of the I18N_USHAPE_...
 *			options.
 *
 * @since_tizen 3.0
 * @remarks The @a dest buffer, if not @c NULL, should be allocated by the user before calling
 *			this function.
 *
 * @param[in]  source	   The input text
 * @param[in]  source_len  The number of Unicode characters in the given @a source buffer, or -1 if
 *						   the @a source buffer is NULL-terminated.
 * @param[in]  options	   A 32-bit set of flags that specify the operations that are
 *						   performed on the input text. If no error occurs, then the result will
 *						   always be written to the @a dest buffer.
 * @param[in]  dest_size   The size (capacity) of the @a dest buffer in Unicode characters.
 *						   If @a dest_size is 0, then no output is produced, but the @a dest_len
 *						   is set to the necessary buffer size ("preflighting").
 * @param[out] dest		   The destination buffer that will receive the results of the requested
 *						   operations. It may be @c NULL only if @a dest_size is 0. The source and
 *						   destination must not overlap.
 * @param[out] dest_len	   The number of Unicode characters written to the @a dest buffer.
 *						   If an error occurred, then no output was written, or it may be incomplete.
 *						   If the #I18N_ERROR_BUFFER_OVERFLOW error code is returned, then the
 *						   @a dest_len value indicates the necessary @a dest buffer size.
 *						   @a dest_len may be @c NULL if not needed, however please note that if
 *						   both @a dest and @a dest_len are @c NULL, then
 *						   #I18N_ERROR_INVALID_PARAMETER error will be returned as such a call
 *						   would produce no useful result.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_BUFFER_OVERFLOW Buffer overflow
 * @retval #I18N_ERROR_NO_SPACE_AVAILABLE No space available for in-buffer expansion
 */
int i18n_ushape_shape_arabic(
	const i18n_uchar *source,
	int32_t source_len,
	uint32_t options,
	int32_t dest_size,
	i18n_uchar *dest,
	int32_t *dest_len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
