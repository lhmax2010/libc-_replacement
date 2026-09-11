/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_APPFW_THEME_MANAGER_THEME_H__
#define __TIZEN_APPFW_THEME_MANAGER_THEME_H__

#include <theme_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file theme.h
 */

/**
 * @addtogroup CAPI_THEME_MODULE
 * @{
 */

/**
 * @brief The theme information handle.
 * @since_tizen 6.0
 */
typedef void *theme_h;

/**
 * @brief Gets the theme ID.
 * @since_tizen 6.0
 * @remarks You must release @a id using free().
 * @param[in]  handle     The theme information
 * @param[out] id         The theme ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_get_id(theme_h handle, char **id);

/**
 * @brief Gets the version of theme.
 * @since_tizen 6.0
 * @remarks You must release @a ver using free().
 * @param[in]  handle     The theme information
 * @param[out] ver        The theme version
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_get_version(theme_h handle, char **ver);

/**
 * @brief Gets the version of tool used to make theme.
 * @since_tizen 6.0
 * @remarks You must release @a ver using free().
 * @param[in]  handle     The theme information
 * @param[out] ver        The theme version
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_get_tool_version(theme_h handle, char **ver);

/**
 * @brief Gets the title of theme.
 * @since_tizen 6.0
 * @remarks You must release @a title using free().
 * @param[in]  handle       The theme information
 * @param[out] title        The title of theme
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_get_title(theme_h handle, char **title);

/**
 * @brief Gets the resolution of theme.
 * @since_tizen 6.0
 * @remarks You must release @a resolution using free().
 * @param[in]  handle         The theme information
 * @param[out] resolution     The resolution of theme
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_get_resolution(theme_h handle, char **resolution);

/**
 * @brief Gets the path of preview image for theme.
 * @since_tizen 6.0
 * @remarks You must release @a preview using free().
 * @param[in]  handle         The theme information
 * @param[out] preview        The path of preview image for theme
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_get_preview(theme_h handle, char **preview);

/**
 * @brief Gets the description of theme.
 * @since_tizen 6.0
 * @remarks You must release @a description using free().
 * @param[in]  handle             The theme information
 * @param[out] description        The description of theme
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_get_description(theme_h handle, char **description);

/**
 * @brief Gets the string corresponding to given key.
 * @since_tizen 6.0
 * @remarks You must release @a val using free().
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to find information
 * @param[out] val        Value corresponding to given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #THTME_MANAGER_ERROR_KEY_NOT_AVAILABLE Key not available
 */
int theme_get_string(theme_h handle, const char *key, char **val);

/**
 * @brief Gets the path corresponding to given key.
 * @since_tizen 6.0
 * @remarks You must release @a val using free().
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to find information
 * @param[out] val        Value corresponding to given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #THTME_MANAGER_ERROR_KEY_NOT_AVAILABLE Key not available
 */
int theme_get_path(theme_h handle, const char *key, char **val);

/**
 * @brief Gets the bundle of strings corresponding with given key.
 * @since_tizen 6.0
 * @remarks You must release @a val using free().
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to find information
 * @param[out] val        Values corresponding to given key
 * @param[out] cnt        Number of returned strings
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #THTME_MANAGER_ERROR_KEY_NOT_AVAILABLE Key not available
 */
int theme_get_string_array(theme_h handle, const char *key,
    char ***val, int *cnt);

/**
 * @brief Gets the bundle of paths corresponding to given key.
 * @since_tizen 6.0
 * @remarks You must release @a val using free().
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to find information
 * @param[out] val        Values corresponding to given key
 * @param[out] cnt        Number of returned strings
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #THTME_MANAGER_ERROR_KEY_NOT_AVAILABLE Key not available
 */
int theme_get_path_array(theme_h handle, const char *key,
    char ***val, int *cnt);

/**
 * @brief Gets the integer corresponding with given key.
 * @since_tizen 6.0
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to find information
 * @param[out] val        Value corresponding to given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THTME_MANAGER_ERROR_KEY_NOT_AVAILABLE Key not available
 */
int theme_get_int(theme_h handle, const char *key, int *val);

/**
 * @brief Gets the float corresponding to given key.
 * @since_tizen 6.0
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to find information
 * @param[out] val        Value corresponding to given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THTME_MANAGER_ERROR_KEY_NOT_AVAILABLE Key not available
 */
int theme_get_float(theme_h handle, const char *key, float *val);

/**
 * @brief Gets the boolean corresponding to given key.
 * @since_tizen 6.0
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to find information
 * @param[out] val        Value corresponding to given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THTME_MANAGER_ERROR_KEY_NOT_AVAILABLE Key not available
 */
int theme_get_bool(theme_h handle, const char *key, bool *val);

/**
 * @brief Check the given key is exist or not.
 * @since_tizen 6.5
 * @param[in]  handle     The theme information
 * @param[in]  key        String key to check existence
 * @param[out] val        Value corresponding to given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int theme_is_key_exist(theme_h handle, const char *key, bool *val);

/**
 * @brief Clones the given theme information.
 * @since_tizen 6.0
 * @remarks You must release @a new_handle using theme_destroy().
 * @param[in]  handle         The theme information to be cloned
 * @param[out] new_handle     Cloned theme information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int theme_clone(theme_h handle, theme_h *new_handle);

/**
 * @brief Destroys the theme information.
 * @since_tizen 6.0
 * @param[in]  handle         The theme information to be destroyed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int theme_destroy(theme_h handle);

#ifdef __cplusplus
}
#endif
#endif  /* __TIZEN_APPFW_THEME_MANAGER_THEME_H__ */
