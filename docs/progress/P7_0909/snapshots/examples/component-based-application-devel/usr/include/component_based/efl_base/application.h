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

#ifndef COMPONENT_BASED_EFL_BASE_APPLICATION_H_
#define COMPONENT_BASED_EFL_BASE_APPLICATION_H_

#include "component_based/base/application_base.h"
#include "component_based/base/frame_component.h"
#include "component_based/base/service_component.h"
#include "component_based/efl_base/elm_window.h"

#ifndef EXPORT_API
#define EXPORT_API __attribute__((visibility("default")))
#endif

namespace component_based {

class EXPORT_API Application : public ApplicationBase {
 public:
  Application();
  virtual ~Application() = default;

  std::map<std::string, std::unique_ptr<Component::FactoryBase>>
      OnCreateFactoryMap() override;
  void OnLoopInit(int argc, char** argv) override;
  void OnLoopFinish() override;
  void OnLoopRun() override;
  void OnLoopExit() override;
};

}  // namespace component_based

#endif  // COMPONENT_BASED_EFL_BASE_APPLICATION_H_
