/**
 * Add a new conformant widget to the given parent Elementary
 * (container) object.
 *
 * @param parent The parent object.
 * @return A new conformant widget handle or @c NULL, on errors.
 *
 * This function inserts a new conformant widget on the canvas.
 *
 * @ingroup Elm_Conformant
 */
EAPI Evas_Object                 *elm_conformant_add(Evas_Object *parent);

// TIZEN_ONLY(20200311): Add conformant resize disable API.
/**
 * @brief Disables conformant input area resize.
 *
 * The conformant resizes when an input area such as a keypad is launched to rebuild the internal layout.
 * This API prevent conformant resize when the input state changed.
 *
 * @param obj The elm_conformant object.
 * @param disable true disable conformant resize, otherwise false.
 *
 * @ingroup Elm_Conformant
 * @see elm_conformant_input_area_resize_disabled_get()
 */
EAPI void                         elm_conformant_input_area_resize_disabled_set(Evas_Object *obj, Eina_Bool disable);

/**
 * @brief Gets the conformant input area resize disabled or not.
 *
 * @param obj The elm_conformant object.
 *
 * @return true confomant resize disabled, otherwise false.
 *
 * @ingroup Elm_Conformant
 * @see elm_conformant_input_area_resize_disabled_set()
 */
EAPI Eina_Bool                    elm_conformant_input_area_resize_disabled_get(Evas_Object *obj);
//

// TIZEN_ONLY(20160218): Improve launching performance.
/**
 * @internal
 *
 * Set the precreated object.
 *
 * @param obj The conformant object
 *
 * @ingroup Elm_Conformant
 * @see elm_conformant_precreated_object_get()
 * @since Tizen 2.4
 */
EAPI void                         elm_conformant_precreated_object_set(Evas_Object *obj);

/**
 * @internal
 *
 * Get the precreated object.
 *
 * @return The precreated conformant object
 *
 * @ingroup Elm_Conformant
 * @see elm_conformant_precreated_object_set()
 * @since Tizen 2.4
 */
EAPI Evas_Object                 *elm_conformant_precreated_object_get(void);
//
