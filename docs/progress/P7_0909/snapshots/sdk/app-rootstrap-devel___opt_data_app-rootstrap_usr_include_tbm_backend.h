/**************************************************************************

libtbm

Copyright 2018 Samsung Electronics co., Ltd. All Rights Reserved.

Contact: SooChan Lim <sc1.lim@samsung.com>,
		 Sangjin Lee <lsj119@samsung.com>,
		 Boram Park <boram1288.park@samsung.com>,
		 Changyeon Lee <cyeon.lee@samsung.com>

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

#ifndef _TBM_BACKEND_H_
#define _TBM_BACKEND_H_

#include <tbm_type_common.h>

/**
 * \file tbm_backend.h
 * \brief The backend header file for Tizen Buffer Manager
 *   This header is for the implementation of the TBM backend module.
 *  This backend header is used from the 3.0 version of libtbm and from
 *  the 5.0 version of the tizen.
 * @code
 * #include <tbm_backend.h>
 * @endcode
 */

#define TBM_BACKEND_ABI_MINOR_MASK          0x0000FFFF
#define TBM_BACKEND_ABI_MAJOR_MASK          0xFFFF0000
#define TBM_BACKEND_GET_ABI_MINOR(v)    ((v) & TBM_BACKEND_ABI_MINOR_MASK)
#define TBM_BACKEND_GET_ABI_MAJOR(v)    (((v) & TBM_BACKEND_ABI_MAJOR_MASK) >> 16)
/*
 * ABI versions.  Each version has a major and minor revision.  Modules
 * using lower minor revisions must work with servers of a higher minor
 * revision.  There is no compatibility between different major revisions.
 * Whenever the ABI_MAJOR_VERSION is changed, the others must also be
 * changed.  The minor revision mask is 0x0000FFFF and the major revision
 * mask is 0xFFFF0000.
 */
#define TBM_BACKEND_SET_ABI_VERSION(maj, min) \
		((((maj) << 16) & TBM_BACKEND_ABI_MAJOR_MASK) | ((min) & TBM_BACKEND_ABI_MINOR_MASK))

#define TBM_BACKEND_ABI_VERSION_1_0 TBM_BACKEND_SET_ABI_VERSION(1, 0)
#define TBM_BACKEND_ABI_VERSION_2_0 TBM_BACKEND_SET_ABI_VERSION(2, 0)
#define TBM_BACKEND_ABI_VERSION_3_0 TBM_BACKEND_SET_ABI_VERSION(3, 0)
#define TBM_BACKEND_ABI_LATEST_VERSION TBM_BACKEND_ABI_VERSION_3_0 /**< the latest version of the tbm backend abi  */

/**
 * @brief The backend bufmgr data
 * @details
 * The init() function of #tbm_backend_module returns the backend module data.
 * This handle will be used in #tbm_backend_bufmgr_func.
 * @see tbm_backend_module, tbm_backend_bufmgr_func
 */
typedef void tbm_backend_bufmgr_data;

/**
 * @brief The backend bo data
 * @details
 * The allocation function and the import function in #tbm_backend_bufmgr_func
 * returns the backend bo data. This handle will be used in #tbm_backend_bo_func.
 * @see tbm_backend_bufmgr_funce, tbm_backend_bo_func
 */
typedef void tbm_backend_bo_data;

/**
 * @brief The type definition of the bufmgr functions
 */
typedef struct _tbm_backend_bufmgr_func tbm_backend_bufmgr_func;

/**
 * @brief The type definition of the bo functions
 */
typedef struct _tbm_backend_bo_func tbm_backend_bo_func;

/**
 * @brief Definition for native display (wl_display in tizen)
 * @since_tizen 5.0
 */
typedef void tbm_native_display;

/**
 * @brief Enumeration of tbm buffer manager capability.
 * @since_tizen 5.0
 */
typedef enum TBM_BUFMGR_CAPABILITY tbm_bufmgr_capability;

/**
 * @brief Enumeration of tbm bo memory type.
 * @since_tizen 5.0
 */
typedef enum TBM_BO_FLAGS tbm_bo_memory_type;

