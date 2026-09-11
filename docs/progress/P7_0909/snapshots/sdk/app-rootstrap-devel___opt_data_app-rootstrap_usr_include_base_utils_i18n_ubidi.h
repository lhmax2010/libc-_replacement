/*
* Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	 http://www.apache.org/licenses/LICENSE-2.0
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
 * @file utils_i18n_ubidi.h
 * @version 0.1
 * @brief utils_i18n_ubidi
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CAPI_BASE_UTILS_I18N_MODULE
 * @defgroup CAPI_BASE_UTILS_I18N_UBIDI_MODULE Ubidi
 * @brief Ubidi module provides an implementation of the Unicode Bidirectional Algorithm.
 *
 * @section CAPI_BASE_UTILS_I18N_UBIDI_MODULE_HEADER Required Header
 * \#include <utils_i18n.h>
 *
 * @section CAPI_BASE_UTILS_I18N_UBIDI_MODULE_OVERVIEW Overview
 * @details Ubidi module provides an implementation of the Unicode Bidirectional Algorithm. The
 *			algorithm is defined in the Unicode Standard Annex #9.
 *			Note: Libraries that perform a bidirectional algorithm and reorder strings accordingly
 *			are sometimes called "Storage Layout Engines".
 *			ubidi and shaping APIs can be used at the core of such "Storage Layout Engines".
 */

/**
 * @addtogroup CAPI_BASE_UTILS_I18N_UBIDI_MODULE
 * @{
 */

/**
 * @brief This function must be called to free the memory associated with an #i18n_ubidi_h handle.
 * @details Important: A parent #i18n_ubidi_h handle must not be destroyed or reused if it still has
 *			children. If an #i18n_ubidi_h handle has become the child of
 *			another one (its parent) by calling i18n_ubidi_set_line(), then the child object must be
 *			destroyed or reused (by calling i18n_ubidi_set_para() or i18n_ubidi_set_line()) before
 *			the parent object.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		#i18n_ubidi_h handle to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_para()
 * @see i18n_ubidi_set_line()
 */
int i18n_ubidi_destroy(i18n_ubidi_h ubidi);

/**
 * @brief Gets the number of paragraphs.
 * @since_tizen 3.0
 *
 * @param[in]	   ubidi		The paragraph or line #i18n_ubidi_h object
 * @param[out]	   count		The number of paragraphs
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_count_paragraphs(i18n_ubidi_h ubidi, int32_t *count);

/**
 * @brief Gets the number of runs.
 * @details This function may invoke the actual reordering on the #i18n_ubidi_h handle, after
 *			i18n_ubidi_set_para() may have resolved only the levels of the text. Therefore,
 *			i18n_ubidi_count_runs() may have to allocate memory, and may fail doing so.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		The paragraph or line #i18n_ubidi_h object
 * @param[out]		count		The number of runs
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 */
int i18n_ubidi_count_runs(i18n_ubidi_h ubidi, int32_t *count);

/**
 * @brief Gets the base direction of the text provided according to the Unicode Bidirectional
 *			Algorithm.
 * @details The base direction is derived from the first character in the string with bidirectional
 *			character type L, R, or AL. If the first such character has type L, #I18N_UBIDI_LTR is
 *			returned. If the first such character has type R or AL, #I18N_UBIDI_RTL is returned.
 *			If the string does not contain any character of these types, then #I18N_UBIDI_NEUTRAL
 *			is returned.
 *
 *			This is a lightweight function for use when only the base direction is needed and no
 *			further bidi processing of the text is needed.
 * @since_tizen 3.0
 *
 * @param[in]	   text		  A pointer to the @a text whose base direction is needed. Note: the @a text
 *							  must be (at least) @a length long.
 * @param[in]	   length	  The length of the @a text; if @a length == -1 then the text must be
 *							  zero-terminated
 * @param[out]	   direction  Base direction of the @a text
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_get_base_direction(const i18n_uchar *text, int32_t length,
								  i18n_ubidi_direction_e *direction);

/**
 * @brief Gets the current callback function used for ubidi class determination.
 * @since_tizen 3.0
 * @remarks The @a fn and @a context should not be released.
 *
 * @param[in]		ubidi		The paragraph #i18n_ubidi_h object
 * @param[out]		fn			The callback function pointer. This can be @c NULL.
 * @param[out]		context		The callback's private context. This can be @c NULL.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_class_cb()
 */
int i18n_ubidi_get_class_cb(i18n_ubidi_h ubidi, i18n_ubidi_class_cb *fn,
							const void **context);

/**
 * @brief Retrieves the ubidi class for a given code point.
 * @details If an #i18n_ubidi_class_cb callback is defined and returns a value other than
 *			#I18N_UBIDI_CLASS_DEFAULT, that value is used; otherwise the default class
 *			determination mechanism is invoked.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		 The paragraph #i18n_ubidi_h object
 * @param[in]		c			 The code point whose ubidi class must be retrieved
 * @param[out]		direction	 The ubidi class for character @a c based on the given @a ubidi instance
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_class_cb()
 */
int i18n_ubidi_get_customized_class(i18n_ubidi_h ubidi, i18n_uchar32 c,
									i18n_uchar_direction_e *direction);

/**
 * @brief Gets the directionality of the text.
 * @since_tizen 3.0
 *
 * @param[in]		 ubidi		 The paragraph or line #i18n_ubidi_h object
 * @param[out]		 direction	 A value of #I18N_UBIDI_LTR, #I18N_UBIDI_RTL or #I18N_UBIDI_MIXED
 *								 that indicates if the entire text represented by this object is
 *								 unidirectional, and which direction, or if it is mixed-directional.
 *								 Note - The value #I18N_UBIDI_NEUTRAL is never returned from this
 *								 method.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_direction_e
 */
int i18n_ubidi_get_direction(const i18n_ubidi_h ubidi, i18n_ubidi_direction_e *direction);

/**
 * @brief Gets the length of the text.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		 The paragraph or line #i18n_ubidi_h object
 * @param[out]		length		 The length of the text that the #i18n_ubidi_h object was created for
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_get_length(const i18n_ubidi_h ubidi, int32_t *length);

/**
 * @brief Gets the level for one character.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		The paragraph or line #i18n_ubidi_h object
 * @param[in]		char_index	The index of a character. It must be in the range
 *								[0..i18n_ubidi_get_processed_length(@a ubidi)-1]
 * @param[out]		level		The level for the character at @a char_index (0 if @a char_index is not in
 *								the valid range)
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_level_t
 * @see i18n_ubidi_get_processed_length()
 */
int i18n_ubidi_get_level_at(const i18n_ubidi_h ubidi, int32_t char_index,
							i18n_ubidi_level_t *level);

/**
 * @brief Gets an array of levels for each character.
 * @details Note that this function may allocate memory under some circumstances, unlike
 *			i18n_ubidi_get_level_at().
 * @since_tizen 3.0
 * @remarks @a levels should not be freed.
 *
 * @param[in]	   ubidi	 The paragraph or line #i18n_ubidi_h object, whose text length must be
 *							 strictly positive
 * @param[out]	   levels	  The levels array for the text, or @c NULL if an error occurs
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @see i18n_ubidi_level_t
 * @see i18n_ubidi_get_processed_length()
 */
int i18n_ubidi_get_levels(i18n_ubidi_h ubidi, const i18n_ubidi_level_t **levels);

