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

#ifndef _TBM_SURFACE_QUEUE_H_
#define _TBM_SURFACE_QUEUE_H_

#include <tbm_surface.h>
#include <tbm_type_common.h>
#include <tbm_error.h>

typedef enum {
	TBM_SURFACE_QUEUE_TRACE_NONE = 0,
	TBM_SURFACE_QUEUE_TRACE_DEQUEUE = 1,
	TBM_SURFACE_QUEUE_TRACE_ENQUEUE = 2,
	TBM_SURFACE_QUEUE_TRACE_ACQUIRE = 3,
	TBM_SURFACE_QUEUE_TRACE_RELEASE = 4,
	TBM_SURFACE_QUEUE_TRACE_CANCEL_DEQUEUE = 5,
	TBM_SURFACE_QUEUE_TRACE_CANCEL_ACQUIRE = 6,
} tbm_surface_queue_trace;

typedef enum {
	TBM_SURFACE_QUEUE_MODE_NONE = 0,
	/**
	 * GUARANTEE_CYCLE mode must do enqueue/aquire/release or cancel_dequeue
	 * for the tbm_surface which is dequeued before tbm_surface_queue is reset.
	 */
	TBM_SURFACE_QUEUE_MODE_GUARANTEE_CYCLE = (1 << 0),
} tbm_surface_queue_mode;

typedef struct _tbm_surface_queue *tbm_surface_queue_h;

typedef void (*tbm_surface_queue_notify_cb)(tbm_surface_queue_h surface_queue,
		void *data);

typedef void (*tbm_surface_queue_trace_cb)(tbm_surface_queue_h surface_queue,
		tbm_surface_h surface, tbm_surface_queue_trace trace, void *data);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates the tbm_surface_queue_h.
 * @details This function creates the tbm_surfac_queue_h with the given queue_size, width, height, format, flags.
 *
 * @param[in] queue_size   The size of the tbm_surface_queue
 * @param[in] width        The width of the tbm_surface_queue
 * @param[in] height       The height of the tbm_surface_queue
 * @param[in] format       The format of the tbm_surface_queue
 * @param[in] flags        The flags of the tbm_surface_queue
 *
 * @return   #tbm_surface_queue_h on success,
 *           otherwise @c NULL
 *
 * @see tbm_surface_queue_destroy()
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;

   surface_queue = tbm_surface_queue_create(3, 128, 128, TBM_FORMAT_ARGB888, TBM_BO_DEFAULT);

   ...

   tbm_surface_queue_destroy (surface_queue);
   @endcode
 */
tbm_surface_queue_h tbm_surface_queue_create(int queue_size, int width,
		int height, int format, int flags);

/**
 * @brief Creates the sequence tbm_surface_queue_h.
 * @details This function creates the sequence tbm_surface_queue_h with the given queue_size, width, height, format, flags.
 *
 * @param[in] queue_size   The size of the tbm_surface_queue
 * @param[in] width        The width of the tbm_surface_queue
 * @param[in] height       The height of the tbm_surface_queue
 * @param[in] format       The format of the tbm_surface_queue
 * @param[in] flags        The flags of the tbm_surface_queue
 *
 * @return   #tbm_surface_queue_h on success,
 *           otherwise @c NULL
 *
 * @see tbm_surface_queue_destroy()
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;

   surface_queue = tbm_surface_queue_sequence_create(3, 128, 128, TBM_FORMAT_ARGB888, TBM_BO_DEFAULT);

   ...

   tbm_surface_queue_destroy (surface_queue);
   @endcode
 */
tbm_surface_queue_h tbm_surface_queue_sequence_create(int queue_size, int width,
		int height, int format, int flags);

/**
 * @brief Destroys the tbm_surface_queue.
 *
 * @param[in] surface_queue  The #tbm_surface_queue_h
 *
 * @return  #TBM_SURFACE_QUEUE_ERROR_NONE on success,
 *          otherwise an error status value
 *
 * @see tbm_surface_queue_create()
 * @see tbm_surface_queue_sequence_create()
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;

   surface_queue = tbm_surface_queue_create(3, 128, 128, TBM_FORMAT_ARGB888, TBM_BO_DEFAULT);

   ...

   tbm_surface_queue_destroy(surface_queue);
   @endcode
 */
