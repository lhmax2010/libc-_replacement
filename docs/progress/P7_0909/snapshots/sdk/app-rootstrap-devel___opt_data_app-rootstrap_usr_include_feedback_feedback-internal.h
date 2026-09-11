/*
 * libfeedback
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __FEEDBACK_INTERNAL_H__
#define __FEEDBACK_INTERNAL_H__

#include <tizen_error.h>
#include <feedback-ids.h>
#include "feedback-ids-internal.h"
#include "feedback-ids-product.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file feedback-internal.h
 * @brief This file contains the feedback internal API
 */

/**
 * @addtogroup CAPI_SYSTEM_FEEDBACK_INTERNAL_MODULE
 * @{
 */

#define FEEDBACK_SUCCEEDED(n)                ((n) == FEEDBACK_ERROR_NONE)
#define FEEDBACK_FAILED(n)                   ((n) != FEEDBACK_ERROR_NONE)

/**
 * @brief Enumeration for feedback sound play method.
 * @details Currently, it is used only in the sound play.
 * @since_tizen 7.0
 */
typedef enum {
	FEEDBACK_FLAG_NONE = 0,                     /**< Feedback flag none */
	FEEDBACK_FLAG_PRIORITY_BASED_PLAY = (1<<0), /**< Feedback flag for priority-based play */
} feedback_flag_e;

/**
 *
 * @brief Plays specific type of reactions that are pre-defined feedback pattern.
 * @details This function can be used to react to pre-defined actions. \n
 *          It play specific type of system pre-defined pattern.
 * @since_tizen 2.3
 * @remarks Currently, there are two types of reactions: sound and vibration.
 * @param[in] type string      The pattern type
 * @param[in] pattern string   The pre-defined pattern
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE               Successful
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED      Not supported device
 * @pre feedback_initialize()
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     feedback_play_type_by_name("FEEDBACK_TYPE_SOUND", "FEEDBACK_PATTERN_SCREEN_CAPTURE");
 *     ...
 *     feedback_deinitialize();
 * }
 * ...
 * @endcode
 */
int feedback_play_type_by_name(char *type, char *pattern);

/**
 * @brief Plays various types of reactions that are pre-defined feedback pattern.
 * @details This function can be used to react to pre-defined actions. \n
 *          It play various types of system pre-defined media or vibration patterns.
 * @since_tizen 4.0
 * @remarks Currently, there are two types of reactions: sound and vibration. \n
 *          Depending on the settings, some types cannot operate.
 *          For example, when set to silent mode, the device doesn't produce any sound.
 *          If to play one of the devices is successful, this function regards as success.
 *          And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 *          If you don't have the haptic privilege, it only works sound operation.
 *          It does not return any error in this case.
 * @param[in] pattern The pre-defined internal pattern
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED Not initialized
 * @pre feedback_initialize()
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     feedback_play_internal(FEEDBACK_PATTERN_SCREEN_CAPTURE);
 *     ...
 *     feedback_deinitialize();
 * }
 * ...
 * @endcode
 * @see feedback_pattern_internal_e
 */
int feedback_play_internal(feedback_pattern_internal_e pattern);

/**
 * @brief Plays various types of reactions that are pre-defined feedback pattern.
 * @details This function can be used to react to pre-defined actions. \n
 *          It play various types of system pre-defined media or vibration patterns.
 *          The @a soundpath parameter must not be NULL.
 * @since_tizen 5.5
 * @remarks Currently, there are two types of reactions: sound and vibration. \n
 *          Depending on the settings, some types cannot operate.
 *          For example, when set to silent mode, the device doesn't produce any sound.
 *          If to play one of the devices is successful, this function regards as success.
 *          And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 *          If you don't have the haptic privilege, it only works sound operation.
 *          It does not return any error in this case.
 * @param[in] pattern The pre-defined internal pattern
 * @param[in] soundpath The resource file path for sound (can't be NULL)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED Not initialized
 * @pre feedback_initialize()
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     feedback_play_soundpath_internal(FEEDBACK_PATTERN_SCREEN_CAPTURE, "/usr/share/feedback/sound/operation/shutter.wav");
 *     ...
 *     feedback_deinitialize();
 * }
 * ...
 * @endcode
 * @see feedback_pattern_internal_e
 */
int feedback_play_soundpath_internal(feedback_pattern_internal_e internal_pattern, const char *soundpath);

/**
 * @brief Plays specific type of reactions that are pre-defined feedback pattern.
 * @details This function can be used to react to pre-defined actions. \n
 *          It play specific type of system pre-defined pattern.
 * @since_tizen 4.0
 * @remarks Currently, there are two types of reactions: sound and vibration. \n
 *          Depending on the settings, some types cannot operate.
 *          For example, when set to silent mode, the device doesn't produce any sound.
 *          And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 *          If you don't have the haptic privilege, it returns FEEDBACK_ERROR_PERMISSION_DENIED error.
 * @param[in] type The pattern type
 * @param[in] pattern The pre-defined internal pattern
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_PERMISSION_DENIED Permission denied
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED Not initialized
 * @pre feedback_initialize()
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     feedback_play_type_internal(FEEDBACK_TYPE_SOUND, FEEDBACK_PATTERN_SCREEN_CAPTURE);
 *     ...
 *     feedback_deinitialize();
 * }
 * ...
 * @endcode
 * @see feedback_type_e
 * @see feedback_pattern_internal_e
 */
