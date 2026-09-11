/**
 * @brief Add a new relative_container to the parent
 *
 * @param parent The parent object
 * @return The new object or NULL if it cannot be created
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI Evas_Object *elm_relative_container_add(Evas_Object *parent);

/**
 * @brief Specifies the left side edge of the @c child object relative to the
 * @c base object.
 * When @c relative_position is 0 the left edges of the two objects are aligned.
 * When @c relative_position is 1 the left edge of the @c child object is
 * aligned to the right edge of the @c base object.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[in] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[in] relative_position The ratio between left and right of the base,
 * ranging from 0.0 to 1.0. 0.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_left_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);

/**
 * @brief Get the left side edge of the @c child object relative to the
 * @c base object.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[out] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[out] relative_position The ratio between left and right of the base,
 * ranging from 0.0 to 1.0. 0.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_left_get(const Evas_Object *obj, Evas_Object *child, Evas_Object **base, double *relative_position);

/**
 * @brief Specifies the right side edge of the @c child object relative to the
 * @c base object.
 * When @c relative_position is 0 the right edge of the @c child object is
 * aligned to the left edge of the @c base object.
 * When @c relative_position is 1 the right edges of the two objects are aligned.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[in] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[in] relative_position The ratio between left and right of the base,
 * ranging from 0.0 to 1.0. 1.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_right_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);

/**
 * @brief Get the right side edge of the @c child object relative to the
 * @c base object.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[out] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[out] relative_position The ratio between left and right of the base,
 * ranging from 0.0 to 1.0. 1.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_right_get(const Evas_Object *obj, Evas_Object *child, Evas_Object **base, double *relative_position);

/**
 * @brief Specifies the top side edge of the @c child object relative to the
 * @c base object.
 * When @c relative_position is 0 the top edges of the two objects are aligned.
 * When @c relative_position is 1 the top edge of the @c child object is aligned
 * to the bottom edge of the @c base object.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[in] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[in] relative_position The ratio between top and bottom of the base,
 * ranging from 0.0 to 1.0. 0.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_top_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);

/**
 * @brief Get the top side edge of the @c child object relative to the
 * @c base object.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[out] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[out] relative_position The ratio between top and bottom of the base,
 * ranging from 0.0 to 1.0. 0.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_top_get(const Evas_Object *obj, Evas_Object *child, Evas_Object **base, double *relative_position);

/**
 * @brief Specifies the bottom side edge of the @c child object relative to the
 * @c base object.
 * When @c relative_position is 0 the bottom edge of the @c child object is
 * aligned to the top edge of the @c base object.
 * When @crelative_position is 1 the bottom edges of the two objects are aligned.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[in] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[in] relative_position The ratio between top and bottom of the base,
 * ranging from 0.0 to 1.0. 1.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_bottom_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);

/**
 * @brief Get the bottom side edge of the @c child object relative to the
 * @c base object.
 *
 * @param[in] obj The object.
 * @param[in] child The child object whose size and position is being changed.
 * @param[out] base The object whose size and position is being used as reference.
 * @c NULL means that the container object is used(this is the default value).
 * @param[out] relative_position The ratio between top and bottom of the base,
 * ranging from 0.0 to 1.0. 1.0 by default.
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI void elm_relative_container_relation_bottom_get(const Evas_Object *obj, Evas_Object *child, Evas_Object **base, double *relative_position);

/**
 * @brief Get a list of the objects in the relative_container.
 *
 * Returns a new @c list with a pointer to @c Evas_Object in its nodes.
 *
 * You must free this list with eina_list_free() once you are done with it.
 *
 * @param[in] obj The object.
 *
 * @return List of children
 *
 * @ingroup Elm_Relative_Container_Group
 */
EAPI Eina_List *elm_relative_container_children_get(const Evas_Object *obj);
