/*
 * vconf-internal-keys
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __VCONF_INTERNAL_NFC_KEYS_H__
#define __VCONF_INTERNAL_NFC_KEYS_H__
/**
 *
 */
#define VCONFKEY_NFC_FEATURE  "db/nfc/feature"

enum {
	VCONFKEY_NFC_FEATURE_OFF = 0x00,
		VCONFKEY_NFC_FEATURE_ON
};

/**
 *
 */
#define VCONFKEY_NFC_STATE  "db/nfc/enable"

enum {
	VCONFKEY_NFC_STATE_OFF = 0,
		VCONFKEY_NFC_STATE_ON,
		VCONFKEY_NFC_STATE_MAX
};

/**
 *
 */
#define VCONFKEY_NFC_SE_TYPE  "db/nfc/se_type"

enum {
	VCONFKEY_NFC_SE_POLICY_NONE      = 0x00,
		VCONFKEY_NFC_SE_POLICY_UICC_ON  ,
		VCONFKEY_NFC_SE_POLICY_UICC_OFF ,
		VCONFKEY_NFC_SE_POLICY_ESE_ON   ,
		VCONFKEY_NFC_SE_POLICY_ESE_OFF  ,
		VCONFKEY_NFC_SE_POLICY_HCE_ON  ,
		VCONFKEY_NFC_SE_POLICY_HCE_OFF
};

/**
 *
 */
#define VCONFKEY_NFC_WALLET_MODE  "db/nfc/wallet_mode"

enum {
	VCONFKEY_NFC_WALLET_MODE_MANUAL     = 0x00,
		VCONFKEY_NFC_WALLET_MODE_AUTOMATIC ,
		VCONFKEY_NFC_WALLET_MODE_UICC ,
		VCONFKEY_NFC_WALLET_MODE_ESE ,
		VCONFKEY_NFC_WALLET_MODE_HCE
};

/**
 *
 */
#define VCONFKEY_NFC_STATE_OFF_BY_FLIGHT  "db/nfc/state_by_flight"

enum {
	VCONFKEY_NFC_NONE_BY_FLIGHT = 0x00,
		VCONFKEY_NFC_OFF_BY_FLIGHT
};

/**
 *
 */
#define VCONFKEY_NFC_PAYMENT_HANDLERS  "db/nfc/payment_handlers"

/**
 *
 */
#define VCONFKEY_NFC_OTHER_HANDLERS  "db/nfc/other_handlers"

/**
 *
 */
#define VCONFKEY_NFC_ESE_DISABLE  "db/nfc/ese_disable"

#endif /*   __VCONF_INTERNAL_NFC_KEYS_H__    */
