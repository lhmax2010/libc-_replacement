/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_AUL_HANDLE_INTERNAL_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_AUL_HANDLE_INTERNAL_H_

#include <aul_screen_connector.h>

#include <string>

namespace screen_connector {

class AulHandle {
 public:
  class IEventListener {
   public:
     virtual void OnAppAdded(const std::string& appId,
                             const std::string& instId, const int pid,
                             const unsigned int surfaceId) = 0;
     virtual void OnAppRemoved(const std::string& appId,
                               const std::string& instId, const int pid,
                               const unsigned int surfaceId) = 0;
     virtual void OnAppUpdated(const std::string& appId,
                               const std::string& instId, const int pid,
                               const unsigned int surfaceId) = 0;
     virtual void OnAppFocused(const std::string& appId,
                               const std::string& instId, const int pid,
                               const unsigned int surfaceId) = 0;
  };

  AulHandle(IEventListener* listener, aul_screen_type_e type)
      : listener_(listener) {
    bool isPrivate = (type == AUL_SCREEN_TYPE_UI) ? false : true;
    aul_screen_connector_add_screen_viewer(AulScreenViewerCB, type, isPrivate,
        listener_, &handle_);
  }

  explicit AulHandle(IEventListener* listener)
      : listener_(listener), handle_(nullptr) {}

  virtual ~AulHandle() {
    if (handle_)
      aul_screen_connector_remove_screen_viewer(handle_);
  }

  IEventListener* GetListener() const {
    return listener_;
  }

 private:
  static void AulScreenViewerCB(const char* appId, const char* instanceId,
                                const int pid, const unsigned int surfaceId,
                                aul_screen_connector_event_type_e event_type,
                                void* data) {
    IEventListener* rs = static_cast<IEventListener*>(data);

    if (event_type == AUL_SCREEN_CONNECTOR_EVENT_TYPE_ADD)
      rs->OnAppAdded(appId, instanceId, pid, surfaceId);
    else if (event_type == AUL_SCREEN_CONNECTOR_EVENT_TYPE_REMOVE)
      rs->OnAppRemoved(appId, instanceId, pid, surfaceId);
    else if (event_type == AUL_SCREEN_CONNECTOR_EVENT_TYPE_UPDATE)
      rs->OnAppUpdated(appId, instanceId, pid, surfaceId);
    else if (event_type == AUL_SCREEN_CONNECTOR_EVENT_TYPE_FOCUS)
      rs->OnAppFocused(appId, instanceId, pid, surfaceId);
  }

 private:
  IEventListener* listener_;
  aul_screen_viewer_h handle_ = NULL;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_AUL_HANDLE_INTERNAL_H_
