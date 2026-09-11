/*
 * Copyright (c) 2011-2013 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_NETWORK_CONNECTION_EXTENSION_H__
#define __TIZEN_NETWORK_CONNECTION_EXTENSION_H__

#include "net_connection.h"
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file connection_extension.h
 */

/**
 * @brief Enumeration for extended network connection type.
 * @since_tizen 5.0
 */
typedef enum {
	CONNECTION_PROFILE_TYPE_MESH = CONNECTION_PROFILE_TYPE_BT + 1,  /**< Wi-Fi Mesh type */
} connection_profile_type_extended_e;

/**
 * @brief Start TCP Dump.
 *
 * @param[in] connection        The connection handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                    Successful
 * @retval #CONNECTION_ERROR_OPERATION_FAILED        Operation failed
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER       Invalid parameter
*/
int connection_profile_start_tcpdump(connection_h connection);

/**
 * @brief Stop TCP Dump.
 *
 * @param[in] connection        The connection handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                    Successful
 * @retval #CONNECTION_ERROR_OPERATION_FAILED        Operation failed
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER       Invalid parameter
*/
int connection_profile_stop_tcpdump(connection_h connection);

/**
 * @brief Get TCP Dump Running Status.
 *
 * @param[in] connection        The connection handle
 * @param[out] tcpdump_state    The state of tcpdump, tcpdump is in progress or not
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                    Successful
 * @retval #CONNECTION_ERROR_OPERATION_FAILED        Operation failed
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER       Invalid parameter
*/
int connection_profile_get_tcpdump_state(connection_h connection, gboolean *tcpdump_state);

/**
 * @brief Creates a handle for managing data connections in C# API.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a handle using connection_destroy_cs().
 * @param[in]  tid              TID in C#
 * @param[out] connection       The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY         Out of memory
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @see connection_destroy_cs()
 */
int connection_create_cs(int tid, connection_h *connection);

/**
 * @brief Destroys the connection handle in C# API.
 * @since_tizen 5.0
 * @param[in]  tid              TID in C#
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @see connection_create_cs()
 */
int connection_destroy_cs(int tid, connection_h connection);

typedef enum {
	CONNECTION_MPTCP_DISABLE = 0,			// disable
	CONNECTION_MPTCP_ENABLE_ALL = 1,		// enable
	CONNECTION_MPTCP_ENABLE_SOCKOPT = 2,	// enable if the MPTCP_ENABLED socket option is set
} connection_mptcp_enable_e;

typedef enum {
	CONNECTION_MPTCP_PM_UNKNOWN,			// Unknown path manager
	CONNECTION_MPTCP_PM_DEFAULT,			// Default path manager - Do nothing
	CONNECTION_MPTCP_PM_FULLMESH,			// Full mesh of subflows
} connection_mptcp_path_manager_e;

typedef enum {
	CONNECTION_MPTCP_SCHEDULER_UNKNOWN,		// Unknown scheduler
	CONNECTION_MPTCP_SCHEDULER_DEFAULT,		// Default scheduler
	CONNECTION_MPTCP_SCHEDULER_ROUNDROBIN,	// Round robin scheduler
} connection_mptcp_scheduler_e;

/**
 * @brief Check if Multipath TCP is supported
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @param[out] supported	true if Multipath TCP is supported, false otherwise
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 */
int connection_mptcp_is_supported(connection_h connection, bool* supported);

/**
 * @brief Enable Multipath TCP
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @param[in] enable	The enabled value
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @retval #CONNECTION_ERROR_OPERATION_FAILED	Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED	Permission denied
 */
int connection_mptcp_enable(connection_h connection, connection_mptcp_enable_e enable);

/**
 * @brief Disable Multipath TCP
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @retval #CONNECTION_ERROR_OPERATION_FAILED	Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED	Permission denied
 */
int connection_mptcp_disable(connection_h connection);

/**
 * @brief Get the enabled value for Multipath TCP
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @param[out] enable	The enabled value
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @retval #CONNECTION_ERROR_OPERATION_FAILED	Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED	Permission denied
 */
int connection_mptcp_get_enabled(connection_h connection, connection_mptcp_enable_e* enable);

/**
 * @brief Set the path manager of Multipath TCP
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @param[in] pm	The path manager of Multipath TCP
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @retval #CONNECTION_ERROR_OPERATION_FAILED	Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED	Permission denied
 */
int connection_mptcp_set_path_manager(connection_h connection, connection_mptcp_path_manager_e pm);

/**
 * @brief Get the path manager of Multipath TCP
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @param[out] pm	The path manager of Multipath TCP
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @retval #CONNECTION_ERROR_OPERATION_FAILED	Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED	Permission denied
 */