void tbm_surface_queue_destroy(tbm_surface_queue_h surface_queue);

/**
 * @brief Gets the width of the tbm_surface_queue.
 *
 * @param[in] surface_queue  The #tbm_surface_queue_h
 *
 * @return  The width of the tbm_surface_queue on success,
 *          otherwise an error status value
 */
int tbm_surface_queue_get_width(tbm_surface_queue_h surface_queue);

/**
 * @brief Gets the height of the tbm_surface_queue.
 *
 * @param[in] surface_queue  The #tbm_surface_queue_h
 *
 * @return  The height of the tbm_surface_queue on success,
 *          otherwise an error status value
 */
int tbm_surface_queue_get_height(tbm_surface_queue_h surface_queue);

/**
 * @brief Gets the format of the tbm_surface_queue.
 *
 * @param[in] surface_queue  The #tbm_surface_queue_h
 *
 * @return  The format of the tbm_surface_queue on success,
 *          otherwise an error status value
 */
int tbm_surface_queue_get_format(tbm_surface_queue_h surface_queue);

/**
 * @brief Gets the size of the tbm_surface_queue.
 *
 * @param[in] surface_queue  The #tbm_surface_queue_h
 *
 * @return  The size of the tbm_surface_queue on success,
 *          otherwise an error status value
 */
int tbm_surface_queue_get_size(tbm_surface_queue_h surface_queue);

/**
 * @brief Gets the tbm_surfaces used by tbm_surface_queue.
 *        Return only surfaces are allocated and current used by the tbm_surface_queue.
 *        the tbm_surface_queue allocate surface when can_dequeue or dequeue.
 *        if the tbm_surface_queue allocate 2 surface even if queue size is 3,
 *        this function will return 2 surfaces.
 *        if the tbm_surface_queue is flushed, this function will return none surfaces.
 *
 * @remarks You must release the surfaces using free().
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[out] surfaces       The array of the tbm_surfaces
 * @param[out] num            The number of the array
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 *
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;
   tbm_surface_h *surfaces;
   int num;
   tbm_surface_queue_error_e tsq_err = TBM_SURFACE_QUEUE_ERROR_NONE;

   ...

   tsq_err = tbm_surface_queue_get_surfaces(surface_queue, surfaces, &num);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
   ...
   }

   ...

   free(surfaces);

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_get_surfaces(
	tbm_surface_queue_h surface_queue,
	tbm_surface_h *surfaces, int *num);

/**
 * @brief Sets modes of the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  modes          modes of The #tbm_surface_queue_mode
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

   surface_queue = tbm_surface_queue_create(3, 128, 128, TBM_FORMAT_ARGB888, TBM_BO_DEFAULT);

   ...

   tsq_err = tbm_surface_queue_set_modes(surface_queue, TBM_SURFACE_QUEUE_MODE_GUARANTEE_CYCLE);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
   ...
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_set_modes(
			tbm_surface_queue_h surface_queue, int modes);

/**
 * @brief Check the tbm_surface_queue can dequeue the tbm_surface.
 *        if wait is 1, this function isn't return until another thread release
 *        the tbm_surface to the tbm_surface_queue
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  wait           wait releasing the tbm_surface
 *
 * @return 1 if it can it, otherwise 0.
 *
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;

   ...

   if (tbm_surface_queue_can_dequeue(surface_queue, 0)) {

   }

   ...

   @endcode
 */
int tbm_surface_queue_can_dequeue(tbm_surface_queue_h surface_queue, int wait);

/**
 * @brief Check the tbm_surface_queue can dequeue the tbm_surface.
 *        this function wait for ms_timeout untill another thread release
 *        the tbm_surface to the tbm_surface_queue
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  ms_timeout     milisecond time for waiting releasing the tbm_surface
 *
 * @return #TBM_SURFACE_QUEUE_ERROR_NONE if it can it,
 *         otherwise an error status value
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;
   tbm_surface_queue_error_e err;

   ...

   err = tbm_surface_queue_can_dequeue_wait_timeout(surface_queue, 10000);
   if (err == TBM_SURFACE_QUEUE_ERROR_NONE) {

   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_can_dequeue_wait_timeout(
        tbm_surface_queue_h surface_queue, int ms_timeout);

/**
 * @brief Check the tbm_surface_queue can acquire the tbm_surface.
 *        if wait is 1, this function isn't return until another thread enqueue
 *        the tbm_surface to the tbm_surface_queue
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  wait           wait enqueueing the tbm_surface
 *
 * @return 1 if it can it, otherwise 0.
 *
 *
 * @par Example
   @code
   #include <tbm_surface_queue.h>

   tbm_surface_queue_h surface_queue;

   ...

   if (tbm_surface_queue_can_acquire(surface_queue, 0)) {

   }

   ...

   @endcode
 */
