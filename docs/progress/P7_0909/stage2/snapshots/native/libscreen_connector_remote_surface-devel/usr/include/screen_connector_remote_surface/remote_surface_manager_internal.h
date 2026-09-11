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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_REMOTE_SURFACE_MANAGER_INTERNAL_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_REMOTE_SURFACE_MANAGER_INTERNAL_H_

#include <wayland-client.h>
#include <wayland-tbm-client.h>
#include <Ecore_Wl2.h>
#include <wayland-extension/tizen-extension-client-protocol.h>
#include <tizen-remote-surface-client-protocol.h>

#include "screen_connector_remote_surface/handle.h"
#include "screen_connector_remote_surface/common.h"
#include "screen_connector_remote_surface/trs_interface.h"

namespace screen_connector {

class RemoteSurfaceManager {
 public:
  RemoteSurfaceManager(const RemoteSurfaceManager&) = delete;
  RemoteSurfaceManager& operator = (const RemoteSurfaceManager&) = delete;

  static RemoteSurfaceManager& GetInst(bool mock = false);
  ITRS* CreateTRS(int rid, std::shared_ptr<WlSurface> suf, bool mock = false);
  void Dispose();
  void Bind(const ITRS& trs, const WlSurface& suf);
  void Unbind(const ITRS& trs);

 private:
  RemoteSurfaceManager();
  ~RemoteSurfaceManager();
  void Init();

 private:
  struct tizen_remote_surface_manager* rsm_ = nullptr;
  struct wayland_tbm_client* tbm_client_ = nullptr;
  struct wl_tbm* tbm_ = nullptr;
  bool is_init_ = false;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_REMOTE_SURFACE_MANAGER_INTERNAL_H_
