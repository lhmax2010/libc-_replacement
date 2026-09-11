/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_UIX_AUTOFILL_COMMON_H__
#define __TIZEN_UIX_AUTOFILL_COMMON_H__

/**
 * @file autofill_common.h
 * @brief This file contains autofill APIs and related enumeration.
 */

#include <tizen.h>
#include <autofill_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_UIX_AUTOFILL_COMMON_MODULE
 * @{
 */

/**
 * @brief Enumeration for autofill hint.
 *
 * @since_tizen 5.5
 */
typedef enum {
    AUTOFILL_HINT_CREDIT_CARD_EXPIRATION_DATE, /**< Autofill hint for a credit card expiration date */
    AUTOFILL_HINT_CREDIT_CARD_EXPIRATION_DAY, /**< Autofill hint for a credit card expiration day */
    AUTOFILL_HINT_CREDIT_CARD_EXPIRATION_MONTH, /**< Autofill hint for a credit card expiration month */
    AUTOFILL_HINT_CREDIT_CARD_EXPIRATION_YEAR, /**< Autofill hint for a credit card expiration year */
    AUTOFILL_HINT_CREDIT_CARD_NUMBER, /**< Autofill hint for a credit card number */
    AUTOFILL_HINT_EMAIL_ADDRESS, /**< Autofill hint for an email address */
    AUTOFILL_HINT_NAME, /**< Autofill hint for a user's real name */
    AUTOFILL_HINT_PHONE, /**< Autofill hint for a phone number */
    AUTOFILL_HINT_POSTAL_ADDRESS, /**< Autofill hint for a postal address */
    AUTOFILL_HINT_POSTAL_CODE, /**< Autofill hint for a postal code */
    AUTOFILL_HINT_ID, /**< Autofill hint for a user's ID */
    AUTOFILL_HINT_PASSWORD, /**< Autofill hint for password */
    AUTOFILL_HINT_CREDIT_CARD_SECURITY_CODE /**< Autofill hint for a credit card security code */
} autofill_hint_e;

/**
 * @brief The autofill item handle.
 * @since_tizen 5.5
 */
typedef struct autofill_item_s *autofill_item_h;

/**
 * @brief The autofill authentication information handle.
 * @since_tizen 5.5
 */
typedef struct autofill_auth_info_s *autofill_auth_info_h;

/**
 * @brief The autofill view information handle.
 * @since_tizen 5.5
 */
typedef struct autofill_view_info_s *autofill_view_info_h;

/**
 * @brief The handle for having a response of the requested autofill.
 * @since_tizen 5.5
 */
typedef struct autofill_fill_response_s *autofill_fill_response_h;

/**
 * @brief The autofill fill response group handle.
 * @since_tizen 5.5
 */
typedef struct autofill_fill_response_group_s *autofill_fill_response_group_h;

/**
 * @brief The autofill fill response item handle.
 * @since_tizen 5.5
 */
typedef struct autofill_response_item_s *autofill_fill_response_item_h;

/**
 * @brief The autofill save item handle.
 * @since_tizen 5.5
 */
typedef struct autofill_save_item_s *autofill_save_item_h;

/**
 * @brief The handle of the view having autofill entries.
 * @since_tizen 5.5
 */
typedef struct autofill_save_view_info_s *autofill_save_view_info_h;

/**
 * @brief The autofill error information handle.
 * @since_tizen 5.5
 */
typedef struct autofill_error_info_s *autofill_error_info_h;

/**
 * @brief Called for each autofill information in view info.
 * @since_tizen 5.5
 * @remarks @a item should not be freed and can be used only in the callback.
 * @param[in] item The autofill item handle
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre autofill_view_info_foreach_item() will invoke this callback.
 * @see autofill_view_info_foreach_item()
 */
typedef bool (*autofill_item_cb)(autofill_item_h item, void *user_data);

/**
 * @brief Called for each autofill information in save view info.
 * @since_tizen 5.5
 * @remarks @a item should not be freed and can be used only in the callback.
 * @param[in] item The autofill item handle
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre autofill_view_info_foreach_item() will invoke this callback.
 * @see autofill_view_info_foreach_item()
 */
typedef bool (*autofill_save_item_cb)(autofill_save_item_h item, void *user_data);

/**
 * @brief Called for each autofill fill response group information in fill response.
 * @since_tizen 5.5
 * @remarks @a item should not be freed and can be used only in the callback.
 * @param[in] item The autofill fill response group handle
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre autofill_fill_response_foreach_group() will invoke this callback.
 * @see autofill_fill_response_foreach_group()
 */
typedef bool (*autofill_fill_response_group_cb)(autofill_fill_response_group_h item, void *user_data);

/**
 * @brief Called for each autofill fill response information in fill response group.
 * @since_tizen 5.5
 * @remarks @a item should not be freed and can be used only in the callback.
 * @param[in] item The autofill fill response item handle
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre autofill_fill_response_group_foreach_item() will invoke this callback.
 * @see autofill_view_info_foreach_item()
 */
typedef bool (*autofill_fill_response_item_cb)(autofill_fill_response_item_h item, void *user_data);

/**
 * @brief Creates an autofill item.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a it handle must be released with autofill_item_destroy().
 * @param[out] it The autofill item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 * @see autofill_item_destroy()
 */
int autofill_item_create(autofill_item_h *it);

/**
 * @brief Destroys an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_destroy(autofill_item_h it);

/**
 * @brief Clones an autofill item.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a clone handle must be released with autofill_item_destroy().
 * @param[in] it The autofill item handle
 * @param[out] clone The autofill item handle to be cloned
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_item_clone(autofill_item_h it, autofill_item_h *clone);

/**
 * @brief Sets the autofill hint (id (username), name, password, phone, credit card number, organization, so on) in an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @param[in] hint The autofill hint
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_set_autofill_hint(autofill_item_h it, autofill_hint_e hint);

/**
 * @brief Gets the autofill hint from an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @param[out] hint The autofill hint
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_get_autofill_hint(autofill_item_h it, autofill_hint_e *hint);

/**
 * @brief Sets the autofill ID in an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @param[in] id The autofill ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_set_id(autofill_item_h it, const char *id);

/**
 * @brief Gets the autofill ID from an autofill item.
 * @since_tizen 5.5
 * @remarks @a id must be released using free().
 * @param[in] it The autofill item handle
 * @param[out] id The autofill ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_item_get_id(autofill_item_h it, char **id);

/**
 * @brief Sets the autofill label in an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @param[in] label The autofill label
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_set_label(autofill_item_h it, const char *label);

/**
 * @brief Gets the autofill label from an autofill item.
 * @since_tizen 5.5
 * @remarks @a label must be released using free().
 * @param[in] it The autofill item handle
 * @param[out] label The autofill label
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_item_get_label(autofill_item_h it, char **label);

/**
 * @brief Sets the sensitive data in an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @param[in] sensitive The sensitive data or not
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_set_sensitive_data(autofill_item_h it, bool sensitive);

/**
 * @brief Gets the sensitive data from an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @param[out] sensitive The sensitive data or not
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_get_sensitive_data(autofill_item_h it, bool *sensitive);

/**
 * @brief Sets the autofill value in an autofill item.
 * @since_tizen 5.5
 * @param[in] it The autofill item handle
 * @param[in] value The autofill value
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_item_set_value(autofill_item_h it, const char *value);

/**
 * @brief Gets the autofill value from an autofill item.
 * @since_tizen 5.5
 * @remarks @a value must be released using free().
 * @param[in] it The autofill item handle
 * @param[out] value The autofill value
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_item_get_value(autofill_item_h it, char **value);

/**
 * @brief Creates autofill authentication information.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a ai handle must be released with autofill_auth_info_destroy().
 * @param[out] ai The autofill authentication information handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_auth_info_create(autofill_auth_info_h *ai);

/**
 * @brief Destroys autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_destroy(autofill_auth_info_h ai);

/**
 * @brief Sets the app id in autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[in] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_set_app_id(autofill_auth_info_h ai, const char *app_id);

/**
 * @brief Gets the app id from autofill authentication information.
 * @since_tizen 5.5
 * @remarks @a app_id must be released using free().
 * @param[in] ai The autofill authentication information handle
 * @param[out] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_auth_info_get_app_id(autofill_auth_info_h ai, char **app_id);

/**
 * @brief Sets the view id in autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[in] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_set_view_id(autofill_auth_info_h ai, const char *view_id);

/**
 * @brief Gets the view id from autofill authentication information.
 * @since_tizen 5.5
 * @remarks @a view_id must be released using free().
 * @param[in] ai The autofill authentication information handle
 * @param[out] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_auth_info_get_view_id(autofill_auth_info_h ai, char **view_id);

/**
 * @brief Sets the 'autofill data present' attribute in autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[in] autofill_data_present The autofill data presence
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_set_autofill_data_present(autofill_auth_info_h ai, bool autofill_data_present);

/**
 * @brief Gets the 'autofill data present' attribute from autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[out] autofill_data_present The autofill data presence
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_get_autofill_data_present(autofill_auth_info_h ai, bool *autofill_data_present);

/**
 * @brief Sets the 'authentication needed' attribute in autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[in] authentication_needed The authentication need
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_set_authentication_needed(autofill_auth_info_h ai, bool authentication_needed);

/**
 * @brief Gets the 'authentication needed' attribute from autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[out] authentication_needed The authentication need
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_get_authentication_needed(autofill_auth_info_h ai, bool *authentication_needed);

/**
 * @brief Sets the service name in autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[in] service_name The autofill service name
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_set_service_name(autofill_auth_info_h ai, const char *service_name);

/**
 * @brief Gets the service name from autofill authentication information.
 * @since_tizen 5.5
 * @remarks @a service_name must be released using free().
 * @param[in] ai The autofill authentication information handle
 * @param[out] service_name The autofill service name
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_auth_info_get_service_name(autofill_auth_info_h ai, char **service_name);

/**
 * @brief Sets the service message in autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[in] service_message The autofill service message
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_set_service_message(autofill_auth_info_h ai, const char *service_message);

/**
 * @brief Gets the service message from autofill authentication information.
 * @since_tizen 5.5
 * @remarks @a service_message must be released using free().
 * @param[in] ai The autofill authentication information handle
 * @param[out] service_message The autofill service message
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_auth_info_get_service_message(autofill_auth_info_h ai, char **service_message);

/**
 * @brief Sets the service logo image path in autofill authentication information.
 * @since_tizen 5.5
 * @param[in] ai The autofill authentication information handle
 * @param[in] service_logo_image_path The autofill service logo image path
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_auth_info_set_service_logo_image_path(autofill_auth_info_h ai, const char *service_logo_image_path);

/**
 * @brief Gets the service logo image path from autofill authentication information.
 * @since_tizen 5.5
 * @remarks @a service_logo_image_path must be released using free().
 * @param[in] ai The autofill authentication information handle
 * @param[out] service_logo_image_path The autofill service logo image path
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_auth_info_get_service_logo_image_path(autofill_auth_info_h ai, char **service_logo_image_path);

/**
 * @brief Creates autofill view information.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a vi handle must be released with autofill_view_info_destroy().
 * @param[out] vi The autofill view info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_view_info_create(autofill_view_info_h *vi);

/**
 * @brief Destroys autofill view information.
 * @since_tizen 5.5
 * @param[in] vi The autofill view info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_view_info_destroy(autofill_view_info_h vi);

/**
 * @brief Sets the app id in autofill view information.
 * @since_tizen 5.5
 * @param[in] vi The autofill view info handle
 * @param[in] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_view_info_set_app_id(autofill_view_info_h vi, const char *app_id);

/**
 * @brief Gets the app id from autofill view information.
 * @since_tizen 5.5
 * @remarks @a app_id must be released using free().
 * @param[in] vi The autofill view info handle
 * @param[out] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_view_info_get_app_id(autofill_view_info_h vi, char **app_id);

/**
 * @brief Sets the view id in autofill view information.
 * @since_tizen 5.5
 * @param[in] vi The autofill view info handle
 * @param[in] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_view_info_set_view_id(autofill_view_info_h vi, const char *view_id);

/**
 * @brief Gets the view id from autofill view information.
 * @since_tizen 5.5
 * @remarks @a view_id must be released using free().
 * @param[in] vi The autofill view info handle
 * @param[out] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_view_info_get_view_id(autofill_view_info_h vi, char **view_id);

/**
 * @brief Adds autofill item in autofill view information.
 * @since_tizen 5.5
 * @param[in] vi The autofill view info handle
 * @param[in] it The autofill item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_view_info_add_item(autofill_view_info_h vi, autofill_item_h it);

/**
 * @brief Retrieves all items in autofill view information.
 * @since_tizen 5.5
 * @param[in] vi The autofill view info handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_view_info_foreach_item(autofill_view_info_h vi, autofill_item_cb callback, void *user_data);

/**
 * @brief Creates the view having autofill entries.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a vi handle must be released with autofill_save_view_info_destroy()
 * @param[out] vi The handle of autofill for the saving view
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_save_view_info_create(autofill_save_view_info_h *vi);

/**
 * @brief Destroys the view having autofill entries.
 * @since_tizen 5.5
 * @param[in] vi The autofill view info handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_view_info_destroy(autofill_save_view_info_h vi);

/**
 * @brief Sets the app ID in the view having autofill entries.
 * @since_tizen 5.5
 * @param[in] vi The handle of autofill for the saving view
 * @param[in] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_view_info_set_app_id(autofill_save_view_info_h vi, const char *app_id);

/**
 * @brief Gets the app ID from the view having autofill entries.
 * @since_tizen 5.5
 * @remarks @a app_id must be released using free().
 * @param[in] vi The handle of autofill for the saving view
 * @param[out] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_save_view_info_get_app_id(autofill_save_view_info_h vi, char **app_id);

/**
 * @brief Sets the view ID in the view having autofill entries.
 * @since_tizen 5.5
 * @param[in] vi The handle of autofill for the saving view
 * @param[in] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_view_info_set_view_id(autofill_save_view_info_h vi, const char *view_id);

/**
 * @brief Gets the view ID from the view having autofill entries.
 * @since_tizen 5.5
 * @remarks @a view_id must be released using free().
 * @param[in] vi The handle of autofill for the saving view
 * @param[out] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_save_view_info_get_view_id(autofill_save_view_info_h vi, char **view_id);

/**
 * @brief Sets the title of the view having autofill entries.
 * @since_tizen 5.5
 * @param[in] vi The handle of autofill for the saving view
 * @param[in] view_title The view title
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_view_info_set_view_title(autofill_save_view_info_h vi, const char *view_title);

/**
 * @brief Gets the title of the view having autofill entries.
 * @since_tizen 5.5
 * @remarks @a view_title must be released using free().
 * @param[in] vi The handle of autofill for the saving view
 * @param[out] view_title The view title
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_save_view_info_get_view_title(autofill_save_view_info_h vi, char **view_title);

/**
 * @brief Adds autofill save item in the view having autofill entries.
 * @since_tizen 5.5
 * @param[in] vi The handle of autofill for the saving view
 * @param[in] it The autofill save item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_view_info_add_item(autofill_save_view_info_h vi, autofill_save_item_h it);

/**
 * @brief Retrieves all items in the view having autofill entries.
 * @since_tizen 5.5
 * @param[in] vi The handle of autofill for the saving view
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_view_info_foreach_item(autofill_save_view_info_h vi, autofill_save_item_cb callback, void *user_data);

/**
 * @brief Creates a handle for having a response of the requested autofill.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a h handle must be released with autofill_fill_response_destroy().
 * @param[out] h The handle for having a response of the requested autofill
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_create(autofill_fill_response_h *h);

/**
 * @brief Destroys a handle for having a response of the requested autofill.
 * @since_tizen 5.5
 * @param[in] h The handle for having a response of the requested autofill
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_destroy(autofill_fill_response_h h);

/**
 * @brief Sets the app ID in the handle for having a response of the requested autofill.
 * @since_tizen 5.5
 * @param[in] h The handle for having a response of the requested autofill
 * @param[in] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_set_app_id(autofill_fill_response_h h, const char *app_id);

/**
 * @brief Gets the app ID from the handle for having a response of the requested autofill.
 * @since_tizen 5.5
 * @remarks @a app_id must be released using free().
 * @param[in] h The handle for having a response of the requested autofill
 * @param[out] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_fill_response_get_app_id(autofill_fill_response_h h, char **app_id);

/**
 * @brief Sets the view ID in the handle for having a response of the requested autofill.
 * @since_tizen 5.5
 * @param[in] h The handle for having a response of the requested autofill
 * @param[in] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_set_view_id(autofill_fill_response_h h, const char *view_id);

/**
 * @brief Gets the view ID from the handle for having a response of the requested autofill.
 * @since_tizen 5.5
 * @remarks @a view_id must be released using free().
 * @param[in] h The handle for having a response of the requested autofill
 * @param[out] view_id The view ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_fill_response_get_view_id(autofill_fill_response_h h, char **view_id);

/**
 * @brief Adds autofill fill response group.
 * @since_tizen 5.5
 * @param[in] h The handle for having a response of the requested autofill
 * @param[in] it The autofill fill response group handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_add_group(autofill_fill_response_h h, autofill_fill_response_group_h it);

/**
 * @brief Retrieves all groups of each fill response.
 * @since_tizen 5.5
 * @param[in] h The handle for having a response of the requested autofill
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_foreach_group(autofill_fill_response_h h, autofill_fill_response_group_cb callback, void *user_data);

/**
 * @brief Gets the number of fill response group.
 * @since_tizen 5.5
 * @param[in] h The handle for having a response of the requested autofill
 * @param[out] count The number of group
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_get_group_count(autofill_fill_response_h h, int *count);

/**
 * @brief Creates an autofill fill response group.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a h handle must be released with autofill_fill_response_group_destroy().
 * @param[out] h The autofill fill response group handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_fill_response_group_create(autofill_fill_response_group_h *h);

/**
 * @brief Destroys an autofill fill response group.
 * @since_tizen 5.5
 * @param[in] h The autofill fill response group handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_group_destroy(autofill_fill_response_group_h h);

/**
 * @brief Clones an autofill fill response group.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a clone handle must be released with autofill_fill_response_group_destroy().
 * @param[in] h The autofill fill response group handle
 * @param[in] clone The autofill fill response group handle to be cloned
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_fill_response_group_clone(autofill_fill_response_group_h h, autofill_fill_response_group_h *clone);

/**
 * @brief Adds autofill item in an autofill fill response group.
 * @since_tizen 5.5
 * @param[in] h The autofill fill response group handle
 * @param[in] it The autofill fill response item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_fill_response_group_add_item(autofill_fill_response_group_h h, autofill_fill_response_item_h it);

/**
 * @brief Retrieves all fill response items of each group.
 * @since_tizen 5.5
 * @param[in] h The autofill fill response group handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_group_foreach_item(autofill_fill_response_group_h h, autofill_fill_response_item_cb callback, void *user_data);

/**
 * @brief Creates a handle for fill response item.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a it handle must be released with autofill_fill_response_item_destroy().
 * @param[out] it The autofill fill response item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_fill_response_item_create(autofill_fill_response_item_h *it);

/**
 * @brief Destroys the handle for fill response item.
 * @since_tizen 5.5
 * @param[in] it The autofill fill response item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_item_destroy(autofill_fill_response_item_h it);

/**
 * @brief Clones a handle for fill response item.
 *
 * @since_tizen 5.5
 *
 * @param[in] it The autofill fill response item handle
 * @param[in] clone The autofill fill response item handle to be cloned
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_fill_response_item_clone(autofill_fill_response_item_h it, autofill_fill_response_item_h *clone);

/**
 * @brief Sets the autofill ID in the handle for fill response item.
 * @since_tizen 5.5
 * @param[in] it The autofill fill response item handle
 * @param[in] id The autofill fill response item ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_item_set_id(autofill_fill_response_item_h it, const char *id);

/**
 * @brief Gets the autofill ID from the handle for fill response item.
 * @since_tizen 5.5
 * @remarks @a id must be released using free().
 * @param[in] it The autofill fill response item handle
 * @param[out] id The autofill fill response item ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_fill_response_item_get_id(autofill_fill_response_item_h it, char **id);

/**
 * @brief Sets the autofill value in the handle for fill response item.
 * @since_tizen 5.5
 * @param[in] it The autofill fill response item handle
 * @param[in] value The autofill fill response item value
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_item_set_value(autofill_fill_response_item_h it, const char *value);

/**
 * @brief Gets the autofill value from the handle for fill response item.
 * @since_tizen 5.5
 * @remarks @a value must be released using free().
 * @param[in] it The autofill fill response item handle
 * @param[out] value The autofill fill response item value
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_fill_response_item_get_value(autofill_fill_response_item_h it, char **value);

/**
 * @brief Sets the presentation text in the handle for fill response item.
 * @since_tizen 5.5
 * @param[in] it The autofill fill response item handle
 * @param[in] presentation_text The presentation text
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_item_set_presentation_text(autofill_fill_response_item_h it, const char *presentation_text);

/**
 * @brief Gets the presentation text from the handle for fill response item.
 * @since_tizen 5.5
 * @remarks @a presentation_text must be released using free().
 * @param[in] it The autofill fill response item handle
 * @param[out] presentation_text The presentation text
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_fill_response_item_get_presentation_text(autofill_fill_response_item_h it, char **presentation_text);

/**
 * @brief Sets the type of autofill hint in the handle for fill response item.
 * @since_tizen 5.5
 * @param[in] it The autofill fill response item handle
 * @param[in] hint The autofill hint
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_fill_response_item_set_autofill_hint(autofill_fill_response_item_h it, autofill_hint_e hint);

/**
 * @brief Gets the type of autofill hint in the handle for fill response item.
 * @since_tizen 5.5
 * @param[in] it The autofill fill response item handle
 * @param[out] hint The autofill hint
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_fill_response_item_get_autofill_hint(autofill_fill_response_item_h it, autofill_hint_e *hint);

/**
 * @brief Creates autofill save item.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a it handle must be released with autofill_save_item_destroy().
 * @param[out] it The autofill save item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_create(autofill_save_item_h *it);

/**
 * @brief Destroys an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_destroy(autofill_save_item_h it);

/**
 * @brief Clones an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[in] clone The autofill save item handle to be cloned
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_save_item_clone(autofill_save_item_h it, autofill_save_item_h *clone);

/**
 * @brief Sets the autofill hint (id(username), name, password, phone, credit card number, organization, so on) in an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[in] hint The autofill hint
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_set_autofill_hint(autofill_save_item_h it, autofill_hint_e hint);

/**
 * @brief Gets the autofill hint from an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[out] hint The autofill hint
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_get_autofill_hint(autofill_save_item_h it, autofill_hint_e *hint);

/**
 * @brief Sets the autofill ID in an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[in] id The autofill ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_set_id(autofill_save_item_h it, const char *id);

/**
 * @brief Gets the autofill ID from an autofill save item.
 * @since_tizen 5.5
 * @remarks @a id must be released using free().
 * @param[in] it The autofill save item handle
 * @param[out] id The autofill ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_save_item_get_id(autofill_save_item_h it, char **id);

/**
 * @brief Sets the autofill label in an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[out] label The autofill label
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_set_label(autofill_save_item_h it, const char *label);

/**
 * @brief Gets the autofill label from an autofill save item.
 * @since_tizen 5.5
 * @remarks @a label must be released using free().
 * @param[in] it The autofill save item handle
 * @param[out] label The autofill label
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_save_item_get_label(autofill_save_item_h it, char **label);

/**
 * @brief Sets the sensitive data in an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[in] sensitive The sensitive data or not
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_set_sensitive_data(autofill_save_item_h it, bool sensitive);

/**
 * @brief Gets the sensitive data from an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[out] sensitive The sensitive data or not
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_get_sensitive_data(autofill_save_item_h it, bool *sensitive);

/**
 * @brief Sets the autofill value in an autofill save item.
 * @since_tizen 5.5
 * @param[in] it The autofill save item handle
 * @param[in] value The autofill value
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_save_item_set_value(autofill_save_item_h it, const char *value);

/**
 * @brief Gets the autofill value from an autofill save item.
 * @since_tizen 5.5
 * @remarks @a value must be released using free().
 * @param[in] it The autofill save item handle
 * @param[out] value The autofill value
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_save_item_get_value(autofill_save_item_h it, char **value);

/**
 * @brief Creates autofill error information.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a ei handle must be released with autofill_error_info_destroy().
 * @param[out] ei The autofill error information handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OUT_OF_MEMORY Out of memory
 */
int autofill_error_info_create(autofill_error_info_h *ei);

/**
 * @brief Destroys autofill error information.
 * @since_tizen 5.5
 * @param[in] ei The autofill error information handle
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_error_info_destroy(autofill_error_info_h ei);

/**
 * @brief Sets the app id in autofill error information.
 * @since_tizen 5.5
 * @param[in] ei The autofill error information handle
 * @param[in] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_error_info_set_app_id(autofill_error_info_h ei, const char *app_id);

/**
 * @brief Gets the app id from autofill error information.
 * @since_tizen 5.5
 * @remarks @a app_id must be released using free().
 * @param[in] ei The autofill error information handle
 * @param[out] app_id The app ID
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #AUTOFILL_ERROR_OPERATION_FAILED Operation failure
 */
int autofill_error_info_get_app_id(autofill_error_info_h ei, char **app_id);

/**
 * @brief Sets the error code in autofill error information.
 * @since_tizen 5.5
 * @param[in] ei The autofill error information handle
 * @param[in] error_code The autofill error code
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int autofill_error_info_set_error_code(autofill_error_info_h ei, autofill_error_e error_code);

/**
 * @brief Gets the error code from autofill error information.
 * @since_tizen 5.5
 * @param[in] ei The autofill error information handle
 * @param[out] error_code The autofill error code
 * @return 0 on success, otherwise a negative error value
 * @retval #AUTOFILL_ERROR_NONE No error
 * @retval #AUTOFILL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see get_error_message()
 */
int autofill_error_info_get_error_code(autofill_error_info_h ei, autofill_error_e *error_code);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_AUTOFILL_COMMON_H__ */
