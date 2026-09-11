/*
* Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#ifndef __TIZEN_CONTENT_MEDIA_BOOK_H__
#define __TIZEN_CONTENT_MEDIA_BOOK_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_book.h
 * @brief This file contains the book metadata API and related functions to proceed with book metadata. \n
 *        Description of the book content involves: author, publisher, date, and subject.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_BOOK_META_MODULE
 * @{
 */

/**
 * @brief Clones the book metadata.
 * @details This function copies the book metadata handle from a source to destination.
 *
 * @since_tizen 6.5
 *
 * @remarks The @a dst should be released using book_meta_destroy().
 *
 * @param[out] dst The destination handle to the book metadata
 * @param[in] src The source handle to the book metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_info_get_book()
 * @see book_meta_destroy()
 */
int book_meta_clone(book_meta_h *dst, book_meta_h src);

/**
 * @brief Destroys the book metadata.
 * @details This function frees all resources related to the book metadata handle. This handle
 *          can no longer be used to perform any operations. A new handle has to
 *          be created before the next use.
 *
 * @since_tizen 6.5
 *
 * @param[in] book The handle to the book metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see book_meta_clone()
 */
int book_meta_destroy(book_meta_h book);

/**
 * @brief Gets the ID of the media of the given book metadata.
 * @since_tizen 6.5
 *
 * @remarks The @a media_id should be released using free().
 *
 * @param[in] book The handle to the book metadata
 * @param[out] media_id The media ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int book_meta_get_media_id(book_meta_h book, char **media_id);

/**
 * @brief Gets the subject of the given book metadata.
 * @details If there is no information, @a subject will be NULL.
 *
 * @since_tizen 6.5
 *
 * @remarks The @a subject should be released using free().
 *
 * @param[in] book The handle to the book metadata
 * @param[out] subject The subject of the book metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int book_meta_get_subject(book_meta_h book, char **subject);

/**
 * @brief Gets the author of the given book metadata.
 * @details If there is no information, @a author will be NULL.
 *
 * @since_tizen 6.5
 *
 * @remarks The @a author should be released using free().
 *
 * @param[in] book The handle to the book metadata
 * @param[out] author The author of the book metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int book_meta_get_author(book_meta_h book, char **author);

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the publication date of the given book metadata.
 * @details If there is no information, @a date will be NULL.
 *
 * @since_tizen 6.5
 *
 * @remarks The @a date should be released using free().
 *
 * @param[in] book The handle to the book metadata
 * @param[out] date The date of the book metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int book_meta_get_date(book_meta_h book, char **date) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the publisher notice of the given book metadata.
 * @details If there is no information, @a publisher will be NULL.
 *
 * @since_tizen 6.5
 *
 * @remarks The @a publisher should be released using free().
 *
 * @param[in] book The handle to the book metadata
 * @param[out] publisher The publisher of the book metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int book_meta_get_publisher(book_meta_h book, char **publisher) TIZEN_DEPRECATED_API;

/**
 * @brief Gets a list of paths to ebooks which contain a given keyword.
 * @details This function returns a list of ebook paths including @a keyword.\n
 *          The search scope is title, table of contents, and body.\n
 *          If there are no ebooks matching the criteria, @a path_list will be NULL.
 *
 * @since_tizen 6.5
 *
 * @remarks Each element of @a path_list should be released with free(), then the array itself should be released with free(). \n
 *          %http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage. \n
 *          %http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
 *
 * @param[in] keyword Keyword to search for
 * @param[out] path_list A list of paths to books containing @a keyword
 * @param[out] len Length of @a path_list
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 *
 * @see media_info_get_media_from_db_by_path()
 */
int book_meta_get_path_with_keyword(const char *keyword, char ***path_list, unsigned int *len);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__TIZEN_CONTENT_MEDIA_BOOK_H__*/
