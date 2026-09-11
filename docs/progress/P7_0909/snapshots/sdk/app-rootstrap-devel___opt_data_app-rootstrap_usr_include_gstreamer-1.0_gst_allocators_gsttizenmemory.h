/*
 * GStreamer tizen memory
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
#ifndef __GST_TIZEN_MEMORY_H__
#define __GST_TIZEN_MEMORY_H__

#include <gst/gst.h>
#include <gst/allocators/allocators-prelude.h>
#include <gst/video/video.h>
#include <gst/video/gstvideometa.h>
//#include <gst/gstallocator.h>
#include <gst/gstmemory.h>
#include <tbm_bufmgr.h>
#include <tbm_surface.h>

G_BEGIN_DECLS

typedef struct _GstTizenAllocator GstTizenAllocator;
typedef struct _GstTizenAllocatorClass GstTizenAllocatorClass;
typedef struct _GstTizenAllocatorPrivate GstTizenAllocatorPrivate;

typedef struct _GstTizenMemory GstTizenMemory;
#define GST_TYPE_TIZEN_ALLOCATOR (gst_tizen_allocator_get_type())

GST_ALLOCATORS_API
GType gst_tizen_allocator_get_type(void);

#define GST_IS_TIZEN_ALLOCATOR(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TIZEN_ALLOCATOR))
#define GST_IS_TIZEN_ALLOCATOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_TIZEN_ALLOCATOR))
#define GST_TIZEN_ALLOCATOR_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_TIZEN_ALLOCATOR, GstTizenAllocatorClass))
#define GST_TIZEN_ALLOCATOR(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TIZEN_ALLOCATOR, GstTizenAllocator))
#define GST_TIZEN_ALLOCATOR_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_TIZEN_ALLOCATOR, GstTizenAllocatorClass))
#define GST_TIZEN_ALLOCATOR_CAST(obj)            ((GstTizenAllocator *)(obj))

#define GST_TIZEN_MEMORY_TYPE       "TizenVideoMemory"
#define GST_TIZEN_MEMORY_MAX_FD     4

struct _GstTizenMemory
{
  GstMemory                 mem;

  tbm_surface_h             surface;
  tbm_surface_info_s        surface_info;
  GstVideoInfo              *info;

  /* <protected> */
  GMutex                    lock;
  gboolean                  video_memory_map[GST_VIDEO_MAX_PLANES];
  gint                      video_memory_map_count;

  /* <private> */
  GDestroyNotify            notify;
  gpointer                  user_data;

  /* for encoded format */
  tbm_bo                    bo;
  tbm_bo_handle             bo_handle;

  /* for fd */
  int fd[GST_TIZEN_MEMORY_MAX_FD];
  int fd_count;
  gboolean is_fd_exported;
  gboolean is_secured;
};

struct _GstTizenAllocatorPrivate
{
  GList *mem_cache;
  GstAllocator *dmabuf_alloc;
};

/**
 * GstTizenMemoryAllocator
 *
 * Base class for allocators with bo memory
 */
struct _GstTizenAllocator
{
  GstAllocator parent;
  GstTizenAllocatorPrivate *priv;
  tbm_bufmgr bufmgr;
};


struct _GstTizenAllocatorClass
{
  GstAllocatorClass parent_class;
};

GST_ALLOCATORS_API
GstAllocator *  gst_tizen_allocator_new (void);

GST_ALLOCATORS_API
GstMemory *     gst_tizen_allocator_alloc (GstAllocator * allocator, GstVideoInfo * vinfo);

GST_ALLOCATORS_API
GstMemory *     gst_tizen_allocator_alloc_surface (GstAllocator * allocator, GstVideoInfo * vinfo,
                                                   tbm_surface_h surface, gpointer user_data, GDestroyNotify notify);

GST_ALLOCATORS_API
GstMemory *     gst_tizen_allocator_alloc_bo (GstAllocator * allocator, GstVideoInfo * vinfo,
                                                   tbm_bo bo, gsize size, gpointer user_data, GDestroyNotify notify);

GST_ALLOCATORS_API
GstMemory *     gst_tizen_allocator_alloc_fd (GstAllocator * allocator, GstVideoInfo * vinfo,
                                                   tbm_surface_info_s * tsinfo, int fd[GST_TIZEN_MEMORY_MAX_FD], int fd_count,
                                                   gpointer user_data, GDestroyNotify notify);

GST_ALLOCATORS_API
GstMemory *     gst_tizen_allocator_alloc_fd2 (GstAllocator * allocator, GstVideoInfo * vinfo,
                                                   tbm_surface_info_s * tsinfo, int fd[GST_TIZEN_MEMORY_MAX_FD], int fd_count,
                                                   gboolean is_secured, gpointer user_data, GDestroyNotify notify);

GST_ALLOCATORS_API
GstMemory *     gst_tizen_allocator_alloc_secure_fd (GstAllocator * allocator, GstVideoInfo * vinfo,
                                                   int fd[GST_TIZEN_MEMORY_MAX_FD], int fd_count, gsize maxsize, gsize size,
                                                   gpointer user_data, GDestroyNotify notify);

GST_ALLOCATORS_API
gboolean        gst_is_tizen_memory (GstMemory * mem);

GST_ALLOCATORS_API
gint            gst_tizen_memory_get_num_bos (GstMemory * mem);

GST_ALLOCATORS_API
void *          gst_tizen_memory_get_bos (GstMemory * mem, gint index);

GST_ALLOCATORS_API
void *          gst_tizen_memory_get_surface (GstMemory * mem);

GST_ALLOCATORS_API
gint            gst_tizen_memory_get_num_fd (GstMemory * mem);

GST_ALLOCATORS_API
gint            gst_tizen_memory_get_fd (GstMemory * mem, gint index);

GST_ALLOCATORS_API
gboolean        gst_tizen_memory_is_secured (GstMemory * mem);

GST_ALLOCATORS_API
GstMemory *     gst_tizen_allocator_dmabuf_export (GstAllocator * allocator, GstMemory * tmem, int bo_idx);

GST_ALLOCATORS_API
GstTizenMemory *gst_tizen_allocator_dmabuf_import (GstAllocator * allocator, gint * fds, gint planes, gsize offsets[4], GstVideoInfo * vinfo);

GST_ALLOCATORS_API
gboolean        gst_tizen_video_meta_map (GstVideoMeta * meta, guint plane, GstMapInfo * info,
                    gpointer * data, gint * stride, GstMapFlags flags);

GST_ALLOCATORS_API
gboolean        gst_tizen_video_meta_unmap (GstVideoMeta * meta, guint plane, GstMapInfo * info);

GST_ALLOCATORS_API
tbm_format      gst_video_format_to_tbm_format (GstVideoFormat format);

G_END_DECLS

#endif /* _GST_TIZEN_MEMORY_H_ */
