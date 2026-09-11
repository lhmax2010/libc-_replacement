/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd. All rights reserved.
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

#pragma once

/**
 * @brief Definition for AUL: The process ID of the caller.
 * @since_tizen 2.2
 */
#define AUL_K_CALLER_PID                "__AUL_CALLER_PID__"

/**
 * @brief Definition for AUL: The process ID of the callee.
 * @since_tizen 2.2
 */
#define AUL_K_CALLEE_PID                "__AUL_CALLEE_PID__"

/**
 * @brief Definition for AUL: The user ID of the caller.
 * @since_tizen 3.0
 */
#define AUL_K_CALLER_UID                "__AUL_CALLER_UID__"

/**
 * @brief Definition for AUL: The user ID of the callee.
 * @since_tizen 3.0
 */
#define AUL_K_CALLEE_UID                "__AUL_CALLEE_UID__"

/**
 * @brief Definition for AUL: The user ID of the taget user.
 * @since_tizen 3.0
 */
#define AUL_K_TARGET_UID                "__AUL_TARGET_UID__"

/**
 * @brief Definition for AUL: The application ID of the caller.
 * @since_tizen 2.2
 */
#define AUL_K_CALLER_APPID              "__AUL_CALLER_APPID__"

/**
 * @brief Definitnon for AUL: The application ID of the callee.
 * @since_tizen 2.2
 */
#define AUL_K_CALLEE_APPID              "__AUL_CALLEE_APPID__"

/**
 * @deprecated Deprecated since 3.0
 * @brief Definition for AUL: The first argument.
 * @since_tizen 2.2
 */
#define AUL_K_ARGV0                     "__AUL_ARGV0__"

/**
 * @brief Definition for AUL: The start time.
 * @since_tizen 2.2
 */
#define AUL_K_STARTTIME                 "__AUL_STARTTIME__"

/**
 * @brief Definition for AUL: The MIME-Type data.
 * @since_tizen 2.2
 */
#define AUL_K_MIME_TYPE                 "__AUL_MIME_TYPE__"

/**
 * @brief Definition for AUL: The unaliased MIME-Type data.
 * @since_tizen 2.2
 */
#define AUL_K_UNALIASED_MIME_TYPE       "__AUL_UNALIASED_MIME_TYPE__"

/**
 * @brief Definition for AUL: The content of the MIME-Type data.
 * @since_tizen 2.2
 */
#define AUL_K_MIME_CONTENT              "__AUL_MIME_CONTENT__"

/**
 * @brief Definition for AUL: The service name.
 * @since_tizen 2.2
 */
#define AUL_K_SERVICE_NAME              "__AUL_SERVICE_NAME__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the app-selector is launched forcedly.
 * @since_tizen 2.2
 */
#define AUL_K_FORCE_LAUNCH_APP_SELECTOR "__AUL_FORCE_LAUNCH_APP_SELECTOR__"

/**
 * @brief Definition for AUL: The flag if it's set, the request is for debugging web-applications.
 * @since_tizen 3.0
 */
#define AUL_K_DEBUG                     "__AUL_DEBUG__"

/**
 * @brief Definition for AUL: The SDK data.
 * @since_tizen 2.3
 */
#define AUL_K_SDK                       "__AUL_SDK__"

/**
 * @deprecated Deprecated since 3.0
 * @brief Definition for AUL: The multiple key.
 * @since_tizen 2.3
 */
#define AUL_K_MULTI_KEY                 "__AUL_MULTI_KEY__"

/**
 * @deprecated Deprecated since 3.0
 * @brief Definition for AUL: The multiple key event.
 * @since_tizen 2.3
 */
#define AUL_K_MULTI_KEY_EVENT           "__AUL_MULTI_KEY_EVENT__"

/**
 * @brief Definition for AUL: The privacy application ID.
 * @since_tizen 2.3
 */
#define AUL_K_PRIVACY_APPID             "__AUL_PRIVACY_APPID__"

/**
 * @deprecased Deprecated since tizen 3.0
 * @brief Definition for AUL: The key pressed event.
 * @since_tizen 2.3
 */
#define AUL_V_KEY_PRESSED               "__AUL_KEY_PRESSED__"

/**
 * @deprecased Deprecated since tizen 3.0
 * @brief Definition for AUL: The key released event.
 * @since_tizen 2.3
 */
#define AUL_V_KEY_RELEASED              "__AUL_KEY_RELEASED__"

