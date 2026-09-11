/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CION_CHANNEL_CONNECTION_RESULT_HH_
#define CION_CHANNEL_CONNECTION_RESULT_HH_

#include<string>

namespace cion {
class ConnectionResult {
 public:
  enum ConnectionStatus {
    OK,
    Rejected,
    Error
  };

  explicit ConnectionResult(ConnectionStatus status, std::string reason = "") {
    status_ = status;
    reason_ = reason;
  }

  std::string GetReason() const {
    return reason_;
  }

  ConnectionStatus GetStatus() const {
    return status_;
  }

 private:
  ConnectionStatus status_;
  std::string reason_;
};

}   // namespace cion

#endif  // CION_CHANNEL_CONNECTION_RESULT_HH_
