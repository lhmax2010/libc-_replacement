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

#ifndef __TIZEN_APPFW_CION_PEER_INFO_H__
#define __TIZEN_APPFW_CION_PEER_INFO_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_MODULE
 * @{
 */

/**
 * @brief The Cion peer info handle.
 * @since_tizen 6.5
 */
typedef void *cion_peer_info_h;

/**
 * @brief Creates a clone of peer info handle.
 * @since_tizen 6.5
 * @remarks @a peer_info_clone must be released using cion_peer_info_destroy().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] peer_info_clone The cloned peer information handle
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
    cion_peer_info_h peer_info_cloned;

    ret = cion_peer_info_clone(peer_info, &peer_info_cloned);
}
 * @endcode
 */
int cion_peer_info_clone(const cion_peer_info_h peer_info,
        cion_peer_info_h *peer_info_clone);

/**
 * @brief Destroys the peer info handle and releases all its resources.
 * @since_tizen 6.5
 * @param[in] peer_info The Cion peer information handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_peer_info_clone()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_peer_info_destroy(peer_info);
}
 * @endcode
 */
int cion_peer_info_destroy(cion_peer_info_h peer_info);

/**
 * @brief Gets the device ID.
 * @since_tizen 6.5
 * @remarks @a device_id must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] device_id The device ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *device_id = NULL;

    ret = cion_peer_info_get_device_id(peer_info, &device_id);
}
 * @endcode
 */
int cion_peer_info_get_device_id(cion_peer_info_h peer_info,
        char **device_id);

/**
 * @brief Gets the device name.
 * @since_tizen 6.5
 * @remarks @a device_name must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] device_name The device name
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *device_name = NULL;

    ret = cion_peer_info_get_device_name(peer_info, &device_name);
}
 * @endcode
 */
int cion_peer_info_get_device_name(cion_peer_info_h peer_info,
        char **device_name);

/**
 * @brief Gets the device's platform.
 * @since_tizen 6.5
 * @remarks @a device_platform must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] device_platform The platform name
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *device_platform = NULL;

    ret = cion_peer_info_get_device_platform(peer_info, &device_platform);
}
 * @endcode
 */
int cion_peer_info_get_device_platform(cion_peer_info_h peer_info,
        char **device_platform);

/**
 * @brief Gets the device's platform version.
 * @since_tizen 6.5
 * @remarks @a device_platform_version must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] device_platform_version The platform version
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *device_type = NULL;

    ret = cion_peer_info_get_device_platform_version(peer_info, &device_type);
}
 * @endcode
 */
int cion_peer_info_get_device_platform_version(cion_peer_info_h peer_info,
        char **device_platform_version);

/**
 * @brief Gets the device's type.
 * @since_tizen 6.5
 * @remarks @a device_type must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] device_type Device type
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *device_type = NULL;

    ret = cion_peer_info_get_device_type(peer_info, &device_type);
}
 * @endcode
 */
int cion_peer_info_get_device_type(cion_peer_info_h peer_info,
        char **device_type);

/**
 * @brief Gets application ID of peer.
 * @since_tizen 6.5
 * @remarks @a app_id must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] app_id The application ID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *app_id = NULL;

    ret = cion_peer_info_get_app_id(peer_info, &app_id);
}
 * @endcode
 */
int cion_peer_info_get_app_id(cion_peer_info_h peer_info,
        char **app_id);

/**
 * @brief Gets application version of peer.
 * @since_tizen 6.5
 * @remarks @a app_version must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] app_version The application version
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *app_version = NULL;

    ret = cion_peer_info_get_app_version(peer_info, &app_version);
}
 * @endcode
 */
int cion_peer_info_get_app_version(cion_peer_info_h peer_info,
        char **app_version);

/**
 * @brief Gets UUID of peer.
 * @since_tizen 6.5
 * @remarks @a uuid must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] uuid The UUID
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *uuid = NULL;

    ret = cion_peer_info_get_uuid(peer_info, &uuid);
}
 * @endcode
 */
int cion_peer_info_get_uuid(cion_peer_info_h peer_info,
        char **uuid);

/**
 * @brief Gets display name of peer.
 * @details The custom name for service name. \n
 *          It is possible that @a display_name does not exist.
 * @since_tizen 6.5
 * @remarks @a display_name must be released using free().
 * @param[in] peer_info The Cion peer information handle
 * @param[out] display_name The display name
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see cion_security_set_ca_path()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *display_name = NULL;

    ret = cion_peer_info_get_display_name(peer_info, &display_name);
}
 * @endcode
 */
int cion_peer_info_get_display_name(cion_peer_info_h peer_info,
        char **display_name);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_APPFW_CION_PEER_INFO_H__ */
