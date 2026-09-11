/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
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

#ifndef SCREEN_CONNECTOR_WINDOW_CONTEXT_H_
#define SCREEN_CONNECTOR_WINDOW_CONTEXT_H_

#include <string>
#include <memory>
#include <map>

#include "screen_connector_remote_surface/ecore_event_handler_internal.h"
#include "screen_connector_remote_surface/common.h"

namespace screen_connector {

class EXPORT_API WindowContext : public EcoreEventHandler::IEventListener {
 public:
  WindowContext();
  virtual ~WindowContext() = default;

  bool IsFullyObscured(unsigned int win) {
    if (win_map_.find(win) == win_map_.end())
      return false;
    return win_map_[win];
  }

  unsigned int FindWindow(unsigned int win) {
    if (win_map_.find(win) == win_map_.end())
      return 0;
    return win;
  }

 private:
  void OnWindowShow(unsigned int win) override;
  void OnWindowHide(unsigned int win) override;
  void OnWindowVisibilityChange(unsigned int win,
                                bool fully_obscured) override;
  void OnWindowPreVisibilityChange(unsigned int win,
                                   bool fully_obscured) override;

 private:
  void UpdateWindow(unsigned int win, bool fully_obscured);
  void RemoveWindow(unsigned int win);

 private:
  std::unique_ptr<EcoreEventHandler> ecore_event_handler_;
  std::map<unsigned int, bool> win_map_;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_WINDOW_CONTEXT_H_
