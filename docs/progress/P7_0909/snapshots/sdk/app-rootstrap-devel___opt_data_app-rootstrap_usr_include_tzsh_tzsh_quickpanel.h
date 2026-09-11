#ifndef _TZSH_QUICKPANEL_H_
#define _TZSH_QUICKPANEL_H_

/**
 * @addtogroup TIZEN_WS_SHELL_QUICKPANEL_MODULE
 * @{
 */

#include <tzsh.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief Tizen event type for the quickpanel visible state.
 * @since_tizen 3.0
 * @remarks This event occurs when the visible state of the quickpanel service window is changed.
 *          The value identifying this event type is generated and written to the
 *          TZSH_QUICKPANEL_EVENT_VISIBLE global variable upon tzsh_quickpanel_create_with_type() call.
 *          Then this variable can be used to identify a type when setting a callback and checking
 *          event type in a callback.
 */
extern int TZSH_QUICKPANEL_EVENT_VISIBLE;

/**
 * @brief Tizen event type for the quickpanel orientation.
 * @since_tizen 3.0
 * @remarks This event occurs when the orientation of the quickpanel service window is changed.
 *          The value identifying this event type is generated and written to the
 *          TZSH_QUICKPANEL_EVENT_ORIENTATION global variable upon tzsh_quickpanel_create_with_type() call.
 *          Then this variable can be used to identify a type when setting a callback and checking
 *          event type in a callback.
 */
extern int TZSH_QUICKPANEL_EVENT_ORIENTATION;


/**
 * @brief Handle for the Tizen quickpanel client.
 * @since_tizen 3.0
 */
typedef struct _tzsh_quickpanel_s *tzsh_quickpanel_h;

/**
 * @brief Handle for the event handler of the quickpanel.
 * @details This is the handler for tzsh_quickpanel_event. It is created and returned from #tzsh_quickpanel_event_handler_add and used to manage the lifecycle of the event handler.
 * @since_tizen 3.0
 */
typedef struct _tzsh_event_handler_s *tzsh_quickpanel_event_handler_h;

/**
 * @brief Handle for the event information of the quickpanel.
 * @details This is the handler used to obtain information about events on #tzsh_quickpanel_event_cb.
 * @since_tizen 3.0
 * @see tzsh_quickpanel_event_visible_get()
 * @see tzsh_quickpanel_event_orientation_get()
 */
typedef struct _tzsh_quickpanel_event_info_s *tzsh_quickpanel_event_info_h;

/**
 * @brief Enumeration for visible state of quickpanel.
 * @since_tizen 3.0
 */
typedef enum
{
   TZSH_QUICKPANEL_STATE_VISIBLE_UNKNOWN = 0x0, /**< Unknown state. There is no quickpanel service. */
   TZSH_QUICKPANEL_STATE_VISIBLE_SHOWN = 0x1, /**< Shown state. */
   TZSH_QUICKPANEL_STATE_VISIBLE_HIDDEN = 0x2, /**< Hidden state. */
} tzsh_quickpanel_state_visible_e;

/**
 * @brief Enumeration for scrollable state of quickpanel.
 * @since_tizen 3.0
 */
typedef enum
{
   TZSH_QUICKPANEL_STATE_SCROLLABLE_UNKNOWN = 0x0, /**< Unknown state. There is no quickpanel service. */
   TZSH_QUICKPANEL_STATE_SCROLLABLE_SET = 0x1, /**< Scrollable state. */
   TZSH_QUICKPANEL_STATE_SCROLLABLE_UNSET = 0x2, /**< Not scrollable state. */
   TZSH_QUICKPANEL_STATE_SCROLLABLE_RETAIN = 0x3, /**< Retain scrollable state. (@b Since: 5.5) */
} tzsh_quickpanel_state_scrollable_e;

/**
 * @brief Enumeration for orientation state of quickpanel.
 * @since_tizen 3.0
 */
typedef enum
{
   TZSH_QUICKPANEL_STATE_ORIENTATION_UNKNOWN = 0x0, /**< Unknown state. There is no quickpanel service. */
   TZSH_QUICKPANEL_STATE_ORIENTATION_0 = 0x1, /**< 0 */
   TZSH_QUICKPANEL_STATE_ORIENTATION_90 = 0x2, /**< 90 */
   TZSH_QUICKPANEL_STATE_ORIENTATION_180 = 0x3, /**< 180 */
   TZSH_QUICKPANEL_STATE_ORIENTATION_270 = 0x4, /**< 270 */
} tzsh_quickpanel_state_orientation_e;

