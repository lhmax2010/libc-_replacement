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

#ifndef APP_COMMON_HPP_
#define APP_COMMON_HPP_

#include <app_common.h>

#include "app_event_internal.hh"

namespace tizen_appfw {
namespace app_common {

class EventInfo {
 public:
  enum Event {
    LOW_MEMORY,
    LOW_BATTERY,
    LANGUAGE_CHANGED,
    DEVICE_ORIENTATION_CHANGED,
    REGION_FORMAT_CHANGED,
    SUSPENDED_STATE_CHANGED,
    UPDATE_REQUESTED,
    TIME_ZONE_CHANGED,
  };

  EventInfo() {
    handle_ = nullptr;
  }

  explicit EventInfo(app_event_info_h h) {
    handle_ = h;
  }

  app_event_info_h GetHandle() const  {
    return handle_;
  }

  virtual Event GetEvent() const = 0;

 private:
  app_event_info_h handle_;
};

class LowMemory : public EventInfo {
  public:
  enum class Status {
    NORMAL = 0x01,
    SOFT_WARNING = 0x02,
    HARD_WARNING = 0x04,
  };

  LowMemory() {}
  explicit LowMemory(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return LOW_MEMORY;
  }

  Status GetStatus() const {
    app_event_low_memory_status_e status;
    int r = app_event_get_low_memory_status(GetHandle(), &status);
    if (r != 0)
      return Status::NORMAL;

    return static_cast<Status>(status);
  }
};

class LowBattery : public EventInfo {
  public:
  enum class Status {
    POWER_OFF = 1,
    CRITICAL_LOW,
  };

  LowBattery() {}
  LowBattery(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return LOW_BATTERY;
  }

  Status GetStatus() const {
    app_event_low_battery_status_e status;
    int r = app_event_get_low_battery_status(GetHandle(), &status);
    if (r != 0)
      return Status::POWER_OFF;

    return static_cast<Status>(status);
  }
};

class LanguageChanged : public EventInfo {
  public:
  LanguageChanged() {}
  LanguageChanged(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return LANGUAGE_CHANGED;
  }

  std::string GetLanguage() const {
    char* lang = nullptr;
    int r = app_event_get_language(GetHandle(), &lang);

    if (r != 0)
      return "";

    auto p = std::unique_ptr<char, decltype(std::free)*>(lang, std::free);

    return std::string(lang);
  }
};

class DeviceOrientationChanged : public EventInfo {
  public:
  enum Orientation {
    ORIENTATION_0 = 0,
    ORIENTATION_90 = 90,
    ORIENTATION_180 = 180,
    ORIENTATION_270 = 270,
  };

  DeviceOrientationChanged() {}
  DeviceOrientationChanged(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return DEVICE_ORIENTATION_CHANGED;
  }

  Orientation GetDeviceOrientation() const {
    app_device_orientation_e ori;
    int r = app_event_get_device_orientation(GetHandle(), &ori);

    if (r != 0)
      return ORIENTATION_0;

    return static_cast<Orientation>(ori);
  }
};

class RegionFormatChanged : public EventInfo {
  public:
  RegionFormatChanged() {}
  RegionFormatChanged(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return REGION_FORMAT_CHANGED;
  }

  std::string GetRegionFormat() const {
    char* reg = nullptr;
    int r = app_event_get_region_format(GetHandle(), &reg);

    if (r != 0)
      return "";

    auto p = std::unique_ptr<char, decltype(std::free)*>(reg, std::free);

    return reg;
  }
};

class SuspendedStateChanged : public EventInfo {
  public:
  enum class State {
    WILL_ENTER = 0,
    DID_EXIT,
  };

  SuspendedStateChanged() {}
  SuspendedStateChanged(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return SUSPENDED_STATE_CHANGED;
  }

  State GetState() const {
    app_suspended_state_e state;
    int r = app_event_get_suspended_state(GetHandle(), &state);

    if (r != 0)
      return State::DID_EXIT;

    return static_cast<State>(state);
  }
};

class UpdateRequested : public EventInfo {
  public:
  UpdateRequested() {}
  UpdateRequested(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return UPDATE_REQUESTED;
  }
};

class TimeZoneChanged : public EventInfo {
 public:
  TimeZoneChanged() {}
  TimeZoneChanged(app_event_info_h h) : EventInfo(h) {}

  Event GetEvent() const override {
    return TIME_ZONE_CHANGED;
  }
};

template <typename EVENT_ADDER, typename EVENT_REMOVER>
class AppBase {
 public:
  template <typename REMOVER>
  class UniqueEventHandler {
   public:
    UniqueEventHandler(REMOVER r, app_event_handler_h h)
        : remover_(r), handle_(h) {}
    ~UniqueEventHandler() {
      if (handle_)
        remover_(handle_);
    }

    UniqueEventHandler(const UniqueEventHandler& h) = delete;
    UniqueEventHandler& operator = (const UniqueEventHandler& h) = delete;

    UniqueEventHandler(UniqueEventHandler&& h) noexcept {
      handle_ = h.handle_;
      h.handle_ = nullptr;
    }

    UniqueEventHandler& operator = (UniqueEventHandler&& h) noexcept {
      handle_ = h.handle_;
      h.handle_ = nullptr;
      return *this;
    }

   private:
    REMOVER remover_;
    app_event_handler_h handle_;
  };

  AppBase(EVENT_ADDER adder, EVENT_REMOVER remover)
      : ev_adder_(adder), ev_remover_(remover) {}

  std::string GetId() const {
    char* id = nullptr;
    int r = app_get_id(&id);
    if (r != 0)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(id, std::free);

    return id;
  }

  std::string GetName() const {
    char* name = nullptr;
    int r = app_get_name(&name);
    if (r != 0)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(name, std::free);

    return name;
  }

  std::string GetVersion() const {
    char* version = nullptr;
    int r = app_get_version(&version);
    if (r != 0)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(version, std::free);

    return version;
  }

  std::string GetDataPath() const {
    char* path = app_get_data_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetCachePath() const {
    char* path = app_get_cache_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetResourcePath() const {
    char* path = app_get_resource_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetSharedDataPath() const {
    char* path = app_get_shared_data_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetSharedResourcePath() const {
    char* path = app_get_shared_resource_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetSharedTrustedPath() const {
    char* path = app_get_shared_trusted_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetExternalDataPath() const {
    char* path = app_get_external_data_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetExternalCachePath() const {
    char* path = app_get_external_cache_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  std::string GetTepResourcePath() const {
    char* path = app_get_tep_resource_path();
    if (!path)
      return "";
    std::unique_ptr<char, decltype(std::free)*> p(path, std::free);

    return path;
  }

  template <typename EVENT, typename CB>
  UniqueEventHandler<EVENT_REMOVER> AddEvent(CB cb) {
    app_event_handler_h ev;
    ev_adder_(
        &ev, static_cast<app_event_type_e>(EVENT().GetEvent()),
        [](app_event_info_h event_info, void* user_data) {
          auto* c = static_cast<CB*>(user_data);
          auto info = reinterpret_cast<AppEvent*>(event_info);
          (*c)(EVENT(info->GetHandle()));
        },
        &cb);

    return UniqueEventHandler<EVENT_REMOVER>(ev_remover_, ev);
  }

 private:
  EVENT_ADDER ev_adder_;
  EVENT_REMOVER ev_remover_;
};

}  // namespace app_common
}  // namespace tizen_appfw

#endif  // APP_COMMON_HPP_
