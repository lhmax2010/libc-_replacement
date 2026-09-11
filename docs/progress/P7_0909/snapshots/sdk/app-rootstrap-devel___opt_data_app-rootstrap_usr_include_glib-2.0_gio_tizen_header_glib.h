/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TIZEN_HEADER_GLIB_H__
#define __TIZEN_HEADER_GLIB_H__

#ifndef TIZEN_PUBLIC_DEPRECATED_API
  #ifdef TIZEN_DEPRECATION
    #define TIZEN_PUBLIC_DEPRECATED_API __attribute__((__visibility__("default"), deprecated))
  #else
    #define TIZEN_PUBLIC_DEPRECATED_API
  #endif
#endif

#endif // __TIZEN_HEADER_GLIB_H__
