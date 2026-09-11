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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_IMAGE_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_IMAGE_H_

#include <memory>
#include <string>

#include "screen_connector_remote_surface_evas/evas_object.h"
#include "screen_connector_remote_surface_evas/image_event_listener.h"

namespace screen_connector {

class EXPORT_API Image : public EvasObject {
 public:
  Image(Evas_Object* raw, ImageEventListener* listener, const std::string& instId,
             int pid, void* tag);
  virtual ~Image();

  virtual void Update(std::shared_ptr<WlBuffer> tbm);
  void ClearTbm();

 private:
  static void MouseInCB(void* data, Evas* e, Evas_Object* obj, void* eventInfo);
  static void MouseOutCB(void* data, Evas* e, Evas_Object* obj,
                         void* eventInfo);
  static void MouseDownCB(void* data, Evas* e, Evas_Object* obj,
                          void* eventInfo);
  static void MouseUpCB(void* data, Evas* e, Evas_Object* obj, void* eventInfo);
  static void TouchMultiDownCB(void* data, Evas* e, Evas_Object* obj,
                               void* eventInfo);
  static void TouchMultiUpCB(void* data, Evas* e, Evas_Object* obj,
                             void* eventInfo);
  static void MouseMoveCB(void* data, Evas* e, Evas_Object* obj,
                          void* eventInfo);
  static void MouseWheelCB(void* data, Evas* e, Evas_Object* obj,
                           void* eventInfo);
  static void ShowCB(void* data, Evas* e, Evas_Object* obj, void* eventInfo);
  static void HideCB(void* data, Evas* e, Evas_Object* obj, void* eventInfo);
  static void ResizeCB(void* data, Evas* e, Evas_Object* obj, void* eventInfo);
  static void MoveCB(void* data, Evas* e, Evas_Object* obj, void* eventInfo);

 private:
  ImageEventListener* listener_;
  std::shared_ptr<WlBuffer> prev_buf_;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_IMAGE_H_
