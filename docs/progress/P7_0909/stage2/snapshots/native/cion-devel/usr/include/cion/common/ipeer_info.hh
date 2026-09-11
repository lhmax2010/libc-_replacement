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

#ifndef CION_COMMON_IPEER_INFO_HH_
#define CION_COMMON_IPEER_INFO_HH_

#include <string>
#include <vector>

#ifndef EXPORT_API
#define EXPORT_API __attribute__((visibility("default")))
#endif

namespace cion {

class EXPORT_API IPeerInfo {
 public:
  virtual ~IPeerInfo() = default;
  virtual std::string GetDeviceID() const = 0;
  virtual std::string GetDeviceName() const = 0;
  virtual std::string GetDevicePlatform() const = 0;
  virtual std::string GetDevicePlatformVersion() const = 0;
  virtual std::string GetDeviceType() const = 0;
  virtual std::string GetAppID() const = 0;
  virtual std::string GetAppVersion() const = 0;
  virtual std::string GetUUID() const = 0;
  virtual int GetChannelID() const = 0;
  virtual bool IsValid() const noexcept = 0;
  virtual std::vector<uint8_t> Serialize() const = 0;
  virtual void SetChannelID(int id) = 0;
};

}  // namespace cion

#endif  // CION_COMMON_IPEER_INFO_HH_
