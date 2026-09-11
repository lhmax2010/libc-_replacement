/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Krzysztof Jackiewicz <k.jackiewicz@samsung.com>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */


/**
 * @file yaca_digest.h
 *
 * @brief Advanced API for the message digests.
 */


#ifndef __TIZEN_CORE_YACA_DIGEST_H__
#define __TIZEN_CORE_YACA_DIGEST_H__


#include <stddef.h>
#include <yaca_types.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_YACA_INTEGRITY_MODULE
 * @{
 */


/**
 * @brief Initializes a digest context.
 *
 * @since_tizen 3.0
 *
 * @remarks The @a ctx should be released using yaca_context_destroy().
 *
 * @param[out] ctx Newly created context
 * @param[in] algo Digest algorithm that will be used
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #YACA_ERROR_NONE Successful
 * @retval #YACA_ERROR_INVALID_PARAMETER Required parameters have incorrect values (NULL,
 *                                       invalid @a algo)
 * @retval #YACA_ERROR_OUT_OF_MEMORY Out of memory error
 * @retval #YACA_ERROR_INTERNAL Internal error
 *
 * @see #yaca_digest_algorithm_e
 * @see yaca_digest_update()
 * @see yaca_digest_finalize()
 * @see yaca_context_destroy()
 */
int yaca_digest_initialize(yaca_context_h *ctx,
                           yaca_digest_algorithm_e algo);


/**
 * @brief Feeds the message into the message digest algorithm.
 *
 * @since_tizen 3.0
 *
 * @param[in,out] ctx Context created by yaca_digest_initialize()
 * @param[in] message Message from which the digest is to be calculated
 * @param[in] message_len Length of the message
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #YACA_ERROR_NONE Successful
 * @retval #YACA_ERROR_INVALID_PARAMETER Required parameters have incorrect values (NULL, 0,
 *                                       invalid @a ctx)
 * @retval #YACA_ERROR_INTERNAL Internal error
 *
 * @see yaca_digest_initialize()
 * @see yaca_digest_finalize()
 */
int yaca_digest_update(yaca_context_h ctx,
                       const char *message,
                       size_t message_len);


/**
 * @brief Calculates the final digest.
 *
 * @since_tizen 3.0
 *
 * @remarks Skipping yaca_digest_update() and calling only yaca_digest_finalize() will produce an empty message digest.
 * @remarks The @a digest buffer must be allocated by client (see yaca_context_get_output_length()).
 *
 * @param[in,out] ctx A valid digest context
 * @param[out] digest Buffer for the message digest
 * @param[out] digest_len Length of the digest (the number of returned written bytes)
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #YACA_ERROR_NONE Successful
 * @retval #YACA_ERROR_INVALID_PARAMETER Required parameters have incorrect values (NULL,
 *                                       invalid @a ctx)
 * @retval #YACA_ERROR_INTERNAL Internal error
 *
 * @see yaca_digest_initialize()
 * @see yaca_digest_update()
 * @see yaca_context_get_output_length()
 */
int yaca_digest_finalize(yaca_context_h ctx, char *digest, size_t *digest_len);


/**
 * @}
 */


#ifdef __cplusplus
} /* extern */
#endif


#endif /* __TIZEN_CORE_YACA_DIGEST_H__ */
