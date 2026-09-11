/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd.
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

#ifndef CION_COMMON_UTIL_PROXY_HH_
#define CION_COMMON_UTIL_PROXY_HH_

#include <string>
#include <memory>

#include "cion_proxy.h"

namespace rpc = rpc_port::cion_proxy;
namespace rpc_proxy = rpc_port::cion_proxy::proxy;

namespace cion {

class Proxy {
 public:
  static Proxy& GetInst();
  std::string GetUUID();
  void SetName(std::string service_name, std::string display_name);
  std::string GetName(std::string service_name);
  void SetEnabled(std::string service_name, bool enabled);
  bool GetEnabled(std::string service_name);

 private:
  Proxy() = default;
  ~Proxy() = default;
  void Connect();

  std::unique_ptr<rpc_proxy::CionServer> proxy_;
};

}  // namespace cion

#endif  // CION_COMMON_UTIL_PROXY_HH_
