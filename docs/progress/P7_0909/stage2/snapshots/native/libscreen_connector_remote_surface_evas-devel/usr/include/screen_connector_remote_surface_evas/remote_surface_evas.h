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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_REMOTE_SURFACE_EVAS_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_REMOTE_SURFACE_EVAS_H_

#include <string>
#include <memory>
#include <map>

#include "screen_connector_remote_surface_evas/evas_object.h"
#include "screen_connector_remote_surface_evas/evas_event_interface.h"
#include "screen_connector_remote_surface/remote_surface.h"


namespace screen_connector {

class EXPORT_API RemoteSurfaceEvas : public RemoteSurface, public IEvasEvent {
 public:
  enum Visibility {
    UNOBSCURED,
    PARTIALLY_OBSCURED,
    FULLY_OBSCURED,
    UNKNOWN
  };

  RemoteSurfaceEvas(const std::string& id, RemoteSurface::Type type,
                    std::shared_ptr<EvasObject> viewerWin, bool mock = false);
  RemoteSurfaceEvas(int rid, const std::string& id, RemoteSurface::Type type,
                    std::shared_ptr<EvasObject> viewerWin, bool mock = false);
  RemoteSurfaceEvas(RemoteSurfaceEvas&&) noexcept;
  RemoteSurfaceEvas& operator=(RemoteSurfaceEvas&&) noexcept;
  virtual ~RemoteSurfaceEvas();

  void Bind(const EvasObject& win);
  bool IsVisibleArea() const;
  void SendMouseUp();
  Visibility GetVisibility() const;
  void NotifyWindowVisibilityEvent(int type, void* event);

 public:
  void Unbind() override;

  void SetAutoVisibility(bool enable);

  void SetChangedEventFilter(RemoteSurface::ChangedEventFilter filter) override;

  void OnBufferAdded(const std::string& appId, const std::string& instId,
                     int pid) override;
  void OnBufferRemoved(const std::string& appId, const std::string& instId,
                       int pid) override;
  void OnBufferChanged(int type, std::shared_ptr<WlBuffer> tbm, int fd,
                       uint32_t size, uint32_t time) override;
  void OnBind() override;
  void OnEvasAdded(const std::string& appId, const std::string& instId,
                   int pid, const EvasObject& image) override;
  void OnEvasRemoved(const std::string& appId, const std::string& instId,
                     int pid, const EvasObject& image) override;
  void OnEvasChanged(const std::string& appId, const std::string& instId,
                     int pid, const EvasObject& image) override;
  void ClearNoRenderTimer();
  void NoRenderPush(int timeout);

 private:
  Visibility GetVisibility(unsigned int win);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_REMOTE_SURFACE_EVAS_H_
