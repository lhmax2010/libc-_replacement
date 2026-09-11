/*
 * GStreamer tizen buffer pool
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
 * Author: Sejun Park <sejun79.park@samsung.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _GST_TIZEN_BUFFERPOOL_H_
#define _GST_TIZEN_BUFFERPOOL_H_

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideometa.h>
#include <gst/video/gstvideopool.h>
#include <gst/allocators/gsttizenmemory.h>

G_BEGIN_DECLS

/**
 * GST_BUFFER_POOL_OPTION_TIZEN_META:
 *
 * An option that can be activated on bufferpool to request TizenVideo metadata
 * on buffers from the pool.
 */
#define GST_BUFFER_POOL_OPTION_TIZEN_META "GstBufferPoolOptionTizenVideoMeta"

typedef struct _GstTizenBufferPool GstTizenBufferPool;
typedef struct _GstTizenBufferPoolClass GstTizenBufferPoolClass;
typedef struct _GstTizenBufferPoolPrivate GstTizenBufferPoolPrivate;

/* buffer pool functions */
#define GST_TYPE_TIZEN_BUFFER_POOL      (gst_tizen_buffer_pool_get_type())
#define GST_IS_TIZEN_BUFFER_POOL(obj)   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TIZEN_BUFFER_POOL))
#define GST_TIZEN_BUFFER_POOL(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TIZEN_BUFFER_POOL, GstTizenBufferPool))
#define GST_TIZEN_BUFFER_POOL_CAST(obj) ((GstTizenBufferPool*)(obj))

struct _GstTizenBufferPool
{
  GstBufferPool bufferpool;

  GstAllocator *allocator;
  GstVideoInfo  info;

  gboolean      add_videometa;
  gboolean      add_tizenmeta;

  GMutex       lock;

  GPtrArray    *buffers;
  gint         current_buffer_index;
  gint         outstandings;
  gboolean     empty;

  GstTizenBufferPoolPrivate *priv;
};

struct _GstTizenBufferPoolClass
{
  GstBufferPoolClass parent_class;
};

GST_ALLOCATORS_API
GType gst_tizen_buffer_pool_get_type (void);

GST_ALLOCATORS_API
GstBufferPool *gst_tizen_buffer_pool_new (void);


G_END_DECLS

#endif /* _GST_TIZEN_BUFFER_POOL_H_ */
