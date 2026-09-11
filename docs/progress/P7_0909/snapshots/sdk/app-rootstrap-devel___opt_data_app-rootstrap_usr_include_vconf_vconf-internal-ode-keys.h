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

#ifndef __VCONF_INTERNAL_ODE_KEYS_H__
#define __VCONF_INTERNAL_ODE_KEYS_H__
/**
 *
    SDcard Encryption/Decryption state
    "encrypted":  partition encrypted
    "unencrypted": partition is normal
    "error_partially_encrypted" : Error during encryption need wipe

 */
#define VCONFKEY_SDE_CRYPTO_STATE  "db/sde/crypto_state"

/**
 *
    SDcard Crypto type
    "encrypt": Encryption in progress
    "decrypt": Decryption in progress
    "none" : No function

 */
#define VCONFKEY_SDE_CRYPTO_TYPE  "db/sde/crypto_type"

/**
 *
    @brief Encryption/Decryption progress(String values in percentage)
    0 to 100: As a String

 */
#define VCONFKEY_SDE_ENCRYPT_PROGRESS  "db/sde/encrypt_progress"

/**
 *
    @brief Disable / Enable new file in SD card Encryption
    0 - New file encryption Disable (Default)
    1 - New file encryption Enable

 */
#define VCONFKEY_SDE_ENCRYPT_NEWFILE  "db/sde/encrypt_newfile"

/**
 *
    @brief Disable / Enable excluding media file in SD card Encryption
    0 - Include media file Disable (Default)
    1 - Exclude media file Enable.

 */
#define VCONFKEY_SDE_EXCLUDE_MEDIAFILE  "db/sde/exclude_mediafile"

/**
 *
    @brief Prescan failed with disk full error, need more space
    size - needed space

 */
#define VCONFKEY_ENCRYPT_NEEDED_SIZE  "db/sde/encrypt_size"

/**
 *
    @brief External SD card mount point
    path: As a String

 */
#define VCONFKEY_SDE_MOUNT_POINT  "db/sde/mount_point"

/**
 *
    @brief ODE Encryption/Decryption state
    "encrypted":  partition encrypted
    "unencrypted": partition is normal
    "error_partially_encrypted" : Error during encryption need wipe

 */
#define VCONFKEY_ODE_CRYPTO_STATE  "db/ode/crypto_state"

/**
 *
    @brief ODE Crypto type
    "encrypt": Encryption in progress
    "decrypt": Decryption in progress
    "none" : No function

 */
#define VCONFKEY_ODE_CRYPTO_TYPE  "db/ode/crypto_type"

/**
 *
    @brief Crypto block dev used to map the real block dev
    /dev/dm-0 - crypto block dev as an example

 */
#define VCONFKEY_ODE_CRYPTO_BLKDEV  "db/ode/crypto_blkdev"

/**
 *
    @brief Encryption/Decryption progress(String values in percentage)
    0 to 100: As a String

 */
#define VCONFKEY_ODE_ENCRYPT_PROGRESS  "db/ode/encrypt_progress"

/**
 *
    @brief ODE Fast Encryption Mode Selection
    0 - Fast Encryption Disable (Default)
    1 - Fast Encryption Enable

 */
#define VCONFKEY_ODE_FAST_ENCRYPTION  "db/ode/fast_encryption"

/**
 *
    @brief secure erase progress(String values in percentage)
    0 to 100: As a String

 */
#define VCONFKEY_ODE_ERASE_PROGRESS  "memory/ode/secure_erase"

#endif /*   __VCONF_INTERNAL_ODE_KEYS_H__    */
