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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_INTERNAL_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_INTERNAL_H_

#include <unistd.h>
#include <map>
#include <memory>

#include "screen_connector_remote_surface/handle.h"
#include "screen_connector_remote_surface/trs_interface.h"

namespace screen_connector {

class TRS : public Handle<struct tizen_remote_surface*>, public ITRS {
 public:
  TRS(struct tizen_remote_surface* raw, bool owner)
      : Handle<struct tizen_remote_surface*>(raw, owner) { }

  virtual ~TRS() {
    if (IsOwner()) {
      tizen_remote_surface_set_user_data(GetRaw(), NULL);
      tizen_remote_surface_unredirect(GetRaw());
      tizen_remote_surface_destroy(GetRaw());
    }
  }

  struct tizen_remote_surface* GetHandle() const override {
    return GetRaw();
  }

  void TransferVisibility(bool visible) override {
    if (visible) {
      tizen_remote_surface_transfer_visibility(GetRaw(),
        TIZEN_REMOTE_SURFACE_VISIBILITY_TYPE_VISIBLE);
    } else {
      tizen_remote_surface_transfer_visibility(GetRaw(),
        TIZEN_REMOTE_SURFACE_VISIBILITY_TYPE_INVISIBLE);
    }
  }

  void TransferTouchCancel() override {
    if (!is_down_)
      return;
    is_down_ = false;
    tizen_remote_surface_transfer_touch_cancel(GetRaw());
  }

  void TransferMouseUp(int x, int y, unsigned int timestamp) override {
    tizen_remote_surface_transfer_mouse_event(GetRaw(),
        TIZEN_REMOTE_SURFACE_EVENT_TYPE_MOUSE_UP,
        0, 1, x, y, 0, 0, 0, 0, TIZEN_INPUT_DEVICE_CLAS_MOUSE,
        TIZEN_INPUT_DEVICE_SUBCLAS_NONE, "", timestamp);
  }

  void SetRemoteRender(bool set) override {
    if (set)
      tizen_remote_surface_set_remote_render(GetRaw(), 1);
    else
      tizen_remote_surface_set_remote_render(GetRaw(), 0);
  }

  void SetChangedBufferEventFilter(int filter) override {
    tizen_remote_surface_set_changed_buffer_event_filter(GetRaw(), filter);
  }

  void AddListener(ITRS::IEventListener* listener) override {
    static const struct tizen_remote_surface_listener RSListener = {
      BufferUpdatedCB,
      BufferMissingCB,
      BufferChangedCB,
      InputEventFilterCB,
    };

    listener_ = listener;
    tizen_remote_surface_add_listener(GetRaw(), &RSListener, listener_);
  }

  void Redirect() override {
    tizen_remote_surface_redirect(GetRaw());
  }

  void TransferTouchEvent(uint32_t event_type, int32_t device,
                          int32_t button, int32_t x, int32_t y,
                          wl_fixed_t radius_x, wl_fixed_t radius_y,
                          wl_fixed_t pressure, wl_fixed_t angle,
                          uint32_t clas, uint32_t subclas,
                          const char *identifier, uint32_t time) override {
    tizen_remote_surface_transfer_touch_event(GetRaw(), event_type, device,
        button, x, y, radius_x, radius_y, pressure, angle, clas, subclas,
        identifier, time);
  }

  void TransferKeyEvent(uint32_t event_type, int32_t keycode,
                          uint32_t clas, uint32_t subclas,
                          const char *identifier, uint32_t time) override {
    tizen_remote_surface_transfer_key_event(GetRaw(), event_type, keycode, clas,
        subclas, identifier, time);
  }

  void TransferMouseEvent(uint32_t event_type, int32_t device,
                          int32_t button, int32_t x, int32_t y,
                          wl_fixed_t radius_x, wl_fixed_t radius_y,
                          wl_fixed_t pressure, wl_fixed_t angle,
                          uint32_t clas, uint32_t subclas,
                          const char *identifier, uint32_t time) override {
    if (event_type == TIZEN_REMOTE_SURFACE_EVENT_TYPE_MOUSE_DOWN)
      is_down_ = true;
    else if (event_type == TIZEN_REMOTE_SURFACE_EVENT_TYPE_MOUSE_UP)
      is_down_ = false;
    tizen_remote_surface_transfer_mouse_event(GetRaw(), event_type,
        device, button, x, y, radius_x, radius_y, pressure, angle, clas,
        subclas, identifier, time);
  }

  void TransferMouseWheel(uint32_t direction, int32_t z, uint32_t clas,
                          uint32_t subclas, const char *identifier,
                          uint32_t time) override {
    tizen_remote_surface_transfer_mouse_wheel(GetRaw(), direction, z, clas,
        subclas, identifier, time);
  }

  uint32_t GetRawInputType(InputType type) override {
    return input_type_table_[type];
  }

 private:
  std::map<int, uint32_t>  input_type_table_ = {
    {MOUSE_UP_DOWN, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_MOUSE_UP_DOWN},
    {MOUSE_MOVE_X, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_MOUSE_MOVE_X},
    {MOUSE_MOVE_Y, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_MOUSE_MOVE_Y},
    {MOUSE_MOVE_X_Y, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_MOUSE_MOVE_X_Y},
    {MOUSE_WHEEL, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_MOUSE_WHEEL_ALL},
    {TOUCH_UP_DOWN, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_TOUCH_UP_DOWN},
    {TOUCH_MOVE_X, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_TOUCH_MOVE_X},
    {TOUCH_MOVE_Y, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_TOUCH_MOVE_Y},
    {TOUCH_MOVE_X_Y, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_TOUCH_MOVE_X_Y},
    {TOUCH_CANCEL, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_TOUCH_CANCEL_ALL},
    {KEY, TIZEN_REMOTE_SURFACE_INPUT_EVENT_FILTER_KEY_ALL}
  };

  static void BufferUpdatedCB(void* data, struct tizen_remote_surface* trs,
                              struct wl_buffer* buffer, uint32_t time) {
  }

  static void BufferMissingCB(void* data, struct tizen_remote_surface* trs) {
    IEventListener* l = static_cast<IEventListener*>(data);

    l->OnRawBufferMissing();
  }

  static void BufferChangedCB(void* data, struct tizen_remote_surface* trs,
                       uint32_t type, struct wl_buffer* tbm,
                       int32_t img_file_fd, uint32_t img_file_size,
                       uint32_t time, struct wl_array* keys) {
    IEventListener* l = static_cast<IEventListener*>(data);
    l->OnRawBufferChanged(type,
        std::make_shared<WlBuffer>(tbm, trs, true),
        img_file_fd, img_file_size, time);
    close(img_file_fd);
  }

  static void InputEventFilterCB(void* data, struct tizen_remote_surface* trs,
                          uint32_t event_filter) {
    IEventListener* l = static_cast<IEventListener*>(data);

    l->OnInputFilterChanged(event_filter);
  }

 private:
  ITRS::IEventListener* listener_ = nullptr;
  bool is_down_ = false;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_INTERNAL_H_
