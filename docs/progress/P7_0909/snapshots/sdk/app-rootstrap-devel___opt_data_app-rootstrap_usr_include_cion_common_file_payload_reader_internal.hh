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

#ifndef CION_COMMON_IFILEPAYLOAD_INTERNAL_HH_
#define CION_COMMON_IFILEPAYLOAD_INTERNAL_HH_

#include <string>

#include "cion/common/ipayload.hh"
#include "cion/common/file_payload.hh"

namespace cion {

class FilePayloadReaderInternal {
 public:
  FilePayloadReaderInternal(IPayload* payload);
  std::string GetTemporaryFileName();
  std::string GetFilePath();
  void SetReceivedBytes(uint64_t size);
  void SetTemporaryFilePath(std::string path);
  void UpdateTemporaryFilePath();

 private:
  FilePayload* fp_;
};

}  // namespace cion

#endif  // CION_COMMON_IFILEPAYLOAD_INTERNAL_HH_
