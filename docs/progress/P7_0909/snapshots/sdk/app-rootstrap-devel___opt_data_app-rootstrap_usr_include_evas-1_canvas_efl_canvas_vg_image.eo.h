#ifndef _EFL_CANVAS_VG_IMAGE_EO_H_
#define _EFL_CANVAS_VG_IMAGE_EO_H_

#ifndef _EFL_CANVAS_VG_IMAGE_EO_CLASS_TYPE
#define _EFL_CANVAS_VG_IMAGE_EO_CLASS_TYPE

typedef Eo Efl_Canvas_Vg_Image;

#endif

#ifndef _EFL_CANVAS_VG_IMAGE_EO_TYPES
#define _EFL_CANVAS_VG_IMAGE_EO_TYPES


#endif
#ifdef EFL_BETA_API_SUPPORT
/** Efl vector graphics image class
 *
 * @ingroup Efl_Canvas_Vg_Image
 */
#define EFL_CANVAS_VG_IMAGE_CLASS efl_canvas_vg_image_class_get()

EWAPI const Efl_Class *efl_canvas_vg_image_class_get(void) EINA_CONST;

/**
 * @brief Set image data
 *
 * @param[in] obj The object.
 * @param[in] pixels Image pixels data. The pixel data type is 32bit RGBA
 * @param[in] size The size in pixels.
 *
 * @ingroup Efl_Canvas_Vg_Image
 */
EOAPI void efl_canvas_vg_image_data_set(Eo *obj, void *pixels, Eina_Size2D size);

/**
 * @brief Set path for file to be loaded
 *
 * @param[in] obj The object.
 * @param[in] path Path for image to be loaded
 *
 * @return @c true on success, @c false otherwise
 *
 * @ingroup Efl_Canvas_Vg_Image
 */
EOAPI Eina_Bool efl_canvas_vg_image_file_set(Eo *obj, const char *path);

/**
 * @brief Set image data passing data and size
 *
 * @param[in] obj The object.
 * @param[in] data Data
 * @param[in] size Size of data
 * @param[in] mimetype Mimetype of data
 *
 * @return @c true on success, @c false otherwise
 *
 * @ingroup Efl_Canvas_Vg_Image
 */
EOAPI Eina_Bool efl_canvas_vg_image_memfile_set(Eo *obj, const char *data, uint32_t size, const char *mimetype);

/**
 * @brief Get viewbox for loaded
 *
 * @param[in] obj The object.
 *
 * @return The function returns viewbox coordinates and size for given paint: x
 * y w h
 *
 * @ingroup Efl_Canvas_Vg_Image
 */
EOAPI Eina_Rect efl_canvas_vg_image_viewbox_get(const Eo *obj);
#endif /* EFL_BETA_API_SUPPORT */

#endif
