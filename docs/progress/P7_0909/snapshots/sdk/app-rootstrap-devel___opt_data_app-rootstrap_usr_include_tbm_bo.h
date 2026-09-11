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

#ifndef _TBM_BO_H_
#define _TBM_BO_H_

#include <tbm_type.h>
#include <tbm_type_common.h>
#include <tbm_error.h>

/**
 * \file tbm_bo.h
 * \brief Tizen Buffer Object
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Functions for buffer object */

/**
 * @brief Allocates the buffer object.
 * @details This function create tbm_bo and set reference count to 1.\n
 * The user can craete tbm_bo with memory type flag #TBM_BO_FLAGS\n\n
 * #TBM_BO_DEFAULT indicates default memory: it depends on the backend\n
 * #TBM_BO_SCANOUT indicates scanout memory\n
 * #TBM_BO_NONCACHABLE indicates non-cachable memory\n
 * #TBM_BO_WC indicates write-combine memory\n
 * #TBM_BO_VENDOR indicates vendor specific memory: it depends on the tbm backend
 * @since_tizen 2.3
 * @param[in] bufmgr : the buffer manager
 * @param[in] size : the size of buffer object
 * @param[in] flags : the flags of memory type
 * @return a buffer object
 * @retval #tbm_bo
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo;
   tbm_error_e error;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
   if (!bo)
   {
      error = tbm_get_last_error ();
      ...
   }

   ....

   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
tbm_bo tbm_bo_alloc(tbm_bufmgr bufmgr, int size, int flags);

/**
 * @brief Increases the reference count of bo.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @return a buffer object
 * @retval #tbm_bo
 * @see tbm_bo_unref()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo bo;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);

   ...

   bo = tbm_bo_ref (bo);

   ....

   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
tbm_bo tbm_bo_ref(tbm_bo bo);

/**
 * @brief Decreases the reference count of bo
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @see tbm_bo_ref()
 * @see tbm_bo_alloc()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo bo;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
void tbm_bo_unref(tbm_bo bo);

/**
 * @brief Maps the buffer object according to the device type and the option.
 * @details Cache flushing and Locking is executed, while tbm_bo is mapping in the proper condition according to the device type and the access option.\n
 * If the cache flush type of bufmgr set true, the map cache flushing is executed
 * If the lock type of bufmgr set once, the previous bo which is locked is unlock when the new bo is trying to be locked.\n
 * If the lock type of bufmgr set always, the new bo is locked until the previous bo which is locked is unlocked.\n
 * If the lock type of bufmgr set never, Every bo is never locked.\n\n
 * #TBM_DEVICE_DEFAULT indicates the default handle.\n
 * #TBM_DEVICE_2D indicates the 2D memory handle.\n
 * #TBM_DEVICE_3D indicates the 3D memory handle.\n
 * #TBM_DEVICE_CPU indicates the virtual memory handle.\n
 * #TBM_DEVICE_MM indicates the multimedia handle.\n\n
 * #TBM_OPTION_READ indicates the accss option to read.\n
 * #TBM_OPTION_WRITE indicates the access option to write.\n
 * #TBM_OPTION_VENDOR indicates the vendor specific option that depends on the backend.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @param[in] device : the device type to get a handle
 * @param[in] opt : the option to access the buffer object
 * @return the handle of the buffer object
 * @exception #TBM_ERROR_NONE            Success
 * @exception #TBM_ERROR_BO_LOCK_FAILED  tbm_bo lock failed
 * @exception #TBM_ERROR_BO_MAP_FAILED   tbm_bo map failed
 * @retval #tbm_bo
 * @see tbm_bo_unmap()
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
tbm_bo_handle tbm_bo_map(tbm_bo bo, int device, int opt);

/**
 * @brief Unmaps the buffer object.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @return 1 if this function succeeds, otherwise 0.
 * @see tbm_bo_map()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo bo
   tbm_bo_handle handle;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);

   ...

   handle = tbm_bo_map (bo, TBM_DEVICE_2D, TBM_OPTION_READ|TBM_OPTION_WRITE);

   ...

   tbm_bo_unmap (bo);
   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
int tbm_bo_unmap(tbm_bo bo);

/**
 * @brief Gets the tbm_bo_handle according to the device type.
 * @details The tbm_bo_handle can be get without the map of the tbm_bo.\n
 * In this case, TBM does not guarantee the lock and the cache flush of the tbm_bo.\n\n
 * #TBM_DEVICE_DEFAULT indicates the default handle.\n
 * #TBM_DEVICE_2D indicates the 2D memory handle.\n
 * #TBM_DEVICE_3D indicates the 3D memory handle.\n
 * #TBM_DEVICE_CPU indicates the virtual memory handle.\n
 * #TBM_DEVICE_MM indicates the multimedia handle.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @param[in] device : the device type to get a handle
 * @return the handle of the buffer object
 * @retval #tbm_bo_handle
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

   ...

   handle = tbm_bo_get_handle (bo, TBM_DEVICE_2D);
   if (handle.ptr == NULL)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
  */