/**
 * @brief Gets the logical text position from a visual position.
 * @details If such a mapping is used many times on the same #i18n_ubidi_h object, then calling
 *			i18n_ubidi_get_visual_map() is more efficient.
 *
 *			The value returned may be #I18N_UBIDI_MAP_NOWHERE if there is no logical position
 *			because the corresponding text character is a ubidi mark inserted in the output by
 *			option #I18N_UBIDI_OPTION_INSERT_MARKS.
 *
 *			This is the inverse function to i18n_ubidi_get_visual_index().
 *
 *			When the visual output is altered by using options of i18n_ubidi_write_reordered() such
 *			as #I18N_UBIDI_INSERT_LRM_FOR_NUMERIC, #I18N_UBIDI_KEEP_BASE_COMBINING,
 *			#I18N_UBIDI_OUTPUT_REVERSE, #I18N_UBIDI_REMOVE_BIDI_CONTROLS, the logical position
 *			returned may not be correct.
 *			It is advised to use, when possible, reordering options such as
 *			#I18N_UBIDI_OPTION_INSERT_MARKS and #I18N_UBIDI_OPTION_REMOVE_CONTROLS.
 * @since_tizen 3.0
 *
 * @param[in]	   ubidi		   The paragraph or line #i18n_ubidi_h object
 * @param[in]	   visual_index	   The visual position of a character
 * @param[out]	   logical_index   The index of this character in the text
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_visual_map()
 * @see i18n_ubidi_get_visual_index()
 * @see i18n_ubidi_get_result_length()
 */
int i18n_ubidi_get_logical_index(i18n_ubidi_h ubidi, int32_t visual_index, int32_t *logical_index);

/**
 * @brief Gets a logical-to-visual index map (array) for the characters in the #i18n_ubidi_h (paragraph or
 *			line) object.
 * @details Some values in the map may be #I18N_UBIDI_MAP_NOWHERE if the corresponding text
 *			characters are ubidi controls removed from the visual output by the option
 *			#I18N_UBIDI_OPTION_REMOVE_CONTROLS.
 *
 *			When the visual output is altered by using options of i18n_ubidi_write_reordered()
 *			such as #I18N_UBIDI_INSERT_LRM_FOR_NUMERIC, #I18N_UBIDI_KEEP_BASE_COMBINING,
 *			#I18N_UBIDI_OUTPUT_REVERSE, #I18N_UBIDI_REMOVE_BIDI_CONTROLS, the visual positions
 *			returned may not be correct. It is advised to use, when possible, reordering options
 *			such as #I18N_UBIDI_OPTION_INSERT_MARKS and #I18N_UBIDI_OPTION_REMOVE_CONTROLS.
 *
 *			Note that in right-to-left runs, this mapping places second surrogates before first
 *			ones (which is generally a bad idea) and combining characters before base characters.
 *			Use of i18n_ubidi_write_reordered(), optionally with the #I18N_UBIDI_KEEP_BASE_COMBINING
 *			option can be considered instead of using the mapping, in order to avoid these issues.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		  The paragraph or line #i18n_ubidi_h object
 * @param[out]		index_map	  A pointer to an array of i18n_ubidi_get_processed_length()
 *								  indexes which will reflect the reordering of the characters.
 *								  If option #I18N_UBIDI_OPTION_INSERT_MARKS is set, the number of
 *								  elements allocated in the @a index_map must be no less than
 *								  i18n_ubidi_get_result_length().
 *
 *								  The array does not need to be initialized.
 *
 *								  The index map will result in
 *								  index_map[logical_index] == visual_index.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_visual_map()
 * @see i18n_ubidi_get_visual_index()
 * @see i18n_ubidi_get_processed_length()
 * @see i18n_ubidi_get_result_length()
 */
int i18n_ubidi_get_logical_map(i18n_ubidi_h ubidi, int32_t *index_map);

/**
 * @brief Gets a logical run.
 * @details This function returns information about a run and is used to retrieve runs in logical
 *			order.
 *
 *			This is especially useful for line-breaking on a paragraph.
 * @since_tizen 3.0
 *
 * @param[in]	   ubidi				The paragraph or line #i18n_ubidi_h object
 * @param[in]	   logical_position		A logical position within the source text
 * @param[out]	   logical_limit		The limit of the corresponding run. The l-value that you
 *										point to here may be the same expression (variable) as
 *										the one for @a logical_position.
 *										This pointer can be @c NULL if this value is not necessary.
 * @param[out]	   level				The level of the corresponding run. This pointer can be
 *										@c NULL if this value is not necessary.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_processed_length()
 */
int i18n_ubidi_get_logical_run(const i18n_ubidi_h ubidi,
							   int32_t logical_position,
							   int32_t *logical_limit,
							   i18n_ubidi_level_t *level);

/**
 * @brief Gets a paragraph, given a position within the text.
 * @details This function returns information about a paragraph.
 *
 *			Note: if the paragraph index is known, it is more efficient to retrieve the paragraph
 *			information using i18n_ubidi_get_paragraph_by_index().
 * @since_tizen 3.0
 *
 * @param[in]		ubidi			 The paragraph or line #i18n_ubidi_h object
 * @param[in]		char_index		 The index of a character within the text, in the range
 *									 [0..i18n_ubidi_get_processed_length(@a ubidi)-1]
 * @param[out]		para_start		 The index of the first character of the paragraph in the text.
 *									 This pointer can be @c NULL if this value is not necessary.
 * @param[out]		para_limit		 The limit of the paragraph. The l-value that you point to here
 *									 may be the same expression (variable) as the one for
 *									 @a char_index. This pointer can be @c NULL if this value is not
 *									 necessary.
 * @param[out]		para_level		 The level of the paragraph. This pointer can be @c NULL if this
 *									 value is not necessary.
 * @param[out]		index			 The index of the paragraph containing the specified position
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_processed_length()
 */
int i18n_ubidi_get_paragraph(const i18n_ubidi_h ubidi,
							 int32_t char_index,
							 int32_t *para_start,
							 int32_t *para_limit,
							 i18n_ubidi_level_t *para_level,
							 int32_t *index);

/**
 * @brief Gets a paragraph, given the index of this paragraph.
 * @details This function returns information about paragraphs.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi			 The paragraph #i18n_ubidi_h object
 * @param[in]		para_index		 The number of the paragraph, in the range
 *									 [0..i18n_ubidi_count_paragraphs(@a ubidi)-1]
 * @param[out]		para_start		 The index of the first character of the paragraph in the text.
 *									 This pointer can be @c NULL if this value is not necessary.
 * @param[out]		para_limit		 The limit of the paragraph. This pointer can be @c NULL if this
 *									 value is not necessary.
 * @param[out]		para_level		 The level of the paragraph. This pointer can be @c NULL if this
 *									 value is not necessary.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_get_paragraph_by_index(const i18n_ubidi_h ubidi,
									  int32_t para_index,
									  int32_t *para_start,
									  int32_t *para_limit,
									  i18n_ubidi_level_t *para_level);

/**
 * @brief Gets the paragraph level of the text.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		  The paragraph or line #i18n_ubidi_h object
 * @param[out]		level		  The paragraph level. If there are multiple paragraphs, their
 *								  level may vary if the required para_level is
 *								  #I18N_UBIDI_DEFAULT_LTR or #I18N_UBIDI_DEFAULT_RTL. In that case,
 *								  the level of the first paragraph is returned.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_level_t
 * @see i18n_ubidi_get_paragraph()
 * @see i18n_ubidi_get_paragraph_by_index()
 */
int i18n_ubidi_get_para_level(const i18n_ubidi_h ubidi, i18n_ubidi_level_t *level);