/**
 * @brief Definition for RUA: The caller information.
 * @since_tizen 3.0
 */
#define AUL_SVC_K_RUA_STAT_CALLER       "__K_RUA_STAT_CALLER__"

/**
 * @brief Definition for RUA: The tag information.
 * @since_tizen 3.0
 */
#define AUL_SVC_K_RUA_STAT_TAG          "__K_RUA_STAT_TAG__"

/**
 * @brief Definition for RUA: The application ID.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_PKGNAME               "__K_RUA_PKGNAME"

/**
 * @brief Definition for RUA: The application path.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_APPPATH               "__K_RUA_APPPATH"

/**
 * @brief Definition for RUA: The arguments.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_ARG                   "__K_RUA_ARG"

/**
 * @brief Definition for RUA: The launched time.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_TIME                  "__K_RUA_TIME"

/**
 * @brief Definition for RUA: The instance ID.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_INSTANCE_ID           "__K_RUA_INSTANCE_ID"

/**
 * @brief Definition for RUA: The instance name.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_INSTANCE_NAME         "__K_RUA_INSTANCE_NAME"

/**
 * @brief Definition for RUA: The icon path.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_ICON                  "__K_RUA_ICON"

/**
 * @brief Definition for RUA: The URI data.
 * @since_tizen 3.0
 */
#define AUL_K_RUA_URI                   "__K_RUA_URI"

/**
 * @brief Definition for AUL: The application ID.
 * @since_tizen 2.2
 */
#define AUL_K_PKG_NAME                  "__AUL_PKG_NAME__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the caller process waits to get the result from the callee application.
 * @since_tizen 2.2
 */
#define AUL_K_WAIT_RESULT               "__AUL_WAIT_RESULT__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the request is for sending the result.
 * @since_tizen 2.2
 */
#define AUL_K_SEND_RESULT               "__AUL_SEND_RESULT__"

/**
 * @brief Definition for AUL: The taskmanage value.
 * @since_tizen 2.2
 */
#define AUL_K_TASK_MANAGE               "__AUL_TASK_MANAGE__"

/**
 * @brief Definition for AUL: The application type.
 * @since_tizen 2.2
 */
#define AUL_K_APP_TYPE                  "__AUL_APP_TYPE__"

/**
 * @brief Definition for AUL: The process ID of the original caller.
 * @since_tizen 2.2
 */
#define AUL_K_ORG_CALLER_PID            "__AUL_ORG_CALLER_PID__"

/**
 * @brief Definition for AUL: The process ID of the forwarded callee.
 * @since_tizen 2.2
 */
#define AUL_K_FWD_CALLEE_PID            "__AUL_FWD_CALLEE_PID__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the request is not cancelled.
 * @since_tizen 2.2
 */
#define AUL_K_NO_CANCEL                 "__AUL_NO_CANCEL__"

/**
 * @brief Definition for AUL: The executable file path.
 * @since_tizen 2.2
 */
#define AUL_K_EXEC                      "__AUL_EXEC__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the application can be launched by multiple processes.
 * @since_tizen 2.2
 */
#define AUL_K_MULTIPLE                  "__AUL_MULTIPLE__"

/**
 * @brief Definition for AUL: The type of the package.
 * @since_tizen 2.2
 */
#define AUL_K_PACKAGETYPE               "__AUL_PACKAGETYPE__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the application uses hardware-acceleration.
 * @since_tizen 2.2
 */
#define AUL_K_HWACC                     "__AUL_HWACC__"

/**
 * @brief Definition for AUL: The application ID.
 * @since_tizen 2.2
 */
#define AUL_K_APPID                     "__AUL_APPID__"

/**
 * @brief Definition for AUL: The process ID.
 * @since_tizen 2.2
 */
#define AUL_K_PID                       "__AUL_PID__"

/**
 * @brief Definition for AUL: The window ID.
 * @since_tizen 2.4
 */
#define AUL_K_WID                       "__AUL_WID__"

/**
 * @breif Definition for AUL: The process ID of the leader process.
 * @since_tizen 2.4
 */
#define AUL_K_LEADER_PID                "__AUL_LEADER_PID__"

/**
 * @brief Definition for Data-Control: The type of data-control.
 * @since_tizen 2.2
 */
#define AUL_K_DATA_CONTROL_TYPE         "__AUL_DATA_CONTROL_TYPE__"

