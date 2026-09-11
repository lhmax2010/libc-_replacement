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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_INTERFACE_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_INTERFACE_H_

#include <tizen-remote-surface-client-protocol.h>

#include <memory>
#include <string>

#include "screen_connector_remote_surface/common.h"

namespace screen_connector {

class EXPORT_API ITRS {
 public:
  class EXPORT_API IEventListener {
   public:
    virtual void OnRawBufferMissing() = 0;
    virtual void OnRawBufferChanged(int type, std::shared_ptr<WlBuffer> tbm,
                                    int fd, uint32_t size, uint32_t time) = 0;
    virtual void OnInputFilterChanged(uint32_t event_filter) = 0;
  };

  enum InputType {
    MOUSE_UP_DOWN,
    MOUSE_MOVE_X,
    MOUSE_MOVE_Y,
    MOUSE_MOVE_X_Y,
    MOUSE_WHEEL,
    TOUCH_UP_DOWN,
    TOUCH_MOVE_X,
    TOUCH_MOVE_Y,
    TOUCH_MOVE_X_Y,
    TOUCH_CANCEL,
    KEY
  };

  virtual ~ITRS() = default;
  virtual struct tizen_remote_surface* GetHandle() const = 0;
  virtual void TransferVisibility(bool visible) = 0;
  virtual void TransferTouchCancel() = 0;
  virtual void TransferMouseUp(int x, int y, unsigned int timestamp) = 0;
  virtual void SetRemoteRender(bool set) = 0;
  virtual void SetChangedBufferEventFilter(int filter) = 0;
  virtual void AddListener(IEventListener* listener) = 0;
  virtual void Redirect() = 0;
  virtual uint32_t GetRawInputType(InputType type) = 0;
  virtual void TransferTouchEvent(uint32_t event_type, int32_t device,
                                  int32_t button, int32_t x, int32_t y,
                                  wl_fixed_t radius_x, wl_fixed_t radius_y,
                                  wl_fixed_t pressure, wl_fixed_t angle,
                                  uint32_t clas, uint32_t subclas,
                                  const char *identifier, uint32_t time) = 0;
  virtual void TransferMouseEvent(uint32_t event_type, int32_t device,
                                  int32_t button, int32_t x, int32_t y,
                                  wl_fixed_t radius_x, wl_fixed_t radius_y,
                                  wl_fixed_t pressure, wl_fixed_t angle,
                                  uint32_t clas, uint32_t subclas,
                                  const char *identifier, uint32_t time) = 0;
  virtual void TransferKeyEvent(uint32_t event_type, int32_t keycode,
                                  uint32_t clas, uint32_t subclas,
                                  const char *identifier, uint32_t time) = 0;
  virtual void TransferMouseWheel(uint32_t direction, int32_t z, uint32_t clas,
                                  uint32_t subclas, const char *identifier,
                                  uint32_t time) = 0;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_INTERFACE_H_
