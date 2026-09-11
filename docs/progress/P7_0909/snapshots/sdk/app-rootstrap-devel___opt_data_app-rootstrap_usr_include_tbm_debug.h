/**************************************************************************

libtbm

Copyright 2012 Samsung Electronics co., Ltd. All Rights Reserved.

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

#ifndef _TBM_DEBUG_H_
#define _TBM_DEBUG_H_

#include <tbm_type.h>
#include <tbm_type_common.h>
#include <tbm_bo.h>
#include <tbm_error.h>

/**
 * \file tbm_debug.h
 */

/**
 * @brief Enumeration of the trace log for debug
 * @since_tizen 5.0
 */
typedef enum {
	TBM_BUFGMR_DEBUG_TRACE_NONE             = 0,
	TBM_BUFGMR_DEBUG_TRACE_BO               = (1 << 0),
	TBM_BUFGMR_DEBUG_TRACE_SURFACE_INTERNAL = (1 << 1),
	TBM_BUFGMR_DEBUG_TRACE_SURFACE          = (1 << 2),
	TBM_BUFGMR_DEBUG_TRACE_SURFACE_QUEUE    = (1 << 3),
} tbm_bufmgr_debug_trace_mask;

/* type to string ***********************************************************/
struct tbm_type_name {
	int type;
	const char *name;
};

