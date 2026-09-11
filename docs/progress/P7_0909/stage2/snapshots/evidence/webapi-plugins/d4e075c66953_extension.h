// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMMON_EXTENSION_H_
#define COMMON_EXTENSION_H_

// This is a C++ wrapper over Crosswalk Extension C API. It implements once the
// boilerplate for the common case of mapping XW_Extension and XW_Instance to
// objects of their own. The wrapper deals automatically with creating and
// destroying the objects.
//
// Extension object lives during the lifetime of the extension, and when the
// extension process is properly shutdown, it's destructor will be
// called. Instance objects (there can be many) live during the lifetime of a
// script context associated with a frame in the page. These objects serves as
// storage points for extension specific objects, use them for that.

#include <sys/types.h>

#include <exception>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

#include "common/XW_Extension.h"
#include "common/XW_Extension_EntryPoints.h"
#include "common/XW_Extension_Permissions.h"
#include "common/XW_Extension_Runtime.h"
#include "common/XW_Extension_SyncMessage.h"
#include "common/platform_exception.h"
#include "common/platform_result.h"

namespace common {

class Instance;
class Extension;

}  // namespace common

// This function should be implemented by each extension and should return
// an appropriate Extension subclass.
common::Extension* CreateExtension();

namespace common {

// implemented in XW_Extension.cc
// can be called only after the extension is fully created
//    (CreateExtension() has been called)
Extension* GetCurrentExtension();

class Extension {
 public:
  Extension();
  virtual ~Extension();

  // These should be called in the subclass constructor.
  void SetExtensionName(const char* name);
  void SetJavaScriptAPI(const char* api);
  void SetExtraJSEntryPoints(const char** entry_points);
  bool RegisterPermissions(const char* perm_table);

  // This API should be called in the message handler of extension
  bool CheckAPIAccessControl(const char* api_name);

  virtual Instance* CreateInstance();

  std::string GetRuntimeVariable(const char* var_name, unsigned len = 512);
  static std::string GetRuntimeVariableString(const char* var_name);

 private:
  friend int32_t(::XW_Initialize)(XW_Extension extension, XW_GetInterface get_interface);

  static int32_t XW_Initialize(XW_Extension extension, XW_GetInterface get_interface,
                               XW_Initialize_Func initialize,
                               XW_CreatedInstanceCallback created_instance,
                               XW_ShutdownCallback shutdown);

  // XW_Extension callbacks.
  static void OnInstanceCreated(XW_Instance xw_instance, Instance* instance);  // modified
  static void OnInstanceDestroyed(XW_Instance xw_instance);
  static void HandleMessage(XW_Instance xw_instance, const char* msg);

  static void HandleSyncMessage(XW_Instance xw_instance, const char* msg);
  static void HandleBinaryMessage(XW_Instance instance, const char* message, const size_t size);
  XW_Extension xw_extension_;

  class Detail;
};

class Instance {
 public:
  Instance();
  virtual ~Instance();

  static void DoAndPostMessage(Instance* that, const std::function<void()>& work,
                               const picojson::value& json);
  static void PostMessage(Instance* that, const char* msg);
  static void PostMessage(Instance* that, const char* msg, size_t size);
  static void PostMessage(Instance* that, const picojson::value& json);
  void PostMessage(const char* msg);
  void PostMessage(const char* msg, size_t size);
  void SendSyncReply(const char* reply);
  void SendSyncBinaryReply(const std::vector<uint8_t>& reply);

  virtual void Initialize() {
  }
  virtual void HandleMessage(const char* msg) = 0;
  virtual void HandleSyncMessage(const char* msg) {
  }

  virtual void HandleBinaryMessage(const char* msg, size_t size) = 0;
  virtual void HandleSyncBinaryMessage(const char* msg, size_t size) = 0;
  virtual void SetWebWorker(bool worker) {
    is_web_worker_ = worker;
  }

  XW_Instance xw_instance() const {
    return xw_instance_;
  }

 protected:
  bool is_web_worker_ = false;

 private:
  friend class Extension;

  static std::mutex instances_mutex_;
  static std::unordered_set<Instance*> all_instances_;

  XW_Instance xw_instance_;
};

typedef std::function<void(const picojson::value&, picojson::object&)> NativeHandler;
typedef std::function<void(const picojson::value&, std::vector<uint8_t>*)>
    NativeHandlerWithBinaryAnswer;
typedef std::function<void(const char*, size_t size, picojson::object&)> BinaryNativeHandler;

class ParsedInstance : public Instance {
 public:
  ParsedInstance();
  virtual ~ParsedInstance();

 protected:
  void RegisterHandler(const std::string& name, const NativeHandler& func);
  void RegisterSyncHandler(const std::string& name, const NativeHandler& func);
  void RegisterSyncHandlerWithBinaryAnswer(const std::string& name,
                                           const NativeHandlerWithBinaryAnswer& func);

  void RegisterBinaryHandler(const BinaryNativeHandler& func);
  void RegisterBinarySyncHandler(const BinaryNativeHandler& func);

  static void ReportSuccess(picojson::object& out);
  static void ReportSuccess(const picojson::value& result, picojson::object& out);
  static void ReportError(picojson::object& out);
  static void ReportError(const PlatformException& ex, picojson::object& out);
  static void ReportError(const PlatformResult& error, picojson::object* out);

 private:
  void HandleMessage(const char* msg);
  void HandleSyncMessage(const char* msg);

  void HandleBinaryMessage(const char* msg, size_t size);
  void HandleSyncBinaryMessage(const char* msg, size_t size);

  void HandleMessage(const char* msg, bool is_sync);
  void HandleBinaryMessage(const char* msg, size_t size, bool is_sync);
  void HandleException(const PlatformException& ex);
  void HandleException(const std::exception& e);
  void HandleError(const PlatformResult& error);

  std::map<std::string, NativeHandler> handler_map_;
  std::map<std::string, NativeHandlerWithBinaryAnswer> handler_with_binary_anser_map_;
  std::vector<BinaryNativeHandler> binary_handler_vec_;
};

}  // namespace common

#endif  // COMMON_EXTENSION_H_
