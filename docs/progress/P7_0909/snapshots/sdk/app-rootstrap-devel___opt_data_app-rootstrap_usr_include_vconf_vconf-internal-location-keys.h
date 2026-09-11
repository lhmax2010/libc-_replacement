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

#ifndef __VCONF_INTERNAL_LOCATION_KEYS_H__
#define __VCONF_INTERNAL_LOCATION_KEYS_H__
/**
 *
 */
#define VCONFKEY_LOCATION_USE_MY_LOCATION  "db/location/setting/Usemylocation"

/**
 *
 */
#define VCONFKEY_LOCATION_ENABLED  "db/location/setting/GpsEnabled"

enum {
	VCONFKEY_LOCATION_SETTING_OFF = 0,
		VCONFKEY_LOCATION_SETTING_ON
};

/**
 *
 */
#define VCONFKEY_LOCATION_NETWORK_ENABLED  "db/location/setting/NetworkEnabled"

/**
 * CAUTION: Write privilege is http://tizen.org/privilege/location.
 */
#define VCONFKEY_LOCATION_MOCK_ENABLED  "db/location/setting/MockEnabled"

/**
 *
 */
#define VCONFKEY_LBS_SETTING_IS_SHOW_GPS_POPUP  "db/location/setting/GpsPopup"

enum {
	VCONFKEY_LOCATION_POPUP_OFF = 0,
		VCONFKEY_LOCATION_POPUP_SHOW_GPS,
		VCONFKEY_LOCATION_POPUP_SHOW_WPS,
		VCONFKEY_LOCATION_POPUP_SHOW_ALL
};

/**
 *
 */
#define VCONFKEY_LOCATION_RESTRICT  "db/location/setting/Restriction"

/**
 *
 */
#define VCONFKEY_LOCATION_GYRO_DRIFT  "db/location/setting/GyroDrift"

/**
 *
 */
#define VCONFKEY_LOCATION_POSITION_STATE  "memory/location/position/state"

