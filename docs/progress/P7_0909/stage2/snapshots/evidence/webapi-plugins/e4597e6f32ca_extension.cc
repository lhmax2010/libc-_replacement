// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "common/extension.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "common/assert.h"
#include "common/logger.h"
#include "common/tools.h"

// This function is hidden, because each plugin needs to have own implementation.
__attribute__((visibility("hidden"))) common::Extension* CreateExtension() {
  common::Extension* e = new common::Extension();
  e->SetExtensionName("common");
  return e;
}

namespace {

// this variable is valid only during Extension::XW_Initialize() call
// do not use !!!
// it's here, so we don't have to modify the interface of CreateExtension(), Extension(), etc.
XW_Extension g_xw_extension_ = 0;

const XW_CoreInterface* g_core = NULL;
const XW_MessagingInterface* g_messaging = NULL;
const XW_Internal_SyncMessagingInterface* g_sync_messaging = NULL;
const XW_Internal_EntryPointsInterface* g_entry_points = NULL;
const XW_Internal_RuntimeInterface* g_runtime = NULL;
const XW_Internal_PermissionsInterface* g_permission = NULL;

bool InitializeInterfaces(XW_GetInterface get_interface) {
  ScopeLogger();
  static bool initialized = false;

  if (!initialized) {
    g_core = reinterpret_cast<const XW_CoreInterface*>(get_interface(XW_CORE_INTERFACE));
    if (!g_core) {
      LoggerE("Can't initialize extension: error getting Core interface.");
      return false;
    }

    g_messaging =
        reinterpret_cast<const XW_MessagingInterface*>(get_interface(XW_MESSAGING_INTERFACE));
    if (!g_messaging) {
      LoggerE("Can't initialize extension: error getting Messaging interface.");
      return false;
    }

    g_sync_messaging = reinterpret_cast<const XW_Internal_SyncMessagingInterface*>(
        get_interface(XW_INTERNAL_SYNC_MESSAGING_INTERFACE));
    if (!g_sync_messaging) {
      LoggerE("Can't initialize extension: error getting SyncMessaging interface.");
      return false;
    }

    g_entry_points = reinterpret_cast<const XW_Internal_EntryPointsInterface*>(
        get_interface(XW_INTERNAL_ENTRY_POINTS_INTERFACE));
    if (!g_entry_points) {
      LoggerE(
          "NOTE: Entry points interface not available in this version "
          "of runtime, ignoring entry point data for extensions.");
    }

    g_runtime = reinterpret_cast<const XW_Internal_RuntimeInterface*>(
        get_interface(XW_INTERNAL_RUNTIME_INTERFACE));
    if (!g_runtime) {
      LoggerE(
          "NOTE: runtime interface not available in this version "
          "of runtime, ignoring runtime variables for extensions.");
    }

    g_permission = reinterpret_cast<const XW_Internal_PermissionsInterface*>(
        get_interface(XW_INTERNAL_PERMISSIONS_INTERFACE));
    if (!g_permission) {
      LoggerE(
          "NOTE: permission interface not available in this version "
          "of runtime, ignoring permission for extensions.");
    }

    initialized = true;
  }

  return true;
}

}  // namespace

