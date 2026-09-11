/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_APP_EVENT_INTERNAL_H__
#define __TIZEN_APPFW_APP_EVENT_INTERNAL_H__

#include <app_common.h>

#include <string>

#include <app_core_base.hh>

struct app_event_info {
  app_event_type_e type;
  void* value;
};

class AppEvent : public tizen_cpp::AppCoreBase::EventBase {
 public:
  AppEvent(Type type, app_event_cb callback, void* data)
      : EventBase(type), callback_(callback), data_(data) {
    handle_.type = ConvertAppEvent(type);
  }

  ~AppEvent() = default;

  void OnEvent(const std::string& val) override {
    handle_.value = reinterpret_cast<void*>(const_cast<char*>(val.c_str()));
    callback_(reinterpret_cast<app_event_info_h>(&handle_), data_);
  }

  void OnEvent(int val) override {
    handle_.value = reinterpret_cast<void*>(&val);
    callback_(reinterpret_cast<app_event_info_h>(&handle_), data_);
  }

  app_event_info_h GetHandle() {
    return reinterpret_cast<app_event_info_h>(&handle_);
  }

  static constexpr Type InvertAppEvent(app_event_type_e type) {
    if (type == APP_EVENT_LOW_MEMORY)
      return Type::LOW_MEMORY;
    if (type == APP_EVENT_LOW_BATTERY)
      return Type::LOW_BATTERY;
    if (type == APP_EVENT_LANGUAGE_CHANGED)
      return Type::LANG_CHANGE;
    if (type == APP_EVENT_DEVICE_ORIENTATION_CHANGED)
      return Type::DEVICE_ORIENTATION_CHANGED;
    if (type == APP_EVENT_REGION_FORMAT_CHANGED)
      return Type::REGION_CHANGE;
    if (type == APP_EVENT_SUSPENDED_STATE_CHANGED)
      return Type::SUSPENDED_STATE_CHANGE;
    if (type == APP_EVENT_UPDATE_REQUESTED)
      return Type::UPDATE_REQUESTED;
    if (type == APP_EVENT_TIME_ZONE_CHANGED)
      return Type::TIME_ZONE_CHANGED;

    return Type::SUSPENDED_STATE_CHANGE;
  }

 private:
  constexpr app_event_type_e ConvertAppEvent(Type type) {
    if (type == Type::LOW_MEMORY)
      return APP_EVENT_LOW_MEMORY;
    if (type == Type::LOW_BATTERY)
      return APP_EVENT_LOW_BATTERY;
    if (type == Type::LANG_CHANGE)
      return APP_EVENT_LANGUAGE_CHANGED;
    if (type == Type::DEVICE_ORIENTATION_CHANGED)
      return APP_EVENT_DEVICE_ORIENTATION_CHANGED;
    if (type == Type::REGION_CHANGE)
      return APP_EVENT_REGION_FORMAT_CHANGED;
    if (type == Type::SUSPENDED_STATE_CHANGE)
      return APP_EVENT_SUSPENDED_STATE_CHANGED;
    if (type == Type::UPDATE_REQUESTED)
      return APP_EVENT_UPDATE_REQUESTED;
    if (type == Type::TIME_ZONE_CHANGED)
      return APP_EVENT_TIME_ZONE_CHANGED;

    return APP_EVENT_SUSPENDED_STATE_CHANGED;
  }

  app_event_cb callback_;
  void* data_;
  app_event_info handle_{};
};

#endif  // __TIZEN_APPFW_APP_EVENT_INTERNAL_H__
