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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_REMOTE_SURFACE_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_REMOTE_SURFACE_H_

#include <string>
#include <memory>

#include <screen_connector_input/key_event_args.hh>

#include "screen_connector_remote_surface/buffer_event_interface.h"
#include "screen_connector_remote_surface/trs_interface.h"
#include "screen_connector_remote_surface/common.h"

namespace screen_connector {

class EXPORT_API RemoteSurface : public IBufferEvent {
 public:
  enum Type {
    WIDGET = 0x1,
    WATCH = 0x2,
    UI = 0x4,
    COMPONENT_BASED = 0x8,
    ALL = WIDGET | WATCH | UI | COMPONENT_BASED
  };

  enum ChangedEventFilter {
    FILTER_NONE,
    FILTER_TBM,
    FILTER_IMAGE_FILE,
    FILTER_ALL
  };

  RemoteSurface(const std::string& id, Type type,
      std::shared_ptr<WlSurface> wl_surface, bool mock = false);
  RemoteSurface(int rid, const std::string& id, Type type,
      std::shared_ptr<WlSurface> wl_surface, bool mock = false);
  RemoteSurface(RemoteSurface&&) noexcept;
  RemoteSurface& operator=(RemoteSurface&&) noexcept;
  virtual ~RemoteSurface();

  const std::string& GetAppId() const;
  const std::string& GetInstId() const;
  int GetPid() const;
  int GetRid() const;
  Type GetType() const;
  bool IsFrozen() const;
  ITRS* GetRaw();
  ChangedEventFilter GetChangedEventFilter() const;
  void SendVisibility(bool visible);
  void SendTouchCancel();
  void SendMouseUp(int x, int y, unsigned int timestamp);
  void Bind(std::shared_ptr<WlSurface> surface);
  void Freeze(bool visible);
  void Thaw();
  virtual void SetChangedEventFilter(ChangedEventFilter filter);
  bool IsRemoteRender() const;
  void ClearRemoteRender();
  int SetRemoteRender();
  void SetBlock(bool is_block);
  virtual void Unbind();
  bool IsBound() const;
  bool SendKeyEvent(const input::KeyEventArgs& args);

  void OnBufferAdded(const std::string& appId,
                     const std::string& instId, int pid) override;
  void OnBufferRemoved(const std::string& appId,
                       const std::string& instId, int pid) override;
  void OnBufferChanged(int type, std::shared_ptr<WlBuffer> tbm, int fd,
                       uint32_t size, uint32_t time) override;
  void OnBind() override;

  static void SetDelayedResumingTime(uint32_t ms);
  static void InitManager();
  static void FiniManager();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_REMOTE_SURFACE_H_
