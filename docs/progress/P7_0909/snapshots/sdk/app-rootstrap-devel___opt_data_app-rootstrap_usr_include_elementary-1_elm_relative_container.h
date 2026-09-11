/**
 * @defgroup Elm_Relative_Container_Group Relative_Container
 * @ingroup Elementary
 *
 * @image html relative_container_inheritance_tree.png
 * @image latex relative_container_inheritance_tree.eps
 *
 * A relative container calculates the size and position of all the children
 * based on their relationship to each other.
 *
 * By default, corner of child are same to its parent corner, meaning default
 * values of the edge are:
 * @code
 * elm_relative_container_relation_left_set(layout, child, layout, 0.0);
 * elm_relative_container_relation_top_set(layout, child, layout, 0.0);
 * elm_relative_container_relation_right_set(layout, child, layout, 1.0);
 * elm_relative_container_relation_bottom_set(layout, child, layout, 1.0);
 * @endcode
 *
 * The following are examples of how to use a relative_container:
 * @li @ref tutorial_relative_container_01
 * @li @ref tutorial_relative_container_02
 */

/**
 * @addtogroup Elm_Relative_Container_Group
 * @{
 */

#ifndef EFL_NOLEGACY_API_SUPPORT
#include "elm_relative_container_legacy.h"
#endif
/**
 * @}
 */