/**
 * @brief Definition for AUL: The package ID.
 * @since_tizen 2.2
 */
#define AUL_K_PKGID                     "__AUL_PKGID_"

/**
 * @deprecated Deprecated since 3.0
 * @brief Definition for AUL: The flag if it's 'true', the application can be launched by internal process-pool.
 * @since_tizen 2.2
 */
#define AUL_K_INTERNAL_POOL             "__AUL_INTERNAL_POOL__"

/**
 * @brief Definition for AUL: The TEP resource path.
 * @since_tizen 2.4
 */
#define AUL_K_TEP_PATH                  "__AUL_TEP_PATH__"

/**
 * @brief Definition for AUL: The component type.
 * @since_tizen 2.2
 */
#define AUL_K_COMP_TYPE                 "__AUL_COMP_TYPE__"

/**
 * @brief Definition for AUL: The loader ID.
 * @since_tizen 3.0
 */
#define AUL_K_LOADER_ID                 "__AUL_LOADER_ID__"

/**
 * @brief Definition for AUL: The executable file path of the loader.
 * @since_tizen 3.0
 */
#define AUL_K_LOADER_PATH               "__AUL_LOADER_PATH__"

/**
 * @brief Definition for AUL: The extra data of the loader.
 * @since_tizen 3.0
 */
#define AUL_K_LOADER_EXTRA              "__AUL_LOADER_EXTRA__"

/**
 * @brief Definition for AUL: The loader name.
 * @since_tizen 3.0
 */
#define AUL_K_LOADER_NAME               "__AUL_LOADER_NAME__"

/**
 * @brief Definition for AUL: The wayland display.
 * @since_tizen 3.0
 */
#define AUL_K_WAYLAND_DISPLAY           "__AUL_WAYLAND_DISPLAY__"

/**
 * @brief Definition for AUL: The wayland working directory path.
 * @since_tizen 3.0
 */
#define AUL_K_WAYLAND_WORKING_DIR       "__AUL_WAYLAND_WORKING_DIR__"

/**
 * @brief Definition for AUL: The sender process ID of app-com.
 * @since_tizen 3.0
 */
#define AUL_K_COM_SENDER_PID            "__AUL_COM_SENDER_PID__"

/**
 * @brief Definition for AUL: The endpoint of app-com.
 * @since_tizen 3.0
 */
#define AUL_K_COM_ENDPOINT              "__AUL_COM_ENDPOINT__"

/**
 * @brief Definition for AUL: The privilege of app-com.
 * @since_tizen 3.0
 */
#define AUL_K_COM_PRIVILEGE             "__AUL_COM_PRIVILEGE__"

/**
 * @brief Definition for AUL: The propagate information of app-com.
 * @since_tizen 3.0
 */
#define AUL_K_COM_PROPAGATE             "__AUL_COM_PROPAGATE__"

/**
 * @brief Definition for AUL: The filter of app-com.
 * @since_tizen 3.0
 */
#define AUL_K_COM_FILTER                "__AUL_COM_FILTER__"

/**
 * @brief Definition for AUL: The result of app-com.
 * @since_tizen 3.0
 */
#define AUL_K_COM_RESULT                "__AUL_COM_RESULT__"

/**
 * @brief Definition for AUL: The root path.
 * @since_tizen 3.0
 */
#define AUL_K_ROOT_PATH                 "__AUL_ROOT_PATH__"

/**
 * @brief Definition for AUL: The sequence number.
 * @since_tizen 3.0
 */
#define AUL_K_SEQ_NUM                   "__AUL_SEQ_NUM__"

/**
 * @brief Definition for AUL: The API version.
 * @since_tizen 3.0
 */
#define AUL_K_API_VERSION               "__AUL_API_VERSION__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the application can be running in the background.
 * @since_tizen 2.4
 */
#define AUL_K_ALLOWED_BG                "__AUL_ALLOWED_BG__"

/**
 * @brief Definition for AUL: The owner process ID of app-group.
 * @since_tizen 2.4
 */
#define AUL_K_OWNER_PID                 "__AUL_OWNER_PID__"

/**
 * @brief Definition for AUL: The child process ID of app-group.
 * @since_tizen 2.4
 */
#define AUL_K_CHILD_PID                 "__AUL_CHILD_PID__"

/**
 * @brief Definition for Widget: The widget viewer.
 * @since_tizen 3.0
 */
