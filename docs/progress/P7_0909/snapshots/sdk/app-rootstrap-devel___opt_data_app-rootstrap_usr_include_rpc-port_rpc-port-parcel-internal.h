/*
 * Copyright (c) 2017 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_RPC_PORT_PARCEL_INTERNAL_INCLUDE_H__
#define __TIZEN_APPFW_RPC_PORT_PARCEL_INTERNAL_INCLUDE_H__

#include <rpc-port-parcel.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  RPC_PORT_PARCEL_HEADER_FLAG_MASK_NEW_HEADER = 0b00000001,
  RPC_PORT_PARCEL_HEADER_FLAG_MASK_PROXY = 0b00000010,
  RPC_PORT_PARCEL_HEADER_FLAG_MASK_LAN = 0b00011100,
  RPC_PORT_PARCEL_HEADER_FLAG_MASK_RESERVED = 0b11100000,
} rpc_port_parcel_header_flag_mask_e;

typedef enum {
  RPC_PORT_PARCEL_HEADER_UNKNOWN = 0b00000000,
  RPC_PORT_PARCEL_HEADER_IS_FROM_PROXY =
      RPC_PORT_PARCEL_HEADER_FLAG_MASK_PROXY |
      RPC_PORT_PARCEL_HEADER_FLAG_MASK_NEW_HEADER,
  RPC_PORT_PARCEL_HEADER_LAN_C =
      (RPC_PORT_PARCEL_HEADER_FLAG_MASK_LAN & 0b00000100) |
      RPC_PORT_PARCEL_HEADER_FLAG_MASK_NEW_HEADER,
  RPC_PORT_PARCEL_HEADER_LAN_CPP =
      (RPC_PORT_PARCEL_HEADER_FLAG_MASK_LAN & 0b00001000) |
      RPC_PORT_PARCEL_HEADER_FLAG_MASK_NEW_HEADER,
  RPC_PORT_PARCEL_HEADER_LAN_CS =
      (RPC_PORT_PARCEL_HEADER_FLAG_MASK_LAN & 0b00001100) |
      RPC_PORT_PARCEL_HEADER_FLAG_MASK_NEW_HEADER,
  RPC_PORT_PARCEL_HEADER_LAN_RUST =
      (RPC_PORT_PARCEL_HEADER_FLAG_MASK_LAN & 0b00010000) |
      RPC_PORT_PARCEL_HEADER_FLAG_MASK_NEW_HEADER,
  RPC_PORT_PARCEL_HEADER_LAN_DART =
      (RPC_PORT_PARCEL_HEADER_FLAG_MASK_LAN & 0b00010100) |
      RPC_PORT_PARCEL_HEADER_FLAG_MASK_NEW_HEADER,
} rpc_port_parcel_header_flags_e;

int rpc_port_parcel_reset_reader(rpc_port_parcel_h h);

int rpc_port_parcel_to_array(rpc_port_parcel_h h, void** array,
                             unsigned int* size);

int rpc_port_parcel_from_array(rpc_port_parcel_h h, const void* array,
                               unsigned int size);

int rpc_port_parcel_create_from_raw_without_header(rpc_port_parcel_h* h,
                                                   const void* raw,
                                                   unsigned int size);
int rpc_port_parcel_create_from_port_without_header(rpc_port_parcel_h* h,
                                                    rpc_port_h port);
int rpc_port_parcel_reserve(rpc_port_parcel_h h, unsigned int size);

int rpc_port_parcel_create_from_parcel(rpc_port_parcel_h* h,
                                       rpc_port_parcel_h origin_parcel,
                                       unsigned int start_pos,
                                       unsigned int size);
int rpc_port_parcel_set_data_size(rpc_port_parcel_h h, unsigned int size);
int rpc_port_parcel_get_data_size(rpc_port_parcel_h h, unsigned int* size);
int rpc_port_parcel_pin(rpc_port_parcel_h h);
int rpc_port_parcel_get_reader(rpc_port_parcel_h h, unsigned int* reader_pos);
int rpc_port_parcel_set_reader(rpc_port_parcel_h h, unsigned int reader_pos);
int rpc_port_parcel_read_fd(rpc_port_parcel_h h, int* fd);
int rpc_port_parcel_write_fd(rpc_port_parcel_h h, int fd);
int rpc_port_parcel_clone(rpc_port_parcel_h* h, rpc_port_parcel_h origin);
int rpc_port_parcel_get_data_ptr(rpc_port_parcel_h h, unsigned char** ptr);

int rpc_port_parcel_header_set_tag_ex(rpc_port_parcel_header_h header,
                                      unsigned char tidlc_version_major,
                                      unsigned char tidlc_version_minor,
                                      unsigned char tidlc_version_patch,
                                      unsigned char tidl_protocol_ver,
                                      unsigned char flags);
int rpc_port_parcel_header_get_tag_ex(rpc_port_parcel_header_h header,
                                      unsigned char* tidlc_version_major,
                                      unsigned char* tidlc_version_minor,
                                      unsigned char* tidlc_version_patch,
                                      unsigned char* tidl_protocol_ver,
                                      unsigned char* flags);
int rpc_port_parcel_create_with_capacity(rpc_port_parcel_h* h, size_t capacity);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_RPC_PORT_PARCEL_INTERNAL_INCLUDE_H__ */
