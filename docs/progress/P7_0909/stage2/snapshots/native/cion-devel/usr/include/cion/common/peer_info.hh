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

#ifndef CION_COMMON_PEER_INFO_HH_
#define CION_COMMON_PEER_INFO_HH_

#include <parcel/parcel.hh>
#include <parcel/parcelable.hh>

#include <memory>
#include <string>
#include <vector>


namespace cion {

namespace channel {
class ServerChannel;
}

class PeerInfo : private tizen_base::Parcelable {
 public:
  PeerInfo();
  explicit PeerInfo(const void* buf, uint32_t size);
  PeerInfo(const PeerInfo& peerinfo);
  ~PeerInfo();

  bool operator==(PeerInfo& peerinfo);
  bool operator!=(PeerInfo& perrinfo);

  std::string GetDeviceID() const;
  std::string GetDeviceName() const;
  std::string GetDevicePlatform() const;
  std::string GetDevicePlatformVersion() const;
  std::string GetDeviceType() const;
  std::string GetAppID() const;
  std::string GetAppVersion() const;
  std::string GetUUID() const;
  std::string GetDisplayName() const;
  int GetChannelID() const;

  bool IsValid() const noexcept;
  std::vector<uint8_t> Serialize() const;
  void SetChannelID(int id);

 private:
  friend class channel::ServerChannel;

  void WriteToParcel(tizen_base::Parcel* parcel) const override;
  void ReadFromParcel(tizen_base::Parcel* parcel) override;
  void SetDisplayName(std::string display_name);

  bool valid_;
  std::string device_id_;
  std::string device_name_;
  std::string device_platform_;
  std::string device_platform_version_;
  std::string device_type_;
  std::string app_id_;
  std::string app_version_;
  std::string uuid_;
  std::string display_name_;
  int channel_id_;
};

}  // namespace cion

#endif  // CION_COMMON_PEER_INFO_HH_
