
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

#ifndef CION_CHANNEL_CLIENT_CHANNEL_IMPLEMENTATION_HH_
#define CION_CHANNEL_CLIENT_CHANNEL_IMPLEMENTATION_HH_

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <queue>
#include <map>

#include <shared-queue.hpp>

#include "cion/channel/client_channel.hh"
#include "cion/channel/channel_job.hh"
#include "cion/channel/idle_invoker.hh"
#include "cion/common/cion_cmd.hh"
#include "cion/security/security_info.hh"
#include "cion/channel/payload_async_result_manager.hh"
#include "cion/channel/payload_manager.hh"

#include "cion/vine/vine_client_dp.hh"
#include "cion/vine/vine_session.hh"
#include "cion/vine/vine_discoverer.hh"
#include "cion/vine/vine_interfaces.hh"

namespace cion {
namespace channel {

class ClientChannel::Impl : public IVineServiceBrowser, IVineDpOpenedEventHandler,
    IVineDpReceivedEventHandler, IVineDpTerminatedEventHandler {
 public:
  Impl(ClientChannel* parent, std::string service_name, SecurityInfo security);
  virtual ~Impl();
  void SendPeerInfo(PeerInfo* peerinfo);
  void Send(cmd::CionCmd cmd, IPayload* data);
  void Send(cmd::CionCmd cmd, const std::vector<uint8_t>& raw,
      VineDpPtr dp = nullptr);
  std::vector<char> SendData(std::vector<char> data, int timeout);
  void SendPayloadAsync(IPayload* data,
      PayloadAsyncResult::PayloadAsyncResultCallback func);
  std::shared_ptr<VineClientDp> PopDp(VineDpPtr dp);
  void Disconnect(VineDpPtr dp);
  std::shared_ptr<VineClientDp> GetDp(std::string ip, int port);
  std::shared_ptr<VineClientDp> GetDp(std::shared_ptr<VineDp> dp);
  void TrySyncReply(std::vector<unsigned char> data);
  void TryDiscovery();
  void StopDiscovery();
  void TryConnect(std::shared_ptr<PeerInfo> peer);
  void Disconnect();

  void OnServiceDiscovered(VineService service, std::string ip) override;
  void OnDpOpened(VineDpPtr dp, int result) override;
  void OnDpReceived(VineDpPtr dp,
      const std::vector<unsigned char>& data) override;
  void OnDpTerminated(VineDpPtr dp) override;

 private:
  friend class ClientChannel;
  ClientChannel* parent_;
  std::string service_name_;
  int channel_id_;
  std::shared_ptr<PeerInfo> my_peer_info_;
  std::shared_ptr<PeerInfo> connected_peer_;
  std::list<std::shared_ptr<VineClientDp>> dp_list_;
  std::map<std::shared_ptr<PeerInfo>, std::shared_ptr<VineClientDp>> dp_map_;
  std::shared_ptr<VineClientDp> dp_;
  std::shared_ptr<VineClientDp> sync_dp_;
  std::shared_ptr<VineSession> session_;
  bool is_discovering_ = false;
  tizen_base::SharedQueue<std::vector<char>> reply_queue_;
  std::shared_ptr<VineDiscoverer> discoverer_ = nullptr;
  std::shared_ptr<PayloadAsyncResultManager> async_result_manager_ = nullptr;
  std::map<std::string, std::shared_ptr<PeerInfo>> ondemand_list_;
  bool is_timeover_ = false;
  bool is_connected_ = false;
  unsigned int sequence_id_ = 0;
  mutable std::mutex mutex_;
  SecurityInfo security_;
  IdleInvoker idle_invoker_;
};

}  // namespace channel
}  // namespace cion

#endif  // CION_CHANNEL_CLIENT_CHANNEL_IMPLEMENTATION_HH_