/**
 * @brief The bufmgr functions for a backend module.
 */
struct _tbm_backend_bufmgr_func {
	/**
	 * @brief Get the capabilities of a buffer manager
	 * @remarks The backend must support the TBM_BUFMGR_CAPABILITY_SHARE_FD.
	 *        TBM_BUFMGR_CAPABILITY_SHARE_KEY will help you do debugging to
	 *        develop the platform because the tbm_key will be the unique
	 *        identification of the tbm_bo memory in the system.
	 * @param[in] bufmgr_data The backend module data
	 * @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	 * @return the capabilities of a backend modul
	 */
	tbm_bufmgr_capability (*bufmgr_get_capabilities)(tbm_backend_bufmgr_data *bufmgr_data,
										tbm_error_e *error);

	/**
	* @brief set(bind) the native display
	* @param[in] bufmgr_data The backend bufmgr data
	* @param[in] native_display : the native display (wl_display in wayland window system)
	* @return #TBM_ERROR_NONE if success. Otherwise, error value.
	*/
	tbm_error_e (*bufmgr_bind_native_display)(tbm_backend_bufmgr_data *bufmgr_data,
											tbm_native_display *native_display);

	/**
	* @brief get the formats list and the num to be supported by backend.
	* @param[in] bufmgr_data The backend bufmgr data
	* @param[out] **formats : format array list. it is allocated by backend funtion
	* @param[out] *num : the number of the formats to be supported by backend
	* @return #TBM_ERROR_NONE if success. Otherwise, error value.
	*/
	tbm_error_e (*bufmgr_get_supported_formats)(tbm_backend_bufmgr_data *bufmgr_data,
											uint32_t **formats, uint32_t *num);

	/**
	* @brief get the plane data of the plane_idx according to the format.
	* @param[in] bufmgr_data The backend bufmgr data
	* @param[in] format : the format of the surface
	* @param[in] plane_idx : the index of the plane
	* @param[in] width : the width of the surface with the format
	* @param[in] height : the height of the surface with the format
	* @param[out] size : the size of the plane index
	* @param[out] offset : the offset of the plane index at the bo index
	* @param[out] pitch : the pitch of the plane index
	* @param[out] bo_idx : the bo index of the plane index
	* @return #TBM_ERROR_NONE if success. Otherwise, error value.
	*/
	tbm_error_e (*bufmgr_get_plane_data)(tbm_backend_bufmgr_data *bufmgr_data,
									tbm_format format, int plane_idx, int width,
									int height, uint32_t *size, uint32_t *offset,
									uint32_t *pitch, int *bo_idx);

	/**
	* @brief allocate the bo data of the tbm_backend
	* @param[in] bufmgr_data The backend bufmgr data
	* @param[in] size : the size of bo data
	* @param[in] mem_types : the memory types
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return pointer of the bo_data of the tbm backend.
	*/
	tbm_backend_bo_data *(*bufmgr_alloc_bo)(tbm_backend_bufmgr_data *bufmgr_data,
								unsigned int size, tbm_bo_memory_type mem_types,
								tbm_error_e *error);

	/**
	* @brief allocate the bo data of the bo index according to the format
	* @param[in] bufmgr_data The backend bufmgr data
	* @param[in] width : the width of the surface with the format
	* @param[in] height : the height of the surface with the format
	* @param[in] format : the format of the surface
	* @param[in] mem_types : the memory types
	* @param[in] bo_idx : the bo index of the surface
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return pointer of the bo data of the tbm backend.
	*/
	tbm_backend_bo_data *(*bufmgr_alloc_bo_with_format)(tbm_backend_bufmgr_data *bufmgr_data,
											int format, int bo_idx, int width,
											int height, tbm_bo_memory_type mem_types,
											tbm_error_e *error);

