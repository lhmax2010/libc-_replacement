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

#ifndef CION_CHANNEL_CHANNEL_JOB_HH_
#define CION_CHANNEL_CHANNEL_JOB_HH_

#include <queue>
#include <string>
#include <memory>

#include "cion/channel/client_channel.hh"

namespace cion {

class VineDp;

template <class T>
class ChannelJob {
 public:
  ChannelJob() : channel_(nullptr) {
  }
  ChannelJob(T* channel,
      std::shared_ptr<tizen_base::Parcel> parcel, std::shared_ptr<VineDp> dp)
    : channel_(channel), parcel_(parcel), client_dp_(dp) {
  }

  ChannelJob(T* channel, std::shared_ptr<tizen_base::Parcel> parcel)
    : channel_(channel), parcel_(parcel) {
  }
  ~ChannelJob() = default;

  std::shared_ptr<VineDp> GetClientDp() {
    return client_dp_;
  }

  std::shared_ptr<tizen_base::Parcel> GetParcel() {
    return parcel_;
  }

  T* GetChannel() {
    return channel_;
  }

 private:
  T* channel_;
  std::shared_ptr<tizen_base::Parcel> parcel_;
  std::shared_ptr<VineDp> client_dp_;
};

}  // namespace cion

#endif  // CION_CHANNEL_CHANNEL_JOB_HH_
