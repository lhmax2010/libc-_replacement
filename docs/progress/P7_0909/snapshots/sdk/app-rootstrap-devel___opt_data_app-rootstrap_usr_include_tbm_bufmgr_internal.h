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

#ifndef _TBM_BUFMGR_INTERNAL_H_
#define _TBM_BUFMGR_INTERNAL_H_

#include <tbm_type.h>
#include <tbm_type_common.h>
#include <tbm_bo.h>
#include <tbm_error.h>

/**
 * \file tbm_bufmgr_internal.h
 * \brief Tizen Buffer Manager Internal
 */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief bind the native_display.
 * @since_tizen 3.0
 * @param[in] bufmgr : the buffer manager
 * @param[in] native_display : the native_display
 */
int tbm_bufmgr_bind_native_display(tbm_bufmgr bufmgr, void *native_display);

/**
 * @brief Initializes the buffer manager at the display server.
 * @details use this api to initialize the tbm_bufmgr at the display server.
 * @since_tizen 5.0
 */
tbm_bufmgr tbm_bufmgr_server_init(void);


#ifdef __cplusplus
}
#endif
#endif /* _TBM_BUFMGR_INTERNAL_H_ */
