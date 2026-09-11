/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_IMAGE_EVENT_LISTENER_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_IMAGE_EVENT_LISTENER_H_

#include <memory>
#include <string>

#include <Ecore_Wl2.h>

#include "screen_connector_remote_surface_evas/evas_object.h"
#include "screen_connector_remote_surface/remote_surface.h"
#include "screen_connector_remote_surface/trs_interface.h"

namespace screen_connector {

class EXPORT_API ImageEventListener {
 public:
  ImageEventListener(RemoteSurface* parent);
  virtual ~ImageEventListener();
  virtual void OnMouseIn(const EvasObject& obj, void* eventInfo);
  virtual void OnMouseOut(const EvasObject& obj, void* eventInfo);
  virtual void OnMouseDown(const EvasObject& obj, void* eventInfo);
  virtual void OnMouseUp(const EvasObject& obj, void* eventInfo);
  virtual void OnTouchMultiDown(const EvasObject& obj, void* eventInfo);
  virtual void OnTouchMultiUp(const EvasObject& obj, void* eventInfo);
  virtual void OnMouseMove(const EvasObject& obj, void* eventInfo);
  virtual void OnMouseWheel(const EvasObject& obj, void* eventInfo);
  virtual void OnMove(const EvasObject& obj, void* eventInfo);
  virtual void OnShow(const EvasObject& obj, void* eventInfo);
  virtual void OnHide(const EvasObject& obj, void* eventInfo);
  virtual void OnResize(const EvasObject& obj, void* eventInfo);
  virtual void OnDownKey(void* eventInfo);
  virtual void OnUpKey(void* eventInfo);
  void SetInputEventFilter(uint32_t filter);
  bool IsDisabledInput(ITRS::InputType type);
  bool IsFlickEnabled(bool enable);

 private:
  static Eina_Bool KeyUpCb(void* data, int type, void* event);
  static Eina_Bool KeyDownCb(void* data, int type, void* event);

 private:
  RemoteSurface* parent_ = nullptr;
  bool is_flick_enabled_ = false;
  uint32_t input_event_filter_ = 0;
  bool flick_ = false;
  int prev_y_ = 0;
  Ecore_Event_Handler* key_down_ = nullptr;
  Ecore_Event_Handler* key_up_ = nullptr;
};
}

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_IMAGE_EVENT_LISTENER_H_