#define AUL_K_WIDGET_VIEWER             "__AUL_WIDGET_VIEWER__"

/**
 * @brief Definition for Widget: The widget ID.
 * @since_tizen 3.0
 */
#define AUL_K_WIDGET_ID                 "__AUL_WIDGET_ID__"

/**
 * @brief Definition for Widget: The instance ID of the widget.
 * @since_tizen 3.0
 */
#define AUL_K_WIDGET_INSTANCE_ID        "__AUL_WIDGET_INSTANCE_ID__"

/**
 * @brief Definition for Widget: The status of the widget.
 * @since_tizen 3.0
 */
#define AUL_K_WIDGET_STATUS             "__AUL_WIDGET_STATUS__"

/**
 * @brief Definition for Widget: The error code of the widget.
 * @since_tizen 3.0
 */
#define AUL_K_WIDGET_ERROR_CODE         "__AUL_WIDGET_ERROR_CODE__"

/**
 * @brief Definition for Widget: The widget content infomation.
 * @since_tizen 3.0
 */
#define AUL_K_WIDGET_CONTENT_INFO       "__AUL_WIDGET_CONTENT_INFO__"

/**
 * @brief Definition for AUL: The target process ID.
 * @since_tizen 3.0
 */
#define AUL_K_TARGET_PID                "__AUL_TARGET_PID__"

/**
 * @brief Definition for AUL: The alias application ID.
 * @since_tizen 3.0
 */
#define AUL_K_ALIAS_APPID               "__AUL_ALIAS_APPID__"

/**
 * @brief Definition for AUL: The original application ID.
 * @since_tizen 3.0
 */
#define AUL_K_ORG_APPID                 "__AUL_ORG_APPID__"

/**
 * @brief Definition for AUL: The status value.
 * @since_tizen 3.0
 */
#define AUL_K_STATUS                    "__AUL_STATUS__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the application is launched by sub-application of app-group.
 * @since_tizen 2.4
 */
#define AUL_K_IS_SUBAPP                 "__AUL_IS_SUBAPP__"

/**
 * @brief Definition for AUL: The instance ID.
 * @since_tizen 4.0
 */
#define AUL_K_INSTANCE_ID               "__AUL_INSTANCE_ID__"

/**
 * @brief Definition for AUL: The user ID of the original caller.
 * @since_tizen 3.0
 */
#define AUL_K_ORG_CALLER_UID            "__AUL_ORG_CALLER_UID__"

/**
 * @brief Definition for AUL: The checksum data to store app-control information.
 * @details This definition is to improve the launch performance.
 * @since_tizen 3.0
 */
#define AUL_K_CHECKSUM                  "__AUL_CHECKSUM__"

/**
 * @brief Definition for Screen-Connector: The flag if it's 'true', the request type is private.
 * @since_tizen 3.0
 */
#define AUL_K_PRIVATE                   "__AUL_PRIVATE__"

/**
 * @brief Definition for Screen-Connector: The screen type.
 * @since_tizen 3.0
 */
#define AUL_K_SCREEN_TYPE               "__AUL_SCREEN_TYPE__"

/**
 * @brief Definition for Screen-Connector: The reference count of the viewer.
 * @since_tizen 3.0
 */
#define AUL_K_VIEWER_REF                "__AUL_VIEWER_REF__"

/**
 * @brief Definition for AUL: The application ID of the parent for transient-for.
 * @since_tizen 3.0
 */
#define AUL_K_PARENT_APPID              "__AUL_PARENT_APPID__"

/**
 * @brief Definition for AUL: The application ID of the child for transient-for.
 * @since_tizen 3.0
 */
#define AUL_K_CHILD_APPID               "__AUL_CHILD_APPID__"

/**
 * @deprecated Deprecated since 4.0
 * @brief Definition for AUL: The flag if it's 'true', the request is for fast launch.
 * @since_tizen 3.0
 */
#define AUL_K_FAST_LAUNCH               "__AUL_FAST_LAUNCH__"

/**
 * @brief Definition for Widget: The flag if it's 'true', the widget is faulted.
 * @since_tizen 3.0
 */
#define AUL_K_IS_FAULT                  "__AUL_IS_FAULT__"

/**
 * @brief Definition for AUL: The profile.
 * @since_tizen 3.0
 */
#define AUL_K_PROFILE                   "__AUL_PROFILE__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the application is installed for global user.
 * @since_tizen 3.0
 */
