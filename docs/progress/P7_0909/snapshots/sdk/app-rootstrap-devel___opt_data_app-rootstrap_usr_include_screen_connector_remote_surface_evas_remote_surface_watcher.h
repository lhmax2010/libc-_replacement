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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_REMOTE_SURFACE_WATCHER_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_REMOTE_SURFACE_WATCHER_H_

#include <string>
#include <memory>
#include <map>
#include <list>

#include "screen_connector_remote_surface_evas/evas_object.h"
#include "screen_connector_remote_surface_evas/watcher_event_interface.h"
#include "screen_connector_remote_surface/remote_surface.h"

namespace screen_connector {
class EXPORT_API RemoteSurfaceWatcher : public IWatcherEvent {
 public:
  RemoteSurfaceWatcher(RemoteSurface::Type type,
                       std::shared_ptr<EvasObject> viewerWin);
  RemoteSurfaceWatcher(RemoteSurface::Type type,
                       std::shared_ptr<EvasObject> viewerWin,
                       bool watchFocusedOnly, bool mock = false);
  RemoteSurfaceWatcher(RemoteSurfaceWatcher&&) noexcept;
  RemoteSurfaceWatcher& operator=(RemoteSurfaceWatcher&&) noexcept;
  virtual ~RemoteSurfaceWatcher();

 public:
  void SetChangedEventFilter(RemoteSurface::ChangedEventFilter filter);
  int SetRemoteRender();
  int SetBlock(bool enable);
  void OnWatcherAdded(const std::string& appId, const std::string& instId,
                      const int pid) override;
  void OnWatcherChanged(const std::string& appId, const std::string& instId,
                        const int pid, const EvasObject& image) override;
  void OnWatcherRemoved(const std::string& appId, const std::string& instId,
                        const int pid) override;
  void OnWatcherFocusChanged(const std::string& appId,
                        const std::string& instId, const int pid) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_REMOTE_SURFACE_WATCHER_H_
