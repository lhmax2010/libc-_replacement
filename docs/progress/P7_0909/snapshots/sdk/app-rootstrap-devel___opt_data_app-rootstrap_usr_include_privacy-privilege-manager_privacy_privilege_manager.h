/*
 *  Copyright (c) 2017 - 2020 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __PRIVACY_PRIVILEGE_MANAGER_H__
#define __PRIVACY_PRIVILEGE_MANAGER_H__

#include <stdlib.h>
#include <tizen.h>
#include <ppm_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_PRIVACY_PRIVILEGE_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration for results of a permission check.
 * @since_tizen 4.0
 */
typedef enum {
    /** The application has permission to use a privilege. */
    PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW,
    /** The application doesn't have permission to use a privilege. */
    PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY,
    /** The user has to be asked whether to grant permission to use a privilege. */
    PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK,
    /** The application has permission to use a privilege for current session only. */
    PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW_SESSION,
    /** The application doesn't have permission to use a privilege for current session only. */
    PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY_SESSION,
    /** The application has permission to use a privilege, only while it's in foreground. */
    PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW_IN_USE,
} ppm_check_result_e;

/**
 * @brief Enumeration for results of a permission request.
 * @since_tizen 4.0
 */
typedef enum {
    /** The user granted permission to use a privilege for an indefinite period of time. */
    PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER,
    /** The user denied granting permission to use a privilege for an indefinite period of time. */
    PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER,
    /** The user denied granting permission to use a privilege once. */
    PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE,
    /** The user granted permission to use a privilege for current application session. */
    PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_SESSION,
    /** The user denied granting permission to use a privilege for current application session. */
    PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_SESSION,
    /** The user granted permission to use a privilege only when application is used (on top). */
    PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_IN_USE,
} ppm_request_result_e;

/**
 * @brief Enumeration for status codes of a permission request.
 * @since_tizen 4.0
 */
typedef enum {
    /** Callback was called with a valid answer. */
    PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ANSWER,
    /** Callback was called because of an error. */
    PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ERROR,
} ppm_call_cause_e;

/**
 * @brief Called when the application receives a response upon calling ppm_request_permission().
 *
 * @since_tizen 4.0
 *
 * @param[in]   cause       The value representing a reason why this callback
 *                          has been called.
 * @param[in]   result      The result of a response triggered by calling ppm_request_permission().
 *                          This is a valid value only if the @a cause parameter is equal to
 *                          #PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ANSWER.
 * @param[in]   privilege   The privilege that has been checked. This pointer is managed by the API and
 *                          it is valid only in the body of the callback function.
 * @param[in]   user_data   User specific data, this pointer has been passed
 *                          to ppm_request_permission().
 *
 * @see ppm_request_permission()
 */
typedef void (*ppm_request_response_cb) (ppm_call_cause_e cause,
                                         ppm_request_result_e result,
                                         const char *privilege,
                                         void *user_data);

/**
 * @brief Called when the application receives a response upon calling ppm_request_permissions().
 *
 * @since_tizen 5.0
 *
 * @param[in]   cause             The value representing a reason why this callback
 *                                has been called.
 * @param[in]   results           The results of a response triggered by calling ppm_request_permissions().
 *                                This is a valid value only if the @a cause parameter is equal to
 *                                #PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ANSWER.
 * @param[in]   privileges        The privileges array that has been checked. This pointer is managed
 *                                by the API and it is valid only in the body of the callback function.
 * @param[in]   privileges_count  The number of elements in the privileges array and also results arrays
 *                                if @a cause parameter is equal to #PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ANSWER.
 * @param[in]   user_data         User specific data, this pointer has been passed
 *                                to ppm_request_permissions().
 *
 * @see ppm_request_permissions()
 */
typedef void (*ppm_request_multiple_response_cb) (ppm_call_cause_e cause,
                                                  const ppm_request_result_e *results,
                                                  const char **privileges,
                                                  size_t privileges_count,
                                                  void *user_data);

/**
 * @brief Checks if an application, which calls this function, has permission to use the
 * given privilege.
 *
 * @since_tizen 4.0
 *
 * @param[in]   privilege   The privilege that is to be checked.
 * @param[out]  result      The result of the privilege check.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE               Successful
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_UNKNOWN            Unknown error
 */
int ppm_check_permission(const char *privilege, ppm_check_result_e *result);

/**
 * @platform
 * @brief Checks if an application, with given app_id, has permission to use the
 * given privilege.
 *
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/permission.check
 *
 * @param[in]   app_id      The app_id of the app that is to be checked.
 * @param[in]   privilege   The privilege that is to be checked.
 * @param[out]  result      The result of the privilege check.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE               Successful
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_UNKNOWN            Unknown error
 */
int ppm_check_app_permission(const char *app_id, const char *privilege,
                             ppm_check_result_e *result);

/**
 * @brief Checks if an application, which calls this function, has permission to use the
 * given privileges.
 *
 * @since_tizen 5.0
 *
 * @param[in]   privileges        The privileges array that is to be checked.
 * @param[in]   privileges_count  The number of elements in the privileges and results arrays.
 * @param[out]  results           The results of the privilege check. Caller is responsible for
 *                                allocating this array with proper size and freeing it afterwards.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE               Successful
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_INVALID_PARAMETER  Non unique privileges passed
 * in first argument, privileges_count is more than 100 or other invalid parameter
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_UNKNOWN            Unknown error
 */
