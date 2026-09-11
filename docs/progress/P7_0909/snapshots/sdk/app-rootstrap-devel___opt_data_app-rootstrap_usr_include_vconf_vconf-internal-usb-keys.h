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

#ifndef __VCONF_INTERNAL_USB_KEYS_H__
#define __VCONF_INTERNAL_USB_KEYS_H__
/**
 * Current usb gadget mode, combination of usb functions,
 * e.g., USB_GADGET_FUNC_SDB | USB_GADGET_FUNC_ACM. Zero if usb is disconnected.
 * The enum, USB_GADGET_FUNC_XXX, is defined in capi-system-device package.
 */
#define VCONFKEY_USB_CUR_MODE  "memory/usb/cur_mode"

/**
 * Configured usb gadget mode, not affected by connection status(connected or disconnected).
 * It comprises usb-gadget functions, e.g., USB_GADGET_FUNC_SDB | USB_GADGET_FUNC_ACM.
 * The enum, USB_GADGET_FUNC_XXX, is defined in capi-system-device package.
 * The default value is 0xb, which means 0x01(SDB) | 0x02(MTP) | 0x08(ACM). It might be modified by user setting
 * such as debugging mode or usb-tethering mode.
 */
#define VCONFKEY_USB_SEL_MODE  "db/usb/sel_mode"

#endif /*   __VCONF_INTERNAL_USB_KEYS_H__    */