/**
 * @brief Enumeration for type of quickpanel.
 * @since_tizen 5.5
 */
typedef enum
{
   TZSH_QUICKPANEL_TYPE_UNKNOWN = 0x0, /**< Unknown type. There is no quickpanel service. */
   TZSH_QUICKPANEL_TYPE_SYSTEM_DEFAULT = 0x1, /**< System default type. */
   TZSH_QUICKPANEL_TYPE_CONTEXT_MENU = 0x2, /**< Context menu type. */
   TZSH_QUICKPANEL_TYPE_APPS_MENU = 0x3, /**< Apps menu type. (Since 6.0) */
} tzsh_quickpanel_type_e;

/**
 * @brief Called when a quickpanel event occurs.
 * @details This is the interface of the callback function that is called when the state of the quickpanel service window changes.
 *          When the value of the event type set during #tzsh_quickpanel_event_handler_add is changed,
 *          the registered function as an event callback is invoked,
 *          allowing you to retrieve each event type and its corresponding change values within the callback function for necessary actions.
 * @since_tizen 3.0
 * @remarks The @a event_info handle is only valid until the callbacks exits.
 *          After the callback exits, the @a event_info handle is not valid and should not be used.
 *          The @a event_info should not be released.
 * @param[in] type The event type (#TZSH_QUICKPANEL_EVENT_VISIBLE, #TZSH_QUICKPANEL_EVENT_ORIENTATION)
 * @param[in] event_info The #tzsh_quickpanel_event_info_h handle
 * @param[in] user_data The user data
 * @par Example
 * @code
 *  #include <tzsh_quickpanel.h>
 *
 *  tzsh_quickpanel_h qp = NULL;
 *  tzsh_quickpanel_event_handler_h handler[2] = {NULL, NULL};
 *
 *  static void _cb_vis_changed (int type, tzsh_quickpanel_event_info_h ev_info, void *data)
 *  {
 *      tzsh_quickpanel_state_visible_e state;
 *
 *      if (type != TZSH_QUICKPANEL_EVENT_VISIBLE)
 *      {
 *          return;
 *      }
 *
 *      tzsh_quickpanel_event_visible_get(ev_info, &state);
 *      if (state == TZSH_QUICKPANEL_STATE_VISIBLE_SHOWN)
 *      {
 *          // quick panel is shown
 *      }
 *      else if (state == TZSH_QUICKPANEL_STATE_VISIBLE_HIDDEN)
 *      {
 *          // quick panel is hidden
 *      }
 *      else
 *      {
 *          // quick panel's state is unknown
 *      }
 *  }
 *
 *  static void _cb_orientation_changed(int type, tzsh_quickpanel_event_info_h ev_info, void *data)
 *  {
 *      tzsh_quickpanel_state_orientation_e state;
 *
 *      if (type != TZSH_QUICKPANEL_EVENT_ORIENTATION)
 *      {
 *          return;
 *      }
 *
 *      tzsh_quickpanel_event_orientation_get(ev_info, &state);
 *      switch (state)
 *      {
 *          case TZSH_QUICKPANEL_STATE_ORIENTATION_0:
 *              // quickpanel is 0 degree
 *              break;
 *
 *          case TZSH_QUICKPANEL_STATE_ORIENTATION_90:
 *              // quickpanel is 90 degree
 *              break;
 *
 *          case TZSH_QUICKPANEL_STATE_ORIENTATION_180:
 *              // quickpanel is 180 degree
 *              break;
 *
 *          case TZSH_QUICKPANEL_STATE_ORIENTATION_270:
 *              // quickpanel is 270 degree
 *              break;
 *
 *          default:
 *              // quickpanel's degree is unknown
 *              break;
 *      }
 *  }
 *
 *  void create_window(void)
 *  {
 *      ...
 *
 *      qp = tzsh_quickpanel_create_with_type(tzsh, tz_win,
 *                                            TZSH_QUICKPANEL_TYPE_SYSTEM_DEFAULT);
 *      ...
 *
 *     handler[0] = tzsh_quickpanel_event_handler_add(qp, TZSH_QUICKPANEL_EVENT_VISIBLE, _cb_vis_changed, NULL);
 *     handler[1] = tzsh_quickpanel_event_handler_add(qp, TZSH_QUICKPANEL_EVENT_ORIENTATION, _cb_orientation_changed, NULL);
 *     ...
 *  }
 * @endcode
 */
 typedef void (*tzsh_quickpanel_event_cb)(int type,
                                          tzsh_quickpanel_event_info_h event_info,
                                          void *user_data);

