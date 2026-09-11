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

#ifndef SCREEN_CONNECTOR_REMOTE_SURFACE_BUFFER_EVENT_INTERFACE_H_
#define SCREEN_CONNECTOR_REMOTE_SURFACE_BUFFER_EVENT_INTERFACE_H_

#include <memory>
#include <string>

#include "screen_connector_remote_surface/handle.h"
#include "screen_connector_remote_surface/common.h"

namespace screen_connector {

class EXPORT_API IBufferEvent {
 public:
  virtual void OnBufferAdded(const std::string& appId,
                             const std::string& instId, int pid) = 0;
  virtual void OnBufferRemoved(const std::string& appId,
                               const std::string& instId, int pid) = 0;
  virtual void OnBufferChanged(int type, std::shared_ptr<WlBuffer> tbm, int fd,
                               uint32_t size, uint32_t time) = 0;
  virtual void OnBind() = 0;
};

}  // namespace screen_connector

#endif  // SCREEN_CONNECTOR_REMOTE_SURFACE_BUFFER_EVENT_INTERFACE_H_
