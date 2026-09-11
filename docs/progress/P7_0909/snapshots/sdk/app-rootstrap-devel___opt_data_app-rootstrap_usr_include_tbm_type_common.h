/**************************************************************************

libtbm

Copyright 2012-2018 Samsung Electronics co., Ltd. All Rights Reserved.

Contact: SooChan Lim <sc1.lim@samsung.com>, Sangjin Lee <lsj119@samsung.com>
Inpyo Kang <mantiger@samsung.com>, Dongyeon Kim <dy5.kim@samsung.com>
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

#ifndef _TBM_TYPE_COMMON_H_
#define _TBM_TYPE_COMMON_H_

#include <tbm_type.h>

/**
 * \file tbm_type_common.h
 * \brief Type definition used internally
 */

/* tbm error base : this error base is same as TIZEN_ERROR_TBM in tizen_error.h */
#ifndef TBM_ERROR_BASE
#define TBM_ERROR_BASE			-0x02830000
#endif

/**
 * @brief Definition for the tizen buffer manager
 * @since_tizen 2.3
 */
typedef struct _tbm_bufmgr *tbm_bufmgr;

/**
 * @brief Definition for the tizen buffer object
 * @since_tizen 2.3
 */
typedef struct _tbm_bo *tbm_bo;

/**
 * @brief Definition for the key associated with the buffer object
 * @since_tizen 2.3
 */

typedef uint32_t tbm_key;
/**
 * @brief Definition for the file descripter of the system buffer manager
 * @since_tizen 2.3
 */
typedef int32_t tbm_fd;

/* TBM_DEVICE_TYPE */

/**
 * @brief Enumeration for tbm error type.
 * @since_tizen 2.4
 */
typedef enum {
	/* TBM_ERROR_XXX. start from (TBM_ERROR_BASE | 0x0001) to (TBM_ERROR_BASE | 0x0099) */
	TBM_ERROR_NONE = TIZEN_ERROR_NONE,                               /**< Successful */
	TBM_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,             /**< Out of memory */
	TBM_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,     /**< Invalid function parameter */
	TBM_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,     /**< Function not implemented */
	TBM_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,             /**< Not supported */

	/* TBM_BO_ERROR_XXX. start from (TBM_ERROR_BASE | 0x0101) to (TBM_ERROR_BASE | 0x0199) */
	TBM_BO_ERROR_GET_FD_FAILED = TBM_ERROR_BASE | 0x0101,            /**< failed to get fd in bo  (DEPRECATED) */
	TBM_BO_ERROR_HEAP_ALLOC_FAILED = TBM_ERROR_OUT_OF_MEMORY,        /**< failed to allocate the heap memory in bo  (DEPRECATED) */
	TBM_BO_ERROR_LOAD_MODULE_FAILED = TBM_ERROR_BASE | 0x0103,       /**< failed to load module  (DEPRECATED) */
	TBM_BO_ERROR_THREAD_INIT_FAILED = TBM_ERROR_BASE | 0x0104,       /**< failed to initialize the pthread  (DEPRECATED) */
	TBM_BO_ERROR_BO_ALLOC_FAILED = TBM_ERROR_OUT_OF_MEMORY,          /**< failed to allocate tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_INIT_STATE_FAILED = TBM_ERROR_BASE | 0x0106,        /**< failed to initialize the state of tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_IMPORT_FAILED = TBM_ERROR_BASE | 0x0107,            /**< failed to import the handle of tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_IMPORT_FD_FAILED = TBM_ERROR_BASE | 0x0108,         /**< failed to import fd of tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_EXPORT_FAILED = TBM_ERROR_BASE | 0x0109,            /**< failed to export the handle of the tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_EXPORT_FD_FAILED = TBM_ERROR_BASE | 0x0110,         /**< failed to export fd of tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_GET_HANDLE_FAILED = TBM_ERROR_BASE | 0x0111,        /**< failed to get the tbm_bo_handle  (DEPRECATED) */
	TBM_BO_ERROR_LOCK_FAILED = TBM_ERROR_BASE | 0x0112,              /**< failed to lock the tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_MAP_FAILED = TBM_ERROR_BASE | 0x0113,               /**< failed to map the tbm_bo to get the tbm_bo_handle  (DEPRECATED) */
	TBM_BO_ERROR_UNMAP_FAILED = TBM_ERROR_BASE | 0x0114,             /**< failed to unmap the tbm_bo  (DEPRECATED) */
	TBM_BO_ERROR_SWAP_FAILED = TBM_ERROR_BASE | 0x0115,              /**< failed to swap the tbm_bos  (DEPRECATED) */
	TBM_BO_ERROR_DUP_FD_FAILED = TBM_ERROR_BASE | 0x0116,            /**< failed to duplicate fd  (DEPRECATED) */

	/* TBM_SURFACE_ERROR_XXX. start from (TBM_ERROR_BASE | 0x0201) to (TBM_ERROR_BASE | 0x0299)
	 * TBM_SURFACE_ERROR_XXX is defined in tbm_surface.h file because it is CAPI.
	 *
	 * TBM_SURFACE_ERROR_NONE = TIZEN_ERROR_NONE,                             // Successful
	 * TBM_SURFACE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   // Invalid parameter
	 * TBM_SURFACE_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,   // Invalid Operation
	 *
	 */

	/* TBM_SURFACE_QUEUE_ERROR_XXX. start from (TBM_ERROR_BASE | 0x0301) to (TBM_ERROR_BASE | 0x0399) */
	TBM_SURFACE_QUEUE_ERROR_NONE = TBM_ERROR_NONE,                            /**< Successful */
	TBM_SURFACE_QUEUE_ERROR_EMPTY = TBM_ERROR_BASE | 0x0301,                  /**< Empty queue */
	TBM_SURFACE_QUEUE_ERROR_INVALID_QUEUE = TBM_ERROR_BASE | 0x0302,          /**< Invalid queue */
	TBM_SURFACE_QUEUE_ERROR_INVALID_SURFACE = TBM_ERROR_BASE | 0x0303,        /**< Invalid surface */
	TBM_SURFACE_QUEUE_ERROR_SURFACE_ALLOC_FAILED = TBM_ERROR_BASE | 0x0304,   /**< Allocation of surface failed */
	TBM_SURFACE_QUEUE_ERROR_ALREADY_EXIST = TBM_ERROR_BASE | 0x0305,	      /**< Already existed surface */
	TBM_SURFACE_QUEUE_ERROR_UNKNOWN_SURFACE = TBM_ERROR_BASE | 0x0306,	      /**< Unknown surface */
	TBM_SURFACE_QUEUE_ERROR_INVALID_SEQUENCE = TBM_ERROR_BASE | 0x0307,       /**< Invalid sequence */
	TBM_SURFACE_QUEUE_ERROR_TIMEOUT = TBM_ERROR_BASE | 0x0308,                /**< Timeout */
} tbm_error_e;