/**
 * @deprecated Deprecated since 5.5. Use the return value of tzsh_quickpanel_create_with_type() instead.
 * @brief Creates a #tzsh_quickpanel_h object which handles the quickpanel service.
 * @details This is the creator of the tzsh_quickpanel handler for the quickpanel clients.
 *          It creates and returns a handler of the tzsh_quickpanel using the given tzsh handler and the native window to utilize tzsh_quickpanel.
 *          By default, it generates with #TZSH_QUICKPANEL_TYPE_SYSTEM_DEFAULT.
 *          To acquire the handler of a specified quickpanel type, should be used #tzsh_quickpanel_create_with_type.
 * @since_tizen 3.0
 * @remarks The returned handle should be released using tzsh_quickpanel_destroy().
 *          The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 *          Since 5.5, this function creates #tzsh_quickpanel_h object only with system default type.
 * @param[in] tzsh The #tzsh_h instance
 * @param[in] win The native window
 * @return #tzsh_quickpanel_h object on success, @c NULL otherwise
 * @exception #TZSH_ERROR_NONE Successful
 * @exception #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre You have to create a #tzsh_h instance using tzsh_create() before calling this function.
 *      You can get the #tzsh_window instance using the toolkit API such as elm_win_window_id_get().
 * @see tzsh_create()
 * @see tzsh_quickpanel_create_with_type()
 * @see tzsh_quickpanel_destroy()
 * @par Example
 * @code
 *  #include <tzsh_quickpanel.h>
 *
 *  tzsh_h tzsh = NULL;
 *  tzsh_quickpanel_h qp = NULL;
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
 *      qp = tzsh_quickpanel_create(tzsh, tz_win);
 *      ...
 *  }
 * @endcode
 */
tzsh_quickpanel_h tzsh_quickpanel_create(tzsh_h tzsh,
                                         tzsh_window win) TIZEN_DEPRECATED_API;

/**
 * @brief Creates a #tzsh_quickpanel_h object for a specific #tzsh_quickpanel_type_e
 *        which handles the quickpanel service.
 * @details This is the creator of the tzsh_quickpanel handler for clients requiring a specific type of quick panel.
 *          Using the given tzsh handler, the native window to utilize tzsh_quickpanel, and the type of target quickpanel as arguments,
 *          it creates and returns a specific tzsh_quickpanel handler corresponding to the given type.
 * @since_tizen 5.5
 * @remarks The returned handle should be released using tzsh_quickpanel_destroy().
 *          The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @param[in] tzsh The #tzsh_h instance
 * @param[in] win The native window
 * @param[in] type The type of #tzsh_quickpanel_h object
 * @return #tzsh_quickpanel_h object on success, @c NULL otherwise
 * @exception #TZSH_ERROR_NONE Successful
 * @exception #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre You have to create a #tzsh_h instance using tzsh_create() before calling this function.
 *      You can get the #tzsh_window instance using the toolkit API such as elm_win_window_id_get().
 * @see tzsh_create()
 * @see tzsh_quickpanel_destroy()
 * @par Example
 * @code
 *  #include <tzsh_quickpanel.h>
 *
 *  tzsh_h tzsh = NULL;
 *  tzsh_quickpanel_h qp = NULL;
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
 *      qp = tzsh_quickpanel_create_with_type(tzsh, tz_win,
 *                                            TZSH_QUICKPANEL_TYPE_SYSTEM_DEFAULT);
 *      ...
 *  }
 * @endcode
 */
tzsh_quickpanel_h tzsh_quickpanel_create_with_type(tzsh_h tzsh,
                                                   tzsh_window win,
                                                   tzsh_quickpanel_type_e type);