int tbm_surface_queue_can_acquire(tbm_surface_queue_h surface_queue, int wait);

/**
 * @brief Dequeue the tbm_surface from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[out] surface        The #tbm_surface_h
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
   tbm_surface_h surface;

   ...

   if (tbm_surface_queue_can_dequeue(surface_queue, 0)) {
      tsq_err = tbm_surface_queue_dequeue(surface_queue, &surface);
      if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
          ...
      }
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_dequeue(
		tbm_surface_queue_h surface_queue, tbm_surface_h *surface);

/**
 * @brief Enqueue the tbm_surface to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  surface        The #tbm_surface_h
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
   tbm_surface_h surface;

   if (tbm_surface_queue_can_dequeue(surface_queue, 0)) {
      tsq_err = tbm_surface_queue_dequeue(surface_queue, &surface);
      if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
          ...
      }
   } else {
       ...
   }

   ...

   tsq_err = tbm_surface_queue_enqueue(surface_queue, surface);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
       ...
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_enqueue(
		tbm_surface_queue_h surface_queue, tbm_surface_h surface);

/**
 * @brief Acquire the tbm_surface from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[out] surface        The #tbm_surface_h
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
   tbm_surface_h surface, surface2;

   if (tbm_surface_queue_can_dequeue(surface_queue, 0)) {
      tsq_err = tbm_surface_queue_dequeue(surface_queue, &surface);
      if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
          ...
      }
   } else {
       ...
   }

   ...

   tsq_err = tbm_surface_queue_enqueue(surface_queue, surface);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
       ...
   }

   ...

   if (tbm_surface_queue_can_acquire(surface_queue, 0)) {
      tsq_err = tbm_surface_queue_acquire(surface_queue, &surface2)
      if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
          ...
      }
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_acquire(
		tbm_surface_queue_h surface_queue, tbm_surface_h *surface);

/**
 * @brief Release the tbm_surface to the tbm_surface_queue.
 *        the tbm_surface should be released when the tbm_surface can be dequeued and rendered.
 *        if the tbm_surface is released even though consumer is using it, it cause issue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  surface        The #tbm_surface_h
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
   tbm_surface_h surface, surface2;

   if (tbm_surface_queue_can_dequeue(surface_queue, 0)) {
      tsq_err = tbm_surface_queue_dequeue(surface_queue, &surface);
      if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
          ...
      }
   } else {
       ...
   }

   ...

   tsq_err = tbm_surface_queue_enqueue(surface_queue, surface);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
       ...
   }

   ...

   if (tbm_surface_queue_can_acquire(surface_queue, 0)) {
       tsq_err = tbm_surface_queue_acquire(surface_queue, &surface2)
       if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
           ...
       }
   } else {
       ...
   }
   ...

   tsq_err = tbm_surface_queue_release(surface_queue, surface2)
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
       ...
   }

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_release(
		tbm_surface_queue_h surface_queue, tbm_surface_h surface);

/**
 * @brief Cancel dequeue the tbm_surface to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  surface        The #tbm_surface_h
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
   tbm_surface_h surface;

   if (tbm_surface_queue_can_dequeue(surface_queue, 0)) {
      tsq_err = tbm_surface_queue_dequeue(surface_queue, &surface);
      if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
          ...
      }
   } else {
       ...
   }

   tsq_err = tbm_surface_queue_cancel_dequeue(surface_queue, surface);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
       ...
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_cancel_dequeue(
		tbm_surface_queue_h surface_queue, tbm_surface_h surface);

/**
 * @brief Cancel acquire the tbm_surface to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  surface        The #tbm_surface_h
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
   tbm_surface_h surface;

   if (tbm_surface_queue_can_acquire(surface_queue, 0)) {
       tsq_err = tbm_surface_queue_acquire(surface_queue, &surface);
       if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
           ...
      }
   } else {
       ...
   }

   tsq_err = tbm_surface_queue_cancel_acquire(surface_queue, surface);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
       ...
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_cancel_acquire(
		tbm_surface_queue_h surface_queue, tbm_surface_h surface);

/**
 * @brief Reset the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  width          The width of the tbm_surface_queue
 * @param[in]  height         The height of the tbm_surface_queue
 * @param[in]  format         The format of the tbm_surface_queue
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

   tsq_err = tbm_surface_queue_reset(surface_queue, 256,256, TBM_FORMAT_YUV420);
   if (tsq_err != TBM_SURFACE_QUEUE_ERROR_NONE) {
       ...
   }

   ...

   @endcode
 */
