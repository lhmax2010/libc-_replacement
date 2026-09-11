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

#ifndef CION_CHANNEL_PAYLOAD_ASYNC_RESULT_MANAGER_HH_
#define CION_CHANNEL_PAYLOAD_ASYNC_RESULT_MANAGER_HH_

#include <vector>
#include <string>
#include <functional>
#include <list>
#include <memory>

#include "cion/channel/payload_async_result.hh"
#include "cion/common/peer_info.hh"


namespace cion {

class PayloadAsyncResultManager {
 public:
  class TimeoutInfo {
   public:
    TimeoutInfo(PayloadAsyncResultManager* manager,
        int async_result_id)
        : manager_(manager), async_result_id_(async_result_id) {}
    PayloadAsyncResultManager* manager_;
    int async_result_id_;
  };
  PayloadAsyncResultManager();
  void AddAsyncResultToWaitList(
      std::shared_ptr<PeerInfo> info, std::string payload_id,
      PayloadAsyncResult::ResultCode result,
      PayloadAsyncResult::PayloadAsyncResultCallback func);
  void UpdateAsyncResultCode(std::string peer_uuid,
      int peer_channel_id, std::string payload_id,
      PayloadAsyncResult::ResultCode code);
  bool IsValidPayload(std::string payload_id);
  void InvokeResultCallback(std::shared_ptr<PayloadAsyncResult> result);

 private:
  std::list<std::shared_ptr<PayloadAsyncResult>> result_list_;
};

}  // namespace cion

#endif  // CION_CHANNEL_PAYLOAD_ASYNC_RESULT_MANAGER_HH_
