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

#ifndef TIZEN_APPFW_APP_HPP_
#define TIZEN_APPFW_APP_HPP_

#include <app.h>

#include <app_control.hpp>
#include <app_common.hpp>

namespace tizen_appfw {

class UiAppBase : public app_common::AppBase<
    decltype(ui_app_add_event_handler)*,
    decltype(ui_app_remove_event_handler)*> {
 public:
  using Remover = decltype(ui_app_remove_event_handler)*;

  UiAppBase()
      : AppBase(ui_app_add_event_handler, ui_app_remove_event_handler) {}

  virtual bool OnCreate() { return true; }
  virtual void OnTerminate() {}
  virtual void OnResume() {}
  virtual void OnPause() {}
  virtual void OnAppControl(const ReceivedAppControl& ctrl) {}

  int Run(int argc, char **argv) {
    ui_app_lifecycle_callback_s callback = {
      create : [](void* user_data) -> bool {
        UiAppBase* b = static_cast<UiAppBase*>(user_data);
        return b->OnCreate();
      },
      terminate : [](void* user_data) {
        UiAppBase* b = static_cast<UiAppBase*>(user_data);
        b->OnTerminate();
      },
      pause : [](void* user_data) {
        UiAppBase* b = static_cast<UiAppBase*>(user_data);
        b->OnPause();
      },
      resume : [](void* user_data) {
        UiAppBase* b = static_cast<UiAppBase*>(user_data);
        b->OnResume();
      },
      app_control : [](app_control_h app_control, void* user_data) {
        UiAppBase* b = static_cast<UiAppBase*>(user_data);
        b->OnAppControl(ReceivedAppControl(app_control));
      }
    };

    return ui_app_main(argc, argv, &callback, this);
  }

  void Exit() noexcept {
    ui_app_exit();
  }
};

}  // namespace tizen_appfw

#endif  // TIZEN_APPFW_APP_HPP_