int ppm_check_permissions(const char **privileges, size_t privileges_count,
                          ppm_check_result_e *results);

/**
 * @platform
 * @brief Checks if an application, with given app_id, has permission to use the
 * given privileges.
 *
 * @since_tizen 5.0
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/permission.check
 *
 * @param[in]   app_id            The app_id of the app that is to be checked.
 * @param[in]   privileges        The privileges array that is to be checked.
 * @param[in]   privileges_count  The number of elements in the privileges and results arrays.
 * @param[out]  results           The results of the privilege check. Caller is responsible for
 *                                allocating this array with proper size and freeing it afterwards.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE               Successful
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_INVALID_PARAMETER  Non unique privileges passed
 * in first argument, privileges_count is more than 100 or other invalid parameter
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_UNKNOWN            Unknown error
 */
int ppm_check_app_permissions(const char *app_id, const char **privileges, size_t privileges_count,
                              ppm_check_result_e *results);

/**
 * @brief Requests a user's response to obtain permission for using the given privilege.
 *
 * @details When this function is called, an underlying service may show an appropriate
 * UI dialogue box (pop-up) with a question about granting the application access
 * to the given privilege. Once a user makes a decision, the service may modify
 * the privacy policy (when it is a definitive decision). After that, the service
 * sends the response back to the application. The possible response values are as follows:
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE\n
 * The application receives #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE value after
 * pressing 'Deny' button while not having selected the 'Don't ask again?' checkbox. If the device
 * has the home and back buttons, pressing either of them gives the
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE response.
 * The application is informed about the user's decision by invoking ppm_request_response_cb().
 * When a privacy policy for the given privilege has already been resolved, no pop-up will
 * be shown and the service will reply immediately with an appropriate result:
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER\n
 *
 * @since_tizen 4.0
 *
 * @remarks Before calling this function, call ppm_check_permission() to check if the application has
 * permission to use the given privilege. If the result of calling ppm_check_permission() is
 * #PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK, the application should call
 * this function to determine access to the privilege.
 *
 * @param[in]   privilege   The given privilege for which a pop-up must be shown.
 * @param[in]   callback    The given callback function which will be invoked
 *                          when the API receives a response.
 * @param[in]   user_data   User specific data which will be passed to
 *                          the given callback.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE                Successful
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_IO_ERROR            I/O error
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_ALREADY_IN_PROGRESS Operation already in progress
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_UNKNOWN             Unknown error
 *
 * @post ppm_request_response_cb() will be invoked.
 * @see ppm_request_response_cb()
 */
int ppm_request_permission(const char *privilege,
                           ppm_request_response_cb callback,
                           void *user_data);

/**
 * @brief Requests a user's response to obtain permission for using the given privileges.
 *
 * @details When this function is called, an underlying service may show an appropriate
 * UI dialogue box (pop-up) with a question about granting the application access
 * to the given privileges. Once a user makes a decision, the service may modify
 * the privacy policy (when it is a definitive decision). After that, the service
 * sends the response back to the application. The possible response values are as follows:\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE\n
 * The application receives #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE value after
 * pressing 'Deny' button while not having selected the 'Don't ask again?' checkbox. If the device
 * has the home and back buttons, pressing either of them gives the
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE response.
 * The application is informed about the user's decision by invoking ppm_request_multiple_response_cb().
 * When a privacy policy for the given privileges has already been resolved, no pop-up will
 * be shown and the service will reply immediately with an appropriate results:\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER\n
 * #PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER\n
 *
 * @since_tizen 5.0
 *
 * @remarks Before calling this function, call ppm_check_permission() or ppm_check_permissions()
 * to check if the application has permission to use the given privileges.
 * This function should be called for each privilege with result
 * #PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK returned from ppm_check_permission() or ppm_check_permissions().
 *
 * @param[in]   privileges        The given privileges array for which a pop-up must be shown.
 * @param[in]   privileges_count  The number of elements in the privileges array.
 * @param[in]   callback          The given callback function which will be invoked
 *                                when the API receives a response.
 * @param[in]   user_data         User specific data which will be passed to
 *                                the given callback.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE                Successful
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_IO_ERROR            I/O error
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_INVALID_PARAMETER   Non unique privileges passed
 * in first argument, privileges_count is more than 100 or other invalid parameter
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_OUT_OF_MEMORY       Out of memory
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_ALREADY_IN_PROGRESS Operation already in progress
 * @retval #PRIVACY_PRIVILEGE_MANAGER_ERROR_UNKNOWN             Unknown error
 *
 * @post ppm_request_multiple_response_cb() will be invoked.
 * @see ppm_request_multiple_response_cb()
 */
int ppm_request_permissions(const char **privileges, size_t privileges_count,
                            ppm_request_multiple_response_cb callback,
                            void *user_data);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __PRIVACY_PRIVILEGE_MANAGER_H__ */
