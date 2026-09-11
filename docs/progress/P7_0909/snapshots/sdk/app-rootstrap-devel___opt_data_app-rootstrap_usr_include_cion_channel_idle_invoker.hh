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

#ifndef CION_CHANNEL_IDLE_INVOKER_HH_
#define CION_CHANNEL_IDLE_INVOKER_HH_

#include <glib.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "cion/common/util/logging.hh"

namespace cion {
namespace channel {

class Channel;
class IdContainer;

struct IdleCallbackData {
  IdleCallbackData(std::function<void()> f, IdContainer* ct)
      : cb(std::move(f)), container(ct) { }
  ~IdleCallbackData() = default;
  std::function<void()> cb;
  IdContainer* container;
};

class IdContainer {
 public:
  ~IdContainer() {
    std::lock_guard<std::mutex> lock(mutex_);

    for (const auto& i : ids_)
      g_source_remove(i.second);
    ids_.clear();
  }

  void Add(std::unique_ptr<IdleCallbackData> ptr, guint id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto element = std::make_pair(std::move(ptr), id);
    ids_.push_back(std::move(element));
  }
  void Remove(IdleCallbackData* data) {
    std::lock_guard<std::mutex> lock(mutex_);

    ids_.erase(
        std::remove_if(ids_.begin(), ids_.end(),
        [data](const std::pair<
            std::unique_ptr<IdleCallbackData>, guint>& id_data) -> bool {
          if (id_data.first.get() == data)
            return true;
          return false;
        }), ids_.end());
  }

 private:
  std::vector<std::pair<std::unique_ptr<IdleCallbackData>, guint>> ids_;
  std::mutex mutex_;
};

class IdleInvoker {
 public:
  virtual ~IdleInvoker() = default;
  void InvokeAtIdle(std::function<void()> cb);

 private:
  IdContainer ids_;
};

}  // namespace channel
}  // namespace cion

#endif  // CION_CHANNEL_IDLE_INVOKER_HH_