int connection_mptcp_get_path_manager(connection_h connection, connection_mptcp_path_manager_e* pm);

/**
 * @brief Set the path manager of Multipath TCP
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @param[in] scheduler		The schedduler of Multipath TCP
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @retval #CONNECTION_ERROR_OPERATION_FAILED	Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED	Permission denied
 */
int connection_mptcp_set_scheduler(connection_h connection, connection_mptcp_scheduler_e scheduler);

/**
 * @brief Set the path manager of Multipath TCP
 * @since_tizen 5.0
 * @param[in] connection	The connection handle
 * @param[out] scheduler		The schedduler of Multipath TCP
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE				Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @retval #CONNECTION_ERROR_OPERATION_FAILED	Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED	Permission denied
 */
int connection_mptcp_get_scheduler(connection_h connection, connection_mptcp_scheduler_e* scheduler);

/**
 * @brief Enumeration for EAP type.
 * @since_tizen 6.0
 */
typedef enum {
	CONNECTION_ETHERNET_EAP_TYPE_MD5 = 0,        /**< EAP MD5 type */
	CONNECTION_ETHERNET_EAP_TYPE_PEAP = 1,       /**< EAP PEAP type */
	CONNECTION_ETHERNET_EAP_TYPE_TLS = 2,          /**< EAP TLS type */
	CONNECTION_ETHERNET_EAP_TYPE_TTLS = 3,        /**< EAP TTLS type */
	CONNECTION_ETHERNET_EAP_TYPE_FAST = 4,        /**< EAP FAST type */
} connection_ethernet_eap_type_e;

/**
 * @brief Enumeration for EAP phase2 authentication type.
 * @since_tizen 6.0
 */
typedef enum {
	CONNECTION_ETHERNET_EAP_AUTH_TYPE_NONE = 0,            /**< EAP phase2 authentication none */
	CONNECTION_ETHERNET_EAP_AUTH_TYPE_PAP = 1,                /**< EAP phase2 authentication PAP */
	CONNECTION_ETHERNET_EAP_AUTH_TYPE_MSCHAP = 2,       /**< EAP phase2 authentication MSCHAP */
	CONNECTION_ETHERNET_EAP_AUTH_TYPE_MSCHAPV2 = 3,  /**< EAP phase2 authentication MSCHAPv2 */
	CONNECTION_ETHERNET_EAP_AUTH_TYPE_GTC = 4,               /**< EAP phase2 authentication GTC */
	CONNECTION_ETHERNET_EAP_AUTH_TYPE_MD5 = 5,             /**< EAP phase2 authentication MD5 */
} connection_ethernet_eap_auth_type_e;

/**
 * @brief Enumeration for PEAP version.
 * @since_tizen 6.0
 */
typedef enum {
	CONNECTION_ETHERNET_EAP_PEAP_VERSION_AUTO = 0, /**< PEAP version Automatic */
	CONNECTION_ETHERNET_EAP_PEAP_VERSION_0 = 1,         /**< PEAP version 0 */
	CONNECTION_ETHERNET_EAP_PEAP_VERSION_1 = 2,         /**< PEAP version 1 */
} connection_ethernet_eap_peap_version_e;

/**
 * @brief Called for each found ethernet configuration.
 * @since_tizen 6.0
 * @param[in] profile		The profile handle
 * @param[in] user_data		The user data passed from the request function
 * @return @c true to continue with the next iteration of the loop, otherwise @c false to break out of the loop \n
 * @pre connection_profile_foreach_ethernet_eap_config() will invoke this callback.
 * @see connection_profile_foreach_ethernet_eap_config()
 */
typedef bool (*connection_profile_list_cb)(connection_profile_h profile, void *user_data);

/**
 * @brief Gets configurations of an ethernet.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile
 *
 * @param[in] connection	The connection handle
 * @param[in] callback		The callback to be called
 * @param[in] user_data		The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 * @pre This function needs connection_create() before use.
 * @post This function invokes connection_profile_list_cb().
 */
int connection_profile_foreach_ethernet_eap_config(connection_h connection,
						connection_profile_list_cb callback, void *user_data);

/**
 * @brief Enable EAP over ethernet network
 * @since_tizen 6.0
 * @param[in] profile		The profile handle
 * @param[in] enable		The flag to enable or disable EAP over ethernet
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE			Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED		Permission denied
 * @retval #CONNECTION_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 */
int connection_profile_enable_ethernet_eap(connection_profile_h profile, bool enable);

