
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

#ifndef __TIZEN_APPFW_CION_PAYLOAD_H__
#define __TIZEN_APPFW_CION_PAYLOAD_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_MODULE
 * @{
 */

/**
 * @brief The Cion payload handle.
 * @since_tizen 6.5
 */
typedef void *cion_payload_h;

/**
 * @brief Enumeration for payload types.
 * @since_tizen 6.5
 */
typedef enum _payload_type_e {
  CION_PAYLOAD_TYPE_DATA, /**< Data type payload **/
  CION_PAYLOAD_TYPE_FILE, /**< File type payload **/
} cion_payload_type_e;


/**
 * @brief Enumeration for payload transfer status types.
 * @since_tizen 6.5
 */
typedef enum _payload_transfer_status_e {
  CION_PAYLOAD_TRANSFER_STATUS_SUCCESS, /**< Transfer is success **/
  CION_PAYLOAD_TRANSFER_STATUS_FAILURE, /**< Transfer is failed **/
  CION_PAYLOAD_TRANSFER_STATUS_IN_PROGRESS, /**< Transfer is in progress **/
} cion_payload_transfer_status_e;

/**
 * @brief Creates a payload handle.
 * @since_tizen 6.5
 * @remarks @a payload must be released using cion_payload_destroy().
 * @param[out] payload The payload handle
 * @param[in] type The type of payload
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see cion_payload_destroy()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    cion_payload_h payload;

    ret = cion_payload_create(&payload, CION_PAYLOAD_TYPE_DATA);
}
 * @endcode
 */
int cion_payload_create(cion_payload_h *payload, cion_payload_type_e type);

/**
 * @brief Destroys the payload handle.
 * @since_tizen 6.5
 * @param[in] payload The payload handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_payload_create()
 * @par Sample code:
 * @code
 #include <cion.h>

{
    int ret;

    ret = cion_payload_destroy(payload);
}
 * @endcode
 */
int cion_payload_destroy(cion_payload_h payload);

/**
 * @brief Gets the type of payload.
 * @since_tizen 6.5
 * @param[in] payload The payload handle
 * @param[out] type The type of payload
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #cion_payload_type_e
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    cion_payload_type_e type;

    ret = cion_payload_get_type(payload, &type);
}
 * @endcode
 */
int cion_payload_get_type(cion_payload_h payload, cion_payload_type_e *type);

/**
 * @brief Gets the data from payload.
 * @since_tizen 6.5
 * @remarks This can be used when the payload type is #CION_PAYLOAD_TYPE_DATA.
 * @remarks @a data must be released using free().
 * @param[in] payload The payload handle
 * @param[out] data The data
 * @param[out] data_size The size of data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see #CION_PAYLOAD_TYPE_DATA
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    unsigned char *data = NULL;
    unsigned int data_size;

    ret = cion_payload_get_data(payload, &data, &data_size);
}
 * @endcode
 */
int cion_payload_get_data(cion_payload_h payload, unsigned char **data,
    unsigned int *data_size);

/**
 * @brief Sets the data to payload.
 * @since_tizen 6.5
 * @remarks This can be used when the payload type is #CION_PAYLOAD_TYPE_DATA.
 * @param[in] payload The payload handle
 * @param[in] data The data
 * @param[in] data_size The size of data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #CION_PAYLOAD_TYPE_DATA
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_payload_set_data(payload, "mydata", 6);
}
 * @endcode
 */
int cion_payload_set_data(cion_payload_h payload,
    const unsigned char *data, unsigned int data_size);

/**
 * @brief Saves the payload as a file.
 * @since_tizen 6.5
 * @remarks This can be used when the payload type is #CION_PAYLOAD_TYPE_FILE.
 * @remarks The @a path is absolute path.
 * @remarks If the media storage is used, %http://tizen.org/privilege/mediastorage privilege is necessary. \n
 *          If the external storage is used, %http://tizen.org/privilege/externalstorage privilege is necessary.
 * @param[in] payload The payload handle
 * @param[in] path The path of file
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OPERATION_FAILED Operation failed
 * @see #CION_PAYLOAD_TYPE_FILE
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_payload_save_as_file(payload, "filepath");
}
 * @endcode
 */
int cion_payload_save_as_file(cion_payload_h payload, const char *path);

/**
 * @brief Gets the name of received file from the payload.
 * @since_tizen 6.5
 * @remarks This can be used when the payload type is #CION_PAYLOAD_TYPE_FILE.
 * @remarks @a file_name must be released using free().
 * @param[in] payload The payload handle
 * @param[out] file_name The name of file
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see #CION_PAYLOAD_TYPE_FILE
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *file_name = NULL;

    ret = cion_payload_get_received_file_name(payload, &file_name);
}
 * @endcode
 */
int cion_payload_get_received_file_name(cion_payload_h payload,
    char **file_name);

/**
 * @brief Gets the size of currently received file from the payload.
 * @since_tizen 6.5
 * @remarks This can be used when the payload type is #CION_PAYLOAD_TYPE_FILE.
 * @param[in] payload The payload handle
 * @param[out] bytes The size of file
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #CION_PAYLOAD_TYPE_FILE
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    uint64_t bytes;

    ret = cion_payload_get_received_bytes(payload, &bytes);
}
 * @endcode
 */
int cion_payload_get_received_bytes(cion_payload_h payload, uint64_t *bytes);

/**
 * @brief Gets the total size of file from the payload.
 * @since_tizen 6.5
 * @remarks This can be used when the payload type is #CION_PAYLOAD_TYPE_FILE.
 * @param[in] payload The payload handle
 * @param[out] bytes The size of file
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #CION_PAYLOAD_TYPE_FILE
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    uint64_t bytes;

    ret = cion_payload_get_total_bytes(payload, &bytes);
}
 * @endcode
 */
int cion_payload_get_total_bytes(cion_payload_h payload, uint64_t *bytes);

/**
 * @brief Sets the file path to send payload.
 * @since_tizen 6.5
 * @remarks This can be used when the payload type is #CION_PAYLOAD_TYPE_FILE.
 * @remarks If media storage is used, %http://tizen.org/privilege/mediastorage privilege is necessary. \n
 *          If external storage is used, %http://tizen.org/privilege/externalstorage privilege is necessary.
 * @param[in] payload The payload handle
 * @param[in] path The path of file
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #CION_PAYLOAD_TYPE_FILE
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_payload_set_file_path(payload, "filepath");
}
 * @endcode
 */
int cion_payload_set_file_path(cion_payload_h payload, const char *path);

/**
 * @brief Gets the ID of payload.
 * @since_tizen 6.5
 * @remarks @a payload_id must be released using free().
 * @param[in] payload The payload handle
 * @param[out] payload_id The ID of payload
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_payload_get_payload_id(payload, &payload_id);
}
 * @endcode
 */
int cion_payload_get_payload_id(cion_payload_h payload, char **payload_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_APPFW_CION_PAYLOAD_H__ */
