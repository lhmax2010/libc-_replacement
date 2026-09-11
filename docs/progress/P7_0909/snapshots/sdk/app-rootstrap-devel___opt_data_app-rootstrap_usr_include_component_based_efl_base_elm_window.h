/*
 * Copyright (c) 2019 - 2021 Samsung Electronics Co., Ltd.
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

#ifndef COMPONENT_BASED_EFL_BASE_ELM_WINDOW_H_
#define COMPONENT_BASED_EFL_BASE_ELM_WINDOW_H_

#include <Elementary.h>

#include <string>

#include "component_based/base/window_interface.h"

#ifndef EXPORT_API
#define EXPORT_API __attribute__((visibility("default")))
#endif

namespace component_based {

class EXPORT_API ElmWindow : public IWindow {
 public:
  ElmWindow(std::string id);
  ElmWindow(Evas_Object* win);
  virtual ~ElmWindow();

  int GetResID() const override;
  void* GetRaw() override;

 private:
  Evas_Object* win_ = nullptr;
};

}  // namespace component_based

#endif  // COMPONENT_BASED_EFL_BASE_ELM_WINDOW_H_
