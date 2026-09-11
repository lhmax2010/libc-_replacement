/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_APPFW_CION_PAYLOAD_ASYNC_RESULT_H__
#define __TIZEN_APPFW_CION_PAYLOAD_ASYNC_RESULT_H__

#include <cion_peer_info.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_MODULE
 * @{
 */

/**
 * @brief The Cion payload async result handle.
 * @since_tizen 6.5
 */
typedef void *cion_payload_async_result_h;

/**
 * @brief Enumeration for payload async result types.
 * @since_tizen 6.5
 */
typedef enum _payload_async_result_e {
  CION_PAYLOAD_ASYNC_RESULT_PENDING, /**< Pending **/
  CION_PAYLOAD_ASYNC_RESULT_SUCCESS, /**< Success **/
  CION_PAYLOAD_ASYNC_RESULT_FAIL, /**< Fail **/
} cion_payload_async_result_e;

/**
 * @brief Creates a clone of payload async result.
 * @since_tizen 6.5
 * @remarks @a result_clone must be released using cion_payload_async_result_destroy()
 * @param[in] result The payload async result handle
 * @param[out] result_clone The cloned handle of payload async result
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see cion_payload_async_result_destroy()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    cion_payload_async_result_h result_clone;

    ret = cion_payload_async_result_clone(result, &result_clone);
}
 * @endcode
 */
int cion_payload_async_result_clone(const cion_payload_async_result_h result,
        cion_payload_async_result_h *result_clone);

/**
 * @brief Destroys the payload async result.
 * @since_tizen 6.5
 * @param[in] result The payload async result handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_payload_async_result_clone()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_payload_async_result_destroy(result);
}
 * @endcode
 */
int cion_payload_async_result_destroy(cion_payload_async_result_h result);

/**
 * @brief Gets the result status from payload async result handle.
 * @since_tizen 6.5
 * @param[in] result The payload async result handle
 * @param[out] code The result state of payload async result
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #cion_payload_async_result_e
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    cion_payload_async_result_e code;

    ret = cion_payload_async_result_get_result(result, &code);
}
 * @endcode
 */
int cion_payload_async_result_get_result(
        const cion_payload_async_result_h result,
        cion_payload_async_result_e *code);

/**
 * @brief Gets the peer information from async result.
 * @since_tizen 6.5
 * @remarks @a peer_info must be released using cion_peer_info_destroy().
 * @param[in] result The result of payload async
 * @param[out] peer_info The Cion peer information handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CION_ERROR_IO_ERROR IO error
 * @see cion_peer_info_destroy()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    cion_peer_info_h peer_info;

    ret = cion_payload_async_result_get_peer_info(result, &peer_info);
}
 * @endcode
 */
int cion_payload_async_result_get_peer_info(
        const cion_payload_async_result_h result,
        cion_peer_info_h *peer_info);

/**
 * @brief Gets the payload ID.
 * @since_tizen 6.5
 * @remarks @a payload_id must be released using free().
 * @param[in] result The result of payload async
 * @param[out] payload_id The ID of payload
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *payload_id = NULL;

    ret = cion_payload_async_result_get_payload_id(result, &payload_id);
}
 * @endcode
 */
int cion_payload_async_result_get_payload_id(
        const cion_payload_async_result_h result,
        char **payload_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_APPFW_CION_PAYLOAD_ASYNC_RESULT_H__ */
