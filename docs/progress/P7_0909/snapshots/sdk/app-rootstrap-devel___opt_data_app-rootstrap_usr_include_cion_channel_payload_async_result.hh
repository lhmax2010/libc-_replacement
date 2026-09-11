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

#ifndef CION_CHANNEL_PAYLOAD_ASYNC_RESULT_HH_
#define CION_CHANNEL_PAYLOAD_ASYNC_RESULT_HH_

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include <parcel/parcelable.hh>
#include <parcel/parcel.hh>

#include "cion/common/peer_info.hh"

namespace cion {

class PayloadAsyncResult : private tizen_base::Parcelable {
 public:
  enum class ResultCode {
    Pending,
    Success,
    Fail
  };

  PayloadAsyncResult& operator=(const PayloadAsyncResult& other) = delete;
  bool operator!=(const PayloadAsyncResult& other);
  bool operator==(const PayloadAsyncResult& other);

  typedef std::function<void (std::shared_ptr<PayloadAsyncResult> result)>
      PayloadAsyncResultCallback;
  PayloadAsyncResult(std::shared_ptr<PeerInfo> info,
      std::string payload_id, ResultCode result,
      PayloadAsyncResultCallback func);
  PayloadAsyncResult(std::string peer_uuid, int channel_id,
      std::string payload_id, ResultCode result);
  explicit PayloadAsyncResult(std::vector<uint8_t> result);

  std::vector<uint8_t> Serialize();
  void Deserialize(std::vector<uint8_t> result);
  unsigned int GetID();
  std::shared_ptr<PeerInfo> GetPeerInfo();
  void SetPeerInfo(std::shared_ptr<PeerInfo> peer);
  std::string GetPeerUUID();
  int GetChannelID();
  std::string GetPayloadID();
  ResultCode GetResultCode();
  PayloadAsyncResultCallback GetResultCallback();
  void UpdateResult(ResultCode result);

 private:
  void WriteToParcel(tizen_base::Parcel* parcel) const override;
  void ReadFromParcel(tizen_base::Parcel* parcel) override;
  std::shared_ptr<PeerInfo> peer_info_;
  std::string peer_uuid_;
  int channel_id_;
  std::string payload_id_;
  ResultCode result_;
  PayloadAsyncResultCallback func_;
  unsigned int id_;
};

}  // namespace cion

#endif  // CION_CHANNEL_PAYLOAD_ASYNC_RESULT_HH_
