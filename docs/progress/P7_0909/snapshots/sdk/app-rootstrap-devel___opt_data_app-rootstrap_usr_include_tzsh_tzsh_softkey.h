#ifndef _TZSH_SOFTKEY_H_
#define _TZSH_SOFTKEY_H_

/**
 * @addtogroup TIZEN_WS_SHELL_SOFTKEY_MODULE
 * @{
 */

#include <tzsh.h>

#ifdef  __cplusplus
extern "C" {
#endif


/**
 * @brief Handle for the Tizen softkey.
 * @since_tizen 5.5
 */
typedef struct _tzsh_softkey_s *tzsh_softkey_h;

/**
 * @brief Enumeration for visible state of softkey.
 * @details An enumeration type that defines the visibility state of a given tzsh softkey service window.
 * @since_tizen 5.5
 */
typedef enum
{
   TZSH_SOFTKEY_STATE_VISIBLE_UNKNOWN = 0x0, /**< Unknown state. There is no softkey service. */
   TZSH_SOFTKEY_STATE_VISIBLE_SHOW = 0x1,    /**< The softkey window is shown. */
   TZSH_SOFTKEY_STATE_VISIBLE_HIDE = 0x2,    /**< The softkey window is hidden. */
} tzsh_softkey_state_visible_e;

/**
 * @brief Enumeration for expand state of softkey.
 * @details An enumeration type that defines the expand state of a given tzsh softkey service window.
 * @since_tizen 5.5
 */
typedef enum
{
   TZSH_SOFTKEY_STATE_EXPAND_UNKNOWN = 0x0,  /**< Unknown state. There is no softkey service. */
   TZSH_SOFTKEY_STATE_EXPAND_ON = 0x1,       /**< The softkey window is expandable. */
   TZSH_SOFTKEY_STATE_EXPAND_OFF = 0x2,      /**< The softkey window is not expandable. */
} tzsh_softkey_state_expand_e;

/**
 * @brief Enumeration for opacity state of softkey.
 * @details An enumeration type that defines the transparency level of a given tzsh softkey service window.
 * @since_tizen 5.5
 */
typedef enum
{
   TZSH_SOFTKEY_STATE_OPACITY_UNKNOWN = 0x0,       /**< Unknown state. There is no softkey service. */
   TZSH_SOFTKEY_STATE_OPACITY_OPAQUE = 0x1,        /**< The softkey window is opaque */
   TZSH_SOFTKEY_STATE_OPACITY_TRANSPARENT = 0x2,   /**< The softkey window is transparent */
} tzsh_softkey_state_opacity_e;


/**
 * @platform
 * @brief Creates a #tzsh_softkey_h object which handle for the softkey client.
 * @details This is the creator creator for handler of tzsh_softkey for softkey clients.
 *          It creates and returns a handler of the tzsh_softkey using the given tzsh handler and native window to use tzsh_softkey.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/windowsystem.admin
 * @remarks The returned handle should be released using tzsh_softkey_destroy().
 *          The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @param[in] tzsh The #tzsh_h instance
 * @param[in] win The native window
 * @return #tzsh_softkey_h object on success, @c NULL otherwise
 * @exception #TZSH_ERROR_NONE Successful
 * @exception #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #TZSH_ERROR_NO_SERVICE Service does not exist
 * @exception #TZSH_ERROR_OUT_OF_MEMORY Out of memory
 * @exception #TZSH_ERROR_PERMISSION_DENIED Permission denied
 * @exception #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @pre You have to create a #tzsh_h instance using tzsh_create() before calling this function.
 *      You can get the #tzsh_window instance using the toolkit API such as elm_win_window_id_get().
 * @see tzsh_create()
 * @see tzsh_softkey_destroy()
 * @par Example
 * @code
 *  #include <tzsh_softkey.h>
 *
 *  tzsh_h tzsh = NULL;
 *  tzsh_softkey_h tz_softkey = NULL;
 *  Evas_Object *win;
 *
 *  void init_tzsh(void)
 *  {
 *      tzsh = tzsh_create(TZSH_TOOLKIT_TYPE_EFL);
 *      ...
 *  }
 *
 *  void create_window(void)
 *  {
 *      tzsh_window tz_win;
 *
 *      win = elm_win_add(NULL, "sample", ELM_WIN_BASIC);
 *      ...
 *
 *      tz_win = elm_win_window_id_get(win);
 *      ...
 *
 *      tz_softkey = tzsh_softkey_create(tzsh, tz_win);
 *      if (tz_softkey == NULL)
 *        {
 *           if (get_last_result() == TZSH_ERROR_PERMISSION_DENIED)
 *             {
 *                ... // error control code
 *             }
 *           else if (get_last_result() == TZSH_ERROR_NOT_SUPPORTED)
 *             {
 *                ... // error control code
 *             }
 *           ... // other error control code
 *        }
 *      ...
 *  }
 * @endcode
 */
tzsh_softkey_h tzsh_softkey_create(tzsh_h tzsh,
                                   tzsh_window win);

/**
 * @brief Destroys the given #tzsh_softkey_h object.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object to be destroyed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_softkey_create()
 * @par Example
 * @code
 *  #include <tzsh_softkey.h>
 *
 *  tzsh_h tzsh = NULL;
 *  tzsh_softkey_h tzsh_softkey = NULL;
 *  Evas_Object *win;
 *
 *  void create_window(void)
 *  {
 *      tzsh_window tz_win;
 *
 *      win = elm_win_add(NULL, "sample", ELM_WIN_BASIC);
 *      ...
 *
 *      tz_win = (tzsh_window)elm_win_window_id_get(win);
 *      ...
 *
 *      tzsh_softkey = tzsh_softkey_create(tzsh, tz_win);
 *      ...
 *  }
 *
 *  void destroy_window(void)
 *  {
 *      ...
 *      tzsh_softkey_destroy(tzsh_softkey);
 *      tzsh_softkey = NULL;
 *      ...
 *  }
 * @endcode
 */
int tzsh_softkey_destroy(tzsh_softkey_h softkey);

/**
 * @brief Shows the global softkey service window.
 * @details Shows the softkey service window.
 *          This applies system wide, so even if the active client switches to another softkey client,
 *          the softkey service window remains shown. If the client wants to always hide the softkey service window,
 *          check its visibility using #tzsh_softkey_global_visible_state_get when resumed,
 *          then call #tzsh_softkey_global_hide if needed.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist or @a softkey is NULL
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_create()
 */
int tzsh_softkey_global_show(tzsh_softkey_h softkey);

/**
 * @brief Hides the global softkey service window.
 * @details Hides the softkey service window.
 *          This applies system wide, so even if the active client switches to another softkey client,
 *          the softkey service window remains hidden.
 *          If the client wants to always show the softkey service window,
 *          check its visibility using #tzsh_softkey_global_visible_state_get when resumed,
 *          then call #tzsh_softkey_global_show if needed.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist or @a softkey is NULL
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_create()
 */
int tzsh_softkey_global_hide(tzsh_softkey_h softkey);

/**
 * @brief Gets the global visible state of a softkey service window.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object
 * @param[out] visible The global visible state of the softkey service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist or @a softkey is NULL
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_create()
 */
int tzsh_softkey_global_visible_state_get(tzsh_softkey_h softkey,
                                          tzsh_softkey_state_visible_e *visible);

/**
 * @brief Sets the global expand state of a softkey service window.
 * @details This function lets the application controls the softkey service window's global
 *          expand status. If @a expand is set to #TZSH_SOFTKEY_STATE_EXPAND_OFF, then the
 *          softkey service window does not show the expand button. When the @a expand state
 *          is changed to #TZSH_SOFTKEY_STATE_EXPAND_ON, the softkey service window shows
 *          its expand button and can expand or minimize the softkey service window. This
 *          function affects all applications because it sets global expand state. The default
 *          expand state of the window is #TZSH_SOFTKEY_STATE_EXPAND_ON.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object
 * @param[in] expand The global expand state of the softkey service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist or @a softkey is NULL
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_create()
 */
int tzsh_softkey_global_expand_state_set(tzsh_softkey_h softkey,
                                         tzsh_softkey_state_expand_e expand);

/**
 * @brief Gets the global expand state of softkey service window.
 * @details Retrieves the expand state of the softkey service window.
 *          This is a system global value, so even if called from different softkey clients at the same time, it will return the same result.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object
 * @param[out] expand The global expand state of the softkey service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist or @a softkey is NULL
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_create()
 */
int tzsh_softkey_global_expand_state_get(tzsh_softkey_h softkey,
                                         tzsh_softkey_state_expand_e *expand);

/**
 * @brief Sets the global opacity state of a softkey service window.
 * @details This function can control transparency of the softkey service window.
 *
 *          If @a opacity is set to #TZSH_SOFTKEY_STATE_OPACITY_TRANSPARENT, the softkey service window
 *          becomes transparent like a 32-bit color window.
 *
 *          When the @a opacity state is changed to #TZSH_SOFTKEY_STATE_OPACITY_OPAQUE,
 *          the softkey service window is shown as an opaque window.
 *
 *          This function affects all applications because it sets global opacity state.
 *
 *          The default opacity state of the window is #TZSH_SOFTKEY_STATE_OPACITY_OPAQUE.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object
 * @param[in] opacity The global opacity state of the softkey service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist or @a softkey is NULL
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_create()
 */
int tzsh_softkey_global_opacity_state_set(tzsh_softkey_h softkey,
                                          tzsh_softkey_state_opacity_e opacity);

/**
 * @brief Gets the global opacity state of a softkey service window.
 * @details Retrieves the opacity state of the softkey service window.
 *          This is a system global value, so even if called from different softkey clients at the same time, it will return the same result.
 * @since_tizen 5.5
 * @param[in] softkey The #tzsh_softkey_h object
 * @param[out] opacity The global opacity state of the softkey service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist or @a softkey is NULL
 * @retval #TZSH_ERROR_NOT_SUPPORTED The device cannot support this API
 * @see tzsh_create()
 */
int tzsh_softkey_global_opacity_state_get(tzsh_softkey_h softkey,
                                          tzsh_softkey_state_opacity_e *opacity);

#ifdef  __cplusplus
}
#endif

/**
 * @}
 */

#endif
