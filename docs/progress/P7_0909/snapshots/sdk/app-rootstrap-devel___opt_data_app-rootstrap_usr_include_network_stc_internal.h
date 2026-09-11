/*
 * Smart Traffic Control (STC)
 *
 * Copyright (c) 2017 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
 */

#ifndef __TIZEN_STC_INTERNAL_H__
#define __TIZEN_STC_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file stc_internal.h
 */

/*****************************************************************************
 *  Standard headers
 *****************************************************************************/
#include <tizen.h>
#include <time.h>
#include <inttypes.h>

/*****************************************************************************
 *  Smart Traffic Control (STC) library headers
 *****************************************************************************/
#include "stc_mgr_internal.h"
#include "stc_stat_internal.h"
#include "stc_rstn_internal.h"
#include "stc_fw_internal.h"
#include "stc_ipt_internal.h"
#include "stc_pcap_internal.h"

/*****************************************************************************
 *  Macros and Typedefs
 *****************************************************************************/

/*****************************************************************************
 *  Enumeration
 *****************************************************************************/

/*****************************************************************************
 *  Callback functions
 *****************************************************************************/

/*****************************************************************************
 *  Core API functions
 *****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_STC_INTERNAL_H__ */