#define AUL_K_IS_GLOBAL                 "__AUL_IS_GLOBAL__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the request is to create a new instance.
 * @since_tizen 3.0
 */
#define AUL_K_NEW_INSTANCE              "__AUL_NEW_INSTANCE__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the application is installed.
 * @since_tizen 3.0
 */
#define AUL_K_IS_INSTALLED              "__AUL_IS_INSTALLED__"

/**
 * @brief Definition for AUL: The request type.
 * @details This key is only for Application Manager.
 * @since_tizen 4.0
 */
#define AUL_K_REQUEST_TYPE              "__AUL_REQUEST_TYPE__"

/**
 * @brief Definition for AUL: The application ID of the original caller.
 * @since_tizen 3.0
 */
#define AUL_K_ORG_CALLER_APPID          "__AUL_ORG_CALLER_APPID__"

/**
 * @brief Definition for AUL: The package ID of the original caller.
 * @since_tizen 3.0
 */
#define AUL_K_ORG_CALLER_PKGID          "__AUL_ORG_CALLER_PKGID__"

/**
 * @brief Definition for AUL: The installed storage.
 * @since_tizen 2.4
 */
#define AUL_K_INSTALLED_STORAGE         "__AUL_INSTALLED_STORAGE__"

/**
 * @brief Definition for AUL: The component ID.
 * @since_tizen 5.5
 */
#define AUL_K_COMPONENT_ID              "__AUL_COMPONENT_ID__"

/**
 * @brief Definition for AUL: The instance ID of the caller.
 * @since_tizen 5.5
 */
#define AUL_K_CALLER_INSTANCE_ID        "__AUL_CALLER_INSTANCE_ID__"

/**
 * @brief Definition for AUL: The instance ID of the original caller.
 * @since_tizen 5.5
 */
#define AUL_K_ORG_CALLER_INSTANCE_ID    "__AUL_ORG_CALLER_INSTANCE_ID__"

/**
 * @brief Definition for AUL: The IDs of the app group leader.
 * @since_tizen 5.5
 */
#define AUL_K_LEADER_IDS                "__AUL_LEADER_IDS__"

/**
 * @brief Definition for AUL: The ID of the app group leader.
 * @since_tizen 5.5
 */
#define AUL_K_LEADER_ID                 "__AUL_LEADER_ID__"

/**
 * @brief Definition for AUL: The flag of the foreground.
 * @since_tizen 5.5
 */
#define AUL_K_FG_FLAG                   "__AUL_FG_FLAG__"

/**
 * @brief Definition for AUL: The type of the component.
 * @since_tizen 5.5
 */
#define AUL_K_COMPONENT_TYPE            "__AUL_COMPONENT_TYPE__"

/**
 * @breif Definition for AUL: The flag if it's 'true', the component is sub component of the group.
 * @since_tizen 5.5
 */
#define AUL_K_IS_SUB_COMP               "__AUL_IS_SUB_COMP__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the component is the main component of the application.
 * @since_tizen 5.5
 */
#define AUL_K_MAIN_COMP                 "__AUL_MAIN_COMP__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the icon of the component should be display.
 * @since_tizen 5.5
 */
#define AUL_K_ICON_DISPLAY              "__AUL_ICON_DISPLAY__"

/**
 * @brief Definition for AUL: The localized information.
 * @since_tizen 5.5
 */
#define AUL_K_LOCALIZED_INFO            "__AUL_LOCALIZED_INFO__"

/**
 * @brief Definition for AUL: The size of the localized information.
 * @since_tizen 5.5
 */
#define AUL_K_LOCALIZED_INFO_SIZE       "__AUL_LOCALIZED_INFO_SIZE__"

/**
 * @brief Definition for AUL: The locale.
 * @since_tizen 5.5
 */
#define AUL_K_LOCALE                    "__AUL_LOCALE__"

/**
 * @brief Definition for AUL: The icon path.
 * @since_tizen 5.5
 */
#define AUL_K_ICON                      "__AUL_ICON__"

/**
 * @brief Definition for AUL: The label.
 * @since_tizen 5.5
 */
#define AUL_K_LABEL                     "__AUL_LABEL__"

/**
 * @brief Definition for AUL: The launch mode.
 * @since_tizen 5.5
 */
#define AUL_K_LAUNCH_MODE               "__AUL_LAUNCH_MODE__"

/**
 * @brief Definition for AUL: The interval.
 * @since_tizen 5.5
 */
