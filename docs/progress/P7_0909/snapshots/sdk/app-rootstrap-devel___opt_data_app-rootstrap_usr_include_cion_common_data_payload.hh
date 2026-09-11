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

#ifndef CION_COMMON_DATA_PAYLOAD_HH_
#define CION_COMMON_DATA_PAYLOAD_HH_

#include <vector>
#include <string>
#include <parcel/parcel.hh>
#include <parcel/parcelable.hh>

#include "cion/common/ipayload.hh"

namespace cion {

class DataPayload : public IPayload,  private tizen_base::Parcelable {
 public:
  std::vector<char> GetData();
  void SetData(std::vector<char> data);
  std::string& GetID() override;
  IPayload::PayloadType GetType() override;
  std::vector<uint8_t> Serialize() override;
  void Deserialize(std::vector<uint8_t> payload) override;
  DataPayload();

 private:
  void WriteToParcel(tizen_base::Parcel* parcel) const override;
  void ReadFromParcel(tizen_base::Parcel* parcel) override;
  std::vector<char> data_;
  std::string id_;
};

}  // namespace cion

#endif  // CION_COMMON_DATA_PAYLOAD_HH_
