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

#ifndef _SECURITY_INFO_H
#define _SECURITY_INFO_H

#include <string>

namespace cion {

class SecurityInfo {
 public:
  SecurityInfo() = default;
  SecurityInfo(const SecurityInfo& other);
  SecurityInfo(SecurityInfo&& other) noexcept;
  SecurityInfo& operator=(SecurityInfo&& other) noexcept;
  void SetCaPath(std::string path);
  void SetCertPath(std::string path);
  void SetPrivateKeyPath(std::string path);
  std::string GetCaPath() const;
  std::string GetCertPath() const;
  std::string GetPrivateKeyPath() const;
  bool IsEmpty();

 private:
  std::string ca_path_;
  std::string cert_path_;
  std::string private_key_path_;
};

} //namespace cion


#endif //_SECURITY_INFO_H
