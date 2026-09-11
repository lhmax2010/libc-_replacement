#ifndef _ECORE_WAYLAND2_H_
# define _ECORE_WAYLAND2_H_

# include <Eina.h>

# ifdef EAPI
#  undef EAPI
# endif

# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif

# ifdef __cplusplus
extern "C" {
# endif

#ifndef TIZEN_DEPRECATED_API
# ifdef TIZEN_DEPRECATION
#  define TIZEN_DEPRECATED_API __attribute__((__visibility__("default"), deprecated))
# else
#  define TIZEN_DEPRECATED_API
# endif
#endif

/**
 * @file
 * @brief Ecore functions for dealing with the Wayland display protocol
 *
 * @defgroup Ecore_Wl2_Group Ecore_Wl2 - Wayland integration
 * @ingroup Ecore
 *
 * Ecore_Wl2 provides a wrapper and convenience functions for using the
 * Wayland protocol in implementing a window system. Function groups for
 * this part of the library include the following:
 *
 * @li @ref Ecore_Wl2_Init_Group
 * @li @ref Ecore_Wl2_Display_Group
 * @li @ref Ecore_Wl2_Window_Group
 * @li @ref Ecore_Wl2_Input_Group
 * @li @ref Ecore_Wl2_Subsurface_Group
 */

#  ifndef _ECORE_WL2_WINDOW_PREDEF
/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Window
 * Type for Ecore_Wl2_Window
 */
typedef struct _Ecore_Wl2_Window Ecore_Wl2_Window;
#  endif
/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Display
 * Type for Ecore_Wl2_Display
 */
typedef struct _Ecore_Wl2_Display Ecore_Wl2_Display;

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Input
 * Type for Ecore_Wl2_Input
 */
typedef struct _Ecore_Wl2_Input Ecore_Wl2_Input;

// TIZEN_ONLY(20190807): Support for wl_egl interface
/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Egl_Window
 * Type for Ecore_Wl2_Egl_Window
 */
typedef struct _Ecore_Wl2_Egl_Window Ecore_Wl2_Egl_Window;
//

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Subsurface
 * Type for Ecore_Wl2_Subsurface
 */
typedef struct _Ecore_Wl2_Subsurface Ecore_Wl2_Subsurface;

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_VideoShell_Surface
 * Type for Ecore_Wl2_VideoShell_Surface
 */
typedef struct _Ecore_Wl2_VideoShell_Surface Ecore_Wl2_VideoShell_Surface;

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_VideoShell_Surface_Wrapper
 * Type for Ecore_Wl2_VideoShell_Surface_Wrapper
 */
typedef struct _Ecore_Wl2_VideoShell_Surface_Wrapper Ecore_Wl2_VideoShell_Surface_Wrapper;

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Frame_Cb
 * Type for Ecore_Wl2_Frame_Cb
 */
typedef void (*Ecore_Wl2_Frame_Cb)(Ecore_Wl2_Window *win, uint32_t timestamp, void *data);

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Frame_Cb_Handle
 * Type for Ecore_Wl2_Frame_Cb_Handle
 */
typedef struct _Ecore_Wl2_Frame_Cb_Handle Ecore_Wl2_Frame_Cb_Handle;

/**
 * @brief Defines the types to create a file descriptor
 * which is used for informing render event on the native surface
 *
 * @see ecore_wl2_window_render_sync_fd_create
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.0
 */
typedef enum _Ecore_Wl2_Window_Render_Sync_Type
{
   ECORE_WL2_WINDOW_RENDER_SYNC_COMMIT = 0, /**< To get commit event emitted after calling wl_surface_commit */
   ECORE_WL2_WINDOW_RENDER_SYNC_PRESENT = 1 /**< To get presentation event emitted after the compositor draws */
} Ecore_Wl2_Window_Render_Sync_Type;

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
//this enum for keyrouter and client window side
//keycode (8~255)
/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Window_Keygrab_Mode
 * Type for Ecore_Wl2_Window_Keygrab_Mode
 *
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode.
 *          don't use other modes. those are not for third-party application.
 */
typedef enum _Ecore_Wl2_Window_Keygrab_Mode
{
   ECORE_WL2_WINDOW_KEYGRAB_UNKNOWN = 0, /**< Unknown keygrab mode */
   ECORE_WL2_WINDOW_KEYGRAB_SHARED = 1, /**< Not for third-party application. Getting the grabbed-key together with the other client windows */
   ECORE_WL2_WINDOW_KEYGRAB_TOPMOST = 2, /**< Getting the grabbed-key only when window is top of the stack */
   ECORE_WL2_WINDOW_KEYGRAB_OVERRIDE_EXCLUSIVE = 3, /**< Not for third-party application Getting the grabbed-key exclusively regardless of window's position. Being overrided the grab by the other client window  */
   ECORE_WL2_WINDOW_KEYGRAB_EXCLUSIVE = 4 /**< Not for third-party application Getting the grabbed-key exclusively regardless of window's position */
} Ecore_Wl2_Window_Keygrab_Mode;

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Window_Keygrab_Info
 * Type for Ecore_Wl2_Window_Keygrab_Info
 */
typedef struct _Ecore_Wl2_Window_Keygrab_Info
{
   char *key;
   Ecore_Wl2_Window_Keygrab_Mode mode;
} Ecore_Wl2_Window_Keygrab_Info;
//

/**
 * @defgroup Ecore_Wl2_Init_Group Wayland Library Init and Shutdown Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that start and shutdown the Ecore Wl2 Library.
 */

/**
 * @brief Initialize the Ecore_Wl2 library
 *
 * @return  The number of times the library has been initialized without being
 *          shutdown. 0 is returned if an error occurs.
 *
 * @ingroup Ecore_Wl2_Init_Group
 * @since 1.17
 */
EAPI int ecore_wl2_init(void);

/**
 * @brief Shutdown the Ecore_Wl2 Library
 *
 * In shutting down the library, the Wayland display connection is terminated
 * and any event handlers for it are removed
 *
 * @return  The number of times the library has been initialized without
 *          being shutdown.
 *
 * @ingroup Ecore_Wl2_Init_Group
 * @since 1.17
 */
EAPI int ecore_wl2_shutdown(void);

/**
 * @defgroup Ecore_Wl2_Display_Group Wayland Library Display Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, connecting, or interacting with
 * Wayland displays
 */

/**
 * @brief Connect to an existing Wayland display
 *
 * This function is typically used by clients to connect to an
 * existing Wayland display.
 *
 * @param name The display target name to connect to. If @c NULL, the default
 *             display is assumed.
 *
 * @return The Ecore_Wl2_Display which was connected to
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI Ecore_Wl2_Display *ecore_wl2_display_connect(const char *name);

/**
 * @brief Disconnect an existing Wayland display
 *
 * This function is typically used by clients to disconnect from an
 * existing Wayland display.
 *
 * @param display The display to disconnect from
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI void ecore_wl2_display_disconnect(Ecore_Wl2_Display *display);

// TIZEN_ONLY(20190807): Retrieve the existing native wayland display
/**
 * @brief Retrieve the existing native wayland display
 *
 * @note The native wl_display struct is not included in the managed API list
 *       and is provided as a void pointer
 *       if the user needs to call a specific module function using that struct
 *
 * @param display The Ecore_Wl2_Display for which to retrieve the existing
 *                Wayland display from
 *
 * @return The wl_display which this Ecore_Wl2_Display is using
 *         It provided as void type
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI void *ecore_wl2_display_native_get(Ecore_Wl2_Display *display);
//

/**
 * @brief Retrieves the size of the current screen.
 *
 * @param display The display to get the screen size of
 * @param w where to return the width. May be NULL. Returns 0 on error.
 * @param h where to return the height. May be NULL. Returns 0 on error.
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI void ecore_wl2_display_screen_size_get(Ecore_Wl2_Display *display, int *w, int *h);

// TIZEN_ONLY(20220407): Gets the screen size of all screens
/**
 * @brief Gets the size of all screens.
 *
 * @param display The display to get all screens size of
 * @param w where to return the width. May be NULL. Returns 0 on error.
 * @param h where to return the height. May be NULL. Returns 0 on error.
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI void ecore_wl2_display_all_screens_size_get(Ecore_Wl2_Display *display, int *w, int *h);
//

/**
 * @brief Find an Ecore_Wl2_Window based on id
 *
 * @param display The display to search for the window
 * @param id The id of the window to find
 *
 * @return The Ecore_Wl2_Window if found, or NULL if no such window exists
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI Ecore_Wl2_Window *ecore_wl2_display_window_find(Ecore_Wl2_Display *display, unsigned int id);

/**
 * @brief Gets the connected display object
 *
 * This function is typically used by clients to get an
 * existing Wayland display.
 *
 * @param name The display target name. If @c NULL, the default
 *        display is assumed.
 *
 * @return The Ecore_Wl2_Display which was connected to
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.24
 */
EAPI Ecore_Wl2_Display *ecore_wl2_connected_display_get(const char *name);

/**
 * @brief Push buffered wayland protocol to compositor
 *
 * Wayland protocol is only actually sent when a flush occurs,
 * so the display should be flushed at appropriate times, such
 * as after a commit.
 *
 * @param display The display to flush
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.21
 */
EAPI void ecore_wl2_display_flush(Ecore_Wl2_Display *display);

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
/**
 * @brief The sync request asks the connected display server.
 *
 * Since requests are handled in-order and events are delivered in-order.
 *
 * @param display The display to sync
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI void ecore_wl2_display_sync(Ecore_Wl2_Display *display);
//

/**
 * @defgroup Ecore_Wl2_Window_Group Wayland Library Window Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland windows
 */

/**
 * @brief Create a new Ecore_Wl2_Window
 *
 * @param display The Ecore_Wl2_Display on which to create this new window
 * @param parent The Ecore_Wl2_Window which is the parent of this window
 * @param x Initial x position of window
 * @param y Initial y position of window
 * @param w Initial width of window
 * @param h Initial height of window
 *
 * @return The Ecore_Wl2_Window that newly creating, or NULL failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI Ecore_Wl2_Window *ecore_wl2_window_new(Ecore_Wl2_Display *display, Ecore_Wl2_Window *parent, int x, int y, int w, int h);

/**
 * @brief Get the window id associated with an Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window of which to retrieve the window id
 *
 * @return The id associated with this window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI int ecore_wl2_window_id_get(Ecore_Wl2_Window *window);

/**
 * @brief Show a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window to show
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_show(Ecore_Wl2_Window *window);

/**
 * @brief Hide a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window to hide
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_hide(Ecore_Wl2_Window *window);

/**
 * @brief Free a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window to free
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_free(Ecore_Wl2_Window *window);

/**
 * @brief Raise a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window which to raise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_raise(Ecore_Wl2_Window *window);

// TIZEN_ONLY(20171108): tizen window function
/**
 * @brief Lower the target window
 *
 * @note This API call causes the ECORE_WL2_EVENT_WINDOW_LOWER event
 *
 * @param window The target window
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void ecore_wl2_window_lower(Ecore_Wl2_Window *window);

/**
 * @brief Activate the target window
 *
 * @param window The target window
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void ecore_wl2_window_activate(Ecore_Wl2_Window *window);
//

/**
 * @brief Attach a buffer to a window
 *
 * Note that the GL stack my attach buffers to a surface - we should call this
 * function at that time (with a NULL buffer) to track whether a surface
 * has a valid buffer.  That is, call with implicit true and buffer NULL at
 * the time of glSwapBuffers.
 *
 * @param window The target window
 * @param buffer The buffer to attach
 * @param x X offset from corner
 * @param y Y offset from corner
 * @param implicit True if an external library is doing the actual attaching
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
EAPI void ecore_wl2_window_buffer_attach(Ecore_Wl2_Window *win, void *buffer, int x, int y, Eina_Bool implicit);

/**
 * @brief Specifies the area of the window's surface that needs updating
 *        internally calling wl_surface_damage
 *
 * @param window The target window
 * @param rects The rect area that will cause damage
 * @param count The number of rects
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void ecore_wl2_window_damage(Ecore_Wl2_Window *window, Eina_Rectangle *rects, unsigned int count);

/**
 * @brief Get if a given window is alpha
 *
 * @param window The window to get if is alpha
 *
 * @return EINA_TRUE if window is alpha, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI Eina_Bool ecore_wl2_window_alpha_get(Ecore_Wl2_Window *window);

/**
 * @brief Set a given window's alpha property
 *
 * @param window The window on which to set the alpha property
 * @param alpha EINA_TRUE to set window as alpha, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_alpha_set(Ecore_Wl2_Window *window, Eina_Bool alpha);

/**
 * @brief Set the title of a given window
 *
 * @param window The window to set the title of
 * @param title The title of the window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_title_set(Ecore_Wl2_Window *window, const char *title);

/**
 * @brief Get the title of a given window
 *
 * @param window The window to set the title of
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI const char *ecore_wl2_window_title_get(Ecore_Wl2_Window *window);

/**
 * @brief Get which display a given window is using
 *
 * @param window The window to get the display of
 *
 * @return The Ecore_Wl2_Display that this window is using, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI Ecore_Wl2_Display *ecore_wl2_window_display_get(const Ecore_Wl2_Window *window);

/**
 * @brief Set if this window should ignore focus requests
 *
 * @param window The window to set ignore focus requests on
 * @param focus_skip EINA_TRUE if this window should skip focus requests, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_focus_skip_set(Ecore_Wl2_Window *window, Eina_Bool focus_skip);

/**
 * @brief Get if this window ignores focus requests
 *
 * @param window The window to get the focus skip value from
 *
 * @return EINA_TRUE if a window should skip focus requests, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_window_focus_skip_get(Ecore_Wl2_Window *window);

//TIZEN_ONLY: ecore_wl2: add ecore_wl_window_video_has
/**
 * @brief Register the tizen protocol to provide video through the window's surface.
 *
 * @param window The target window
 * @param has EINA_TRUE to set window has video, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void ecore_wl2_window_video_has(Ecore_Wl2_Window *window, Eina_Bool has);
//

/**
 * @brief Commit the surface of a wayland window.
 *
 * If flush is set this generates a wl_surface_commit(), otherwise it is
 * expected that some other call in the very near future (such as
 * eglSwapBuffers) will cause an implicit flush.
 *
 * A surface that has been commit will be in the "pending" state until
 * the compositor tells us it's time to draw again via a frame callback.
 *
 * @param window The window whose surface we want to commit
 * @param flush EINA_TRUE if we need to flush immediately.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
EAPI void ecore_wl2_window_commit(Ecore_Wl2_Window *window, Eina_Bool flush);

/**
 * @brief Add a callback that fires when the window's surface_frame callback fires
 *
 * @param window The window to add a callback on
 * @param cb The callback
 * @param data User data to provide to the callback handler
 *
 * @return The Ecore_Wl2_Frame_Cb_Handle
 *
 * @see ecore_wl2_window_frame_callback_del
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
EAPI Ecore_Wl2_Frame_Cb_Handle *ecore_wl2_window_frame_callback_add(Ecore_Wl2_Window *window, Ecore_Wl2_Frame_Cb cb, void *data);

/**
 * @brief Delete a callback that fires when the window's surface_frame callback fires
 *
 * @param handle The callback handle obtained through ecore_wl2_window_frame_callback_add().
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
EAPI void ecore_wl2_window_frame_callback_del(Ecore_Wl2_Frame_Cb_Handle *handle);

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
/**
 * @platform
 * @brief Sets keygrab value of the window
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * This function grabs the @c key of window using @c grab_mode
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keygrab
 *
 * @param win The target Ecore_Wl2_Window
 * @param key The string is the keyname to grab
 * @param mod A combination of modifier keys that must be present to
 *              trigger the event. Not supported yet
 * @param not_mod A combination of modifier keys that must not be
 *                present to trigger the event. Not supported yet
 * @param priority Not supported yet
 * @param grab_mode Describes how the key should be grabbed, wrt. focus and
 *                  stacking
 *
 * @return @c true on success, @c false otherwise
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_ALREADY_IN_PROGRESS The key already has been grabbed
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_Bool ecore_wl2_window_keygrab_set(Ecore_Wl2_Window *win, const char *key, int mod EINA_UNUSED, int not_mod EINA_UNUSED, int priority EINA_UNUSED, Ecore_Wl2_Window_Keygrab_Mode grab_mode);

/**
 * @platform
 * @brief Unsets keygrab value of the window
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * This function unset keygrab value. Ungrab @c key of window.
 *
 * @privlevel platform
 * @privilege http://tizen.org/privilege/keygrab
 *
 * @param win The target Ecore_Wl2_Window
 * @param key The string is the keyname to grab
 * @param mod A combination of modifier keys that must be present to
 *              trigger the event. Not supported yet
 * @param not_mod A combination of modifier keys that must not be
 *                present to trigger the event. Not supported yet
 *
 * @return @c true on success, @c false otherwise
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_ALREADY_IN_PROGRESS The key already has been grabbed
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_Bool ecore_wl2_window_keygrab_unset(Ecore_Wl2_Window *win, const char *key, int mod EINA_UNUSED, int any_mod EINA_UNUSED);

/**
 * @platform
 * @brief Sets keygrab values of the window.
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * The returned list should be freed with @c eina_list_free() when you no
 * longer need it.
 *
 * @privlevel platform
 * @privilege http://tizen.org/privilege/keygrab
 * @param win The window
 * @param infos The list of keys and grab modes for setting keygrab
 *
 * @return NULL, or the list of keys being failed keygrab set.
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @Example
 *
 * Ecore_Wl2_Window_Keygrab_Info *info = malloc(sizeof(Ecore_Wl2_Window_Keygrab_Info));
 * info->key = "XF86AudioLowerVolume";
 * info->mode = ECORE_WL2_WINDOW_KEYGRAB_TOPMOST;
 * infos = eina_list_append(infos, info);
 *
 * ecore_wl2_window_keygrab_list_set(win, infos);
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_List *ecore_wl2_window_keygrab_list_set(Ecore_Wl2_Window *win, Eina_List *infos);

/**
 * @platform
 * @brief Unsets keygrab values of the window.
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * The returned list should be freed with @c eina_list_free() when you no
 * longer need it.
 *
 * @privlevel platform
 * @privilege http://tizen.org/privilege/keygrab
 *
 * @param win The window
 * @param infos The list of infos for unsetting keygrab
 *
 * @return NULL, or the list of keys being failed keygrab unset.
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @Example
 *
 * Ecore_Wl2_Window_Keygrab_Info *info = malloc(sizeof(Ecore_Wl2_Window_Keygrab_Info));
 * info->key = "XF86AudioLowerVolume";
 * infos = eina_list_append(infos, info);
 *
 * ecore_wl2_window_keygrab_list_unset(win, infos);
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_List *ecore_wl2_window_keygrab_list_unset(Ecore_Wl2_Window *win, Eina_List *infos);
//

// TIZEN_ONLY(20190807): Support for wl_egl interface
/**
 * @brief Creates wl_egl_window
 *
 * Create wl_egl_window through the connected wl_surface of ecore_wl2_window.
 *
 * @param window The Ecore_Wl2_Window
 * @param w The width of wl_egl_window
 * @param h The height of wl_egl_window
 *
 * @return The newly created Ecore_Wl2_Egl_Window
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Ecore_Wl2_Egl_Window *ecore_wl2_egl_window_create(Ecore_Wl2_Window *window, int w, int h);

/**
 * @brief Destroy a given Ecore_Wl2_Egl_Window
 *
 * @param egl_win The Ecore_Wl2_Egl_Window to destroy
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void                  ecore_wl2_egl_window_destroy(Ecore_Wl2_Egl_Window *egl_win);

/**
 * @brief Gets the wl_egl_window that this ecore_wl2_egl_window is using.
 *
 * Cast the wl_egl_window struct pointer not included in the Managed API to void pointer
 * This pointer can be used as an argument to the function passed to wl_egl_window.
 *
 * @param egl_win The Ecore_Wl2_Egl_Window
 *
 * @return The wl_egl_window, it provided as void type
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void                 *ecore_wl2_egl_window_native_get(Ecore_Wl2_Egl_Window *egl_win);

/**
 * @brief Ecore_Wl2_Egl_Window resize with rotation
 *
 * @param egl_win The Ecore_Wl2_Egl_Window
 * @param dx The x coord resize
 * @param dy The y coord resize
 * @param w The width of Ecore_Wl2_Egl_Window
 * @param h The height of Ecore_Wl2_Egl_Window
 * @param rotation The angle of rotation
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void                  ecore_wl2_egl_window_resize_with_rotation(Ecore_Wl2_Egl_Window *egl_win, int dx, int dy, int w, int h, int rotation);
//

//TIZEN_ONLY(20200309): The 3rd party app may needs native surface handle.
/**
 * @brief Get the native surface(wl_surface) which belongs to this window
 *
 * @note The native wl_surface struct is not included in the managed API list
 *       and is provided as a void pointer
 *
 * @param window The Ecore_Wl2_Window to get the surface of
 *
 * @return The wl_surface associated with this window
 *         It provided as void type
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void *ecore_wl2_window_native_surface_get(Ecore_Wl2_Window *window) TIZEN_DEPRECATED_API;
//

/**
 * @deprecated Deprecated since 10.0.
 * @brief Create a video surface for a given window
 *
 * @param win The window to create a video surface
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.0
 */
EAPI Eina_Bool ecore_wl2_window_video_surface_create(Ecore_Wl2_Window *win) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Get video surface for a given window
 *
 * @param win The window to get video surface
 * @return The wl_surface which is used by this window as a video surface
 *         It provided as void type
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.0
 */
EAPI void *ecore_wl2_window_video_surface_get(Ecore_Wl2_Window *win) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Destroy a video surface for a given window
 *
 * @param win The window to destroy a video surface
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.0
 */
EAPI Eina_Bool ecore_wl2_window_video_surface_destroy(Ecore_Wl2_Window *win) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set video surface for a given window to synchronized mode
 *        the default commit behaviour is synchronized mode.
 *
 * @param win The window to set video surface to synchronized mode
 * @param sync EINA_TRUE to set video surface to synchronized mode,
 *             EINA_FALSE to set video surface to desynchronized mode
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.0
 */
EAPI Eina_Bool ecore_wl2_window_video_surface_sync_set(Ecore_Wl2_Window *win, Eina_Bool sync) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set destination area of video surface for a given window
 *
 * @param win The window to set destination area of video surface
 * @param x  X position of destination area
 * @param y  Y position of destination area
 * @param w  Width of destination area
 * @param h  Height of destination area
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.0
 */
EAPI Eina_Bool ecore_wl2_window_video_surface_destination_set(Ecore_Wl2_Window *win, int x, int y, int w, int h) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 10.0.
 * @brief Create a file descriptor for rendering sync for a given window
 *
 * @param win The window to create a file descriptor for rendering sync
 * @param sync_type The one of #Ecore_Wl2_Window_Render_Sync_Type to create a file descriptor
 * @return The file descriptor. It must be closed manually  by the caller using `close`
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.0
 */
EAPI int ecore_wl2_window_render_sync_fd_create(Ecore_Wl2_Window *win, Ecore_Wl2_Window_Render_Sync_Type sync_type);
//

/**
 * @defgroup Ecore_Wl2_Input_Group Wayland Library Input Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Input
 */

/**
 * Gets default input of a given display
 *
 * @param ewd The display
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.24
 */
EAPI Ecore_Wl2_Input *ecore_wl2_input_default_input_get(const Ecore_Wl2_Display *ewd);

/**
 * Retrieves the mouse position of the seat
 *
 * @param input The seat
 * @param x where to return the horizontal position. May be NULL. Returns 0 on error.
 * @param y where to return the vertical position. May be NULL. Returns 0 on error.
 * @return @c EINA_TRUE if coords were successfully retrieved
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_input_pointer_xy_get(const Ecore_Wl2_Input *input, int *x, int *y) TIZEN_DEPRECATED_API;

// TIZEN_ONLY(20201022): support restack video
/**
 * @defgroup Ecore_Wl2_Subsurface_Group Functions to manipulate subsurfaces.
 * @ingroup Ecore_Wl2_Group
 *
 * Functions to manipulate wayland subsurfaces, using Ecore_Wl2_Subsurface.
 *
 * This API is intended to expose Wayland subsurface functionality, although it
 * should not be necessary for most applications to use it, as soon as we have
 * means to make Evas automatically switch Evas images to use subsurfaces.
 *
 * It can/should be used, for instance, when subsurfaces are needed to be not
 * in sync with the main window surface.
 */

/**
 * @deprecated Deprecated since 10.0.
 * @brief Create and return a new subsurface.
 *
 * Create a new surface (and subsurface interface), with the parent surface
 * being the one associated with the given window.
 *
 * @param window The window. It must be visible, otherwise there will be no surface created
 * for it yet.
 *
 * @return the allocated and initialized Ecore_Wl2_Subsurface object, or
 * NULL on failure
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since 1.17
 * @since_tizen 6.0
 */
EAPI Ecore_Wl2_Subsurface * ecore_wl2_subsurface_new(Ecore_Wl2_Window *window) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Destroy the given subsurface, as well as the surface associated with it.
 *
 * @param subsurface the subsurface
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since 1.17
 * @since_tizen 6.0
 */
EAPI void ecore_wl2_subsurface_del(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Enables or disables sub-surface synchronization
 *
 * When synchronization is enabled, surface commits on the subsurface
 * will be cached and only applied when the parent surface's state is
 * applied.  This ensures atomic updates of the parent and all of its
 * synchronized sub-surfaces.
 *
 * When synchronization is disabled, commits will apply to the pending
 * state directly without caching, just like a normal wl_surface.  If
 * there are already cached events when this is set, those events are
 * applied simultaneously with the desync event.
 *
 * Attempting to enable synchronization when the subsurface already
 * thinks it's sync'd, or desync when it believes its desync'd, will
 * be trivially ignored and will not generate a Wayland event.
 *
 * See Wayland's set_desync documentation for further details and
 * exceptional cases.
 *
 * @param subsurface the subsurface
 * @param sync true to enable synchronization, false to desynchronize
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since 1.17
 * @since_tizen 6.0
 */
EAPI void ecore_wl2_subsurface_sync_set(Ecore_Wl2_Subsurface *subsurface, Eina_Bool sync) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Get the wl_surface for this subsurface
 *
 * @param subsurface the subsurface
 *
 * @return the wl_surface associated with this subsurface, or NULL on failure,
 *         provided as void type
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
EAPI void *ecore_wl2_subsurface_native_surface_get(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Get parent window for this subsurface
 *
 * @param subsurface the subsurface
 *
 * @return the Ecore_Wl2_Window associated with this subsurface, or NULL on failure.
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI Ecore_Wl2_Window *ecore_wl2_subsurface_window_get(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Place subsurface on layer above a reference subsurface
 *
 * Moves the subsurface to just above the reference subsurface,
 * changing the z-order.  The reference subsurface must
 * be either a sibling or parent surface, else a protocol error will
 * be generated.
 *
 * @param subsurface the subsurface
 * @param other the sibling reference subsurface, or NULL for parent surface
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
EAPI void ecore_wl2_subsurface_place_surface_above(Ecore_Wl2_Subsurface *subsurface, Ecore_Wl2_Subsurface *other) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Place subsurface on layer below a reference subsurface
 *
 * See ecore_wl2_subsurface_place_surface_above.
 *
 * @param subsurface the subsurface
 * @param other the sibling reference subsurface, or NULL for parent surface
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
EAPI void ecore_wl2_subsurface_place_surface_below(Ecore_Wl2_Subsurface *subsurface, Ecore_Wl2_Subsurface *other) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Prepare video surface for a given subsurface
 *
 * @param subsurface The subsurface to prepare video surface
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
EAPI Eina_Bool ecore_wl2_subsurface_video_surface_prepare(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set destination area of video surface for a given subsurface
 *
 * @param subsurface The subsurface to set destination area of video surface
 * @param x  X position of destination area
 * @param y  Y position of destination area
 * @param w  Width of destination area
 * @param h  Height of destination area
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
EAPI Eina_Bool ecore_wl2_subsurface_video_surface_destination_set(Ecore_Wl2_Subsurface *subsurface, int x, int y, int w, int h) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Defines the rotation types of video surface
 *
 * @see ecore_wl2_subsurface_video_surface_rotation_set
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
typedef enum _Ecore_Wl2_Subsurface_Rotation
{
   ECORE_WL2_SUBSURFACE_ROTATION_NORMAL, /**< No rotation */
   ECORE_WL2_SUBSURFACE_ROTATION_90, /**< 90 degree counter-clockwise */
   ECORE_WL2_SUBSURFACE_ROTATION_180, /**< 180 degree counter-clockwise */
   ECORE_WL2_SUBSURFACE_ROTATION_270, /**< 270 degree counter-clockwise */
   ECORE_WL2_SUBSURFACE_ROTATION_FLIPPED, /**< 180 degree flip around a vertical axis */
   ECORE_WL2_SUBSURFACE_ROTATION_FLIPPED_90, /**< Flip and rotate 90 degree counter-clockwise */
   ECORE_WL2_SUBSURFACE_ROTATION_FLIPPED_180, /**< Flip and rotate 180 degree counter-clockwise */
   ECORE_WL2_SUBSURFACE_ROTATION_FLIPPED_270 /**< Flip and rotate 270 degree counter-clockwise */
} Ecore_Wl2_Subsurface_Rotation;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set video surface rotation for a given subsurface
 *
 * @param subsurface The subsurface to set video surface rotation
 * @param rotation  The rotation type to set video surface rotation
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
EAPI Eina_Bool ecore_wl2_subsurface_video_surface_rotation_set(Ecore_Wl2_Subsurface *subsurface, Ecore_Wl2_Subsurface_Rotation rotation) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set an auxiliary hint on a given subsurface
 *
 * @param subsurface The subsurface to set an auxiliary hint
 * @param hint The auxiliary hint string
 * @param val The value string
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.0
 */
EAPI void ecore_wl2_subsurface_aux_hint_set(Ecore_Wl2_Subsurface *subsurface, const char *hint, const char *val) TIZEN_DEPRECATED_API;
//


//TIZEN_ONLY(20210503): support foreign shell
/**
 * @deprecated Deprecated since 10.0.
 * @brief Export given subsurface
 *
 * @param subsurface The subsurface to export
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI Eina_Bool ecore_wl2_subsurface_export(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Get exported surface handle for a given subsurface
 *
 * @param subsurface The subsurface to get exported surface handle
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI const char* ecore_wl2_subsurface_exported_surface_handle_get(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Show exported surface for a given subsurface
 *
 * @param subsurface The subsurface to show exported surface
 *
 * @note This change won't be applied unless parent wl_surface.commit,
 *       if commit behaviour is synchronized mode which can be changed
 *       by ecore_wl2_subsurface_exported_surface_sync_set
 *
 * @see ecore_wl2_subsurface_exported_surface_commit
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_show(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Hide exported surface for a given subsurface
 *
 * @param subsurface The subsurface to hide exported surface
 *
 * @note This change won't be applied unless parent wl_surface.commit,
 *       if commit behaviour is synchronized mode which can be changed
 *       by ecore_wl2_subsurface_exported_surface_sync_set
 *
 * @see ecore_wl2_subsurface_exported_surface_commit
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_hide(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Place exported surface for a given subsurface on layer above a reference subsurface
 *
 * @param subsurface the subsurface
 * @param other the sibling reference subsurface, or NULL for parent surface
 * @see ecore_wl2_subsurface_place_surface_above
 *
 * @note Make sure that parent wl_surface.commit. This change won't be applied
 *       unless parent wl_surface.commit.
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_place_above(Ecore_Wl2_Subsurface *subsurface, Ecore_Wl2_Subsurface *other) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Place exported surface for a given subsurface on layer below a reference subsurface
 *
 * @param subsurface the subsurface
 * @param other the sibling reference subsurface, or NULL for parent surface
 * @see ecore_wl2_subsurface_place_surface_above
 *
 * @note Make sure that parent wl_surface.commit. This change won't be applied
 *       unless parent wl_surface.commit.
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_place_below(Ecore_Wl2_Subsurface *subsurface, Ecore_Wl2_Subsurface *other) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set position of exported surface for a given subsurface
 *
 * @param subsurface the subsurface
 * @param x X position of exported surface
 * @param y Y position of exported surface
 *
 * @note This change won't be applied unless parent wl_surface.commit,
 *       if commit behaviour is synchronized mode which can be changed
 *       by ecore_wl2_subsurface_exported_surface_sync_set
 *
 * @see ecore_wl2_subsurface_exported_surface_commit
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_move(Ecore_Wl2_Subsurface *subsurface, int x, int y) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set size of exported surface for a given subsurface
 *
 * @param subsurface the subsurface
 * @param w Width of exported surface
 * @param h Height of exported surface
 *
 * @note This change won't be applied unless parent wl_surface.commit,
 *       if commit behaviour is synchronized mode which can be changed
 *       by ecore_wl2_subsurface_exported_surface_sync_set
 *
 * @see ecore_wl2_subsurface_exported_surface_commit
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_resize(Ecore_Wl2_Subsurface *subsurface, int w, int h) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set transform of exported surface for a given subsurface
 *
 * @param subsurface the subsurface
 * @param transform The exported surface transform being requested
 * The available values of transform are:
 * <ul style="list-style-type:none">
 *     <li>0 - no transform (WTZ_EXPORTED_SHELL_TRANSFORM_NORMAL)</li>
 *     <li>1 - 90 degrees clockwise (WTZ_EXPORTED_SHELL_TRANSFORM_90)</li>
 *     <li>2 - 180 degrees clockwise (WTZ_EXPORTED_SHELL_TRANSFORM_180)</li>
 *     <li>3 - 270 degrees clockwise (WTZ_EXPORTED_SHELL_TRANSFORM_270)</li>
 * </ul>
 *
 * @note This change won't be applied unless parent wl_surface.commit,
 *       if commit behaviour is synchronized mode which can be changed
 *       by ecore_wl2_subsurface_exported_surface_sync_set
 *
 * @see ecore_wl2_subsurface_exported_surface_commit
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_transform_set(Ecore_Wl2_Subsurface *subsurface, int transform) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Enables or disables exported surface synchronization for a given subsurface
 *        the default commit behaviour is synchronized mode.
 *
 * @param subsurface the subsurface
 * @param sync true to enable synchronization, false to desynchronize
 *
 * @see ecore_wl2_subsurface_sync_set
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_sync_set(Ecore_Wl2_Subsurface *subsurface, Eina_Bool sync) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Commit exported surface for a given subsurface
 *
 * This generates a wl_surface_commit() immediately
 *
 * @param subsurface the subsurface
 *
 * @ingroup Ecore_Wl2_Subsurface_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_subsurface_exported_surface_commit(Ecore_Wl2_Subsurface *subsurface) TIZEN_DEPRECATED_API;
//

/**
 * @deprecated Deprecated since 10.0.
 * @brief Set the background blur radius of a given window
 *
 * @param window The window to set the background blur radius
 * @param radius The radius of the blur effect
 * @param rx  The radius of the rounded corners in x direction
 * @param ry  The radius of the rounded corners in y direction
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 9.0
 */
EAPI void ecore_wl2_window_background_blur_set(Ecore_Wl2_Window *window, int radius, int rx, int ry) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Get the background blur radius of a given window
 *
 * @param window The window to get the background blur radius
 *
 * @return radius The radius of the blur effect
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 9.0
 */
EAPI int ecore_wl2_window_background_blur_get(Ecore_Wl2_Window *window) TIZEN_DEPRECATED_API;

// TIZEN_ONLY(20240910) : supports behind blur
/**
 * @deprecated Deprecated since 10.0.
 * @brief Sets the behind blur radius of a given window
 *
 * @param window The window to set the behind blur radius
 * @param radius The radius of the blur effect
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 9.0
 */
EAPI void ecore_wl2_window_behind_blur_set(Ecore_Wl2_Window *window, int radius) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the behind blur radius of a given window
 *
 * @param window The window to get the behind blur radius
 *
 * @return radius The radius of the blur effect
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 9.0
 */
EAPI int ecore_wl2_window_behind_blur_get(Ecore_Wl2_Window *window);
//

/**
 * @deprecated Deprecated since 10.0.
 * @brief Sets the behind dim color of a given window
 *
 * @param window The window to set the behind dim color
 * @param enable True to enable the behind dim color
 * @param r The red channel of the behind dim effect
 * @param g The green channel of the behind dim effect
 * @param b The blue channel of the behind dim effect
 * @param a The alpha channel of the behind dim effect
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 10.0
 */
EAPI void ecore_wl2_window_behind_dim_set(Ecore_Wl2_Window * window, int enable, int r, int g, int b, int a);
/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the behind dim color of a given window
 *
 * @param window The window to get the behind dim color
 *
 * @param enable True to enable the behind dim color
 * @param r The red channel of the behind dim effect
 * @param g The green channel of the behind dim effect
 * @param b The blue channel of the behind dim effect
 * @param a The alpha channel of the behind dim effect
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 10.0
 */
EAPI void ecore_wl2_window_behind_dim_get(Ecore_Wl2_Window *window, int *enable, int *r, int *g, int *b, int *a);

////////////////////////////////////////////////////////////////////////////////////////////
/// Supports new video shell

/**
* Creates a new Video Shell Surface object
*
* @param[in] window Window to create the video shell surface for
*
* @return A newly created Ecore_Wl2_VideoShell_Surface object or NULL if an error occurred
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI Ecore_Wl2_VideoShell_Surface * Ecore_Wl2_VideoShell_Surface_new(Ecore_Wl2_Window *window);

/**
* Frees the memory used by the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to be freed
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_del(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Retrieves the handle associated with the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to retrieve the handle from
*
* @return The handle associated with the given Video Shell Surface object
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI const char * Ecore_Wl2_VideoShell_Surface_handle_get(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Retrieves the underlying Wayland surface associated with the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to retrieve the surface from
*
* @return The underlying Wayland surface associated with the given Video Shell Surface object
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI struct wl_surface * Ecore_Wl2_VideoShell_Surface_surface_get(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Maps (shows) the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to map
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_map(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Unmaps (hides) the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to unmap
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_unmap(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Places the given Video Shell Surface object above its sibling in the stacking order
*
* @param[in] videoShellSurface The Video Shell Surface object to move up
* @param[in] sibling The sibling Video Shell Surface object to use as reference point
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_place_above(Ecore_Wl2_VideoShell_Surface *videoShellSurface, Ecore_Wl2_VideoShell_Surface *sibling);

/**
* Places the given Video Shell Surface object below its sibling in the stacking order
*
* @param[in] videoShellSurface The Video Shell Surface object to move down
* @param[in] sibling The sibling Video Shell Surface object to use as reference point
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_place_below(Ecore_Wl2_VideoShell_Surface *videoShellSurface, Ecore_Wl2_VideoShell_Surface *sibling);

/**
* Sets the position of the given Video Shell Surface object relative to its parent
*
* @param[in] videoShellSurface The Video Shell Surface object to set the position for
* @param[in] x X coordinate of the position
* @param[in] y Y coordinate of the position
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_set_position(Ecore_Wl2_VideoShell_Surface *videoShellSurface, int32_t x, int32_t y);

/**
* Sets the destination size of the given Video Shell Surface object within its parent
*
* @param[in] videoShellSurface The Video Shell Surface object to set the destination size for
* @param[in] width Width of the destination area
* @param[in] height Height of the destination area
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_set_destination(Ecore_Wl2_VideoShell_Surface *videoShellSurface, int32_t width, int32_t height);

/**
* Sets the transformation of the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to set the transformation for
* @param[in] transform Transformation value (@see #Ecore_Wl2_Transform)
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_set_transform(Ecore_Wl2_VideoShell_Surface *videoShellSurface, int32_t transform);

/**
* Enables sync mode for the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to enable sync mode for
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_set_sync(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Enables desync mode for the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to disable sync mode for
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_set_desync(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Commits changes made to the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to commit changes for
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_commit(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Creates a wrapper around the given Video Shell Surface object
*
* @param[in] videoShellSurface The Video Shell Surface object to wrap
*
* @return A newly created Ecore_Wl2_VideoShell_Surface_Wrapper object or NULL if an error occurred
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI Ecore_Wl2_VideoShell_Surface_Wrapper * Ecore_Wl2_VideoShell_Surface_wrapper_new(Ecore_Wl2_VideoShell_Surface *videoShellSurface);

/**
* Destroys the given Video Shell Surface Wrapper object and releases all resources associated with it.
* Must be destroyed before Ecore_Wl2_VideoShell_Surface is destroyed.
*
* @param[in] wrapper The Video Shell Surface Wrapper object to destroy
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_wrapper_destroy(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper);

/**
* Updates the geometry of the given Video Shell Surface Wrapper object
*
* @param[in] wrapper The Video Shell Surface Wrapper object to update the geometry for
* @param[in] x X coordinate of the top left corner of the geometry rectangle
* @param[in] y Y coordinate of the top left corner of the geometry rectangle
* @param[in] width Width of the geometry rectangle
* @param[in] height Height of the geometry rectangle
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_wrapper_update_geometry(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper, int32_t x, int32_t y, int32_t width, int32_t height);

/**
* Commits changes made to the given Video Shell Surface Wrapper object
*
* @param[in] wrapper The Video Shell Surface Wrapper object to commit changes for
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
void Ecore_Wl2_VideoShell_Surface_wrapper_commit(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper);

/**
* Retrieves the handle associated with the given Video Shell Surface Wrapper object
*
* @param[in] videoShellSurface The Video Shell Surface Wrapper object to retrieve the handle from
*
* @return The handle associated with the given Video Shell Surface Wrapper object
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI char * Ecore_Wl2_VideoShell_Surface_wrapper_handle_get(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper);

/**
* Reference a Video Shell Surface Wrapper object.
*
* @param[in] videoShellSurface The Video Shell Surface Wrapper object to reference.
*
* @return The Video Shell Surface Wrapper object
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI Ecore_Wl2_VideoShell_Surface_Wrapper * Ecore_Wl2_VideoShell_Surface_wrapper_ref(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper);

/**
* Unreference a Video Shell Surface Wrapper object.
*
* @param[in] videoShellSurface The Video Shell Surface Wrapper object to unreference.
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_wrapper_unref(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper);

/**
* Sets the transformation of the given Video Shell Surface Wrapper object
*
* @param[in] videoShellSurface The Video Shell Surface Wrapper object to set the transformation for
* @param[in] transform Transformation value
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_wrapper_set_transform(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper, int32_t transform);

/**
* Updates the window's information of the given Video Shell Surface Wrapper object
*
* @param[in] wrapper The Video Shell Surface Wrapper object to update the window's geometry information.
* @param[in] win_width current window's width
* @param[in] win_height current window's height
* @param[in] win_orientation current window's orientation
*
* @ingroup Ecore_Wl2_VideoShell_Surface_Wrapper_Group
* @since_tizen 9.0
*/
EAPI void Ecore_Wl2_VideoShell_Surface_wrapper_update_win_info(Ecore_Wl2_VideoShell_Surface_Wrapper *wrapper, int32_t win_width, int32_t win_height, int win_orientation);

# undef EAPI
# define EAPI

# ifdef __cplusplus
}
# endif

#endif //_ECORE_WAYLAND2_H_

