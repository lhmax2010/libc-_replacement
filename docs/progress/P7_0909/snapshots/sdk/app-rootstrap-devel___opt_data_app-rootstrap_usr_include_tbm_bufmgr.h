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

#ifndef _TBM_BUFMGR_H_
#define _TBM_BUFMGR_H_

#include <tbm_type.h>
#include <tbm_type_common.h>
#include <tbm_bo.h>
#include <tbm_error.h>

/**
 * \file tbm_bufmgr.h
 * \brief Tizen Buffer Manager
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Functions for buffer manager */

/**
 * @brief Initializes the buffer manager.
 * @details If fd is lower than zero, fd is get drm fd in tbm_bufmgr_init function\n
 * @since_tizen 2.3
 * @param[in] fd : file descripter of the system buffer manager
 * @return a buffer manager
 * @retval #tbm_bufmgr
 * @see tbm_bufmgr_deinit();
 * @par Example
   @code
   #include <tbm_bufmgr.h>
   int bufmgr_fd;

   tbm_bufmgr bufmgr;
   bufmgr = tbm_bufmgr_init (bufmgr_fd);

   ....

   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
tbm_bufmgr tbm_bufmgr_init(int fd);

/**
 * @brief Deinitializes the buffer manager.
 * @since_tizen 2.3
 * @param[in] bufmgr : the buffer manager
 * @see tbm_bufmgr_init()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_error_e error;
   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   if (!bufmgr)
   {
      error = tbm_get_last_error ();
      ...
   }

   ....

   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
void tbm_bufmgr_deinit(tbm_bufmgr bufmgr);

/**
 * @brief Gets the tbm buffer capability.
 * @since_tizen 2.4
 * @param[in] bufmgr : the buffer manager
 * @return the tbm bufmgr capability
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   unsigned int capability;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);

   capability = tbm_bufmgr_get_capability (bufmgr);

   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
unsigned int tbm_bufmgr_get_capability(tbm_bufmgr bufmgr);

#ifdef __cplusplus
}
#endif
#endif							/* _TBM_BUFMGR_H_ */