#define AUL_K_INTERVAL                  "__AUL_INTERVAL__"

/**
 * @brief Definition for AUL: The command.
 * @since_tizen 5.5
 */
#define AUL_K_COMMAND                   "__AUL_COMMAND__"

/**
 * @brief Definition for AUL: The name for the launcher service.
 * @since_tizen 5.5
 */
#define AUL_K_LAUNCHER_SERVICE          "__K_LAUNCHER_SERVICE__"

/**
 * @brief Definition for AUL: The unqiue ID of the launcher service.
 * @since_tizen 5.5
 */
#define AUL_K_LAUNCHER_SERVICE_SERIAL   "__K_LAUNCHER_SERVICE_SERIAL__"

/**
 * @brief Definition for AUL: The event of the launcher service.
 * @since_tizen 5.5
 */
#define AUL_K_LAUNCHER_SERVICE_EVENT    "__K_LAUNCHER_SERVICE_EVENT__"

/**
 * @brief Definition for AUL: The name of the app-defined loader.
 * @since_tizen 5.5
 */
#define AUL_K_APP_DEFINED_LOADER        "__K_APP_DEFINED_LOADER__"

/**
 * @brief Definition for AUL: The widget disable.
 * @since_tizen 5.5
 */
#define AUL_K_WIDGET_DISABLE            "__AUL_WIDGET_DISABLE__"

/**
 * @brief Definition for AUL: The flag of the multiple instance shortcut.
 * @details If it's true, the request is for the multiple instance shortcut.
 * @since_tizen 5.5
 */
#define AUL_K_MULTI_INSTANCE_SHORTCUT   "__AUL_MULTI_INSTANCE_SHORTCUT__"

/**
 * @brief Definition for AUL: The event name.
 * @since_tizen 5.5
 */
#define AUL_K_EVENT_NAME                "__AUL_EVENT_NAME__"

/**
 * @brief Definition for AUL: The event data.
 * @since_tizen 5.5
 */
#define AUL_K_EVENT_DATA                "__AUL_EVENT_DATA__"

/**
 * @brief Definition for AUL: The value for supporting positioning of the app group.
 * @details If the key exists, the callee app group will be inserted into
 *          the app group before the given position.
 * @since_tizen 6.0
 */
#define AUL_K_INSERT_BEFORE_WINDOW      "__K_INSERT_BEFORE_WINDOW"

/**
 * @brief Definition for AUL: The list of application ID.
 * @since_tizen 6.0
 */
#define AUL_K_APPID_LIST                "__AUL_K_APPID_LIST__"

/**
 * @brief Definition for AUL: The name of the component port.
 * @since_tizen 6.5
 */
#define AUL_K_COMPONENT_PORT            "__AUL_COMPONENT_PORT__"

/**
 * @brief Definition for AUL: The result.
 * @since_tizen 6.5
 */
#define AUL_K_RESULT                    "__AUL_RESULT__"

/**
 * @brief Definition for AUL: The flag if it's 'true', the application has focus.
 * @since_tizen 6.5
 */
#define AUL_K_HAS_FOCUS                 "__AUL_HAS_FOCUS__"

/**
 * @brief Definition for AUL: The state of the application lifecycle.
 * @since_tizen 6.5
 */
#define AUL_K_STATE                     "__AUL_STATE__"

/**
 * @brief Definition for AUL: The list of paths in a global resource packages.
 * @since_tizen 6.5
 */
#define AUL_K_MOUNT_GLOBAL_RES_DIR      "__AUL_MOUNT_GLOBAL_RES_DIR__"

/**
 * @brief Definition for AUL: The list of paths in a allowed resource packages.
 * @since_tizen 6.5
 */
#define AUL_K_MOUNT_ALLOWED_RES_DIR     "__AUL_MOUNT_ALLOWED_RES_DIR__"

/**
 * @brief Definition for AUL: The name of the process.
 * @since_tizen 6.5
 */
#define AUL_K_PROC_NAME                 "__AUL_PROC_NAME__"

/**
 * @brief Definition for AUL: The extra data of the process.
 * @since_tizen 6.5
 */
#define AUL_K_PROC_EXTRA                "__AUL_PROC_EXTRA__"

/**
 * @brief Definition for AUL: The flag of the auto restart.
 * @since_tizen 7.0
 */
#define AUL_K_AUTO_RESTART              "__AUL_AUTO_RESTART__"

