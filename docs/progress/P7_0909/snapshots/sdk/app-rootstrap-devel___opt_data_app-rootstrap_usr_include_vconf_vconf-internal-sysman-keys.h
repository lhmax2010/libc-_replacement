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

#ifndef __VCONF_INTERNAL_SYSMAN_KEYS_H__
#define __VCONF_INTERNAL_SYSMAN_KEYS_H__
/**
 *
 */
#define VCONFKEY_SYSMAN_ADDED_USB_STORAGE  "memory/sysman/added_usb_storage"

/**
 *
 */
#define VCONFKEY_SYSMAN_REMOVED_USB_STORAGE  "memory/sysman/removed_usb_storage"

/**
 * public vconf key (runtime-info)
 */
#define VCONFKEY_SYSMAN_CHARGER_STATUS  "memory/sysman/charger_status"

enum {
	VCONFKEY_SYSMAN_CHARGER_DISCONNECTED = 0,
		VCONFKEY_SYSMAN_CHARGER_CONNECTED
};

/**
 * public vconf key (device)
 */
#define VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW  "memory/sysman/charge_now"

/**
 * public vconf key (device)
 */
#define VCONFKEY_SYSMAN_BATTERY_STATUS_LOW  "memory/sysman/battery_status_low"

enum {
	VCONFKEY_SYSMAN_BAT_POWER_OFF = 1,
		VCONFKEY_SYSMAN_BAT_CRITICAL_LOW,
		VCONFKEY_SYSMAN_BAT_WARNING_LOW,
		VCONFKEY_SYSMAN_BAT_NORMAL,
		VCONFKEY_SYSMAN_BAT_FULL,
		VCONFKEY_SYSMAN_BAT_REAL_POWER_OFF
};

/**
 * public vconf key (device)
 */
#define VCONFKEY_SYSMAN_BATTERY_CAPACITY  "memory/sysman/battery_capacity"

/**
 * public vconf key (device)
 */
#define VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS  "memory/sysman/battery_level_status"

enum {
	VCONFKEY_SYSMAN_BAT_LEVEL_EMPTY    = 0,
		VCONFKEY_SYSMAN_BAT_LEVEL_CRITICAL,
		VCONFKEY_SYSMAN_BAT_LEVEL_LOW     ,
		VCONFKEY_SYSMAN_BAT_LEVEL_HIGH    ,
		VCONFKEY_SYSMAN_BAT_LEVEL_FULL
};

/**
 *
 */
#define VCONFKEY_SYSMAN_USB_STATUS  "memory/sysman/usb_status"

enum {
	VCONFKEY_SYSMAN_USB_DISCONNECTED = 0,
		VCONFKEY_SYSMAN_USB_CONNECTED,
		VCONFKEY_SYSMAN_USB_AVAILABLE
};

/**
 *
 */
#define VCONFKEY_SYSMAN_USB_HOST_STATUS  "memory/sysman/usbhost_status"

enum {
	VCONFKEY_SYSMAN_USB_HOST_DISCONNECTED = 0,
		VCONFKEY_SYSMAN_USB_HOST_CONNECTED,
		VCONFKEY_SYSMAN_USB_HOST_OVERCURRENT
};

/**
 * public vconf key (runtime-info)
 */
#define VCONFKEY_SYSMAN_EARJACK  "memory/sysman/earjack"

enum {
	VCONFKEY_SYSMAN_EARJACK_REMOVED = 0,
		VCONFKEY_SYSMAN_EARJACK_3WIRE = 0x1,
		VCONFKEY_SYSMAN_EARJACK_4WIRE = 0x3,
		VCONFKEY_SYSMAN_EARJACK_TVOUT = 0x10
};

/**
 *
 */
#define VCONFKEY_SYSMAN_EARJACKKEY  "memory/sysman/earjack_key"

/**
 *
 */
#define VCONFKEY_SYSMAN_CRADLE_STATUS  "memory/sysman/cradle_status"

/**
 *
 */
#define VCONFKEY_SYSMAN_LOW_MEMORY  "memory/sysman/low_memory"

enum {
	VCONFKEY_SYSMAN_LOW_MEMORY_NORMAL       = 0x01,
		VCONFKEY_SYSMAN_LOW_MEMORY_SOFT_WARNING = 0x02,
		VCONFKEY_SYSMAN_LOW_MEMORY_HARD_WARNING = 0x04
};

/**
 *
 */
#define VCONFKEY_SYSMAN_SLIDING_KEYBOARD  "memory/sysman/sliding_keyboard"

enum {
	VCONFKEY_SYSMAN_SLIDING_KEYBOARD_NOT_SUPPORTED = -1,
		VCONFKEY_SYSMAN_SLIDING_KEYBOARD_NOT_AVAILABE = 0,
		VCONFKEY_SYSMAN_SLIDING_KEYBOAED_AVAILABLE
};

/**
 * public vconf key (libstorage)
 */
#define VCONFKEY_SYSMAN_MMC_STATUS  "memory/sysman/mmc"

enum {
	VCONFKEY_SYSMAN_MMC_REMOVED = 0,
		VCONFKEY_SYSMAN_MMC_MOUNTED,
		VCONFKEY_SYSMAN_MMC_INSERTED_NOT_MOUNTED,
		VCONFKEY_SYSMAN_MMC_EXTENDEDINTERNAL_MOUNTED
};

/**
 *
 */