/**
 * @brief Gets the length of the source text processed by the last call to i18n_ubidi_set_para().
 * @details This length may be different from the length of the source text if option
 *			#I18N_UBIDI_OPTION_STREAMING has been set.
 *
 *			Note that whenever the length of the text affects the execution or the result of a
 *			function, it is the processed length which must be considered, except for
 *			i18n_ubidi_set_para() (which receives unprocessed source text) and i18n_ubidi_get_length()
 *			(which returns the original length of the source text).
 *
 *			In particular, the processed @a length is the one to consider in the following cases:
 *				- maximum value of the limit argument of i18n_ubidi_set_line()
 *				- maximum value of the char_index argument of i18n_ubidi_get_paragraph()
 *				- maximum value of the char_index argument of i18n_ubidi_get_level_at()
 *				- number of elements in the array returned by i18n_ubidi_get_levels()
 *				- maximum value of the logical_start argument of i18n_ubidi_get_logical_run()
 *				- maximum value of the logical_index argument of i18n_ubidi_get_visual_index()
 *				- number of elements filled in the @a index_map argument of i18n_ubidi_get_logical_map()
 *				- length of text processed by i18n_ubidi_write_reordered()
 *
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		   The paragraph #i18n_ubidi_h object
 * @param[out]		length		   The length of the part of the source text processed by the last
 *								   call to i18n_ubidi_set_para()
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_para()
 * @see I18N_UBIDI_OPTION_STREAMING
 */
int i18n_ubidi_get_processed_length(const i18n_ubidi_h ubidi, int32_t *length);

/**
 * @brief Gets the requested reordering mode for a given #i18n_ubidi_h object.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		An #i18n_ubidi_h object
 * @param[out]		mode		The current reordering mode of the @a ubidi object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_reordering_mode()
 */
int i18n_ubidi_get_reordering_mode(i18n_ubidi_h ubidi, i18n_ubidi_reordering_mode_e *mode);

/**
 * @brief Gets the reordering options applied to a given #i18n_ubidi_h object.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		An #i18n_ubidi_h object
 * @param[out]		options		The current reordering options of the @a ubidi object;
 *								#i18n_ubidi_reordering_option_e values combined with bitwise 'or'
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_reordering_options()
 */
int i18n_ubidi_get_reordering_options(i18n_ubidi_h ubidi, uint32_t *options);

/**
 * @brief Gets the length of the reordered text resulting from the last call to
 *			i18n_ubidi_set_para().
 * @details This length may be different from the length of the source text if option
 *			#I18N_UBIDI_OPTION_INSERT_MARKS or option #I18N_UBIDI_OPTION_REMOVE_CONTROLS has been set.
 *
 *			This resulting @a length is the one to consider in the following cases:
 *				- maximum value of the visual_index argument of i18n_ubidi_get_logical_index()
 *				- number of elements of the a index_map argument of i18n_ubidi_get_visual_map()
 *
 *			Note that this @a length stays identical to the source text length if ubidi marks are
 *			inserted or removed using option bits of i18n_ubidi_write_reordered(), or if option
 *			#I18N_UBIDI_REORDER_INVERSE_NUMBERS_AS_L has been set.
 * @since_tizen 3.0
 *
 * @param[in]		 ubidi		  The paragraph #i18n_ubidi_h object
 * @param[out]		 length		  The length of the reordered text resulting from the last call to
 *								  i18n_ubidi_set_para()
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_para()
 * @see I18N_UBIDI_OPTION_INSERT_MARKS
 * @see I18N_UBIDI_OPTION_REMOVE_CONTROLS
 */
int i18n_ubidi_get_result_length(const i18n_ubidi_h ubidi, int32_t *length);

/**
 * @brief Gets the pointer to the given #i18n_ubidi_h object's text.
 * @since_tizen 3.0
 * @remarks @a text should be freed with free().
 *
 * @param[in]		ubidi	   The paragraph or line #i18n_ubidi_h object
 * @param[out]		text	   The pointer to the text that the @a ubidi object was created for
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #I18N_ERROR_OUT_OF_MEMORY Out of memory
 * @see i18n_ubidi_set_para()
 * @see i18n_ubidi_set_line()
 */
int i18n_ubidi_get_text(const i18n_ubidi_h ubidi, char **text);

/**
 * @brief Gets the visual position from a logical text position.
 * @details If such a mapping is used many times on the same #i18n_ubidi_h object, then calling
 *			i18n_ubidi_get_logical_map() is more efficient.
 *
 *			The value returned may be #I18N_UBIDI_MAP_NOWHERE if there is no visual position because
 *			the corresponding text character is an ubidi control removed from output by the option
 *			#I18N_UBIDI_OPTION_REMOVE_CONTROLS.
 *
 *			When the visual output is altered by using options of i18n_ubidi_write_reordered()
 *			such as #I18N_UBIDI_INSERT_LRM_FOR_NUMERIC, #I18N_UBIDI_KEEP_BASE_COMBINING,
 *			#I18N_UBIDI_OUTPUT_REVERSE, #I18N_UBIDI_REMOVE_BIDI_CONTROLS, the visual position
 *			returned may not be correct.
 *			It is advised to use, when possible, reordering options such as
 *			#I18N_UBIDI_OPTION_INSERT_MARKS and #I18N_UBIDI_OPTION_REMOVE_CONTROLS.
 *
 *			Note that in right-to-left runs, this mapping places second surrogates before first ones
 *			(which is generally a bad idea) and combining characters before base characters. Use of
 *			i18n_ubidi_write_reordered(), optionally with the #I18N_UBIDI_KEEP_BASE_COMBINING
 *			option can be considered instead of using the mapping, in order to avoid these issues.
 * @since_tizen 3.0
 * @param[in]		ubidi			  The paragraph or line #i18n_ubidi_h object
 * @param[in]		logical_index	  The index of a character in the text
 * @param[out]		visual_index	  The visual position of this character
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_logical_map()
 * @see i18n_ubidi_get_logical_index()
 * @see i18n_ubidi_get_processed_length()
 */
int i18n_ubidi_get_visual_index(i18n_ubidi_h ubidi, int32_t logical_index, int32_t *visual_index);

/**
 * @brief Gets a visual-to-logical index map (array) for the characters in the #i18n_ubidi_h
 *			(paragraph or line) object.
 * @details Some values in the map may be #I18N_UBIDI_MAP_NOWHERE if the corresponding text
 *			characters are ubidi marks inserted in the visual output by the option
 *			#I18N_UBIDI_OPTION_INSERT_MARKS.
 *
 *			When the visual output is altered by using options of i18n_ubidi_write_reordered()
 *			such as #I18N_UBIDI_INSERT_LRM_FOR_NUMERIC, #I18N_UBIDI_KEEP_BASE_COMBINING,
 *			#I18N_UBIDI_OUTPUT_REVERSE, #I18N_UBIDI_REMOVE_BIDI_CONTROLS, the logical positions
 *			returned may not be correct. It is advised to use, when possible, reordering options
 *			such as #I18N_UBIDI_OPTION_INSERT_MARKS and #I18N_UBIDI_OPTION_REMOVE_CONTROLS.
 * @since_tizen 3.0
 *
 * @param[in]		 ubidi			 The paragraph or line #i18n_ubidi_h object
 * @param[out]		 index_map		 Pointer to an array of i18n_ubidi_get_result_length() indexes
 *									 which will reflect the reordering of the characters. If option
 *									 #I18N_UBIDI_OPTION_REMOVE_CONTROLS is set, the number of
 *									 elements allocated in @a index_map must be no less than
 *									 i18n_ubidi_get_processed_length(). The array does not need to
 *									 be initialized.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_logical_map()
 * @see i18n_ubidi_get_logical_index()
 * @see i18n_ubidi_get_processed_length()
 * @see i18n_ubidi_get_result_length()
 */