/**
 * @brief Check if EAP over ethernet is enabled.
 * @since_tizen 6.0
 * @param[in] profile		The profile handle
 * @param[out] enabled		true if EAP over ethernet is enabled, false otherwise
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE			Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION		Invalid operation
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED		Permission denied
 * @retval #CONNECTION_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 */
int connection_profile_is_ethernet_eap_enabled(connection_profile_h profile, bool *enabled);

/**
 * @brief Sets the EAP type of ethernet.
 * @since_tizen 6.0
 * @param[in] profile		The profile handle
 * @param[in] type              The type of EAP
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_type(connection_profile_h profile, connection_ethernet_eap_type_e type);

/**
 * @brief Gets the EAP type of ethernet.
 * @since_tizen 6.0
 * @param[in] profile		The profile handle
 * @param[out] type             The type of EAP
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_type(connection_profile_h profile, connection_ethernet_eap_type_e *type);

/**
 * @brief Sets the type of EAP phase2 authentication of ethernet.
 * @since_tizen 6.0
 * @param[in] profile		The profile handle
 * @param[in] type		The type of EAP phase2 authentication
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE			Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION		Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 * @pre This function needs connection_profile_set_ethernet_eap_type() before use.
 */
int connection_profile_set_ethernet_eap_auth_type(connection_profile_h profile,
						connection_ethernet_eap_auth_type_e type);

/**
 * @brief Gets the type of EAP phase2 authentication of ethernet.
 * @since_tizen 6.0
 * @param[in] profile		The profile handle
 * @param[out] type		The type of EAP phase2 authentication
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_auth_type(connection_profile_h profile,
						connection_ethernet_eap_auth_type_e *type);

/**
 * @brief Sets anonymous identity of EAP over ethernet.
 * @since_tizen 6.0
 * @param[in] profile			The profile handle
 * @param[in] anonymous_identity	The anonymous identity
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_anonymous_identity(connection_profile_h profile,
							const char *anonymous_identity);

/**
 * @brief Gets anonymous identity of EAP over ethernet.
 * @since_tizen 6.0
 * @remarks You must release @a anonymous_identity using free().
 * @param[in] profile			The profile handle
 * @param[out] anonymous_identity   The anonymous identity of ethernet.
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY                Out of memory
 * @retval #CONNECTION_ERROR_INVALID_OPERATION            Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED                Not supported
 */
int connection_profile_get_ethernet_eap_anonymous_identity(connection_profile_h profile,
							char **anonymous_identity);

/**
 * @brief Sets the CA Certificate of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS,
 *	#CONNECTION_ETHERNET_EAP_TYPE_TTLS or #CONNECTION_ETHERNET_EAP_TYPE_PEAP.
 * @param[in] profile		The profile handle
 * @param[in] file              The file path of CA Certificate
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_ca_cert_file(connection_profile_h profile, const char *file);

/**
 * @brief Gets the CA Certificate of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS,
 *	#CONNECTION_ETHERNET_EAP_TYPE_TTLS or #CONNECTION_ETHERNET_EAP_TYPE_PEAP.
 *	You must release @a file using free().
 * @param[in] profile		The profile handle
 * @param[out] file		The file path of CA Certificate
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_ca_cert_file(connection_profile_h profile, char **file);

/**
 * @brief Sets the Client Certificate of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS.
 * @param[in] profile		The profile handle
 * @param[out] file		The file path of Client Certificate
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_client_cert_file(connection_profile_h profile, const char *file);

/**
 * @brief Gets the Client Certificate of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS.
 *	You must release @a file using free().
 * @param[in] profile		The profile handle
 * @param[out] file             The file path of Client Certificate
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_client_cert_file(connection_profile_h profile, char **file);

/**
 * @brief Sets the PAC file of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_FAST.
 * @param[in] profile		The profile handle
 * @param[out] file		The path of PAC file
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_pac_file(connection_profile_h profile, const char *file);

/**
 * @brief Gets the PAC file of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_FAST.
 *	You must release @a file using free().
 * @param[in] profile		The profile handle
 * @param[out] file             The path of PAC file
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_pac_file(connection_profile_h profile, char **file);

/**
 * @brief Sets the version of EAP-PEAP of ethernet.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_PEAP.
 * @param[in] profile		The profile handle
 * @param[in] version		The version of EAP-PEAP.
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE			Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER		Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION		Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 */
int connection_profile_set_ethernet_eap_peap_version(connection_profile_h profile,
						connection_ethernet_eap_peap_version_e version);

