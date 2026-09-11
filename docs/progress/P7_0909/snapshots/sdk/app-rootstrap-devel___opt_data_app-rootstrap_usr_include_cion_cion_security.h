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

#ifndef __TIZEN_APPFW_CION_SECURITY_H__
#define __TIZEN_APPFW_CION_SECURITY_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_MODULE
 * @{
 */

/**
 * @brief The Cion security handle.
 * @since_tizen 6.5
 */
typedef void *cion_security_h;

/**
 * @brief Creates the Cion security handle.
 * @since_tizen 6.5
 * @remarks @a security must be released using cion_security_destroy().
 * @param[out] security The Cion security handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_security_destroy()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    cion_security_h security = NULL;

    ret = cion_security_create(&security);
}
 * @endcode
 */
int cion_security_create(cion_security_h *security);

/**
 * @brief Destroys the Cion security handle.
 * @since_tizen 6.5
 * @param[in] security The Cion security handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_security_create();
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_security_destroy(security);
}
 * @endcode
 */
int cion_security_destroy(cion_security_h security);

/**
 * @brief Sets the path of certificate authority certificates.
 * @since_tizen 6.5
 * @remarks If the media storage is used, %http://tizen.org/privilege/mediastorage privilege is necessary. \n
 *          If the external storage is used, %http://tizen.org/privilege/externalstorage privilege is necessary.
 * @param[in] security The Cion security handle
 * @param[in] ca_path The path of certificate authority certificates
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_security_set_ca_path(security, "CaPath");
}

 * @endcode
 */
int cion_security_set_ca_path(cion_security_h security, const char *ca_path);

/**
 * @brief Sets the path of certificate.
 * @since_tizen 6.5
 * @remarks If the media storage is used, %http://tizen.org/privilege/mediastorage privilege is necessary. \n
 *          If the external storage is used, %http://tizen.org/privilege/externalstorage privilege is necessary.
 * @param[in] security The Cion security handle
 * @param[in] cert_path The path of certificate
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_security_set_cert_path(security, "CertPath");
}
 * @endcode
 */
int cion_security_set_cert_path(cion_security_h security,
    const char *cert_path);

/**
 * @brief Sets the path of private key.
 * @since_tizen 6.5
 * @remarks If the media storage is used, %http://tizen.org/privilege/mediastorage privilege is necessary. \n
 *          If the external storage is used, %http://tizen.org/privilege/externalstorage privilege is necessary.
 * @param[in] security The Cion security handle
 * @param[in] key_path The path of private key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    ret = cion_security_set_private_key_path(security, "KeyPath");
}

 * @endcode
 */
int cion_security_set_private_key_path(cion_security_h security,
    const char *key_path);

/**
 * @brief Gets the path of certificate authority certificates.
 * @since_tizen 6.5
 * @remarks @a ca_path must be released using free().
 * @param[in] security The Cion security handle
 * @param[out] ca_path The path of certificate authority certificates
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
    char *ca_path = NULL;

    ret = cion_security_get_ca_path(security, &ca_path);
}
 * @endcode
 */
int cion_security_get_ca_path(cion_security_h security, char **ca_path);

/**
 * @brief Gets the path of certificate.
 * @since_tizen 6.5
 * @remarks @a cert_path must be released using free().
 * @param[in] security The Cion security handle
 * @param[out] cert_path The path of certificate
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see cion_security_set_cert_path()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *cert_path = NULL;

    ret = cion_security_get_cert_path(security, &cert_path);
}

 * @endcode
 */
int cion_security_get_cert_path(cion_security_h security, char **cert_path);

/**
 * @brief Gets the path of private key.
 * @since_tizen 6.5
 * @remarks @a key_path must be released using free().
 * @param[in] security The Cion security handle
 * @param[out] key_path The path of private key
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @see cion_security_set_private_key_path()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;
    char *key_path = NULL;

    ret = cion_security_get_private_key_path(security, &key_path);
}
 * @endcode
 */
int cion_security_get_private_key_path(cion_security_h security,
    char **key_path);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_APPFW_CION_SECURITY_H__ */