namespace common {

Extension::Extension() : xw_extension_(g_xw_extension_) {
  ScopeLogger();
}

Extension::~Extension() {
  ScopeLogger();
}

void Extension::SetExtensionName(const char* name) {
  ScopeLogger();
  g_core->SetExtensionName(xw_extension_, name);
}

void Extension::SetJavaScriptAPI(const char* api) {
  ScopeLogger();
  g_core->SetJavaScriptAPI(xw_extension_, api);
}

void Extension::SetExtraJSEntryPoints(const char** entry_points) {
  ScopeLogger();
  if (g_entry_points) g_entry_points->SetExtraJSEntryPoints(xw_extension_, entry_points);
}

bool Extension::RegisterPermissions(const char* perm_table) {
  ScopeLogger();
  if (g_permission) return g_permission->RegisterPermissions(xw_extension_, perm_table);
  return false;
}

bool Extension::CheckAPIAccessControl(const char* api_name) {
  ScopeLogger();
  if (g_permission) return g_permission->CheckAPIAccessControl(xw_extension_, api_name);
  return false;
}

Instance* Extension::CreateInstance() {
  ScopeLogger();
  return NULL;
}

// static
std::string Extension::GetRuntimeVariableString(const char* var_name) {
  ScopeLogger();
  if (!g_runtime) return "";

  const int result_len = 1024;
  std::vector<char> res(result_len + 1, 0);
  g_runtime->GetRuntimeVariableString(0, var_name, &res[0], result_len);
  LoggerI("GetRuntimeVariable: result %s: \"%s\"", var_name, &res[0]);
  return std::string(res.data());
}

std::string Extension::GetRuntimeVariable(const char* var_name, unsigned len) {
  ScopeLogger();
  if (!g_runtime) return "";

  std::vector<char> res(len + 1, 0);
  g_runtime->GetRuntimeVariableString(xw_extension_, var_name, &res[0], len);
  // crosswalk has used the double quote for the app_id from the first.
  // the n-wrt (new wrt) is using the double quote also.
  // but that's wrt and wrt-service's bug.
  // To keep compatibilities, two case of formats should be considered in webapi-plugins.
  // removing double quote to keep compatibilities with new and old wrt
  std::string value = std::string(res.data());
  if (0 == strncmp(var_name, "app_id", 6) && value.find('"', 0) != std::string::npos &&
      value.find('"', value.size() - 1) != std::string::npos) {
    value = value.erase(0, 1);
    value = value.erase(value.size() - 1, 1);
  }

  return value;
}

// static
void Extension::OnInstanceCreated(XW_Instance xw_instance, Instance* instance) {
  ScopeLogger();
  Assert(!g_core->GetInstanceData(xw_instance));
  if (!instance) return;
  instance->xw_instance_ = xw_instance;
  g_core->SetInstanceData(xw_instance, instance);
  instance->Initialize();
}

// static
void Extension::OnInstanceDestroyed(XW_Instance xw_instance) {
  ScopeLogger();
  Instance* instance = reinterpret_cast<Instance*>(g_core->GetInstanceData(xw_instance));
  if (!instance) return;
  instance->xw_instance_ = 0;
  delete instance;
}

// static
void Extension::HandleMessage(XW_Instance xw_instance, const char* msg) {
  ScopeLogger();
  Instance* instance = reinterpret_cast<Instance*>(g_core->GetInstanceData(xw_instance));
  if (!instance) return;
  instance->HandleMessage(msg);
}

void Extension::HandleBinaryMessage(XW_Instance xw_instance, const char* msg, size_t size) {
  ScopeLogger();
  Instance* instance = reinterpret_cast<Instance*>(g_core->GetInstanceData(xw_instance));
  if (!instance) {
    return;
  }
  // TODO maybe async version will be needed in future
  instance->HandleSyncBinaryMessage(msg, size);
}

// static
void Extension::HandleSyncMessage(XW_Instance xw_instance, const char* msg) {
  ScopeLogger();
  Instance* instance = reinterpret_cast<Instance*>(g_core->GetInstanceData(xw_instance));
  if (!instance) return;
  instance->HandleSyncMessage(msg);
}

// static
int32_t Extension::XW_Initialize(XW_Extension extension, XW_GetInterface get_interface,
                                 XW_Initialize_Func initialize,
                                 XW_CreatedInstanceCallback created_instance,
                                 XW_ShutdownCallback shutdown) {
  ScopeLogger();
  Assert(extension);

  if (!InitializeInterfaces(get_interface)) {
    return XW_ERROR;
  }

  g_xw_extension_ = extension;

  if (XW_ERROR == initialize(extension, get_interface)) {
    return XW_ERROR;
  }

  g_xw_extension_ = 0;

  using common::Extension;
  g_core->RegisterShutdownCallback(extension, shutdown);
  g_core->RegisterInstanceCallbacks(extension, created_instance, Extension::OnInstanceDestroyed);
  g_messaging->Register(extension, Extension::HandleMessage);
  g_sync_messaging->Register(extension, Extension::HandleSyncMessage);

  g_messaging->RegisterBinaryMesssageCallback(extension, Extension::HandleBinaryMessage);
  return XW_OK;
}

std::mutex Instance::instances_mutex_;
std::unordered_set<Instance*> Instance::all_instances_;

Instance::Instance() : xw_instance_(0) {
  ScopeLogger();
  {
    std::lock_guard<std::mutex> lock{instances_mutex_};
    all_instances_.insert(this);
  }
}

Instance::~Instance() {
  ScopeLogger();
  {
    std::lock_guard<std::mutex> lock{instances_mutex_};
    all_instances_.erase(this);
  }
  Assert(xw_instance_ == 0);
}

void Instance::DoAndPostMessage(Instance* that, const std::function<void()>& work,
                                const picojson::value& json) {
  ScopeLogger();
  if (nullptr != that) {
    std::lock_guard<std::mutex> lock{instances_mutex_};
    if (all_instances_.end() != all_instances_.find(that)) {
      work();
      that->PostMessage(json.serialize().c_str());
      return;
    }
  }
  LoggerE("Trying to post message to non-existing instance: [%p], ignoring", that);
}

void Instance::PostMessage(Instance* that, const char* msg) {
  ScopeLogger();
  if (nullptr != that) {
    std::lock_guard<std::mutex> lock{instances_mutex_};
    if (all_instances_.end() != all_instances_.find(that)) {
      that->PostMessage(msg);
      return;
    }
  }
  LoggerE("Trying to post message to non-existing instance: [%p], ignoring", that);
}

void Instance::PostMessage(Instance* that, const char* msg, size_t size) {
  ScopeLogger();
  if (nullptr != that) {
    std::lock_guard<std::mutex> lock{instances_mutex_};
    if (all_instances_.end() != all_instances_.find(that)) {
      that->PostMessage(msg, size);
      return;
    }
  }
  LoggerE("Trying to post message to non-existing instance: [%p], ignoring", that);
}

void Instance::PostMessage(Instance* that, const picojson::value& json) {
  Instance::PostMessage(that, json.serialize().c_str());
}

void Instance::PostMessage(const char* msg) {
  ScopeLogger();
  if (!xw_instance_) {
    LoggerE(
        "Ignoring PostMessage() in the constructor or after the "
        "instance was destroyed.");
    return;
  }
  g_messaging->PostMessage(xw_instance_, msg);
}

void Instance::PostMessage(const char* msg, size_t size) {
  ScopeLogger();
  if (!xw_instance_) {
    LoggerE(
        "Ignoring PostMessage() in the constructor or after the "
        "instance was destroyed.");
    return;
  }
  g_messaging->PostBinaryMessage(xw_instance_, msg, size);
}

void Instance::SendSyncReply(const char* reply) {
  ScopeLogger();
  if (!xw_instance_) {
    LoggerE(
        "Ignoring SendSyncReply() in the constructor or after the "
        "instance was destroyed.");
    return;
  }
  g_sync_messaging->SetSyncReply(xw_instance_, reply);
}

void Instance::SendSyncBinaryReply(const std::vector<uint8_t>& reply) {
  ScopeLogger();
  if (!xw_instance_) {
    LoggerE(
        "Ignoring SendSyncBinaryReply() in the constructor or after the "
        "instance was destroyed.");
    return;
  }
  g_sync_messaging->SetSyncBinaryReply(xw_instance_, reply);
}

ParsedInstance::ParsedInstance() {
  ScopeLogger();
}

ParsedInstance::~ParsedInstance() {
  ScopeLogger();
}

void ParsedInstance::RegisterHandler(const std::string& name, const NativeHandler& func) {
  ScopeLogger();
  handler_map_.insert(std::make_pair(name, func));
}

void ParsedInstance::RegisterSyncHandler(const std::string& name, const NativeHandler& func) {
  ScopeLogger();
  handler_map_.insert(std::make_pair("#SYNC#" + name, func));
}

void ParsedInstance::RegisterSyncHandlerWithBinaryAnswer(
    const std::string& name, const NativeHandlerWithBinaryAnswer& func) {
  ScopeLogger();
  handler_with_binary_anser_map_.insert(std::make_pair("#SYNC#" + name, func));
}

void ParsedInstance::RegisterBinaryHandler(const BinaryNativeHandler& func) {
  ScopeLogger();
  binary_handler_vec_.push_back(func);
}

void ParsedInstance::RegisterBinarySyncHandler(const BinaryNativeHandler& func) {
  ScopeLogger();
  // TODO - maybe async version will be needed in future
  binary_handler_vec_.push_back(func);
}

void ParsedInstance::ReportSuccess(picojson::object& out) {
  ScopeLogger();
  tools::ReportSuccess(out);
}

void ParsedInstance::ReportSuccess(const picojson::value& result, picojson::object& out) {
  ScopeLogger();
  tools::ReportSuccess(result, out);
}

void ParsedInstance::ReportError(picojson::object& out) {
  ScopeLogger();
  tools::ReportError(out);
}

void ParsedInstance::ReportError(const PlatformException& ex, picojson::object& out) {
  ScopeLogger();
  tools::ReportError(ex, out);
}

void ParsedInstance::ReportError(const PlatformResult& error, picojson::object* out) {
  ScopeLogger();
  tools::ReportError(error, out);
}

void ParsedInstance::HandleMessage(const char* msg) {
  ScopeLogger();
  HandleMessage(msg, false);
}

void ParsedInstance::HandleBinaryMessage(const char* msg, size_t size) {
  ScopeLogger();
  HandleBinaryMessage(msg, size, false);
}

void ParsedInstance::HandleSyncMessage(const char* msg) {
  ScopeLogger();
  HandleMessage(msg, true);
}

void ParsedInstance::HandleSyncBinaryMessage(const char* msg, size_t size) {
  ScopeLogger();
  HandleBinaryMessage(msg, size, true);
}

void ParsedInstance::HandleMessage(const char* msg, bool is_sync) {
  ScopeLogger();
  try {
    picojson::value value;
    std::string err;
    picojson::parse(value, msg, msg + strlen(msg), &err);
    if (!err.empty()) {
      LoggerE("Ignoring message:\"%s\", error: %s", msg, err.c_str());
      return;
    }

    if (!value.is<picojson::object>()) {
      LoggerE("Ignoring message. It is not an object.");
      return;
    }

    std::string cmd = (is_sync ? "#SYNC#" : "") + value.get("cmd").to_str();
    const bool is_binary_answer = value.contains("__binaryAnswer") &&
                                  value.get("__binaryAnswer").is<bool>() &&
                                  value.get("__binaryAnswer").get<bool>();

    // check for args in JSON message
    const picojson::value& args = value.get("args");
    if (!args.is<picojson::object>()) {
      throw InvalidValuesException("No \"args\" field in message");
    }

    if (is_binary_answer) {
      auto it = handler_with_binary_anser_map_.find(cmd);
      if (handler_with_binary_anser_map_.end() == it) {
        LoggerE("Unknown binary answer command: %s", cmd.c_str());
        throw UnknownException("Unknown command.");
      }

      NativeHandlerWithBinaryAnswer func = it->second;
      std::vector<uint8_t> vec;
      func(args, &vec);
      if (is_sync) SendSyncBinaryReply(vec);
    } else {
      auto it = handler_map_.find(cmd);
      if (handler_map_.end() == it) {
        LoggerE("Unknown json answer command: %s", cmd.c_str());
        throw UnknownException("Unknown command.");
      }

      NativeHandler func = it->second;
      picojson::value result = picojson::value(picojson::object());
      func(args, result.get<picojson::object>());

      if (is_sync) SendSyncReply(result.serialize().c_str());
    }
  } catch (const PlatformException& e) {
    return HandleException(e);
  } catch (const PlatformException* e) {
    return HandleException(*e);
  } catch (const std::exception& e) {
    return HandleException(e);
  } catch (...) {
    return HandleException(UnknownException("Unknown exception"));
  }
}

void ParsedInstance::HandleBinaryMessage(const char* msg, size_t size, bool is_sync) {
  ScopeLogger();
  try {
    // first byte is a method ID
    auto func = binary_handler_vec_.at(msg[0]);

    picojson::value result = picojson::value(picojson::object());
    func(msg + 1, size - 1, result.get<picojson::object>());

    if (is_sync) SendSyncReply(result.serialize().c_str());
  } catch (const std::out_of_range& e) {
    return HandleException(UnknownException("Unknown command"));
  } catch (const PlatformException& e) {
    return HandleException(e);
  } catch (const PlatformException* e) {
    return HandleException(*e);
  } catch (const std::exception& e) {
    return HandleException(e);
  } catch (...) {
    return HandleException(UnknownException("Unknown exception"));
  }
}

void ParsedInstance::HandleException(const PlatformException& ex) {
  ScopeLogger();
  LoggerE("Exception: %s", ex.message().c_str());
  picojson::value result = picojson::value(picojson::object());
  ReportError(ex, result.get<picojson::object>());
  SendSyncReply(result.serialize().c_str());
}

void ParsedInstance::HandleException(const std::exception& e) {
  ScopeLogger();
  LoggerE("std::exception: %s", e.what());
  picojson::value result = picojson::value(picojson::object());
  ReportError(common::PlatformResult{common::ErrorCode::ABORT_ERR, "An unknown internal error"},
              &result.get<picojson::object>());
  SendSyncReply(result.serialize().c_str());
}

void ParsedInstance::HandleError(const PlatformResult& e) {
  LoggerE("Error: %d", static_cast<int>(e.error_code()));
  picojson::value result = picojson::value(picojson::object());
  ReportError(e, &result.get<picojson::object>());
  SendSyncReply(result.serialize().c_str());
}

}  // namespace common