#define TBM_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define TBM_TYPE_NAME_FN(res) \
static inline const char * tbm_##res##_str(int type)	\
{			\
	unsigned int i;					\
	for (i = 0; i < TBM_ARRAY_SIZE(tbm_##res##_names); i++) { \
		if (tbm_##res##_names[i].type == type)	\
			return tbm_##res##_names[i].name;	\
	}						\
	return "(invalid)";				\
}

static struct tbm_type_name tbm_error_names[] = {
	{ TBM_ERROR_NONE, "TBM_ERROR_NONE" },
	{ TBM_ERROR_OUT_OF_MEMORY, "TBM_ERROR_OUT_OF_MEMORY" },
	{ TBM_ERROR_INVALID_PARAMETER, "TBM_ERROR_INVALID_PARAMETER" },
	{ TBM_ERROR_INVALID_OPERATION, "TBM_ERROR_INVALID_OPERATION" },
	{ TBM_ERROR_NOT_SUPPORTED, "TBM_ERROR_NOT_SUPPORTED" },
	{ TBM_SURFACE_QUEUE_ERROR_EMPTY, "TBM_SURFACE_QUEUE_ERROR_EMPTY" },
	{ TBM_SURFACE_QUEUE_ERROR_INVALID_QUEUE, "TBM_SURFACE_QUEUE_ERROR_INVALID_QUEUE" },
	{ TBM_SURFACE_QUEUE_ERROR_INVALID_SURFACE, "TBM_SURFACE_QUEUE_ERROR_INVALID_SURFACE" },
	{ TBM_SURFACE_QUEUE_ERROR_SURFACE_ALLOC_FAILED, "TBM_SURFACE_QUEUE_ERROR_SURFACE_ALLOC_FAILED" },
	{ TBM_SURFACE_QUEUE_ERROR_ALREADY_EXIST, "TBM_SURFACE_QUEUE_ERROR_ALREADY_EXIST" },
	{ TBM_SURFACE_QUEUE_ERROR_UNKNOWN_SURFACE, "TBM_SURFACE_QUEUE_ERROR_UNKNOWN_SURFACE" },
};
TBM_TYPE_NAME_FN(error)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Print out the information of tbm_bos.
 * @since_tizen 3.0
 * @param[in] bufmgr : the buffer manager
 */
void tbm_bufmgr_debug_show(tbm_bufmgr bufmgr);

/**
 * @brief Get string with the information of tbm_bos.
 * @since_tizen 3.0
 * @param[in] bufmgr : the buffer manager
 * @return sting with info if this function succeeds, otherwise NULL. It has to be free by user.
 */
char * tbm_bufmgr_debug_tbm_info_get(tbm_bufmgr bufmgr);

/**
 * @brief Print out the trace of tbm_bos.
 * @since_tizen 3.0
 * @param[in] bufmgr : the buffer manager
 * @param[in] onoff : 1 is on, and 0 is off
 */
void tbm_bufmgr_debug_trace(tbm_bufmgr bufmgr, int onoff);

/**
 * @brief Dump all tbm surfaces
 * @param[in] path : the given dump path
 * @return 1 if this function succeeds, otherwise 0.
 */
int tbm_bufmgr_debug_dump_all(char *path);

/**
 * @brief Start the dump debugging for queue.
 * @since_tizen 3.0
 * @param[in] path : the given dump path
 * @param[in] count : the dump count number
 * @param[in] onoff : 1 is on, and 0 is off, if onoff==0 path and count are ignored
 * @return 1 if this function succeeds, otherwise 0.
 */
int tbm_bufmgr_debug_queue_dump(char *path, int count, int onoff);

/**
 * @brief Set scale factor for the nearest calling tbm_bufmgr_debug_dump_all() or tbm_bufmgr_debug_queue_dump()
 * @since_tizen 3.0
 * @param[in] scale : the scale factor, 0 - disable scaling
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   // Dump all surface with scale factor 0.5
   tbm_bufmgr_debug_dump_set_scale(0.5);
   tbm_bufmgr_debug_dump_all("/tmp/");

   // Start the dump debugging for queue with scale factor 0.5
   tbm_bufmgr_debug_dump_set_scale(0.2);
   tbm_bufmgr_debug_queue_dump("/tmp/", 10, 1);

   @endcode
 */
void tbm_bufmgr_debug_dump_set_scale(double scale);

/**
 * @brief Get ref_count of a global tbm_bufmgr
 * @since_tizen 5.0
 */
int tbm_bufmgr_debug_get_ref_count(void);

/**
 * @brief set or unset the trace_mask to print out the trace logs.
 * @since_tizen 5.0
 * @param[in] bufmgr : the buffer manager
 * @param[in] mask : enum value for the trace log
 * @param[in] set : set the mask when set is 1, otherwise unset the mask when set is 0
 */
void tbm_bufmgr_debug_set_trace_mask(tbm_bufmgr bufmgr, tbm_bufmgr_debug_trace_mask mask, int set);

/**
 * @brief Start the dump debugging.
 * @since_tizen 3.0
 * @param[in] path : the given dump path
 * @param[in] w : the width of dump image
 * @param[in] h : the height of dump image
 * @param[in] count : the dump count number
 * @see #tdm_helper_dump_stop()
 */
void tbm_surface_internal_dump_start(char *path, int w, int h, int count);

/**
 * @brief Start the dump with scale debugging.
 * @details
 * Dump with scale supports only if a buffer has below formats.
 * - TBM_FORMAT_ARGB8888
 * - TBM_FORMAT_XRGB8888
 * @since_tizen 4.0
 * @param[in] path : the given dump path
 * @param[in] w : the width of dump image
 * @param[in] h : the height of dump image
 * @param[in] count : the dump count number
 * @param[in] scale : the scale factor
 * @see #tdm_helper_dump_stop()
 */
void tbm_surface_internal_dump_with_scale_start(char *path, int w, int h,
					  int count, double scale);

/**
 * @brief End the dump debugging.
 * @since_tizen 3.0
 * @see #tdm_helper_dump_start()
 */
void tbm_surface_internal_dump_end(void);

/**
 * @brief Dump a buffer
 * @details
 * This function supports only if a buffer has below formats.
 * - TBM_FORMAT_ARGB8888
 * - TBM_FORMAT_XRGB8888
 * - TBM_FORMAT_YVU420
 * - TBM_FORMAT_YUV420
 * - TBM_FORMAT_NV12
 * - TBM_FORMAT_NV21
 * - TBM_FORMAT_YUYV
 * - TBM_FORMAT_UYVY
 * The filename extension should be "png" for TBM_FORMAT_ARGB8888 and TBM_FORMAT_XRGB8888
 * or "yuv" for YUV formats.
 * @param[in] surface : a tbm surface
 * @param[in] name : a string used by a file name
 */
void tbm_surface_internal_dump_buffer(tbm_surface_h surface, const char *name);

/**
 * @brief Dump a shared memory buffer
 * @details
 * This function supports shared memory buffer dump.
 * @param[in] ptr : a pointer of dump buffer
 * @param[in] w : a width of dump buffer
 * @param[in] h : a height of dump buffer
 * @param[in] stride : a stride of dump buffer
 * @param[in] name : a string used by a file name
 */
void tbm_surface_internal_dump_shm_buffer(void *ptr, int w, int h, int stride, const char *name);

/**
 * @brief Capture a shared memory buffer
 * @details
 * This function supports shared memory buffer dump.
 * The type should be "png".
 * @param[in] ptr : a pointer of dump buffer
 * @param[in] w : a width of dump buffer
 * @param[in] h : a height of dump buffer
 * @param[in] stride : a stride of dump buffer
 * @param[in] path : the given dump path
 * @param[in] name : a string used by a file name
 * @param[in] type : a string used by a file type ex)png, yuv
 * @return 1 if success, otherwise 0.
 */
int tbm_surface_internal_capture_shm_buffer(void *ptr, int w, int h, int stride,
				       const char *path, const char *name, const char *type);

#ifdef __cplusplus
}
#endif
#endif /* _TBM_BUFMGR_H_ */