int i18n_ubidi_get_visual_map(i18n_ubidi_h ubidi, int32_t *index_map);

/**
 * @brief Gets one run's logical start, length, and directionality, which can be 0 for LTR or 1 for
 *		  RTL.
 * @details In an RTL run, the character at the logical start is visually on the right of the
 *			displayed run.
 *			The @a length is the number of characters in the run.
 *			i18n_ubidi_count_runs() should be called before the runs are retrieved.
 *
 *			Note that in right-to-left runs, code like this places second surrogates before first
 *			ones (which is generally a bad idea) and combining characters before base characters.
 *
 *			Use of i18n_ubidi_write_reordered(), optionally with the #I18N_UBIDI_KEEP_BASE_COMBINING
 *			option, can be considered in order to avoid these issues.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi			  The paragraph or line #i18n_ubidi_h object
 * @param[in]		run_index		  The number of the run in visual order, in the range
 *									  [0..i18n_ubidi_count_runs(@a ubidi)-1]
 * @param[out]		logical_index	  The first logical character index in the text. The pointer
 *									  may be @c NULL if this index is not needed
 * @param[out]		length			  The number of characters (at least one) in the run.
 *									  The pointer may be @c NULL if this is not needed.
 * @param[out]		direction		  The directionality of the run, #I18N_UBIDI_LTR == 0 or
 *									  #I18N_UBIDI_RTL == 1, never #I18N_UBIDI_MIXED, never
 *									  #I18N_UBIDI_NEUTRAL.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_count_runs()
 */
int i18n_ubidi_get_visual_run(i18n_ubidi_h ubidi,
							  int32_t run_index,
							  int32_t *logical_index,
							  int32_t *length,
							  i18n_ubidi_direction_e *direction);

/**
 * @brief Inverts an index map.
 * @details The index mapping of the first map is inverted and written to the second one.
 * @since_tizen 3.0
 * @remarks The @a dest_map array should be provided by the user.
 *
 * @param[in]		src_map		  An array with @a length elements which defines the original mapping
 *								  from a source array containing @a length elements to a destination
 *								  array. Some elements of the source array may have no mapping in
 *								  the destination array. In that case, their value will be the
 *								  special value #I18N_UBIDI_MAP_NOWHERE.
 *								  All elements must be >=0 or equal to #I18N_UBIDI_MAP_NOWHERE.
 *								  Some elements may have a value >= length, if the destination array
 *								  has more elements than the source array. There must be no
 *								  duplicate indexes (two or more elements with the same value
 *								  except #I18N_UBIDI_MAP_NOWHERE).
 * @param[in]		length		  The length of each array
 * @param[out]		dest_map	  An array with a number of elements equal to 1 + the highest
 *								  value in @a src_map. @a dest_map will be filled with the inverse
 *								  mapping. If element with index i in @a src_map has a value k
 *								  different from #I18N_UBIDI_MAP_NOWHERE, this means that element i
 *								  of the source array maps to element k in the destination array.
 *								  The inverse map will have value i in its k-th element.
 *								  For all elements of the destination array which do not map to an
 *								  element in the source array, the corresponding element in the
 *								  inverse map will have a value equal to #I18N_UBIDI_MAP_NOWHERE.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see I18N_UBIDI_MAP_NOWHERE
 */
int i18n_ubidi_invert_map(const int32_t *src_map, int32_t length, int32_t *dest_map);

/**
 * @brief Gets whether the given #i18n_ubidi_h object is set to perform the inverse ubidi algorithm.
 * @details Note: calling this function after setting the reordering mode with
 *			i18n_ubidi_set_reordering_mode() will return @c true if the reordering mode was set
 *			to #I18N_UBIDI_REORDER_INVERSE_NUMBERS_AS_L, @c false for all other values.
 * @since_tizen 3.0
 *
 * @param[in]		 ubidi		   An #i18n_ubidi_h object
 * @param[out]		 is_inverse	   @c true if the @a ubidi object is set to perform the inverse ubidi
 *								   algorithm by handling numbers as L
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_inverse()
 * @see i18n_ubidi_set_reordering_mode()
 */
int i18n_ubidi_is_inverse(i18n_ubidi_h ubidi, i18n_ubool *is_inverse);

/**
 * @brief Gets whether the given #i18n_ubidi_h object is set to allocate level 0 to block separators.
 * @details This function gets the information whether the given #i18n_ubidi_h object is set to allocate level 0 to block separators.
 * so that successive paragraphs progress from left to right.
 * @since_tizen 3.0
 *
 * @param[in]		 ubidi		   An #i18n_ubidi_h object
 * @param[out]		 is_order	   @c true if the @a ubidi object is set to allocate level 0 to block
 *								   separators
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_order_paragraphs_ltr()
 */
int i18n_ubidi_is_order_paragraphs_ltr(i18n_ubidi_h ubidi, i18n_ubool *is_order);

/**
 * @brief Creates an ubidi object.
 * @details Such an object is initially empty. It is assigned the ubidi properties of a piece of text
 *			containing one or more paragraphs by i18n_ubidi_set_para() or the ubidi properties of
 *			a line within a paragraph by i18n_ubidi_set_line().
 *
 *			This object can be reused for as long as it is not deallocated by calling
 *			i18n_ubidi_destroy().
 *
 *			i18n_ubidi_set_para() and i18n_ubidi_set_line() will allocate additional memory for
 *			internal structures as necessary.
 * @since_tizen 3.0
 * @remarks @a ubidi should be destroyed with i18n_ubidi_destroy().
 *
 * @param[out]		 ubidi		   An empty #i18n_ubidi_h object
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_create(i18n_ubidi_h *ubidi);

/**
 * @brief Creates an ubidi structure with preallocated memory for internal structures.
 * @details This function provides an ubidi object like i18n_ubidi_create() with no arguments,
 *			but it also preallocates memory for internal structures according to the sizings
 *			supplied by the caller.
 *
 *			Subsequent functions will not allocate any more memory, and are thus guaranteed not
 *			to fail because of lack of memory.
 *
 *			The preallocation can be limited to some of the
 *			internal memory by setting some values to 0 here. That means that if, e.g.,
 *			@a max_run_count cannot be reasonably predetermined and should not be set to @a max_length
 *			(the only failproof value) to avoid wasting memory, then @a max_run_count could be set to
 *			0 here and the internal structures that are associated with it will be allocated on
 *			demand, just like with i18n_ubidi_create().
 * @since_tizen 3.0
 * @remarks @a ubidi should be destroyed with i18n_ubidi_destroy().
 *
 * @param[in]		max_length		 The maximum text or line length that internal memory will be
 *									 preallocated for. An attempt to associate this object with a
 *									 longer text will fail, unless this value is 0, which
 *									 leaves the allocation up to the implementation.
 * @param[in]		max_run_count	 The maximum anticipated number of same-level runs that internal
 *									 memory will be preallocated for. An attempt to access visual
 *									 runs on an object that was not preallocated for as many runs
 *									 as the text was actually resolved to will fail, unless this
 *									 value is 0, which leaves the allocation up to the
 *									 implementation.
 *									 The number of runs depends on the actual text and maybe
 *									 anywhere between 1 and @a max_length. It is typically small
 * @param[out]		ubidi			 An empty #i18n_ubidi_h handle with preallocated memory
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_create_sized(int32_t max_length, int32_t max_run_count, i18n_ubidi_h *ubidi);

/**
 * @brief Sets whether block separators must be allocated level zero, so that successive
 *			paragraphs will progress from left to right.
 * @details This function must be called before i18n_ubidi_set_para(). Paragraph separators (B) may
 *			appear in the text. Setting them to level zero means that all paragraph separators
 *			(including one possibly appearing in the last text position) are kept in the reordered
 *			text after the text that they follow in the source text. When this feature is not
 *			enabled, a paragraph separator at the last position of the text before reordering will
 *			go to the first position of the reordered text when the paragraph level is odd.
 * @since_tizen 3.0
 * @param[in]		ubidi				  An #i18n_ubidi_h object
 * @param[in]		order_paragraphs_ltr  Specifies whether paragraph separators (B) must receive
 *										  level 0, so that successive paragraphs progress from left
 *										  to right
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_para()
 */
