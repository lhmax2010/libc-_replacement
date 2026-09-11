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

#ifndef SCREEN_CONNECTOR_ECORE_EVENT_HANDLER_INTERNAL_H_
#define SCREEN_CONNECTOR_ECORE_EVENT_HANDLER_INTERNAL_H_

#include <Ecore_Wl2.h>

#include <string>
#include <memory>

#include "screen_connector_remote_surface/common.h"

namespace screen_connector {

class EcoreEventHandler {
 public:
  class IEventListener {
   public:
    virtual void OnWindowShow(unsigned int win) = 0;
    virtual void OnWindowHide(unsigned int win) = 0;
    virtual void OnWindowVisibilityChange(unsigned int win,
                                          bool fully_obscured) = 0;
    virtual void OnWindowPreVisibilityChange(unsigned int win,
                                             bool fully_obscured) = 0;
  };

  explicit EcoreEventHandler(IEventListener* listener) : listener_(listener) {
    show_ = ecore_event_handler_add(
        ECORE_WL2_EVENT_WINDOW_SHOW,
        EcoreEventCB, listener_);
    hide_ = ecore_event_handler_add(
        ECORE_WL2_EVENT_WINDOW_HIDE,
        EcoreEventCB, listener_);
    visibility_change_ = ecore_event_handler_add(
        ECORE_WL2_EVENT_WINDOW_VISIBILITY_CHANGE,
        EcoreEventCB, listener_);
    pre_visibility_change_ = ecore_event_handler_add(
        ECORE_WL2_EVENT_WINDOW_PRE_VISIBILITY_CHANGE,
        EcoreEventCB, listener_);
  }

  virtual ~EcoreEventHandler() {
    if (pre_visibility_change_)
      ecore_event_handler_del(pre_visibility_change_);
    if (visibility_change_)
      ecore_event_handler_del(visibility_change_);
    if (hide_)
      ecore_event_handler_del(hide_);
    if (show_)
      ecore_event_handler_del(show_);
  }

 private:
  static Eina_Bool EcoreEventCB(void* data, int type, void* event) {
    IEventListener* rs = static_cast<IEventListener*>(data);

    if (type == ECORE_WL2_EVENT_WINDOW_SHOW) {
      Ecore_Wl2_Event_Window_Show* ev =
          static_cast<Ecore_Wl2_Event_Window_Show*>(event);
      rs->OnWindowShow(ev->win);
    } else if (type == ECORE_WL2_EVENT_WINDOW_HIDE) {
      Ecore_Wl2_Event_Window_Hide* ev =
          static_cast<Ecore_Wl2_Event_Window_Hide*>(event);
      rs->OnWindowHide(ev->win);
    } else if (type == ECORE_WL2_EVENT_WINDOW_VISIBILITY_CHANGE) {
      Ecore_Wl2_Event_Window_Visibility_Change* ev =
          static_cast<Ecore_Wl2_Event_Window_Visibility_Change*>(event);
      rs->OnWindowVisibilityChange(ev->win, ev->fully_obscured);
    } else if (type == ECORE_WL2_EVENT_WINDOW_PRE_VISIBILITY_CHANGE) {
      Ecore_Wl2_Event_Window_Pre_Visibility_Change* ev =
          static_cast<Ecore_Wl2_Event_Window_Pre_Visibility_Change*>(event);
      if (ev->type == ECORE_WL2_WINDOW_VISIBILITY_TYPE_PRE_UNOBSCURED) {
        rs->OnWindowPreVisibilityChange(ev->win, false);
      } else if (ev->type == ECORE_WL2_WINDOW_VISIBILITY_TYPE_FULLY_OBSCURED) {
        rs->OnWindowPreVisibilityChange(ev->win, true);
      }
    }
    return ECORE_CALLBACK_RENEW;
  }

 private:
  IEventListener* listener_;
  Ecore_Event_Handler* show_ = nullptr;
  Ecore_Event_Handler* hide_ = nullptr;
  Ecore_Event_Handler* visibility_change_ = nullptr;
  Ecore_Event_Handler* pre_visibility_change_ = nullptr;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_ECORE_EVENT_HANDLER_INTERNAL_H_
