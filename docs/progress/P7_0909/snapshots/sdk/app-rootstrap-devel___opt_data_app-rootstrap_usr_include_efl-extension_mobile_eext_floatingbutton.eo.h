#ifndef _EEXT_FLOATINGBUTTON_EO_H_
#define _EEXT_FLOATINGBUTTON_EO_H_

#ifndef _EEXT_FLOATINGBUTTON_EO_CLASS_TYPE
#define _EEXT_FLOATINGBUTTON_EO_CLASS_TYPE

typedef Eo Eext_Floatingbutton;

#endif

#ifndef _EEXT_FLOATINGBUTTON_EO_TYPES
#define _EEXT_FLOATINGBUTTON_EO_TYPES

/** Floating button mode
 *
 * @since 1.23
 *
 * @ingroup Eext_Floatingbutton_Mode
 */
typedef enum
{
  EEXT_FLOATINGBUTTON_MODE_DEFAULT = 0, /**< allows all positions
                                         *
                                         * @since 1.23 */
  EEXT_FLOATINGBUTTON_MODE_BOTH_SIDES, /**< allows LEFT and RIGHT positions only
                                        *
                                        * @since 1.23 */
  EEXT_FLOATINGBUTTON_MODE_LAST /**< indicates the last, do not use this
                                 *
                                 * @since 1.23 */
} Eext_Floatingbutton_Mode;


#endif
#define EEXT_FLOATINGBUTTON_CLASS eext_floatingbutton_class_get()

EWAPI const Efl_Class *eext_floatingbutton_class_get(void) EINA_CONST;

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
EOAPI Eina_Bool eext_ui_floatingbutton_pos_set(Eo *obj, Eext_Floatingbutton_Pos pos);

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
EOAPI Eext_Floatingbutton_Pos eext_ui_floatingbutton_pos_get(const Eo *obj);

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
EOAPI void eext_ui_floatingbutton_movement_block_set(Eo *obj, Eina_Bool block);

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
EOAPI Eina_Bool eext_ui_floatingbutton_movement_block_get(const Eo *obj);

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
EOAPI void eext_ui_floatingbutton_mode_set(Eo *obj, Eext_Floatingbutton_Mode mode);

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
EOAPI Eext_Floatingbutton_Mode eext_ui_floatingbutton_mode_get(const Eo *obj);

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
 * @since 1.23
 *
 * @ingroup Eext_Floatingbutton
 */
EOAPI Eina_Bool eext_ui_floatingbutton_pos_bring_in(Eo *obj, const Eext_Floatingbutton_Pos pos);

#endif