int i18n_ubidi_order_paragraphs_ltr(i18n_ubidi_h ubidi, i18n_ubool order_paragraphs_ltr);

/**
 * @brief Performs logical reordering.
 * @details This is a convenience function that does not use an #i18n_ubidi_h object.
 *			It is intended to be used for when an application has determined the levels of objects
 *			(character sequences) and just needs to have them reordered (L2). This is equivalent to
 *			using i18n_ubidi_get_logical_map() on an #i18n_ubidi_h object.
 *
 *			The index map will result in index_map[logical_index] == visual_index.
 * @since_tizen 3.0
 *
 * @param[in]		levels		  An array with @a length levels that have been determined by the
 *								  application
 * @param[in]		length		  The number of levels in the array, or, semantically, the number
 *								  of objects to be reordered. @a length must be > 0.
 * @param[out]		index_map	  Pointer to an array of @a length indexes which will reflect the
 *								  reordering of the characters. The array does not need to be
 *								  initialized.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_reorder_logical(const i18n_ubidi_level_t *levels, int32_t length,
							   int32_t *index_map);

/**
 * @brief Performs visual reordering.
 * @details This is a convenience function that does not use an #i18n_ubidi_h object.
 *			It is intended to be used for when an application has determined the levels of objects
 *			(character sequences) and just needs to have them reordered (L2). This is equivalent
 *			to using i18n_ubidi_get_visual_map() on an #i18n_ubidi_h handle.
 *			The index map will result in @a index_map[visual_index] == logical_index.
 * @since_tizen 3.0
 *
 * @param[in]		levels		  An array with @a length levels that have been determined by the
 *								  application
 * @param[in]		length		  The number of levels in the array, or, semantically, the number
 *								  of objects to be reordered. @a length must be > 0.
 * @param[out]		index_map	  Pointer to an array of @a length indexes which will reflect the
 *								  reordering of the characters. The array does not need to be
 *								  initialized.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_reorder_visual(const i18n_ubidi_level_t *levels, int32_t length, int32_t *index_map);

/**
 * @brief Sets the callback function and callback data used by the UBA implementation for ubidi class
 *			determination.
 * @details This may be useful for assigning ubidi classes to PUA characters, or for special
 *			application needs. For instance, an application may want to handle all spaces like L
 *			or R characters (according to the base direction) when creating the visual ordering of
 *			logical lines which are part of a report organized in columns: there should not be
 *			interaction between adjacent cells.
 * @since_tizen 3.0
 * @remarks The @a old_fn and @a old_context should not be released.
 *
 * @param[in]		ubidi			 The paragraph #i18n_ubidi_h object
 * @param[in]		new_fn			 The new callback function pointer
 * @param[in]		new_context		 The new callback context pointer. This can be @c NULL.
 * @param[out]		old_fn			 The old callback function pointer. This can be @c NULL.
 * @param[out]		old_context		 The old callback's context. This can be @c NULL.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_class_cb()
 */
int i18n_ubidi_set_class_cb(i18n_ubidi_h ubidi,
							i18n_ubidi_class_cb new_fn,
							const void *new_context,
							i18n_ubidi_class_cb *old_fn,
							const void **old_context);
/**
 * @brief Sets the context before a call to i18n_ubidi_set_para().
 * @details i18n_ubidi_set_para() computes the left-right directionality for a given piece of text
 *			which is supplied as one of its arguments. Sometimes this piece of text (the "main
 *			text") should be considered in context, because text appearing before ("prologue")
 *			and/or after ("epilogue") the main text may affect the result of this computation.
 *
 *			This function specifies the prologue and/or the epilogue for the next call to
 *			i18n_ubidi_set_para(). The characters specified as prologue and epilogue should not
 *			be modified by the calling program until the call to i18n_ubidi_set_para() has returned.
 *			If successive calls to i18n_ubidi_set_para() all need specification of a context,
 *			i18n_ubidi_set_context() must be called before each call to i18n_ubidi_set_para().
 *			In other words, a context is not "remembered" after the following successful call to
 *			i18n_ubidi_set_para().
 *
 *			If a call to i18n_ubidi_set_para() specifies #I18N_UBIDI_DEFAULT_LTR or
 *			#I18N_UBIDI_DEFAULT_RTL as para_level and is preceded by a call to
 *			i18n_ubidi_set_context() which specifies a prologue, the paragraph level will be
 *			computed taking into consideration the text in the prologue.
 *
 *			When i18n_ubidi_set_para() is called without a previous call to i18n_ubidi_set_context(),
 *			the main text is handled as if preceded and followed by strong directional characters
 *			at the current paragraph level. Calling i18n_ubidi_set_context() with specification of
 *			a prologue will change this behavior by handling the main text as if preceded by the
 *			last strong character appearing in the prologue, if any. Calling
 *			i18n_ubidi_set_context() with specification of an epilogue will change the behavior of
 *			i18n_ubidi_set_para() by handling the main text as if followed by the first strong
 *			character or digit appearing in the epilogue, if any.
 *
 *			Note 1: if i18n_ubidi_set_context() is called repeatedly without calling
 *			i18n_ubidi_set_para(), the earlier calls have no effect, only the last call will be
 *			remembered for the next call to i18n_ubidi_set_para().
 *
 *			Note 2: calling i18n_ubidi_set_context(ubidi, NULL, 0, NULL, 0) cancels any previous
 *			setting of non-empty prologue or epilogue. The next call to i18n_ubidi_set_para() will
 *			process no prologue or epilogue.
 *
 *			Note 3: users must be aware that even after setting the context before a call to
 *			i18n_ubidi_set_para() to perform e.g. a logical to visual transformation, the resulting
 *			string may not be identical to what it would have been if all the text, including
 *			prologue and epilogue, had been processed together.
 *			Example (upper case letters represent RTL characters):
 *
 *			  prologue = <code>"abc DE"</code>\n
 *			  epilogue = none\n
 *			  main text = <code>"FGH xyz"</code>\n
 *			  para_level = I18N_UBIDI_LTR\n
 *			  display without prologue = <code>"HGF xyz"</code> ("HGF" is adjacent to "xyz")\n
 *			  display with prologue = <code>"abc HGFED xyz"</code> ("HGF" is not adjacent to "xyz")\n
 * @since_tizen 3.0
 *
 * @param[in]		ubidi		 A paragraph #i18n_ubidi_h object
 * @param[in]		prologue	 Pointer to the text which precedes the text that will be specified
 *								 in a coming call to i18n_ubidi_set_para(). If there is no prologue
 *								 to consider, then @a pro_length must be zero and this pointer can be
 *								 @c NULL.
 * @param[in]		pro_length	 The length of the @a prologue; if @a pro_length == -1 then the prologue
 *								 must be zero-terminated. Otherwise @a pro_length must be >= 0.
 *								 If @a pro_length == 0, it means that there is no prologue to consider.
 * @param[in]		epilogue	 A pointer to the text which follows the text that will be specified
 *								 in a coming call to i18n_ubidi_set_para(). If there is no epilogue
 *								 to consider, then @a epi_length must be zero and this pointer can be
 *								 @c NULL.
 * @param[in]		epi_length	 The length of the @a epilogue; if @a epi_length == -1 then the epilogue
 *								 must be zero-terminated. Otherwise @a epi_length must be >= 0.
 *								 If @a epi_length == 0, it means that there is no epilogue to consider.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_para()
 */
