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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_WATCHER_EVENT_INTERFACE_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_WATCHER_EVENT_INTERFACE_H_

#include <string>

#include "screen_connector_remote_surface_evas/evas_object.h"

namespace screen_connector {

class EXPORT_API IWatcherEvent {
 public:
  virtual void OnWatcherAdded(const std::string& appId,
                              const std::string& instId, int pid) = 0;
  virtual void OnWatcherRemoved(const std::string& appId,
                                const std::string& instId, int pid) = 0;
  virtual void OnWatcherChanged(const std::string& appId,
                                const std::string& instId, int pid,
                                const EvasObject& image) = 0;
  virtual void OnWatcherFocusChanged(const std::string& appId,
                                     const std::string& instId, int pid) = 0;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_WATCHER_EVENT_INTERFACE_H_
