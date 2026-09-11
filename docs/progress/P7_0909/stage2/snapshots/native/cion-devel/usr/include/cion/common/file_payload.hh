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

#ifndef CION_COMMON_FILE_PAYLOAD_HH_
#define CION_COMMON_FILE_PAYLOAD_HH_

#include <vector>
#include <string>
#include <parcel/parcel.hh>
#include <parcel/parcelable.hh>

#include "cion/common/ipayload.hh"


namespace cion {

class FilePayloadReaderInternal;

class FilePayload : public IPayload, private tizen_base::Parcelable{
 public:
  void SaveAsFile(std::string path);
  std::string GetFileName();
  uint64_t GetReceivedBytes();
  uint64_t GetTotalBytes();
  void SetFilePath(std::string path);
  FilePayload();
  std::string& GetID() override;
  IPayload::PayloadType GetType() override;
  std::vector<uint8_t> Serialize() override;
  void Deserialize(std::vector<uint8_t> payload) override;

 private:
  friend class FilePayloadReaderInternal;
  std::string& GetFilePath();
  std::string& GetTemporaryFileName();
  void UpdateTemporaryFilePath();
  void SetReceivedBytes(uint64_t size);
  void SetTemporaryFilePath(std::string path);
  void WriteToParcel(tizen_base::Parcel* parcel) const override;
  void ReadFromParcel(tizen_base::Parcel* parcel) override;
  std::string path_;
  std::string tmp_file_name_;
  std::string tmp_path_;
  uint64_t received_bytes_ = 0;
  uint64_t total_bytes_ = 0;
  std::string id_;
};

} // namespace cion

#endif  // CION_COMMON_FILE_PAYLOAD_HH_