int i18n_ubidi_set_context(i18n_ubidi_h ubidi,
						   const i18n_uchar *prologue,
						   int32_t pro_length,
						   const i18n_uchar *epilogue,
						   int32_t epi_length);

/**
 * @brief Modifies the operation of the ubidi algorithm such that it approximates an "inverse ubidi"
 *			algorithm.
 * @details This function must be called before i18n_ubidi_set_para().
 *
 *			The normal operation of the ubidi algorithm as described in the Unicode Technical Report
 *			is to take text stored in logical (keyboard, typing) order and to determine the
 *			reordering of it for visual rendering. Some legacy systems store text in visual order,
 *			and for operations with standard, Unicode-based algorithms, the text needs to be
 *			transformed to logical order. This is effectively the inverse algorithm of the
 *			described ubidi algorithm.
 *			Note that there is no standard algorithm for this "inverse ubidi" and that the current
 *			implementation provides only an approximation of "inverse ubidi".
 *
 *			With @a is_inverse set to @c true, this function changes the behavior of some of the
 *			subsequent functions in a way that they can be used for the inverse ubidi algorithm.
 *			Specifically, runs of text with numeric characters will be treated in a special way and
 *			may need to be surrounded with LRM characters when they are written in reordered
 *			sequence.
 *
 *			Output runs should be retrieved using i18n_ubidi_get_visual_run(). Since the actual
 *			input for "inverse ubidi" is visually ordered text and i18n_ubidi_get_visual_run() gets
 *			the reordered runs, these are actually the runs of the logically ordered output.
 *			Calling this function with argument @a is_inverse set to @c true is equivalent to calling
 *			i18n_ubidi_set_reordering_mode() with argument reordering_mode set to
 *			#I18N_UBIDI_REORDER_INVERSE_NUMBERS_AS_L.
 *
 *			Calling this function with argument @a is_inverse set to @c false is equivalent to calling
 *			i18n_ubidi_set_reordering_mode() with argument reordering_mode set to
 *			#I18N_UBIDI_REORDER_DEFAULT.
 * @since_tizen 3.0
 *
 * @param[in]	   ubidi		  An #i18n_ubidi_h object
 * @param[in]	   is_inverse	  Specifies "forward" or "inverse" ubidi operation
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_para()
 * @see i18n_ubidi_write_reordered()
 * @see i18n_ubidi_set_reordering_mode()
 */
int i18n_ubidi_set_inverse(i18n_ubidi_h ubidi, i18n_ubool is_inverse);

/**
 * @brief Sets an #i18n_ubidi_h object to contain the reordering information, especially the resolved levels,
 *			for all the characters in a line of text.
 * @details This line of text is specified by referring to an #i18n_ubidi_h object representing this
 *			information for a piece of text containing one or more paragraphs, and by specifying
 *			a range of indexes in this text.
 *
 *			In the new line object, the indexes will range from 0 to @a limit - @a start - 1.
 *
 *			This is used after calling i18n_ubidi_set_para() for a piece of text, and after
 *			line-breaking on that text. It is not necessary if each paragraph is treated as a
 *			single line.
 *
 *			After line-breaking, rules (L1) and (L2) for the treatment of trailing WS and for
 *			reordering are performed on an #i18n_ubidi_h object that represents a line.
 *
 *			Important: @a line_bidi shares data with @a para_bidi. You must destroy or reuse
 *			@a line_bidi before @a para_bidi. In other words, you must destroy or reuse the
 *			#i18n_ubidi_h object for a line before the object for its parent paragraph.
 *
 *			The text pointer that was stored in @a para_bidi is also copied, and @a start is added to
 *			it so that it points to the beginning of the line for this object.
 * @since_tizen 3.0
 *
 * @param[in]		para_bidi		  The parent paragraph object. It must have been set by a
 *									  successful call to i18n_ubidi_set_para()
 * @param[in]		start			  The line's first index into the text
 * @param[in]		limit			  The index after the last line's index (its last index + 1)
 * @param[in]		line_bidi		  The object that will now represent a line of the text
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_set_para()
 * @see i18n_ubidi_get_processed_length()
 */
int i18n_ubidi_set_line(const i18n_ubidi_h para_bidi, int32_t start, int32_t limit,
						i18n_ubidi_h line_bidi);

