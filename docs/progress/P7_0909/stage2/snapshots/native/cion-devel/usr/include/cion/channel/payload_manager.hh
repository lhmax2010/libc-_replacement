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

#ifndef CION_CHANNEL_PAYLOAD_MANAGER_HH_
#define CION_CHANNEL_PAYLOAD_MANAGER_HH_

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "cion/channel/ipayload_receiver.hh"
#include "cion/channel/payload_async_result.hh"
#include "cion/common/peer_info.hh"
#include "cion/common/ipayload.hh"
#include "cion/vine/vine_dp.hh"

namespace cion {

class PayloadManager {
 public:
  explicit PayloadManager(IPayloadReceiver* receiver) : receiver_(receiver) {}
  virtual ~PayloadManager();

  void Process(std::shared_ptr<tizen_base::Parcel> parcel,
    std::shared_ptr<VineDp> peer_dp,
    std::shared_ptr<PeerInfo> my_peer_info,
    std::shared_ptr<PeerInfo> sender_peer_info,
    bool with_reply);
  void ProcessFile(std::shared_ptr<IPayload> payload,
    std::shared_ptr<VineDp> peer_dp,
    std::shared_ptr<PeerInfo> my_peer_info,
    std::shared_ptr<PeerInfo> sender_peer_info,
    std::shared_ptr<tizen_base::Parcel> parcel,
    bool with_reply);
  void ProcessData(std::shared_ptr<IPayload> payload,
    std::shared_ptr<VineDp> peer_dp,
    std::shared_ptr<PeerInfo> my_peer_info,
    std::shared_ptr<PeerInfo> sender_peer_info,
    bool with_reply);
  bool IsExist(const std::string &path);

 private:
  void SendResult(std::shared_ptr<VineDp> dp,
    std::shared_ptr<PeerInfo> peer,
    std::string payload_id,
    PayloadAsyncResult::ResultCode result);
  void ResultHanling(std::shared_ptr<IPayload> payload,
    std::shared_ptr<VineDp> peer_dp,
    std::shared_ptr<PeerInfo> my_peer_info,
    std::shared_ptr<PeerInfo> sender_peer_info,
    IPayloadReceiver::PayloadTransferStatus status,
    bool with_reply);

 private:
  IPayloadReceiver* receiver_;
};

}  // namespace cion

#endif  // CION_CHANNEL_PAYLOAD_MANAGER_HH_
