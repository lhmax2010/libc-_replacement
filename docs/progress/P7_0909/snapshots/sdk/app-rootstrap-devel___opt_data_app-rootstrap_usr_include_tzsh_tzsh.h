#ifndef _TZSH_COMMON_H_
#define _TZSH_COMMON_H_


/**
 * @addtogroup TIZEN_WS_SHELL_MODULE
 * @{
 */

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handle for the Tizen shell.
 * @since_tizen 3.0
 */
typedef struct _tzsh_s *tzsh_h;

/**
 * @brief The window handle for native window.
 * @details This is abstract window type for native window handle.
 * @since_tizen 3.0
 */
typedef unsigned int tzsh_window;

/**
 * @brief Enumeration for toolkit type.
 * @since_tizen 3.0
 */
typedef enum
{
   TZSH_TOOLKIT_TYPE_UNKNOWN, /**< Unknown type */
   TZSH_TOOLKIT_TYPE_EFL, /**< Use EFL toolkit */
   TZSH_TOOLKIT_TYPE_TCORE_WAYLAND, /**< Use tizen-core-wl toolkit */
} tzsh_toolkit_type_e;

/**
 * @brief Enumeration for Tizen shell error.
 * @since_tizen 3.0
 */
typedef enum
{
   TZSH_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
   TZSH_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
   TZSH_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
   TZSH_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
   TZSH_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported*/
   TZSH_ERROR_NO_SERVICE = TIZEN_ERROR_TZSH | 0x01,/**< Service does not exist */
   TZSH_ERROR_ALREADY_REGISTERED_WINDOW = TIZEN_ERROR_TZSH | 0x02, /**< Window is already registered */

} tzsh_error_e;


/**
 * @brief Creates a #tzsh_h instance.
 * @details This function creates a #tzsh_h instance for the given toolkit type.
 * @since_tizen 3.0
 * @remarks The returned handle should be released using tzsh_destroy().
 *          The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] type The toolkit type
 * @return #tzsh_h instance on success, @c NULL otherwise
 * @exception #TZSH_ERROR_NONE Successful
 * @exception #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #TZSH_ERROR_OUT_OF_MEMORY Out of memory
 * @see tzsh_destroy()
 */
tzsh_h tzsh_create(tzsh_toolkit_type_e type);

/**
 * @brief Destroys the given #tzsh_h instance.
 * @since_tizen 3.0
 * @param[in] tzsh The #tzsh_h instance to be destroyed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tzsh_create()
 */
int tzsh_destroy(tzsh_h tzsh);

#ifdef  __cplusplus
}
#endif

/**
 * @}
 */

#endif
