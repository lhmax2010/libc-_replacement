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

#ifndef CION_COMMON_UTIL_FACTORY_MANAGER_HH_
#define CION_COMMON_UTIL_FACTORY_MANAGER_HH_

#include <vector>
#include <memory>

#include "cion/common/ipayload.hh"

namespace cion {

class FactoryManager {
 public:
  static FactoryManager& GetInst();
  std::shared_ptr<IPayload> CreatePayload(const std::vector<uint8_t>& payload);
  std::shared_ptr<IPayload> CreatePayload(IPayload::PayloadType type);

 private:
  FactoryManager();
  ~FactoryManager();
  FactoryManager(const FactoryManager&);
  const FactoryManager &operator =(const FactoryManager &);
};

}  // namespace cion

#endif  // CION_COMMON_UTIL_FACTORY_MANAGER_HH_