int feedback_play_type_internal(feedback_type_e type, feedback_pattern_internal_e pattern);


/**
 * @brief Plays specific type of reactions that are pre-defined feedback pattern.
 * @details This function can be used to react to pre-defined actions. \n
 *          It play specific type of system pre-defined pattern.
 *          The @a soundpath parameter must not be NULL for sound type.
 * @since_tizen 4.0
 * @remarks Currently, there are two types of reactions: sound and vibration. \n
 *          Depending on the settings, some types cannot operate.
 *          For example, when set to silent mode, the device doesn't produce any sound.
 *          And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 *          If you don't have the haptic privilege, it returns FEEDBACK_ERROR_PERMISSION_DENIED error.
 * @param[in] type The pattern type
 * @param[in] pattern The pre-defined internal pattern
 * @param[in] soundpath The resource file path for sound
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_PERMISSION_DENIED Permission denied
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED Not initialized
 * @pre feedback_initialize()
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     feedback_play_type_internal(FEEDBACK_TYPE_SOUND, FEEDBACK_PATTERN_SCREEN_CAPTURE, "/usr/share/feedback/sound/operation/shutter.wav");
 *     ...
 *     feedback_deinitialize();
 * }
 * ...
 * @endcode
 * @see feedback_type_e
 * @see feedback_pattern_internal_e
 */
int feedback_play_type_soundpath_internal(feedback_type_e type, feedback_pattern_internal_e internal_pattern, const char *soundpath);

/**
 * @brief Gets the number of themes supported as described in the configuration.
 * @details This function gets the number of theme described in the config file.
 *          The range of counted theme will be 1~N according to conf file.
 * @since_tizen 7.0
 * @remarks If it is not properly defined in the configuration file, this fails from the feedback_initialize() step.
 * @param[in] type The feedback type
 * @param[out] count_of_theme The number of theme supported
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED Not initialized
 * @pre feedback_initialize()
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * unsigned int count_of_theme;
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     ret = feedback_get_count_of_theme_internal(FEEDBACK_TYPE_SOUND, &count_of_theme);
 *     ...
 * }
 * ...
 * @endcode
 * @see feedback_type_e
 */
int feedback_get_count_of_theme_internal(feedback_type_e feedback_type, unsigned int *count_of_theme);

/**
 * @brief Gets the current ID of the theme selected from available themes described in the conf file.
 * @details This function gets the current theme id selected.
 *          The theme ID is positive value according to conf file.
 * @since_tizen 7.0
 * @remarks If feedback_set_theme_id_internal() is never called, it will returns the default theme ID.
 * @param[in] type The feedback type
 * @param[out] id_of_theme The current ID of theme selected
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * unsigned int id_of_theme;
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     int ret = feedback_get_theme_id_internal(FEEDBACK_TYPE_SOUND, &id_of_theme);
 *     ...
 * }
 * ...
 * @endcode
 * @see feedback_type_e
 */
int feedback_get_theme_id_internal(feedback_type_e feedback_type, unsigned int *id_of_theme);

/**
 * @brief Sets the current ID of the theme from available themes described in the conf file.
 * @details This function sets the theme id.
 *          The theme id is positive value according to conf file.
 *          Please put the accurate theme id value.
 * @since_tizen 7.0
 * @remarks For setting feedback theme id, the privilege should be set to, %http://tizen.org/privilege/systemsettings.admin.
 *          If app doesn't have the privilege, it returns FEEDBACK_ERROR_PERMISSION_DENIED error.
 * @param[in] type The feedback type
 * @param[in] id_of_theme The id of theme will be selected
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_PERMISSION_DENIED Permission denied
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * unsigned int id_of_theme = 1;
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     int ret = feedback_set_theme_id_internal(FEEDBACK_TYPE_SOUND, id_of_theme);
 *     ...
 * }
 * ...
 * @endcode
 * @see feedback_type_e
 */
int feedback_set_theme_id_internal(feedback_type_e feedback_type, unsigned int id_of_theme);