tbm_bo_handle tbm_bo_get_handle(tbm_bo bo, int device);

/**
 * @brief Exports the buffer object by key.
 * @details The tbm_bo can be exported to the anther process with the unique key associated with the the tbm_bo.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @return key associated with the buffer object
 * @retval #tbm_key
 * @see tbm_bo_import()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo;
   tbm_key key;
   tbm_error_e error;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
   key = tbm_bo_export (bo);
   if (key == 0)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
  */
tbm_key tbm_bo_export(tbm_bo bo);

/**
 * @brief Exports the buffer object by fd.
 * @details The tbm_bo can be exported to the anther process with the unique fd associated with the the tbm_bo.
 * @since_tizen 2.3
 * @remarks You must release the fd using close().
 * @param[in] bo : the buffer object
 * @return fd associated with the buffer object
 * @retval #tbm_fd
 * @see tbm_bo_import_fd()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_fd bo_fd;
   tbm_bufmgr bufmgr;
   tbm_bo;
   tbm_error_e error;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
   bo_fd = tbm_bo_export (bo);
   if (bo_fd == 0)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
  */
tbm_fd tbm_bo_export_fd(tbm_bo bo);

/**
 * @brief Imports the buffer object associated with the key.
 * @details The reference count of the tbm_bo is 1.
 * @since_tizen 2.3
 * @param[in] bufmgr : the buffer manager
 * @param[in] key : the key associated with the buffer object
 * @return a buffer object
 * @retval #tbm_bo
 * @see tbm_bo_export()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   int bo_key;
   tbm_bufmgr bufmgr;
   tbm_bo;
   tbm_error_e error;

   ...

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_import (bufmgr, key);
   if (bo == NULL)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
  */
tbm_bo tbm_bo_import(tbm_bufmgr bufmgr, tbm_key key);

/**
 * @brief Imports the buffer object associated with the fd.
 * @details The reference count of the tbm_bo is 1.
 * @since_tizen 2.3
 * @remarks You must release the fd using close().
 * @param[in] bufmgr : the buffer manager
 * @param[in] fd : the fd associated with the buffer object
 * @return a buffer object
 * @retval #tbm_bo
 * @see tbm_bo_export_fd()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_fd bo_fd;
   tbm_bufmgr bufmgr;
   tbm_bo bo;
   tbm_error_e error;

   ...

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_import_fd (bo_fd);
   if (bo == 0)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
  */
tbm_bo tbm_bo_import_fd(tbm_bufmgr bufmgr, tbm_fd fd);

/**
 * @brief Gets the size of a bo.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @return 1 if this function succeeds, otherwise 0.
 * @see tbm_bo_alloc()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo;
   int size;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
   size = tbm_bo_size (bo);

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
  */
int tbm_bo_size(tbm_bo bo);

/**
 * @brief Gets the state where the buffer object is locked.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @return 1 if this bo is locked, otherwise 0.
 * @see tbm_bo_map()
 * @see tbm_bo_unmap()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo bo;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);

   ...

   if (tbm_bo_locked (bo))
   {

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
*/
int tbm_bo_locked(tbm_bo bo);

/**
 * @brief Swaps the buffer object.
 * @since_tizen 2.3
 * @param[in] bo1 : the buffer object
 * @param[in] bo2 : the buffer object
 * @return 1 if this function succeeds, otherwise 0.
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo bo1;
   tbm_bo bo2;
   int ret;
   tbm_error_e error;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo1 = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
   bo2 = tbm_bo_alloc (bufmgr, 256 * 256, TBM_BO_DEFAULT);

   ...

   ret = tbm_bo_swap (bo1, bo2);
   if (ret == 0)
   {
      error = tbm_get_last_error ();
      ...
   }

   ...

   tbm_bo_unref (bo1);
   tbm_bo_unref (bo2);
   tbm_bufmgr_deinit (bufmgr);
   @endcode
 */
int tbm_bo_swap(tbm_bo bo1, tbm_bo bo2);

/**
 * @brief Adds a user_data to the buffer object.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @param[in] key : the key associated with the user_data
 * @param[in] data_free_func : the function pointer to free the user_data
 * @return 1 if this function succeeds, otherwise 0.
 * @post tbm_data_free() will be called under certain conditions, after calling tbm_bo_delete_user_data().
 * @see tbm_data_free()
 * @see tbm_bo_set_user_data()
 * @see tbm_bo_get_user_data()
 * @see tbm_bo_delete_user_data()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   void example_data_free (void *user_data)
   {
       char *data = (char*) user_data;
       free(data);
   }

   int main()
   {
       int bufmgr_fd;
       tbm_bufmgr bufmgr;
       tbm_bo bo;
       char *user_data;
       char *get_data;
       int ret;

       bufmgr = tbm_bufmgr_init (bufmgr_fd);
       bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
       user_data = (char*) malloc (sizeof(char) * 128);

       ...

       tbm_bo_add_user_data (bo, 1, example_data_free);
       tbm_bo_set_user_data (bo, 1, user_data);

       ...

       ret = tbm_bo_get_user_data (bo, 1, &get_data);
       tbm_bo_delete_user_data (bo, 1);

       ...

       tbm_bo_unref (bo);
       tbm_bufmgr_deinit (bufmgr);
   }
   @endcode
 */

