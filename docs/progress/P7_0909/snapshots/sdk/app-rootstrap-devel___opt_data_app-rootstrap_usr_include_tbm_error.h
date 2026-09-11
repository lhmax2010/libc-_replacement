/**************************************************************************

libtbm

Copyright 2012-2018 Samsung Electronics co., Ltd. All Rights Reserved.

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

#ifndef _TBM_ERROR_H_
#define _TBM_ERROR_H_

#include <tbm_type.h>
#include <tbm_type_common.h>

/**
 * \file tbm_error.h
 * \brief TBM Error
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the latest tbm_error.
 * @since_tizen 2.4
 * @return the latest tbm_error
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo bo;
   tbm_bo_handle handle;
   tbm_error_e error;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
   if (!bo)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   handle = tbm_bo_map (bo, TBM_DEVICE_2D, TBM_OPTION_READ|TBM_OPTION_WRITE);
   if (handle.ptr == NULL)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   tbm_bo_unmap (bo);
   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
tbm_error_e tbm_get_last_error(void);

#ifdef __cplusplus
}
#endif
#endif							/* _TBM_ERROR_H_ */