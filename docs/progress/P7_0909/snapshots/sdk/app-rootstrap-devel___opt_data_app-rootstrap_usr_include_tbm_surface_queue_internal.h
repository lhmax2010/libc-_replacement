/**************************************************************************

libtbm

Copyright 2014 Samsung Electronics co., Ltd. All Rights Reserved.

Contact: SooChan Lim <sc1.lim@samsung.com>, Sangjin Lee <lsj119@samsung.com>
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

#ifndef _TBM_SURFACE_QUEUE_INTERNAL_H_
#define _TBM_SURFACE_QUEUE_INTERNAL_H_

#include <tbm_surface.h>
#include <tbm_surface_queue.h>
#include <tbm_type_common.h>
#include <tbm_error.h>

typedef tbm_surface_h (*tbm_surface_alloc_cb)(tbm_surface_queue_h surface_queue,
		void *data);

typedef tbm_surface_h (*tbm_surface_alloc_cb2)(tbm_surface_queue_h surface_queue,
		int width, int height, int format, int flags, void *data);

typedef void (*tbm_surface_free_cb)(tbm_surface_queue_h surface_queue,
		void *data, tbm_surface_h surface);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sets allocate callback to the tbm_surface_queue.
 *        alloc_cb is called when the tbm_surface_queue try to allocate the tbm_surface and
 *        the tbm_surface_queue dequeue the tbm_surface returned in alloc_cb
 *        free_cb is called when the tbm_surface_queue unreference the tbm_surface.
 *
 * @remarks You must use either tbm_surface_queue_set_alloc_cb or tbm_surface_queue_set_alloc_cb2
 *
 * @param[in] surface_queue  The #tbm_surface_queue_h
 * @param[in] alloc_cb       The callback of allocate
 * @param[in] free_cb        The callback of free
 * @param[in] data           The user data
 *
 * @return  #TBM_SURFACE_QUEUE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_set_alloc_cb(
	tbm_surface_queue_h surface_queue,
	tbm_surface_alloc_cb alloc_cb,
	tbm_surface_free_cb free_cb,
	void *data);

/**
 * @brief Sets allocate callback to the tbm_surface_queue.
 *        alloc_cb2 is called when the tbm_surface_queue try to allocate the tbm_surface and
 *        the tbm_surface_queue dequeue the tbm_surface returned in alloc_cb2
 *        free_cb is called when the tbm_surface_queue unreference the tbm_surface.
 *
 * @remarks You must use either tbm_surface_queue_set_alloc_cb or tbm_surface_queue_set_alloc_cb2
 *
 * @param[in] surface_queue  The #tbm_surface_queue_h
 * @param[in] alloc_cb2      The callback of allocate
 * @param[in] free_cb        The callback of free
 * @param[in] data           The user data
 *
 * @return  #TBM_SURFACE_QUEUE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_set_alloc_cb2(
	tbm_surface_queue_h surface_queue,
	tbm_surface_alloc_cb2 alloc_cb2,
	tbm_surface_free_cb free_cb,
	void *data);

tbm_surface_queue_error_e tbm_surface_queue_get_acquirable_surfaces(
	tbm_surface_queue_h surface_queue,
	tbm_surface_h *surfaces, int *num);

/**
 * @brief Sets size of the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  queue_size     The size of the tbm_surface_queue
 * @param[in]  flush          flush the tbm_surface_queue
  *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 *
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;
   tbm_surface_queue_error_e tsq_err = TBM_SURFACE_QUEUE_ERROR_NONE;

   ...

   tsq_err = tbm_surface_queue_set_size(surface_queue, 2, 1);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
   ...
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_set_size(
		tbm_surface_queue_h surface_queue, int queue_size, int flush);

/**
 * @brief Notify reset of the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_notify_reset(tbm_surface_queue_h surface_queue);

/**
 * @brief Notify dequeuable of the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_notify_dequeuable(tbm_surface_queue_h surface_queue);

/**
 * @brief Add callback of can_dequeue to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  can_dequeue_cb The callback of can_dequeue
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_add_can_dequeue_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb can_dequeue_cb,
	void *data);

/**
 * @brief Remove callback of can_dequeue from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  can_dequeue_cb The callback of can_dequeue
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_remove_can_dequeue_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb can_dequeue_cb,
	void *data);

/**
 * @brief Get Next Dequeue the tbm_surface from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[out] surface        The #tbm_surface_h
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_get_next_dequeue(
		tbm_surface_queue_h surface_queue, tbm_surface_h *surface);

#ifdef __cplusplus
}
#endif
#endif /* _TBM_SURFACE_INTERNAL_H_ */
