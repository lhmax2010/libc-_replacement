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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_EVAS_OBJECT_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_EVAS_OBJECT_H_

#include <Elementary.h>

#include "screen_connector_remote_surface/handle.h"

namespace screen_connector {

class EXPORT_API EvasObject : public Handle<Evas_Object*> {
 public:
  EvasObject(Evas_Object* raw, bool owner);
  virtual ~EvasObject();
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_EVAS_EVAS_OBJECT_H_