/**
 * @brief Stops reactions of various types according to the feedback type.
 * @details This function can be used to stop reaction to pre-defined actions.
 *          It stops system pre-defined vibration and sound patterns.
 * @since_tizen 7.0
 * @remarks To stop vibrator feedback, the privilege should be set to, %http://tizen.org/privilege/haptic.
 * @param[in] feedback_type The feedback type
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_PERMISSION_DENIED Permission denied
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * unsigned int id_of_theme = 1;
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     feedback_play_type_internal(FEEDBACK_TYPE_SOUND, FEEDBACK_PATTERN_SCREEN_CAPTURE);
 *     ...
 *     feedback_stop_type_internal(FEEDBACK_TYPE_SOUND);
 * }
 * ...
 * @endcode
 * @see feedback_type_e
 */
int feedback_stop_type_internal(feedback_type_e feedback_type);

/**
 * @brief Gets the array of theme ids supported described in the conf file.
 * @details This function gets all theme id as defined in the conf file.
 *          The theme id is positive value according to conf file.
 *          After using theme ids, it should be freed by caller.
 * @since_tizen 7.0
 * @remarks Ensure to free the memory allocated for theme ids by feedback_put_theme_ids_internal().
 * @param[in] type The feedback type
 * @param[out] count_of_theme This means size of theme id array
 * @param[out] theme_ids The theme id array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * unsigned int count_of_theme;
 * unsigned int *theme_ids;
 * int index = 0;
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     ret = feedback_get_theme_ids_internal(FEEDBACK_TYPE_SOUND, &count_of_theme, &theme_ids);
 *     if (ret != FEEDBACK_ERROR_NONE) {
 *         return -1;
 *     }
 *     for (int index = 0; index < count_of_theme; index++) {
 *         ...
 *     }
 *    ret = feedback_put_theme_ids_internal(&theme_ids);
 * }
 * ...
 * @endcode
 * @see feedback_put_theme_ids_internal()
 * @see feedback_type_e
 */
int feedback_get_theme_ids_internal(feedback_type_e feedback_type, unsigned int *count_of_theme, unsigned int **theme_ids);

/**
 * @brief Free the array of theme ids allocated by feedback_get_theme_ids_internal().
 * @details This function free the array of theme ids from feedback_get_theme_ids_internal()
 *          Also, it is possible for the user to release the array directly.
 * @since_tizen 7.0
 * @remarks Ensure @a theme_ids is valid address of theme id array from feedback_get_theme_ids_internal().
 * @param[in] theme_ids The Address of theme id array
 * @return @c 0 on success, otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * unsigned int count_of_theme;
 * unsigned int *theme_ids;
 * int index = 0;
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     ret = feedback_get_theme_ids_internal(FEEDBACK_TYPE_SOUND, &count_of_theme, &theme_ids);
 *     if (ret != FEEDBACK_ERROR_NONE) {
 *         return -1;
 *     }
 *     for (int index = 0; index < count_of_theme; index++) {
 *         ...
 *     }
 *    ret = feedback_put_theme_ids_internal(&theme_ids);
 * }
 * ...
 * @endcode
 * @see feedback_get_theme_ids_internal()
 */
int feedback_put_theme_ids_internal(unsigned int **theme_ids);

/**
 * @brief Plays specific type of reactions that are pre-defined with priority.
 * @details This function can be used to react to pre-defined actions. \n
 *          It play specific type of system pre-defined pattern with priority.
 *          If @a flag is FEEDBACK_FLAG_NONE, it works the same as feedback_play_type_internal().
 * @since_tizen 7.0
 * @remarks Currently, there are two types of reactions: sound and vibration. \n
 *          Depending on the settings, some types cannot be operated.
 *          FEEDBACK_FLAG_PRIORITY_BASED_PLAY is for playing sound with priority.
 *          FEEDBACK_FLAG_NONE works the same as feedback_play_type_internal().
 *          And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 *          If you don't have the haptic privilege, it returns FEEDBACK_ERROR_PERMISSION_DENIED error.
 *          The above-mentioned haptic device rule is only for FEEDBACK_FLAG_NONE.
 *          Priority playing is not supported in vibrator.
 * @param[in] type The pattern type
 * @param[in] pattern The pre-defined internal pattern
 * @param[in] flag The sound play method flag
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #FEEDBACK_ERROR_NONE Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED Not supported device
 * @retval #FEEDBACK_ERROR_PERMISSION_DENIED Permission denied
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED Not initialized
 * @pre feedback_initialize()
 *
 * @code
 * #include <feedback-internal.h>
 * ...
 * int ret = feedback_initialize();
 * if (ret == FEEDBACK_ERROR_NONE) {
 *     ret = feedback_play_type_with_flags_internal(FEEDBACK_TYPE_SOUND, FEEDBACK_PATTERN_SCREEN_CAPTURE, FEEDBACK_FLAG_PRIORITY_BASED_PLAY);
 *     ...
 * }
 * ...
 * @endcode
 * @see feedback_play_type_internal()
 * @see feedback_type_e
 * @see feedback_pattern_internal_e
 * @see feedback_flag_e
 */
int feedback_play_type_with_flags_internal(feedback_type_e type, feedback_pattern_internal_e internal_pattern, feedback_flag_e flag);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_INTERNAL_H__
