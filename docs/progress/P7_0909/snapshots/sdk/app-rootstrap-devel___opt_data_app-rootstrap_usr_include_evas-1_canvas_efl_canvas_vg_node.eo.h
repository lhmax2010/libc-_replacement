#ifndef _EFL_CANVAS_VG_NODE_EO_H_
#define _EFL_CANVAS_VG_NODE_EO_H_

#ifndef _EFL_CANVAS_VG_NODE_EO_CLASS_TYPE
#define _EFL_CANVAS_VG_NODE_EO_CLASS_TYPE

typedef Eo Efl_Canvas_Vg_Node;

#endif

#ifndef _EFL_CANVAS_VG_NODE_EO_TYPES
#define _EFL_CANVAS_VG_NODE_EO_TYPES


#endif
#ifdef EFL_BETA_API_SUPPORT
/** Efl vector graphics abstract class
 *
 * @ingroup Efl_Canvas_Vg_Node
 */
#define EFL_CANVAS_VG_NODE_CLASS efl_canvas_vg_node_class_get()

EWAPI const Efl_Class *efl_canvas_vg_node_class_get(void) EINA_CONST;

/**
 * @brief The transformation matrix, the width and the height of the node
 * object in case it was cached.
 *
 * @note Pass @c null to cancel the applied transformation matrix.
 *
 * @param[in] obj The object.
 * @param[in] m Transformation matrix.
 * @param[in] w Width of the node object.
 * @param[in] h Height of the node object.
 *
 * @ingroup Efl_Canvas_Vg_Node
 */
EOAPI void efl_canvas_vg_node_geometric_data_set(Eo *obj, const Eina_Matrix3 *m, double w, double h);

/**
 * @brief The transformation matrix to be used for this node object.
 *
 * @note Pass @c null to cancel the applied transformation.
 *
 * @param[in] obj The object.
 * @param[in] m Transformation matrix.
 *
 * @ingroup Efl_Canvas_Vg_Node
 */
EOAPI void efl_canvas_vg_node_transformation_set(Eo *obj, const Eina_Matrix3 *m);

/**
 * @brief The transformation matrix to be used for this node object.
 *
 * @note Pass @c null to cancel the applied transformation.
 *
 * @param[in] obj The object.
 *
 * @return Transformation matrix.
 *
 * @ingroup Efl_Canvas_Vg_Node
 */
EOAPI const Eina_Matrix3 *efl_canvas_vg_node_transformation_get(const Eo *obj);

/**
 * @brief The origin position of the node object.
 *
 * This origin position affects node transformation.
 *
 * @param[in] obj The object.
 * @param[in] x @c origin x position.
 * @param[in] y @c origin y position.
 *
 * @ingroup Efl_Canvas_Vg_Node
 */
EOAPI void efl_canvas_vg_node_origin_set(Eo *obj, double x, double y);

/**
 * @brief The origin position of the node object.
 *
 * This origin position affects node transformation.
 *
 * @param[in] obj The object.
 * @param[out] x @c origin x position.
 * @param[out] y @c origin y position.
 *
 * @ingroup Efl_Canvas_Vg_Node
 */
EOAPI void efl_canvas_vg_node_origin_get(const Eo *obj, double *x, double *y);

/**
 * @brief Set a composite target node to this node object.
 *
 * @param[in] obj The object.
 * @param[in] target Composite target node
 * @param[in] method Composite Method.
 *
 * @ingroup Efl_Canvas_Vg_Node
 */
EOAPI void efl_canvas_vg_node_comp_method_set(Eo *obj, Efl_Canvas_Vg_Node *target, Efl_Gfx_Vg_Composite_Method method);
#endif /* EFL_BETA_API_SUPPORT */

#endif
