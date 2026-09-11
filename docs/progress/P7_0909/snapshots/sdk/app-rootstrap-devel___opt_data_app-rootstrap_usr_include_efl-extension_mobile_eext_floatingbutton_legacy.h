/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Flora License, Version 1.1 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "eext_floatingbutton_eo.legacy.h"

/**
 * @deprecated Deprecated since 10.0.
 * @MOBILE_ONLY
 *
 * @brief Add a new floatingbutton to the parent
 *
 * @if MOBILE @since_tizen 2.4
 * @endif
 *
 * @param[in] parent The parent object
 * @return The new object or NULL if it cannot be created
 *
 * @ingroup Floatingbutton
 */
EAPI Evas_Object *eext_floatingbutton_add(Evas_Object *parent);

/**
 * @brief Set the floatingbutton position
 *
 * The floatingbutton can be moved among predefined positions (LEFT_OUT, LEFT,
 * CENTER, RIGHT, RIGHT_OUT), eext_floatingbutton_pos_set() moves
 * floatingbutton to a given position immediately (without animation).
 *
 * return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @param[in] obj The object.
 * @param[in] pos the floatingbutton position
 *
 * @ingroup Eext_Floatingbutton
 */
EAPI Eina_Bool eext_floatingbutton_pos_set(Evas_Object *obj, Eext_Floatingbutton_Pos pos);

/**
 * @brief Get the floatingbutton position
 *
 * return Floatingbutton position with Eext_Floatingbutton_Pos enumeration
 *
 * see eext_floatingbutton_pos_set()
 *
 * @param[in] obj The object.
 *
 * @return the floatingbutton position
 *
 * @ingroup Eext_Floatingbutton
 */
EAPI Eext_Floatingbutton_Pos eext_floatingbutton_pos_get(const Evas_Object *obj);

/**
 * @brief Set whether floatingbutton can be moved or not.
 *
 * By default, floatingbutton are movable by user interaction.
 *
 * @param[in] obj The object.
 * @param[in] block the floatingbutton movability
 *
 * @ingroup Eext_Floatingbutton
 */
EAPI void eext_floatingbutton_movement_block_set(Evas_Object *obj, Eina_Bool block);

/**
 * @brief Get movability for a given floatingbutton widget.
 *
 * return EINA_TRUE, if obj is set to be unmovable, EINA_FALSE, if it's
 * movable.
 *
 * see eext_floatingbutton_movement_block_set()
 *
 * @param[in] obj The object.
 *
 * @return the floatingbutton movability
 *
 * @ingroup Eext_Floatingbutton
 */
EAPI Eina_Bool eext_floatingbutton_movement_block_get(const Evas_Object *obj);

/**
 * @brief Set floatingbutton mode.
 *
 * Positions where floatingbutton can stop are restrained by mode.
 *
 * @param[in] obj The object.
 * @param[in] mode floatingbutton mode
 *
 * @ingroup Eext_Floatingbutton
 */
EAPI void eext_floatingbutton_mode_set(Evas_Object *obj, Eext_Floatingbutton_Mode mode);

/**
 * @brief Get floatingbutton mode.
 *
 * see eext_floatingbutton_mode_set()
 *
 * @param[in] obj The object.
 *
 * @return floatingbutton mode
 *
 * @ingroup Eext_Floatingbutton
 */
EAPI Eext_Floatingbutton_Mode eext_floatingbutton_mode_get(const Evas_Object *obj);

/**
 * @brief Set the floatingbutton position with animation
 *
 * Move floatingbutton similar to eext_floatingbutton_pos_set(), but with
 * animation.
 *
 * return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * see eext_floatingbutton_pos_set()
 *
 * @param[in] obj The object.
 * @param[in] pos the floatingbutton position
 *
 * @ingroup Eext_Floatingbutton
 */
EAPI Eina_Bool eext_floatingbutton_pos_bring_in(Evas_Object *obj, const Eext_Floatingbutton_Pos pos);