	/**
	 * @brief allocate the bo data for GPU which support tiled format
	 * @param[in] bufmgr_data : The backend bufmgr data
	 * @param[in] width : the width of surface
	 * @param[in] height : the height of surface
	 * @param[in] bpp: bpp of the surface format
	 * @param[in] format : the format of surface
	 * @param[in] flags : the flags of memory type
	 * @param[in] bo_idx : the bo index of the surface
	 * @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	 * @return pointer of the bo private.
	 */
	tbm_backend_bo_data *(*bufmgr_alloc_bo_with_tiled_format)(tbm_backend_bufmgr_data *bufmgr_data,
											int width, int height, int bpp, int format,
											tbm_bo_memory_type flags, int bo_idx,
											tbm_error_e *error);

	/**
	* @brief import the bo data associated with the tdm_fd(prime fd).
	* @remarks tbm_fd must be free by user.
	* @remarks It must be supported at the backend module. To sharing the tdm_fd(prime fd)
	*        among the processes is the essential feature to run the graphic rendering.
	* @param[in] bufmgr_data The backend bufmgr data
	* @param[in] fd : the tdm_fd(prime fd) associated with the bo data
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return pointer of the bo data of the tbm backend.
	*/
	tbm_backend_bo_data *(*bufmgr_import_fd)(tbm_backend_bufmgr_data *bufmgr_data,
									tbm_fd fd, tbm_error_e *error);

	/**
	* @brief import the bo data associated with the tdm_key.
	* @remarks If the backend doesn't support a buffer sharing by tbm key,
	*        fucntion pointer must be set to NULL.
	* @param[in] bufmgr_data The backend bufmgr data
	* @param[in] key : the tdm_key associated with the bo data
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return pointer of the bo data of the tbm backend.
	*/
	tbm_backend_bo_data *(*bufmgr_import_key)(tbm_backend_bufmgr_data *bufmgr_data,
										tbm_key key, tbm_error_e *error);

	/* Padding for future extension */
	void (*reserved1)(void);
	void (*reserved2)(void);
	void (*reserved3)(void);
	void (*reserved4)(void);
	void (*reserved5)(void);
	void (*reserved6)(void);
};

/**
 * @brief The bo functions for a backend module.
 */
struct _tbm_backend_bo_func {
	/**
	* @brief free the bo data.
	* @param[in] bo_data : the bo data of the tbm backend
	* @return #TBM_ERROR_NONE if success. Otherwise, error value.
	*/
	void (*bo_free)(tbm_backend_bo_data *bo_data);

	/**
	* @brief get the size of a bo data.
	* @param[in] bo_data : the bo data of the tbm backend
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return size if this function succeeds, otherwise 0.
	*/
	int (*bo_get_size)(tbm_backend_bo_data *bo_data, tbm_error_e *error);

	/**
	* @brief get the tbm memory types
	* @param[in] bo_data : the bo data of the tbm backend
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @see #TBM_BO_FLAGS #tbm_bo_memory_type
	* @return tbm flags of memory type is this function succeeds, otherwise 0.
	*/
	tbm_bo_memory_type (*bo_get_memory_types)(tbm_backend_bo_data *bo_data,
										tbm_error_e *error);

	/**
	* @brief get the tbm_bo_handle according to the device type.
	* @param[in] bo_data : the bo data of the tbm backend
	* @param[in] device : the device type to get a handle
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return the handle of the bo data
	*/
	tbm_bo_handle (*bo_get_handle)(tbm_backend_bo_data *bo_data,
								tbm_bo_device_type device, tbm_error_e *error);

	/**
	* @brief map the bo data according to the device type and the option.
	* @param[in] bo_data : the bo data of the tbm backend
	* @param[in] device : the device type to get a handle
	* @param[in] opt : the option to access the bo data
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return the handle of the bo data
	*/
	tbm_bo_handle (*bo_map)(tbm_backend_bo_data *bo_data, tbm_bo_device_type device,
						tbm_bo_access_option opt, tbm_error_e *error);

	/**
	* @brief unmap the bo data.
	* @param[in] bo_data : the bo data of the tbm backend
	* @return #TBM_ERROR_NONE if success. Otherwise, error value.
	*/
	tbm_error_e (*bo_unmap)(tbm_backend_bo_data *bo_data);