/**
 * @brief Definition for AUL: The bundle data for auto restart.
 * @since_tizen 7.0
 */
#define AUL_K_RESTART_EXTRA             "__AUL_RESTART_EXTRA__"

/**
 * @brief Definition for AUL: The priority of boot sequence app.
 * @since_tizen 7.0
 */
#define AUL_K_BOOT_SEQUENCE_PRIORITY    "__AUL_BOOT_SEQUENCE_PRIORITY__"

/**
 * @brief Definition for AUL: The flag of boot sequence app's reqdy wait.
 * @since_tizen 7.0
 */
#define AUL_K_BOOT_SEQUENCE_READY_WAIT  "__AUL_BOOT_SEQUENCE_READY_WAIT__"

/**
 * @brief Definition for AUL: The sub status of app.
 * @since_tizen 7.0
 */
#define AUL_K_SUB_STATUS                "__AUL_SUB_STATUS__"

/**
 * @brief Definition for AUL: The appid list of boot sequence app's after dependency.
 * @since_tizen 7.0
 */
#define AUL_K_BOOT_SEQUENCE_AFTER       "__AUL_BOOT_SEQUENCE_AFTER__"

/**
 * @brief Definition for AUL: The appid list of boot sequence app's before dependency.
 * @since_tizen 7.0
 */
#define AUL_K_BOOT_SEQUENCE_BEFORE      "__AUL_BOOT_SEQUENCE_BEFORE__"

/**
 * @brief Definition for AUL: The appid list of boot sequence app's requires dependency.
 * @since_tizen 7.0
 */
#define AUL_K_BOOT_SEQUENCE_REQUIRES    "__AUL_BOOT_SEQUENCE_REQUIRES__"

/**
 * @brief Definition for AUL: The appid list of boot sequence app's conflicts dependency.
 * @since_tizen 7.0
 */
#define AUL_K_BOOT_SEQUENCE_CONFLICTS   "__AUL_BOOT_SEQUENCE_CONFLICTS__"

/**
 * @brief Definition for AUL: The args bundle data.
 * @since_tizen 7.0
 */
#define AUL_K_ARGS                      "__AUL_ARGS__"

/**
 * @brief Definition for AUL: The timeout.
 * @since_tizen 7.0
 */
#define AUL_K_TIMEOUT                   "__AUL_TIMEOUT__"

/**
 * @brief Definition for AUL: The begin timestamp of boot sequence app start.
 * @since_tizen 7.0
 */
#define AUL_K_BEGIN_TIMESTAMP           "__AUL_BEGIN_TIMESTAMP__"

/**
 * @brief Definition for AUL: The end timestamp of boot sequence app start.
 * @since_tizen 7.0
 */
#define AUL_K_END_TIMESTAMP             "__AUL_END_TIMESTAMP__"

/**
 * @brief Definition for AUL: The socket ready timestamp of boot sequence app.
 * @since_tizen 7.0
 */
#define AUL_K_SOCKET_READY_TIMESTAMP    "__AUL_SOCKET_READY_TIMESTAMP__"

/**
 * @brief Definition for AUL: The terminated timestamp of boot sequence app.
 * @since_tizen 7.0
 */
#define AUL_K_TERMINATED_TIMESTAMP      "__AUL_TERMINATED_TIMESTAMP__"

/**
 * @brief Definition for AUL: The status message of boot sequence app.
 * @since_tizen 7.0
 */
#define AUL_K_STATUS_MSG                "__AUL_STATUS_MSG__"

/**
 * @brief Definition for AUL: The vconf data of boot sequence app.
 * @since_tizen 7.0
 */
#define AUL_K_VCONF_DATA                "__AUL_VCONF_DATA__"

/**
 * @brief Definition for AUL: The value of indicating a enabled light user for app.
 * @since_tizen 7.0
 */
#define AUL_K_ENABLED_LIGHT_USER        "__AUL_ENABLED_LIGHT_USER__"

/**
 * @brief Definition for AUL: The complication mode.
 * @since_tizen 5.0
 */
#define AUL_K_COMPLICATION_MODE         "__AUL_COMPLICATION_MODE__"

/**
 * @brief Definition for AUL: The package event name.
 * @since_tizen 7.0
 */
#define AUL_K_PKG_EVENT_NAME            "__AUL_K_PKG_EVENT_NAME__"