/**
 * @brief Destroys the given #tzsh_quickpanel_h object.
 * @since_tizen 3.0
 * @param[in] quickpanel The #tzsh_quickpanel_h object to be destroyed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tzsh_quickpanel_create_with_type()
 * @par Example
 * @code
 *  #include <tzsh_quickpanel.h>
 *
 *  tzsh_h tzsh = NULL;
 *  tzsh_quickpanel_h qp = NULL;
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
 *      qp = tzsh_quickpanel_create_with_type(tzsh, tz_win,
 *                                            TZSH_QUICKPANEL_TYPE_SYSTEM_DEFAULT);
 *      ...
 *  }
 *
 *  void destroy_window(void)
 *  {
 *      ...
 *      tzsh_quickpanel_destroy(qp);
 *      qp = NULL;
 *      ...
 *  }
 * @endcode
 */
int tzsh_quickpanel_destroy(tzsh_quickpanel_h quickpanel);

/**
 * @brief Gets the type of the quickpanel.
 * @details Retrieves the type of the Quickpanel Service window corresponding to the given handler of tzsh_quickpanel.
 * @since_tizen 5.5
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[out] type The type of the #tzsh_quickpanel_h object
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 */
int tzsh_quickpanel_type_get(tzsh_quickpanel_h quickpanel,
                             tzsh_quickpanel_type_e *type);

/**
 * @brief Shows the quickpanel service window if it is currently scrollable.
 * @since_tizen 3.0
 * @remarks If the quickpanel is not scrollable, nothing will happen.
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 */
int tzsh_quickpanel_show(tzsh_quickpanel_h quickpanel);

/**
 * @brief Hides the quickpanel service window.
 * @since_tizen 3.0
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 */
int tzsh_quickpanel_hide(tzsh_quickpanel_h quickpanel);

/**
 * @brief Gets the visible state of the quickpanel.
 * @details Retrieves the visible state of the Quickpanel Service window corresponding to the given handler of tzsh_quickpanel.
 *          This visible state indicates whether or not the Quickpanel Service window is displayed at the time of invocation.
 * @since_tizen 3.0
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[out] visible The visible state of the quickpanel service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 */
int tzsh_quickpanel_visible_get(tzsh_quickpanel_h quickpanel,
                                tzsh_quickpanel_state_visible_e *visible);

/**
 * @deprecated Deprecated since 5.5. Use the return value of tzsh_quickpanel_scrollable_state_set() instead.
 * @brief Sets the scrollable state of a quickpanel service window.
 * @details This function is useful when the application does not need to show the quickpanel service
 *          window. If the application set the @a scrollable to @c false, the quickpanel service
 *          window does not show on the display as long as the application window is shown.
 *          Default @a scrollable state of the window is true.
 * @since_tizen 3.0
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[in] scrollable The scrollable state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 * @see tzsh_quickpanel_scrollable_state_set()
 */
int tzsh_quickpanel_scrollable_set(tzsh_quickpanel_h quickpanel,
                                   bool scrollable) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 5.5. Use the return value of tzsh_quickpanel_scrollable_state_get() instead.
 * @brief Checks if the quickpanel service window is scrollable.
 * @since_tizen 3.0
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[out] scrollable The scrollable state of the quickpanel service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 * @see tzsh_quickpanel_scrollable_state_get()
 */
int tzsh_quickpanel_scrollable_get(tzsh_quickpanel_h quickpanel,
                                   tzsh_quickpanel_state_scrollable_e *scrollable) TIZEN_DEPRECATED_API;

/**
 * @brief Sets the window's scrollable state of the quickpanel service window.
 * @details This function is useful when the window does not need to show the quickpanel service window.
 *          Each window has it's own scrollable state of the quickpanel window.
 *          If the window set the @a state to #TZSH_QUICKPANEL_STATE_SCROLLABLE_UNSET,
 *          the quickpanel service window is not shown on the display as long as the window is shown.
 *          Default scrollable state of the window is #TZSH_QUICKPANEL_STATE_SCROLLABLE_SET.
 *          If the window set the @a state to #TZSH_QUICKPANEL_STATE_SCROLLABLE_RETAIN,
 *          it follows the scrollable state of the previously shown window. For instance, if the state of the previously shown window was
 *          #TZSH_QUICKPANEL_STATE_SCROLLABLE_UNSET, then the quickpanel service window is not shown on the display.
 *          If the state of the previously shown window was #TZSH_QUICKPANEL_STATE_SCROLLABLE_SET,
 *          then the quickpanel service window can be shown on the display.
 * @since_tizen 5.5
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[in] state The scrollable state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 */
int tzsh_quickpanel_scrollable_state_set(tzsh_quickpanel_h quickpanel,
                                         tzsh_quickpanel_state_scrollable_e state);

