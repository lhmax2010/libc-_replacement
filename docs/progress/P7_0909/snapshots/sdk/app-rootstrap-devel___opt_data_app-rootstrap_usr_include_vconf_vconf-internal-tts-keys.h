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

#ifndef __VCONF_INTERNAL_TTS_KEYS_H__
#define __VCONF_INTERNAL_TTS_KEYS_H__
/**
 *  This key is an appid of TTS default engine.
 */
#define VCONFKEY_TTS_ENGINE_DEFAULT  "db/voice/tts/engine/default"

/**
 *  This key stores the UID for the TTS client which tries to instantly reprepare when it receives service reset error.
 */
#define VCONFKEY_TTS_INSTANT_REPREPARE_CLIENT  "db/voice/tts/instant-reprepare-client"

/**
 *  This key is for TTS auto configuration. If this value is true, TTS default language follows OSD language.
 */
#define VCONFKEY_TTS_CONFIG_AUTO  "db/voice/tts/config/auto"

/**
 *  This key is for TTS default language configuration.
 */
#define VCONFKEY_TTS_CONFIG_LANGUAGE  "db/voice/tts/config/language"

/**
 *  This key is for TTS voice type configuration.
 */
#define VCONFKEY_TTS_CONFIG_VOICETYPE  "db/voice/tts/config/voicetype"

/**
 *  This key is for TTS speech rate configuration.
 */
#define VCONFKEY_TTS_CONFIG_SPEECHRATE  "db/voice/tts/config/speechrate"

/**
 *  This key is for TTS pitch configuration.
 */
#define VCONFKEY_TTS_CONFIG_PITCH  "db/voice/tts/config/pitch"

/**
 *  This key is for TTS background volume configuration. The unit is percentage.
 */
#define VCONFKEY_TTS_CONFIG_BGVOLUME  "db/voice/tts/config/bgvolume"

#endif /*   __VCONF_INTERNAL_TTS_KEYS_H__    */
