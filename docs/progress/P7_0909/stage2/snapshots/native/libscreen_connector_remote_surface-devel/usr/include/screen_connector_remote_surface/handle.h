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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_HANDLE_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_HANDLE_H_

#include <wayland-extension/tizen-extension-client-protocol.h>
#include <tizen-remote-surface-client-protocol.h>
#include <tbm_surface.h>
#include <tbm_surface_internal.h>

#include "screen_connector_remote_surface/common.h"

namespace screen_connector {

template <typename T>
class EXPORT_API Handle {
 public:
  Handle(T raw, bool owner) : raw_(raw), owner_(owner) { }
  virtual ~Handle() { }

  T GetRaw() const { return raw_; }
  bool IsOwner() { return owner_; }
  void SetIsOwner(bool owner) {
    owner_ = owner;
  }

 private:
  T raw_;
  bool owner_;
};

class EXPORT_API WlBuffer : public Handle<struct wl_buffer*> {
 public:
  WlBuffer(struct wl_buffer* raw, struct tizen_remote_surface* trs, bool owner)
      : Handle<struct wl_buffer*>(raw, owner), trs_(trs) { }

  virtual ~WlBuffer() {
    if (IsOwner() && GetRaw() != nullptr) {
      tizen_remote_surface_release(trs_, GetRaw());
      tbm_surface_h curTbmSurface =
          (tbm_surface_h)wl_buffer_get_user_data(GetRaw());
      tbm_surface_internal_unref(curTbmSurface);
      wl_buffer_destroy(GetRaw());
    }
  }

 private:
  struct tizen_remote_surface* trs_;
};

class EXPORT_API WlSurface : public Handle<struct wl_surface*> {
 public:
  WlSurface(struct wl_surface* raw, bool owner) :
      Handle<struct wl_surface*>(raw, owner) { }

  virtual ~WlSurface() { }
};

class EXPORT_API TbmSurface : public Handle<tbm_surface_h> {
 public:
  TbmSurface(tbm_surface_h raw, bool owner) :
      Handle<tbm_surface_h>(raw, owner) { }

  virtual ~TbmSurface() {
    tbm_surface_internal_unref(GetRaw());
  }
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_HANDLE_H_