int tbm_bo_add_user_data(tbm_bo bo, unsigned long key,
			 tbm_data_free data_free_func);

/**
 * @brief Deletes the user_data in the buffer object.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @param[in] key : the key associated with the user_date
 * @return 1 if this function succeeds, otherwise 0.
 * @see tbm_bo_add_user_data()
 * @see tbm_bo_get_user_data()
 * @see tbm_bo_delete_user_data()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   void example_data_free (void *user_data)
   {
       char *data = (char*) user_data;
       free(data);
   }

   int main()
   {
       int bufmgr_fd;
       tbm_bufmgr bufmgr;
       tbm_bo bo;
       char *user_data;
       char *get_data;
       int ret;

       bufmgr = tbm_bufmgr_init (bufmgr_fd);
       bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
       user_data = (char*) malloc (sizeof(char) * 128);

       ...

       tbm_bo_add_user_data (bo, 1, example_data_free);
       tbm_bo_set_user_data (bo, 1, user_data);

       ...

       ret = tbm_bo_get_user_data (bo, 1, &get_data);
       tbm_bo_delete_user_data (bo, 1);

       ...

       tbm_bo_unref (bo);
       tbm_bufmgr_deinit (bufmgr);
   }
   @endcode
 */
int tbm_bo_delete_user_data(tbm_bo bo, unsigned long key);

/**
 * @brief Sets a user_date to the buffer object.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @param[in] key : the key associated with the user_date
 * @param[in] data : a pointer of the user_data
 * @return 1 if this function succeeds, otherwise 0.
 * @see tbm_bo_add_user_data()
 * @see tbm_bo_set_user_data()
 * @see tbm_bo_delete_user_data()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   void example_data_free (void *user_data)
   {
       char *data = (char*) user_data;
       free(data);
   }

   int main()
   {
       int bufmgr_fd;
       tbm_bufmgr bufmgr;
       tbm_bo bo;
       char *user_data;
       char *get_data;
       int ret;

       bufmgr = tbm_bufmgr_init (bufmgr_fd);
       bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
       user_data = (char*) malloc (sizeof(char) * 128);

       ...

       tbm_bo_add_user_data (bo, 1, example_data_free);
       tbm_bo_set_user_data (bo, 1, user_data);

       ...

       ret = tbm_bo_get_user_data (bo, 1, &get_data);
       tbm_bo_delete_user_data (bo, 1);

       ...

       tbm_bo_unref (bo);
       tbm_bufmgr_deinit (bufmgr);
   }
   @endcode
 */
int tbm_bo_set_user_data(tbm_bo bo, unsigned long key, void *data);

/**
 * @brief Gets a user_data from the buffer object with the key.
 * @since_tizen 2.3
 * @param[in] bo : the buffer object
 * @param[in] key : the key associated with the user_date
 * @param[out] data : to get the user data
 * @return 1 if this function succeeds, otherwise 0.
 * @see tbm_bo_add_user_data()
 * @see tbm_bo_set_user_data()
 * @see tbm_bo_get_user_data()
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   void example_data_free (void *user_data)
   {
       char *data = (char*) user_data;
       free(data);
   }

   int main()
   {
       int bufmgr_fd;
       tbm_bufmgr bufmgr;
       tbm_bo bo;
       char *user_data;
       char *get_data;
       int ret;

       bufmgr = tbm_bufmgr_init (bufmgr_fd);
       bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
       user_data = (char*) malloc (sizeof(char) * 128);

       ...

       tbm_bo_add_user_data (bo, 1, example_data_free);
       tbm_bo_set_user_data (bo, 1, user_data);

       ...

       ret = tbm_bo_get_user_data (bo, 1, &get_data);
       tbm_bo_delete_user_data (bo, 1);

       ...

       tbm_bo_unref (bo);
       tbm_bufmgr_deinit (bufmgr);
   }
   @endcode
 */
int tbm_bo_get_user_data(tbm_bo bo, unsigned long key, void **data);

/**
 * @brief Gets the tbm bo flags.
 * @since_tizen 2.4
 * @param[in] bo : the buffer object
 * @return the tbm bo flags
 * @see TBM_BO_FLAGS
 * @par Example
   @code
   #include <tbm_bufmgr.h>

   int bufmgr_fd;
   tbm_bufmgr bufmgr;
   tbm_bo;
   int flags;

   bufmgr = tbm_bufmgr_init (bufmgr_fd);
   bo = tbm_bo_alloc (bufmgr, 128 * 128, TBM_BO_DEFAULT);
   flags = tbm_bo_get_flags (bo);

   ...

   tbm_bo_unref (bo);
   tbm_bufmgr_deinit (bufmgr);

   @endcode
 */
int tbm_bo_get_flags(tbm_bo bo);

#ifdef __cplusplus
}
#endif
#endif							/* _TBM_BO_H_ */