tbm_surface_queue_error_e tbm_surface_queue_reset(
		tbm_surface_queue_h surface_queue, int width, int height, int format);

/**
 * @brief Flush the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_flush(tbm_surface_queue_h surface_queue);

/**
 * @brief Free Flush the tbm_surface_queue.
 *        only the released tbm_surface is flushed.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_free_flush(tbm_surface_queue_h surface_queue);

/**
 * @brief Add callback of destroy to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  destroy_cb     The callback of destroy
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_add_destroy_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb destroy_cb,
	void *data);

/**
 * @brief Remove callback of destroy from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  destroy_cb     The callback of destroy
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_remove_destroy_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb destroy_cb,
	void *data);

/**
 * @brief Add callback of reset to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  reset_cb       The callback of reset
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_add_reset_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb reset_cb,
	void *data);

/**
 * @brief Remove callback of reset from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  reset_cb       The callback of reset
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_remove_reset_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb reset_cb,
	void *data);

/**
 * @brief Add callback of dequeuable to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  dequeuable_cb  The callback of dequeuable
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_add_dequeuable_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb dequeuable_cb,
	void *data);

/**
 * @brief Remove callback of dequeuable from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  dequeuable_cb  The callback of dequeuable
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_remove_dequeuable_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb dequeuable_cb,
	void *data);

/**
 * @brief Add callback of dequeue to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  dequeue_cb     The callback of dequeue
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_add_dequeue_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb dequeue_cb,
	void *data);

/**
 * @brief Remove callback of dequeue from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  dequeue_cb     The callback of dequeue
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_remove_dequeue_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb dequeue_cb,
	void *data);

/**
 * @brief Add callback of acquirable to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  acquirable_cb  The callback of acquirable
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_add_acquirable_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb acquirable_cb,
	void *data);

/**
 * @brief Remove callback of acquirable form the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  acquirable_cb  The callback of acquirable
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_remove_acquirable_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_notify_cb acquirable_cb,
	void *data);

/**
 * @brief Add callback of trace to the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  trace_cb       The callback of trace
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_add_trace_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_trace_cb trace_cb,
	void *data);

/**
 * @brief Remove callback of trace from the tbm_surface_queue.
 *
 * @param[in]  surface_queue  The #tbm_surface_queue_h
 * @param[in]  trace_cb       The callback of trace
 * @param[in]  data           The user data
 *
 * @return  #TBM_SURFACE_ERROR_NONE on success,
 *          otherwise an error status value
 */
tbm_surface_queue_error_e tbm_surface_queue_remove_trace_cb(
	tbm_surface_queue_h surface_queue, tbm_surface_queue_trace_cb trace_cb,
	void *data);

/* temporary */

typedef tbm_surface_h (*tbm_surface_alloc_cb2)(tbm_surface_queue_h surface_queue,
	int width, int height, int format, int flags, void *data);

typedef void (*tbm_surface_free_cb)(tbm_surface_queue_h surface_queue,
	void *data, tbm_surface_h surface);

tbm_surface_queue_error_e tbm_surface_queue_set_alloc_cb2(
	tbm_surface_queue_h surface_queue,
	tbm_surface_alloc_cb2 alloc_cb2,
	tbm_surface_free_cb free_cb,
	void *data);
#ifdef __cplusplus
}
#endif
#endif							/* _TBM_SURFACE_H_ */