#define VCONFKEY_SYSMAN_MMC_MOUNT  "memory/sysman/mmc_mount"

enum {
	VCONFKEY_SYSMAN_MMC_MOUNT_COMPLETED = 0,
		VCONFKEY_SYSMAN_MMC_MOUNT_ALREADY,
		VCONFKEY_SYSMAN_MMC_MOUNT_FAILED
};

/**
 *
 */
#define VCONFKEY_SYSMAN_MMC_UNMOUNT  "memory/sysman/mmc_unmount"

enum {
	VCONFKEY_SYSMAN_MMC_UNMOUNT_COMPLETED = 0,
		VCONFKEY_SYSMAN_MMC_UNMOUNT_FAILED
};

/**
 *
 */
#define VCONFKEY_SYSMAN_MMC_FORMAT  "memory/sysman/mmc_format"

enum {
	VCONFKEY_SYSMAN_MMC_FORMAT_COMPLETED = 0,
		VCONFKEY_SYSMAN_MMC_FORMAT_FAILED
};

/**
 *
 */
#define VCONFKEY_SYSMAN_MMC_ERR_STATUS  "memory/sysman/mmc_err_status"

enum {
	VCONFKEY_SYSMAN_MMC_EPERM = 1,
		VCONFKEY_SYSMAN_MMC_ENOENT = 2,
		VCONFKEY_SYSMAN_MMC_ENXIO = 6,
		VCONFKEY_SYSMAN_MMC_EAGAIN = 11,
		VCONFKEY_SYSMAN_MMC_ENOMEM = 12,
		VCONFKEY_SYSMAN_MMC_EACCESS = 13,
		VCONFKEY_SYSMAN_MMC_EFAULT = 14,
		VCONFKEY_SYSMAN_MMC_ENOTBLK = 15,
		VCONFKEY_SYSMAN_MMC_EBUSY = 16,
		VCONFKEY_SYSMAN_MMC_ENODEV = 19,
		VCONFKEY_SYSMAN_MMC_ENOTDIR = 20,
		VCONFKEY_SYSMAN_MMC_EINVAL = 22,
		VCONFKEY_SYSMAN_MMC_EMFILE = 24,
		VCONFKEY_SYSMAN_MMC_ENAMETOOLONG = 36,
		VCONFKEY_SYSMAN_MMC_ELOOP = 40
};

/**
 *
 */
#define VCONFKEY_SYSMAN_MMC_DEVICE_CHANGED  "db/sysman/mmc_dev_changed"

enum {
	VCONFKEY_SYSMAN_MMC_NOT_CHANGED = 0,
		VCONFKEY_SYSMAN_MMC_CHANGED
};

/**
 *
 */
#define VCONFKEY_SYSMAN_HDMI  "memory/sysman/hdmi"

enum {
	VCONFKEY_SYSMAN_HDMI_DISCONNECTED = 0,
		VCONFKEY_SYSMAN_HDMI_CONNECTED
};

/**
 *
 */
#define VCONFKEY_SYSMAN_STIME  "memory/sysman/stime_changed"

enum {
	VCONFKEY_SYSMAN_STIME_NOT_CHANGED = 0,
		VCONFKEY_SYSMAN_STIME_CHANGED
};

/**
 *
 */
#define VCONFKEY_SYSMAN_POWER_OFF_STATUS  "memory/sysman/power_off"

enum {
	VCONFKEY_SYSMAN_POWER_OFF_NONE = 0,
		VCONFKEY_SYSMAN_POWER_OFF_POPUP = 1,
		VCONFKEY_SYSMAN_POWER_OFF_DIRECT = 2,
		VCONFKEY_SYSMAN_POWER_OFF_RESTART = 3
};

/**
 *
 */
#define VCONFKEY_SYSMAN_MMC_FORMAT_PROGRESS  "memory/sysman/mmc_format_progress"

enum {
	VCONFKEY_SYSMAN_MMC_FORMAT_PROGRESS_NONE = 0,
		VCONFKEY_SYSMAN_MMC_FORMAT_PROGRESS_NOW
};

/**
 *
 */
#define VCONFKEY_SYSMAN_LOW_POWER_MODE  "db/sysman/low_power_mode"

enum {
	VCONFKEY_SYSMAN_LOW_POWER_MODE_OFF = 0,
		VCONFKEY_SYSMAN_LOW_POWER_MODE_ON = 1
};

/**
 *
 */
#define VCONFKEY_SYSMAN_BATTERY_WARNING_LEVEL  "db/sysman/battery_warning_level"

/**
 *
 */
#define VCONFKEY_SYSMAN_BOOTINGSTATUS  "memory/sysman/booting_status"

enum {
	VCONFKEY_SYSMAN_BOOTING_RUNNING = 0,
		VCONFKEY_SYSMAN_BOOTING_SUCCESS = 1,
		VCONFKEY_SYSMAN_BOOTING_SUCCESS_LATE = 2,
		VCONFKEY_SYSMAN_BOOTING_FAILURE = 3
};

/**
 *  deviced changes it depending on charger type
 */
#define VCONFKEY_SYSMAN_CHARGER_TYPE  "memory/sysman/charger_type"

/**
 *  It is to set Doze Mode Level.
 */
#define VCONFKEY_SYSMAN_DOZE_MODE_LEVEL  "memory/sysman/doze_mode_level"

#endif /*   __VCONF_INTERNAL_SYSMAN_KEYS_H__    */