/**
 * @brief Performs the Unicode bidi algorithm.
 * @details It is defined in the Unicode Standard Annex #9, version 13, also described in The
 *			Unicode Standard, Version 4.0 .
 *
 *			This function takes a piece of plain text containing one or more paragraphs, with or
 *			without externally specified embedding levels from styled text and computes the
 *			left-right-directionality of each character.
 *
 *			If the entire text is all of the same directionality, then the function may not perform
 *			all the steps described by the algorithm, i.e., some levels may not be the same as if
 *			all steps were performed. This is not relevant for unidirectional text.
 *			For example, in pure LTR text with numbers the numbers would get a resolved level of 2
 *			higher than the surrounding text according to the algorithm. This implementation may
 *			set all resolved levels to the same value in such a case.
 *
 *			The text can be composed of multiple paragraphs. Occurrence of a block separator in
 *			the text terminates a paragraph, and whatever comes next starts a new paragraph.
 *			The exception to this rule is when a Carriage Return (CR) is followed by a Line Feed
 *			(LF). Both CR and LF are block separators, but in that case, the pair of characters is
 *			considered as terminating the preceding paragraph, and a new paragraph will be started
 *			by a character coming after the LF.
 * @since_tizen 3.0
 *
 * @param[in]	   ubidi			An #i18n_ubidi_h object allocated with i18n_ubidi_create() which
 *									will be set to contain the reordering information, especially
 *									the resolved levels for all the characters in the @a text
 * @param[in]	   text				A pointer to the text that the ubidi algorithm will be performed
 *									on. This pointer is stored in the #i18n_ubidi_h handle and can
 *									be retrieved with i18n_ubidi_get_text().
 *									Note: The @a text must be (at least) @a length long.
 * @param[in]	   length			The length of the @a text; if @a length == -1 then the text must be
 *									zero-terminated
 * @param[in]	   para_level		Specifies the default level for the @a text; it is typically 0
 *									(LTR) or 1 (RTL). If the function shall determine the paragraph
 *									level from the text, then @a para_level can be set to either
 *									#I18N_UBIDI_DEFAULT_LTR or #I18N_UBIDI_DEFAULT_RTL; if the text
 *									contains multiple paragraphs, the paragraph level shall be
 *									determined separately for each paragraph; if a paragraph does
 *									not include any strongly typed character, then the desired
 *									default is used (0 for LTR or 1 for RTL). Any other value
 *									between 0 and #I18N_UBIDI_MAX_EXPLICIT_LEVEL is also valid, with
 *									odd levels indicating RTL.
 * @param[in]	   embedding_levels May be used to preset the embedding and override levels,
 *									ignoring characters like LRE and PDF in the text.
 *									A level overrides the directional property of its corresponding
 *									(same index) character if the level has the
 *									#I18N_UBIDI_LEVEL_OVERRIDE bit set.
 *									Caution: A copy of this pointer, not of the levels, will be stored in the
 *									#i18n_ubidi_h object; the @a embedding_levels array must not be
 *									deallocated before the #i18n_ubidi_h structure is destroyed or reused,
 *									and the @a embedding_levels should not be modified to avoid unexpected
 *									results on subsequent ubidi operations. However, the i18n_ubidi_set_para()
 *									and i18n_ubidi_set_line() functions may modify some or all of the levels.
 *
 *									After the #i18n_ubidi_h object is reused or destroyed, the caller must
 *									take care of the deallocation of the @a embedding_levels array.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_set_para(i18n_ubidi_h ubidi,
						const i18n_uchar *text,
						int32_t length,
						i18n_ubidi_level_t para_level,
						i18n_ubidi_level_t *embedding_levels);

/**
 * @brief Modifies the operation of the ubidi algorithm such that it implements some variant to the
 *			basic ubidi algorithm or approximates an "inverse ubidi" algorithm, depending on different
 *			values of the "reordering mode".
 * @details This function must be called before i18n_ubidi_set_para(), and stays in effect until
 *			called again with a different argument.
 *
 *			The normal operation of the ubidi algorithm as described in the Unicode Standard Annex
 *			#9 is to take text stored in logical (keyboard, typing) order and to determine how to
 *			reorder it for visual rendering.
 *
 *			With the @a reordering_mode set to a value other than #I18N_UBIDI_REORDER_DEFAULT, this
 *			function changes the behavior of some of the subsequent functions in a way such that
 *			they implement an inverse ubidi algorithm or some other algorithm variants.
 *
 *			Some legacy systems store text in visual order, and for operations with standard,
 *			Unicode-based algorithms, the text needs to be transformed into logical order. This is
 *			effectively the inverse algorithm of the described ubidi algorithm.
 *			Note that there is no standard algorithm for this "inverse ubidi", so a number of
 *			variants are implemented here.
 *
 *			In other cases, it may be desirable to emulate some variant of the Logical to Visual
 *			algorithm (e.g. one used in MS Windows), or perform a Logical to Logical transformation.
 *				- When the @a reordering_mode is set to #I18N_UBIDI_REORDER_DEFAULT, the standard ubidi
 *				Logical to Visual algorithm is applied.
 *				- When the @a reordering_mode is set to #I18N_UBIDI_REORDER_NUMBERS_SPECIAL, the
 *				algorithm used to perform ubidi transformations when calling i18n_ubidi_set_para()
 *				should approximate the algorithm used in Microsoft Windows XP rather than strictly
 *				conform to the Unicode bidi algorithm. The differences between the basic algorithm
 *				and the algorithm addressed by this option are as follows:
 *					- Within text at an even embedding level, the sequence "123AB" (where AB
 *					represent R or AL letters) is transformed to "123BA" by the Unicode algorithm
 *					and to "BA123" by the Windows algorithm.
 *					- Arabic-Indic numbers (AN) are handled by the Windows algorithm just like
 *					regular numbers (EN).
 *				- When the @a reordering_mode is set to #I18N_UBIDI_REORDER_GROUP_NUMBERS_WITH_R,
 *				numbers located between LTR text and RTL text are associated with the RTL text.
 *				For instance, an LTR paragraph with content "abc 123 DEF" (where upper case letters
 *				represent RTL characters) will be transformed to "abc FED 123" (and not "abc 123
 *				FED"), "DEF 123 abc" will be transformed to "123 FED abc" and "123 FED abc"
 *				will be transformed to "DEF 123 abc". This makes the algorithm reversible and makes
 *				it useful when round trip (from visual to logical and back to visual) must be
 *				achieved without adding LRM characters. However, this is a variation from the
 *				standard Unicode bidi algorithm. The source text should not contain ubidi control
 *				characters other than LRM or RLM.
 *				- When the @a reordering_mode is set to #I18N_UBIDI_REORDER_RUNS_ONLY, a "Logical to
 *				Logical" transformation must be performed:
 *					- If the default text level of the source text (argument para_level in
 *					i18n_ubidi_set_para() ) is even, the source text will be handled as LTR logical
 *					text and will be transformed to the RTL logical text which has the same LTR
 *					visual display.
 *					- If the default level of the source text is odd, the source text will be
 *					handled as RTL logical text and will be transformed to the LTR logical text
 *					which has the same LTR visual display.
 *				This mode may be needed when logical text which is basically Arabic or Hebrew, with
 *				possible included numbers or phrases in English, has to be displayed as if it had
 *				an even embedding level (this can happen if the displaying application treats all
 *				text as if it was basically LTR).
 *				This mode may also be needed in the reverse case, when logical text which is
 *				basically English, with possible included phrases in Arabic or Hebrew, has to be
 *				displayed as if it had an odd embedding level. Both cases could be handled by
 *				adding LRE or RLE at the head of the text, if the display subsystem supports these
 *				formatting controls. If it does not, the problem may be handled by transforming
 *				the source text in this mode before displaying it, so that it will be displayed
 *				properly.
 *
 *				The source text should not contain ubidi control characters other than
 *				LRM or RLM.
 *				- When the @a reordering_mode is set to #I18N_UBIDI_REORDER_INVERSE_NUMBERS_AS_L, an
 *				"inverse ubidi" algorithm is applied. Runs of text with numeric characters will be
 *				treated like LTR letters and may need to be surrounded with LRM characters when
 *				they are written in reordered sequence (the option #I18N_UBIDI_INSERT_LRM_FOR_NUMERIC
 *				can be used with function i18n_ubidi_write_reordered() to this end. This mode is
 *				equivalent to calling i18n_ubidi_set_inverse() with argument is_inverse set to @c true.
 *				- When the @a reordering_mode is set to #I18N_UBIDI_REORDER_INVERSE_LIKE_DIRECT, the
 *				"direct" Logical to Visual Ubidi algorithm is used as an approximation of an
 *				"inverse ubidi" algorithm. This mode is similar to mode
 *				#I18N_UBIDI_REORDER_INVERSE_NUMBERS_AS_L but is closer to the regular ubidi algorithm.
 *				For example, an LTR paragraph with the content "FED 123 456 CBA" (where upper case
 *				represents RTL characters) will be transformed to "ABC 456 123 DEF", as opposed to
 *				"DEF 123 456 ABC" with mode #I18N_UBIDI_REORDER_INVERSE_NUMBERS_AS_L.
 *				When used in conjunction with option #I18N_UBIDI_OPTION_INSERT_MARKS, this mode
 *				generally adds ubidi marks to the output significantly more sparingly than mode
 *				#I18N_UBIDI_REORDER_INVERSE_NUMBERS_AS_L with option
 *				#I18N_UBIDI_INSERT_LRM_FOR_NUMERIC in calls to i18n_ubidi_write_reordered().
 *				- When the reordering mode is set to #I18N_UBIDI_REORDER_INVERSE_FOR_NUMBERS_SPECIAL,
 *				the Logical to Visual ubidi algorithm used in Windows XP is used as an approximation
 *				of an "inverse ubidi" algorithm.
 *				For example, an LTR paragraph with the content "abc FED123" (where upper case
 *				represents RTL characters) will be transformed to "abc 123DEF."
 *
 *			In all the reordering modes specifying an "inverse ubidi" algorithm (i.e. those with
 *			a name starting with I18N_UBIDI_REORDER_INVERSE), output runs should be retrieved
 *			using i18n_ubidi_get_visual_run(), and the output text with
 *			i18n_ubidi_write_reordered(). The caller should keep in mind that in
 *			"inverse ubidi" modes the input is actually visually ordered text and reordered
 *			output returned by i18n_ubidi_get_visual_run() or i18n_ubidi_write_reordered() are
 *			actually runs or character string of logically ordered output.
 *			For all the "inverse ubidi" modes, the source text should not contain ubidi control
 *			characters other than LRM or RLM.
 *
 *			Note that option #I18N_UBIDI_OUTPUT_REVERSE of i18n_ubidi_write_reordered() has no useful
 *			meaning and should not be used in conjunction with any value of the @a reordering_mode
 *			specifying "inverse ubidi" or with value #I18N_UBIDI_REORDER_RUNS_ONLY.
 * @since_tizen 3.0
 *
 * @param[in]		  ubidi				  An #i18n_ubidi_h object
 * @param[in]		  reordering_mode	  Specifies the required variant of the ubidi algorithm
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_reordering_mode_e
 * @see i18n_ubidi_set_inverse()
 * @see i18n_ubidi_set_para()
 * @see i18n_ubidi_write_reordered()
 */
