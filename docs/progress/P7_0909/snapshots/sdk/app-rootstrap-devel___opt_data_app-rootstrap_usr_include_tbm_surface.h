/**************************************************************************

libtbm

Copyright 2014 Samsung Electronics co., Ltd. All Rights Reserved.

Contact: SooChan Lim <sc1.lim@samsung.com>, Sangjin Lee <lsj119@samsung.com>
Inpyo Kang <mantiger@samsung.com>, Dongyeon Kim <dy5.kim@samsung.com>
Boram Park <boram1288.park@samsung.com>, Changyeon Lee <cyeon.lee@samsung.com>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

#ifndef _TBM_SURFACE_H_
#define _TBM_SURFACE_H_

/**
 * @addtogroup CAPI_UI_TBM_SURFACE_MODULE
 * @{
 */

#include <tbm_type.h>

/**
 * \file tbm_surface.h
 * \brief TBM Surface
 */

/**
 * @brief Enumeration for tbm_surface error type.
 * @since_tizen 2.3
 */
typedef enum {
	TBM_SURFACE_ERROR_NONE = TIZEN_ERROR_NONE,							  /**< Successful */
	TBM_SURFACE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,  /**< Invalid parameter */
	TBM_SURFACE_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,  /**< Invalid Operation */
} tbm_surface_error_e;

/**
 * @brief Definition for the max number of TBM surface plane.
 * @since_tizen 2.3
 */
#define TBM_SURF_PLANE_MAX 4

/* option to map the tbm_surface */
/**
 * @brief Definition for the access option to read.
 * @since_tizen 2.3
 */
#define TBM_SURF_OPTION_READ      (1 << 0)
/**
 * @brief Definition for the access option to write.
 * @since_tizen 2.3
 */
#define TBM_SURF_OPTION_WRITE     (1 << 1)

/**
 * @brief Definition for the TBM plane struct.
 * @since_tizen 2.3
 */
typedef struct _tbm_surface_plane {
	unsigned char *ptr;	  /**< Plane pointer */
	uint32_t size;		  /**< Plane size */
	uint32_t offset;	  /**< Plane offset */
	uint32_t stride;	  /**< Plane stride */

	void *reserved1;	  /**< Reserved pointer1 */
	void *reserved2;	  /**< Reserved pointer2 */
	void *reserved3;	  /**< Reserved pointer3 */
} tbm_surface_plane_s;

/**
 * @brief Definition for the TBM surface information struct.
 * @since_tizen 2.3
 */