/**
 * @brief Gets the version of EAP-PEAP of ethernet.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_PEAP.
 * @param[in] profile		The profile handle
 * @param[out] version		The version of EAP-PEAP.
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_peap_version(connection_profile_h profile,
						connection_ethernet_eap_peap_version_e *version);

/**
 * @brief Sets the identity of EAP over ethernet.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS.
 * @param[in] profile		The profile handle
 * @param[in] identity		The identity
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_identity(connection_profile_h profile, const char *identity);

/**
 * @brief Gets the identity of EAP over ethernet.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS.
 *	You must release @a identity using free().
 * @param[in] profile		The profile handle
 * @param[out] identity		The identity of ethernet.
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE			Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval #CONNECTION_ERROR_INVALID_OPERATION	Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED		Not supported
 */
int connection_profile_get_ethernet_eap_identity(connection_profile_h profile, char **identity);

/**
 * @brief Sets the private key information of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS.
 * @param[in] profile		The profile handle
 * @param[in] file              The file path of private key
 * @param[in] password          The password
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_private_key_info(connection_profile_h profile,
							const char *file, const char *password);
/**
 * @brief Gets the private key file of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_TLS.
 *	You must release @a file using free().
 * @param[in] profile		The profile handle
 * @param[out] file             The file path of private key
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_private_key_file(connection_profile_h profile, char **file);

/**
 * @brief Sets the user name and passphrase of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_MD5,
 *	#CONNECTION_ETHERNET_EAP_TYPE_PEAP, #CONNECTION_ETHERNET_EAP_TYPE_TTLS or #CONNECTION_ETHERNET_EAP_TYPE_FAST.
 * @param[in] profile		The profile handle
 * @param[in] user_name         The user name
 * @param[in] password          The password
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_set_ethernet_eap_passphrase(connection_profile_h profile,
							const char *user_name, const char *password);

/**
 * @brief Gets the username and password status of EAP.
 * @since_tizen 6.0
 * @remarks This function is valid only if the EAP type is #CONNECTION_ETHERNET_EAP_TYPE_MD5,
 *	#CONNECTION_ETHERNET_EAP_TYPE_PEAP, #CONNECTION_ETHERNET_EAP_TYPE_TTLS or #CONNECTION_ETHERNET_EAP_TYPE_FAST.
 *	You must release @a user_name using free().
 * @param[in] profile		The profile handle
 * @param[out] user_name        The user name
 * @param[out] is_password_set  @c true if password is set,
 *                otherwise @c false if password is not set.
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_profile_get_ethernet_eap_passphrase(connection_profile_h profile,
						char **user_name, bool *is_password_set);

/**
 * @brief Removes configuration of ethernet profile.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile
 *
 * @param[in] connection	The connection handle
 * @param[in] profile		The profile handle
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 * @see connection_profile_create()
 * @see connection_profile_foreach_ethernet_eap_config()
 */
int connection_profile_remove_ethernet_eap_config(connection_h connection,
							connection_profile_h profile);

/**
 * @brief Saves configuration of ethernet profile.
 * @details When a configuration is changed, these changes will be not applied to the Connection Manager immediately.\n
 *            When you call this function, your changes affect the Connection Manager and the existing configuration is updated.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile
 *
 * @param[in] connection	The connection handle
 * @param[in] profile		The profile handle
 * @return 0 on success, otherwise negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 * @see connection_profile_create()
 */
int connection_profile_save_ethernet_eap_config(connection_h connection,
							connection_profile_h profile);

/**
 * @brief Check if the time has been updated by NTP.
 * @since_tizen 6.5
 * @param[in] connection	The connection handle
 * @param[out] updated		true if the system time is updated by NTP, false otherwise
 * @return 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_clock_is_updated(connection_h connection, bool *updated);

/**
 * @brief Sets NTP server.
 * @since_tizen 8.0
 * @param[in] connection	The connection handle
 * @param[in] ntp_server	requested NTP server IP/URL
 * @return 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_set_ntp_server(connection_h connection, const char *ntp_server);

/**
 * @brief Get NTP server url.
 * @since_tizen 8.0
 * @remarks You must release @a ntp_server using free().
 * @param[in] connection	The connection handle
 * @param[out] ntp_server       The IP/URL of NTP Server
 * @return 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_get_ntp_server(connection_h connection, char **ntp_server);

/**
 * @brief Clear NTP server.
 * @since_tizen 8.0
 * @param[in] connection	The connection handle
 * @return 0 on success, otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                 Successful
 * @retval #CONNECTION_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED        Not supported
 */
int connection_clear_ntp_server(connection_h connection);

/**
* @}
*/


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __TIZEN_NETWORK_CONNECTION_EXTENSION_H__ */
