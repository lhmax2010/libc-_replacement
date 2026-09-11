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

#ifndef APP_CONTROL_HPP_
#define APP_CONTROL_HPP_

#include <app_control.h>

#include <cstdio>
#include <string>
#include <memory>
#include <vector>

namespace tizen_appfw {

class AppControl {
 public:
  enum class Result {
    APP_STARTED = 1,
    SUCCEEDED = 0,
    FAILED = -1,
    CANCELED = -2,
  };

  class IEventHandler {
    public:
      virtual ~IEventHandler() = default;
      virtual void OnReceive(AppControl request, AppControl reply,
          Result result) = 0;
  };

  enum class LaunchMode {
    SINGLE = 0,
    GROUP
  };

  AppControl(bool app_started_result_event = false) {
    app_control_create(&handle_);
    if (app_started_result_event)
      app_control_enable_app_started_result_event(handle_);
  }

  AppControl(app_control_h h, bool own = false) {
    own_ = own;
    handle_ = h;
  }

  virtual ~AppControl() {
    if (handle_ && own_)
      app_control_destroy(handle_);
  }

  AppControl(const AppControl& c) {
    app_control_clone(&handle_, c.handle_);
    own_ = true;
  }

  AppControl& operator = (const AppControl& c) {
    if (this != &c) {
      if (handle_ && own_)
        app_control_destroy(handle_);
      app_control_clone(&handle_, c.handle_);
      own_ = true;
    }
    return *this;
  }

  AppControl(AppControl&& c) noexcept {
    handle_ = c.handle_;
    c.handle_ = nullptr;
    own_ = c.own_;
  }

  AppControl& operator = (AppControl&& c) noexcept {
    if (this != &c) {
      if (handle_ && own_)
        app_control_destroy(handle_);
      handle_ = c.handle_;
      c.handle_ = nullptr;
      own_ = c.own_;
    }
    return *this;
  }

  app_control_h GetHandle() const {
    return handle_;
  }

  app_control_h Detach() {
    auto h = handle_;
    handle_ = nullptr;
    return h;
  }

  int SetOperation(const std::string& op) {
    return app_control_set_operation(handle_, op.c_str());
  }

  std::string GetOperation() const {
    char* op = nullptr;
    int ret = app_control_get_operation(handle_, &op);
    if (ret != 0)
      return "";
    auto p = std::unique_ptr<char, decltype(std::free)*>(op, std::free);

    return p.get();
  }

  int SetUri(const std::string& uri) {
    return app_control_set_uri(handle_, uri.c_str());
  }

  std::string GetUri() const {
    char* uri = nullptr;
    int ret = app_control_get_uri(handle_, &uri);
    if (ret != 0)
      return "";
    auto p = std::unique_ptr<char, decltype(std::free)*>(uri, std::free);

    return p.get();
  }

  int SetMime(const std::string& mime) {
    return app_control_set_mime(handle_, mime.c_str());
  }

  std::string GetMime() const {
    char* mime = nullptr;
    int ret = app_control_get_mime(handle_, &mime);
    if (ret != 0)
      return "";
    auto p = std::unique_ptr<char, decltype(std::free)*>(mime, std::free);

    return p.get();
  }

  int SetAppId(const std::string& appid) {
    return app_control_set_app_id(handle_, appid.c_str());
  }

  std::string GetAppId() const {
    char* appid = nullptr;
    int ret = app_control_get_app_id(handle_, &appid);
    if (ret != 0)
      return "";
    auto p = std::unique_ptr<char, decltype(std::free)*>(appid, std::free);

    return p.get();
  }

  int AddExtraData(const std::string& key, const std::string& val) {
    return app_control_add_extra_data(handle_, key.c_str(), val.c_str());
  }

  int AddExtraData(const std::string& key, std::vector<std::string> vals) {
    std::vector<const char*> v;
    for (auto& i : vals) {
      v.push_back(i.c_str());
    }

    return app_control_add_extra_data_array(handle_, key.c_str(),
        v.data(), v.size());
  }

  int RemoveExtraData(const std::string& key) {
    return app_control_remove_extra_data(handle_, key.c_str());
  }

  std::string GetExtraData(const std::string& key) const {
    char* val = nullptr;
    int ret = app_control_get_extra_data(handle_, key.c_str(), &val);
    if (ret != 0)
      return "";
    auto p = std::unique_ptr<char, decltype(std::free)*>(val, std::free);

    return p.get();
  }

  std::vector<std::string> GetExtraDataArray(const std::string& key) const {
    char** val = nullptr;
    int len;
    int ret = app_control_get_extra_data_array(handle_, key.c_str(), &val, &len);
    if (ret != 0)
      return {};

    std::vector<std::string> v;
    for (int i = 0; i < len; i++) {
      v.push_back(val[i]);
      std::free(val[i]);
    }
    std::free(val);

    return v;
  }

  bool IsExtraDataArray(const std::string& key) const {
    bool array = false;
    app_control_is_extra_data_array(handle_, key.c_str(), &array);

    return array;
  }

  int SendLaunchRequest(IEventHandler* ev = nullptr) {
    if (!ev)
      return app_control_send_launch_request(handle_, nullptr, nullptr);

    return app_control_send_launch_request(handle_, [](app_control_h request,
        app_control_h reply, app_control_result_e result, void* user_data) {
          IEventHandler* ev = static_cast<IEventHandler*>(user_data);
          ev->OnReceive(AppControl(request), AppControl(reply),
              static_cast<Result>(result));
        }, ev);
  }

  int SetLaunchMode(LaunchMode m) {
    return app_control_set_launch_mode(handle_,
        static_cast<app_control_launch_mode_e>(m));
  }

  LaunchMode GetLaunchMode() const {
    app_control_launch_mode_e m;
    int r = app_control_get_launch_mode(handle_, &m);
    if (r != 0)
      return LaunchMode::SINGLE;

    return static_cast<LaunchMode>(m);
  }

  int SendTerminateRequest() {
    return app_control_send_terminate_request(handle_);
  }

  std::string GetCaller() const {
    char* id = nullptr;

    app_control_get_caller(handle_, &id);
    if (!id)
      return "";
    std::string ret_id = id;
    std::free(id);
    return ret_id;
  }

  std::vector<std::string> GetExtraDataKeys() const {
    std::vector<std::string> v;
    app_control_foreach_extra_data(handle_, [](app_control_h app_control,
        const char* key, void* user_data) -> bool {
          std::vector<std::string>* v =
              static_cast<std::vector<std::string>*>(user_data);
          v->push_back(key);
          return true;
        }, &v);

    return v;
  }

 private:
  app_control_h handle_;
  bool own_ = true;
};

class ReceivedAppControl : public AppControl {
 public:
  ReceivedAppControl(app_control_h h, bool own = false)
      : AppControl(h, own) {}
  ReceivedAppControl(bool app_started_result_event = false)
      : AppControl(app_started_result_event) {}

  int ReplyToLaunchRequest(const AppControl& reply, Result result) {
    return app_control_reply_to_launch_request(reply.GetHandle(),
        GetHandle(), static_cast<app_control_result_e>(result));
  }

  bool IsReplyRequested() const {
    bool req = false;
    app_control_is_reply_requested(GetHandle(), &req);
    return req;
  }
};

}  // namespace tizen_appfw

#endif  // APP_CONTROL_HPP_