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

#ifndef __TIZEN_APPFW_CION_CONNECTION_RESULT_H__
#define __TIZEN_APPFW_CION_CONNECTION_RESULT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_MODULE
 * @{
 */

/**
 * @brief  The Cion connection result handle.
 * @since_tizen 6.5
 */
typedef void *cion_connection_result_h;

/**
 * @brief Enumeration for Cion connection status types.
 * @since_tizen 6.5
 */
typedef enum _connection_status_e {
  CION_CONNECTION_STATUS_OK, /**< Connection is ok **/
  CION_CONNECTION_STATUS_REJECTED, /**< Connection is rejected **/
  CION_CONNECTION_STATUS_ERROR, /**< Connection error occurs **/
} cion_connection_status_e;

/**
 * @brief Gets the connection result status.
 * @since_tizen 6.5
 * @param[in] result The connection result handle
 * @param[out] status Connection result status
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    cion_connection_status_e status;

    ret = cion_connection_result_get_status(result, &status);
}
 * @endcode
 */
int cion_connection_result_get_status(const cion_connection_result_h result,
        cion_connection_status_e *status);

/**
 * @brief Gets the connection result reason.
 * @since_tizen 6.5
 * @remarks @a reason must be released using free().
 * @param[in] result The connection result handle
 * @param[out] reason Connection result reason, should be freed after use
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *reason = NULL;

    ret = cion_connection_result_get_reason(result, &reason);
}
 * @endcode
 */
int cion_connection_result_get_reason(const cion_connection_result_h result,
        char **reason);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_APPFW_CION_CONNECTION_RESULT_H__ */
