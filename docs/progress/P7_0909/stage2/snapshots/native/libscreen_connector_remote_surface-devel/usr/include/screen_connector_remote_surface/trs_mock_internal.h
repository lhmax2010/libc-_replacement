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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_MOCK_INTERNAL_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_MOCK_INTERNAL_H_

#include <glib.h>
#include <tizen-remote-surface-client-protocol.h>

#include <memory>

#include "screen_connector_remote_surface/trs_internal.h"

namespace screen_connector {
namespace test {

class TRSMock : public TRS {
 public:
  TRSMock()
      : TRS(nullptr, false) { }

  virtual ~TRSMock() {
    if (update_timer_ > 0)
      g_source_remove(update_timer_);
  }

  struct tizen_remote_surface* GetHandle() const override {
    return nullptr;
  }

  void TransferVisibility(bool visible) override {
  }

  void TransferTouchCancel() override {
  }

  void TransferMouseUp(int x, int y, unsigned int timestamp) override {
  }

  void SetRemoteRender(bool set) override {
  }

  void SetChangedBufferEventFilter(int filter) override {
  }

  void AddListener(ITRS::IEventListener* listener) override {
    listener_ = listener;
  }

  void Redirect() override {
    if (update_timer_ > 0)
      g_source_remove(update_timer_);

    update_timer_ = g_timeout_add(1000, [](gpointer user_data)->gboolean {
          TRSMock* mock = static_cast<TRSMock*>(user_data);
          mock->OnUpdate();
          return G_SOURCE_CONTINUE;
        }, this);
  }

  void TransferTouchEvent(uint32_t event_type, int32_t device,
                          int32_t button, int32_t x, int32_t y,
                          wl_fixed_t radius_x, wl_fixed_t radius_y,
                          wl_fixed_t pressure, wl_fixed_t angle,
                          uint32_t clas, uint32_t subclas,
                          const char *identifier, uint32_t time) override {
  }

  void TransferMouseEvent(uint32_t event_type, int32_t device,
                          int32_t button, int32_t x, int32_t y,
                          wl_fixed_t radius_x, wl_fixed_t radius_y,
                          wl_fixed_t pressure, wl_fixed_t angle,
                          uint32_t clas, uint32_t subclas,
                          const char *identifier, uint32_t time) override {
  }

  void TransferKeyEvent(uint32_t event_type, int32_t keycode,
                          uint32_t clas, uint32_t subclas,
                          const char *identifier, uint32_t time) override {
  }

  void TransferMouseWheel(uint32_t direction, int32_t z, uint32_t clas,
                          uint32_t subclas, const char *identifier,
                          uint32_t time) override {
  }

 private:
  void OnUpdate() {
    if (listener_ == nullptr)
      return;

    listener_->OnRawBufferChanged(TIZEN_REMOTE_SURFACE_BUFFER_TYPE_TBM,
        std::make_shared<WlBuffer>(nullptr, nullptr, false), 0, 0, 0);
  }

 private:
  ITRS::IEventListener* listener_ = nullptr;
  unsigned int update_timer_ = 0;
};

}  // namespace test
}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_TRS_MOCK_INTERNAL_H_
