/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
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

#ifndef TIZEN_APPFW_SERVICE_APP_HPP_
#define TIZEN_APPFW_SERVICE_APP_HPP_

#include <service_app.h>

#include <app_control.hpp>
#include <app_common.hpp>

namespace tizen_appfw {

class ServiceAppBase : public app_common::AppBase<
    decltype(service_app_add_event_handler)*,
    decltype(service_app_remove_event_handler)*> {
 public:
  using Remover = decltype(service_app_remove_event_handler)*;

  ServiceAppBase()
      : AppBase(service_app_add_event_handler,
                service_app_remove_event_handler) {}

  // LCOV_EXCL_START
  virtual bool OnCreate() { return true; }
  virtual void OnTerminate() {}
  virtual void OnAppControl(const ReceivedAppControl& ctrl) {}
  // LCOV_EXCL_STOP

  int Run(int argc, char** argv) {
    service_app_lifecycle_callback_s callback = {
      .create = [](void* user_data) -> bool {
        ServiceAppBase* b = static_cast<ServiceAppBase*>(user_data);
        return b->OnCreate();
      },
      .terminate = [](void* user_data) {
        ServiceAppBase* b = static_cast<ServiceAppBase*>(user_data);
        b->OnTerminate();
      },
      .app_control = [](app_control_h app_control, void* user_data) {
        // LCOV_EXCL_START
        ServiceAppBase* b = static_cast<ServiceAppBase*>(user_data);
        b->OnAppControl(ReceivedAppControl(app_control));
        // LCOV_EXCL_STOP
      }
    };

    return service_app_main(argc, argv, &callback, this);
  }

  void Exit() noexcept {
    service_app_exit();
  }
};

}  // namespace tizen_appfw

#endif  // TIZEN_APPFW_SERVICE_APP_HPP_