typedef tbm_error_e tbm_surface_queue_error_e;

/**
 * @brief Enumeration of tbm buffer manager capability.
 * @since_tizen 2.4
 */
enum TBM_BUFMGR_CAPABILITY {
	TBM_BUFMGR_CAPABILITY_NONE = 0,                  /**< Not Support capability*/
	TBM_BUFMGR_CAPABILITY_SHARE_KEY = (1 << 0),      /**< Support sharing buffer by tbm key */
	TBM_BUFMGR_CAPABILITY_SHARE_FD = (1 << 1),       /**< Support sharing buffer by tbm fd */
	TBM_BUFMGR_CAPABILITY_TBM_SYNC = (1 << 2),       /**< Support timeline sync */
	TBM_BUFMGR_CAPABILITY_TILED_MEMORY = (1 << 3),   /**< Support tiled memory */
	TBM_BUFMGR_CAPABILITY_SHARE_SURFACE = (1 << 4),   /**< Support sharing surface by buffer data */
};

/**
 * @brief Definition for the device type to access tbm_bo
 * @since_tizen 2.3
 */
typedef enum {
	TBM_DEVICE_DEFAULT = 0,  /**< the device type to get the default handle */
	TBM_DEVICE_CPU,          /**< the device type to get the virtual memory */
	TBM_DEVICE_2D,           /**< the device type to get the 2D memory handle */
	TBM_DEVICE_3D,           /**< the device type to get the 3D memory handle */
	TBM_DEVICE_MM,           /**< the device type to get the the multimedia handle */

} tbm_bo_device_type;

/**
 * @brief Definition for the optoin to access tbm_bo
 * @since_tizen 2.3
 */
typedef enum {
	TBM_OPTION_NONE = (1 << 0),        /**< the option is none */
	TBM_OPTION_READ = (1 << 0),        /**< the option to access to read the bo */
	TBM_OPTION_WRITE = (1 << 1),       /**< the option to access to write the bo */
	TBM_OPTION_VENDOR = (0xffff0000),  /**< the the vendor specific option that depends on the backend */
} tbm_bo_access_option;

/**
 * @brief tbm_bo_handle abstraction of the memory handle by TBM_DEVICE_TYPE
 * @since_tizen 2.3
 */
typedef union _tbm_bo_handle {
	void *ptr;
	int32_t s32;
	uint32_t u32;
	int64_t s64;
	uint64_t u64;
} tbm_bo_handle;

/**
 * @brief Enumeration of bo memory type
 * @since_tizen 2.3
 */
enum TBM_BO_FLAGS {
	TBM_BO_DEFAULT = 0,			   /**< default memory: it depends on the backend         */
	TBM_BO_SCANOUT = (1 << 0),	   /**< scanout memory                                    */
	TBM_BO_NONCACHABLE = (1 << 1), /**< non-cachable memory                               */
	TBM_BO_WC = (1 << 2),		   /**< write-combine memory                              */
	TBM_BO_TILED = (1 << 3),       /**< tiled memory                                      */
	TBM_BO_PROTECTED = (1 << 4),   /**< protected memory                                  */
	TBM_BO_VENDOR = (0xffff0000),  /**< vendor specific memory: it depends on the backend */
};

/**
 * @brief Definition for the TBM surface buffer data struct.
 * @since_tizen 6.5
 */
typedef struct _tbm_surface_buffer_data {
	int *fds;                       /**< an array of dmabuf fds */
	unsigned int num_fds;           /**< the number of dmabuf fds */
	int *meta_data;                 /**< an array of meta data */
	unsigned int num_meta_data;     /**< the number of meta data */
	void *reserved1;                /**< reserved data1 */
	void *reserved2;                /**< reserved data2 */
} tbm_surface_buffer_data;

/**
 * @brief Called when the user data is deleted in buffer object.
 * @since_tizen 2.3
 * @param[in] user_data User_data to be passed to callback function
 * @pre The callback must be registered using tbm_bo_add_user_data().\n
 * tbm_bo_delete_user_data() must be called to invoke this callback.
 * @see tbm_bo_add_user_data()
 * @see tbm_bo_delete_user_data()
 */
typedef void (*tbm_data_free) (void *user_data);

#endif							/* _TBM_TYPE_COMMON_H_ */
