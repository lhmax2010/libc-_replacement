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

#ifndef CION_CHANNEL_CLIENT_CHANNEL_HH_
#define CION_CHANNEL_CLIENT_CHANNEL_HH_

#include <string>
#include <memory>
#include <vector>

#include "cion/security/security_info.hh"
#include "cion/common/peer_info.hh"
#include "cion/common/ipayload.hh"
#include "cion/channel/connection_result.hh"
#include "cion/channel/payload_async_result.hh"
#include "cion/channel/ipayload_receiver.hh"

namespace cion {
namespace channel {

class ClientChannel : public IPayloadReceiver {
 public:
  explicit ClientChannel(std::string service_name);
  explicit ClientChannel(std::string service_name, SecurityInfo security);
  virtual ~ClientChannel();
  void TryDiscovery();
  void StopDiscovery();
  void TryConnect(std::shared_ptr<PeerInfo> peer);
  void Disconnect();
  std::vector<char> SendData(std::vector<char> data, int timeout);
  void SendPayloadAsync(IPayload* data,
      PayloadAsyncResult::PayloadAsyncResultCallback func);
  std::shared_ptr<PeerInfo> GetPeerInfo();

 protected:
  virtual void OnPayloadReceived(std::shared_ptr<IPayload> data,
      std::shared_ptr<PeerInfo> peer,
      PayloadTransferStatus status) = 0;
  virtual void OnConnectionResult(std::shared_ptr<PeerInfo> info,
      const ConnectionResult& result) = 0;
  virtual void OnDiscovered(std::shared_ptr<PeerInfo> peer) = 0;
  virtual void OnDisconnected(std::shared_ptr<PeerInfo> peer) = 0;

  std::string GetServiceName();
  int GetChannelId();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace channel
}  // namespace cion

#endif  // CION_CHANNEL_CLIENT_CHANNEL_HH_