/**
 * @brief Definition for AUL: The package event
 * @since_tizen 7.0
 */
#define AUL_K_PKG_EVENT_RESULT          "__AUL_K_PKG_EVENT_RESULT__"

/**
 * @brief Definition for AUL: The socket timeout milli seconds.
 * @since_tizen 8.0
 */
#define AUL_K_SOCKET_TIMEOUT            "__AUL_SOCKET_TIMEOUT__"

/**
 * @brief Definition for AUL: The list of mounted resource packages
 * @since_tizen 8.0
 */
#define AUL_K_MOUNT_RES_PKGIDS          "__AUL_MOUNT_RES_PKGIDS__"

/**
 * @brief Definition for AUL: X position of the window.
 * @since_tizen 8.0
 */
#define AUL_K_HINT_SCREEN_POS_X         "__K_HINT_SCREEN_POS_X"

/**
 * @brief Definition for AUL: Y position of the window.
 * @since_tizen 8.0
 */
#define AUL_K_HINT_SCREEN_POS_Y         "__K_HINT_SCREEN_POS_Y"

/**
 * @brief Definition for AUL: Width of the window.
 * @since_tizen 8.0
 */
#define AUL_K_HINT_SCREEN_WIDTH         "__K_HINT_SCREEN_WIDTH"

/**
 * @brief Definition for AUL: Height of the window.
 * @since_tizen 8.0
 */
#define AUL_K_HINT_SCREEN_HEIGHT        "__K_HINT_SCREEN_HEIGHT"

/**
 * @brief Definition for AUL: The application ID for multiple instances.
 * @since_tizen 8.0
 */
#define AUL_K_MULTIPLE_INSTANCE_APPID   "__AUL_MULTIPLE_INSTANCE_APPID__"

/**
 * @brief Definition for AUL: The request ID.
 * @since_tizen 8.0
 */
#define AUL_K_REQUEST_ID                "__AUL_REQUEST_ID__"

/**
 * @brief Definition for AUL: The list of paths in a library resource packages.
 * @since_tizen 9.0
 */
#define AUL_K_MOUNT_LIB_DIR     "__AUL_MOUNT_LIB_DIR__"

/**
 * @brief Definition for AUL: The list of mounted gadget package IDs.
 * @since_tizen 7.0
 */
#define AUL_K_MOUNT_GADGET_PKGIDS       "__AUL_MOUNT_GADGET_PKGIDS__"

/**
 * @brief Definition for AUL: The list of paths of gadget packages.
 * @since_tizen 7.0
 */
#define AUL_K_MOUNT_GADGET_PATHS        "__AUL_MOUNT_GADGET_PATHS__"

/**
 * @brief Definition for AUL: The list of mounted service packages IDs.
 * @since_tizen 10.0
 */
#define AUL_K_MOUNT_SERVICE_PKGIDS      "__AUL_MOUNT_SERVICE_PKGIDS__"

/**
 * @brief Definition for AUL: The list of paths of service packages.
 * @since_tizen 10.0
 */
#define AUL_K_MOUNT_SERVICE_PATHS       "__AUL_MOUNT_SERVICE_PATHS__"

/**
 * @brief Definition for AUL: The subsession ID.
 * @since_tizen 10.0
 */
#define AUL_K_SUBSESSION_ID             "__AUL_SUBSESSION_ID__"

/**
 * @brief Definition for AUL: The team leader ID of team application..
 * @since_tizen 10.0
 */
#define AUL_K_TEAM_LEADER_ID             "__AUL_TEAM_LEADER_ID__"

/**
 * @brief Definition for AUL: The list of team member id.
 * @since_tizen 10.0
 */
#define AUL_K_TEAM_MEMBER_IDS            "__AUL_TEAM_MEMBER_IDS__"

/**
 * @brief Definition for AUL: The list of team member type.
 * @since_tizen 10.0
 */
#define AUL_K_TEAM_MEMBER_TYPES          "__AUL_TEAM_MEMBER_TYPES__"

/**
 * @brief Definition for AUL: The list of team member package name.
 * @since_tizen 10.0
 */
#define AUL_K_TEAM_MEMBER_PKG_IDS "__AUL_TEAM_MEMBER_PACKAGE_IDS__"

/**
 * @brief Definition for AUL: Team member id.
 * @since_tizen 10.0
 */
#define AUL_K_MEMBER_ID                  "__AUL_MEMBER_ID__"