typedef struct _tbm_surface_info {
	uint32_t width;		 /**< TBM surface width */
	uint32_t height;	 /**< TBM surface height */
	tbm_format format;	 /**< TBM surface format*/
	uint32_t bpp;		 /**< TBM surface bbp */
	uint32_t size;		 /**< TBM surface size */

	uint32_t num_planes;							/**< The number of planes */
	tbm_surface_plane_s planes[TBM_SURF_PLANE_MAX];	/**< Array of planes */

	void *reserved4;   /**< Reserved pointer4 */
	void *reserved5;   /**< Reserved pointer5 */
	void *reserved6;   /**< Reserved pointer6 */
} tbm_surface_info_s;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Queries surface format list and number of format supported by the system.
 * @since_tizen 2.3
 *
 * @remarks You must release the formats using free().
 *
 * @param[out] formats  The format array which the system can support \n
 *                      This pointer has to be freed by user.
 * @param[out] num      The number of formats
 *
 * @return  #TBM_SURFACE_ERROR_NONE if this function succeeds,
 *          otherwise an error status value
 *
 * @retval #TBM_SURFACE_ERROR_NONE               Success
 * @retval #TBM_SURFACE_ERROR_INVALID_OPERATION  Invalid operation
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   uint32_t *formats;
   uint32_t format_num;
   int ret, i;
   tbm_surface_error_e tse;

   tse = tbm_surface_query_formats (&formats, &format_num))

   for( i = 0 ; i < format_num ; i++)
   {
       if (formats[i] == TBM_FORMAT_RGB332)
       {
   ....

   free (formats);
   @endcode
 */
int tbm_surface_query_formats(uint32_t **formats, uint32_t *num);

/**
 * @brief Creates the tbm_surface.
 * @details This function creates the tbm_surface with the given width, height, and format.
 *
 * @since_tizen 2.3
 *
 * @remark The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 *
 * @param[in] width   The width of surface
 * @param[in] height  The height of surface
 * @param[in] format  The format of surface
 *
 * @return   #tbm_surface_h on success,
 *           otherwise @c NULL
 *
 * @retval #tbm_surface_h  The TBM surface handle
 *
 * @exception #TBM_SURFACE_ERROR_NONE               Success
 * @exception #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @exception #TBM_SURFACE_ERROR_INVALID_OPERATION  Invalid operation
 *
 * @see tbm_surface_destroy()
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_h surface;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);

   ...

   tbm_surface_destroy (surface);
   @endcode
 */
tbm_surface_h tbm_surface_create(int width, int height, tbm_format format);

/**
 * @brief Destroys the tbm_surface.
 * @since_tizen 2.3
 *
 * @param[in] surface  The #tbm_surface_h
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 *
 * @retval #TBM_SURFACE_ERROR_NONE               Success
 * @retval #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see tbm_surface_create()
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_h surface;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);

   ...

   tbm_surface_destroy (surface);
   @endcode
 */
int tbm_surface_destroy(tbm_surface_h surface);

/**
 * @brief Maps the tbm_surface according to the access option.
 * @details After mapping tbm_surface, the information of tbm_surface is assigned in #tbm_surface_info_s struct. \n
 *          The information of tbm_surface has width, height, format, bpp, size, number of planes and information of planes. \n
 *          The information of planes has stride, offset, size and pointer of plane. \n
 *          #TBM_SURF_OPTION_READ indicates access option to read. \n
 *          #TBM_SURF_OPTION_WRITE indicates access option to write.
 *
 * @since_tizen 2.3
 *
 * @param[in]  surface  The #tbm_surface_h
 * @param[in]  opt      The option to access the tbm_surface
 * @param[out] info     The information of the tbm_surface
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 *
 * @retval #TBM_SURFACE_ERROR_NONE               Success
 * @retval #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #TBM_SURFACE_ERROR_INVALID_OPERATION  Invalid operation
 *
 * @see tbm_surface_unmap();
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_h surface;
   tbm_surface_info_s info;
   int ret;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);
   ret = tbm_surface_map (surface, TBM_SURF_OPTION_WRITE|TBM_SURF_OPTION_READ, &info);

   ...

   tbm_surface_unmap (surface);
   tbm_surface_destroy (surface);
   @endcode
 */
int tbm_surface_map(tbm_surface_h surface, int opt, tbm_surface_info_s *info);

/**
 * @brief Unmaps the tbm_surface.
 * @since_tizen 2.3
 *
 * @param[in] surface  The #tbm_surface_h
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 *
 * @retval #TBM_SURFACE_ERROR_NONE               Success
 * @retval #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see tbm_surface_map()
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_h surface;
   tbm_surface_info_s info;
   int ret;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);
   ret = tbm_surface_map (surface, TBM_SURF_OPTION_WRITE|TBM_SURF_OPTION_READ, &info);

   ...

   tbm_surface_unmap (surface);
   tbm_surface_destroy (surface);
   @endcode
 */
int tbm_surface_unmap(tbm_surface_h surface);

/**
 * @brief Gets the information of the tbm_surface.
 * @details The information of tbm_surface is assigned in #tbm_surface_info_s struct. \n
 *          The information of tbm_surface has width, height, format, bpp, size, number of planes and information of planes. \n
 *          The information of planes has stride, offset, size and pointer of plane.
 *
 * @since_tizen 2.3
 *
 * @param[in]   surface  The #tbm_surface_h
 * @param[out]  info     The information of the tbm_surface
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 *
 * @retval #TBM_SURFACE_ERROR_NONE               Success
 * @retval #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #TBM_SURFACE_ERROR_INVALID_OPERATION  Invalid operation
 *
 * @see tbm_surface_map()
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_h surface;
   tbm_surface_info_s info;
   int ret;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);
   ret = tbm_surface_get_info (surface, &info);

   ...

   tbm_surface_destroy (surface);
   @endcode
 */
int tbm_surface_get_info(tbm_surface_h surface, tbm_surface_info_s *info);

/**
 * @brief Gets the width of the tbm_surface.
 * @since_tizen 2.3
 *
 * @param[in] surface  The #tbm_surface_h
 *
 * @return  The width of the tbm_surface on success,
 *          otherwise an error status value
 *
 * @retval #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_h surface;
   int width;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);

   ...

   width = tbm_surface_get_width (surface);

   ...

   tbm_surface_destroy (surface);
   @endcode
 */
int tbm_surface_get_width(tbm_surface_h surface);

/**
 * @brief Gets the height of the tbm_surface.
 * @since_tizen 2.3
 *
 * @param[in] surface  The #tbm_surface_h
 *
 * @return  The height of the tbm_surface if this function succeeds,
 *          otherwise an error status value
 *
 * @retval #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_h surface;
   int height;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);

   ...

   height = tbm_surface_get_height (surface);

   ...

   tbm_surface_destroy (surface);
   @endcode
 */
int tbm_surface_get_height(tbm_surface_h surface);

/**
 * @brief Gets the format of the tbm_surface.
 * @since_tizen 2.3
 *
 * @remark The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 *
 * @param[in] surface  The #tbm_surface_h
 *
 * @return  The format of the tbm_surface on success,
 *          otherwise @c 0 on failure
 *
 * @retval #tbm_format  The format of surface
 *
 * @exception #TBM_SURFACE_ERROR_NONE               Success
 * @exception #TBM_SURFACE_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @par Example
   @code
   #include <tbm_surface.h>

   tbm_surface_s surface;
   tbm_format format;

   surface = tbm_surface_create (128, 128, TBM_FORMAT_RGB332);

   ...

   format = tbm_surface_get_format (surface);

   ...

   tbm_surface_destroy (surface);
   @endcode
 */
tbm_format tbm_surface_get_format(tbm_surface_h surface);

#ifdef __cplusplus
}
#endif
/**
* @}
*/
#endif							/* _TBM_SURFACE_H_ */
