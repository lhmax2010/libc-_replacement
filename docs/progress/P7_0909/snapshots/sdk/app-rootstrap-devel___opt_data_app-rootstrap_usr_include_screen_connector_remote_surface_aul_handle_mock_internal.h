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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_AUL_HANDLE_MOCK_INTERNAL_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_AUL_HANDLE_MOCK_INTERNAL_H_

#include <glib.h>
#include <string>

#include "screen_connector_remote_surface/aul_handle_internal.h"

namespace screen_connector {
namespace test {

class AulHandleMock : public AulHandle {
 public:
  enum Id {
    PID = 100,
    RID = 200
  };

  AulHandleMock(IEventListener* listener, aul_screen_type_e type)
      : AulHandle(listener) {
    start_timer_ = g_timeout_add(1000, [](gpointer user_data)->gboolean {
          AulHandleMock* mock = static_cast<AulHandleMock*>(user_data);
          mock->OnStart();
          mock->start_timer_ = 0;
          return G_SOURCE_REMOVE;
        }, this);
    end_timer_ = g_timeout_add(4000, [](gpointer user_data)->gboolean {
          AulHandleMock* mock = static_cast<AulHandleMock*>(user_data);
          mock->OnEnd();
          mock->end_timer_ = 0;
          return G_SOURCE_REMOVE;
        }, this);
    update_timer_ = g_timeout_add(2000, [](gpointer user_data)->gboolean {
          AulHandleMock* mock = static_cast<AulHandleMock*>(user_data);
          mock->OnUpdate();
          mock->update_timer_ = 0;
          return G_SOURCE_REMOVE;
        }, this);
    focus_timer_ = g_timeout_add(3000, [](gpointer user_data)->gboolean {
          AulHandleMock* mock = static_cast<AulHandleMock*>(user_data);
          mock->OnFocus();
          mock->focus_timer_ = 0;
          return G_SOURCE_REMOVE;
        }, this);
  }

  virtual ~AulHandleMock() {
    if (start_timer_ > 0) {
      g_source_remove(start_timer_);
    }

    if (end_timer_ > 0) {
      g_source_remove(end_timer_);
    }

    if (update_timer_ > 0) {
      g_source_remove(update_timer_);
    }

    if (focus_timer_ > 0) {
      g_source_remove(focus_timer_);
    }
  }

 private:
  void OnStart() {
    GetListener()->OnAppAdded("org.tizen.screen_connector.test.appid",
                             "org.tizen.screen_connector.test.appid.instid",
                             PID,
                             RID);
  }

  void OnEnd() {
    GetListener()->OnAppRemoved("org.tizen.screen_connector.test.appid",
                             "org.tizen.screen_connector.test.appid.instid",
                             PID,
                             RID);
  }

  void OnUpdate() {
    GetListener()->OnAppUpdated("org.tizen.screen_connector.test.appid",
                             "org.tizen.screen_connector.test.appid.instid",
                             PID,
                             RID);
  }

  void OnFocus() {
    GetListener()->OnAppFocused("org.tizen.screen_connector.test.appid",
                             "org.tizen.screen_connector.test.appid.instid",
                             PID,
                             RID);
  }

 private:
  unsigned int start_timer_ = 0;
  unsigned int end_timer_ = 0;
  unsigned int update_timer_ = 0;
  unsigned int focus_timer_ = 0;
};

}  // namespace test
}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_AUL_HANDLE_MOCK_INTERNAL_H_
