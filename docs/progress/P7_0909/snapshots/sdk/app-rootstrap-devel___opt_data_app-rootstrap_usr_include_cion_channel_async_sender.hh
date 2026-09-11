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

#ifndef CION_CHANNEL_ASYNC_SENDER_HH_
#define CION_CHANNEL_ASYNC_SENDER_HH_

#include <thread>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <string>

#include <aul.h>
#include <sys/stat.h>
#include <vine.h>
#include "cion/common/file_payload.hh"
#include "cion/common/exception.hh"
#include "cion/common/cion_cmd.hh"
#include "cion/common/util/logging.hh"
#include "cion/vine/vine_dp.hh"
#include "cion/common/file_payload_reader_internal.hh"
#include "cion/channel/async_sender.hh"
#include "cion/channel/payload_async_result.hh"

namespace {

constexpr int kVineDpMaxBufferSize = 122880; /* 120 * 1024 byte */

}  // namespace

namespace cion {

class AsyncSender {
 private:
  void SendData(cmd::CionCmd cmd,
      std::shared_ptr<std::list<std::shared_ptr<VineDp>>> dplist,
      IPayload* data) {
    tizen_base::Parcel parcel;
    parcel.WriteUInt32(cmd);
    if (data != nullptr) {
      std::vector<uint8_t> raw = data->Serialize();
      parcel.WriteUInt32(raw.size());
      parcel.Write(raw.data(), raw.size());
    }
    for (std::shared_ptr<VineDp> i : *dplist)
      i->SendDataAsync(parcel.GetData(), parcel.GetDataSize());

    LOG(INFO) << "send " << parcel.GetDataSize();
    return;
  }

  void SendFile(cmd::CionCmd cmd,
      std::shared_ptr<std::list<std::shared_ptr<VineDp>>> dplist,
      IPayload* data) {
    FilePayloadReaderInternal file_reader(data);
    std::string path = file_reader.GetFilePath();
    if (path.empty()) {
      LOG(ERROR) << "file path is empty";
      THROW(error::CION_ERROR_INVALID_PARAMETER);
    }
    file_reader.UpdateTemporaryFilePath();

    std::vector<uint8_t> raw = data->Serialize();
    std::ifstream check_file;

    check_file.open(path, std::ifstream::in | std::ifstream::binary);
    if (check_file.is_open() == false) {
      LOG(ERROR) << "sever send file open fail";
      THROW(error::CION_ERROR_INVALID_PARAMETER);
    }
    check_file.close();

    std::thread thd([cmd, dplist, raw, path]() {
      std::ifstream p_file;
      LOG(INFO) << "send file : " << path;
      p_file.open(path, std::ifstream::in | std::ifstream::binary);

      /* file read */
      int remaining_size;
      int send_size;
      char* buf = nullptr;

      p_file.seekg(0, p_file.end);
      remaining_size = p_file.tellg();
      p_file.seekg(0, p_file.beg);
      int cnt = 0;

      LOG(INFO) << "send file remain : " << remaining_size;

      while (remaining_size > 0) {
        cnt++;
        LOG(INFO) << "send file cnt : " << cnt;
        LOG(INFO) << "send file remain : " << remaining_size;

        if (remaining_size > kVineDpMaxBufferSize) {
          send_size = kVineDpMaxBufferSize;
          remaining_size = remaining_size - kVineDpMaxBufferSize;
        } else {
          send_size = remaining_size;
          remaining_size = 0;
        }

        LOG(INFO) << "send file open size " << send_size;
        if (buf == nullptr)
          buf = new char[send_size];

        p_file.read(buf, send_size);

        tizen_base::Parcel parcel;

        /* send - same with data but maybe changed for big file */
        parcel.WriteUInt32(cmd);
        parcel.WriteUInt32(raw.size());
        parcel.Write(raw.data(), raw.size());
        parcel.WriteUInt32(send_size);
        parcel.Write(buf, send_size);
        parcel.WriteUInt32(cnt);

        for (std::shared_ptr<VineDp> i : *dplist)
          i->SendDataAsync(parcel.GetData(), parcel.GetDataSize());
        LOG(INFO) << "file " << parcel.GetDataSize() <<" : " << send_size;
      }

      if (buf != nullptr)
        delete[] buf;

      p_file.close();
    });
    thd.detach();
  }

 public:
  void Send(cmd::CionCmd cmd,
      std::shared_ptr<std::list<std::shared_ptr<VineDp>>> dplist,
      IPayload* data) {
    if (data == nullptr) {
      SendData(cmd, dplist, nullptr);
      return;
    }

    IPayload::PayloadType type = data->GetType();
    if (type == IPayload::PayloadType::File)
      SendFile(cmd, dplist, data);
    else if (type == IPayload::PayloadType::Data)
      SendData(cmd, dplist, data);
  }

  void SendResult(std::shared_ptr<VineDp> dp, std::string uuid,
      int channel_id, std::string payload_id,
      PayloadAsyncResult::ResultCode code) {
    PayloadAsyncResult result(uuid, channel_id, payload_id, code);

    tizen_base::Parcel parcel;
    parcel.WriteUInt32(cmd::CionCmd::AsyncResult);
    std::vector<uint8_t> raw = result.Serialize();
    parcel.WriteUInt32(raw.size());
    parcel.Write(raw.data(), raw.size());

    dp->SendDataAsync(parcel.GetData(), parcel.GetDataSize());
    LOG(INFO) << "send result " << parcel.GetDataSize();
    return;
  }
};

}  // namespace cion

#endif  // CION_CHANNEL_ASYNC_SENDER_HH_
