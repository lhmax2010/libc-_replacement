
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

#ifndef CION_CHANNEL_SERVER_CHANNEL_HH_
#define CION_CHANNEL_SERVER_CHANNEL_HH_

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "cion/security/security_info.hh"
#include "cion/common/peer_info.hh"
#include "cion/channel/connection_result.hh"
#include "cion/common/ipayload.hh"
#include "cion/channel/payload_async_result.hh"
#include "cion/channel/ipayload_receiver.hh"

namespace cion {
namespace channel {

class ServerChannel : public IPayloadReceiver {
 public:
  explicit ServerChannel(std::string service_name, std::string display_name);
  explicit ServerChannel(std::string service_name, std::string display_name,
      SecurityInfo security);
  virtual ~ServerChannel();
  void Listen();
  void Stop();
  void Disconnect(std::shared_ptr<PeerInfo>);
  void Accept(std::shared_ptr<PeerInfo> peer);
  void Reject(std::shared_ptr<PeerInfo> peer, std::string reason = "");
  void SendPayloadAsync(IPayload* data,
        PayloadAsyncResult::PayloadAsyncResultCallback func);
  void SendPayloadAsync(IPayload* data, std::shared_ptr<PeerInfo> peer,
        PayloadAsyncResult::PayloadAsyncResultCallback func);
  std::list<std::shared_ptr<PeerInfo>> GetConnectedPeerList();
  std::string GetServiceName();
  std::string GetDisplayName();
  void SetDisplayName(std::string display_name);
  void SetOndemandLaunchEnable(bool enable);

 protected:
  virtual std::vector<char> OnDataReceived(const std::vector<char>& data,
      std::shared_ptr<PeerInfo> peer) = 0;
  virtual void OnPayloadReceived(std::shared_ptr<IPayload> data,
      std::shared_ptr<PeerInfo> peer,
      PayloadTransferStatus status) = 0;
  virtual void OnConnectionRequest(std::shared_ptr<PeerInfo> peer) = 0;
  virtual void OnConnectionResult(std::shared_ptr<PeerInfo> info,
      const ConnectionResult& result) = 0;
  virtual void OnDisconnected(std::shared_ptr<PeerInfo> peer) = 0;
  virtual void OnOndemandListRequested(std::shared_ptr<IPayload> data,
      std::shared_ptr<PeerInfo> peer) = 0;
  virtual int OnOndemandLaunchRequested(std::shared_ptr<IPayload> data,
      std::shared_ptr<PeerInfo> peer) = 0;

  int GetChannelId();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace channel
}  // namespace cion

#endif  // CION_CHANNEL_SERVER_CHANNEL_HH_