/**
 * @brief Gets the window's scrollable state of the quickpanel service window.
 * @details Retrieves the scrollable state of the Quickpanel Service window corresponding to the given tzsh_quickpanel handler.
 *          The scrollable_state determines whether the Quickpanel Service window is scrollable or not.
 *          And the scrollable_state value can be affect the visibility of the Quickpanel Service window.
 * @since_tizen 5.5
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[out] state The scrollable state of the quickpanel service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 */
int tzsh_quickpanel_scrollable_state_get(tzsh_quickpanel_h quickpanel,
                                         tzsh_quickpanel_state_scrollable_e *state);

/**
 * @brief Gets the orientation of the quickpanel.
 * @since_tizen 3.0
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[out] orientation The orientation of the quickpanel service window
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #TZSH_ERROR_NO_SERVICE Service does not exist
 */
int tzsh_quickpanel_orientation_get(tzsh_quickpanel_h quickpanel,
                                    tzsh_quickpanel_state_orientation_e *orientation);

/**
 * @brief Adds an event handler.
 * @details Register an event handler function for the given event type.
 *          The registered event handler function is called when the argument value changed of the given event type.
 * @since_tizen 3.0
 * @remarks The returned #tzsh_quickpanel_event_handler_h object should be released using
 *          tzsh_quickpanel_event_handler_del(). The @a type parameter is the integer of the event
 *          type that will trigger this callback to be called. The callback @a func is called when
 *          this event is processed and will be passed the event type, a pointer to the private
 *          event structure that is specific to that event type, and a data pointer that is
 *          provided in this call as the @a user_data parameter.
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[in] type The event type (#TZSH_QUICKPANEL_EVENT_VISIBLE, #TZSH_QUICKPANEL_EVENT_ORIENTATION)
 * @param[in] func The callback function to be called
 * @param[in] user_data The data passed to the callback function
 * @return #tzsh_quickpanel_event_handler_h instance on success, @c NULL otherwise
 * @pre tzsh_quickpanel_create_with_type() should be called, to generate event type IDs
 * @see TZSH_QUICKPANEL_EVENT_VISIBLE
 * @see TZSH_QUICKPANEL_EVENT_ORIENTATION
 * @see tzsh_quickpanel_event_cb()
 * @see tzsh_quickpanel_event_handler_del()
 */
tzsh_quickpanel_event_handler_h tzsh_quickpanel_event_handler_add(tzsh_quickpanel_h quickpanel,
                                                                  int type,
                                                                  tzsh_quickpanel_event_cb func,
                                                                  void *user_data);

/**
 * @brief Removes the registered event handler.
 * @since_tizen 3.0
 * @param[in] quickpanel The #tzsh_quickpanel_h object
 * @param[in] event_handler The #tzsh_quickpanel_event_handler_h object
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @see tzsh_quickpanel_event_handler_add()
 */
int tzsh_quickpanel_event_handler_del(tzsh_quickpanel_h quickpanel,
                                      tzsh_quickpanel_event_handler_h event_handler);

/**
 * @brief Gets the visible state from given event info.
 * @since_tizen 3.0
 * @remarks This will be used in the event handler for the #TZSH_QUICKPANEL_EVENT_VISIBLE event.
 * @param[in] event_info The #tzsh_quickpanel_event_info_h object
 * @param[out] visible The visible state of quickpanel
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @see TZSH_QUICKPANEL_EVENT_VISIBLE
 */
int tzsh_quickpanel_event_visible_get(tzsh_quickpanel_event_info_h event_info,
                                      tzsh_quickpanel_state_visible_e *visible);

/**
 * @brief Gets the orientation state from given event info.
 * @since_tizen 3.0
 * @remarks This will be used in the event handler for the #TZSH_QUICKPANEL_EVENT_ORIENTATION event.
 * @param[in] event_info The #tzsh_quickpanel_event_info_h object
 * @param[out] orientation The orientation state of quickpanel
 * @return @c 0 on success, otherwise a negative error value
 * @retval #TZSH_ERROR_NONE Successful
 * @retval #TZSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @see TZSH_QUICKPANEL_EVENT_ORIENTATION
 */
int tzsh_quickpanel_event_orientation_get(tzsh_quickpanel_event_info_h event_info,
                                          tzsh_quickpanel_state_orientation_e *orientation);

#ifdef  __cplusplus
}
#endif

/**
 * @}
 */

#endif
