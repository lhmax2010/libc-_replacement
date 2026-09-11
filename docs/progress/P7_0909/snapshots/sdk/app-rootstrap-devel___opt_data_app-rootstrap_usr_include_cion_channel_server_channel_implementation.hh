
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

#ifndef CION_CHANNEL_SERVER_CHANNEL_IMPLEMENTATION_HH_
#define CION_CHANNEL_SERVER_CHANNEL_IMPLEMENTATION_HH_

#include <string>
#include <vector>
#include <list>
#include <memory>

#include <shared-queue.hpp>

#include "cion/channel/idle_invoker.hh"
#include "cion/channel/server_channel.hh"
#include "cion/channel/channel_job.hh"
#include "cion/common/cion_cmd.hh"
#include "cion/security/security_info.hh"
#include "cion/vine/vine_session.hh"
#include "cion/vine/vine_server_dp.hh"
#include "cion/vine/vine_service.hh"
#include "cion/vine/vine_interfaces.hh"
#include "cion/channel/payload_async_result_manager.hh"
#include "cion/channel/payload_manager.hh"

namespace cion {
namespace channel {

class ServerChannel::Impl : public IVineDpOpenedEventHandler,
    IVineDpReceivedEventHandler, IVineDpTerminatedEventHandler,
    IVineDpAcceptedEventHandler {
 public:
  Impl(ServerChannel* parent, std::string service_name,
      std::string display_name, SecurityInfo security);
  virtual ~Impl();

  void Listen();
  void Stop();
  void Disconnect(std::shared_ptr<PeerInfo> peer);
  void Accept(std::shared_ptr<PeerInfo> peer_info);
  void Reject(std::shared_ptr<PeerInfo> peer_info, const std::string& reason);
  void SendPayloadAsync(IPayload* data, std::shared_ptr<PeerInfo> peer_info,
      PayloadAsyncResult::PayloadAsyncResultCallback func);
  void OnDpOpened(VineDpPtr dp, int result) override;
  void OnDpReceived(VineDpPtr dp,
      const std::vector<unsigned char>& data) override;
  void OnDpTerminated(VineDpPtr dp) override;
  void OnDpAccepted(VineDpPtr dp, VineDpPtr accepted_dp) override;

 private:
  friend class ServerChannel;
  void Send(cmd::CionCmd cmd,
    std::shared_ptr<std::list<std::shared_ptr<VineDp>>>,
    IPayload* data);
  void Send(cmd::CionCmd cmd, VineDpPtr client_dp,
      const std::vector<uint8_t>& raw);

  ServerChannel* parent_;
  std::string service_name_;
  std::string display_name_;
  SecurityInfo security_;
  int channel_id_;
  std::list<std::shared_ptr<PeerInfo>> peerlist_;
  std::map<std::shared_ptr<PeerInfo>, VineDpPtr> dp_map_;
  std::map<std::shared_ptr<PeerInfo>, VineDpPtr> sync_dp_map_;
  std::shared_ptr<PeerInfo> my_peer_info_;
  std::shared_ptr<VineSession> session_ = nullptr;
  std::shared_ptr<VineServerDp> server_dp_ = nullptr;
  std::shared_ptr<VineService> service_ = nullptr;
  std::list<VineDpPtr> accepted_dp_list_;
  std::shared_ptr<PayloadAsyncResultManager> async_result_manager_ = nullptr;
  bool is_listening_;
  IdleInvoker idle_invoker_;
  tizen_base::SharedQueue<int> dp_open_queue_;
};

}  // namespace channel
}  // namespace cion

#endif  // CION_CHANNEL_SERVER_CHANNEL_IMPLEMENTATION_HH_
