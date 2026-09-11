/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

#ifndef CION_CHANNEL_GROUP_CHANNEL_HH_
#define CION_CHANNEL_GROUP_CHANNEL_HH_

#include <string>
#include <vector>
#include <memory>

#include "cion/common/peer_info.hh"
#include "cion/common/ipayload.hh"
#include "cion/security/security_info.hh"

namespace cion {
namespace channel {

class GroupChannel {
 public:
  explicit GroupChannel(std::string topic);
  explicit GroupChannel(std::string topic, SecurityInfo security);
  virtual ~GroupChannel();
  void Subscribe();
  void Unsubscribe();
  void Publish(IPayload* data);

 protected:
  virtual void OnMessageReceived(std::shared_ptr<IPayload> data,
      std::shared_ptr<PeerInfo> peer, std::string topic_name) = 0;
  virtual void OnJoined(std::shared_ptr<PeerInfo> peer,
      std::string topic_name) = 0;

  virtual void OnLeft(std::shared_ptr<PeerInfo> peer,
      std::string topic_name) = 0;

  std::string GetServiceName();
  int GetChannelId();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace channel
}  // namespace cion

#endif  // CION_CHANNEL_GROUP_CHANNEL_HH_