	/**
	* @brief lock the bo data with a device and an opt.
	* @param[in] bo_data : the bo data of the tbm backend
	* @param[in] device : the device type to get a handle
	* @param[in] opt : the option to access the bo data
	* @return #TBM_ERROR_NONE if success. Otherwise, error value.
	* @remark This function pointer could be null.
	*/
	tbm_error_e (*bo_lock)(tbm_backend_bo_data *bo_data, tbm_bo_device_type device,
						tbm_bo_access_option opt);

	/**
	* @brief unlock the bo data.
	* @param[in] bo_data : the bo data of the tbm backend
	* @return #TBM_ERROR_NONE if success. Otherwise, error value.
	* @remark This function pointer could be null.
	*/
	tbm_error_e (*bo_unlock)(tbm_backend_bo_data *bo_data);

	/**
	* @brief export the bo data to the tdm_fd(prime fd)
	* @remarks tbm_fd must be free by user.
	* @remarks It must be supported at the backend module. To sharing the tdm_fd(prime fd)
	*        among the processes is the essential feature to run the graphic rendering.
	* @param[in] bo_data : the bo data of the tbm backend
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return tbm_fd associated with the bo data
	*/
	tbm_fd (*bo_export_fd)(tbm_backend_bo_data *bo_data, tbm_error_e *error);

	/**
	* @brief export the bo data to the tdm_key
	* @remarks If the backend doesn't support a buffer sharing by tbm key,
	*        fucntion pointer must be set to NULL.
	* @param[in] bo_data : the bo data of the tbm backend
	* @param[out] error #TBM_ERROR_NONE if success. Otherwise, error value.
	* @return key associated with the bo data
	*/
	tbm_key (*bo_export_key)(tbm_backend_bo_data *bo_data, tbm_error_e *error);

	/* Padding for future extension */
	void (*reserved1)(void);
	void (*reserved2)(void);
	void (*reserved3)(void);
	void (*reserved4)(void);
	void (*reserved5)(void);
	void (*reserved6)(void);
};

/**
 * @brief The backend module information of the entry point to initialize a tbm
 * backend module.
 * @remark
 * A backend module @b SHOULD define the global data symbol of which name is
 * @b "tbm_backend_module_data". tbm will read this symbol, @b "tbm_backend_module_data",
 * at the initial time and call init() function of #tbm_backend_module.
 */
typedef struct _tbm_backend_module {
	const char *name;           /**< The module name of a backend module */
	const char *vendor;         /**< The vendor name of a backend module */
	unsigned long abi_version;  /**< The ABI version of a backend module */

	/**
	 * @brief The init function of a backend module
	 * @param[in] bufmgr A tbm buffer manager object.
	 * @return The backend module data
	 * @see tbm_backend_bufmgr_data
	 */
	tbm_backend_bufmgr_data *(*init)(tbm_bufmgr bufmgr, tbm_error_e *error);

	/**
	* @brief deinitialize the bufmgr private data.
	* @param[in] bufmgr_data : The backend module data
	*/
	void (*deinit)(tbm_backend_bufmgr_data *bufmgr_data);
} tbm_backend_module;

#ifdef __cplusplus
extern "C" {
#endif

int                      tbm_backend_bufmgr_query_display_server(tbm_bufmgr bufmgr, tbm_error_e *error);
tbm_backend_bufmgr_func *tbm_backend_bufmgr_alloc_bufmgr_func(tbm_bufmgr bufmgr, tbm_error_e *error);
void                     tbm_backend_bufmgr_free_bufmgr_func(tbm_bufmgr bufmgr, tbm_backend_bufmgr_func *func);
tbm_error_e              tbm_backend_bufmgr_register_bufmgr_func(tbm_bufmgr bufmgr, tbm_backend_bufmgr_func *func);
tbm_backend_bo_func     *tbm_backend_bufmgr_alloc_bo_func(tbm_bufmgr bufmgr, tbm_error_e *error);
void                     tbm_backend_bufmgr_free_bo_func(tbm_bufmgr bufmgr, tbm_backend_bo_func *func);
tbm_error_e              tbm_backend_bufmgr_register_bo_func(tbm_bufmgr bufmgr, tbm_backend_bo_func *func);

#ifdef __cplusplus
}
#endif
#endif							/* _TBM_BACKEND_H_ */
