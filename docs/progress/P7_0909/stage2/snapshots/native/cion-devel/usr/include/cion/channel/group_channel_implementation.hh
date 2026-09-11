
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

#ifndef CION_CHANNEL_GROUP_CHANNEL_IMPLEMENTATION_HH_
#define CION_CHANNEL_GROUP_CHANNEL_IMPLEMENTATION_HH_

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "cion/channel/group_channel.hh"
#include "cion/channel/channel_job.hh"
#include "cion/channel/idle_invoker.hh"
#include "cion/common/ipayload.hh"
#include "cion/security/security_info.hh"
#include "cion/vine/vine_dp.hh"
#include "cion/vine/vine_interfaces.hh"
#include "cion/vine/vine_pubsub_dp.hh"
#include "cion/vine/vine_session.hh"

namespace cion {
namespace channel {

class GroupChannel::Impl : private IVineDpTerminatedEventHandler,
    IVineDpOpenedEventHandler, IVineDpReceivedEventHandler,
    IVinePubSubJoinedEventHandler, IVinePubSubleftEventHandler {
 public:
  Impl(GroupChannel* parent, std::string topic, SecurityInfo security);
  virtual ~Impl();

 private:
  friend class GroupChannel;
  void OnDpTerminated(VineDpPtr dp) override;
  void OnDpOpened(VineDpPtr dp, int result) override;
  void OnDpReceived(VineDpPtr dp,
      const std::vector<unsigned char>& data) override;
  void OnDpJoined(VineDpPtr dp, std::string peer_id) override;
  void OnDpLeft(VineDpPtr dp, std::string peer_id) override;
  void Subscribe();
  void Unsubscribe();
  void Publish(IPayload* data);
  void AddJoinedPeerInfo(std::string ip, std::shared_ptr<PeerInfo> peer_info);
  void RemoveJoinedPeerInfo(std::string ip);
  void ClearJoinedPeerInfo();
  std::shared_ptr<PeerInfo> GetJoinedPeerInfo(std::string ip);

  GroupChannel* parent_;
  std::string topic_;
  int channel_id_;
  bool is_online_;
  std::shared_ptr<PeerInfo> my_peer_info_;
  std::shared_ptr<VineSession> session_;
  std::shared_ptr<VinePubSubDp> dp_;
  SecurityInfo security_;
  std::unordered_map<std::string, std::shared_ptr<PeerInfo>> joined_peerinfo_map_;
  IdleInvoker idle_invoker_;
};

}  // namespace channel
}  // namespace cion

#endif  // CION_CHANNEL_GROUP_CHANNEL_IMPLEMENTATION_HH_