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

#ifndef __TIZEN_APPFW_THEME_MANAGER_THEME_LOADER_H__
#define __TIZEN_APPFW_THEME_MANAGER_THEME_LOADER_H__

#include <theme.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file theme_loader.h
 */

/**
 * @addtogroup CAPI_THEME_MODULE
 * @{
 */

/**
 * @brief Called to get the changed theme information.
 * @since_tizen 6.0
 * @remarks The @a handle should not be released
 * @remarks @handle is valid only inside this function. If you want to keep it,
 *          please use theme_clone().
 * @param[in] handle    The changed theme information.
 * @param[in] user_data The user data passed from add event function.
 * @return @c 0 on success,
 *         otherwise a negative value.
 * @pre theme_loader_add_event() will invoke this callback.
 * @see theme_loader_add_event()
 */
typedef int (*theme_loader_changed_cb) (theme_h handle, void *user_data);

/**
 * @brief The theme loader handle.
 * @since_tizen 6.0
 */
typedef void *theme_loader_h;

/**
 * @brief Creates the theme laoder handle.
 * @since_tizen 6.0
 * @remarks You must release @handle id using theme_loader_destroy().
 * @param[out]  handle     The theme loader information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @see theme_loader_destroy()
 */
int theme_loader_create(theme_loader_h *handle);

/**
 * @brief Destroys the theme laoder handle.
 * @since_tizen 6.0
 * @param[in]  handle     The theme loader information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see theme_loader_create()
 */
int theme_loader_destroy(theme_loader_h handle);

/**
 * @brief Registers a callback function to be invoked when the theme is being changed.
 * @since_tizen 6.0
 * @remarks If you no longer use @event_id, you must release it using free().
 * @param[in] handle     The theme loader handle
 * @param[in] changed    The callback function to be registered for theme changed event
 * @param[in] user_data  The user data to be passed to the callback function
 * @param[out] event_id  The theme event id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_OUT_OF_MEMORY           Out of memory
 * @retval #THEME_MANAGER_ERROR_IO_ERROR          I/O error
 * @see theme_loader_remove_event()
*/
int theme_loader_add_event(theme_loader_h handle,
    theme_loader_changed_cb changed, void *user_data, char** event_id);

/**
 * @brief Unregisters a callback function.
 * @since_tizen 6.0
 * @param[in] handle       The theme loader handle
 * @param[in] event_id     The theme event id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see theme_loader_add_event()
*/
int theme_loader_remove_event(theme_loader_h handle, const char* event_id);

/**
 * @brief Loads current theme information.
 * @since_tizen 6.0
 * @remarks You must release @cur_theme using theme_destroy().
 * @param[in]  handle         The theme loader handle
 * @param[out] cur_theme      The information of current theme
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_NO_SUCH_THEME     No such theme
*/
int theme_loader_load_current(theme_loader_h handle, theme_h *cur_theme);

/**
 * @brief Loads information of given theme ID.
 * @since_tizen 6.0
 * @remarks You must release @theme using theme_destroy().
 * @param[in]  handle         The theme loader handle
 * @param[in]  id             The theme ID
 * @param[out] theme          The theme information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_ERROR_NO_SUCH_THEME     No such theme
*/
int theme_loader_load(theme_loader_h handle, const char *id, theme_h *theme);

/**
 * @brief Gets bundle of theme IDs.
 * @since_tizen 6.0
 * @remarks The @a ids should be released using free().
 * @param[in]  handle         The theme loader handle
 * @param[out] ids            The theme IDs
 * @param[out] count          The number of IDs
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #THEME_MANAGER_OUT_OF_MEMORY           Out of memory
*/
int theme_loader_query_id(theme_loader_h handle, char ***ids, int *count);

/**
 * @brief Sets theme which has given ID.
 * @since_tizen 6.0
 * @param[in] handle         The theme loader handle
 * @param[in] id             The ID of theme to be set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #THEME_MANAGER_ERROR_NONE              Successful
 * @retval #THEME_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
*/
int theme_loader_set_current(theme_loader_h handle, const char *id);

#ifdef __cplusplus
}
#endif
#endif  /* __TIZEN_APPFW_THEME_MANAGER_THEME_LOADER_H__ */
