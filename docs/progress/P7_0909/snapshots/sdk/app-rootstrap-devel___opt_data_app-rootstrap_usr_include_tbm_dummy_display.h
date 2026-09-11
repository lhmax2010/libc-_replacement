/**************************************************************************

libtbm

Copyright 2018 Samsung Electronics co., Ltd. All Rights Reserved.

Contact: SooChan Lim <sc1.lim@samsung.com>,
		 Changyeon Lee <cyeon.lee@samsung.com>,
		 Sangjin Lee <lsj119@samsung.com>

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

#ifndef _TBM_DUMMY_DISPLAY_H_
#define _TBM_DUMMY_DISPLAY_H_

/**
 * \file tbm_dummy_display.h
 * \brief TBM dummy display
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Definition for dummy display
 */
typedef void tbm_dummy_display;

/**
 * @brief Create the dummy display.
 * @details The dummy display usually is used by the native_display for the parameger of the eglGetDisplay()\n
 * The user uses the dummy display as the native_display for calling eglGetDisplay() when the user want to \n
 * get EGLDisplay for the offscreen rendering in Tizen.
 * \n
 * @return a tbm dummy display
 * @retval #tbm_dummy_display
 */
tbm_dummy_display *tbm_dummy_display_create(void);

/**
 * @brief Destroy the dummy display.
 * @param[in] dpy : the tbm dummy display
 */
void               tbm_dummy_display_destroy(tbm_dummy_display *dpy);

/**
 * @brief Validate the dummy display.
 * @param[in] dpy : the tbm dummy display
 * @return 1 if this dpy is valid, otherwise 0.
 */
int                tbm_dummy_display_is_valid(tbm_dummy_display *dpy);

#ifdef __cplusplus
}
#endif
#endif							/* _TBM_DUMMY_DISPLAY_H_ */