int i18n_ubidi_set_reordering_mode(i18n_ubidi_h ubidi,
								   i18n_ubidi_reordering_mode_e reordering_mode);

/**
 * @brief Specifies which of the reordering options should be applied during ubidi transformations.
 * @since_tizen 3.0
 *
 * @param[in]		ubidi				  An #i18n_ubidi_h object
 * @param[in]		reordering_options	  A bitwise 'or' combination of zero or more of:
 *										  #I18N_UBIDI_OPTION_DEFAULT, #I18N_UBIDI_OPTION_INSERT_MARKS,
 *										  #I18N_UBIDI_OPTION_REMOVE_CONTROLS,
 *										  #I18N_UBIDI_OPTION_STREAMING.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_reordering_options()
 */
int i18n_ubidi_set_reordering_options(i18n_ubidi_h ubidi, uint32_t reordering_options);

/**
 * @brief Takes an #i18n_ubidi_h object containing the reordering information for a piece of text (one
 *		  or more paragraphs) set by i18n_ubidi_set_para() or for a line of text set by
 *		  i18n_ubidi_set_line() and write a reordered string to the destination buffer.
 * @details This function preserves the integrity of characters with multiple code units and
 *			(optionally) combining characters. Characters in RTL runs can be replaced by
 *			mirror-image characters in the destination buffer. Note that "real" mirroring has to be
 *			done in a rendering engine by glyph selection and that for many "mirrored" characters
 *			there are no Unicode characters as mirror-image equivalents. There are also options to
 *			insert or remove ubidi control characters; see the description of the @a dest_size and
 *			@a options parameters and of the option bit flags.
 * @since_tizen 3.0
 * @remarks The @a dest array should be provided by the user.
 *
 * @param[in]	  ubidi			  An #i18n_ubidi_h object that is set by
 *								  i18n_ubidi_set_para() or i18n_ubidi_set_line() and contains
 *								  the reordering information for the text that it was defined for,
 *								  as well as a pointer to that text.
 * @param[in]	  options		  A bit set of options for the reordering that control how the
 *								  reordered text is written. The options include mirroring the
 *								  characters on a code point basis and inserting LRM characters,
 *								  which is used especially for transforming visually stored text to
 *								  logically stored text (although this is still an imperfect
 *								  implementation of an "inverse ubidi" algorithm because it uses the
 *								  "forward ubidi" algorithm at its core). The available options
 *								  are: #I18N_UBIDI_DO_MIRRORING, #I18N_UBIDI_INSERT_LRM_FOR_NUMERIC,
 *								  #I18N_UBIDI_KEEP_BASE_COMBINING, #I18N_UBIDI_OUTPUT_REVERSE,
 *								  #I18N_UBIDI_REMOVE_BIDI_CONTROLS.
 * @param[in]	  dest_size		  The size of the @a dest buffer, in number of i18n_uchars. If the
 *								  #I18N_UBIDI_INSERT_LRM_FOR_NUMERIC option is set, then the
 *								  destination length could be as large as
 *								  i18n_ubidi_get_length(@a ubidi)+2*i18n_ubidi_count_runs(@a ubidi).
 *								  If the #I18N_UBIDI_REMOVE_BIDI_CONTROLS option is set, then the
 *								  destination length may be less than i18n_ubidi_get_length(@a ubidi).
 *								  If none of these options is set, then the destination length
 *								  will be exactly i18n_ubidi_get_processed_length(@a ubidi).
 * @param[out]	  dest			  A pointer to where the reordered text is to be copied. The
 *								  source text and the destination buffer @a dest must not overlap.
 * @param[out]	  output_length	  The length of the output string
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 * @see i18n_ubidi_get_processed_length()
 */
int i18n_ubidi_write_reordered(i18n_ubidi_h ubidi,
							   uint16_t options,
							   int32_t dest_size,
							   i18n_uchar *dest,
							   int32_t *output_length);

/**
 * @brief Reverses a Right-To-Left run of Unicode text.
 * @details This function preserves the integrity of characters with multiple code units and
 *			(optionally) combining characters. Characters can be replaced by mirror-image
 *			characters in the destination buffer. Note that "real" mirroring has to be done in a
 *			rendering engine by glyph selection and that for many "mirrored" characters there are
 *			no Unicode characters as mirror-image equivalents. There are also options to insert
 *			or remove ubidi control characters.
 *
 *			This function is the implementation for reversing RTL runs as part of
 *			i18n_ubidi_write_reordered(). For detailed descriptions of the parameters, see there.
 *			Since no ubidi controls are inserted here, the output string length will never exceed
 *			@a src_length.
 * @since_tizen 3.0
 *
 * @param[in]		src				   A pointer to the RTL run text
 * @param[in]		src_length		   The length of the RTL run
 * @param[in]		options			   A bit set of options for the reordering that control how the
 *									   reordered text is written. See the options parameter
 *									   in i18n_ubidi_write_reordered().
 * @param[in]		dest_size		   The size of the @a dest buffer, in number of i18n_uchars. If the
 *									   #I18N_UBIDI_REMOVE_BIDI_CONTROLS option is set, then the
 *									   destination length may be less than @a src_length. If this
 *									   option is not set, then the destination length will be
 *									   exactly @a src_length.
 * @param[out]		dest			   A pointer to where the reordered text is to be copied.
 *									   @a src and @a dest arrays (of length @a src_length and @a dest_size,
 *									   respectively) must not overlap.
 * @param[out]		output_length	   The length of the output string
 *
 * @remarks do not free @a dest with free() function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #I18N_ERROR_NONE Successful
 * @retval #I18N_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int i18n_ubidi_write_reverse(const i18n_uchar *src,
							 int32_t src_length,
							 uint16_t options,
							 int32_t dest_size,
							 i18n_uchar *dest,
							 int32_t *output_length);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