enum {
	VCONFKEY_LOCATION_POSITION_OFF = 0,
		VCONFKEY_LOCATION_POSITION_SEARCHING,
		VCONFKEY_LOCATION_POSITION_CONNECTED,
		VCONFKEY_LOCATION_POSITION_STATE_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_GPS_STATE  "memory/location/gps/state"

enum {
	VCONFKEY_LOCATION_GPS_OFF = 0,
		VCONFKEY_LOCATION_GPS_SEARCHING,
		VCONFKEY_LOCATION_GPS_CONNECTED,
		VCONFKEY_LOCATION_GPS_STATE_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_WPS_STATE  "memory/location/wps/state"

enum {
	VCONFKEY_LOCATION_WPS_OFF = 0,
		VCONFKEY_LOCATION_WPS_SEARCHING,
		VCONFKEY_LOCATION_WPS_CONNECTED,
		VCONFKEY_LOCATION_WPS_STATE_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_MOCK_STATE  "memory/location/mock/state"

enum {
	VCONFKEY_LOCATION_MOCK_OFF = 0,
		VCONFKEY_LOCATION_MOCK_SEARCHING,
		VCONFKEY_LOCATION_MOCK_CONNECTED,
		VCONFKEY_LOCATION_MOCK_STATE_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_TIMESTAMP  "memory/location/last/gps/Timestamp"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_LATITUDE  "memory/location/last/gps/Latitude"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_LONGITUDE  "memory/location/last/gps/Longitude"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_ALTITUDE  "memory/location/last/gps/Altitude"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_SPEED  "memory/location/last/gps/Speed"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_DIRECTION  "memory/location/last/gps/Direction"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_HOR_ACCURACY  "memory/location/last/gps/HorAccuracy"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_VER_ACCURACY  "memory/location/last/gps/VerAccuracy"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_WPS_TIMESTAMP  "memory/location/last/wps/Timestamp"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_WPS_LATITUDE  "memory/location/last/wps/Latitude"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_WPS_LONGITUDE  "memory/location/last/wps/Longitude"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_WPS_ALTITUDE  "memory/location/last/wps/Altitude"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_WPS_SPEED  "memory/location/last/wps/Speed"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_WPS_DIRECTION  "memory/location/last/wps/Direction"

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_WPS_HOR_ACCURACY  "memory/location/last/wps/HorAccuracy"

/**
 *
 */
#define VCONFKEY_LOCATION_NV_LAST_GPS_TIMESTAMP  "db/location/last/gps/LocTimestamp"

/**
 *
 */
#define VCONFKEY_LOCATION_NV_LAST_WPS_TIMESTAMP  "db/location/last/wps/LocTimestamp"

/**
 *
 */
#define VCONFKEY_LOCATION_NV_LAST_GPS_LOCATION  "db/location/last/gps/Location"

/**
 *
 */
#define VCONFKEY_LOCATION_NV_LAST_WPS_LOCATION  "db/location/last/wps/Location"

/**
 *
 */
#define VCONFKEY_LOCATION_NMEA_LOGGING  "db/location/nmea/LoggingEnabled"

/**
 * Initial value for device is difference from that of emulator. It's initial value is for device
 */
#define VCONFKEY_LOCATION_REPLAY_ENABLED  "db/location/replay/ReplayEnabled"

/**
 * Initial value for device is difference from that of emulator. It's initial value is for device
 */
#define VCONFKEY_LOCATION_REPLAY_MODE  "db/location/replay/ReplayMode"

enum {
	VCONFKEY_GPS_REPLAY_OFF = 0,
		VCONFKEY_GPS_REPLAY_NMEA,
		VCONFKEY_GPS_REPLAY_MANAUL,
		VCONFKEY_GPS_REPLAY_MODE_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_NMEA_FILE_NAME  "db/location/replay/FileName"

/**
 *
 */
#define VCONFKEY_LOCATION_MANUAL_LATITUDE  "db/location/replay/ManualLatitude"

/**
 *
 */
#define VCONFKEY_LOCATION_MANUAL_LONGITUDE  "db/location/replay/ManualLongitude"

/**
 *
 */
#define VCONFKEY_LOCATION_MANUAL_ALTITUDE  "db/location/replay/ManualAltitude"

/**
 *
 */
#define VCONFKEY_LOCATION_MANUAL_HORIZONTAL_ACCURACY  "db/location/replay/ManualHAccuracy"

/**
 *
 */
#define VCONFKEY_LOCATION_GPS_OPERATION  "db/location/gps/Operation"

enum {
	VCONFKEY_GPS_OPERATION_STANDALONE = 0,
		VCONFKEY_GPS_OPERATION_MS_BASED,
		VCONFKEY_GPS_OPERATION_MS_ASSISTED,
		VCONFKEY_GPS_OPERATION_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_GPS_OPERATION_TEST  "db/location/gps/OperationTest"

enum {
	VCONFKEY_GPS_OPERATION_TEST_NORMAL = 0,
		VCONFKEY_GPS_OPERATION_TEST_HW_SENSITIVITY,
		VCONFKEY_GPS_OPERATION_TEST_HW_CN0,
		VCONFKEY_GPS_OPERATION_TEST_HW_CW,
		VCONFKEY_GPS_OPERATION_TEST_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_GPS_STARTING  "db/location/gps/Starting"

enum {
	VCONFKEY_GPS_STARTING_HOT = 0,
		VCONFKEY_GPS_STARTING_WARM,
		VCONFKEY_GPS_STARTING_COLD,
		VCONFKEY_GPS_STARTING_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_GPS_SESSION  "db/location/gps/Session"

enum {
	VCONFKEY_GPS_SESSION_SINGLE_FIX = 0,
		VCONFKEY_GPS_SESSION_TRACKING,
		VCONFKEY_GPS_SESSION_MAX
};

/**
 *
 */
#define VCONFKEY_LOCATION_LAST_GPS_XTRA_DOWNLOAD_TIME  "db/location/gps/XtraDownloadTime"

/**
 *
 */
#define VCONFKEY_LOCATION_SUPL_SERVER  "db/location/supl/Server"

/**
 *
 */
#define VCONFKEY_LOCATION_SUPL_PORT  "db/location/supl/Port"

/**
 *
 */
#define VCONFKEY_LOCATION_SUPL_SSL  "db/location/supl/SslEnabled"

/**
 *
 */
#define VCONFKEY_LOCATION_SUPL_FQDNTYPE  "db/location/supl/FQDNType"

enum {
	VCONFKEY_LOCATION_SUPL_FQDN_CUSTOM  = 0,
		VCONFKEY_LOCATION_SUPL_FQDN_AUTO,
		VCONFKEY_LOCATION_SUPL_FQDN_SPIRENT,
		VCONFKEY_LOCATION_SUPL_FQDN_DOCOMO,
		VCONFKEY_LOCATION_SUPL_FQDN_HERE,
		VCONFKEY_LOCATION_SUPL_FQDN_VFONE,
		VCONFKEY_LOCATION_SUPL_FQDN_TMO,
		VCONFKEY_LOCATION_SUPL_FQDN_ATT,
		VCONFKEY_LOCATION_SUPL_FQDN_VZW,
		VCONFKEY_LOCATION_SUPL_FQDN_SKT,
		VCONFKEY_LOCATION_SUPL_FQDN_GOOGLE,
		VCONFKEY_LOCATION_SUPL_FQDN_BUILD
};

/**
 *
 */
#define VCONFKEY_LOCATION_SUPL_VERSION  "db/location/supl/Version"

/**
 * CAUTION: Write privilege is http://tizen.org/privilege/mapservice.
 */
#define VCONFKEY_LOCATION_HEREMAPS_CONSENT  "db/location/heremaps/UserConsent"

#endif /*   __VCONF_INTERNAL_LOCATION_KEYS_H__    */
