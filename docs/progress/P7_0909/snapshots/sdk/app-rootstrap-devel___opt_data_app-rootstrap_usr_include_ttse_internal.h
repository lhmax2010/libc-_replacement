/*
*  Copyright (c) 2011-2016 Samsung Electronics Co., Ltd All Rights Reserved
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*  http://www.apache.org/licenses/LICENSE-2.0
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/


#ifndef __TTSE_INTERNAL_H__
#define __TTSE_INTERNAL_H__


#include <ttse.h>



#ifdef __cplusplus
extern "C" {
#endif


/**
* @brief Called when the engine service user sets the engine_type of synthesized speech.
* @since_tizen 9.0
* @remarks This callback function is optional and is registered using ttse_set_engine_type_set_cb().
* @param[in] engine_type The engine_type of synthesized speech
* @param[in] user_data The user data to be passed to the callback function
* @return @c 0 on success,
*         otherwise a negative error value
* @retval #TTSE_ERROR_NONE Successful
* @retval #TTSE_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #TTSE_ERROR_INVALID_STATE Not initialized
* @retval #TTSE_ERROR_OPERATION_FAILED Operation failure
*/
typedef int (*ttse_set_engine_type_cb)(const char* engine_type, void* user_data);




/**
* @brief Sets a callback function to be called when the engine_type of synthesized speech is set.
* @since_tizen 9.0
* @remarks The ttse_set_engine_type_cb() function is called when the engine service user sets the engine_type of synthesized speech.
* @param[in] callback ttse_set_engine_type_cb event callback function
* @param[in] user_data The user data passed from engine
* @return @c 0 on success,
*         otherwise a negative error value
* @retval #TTSE_ERROR_NONE Successful
* @retval #TTSE_ERROR_NOT_SUPPORTED TTS NOT supported
* @retval #TTSE_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #TTSE_ERROR_INVALID_STATE Not initialized
* @pre The ttse_main() function should be invoked before this function is called.
* @see ttse_set_engine_type_cb()
*/
int ttse_set_engine_type_set_cb(ttse_set_engine_type_cb callback, void* user_data);


#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif /* __TTSE_INTERNAL_H__ */
