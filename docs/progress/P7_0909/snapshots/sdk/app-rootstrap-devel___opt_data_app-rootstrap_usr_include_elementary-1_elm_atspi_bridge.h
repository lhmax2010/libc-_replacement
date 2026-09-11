#ifndef EFL_NOLEGACY_API_SUPPORT
#include "elm_atspi_bridge_eo.legacy.h"
#endif

/**
 * @defgroup Elm_Atspi_Bridge Atspi Bridge
 * @ingroup elm_accessibility_group
 */
//TIZEN_ONLY(20160527) - Add direct reading feature
/**
 * This callback type is used to provide reading status information.
 * @param data The data specified as the last param when setting this callback
 * @param say_signal The signal describes reading status information
 * @see elm_atspi_bridge_utils_say
 * @ingroup Elm_Atspi_Bridge
 * @since_tizen 4.0
 */
typedef void (*Elm_Atspi_Say_Signal_Cb)(void *data, const char *say_signal);
/**
 * @deprecated Deprecated since 10.0.
 * @brief Reads given text by screen reader
 *
 * @param text The text to read
 * @param discardable If TRUE, reading can be discarded by subsequent reading requests,
 * if FALSE the reading must finish before next reading request can be started
 * @param func the callback function that is called on reading signals emitted
 * during processing of this reading request. Elm_Atspi_Say_Signal_Cb callback
 * type specifies two parameters (void *data, const char *signal), where data
 * is a user data and signal can be one of the following signals:
 * “ReadingCancelled”
 * “ReadingStopped”
 * “ReadingSkipped”
 * “ReadingPaused”
 * “ReadingResumed”
 * @param data The user data to be passed to the callback function
 *
 * @ingroup Elm_Atspi_Bridge
 *
 * @since_tizen 4.0
 */
EAPI void elm_atspi_bridge_utils_say(const char* text,
                                     Eina_Bool discardable,
                                     const Elm_Atspi_Say_Signal_Cb func,
                                     const void *data);

/**
 * @brief Pause / resume screen-reader reading
 *
 * @param pause True, when reading should be paused, otherwise reading will be resumed.
 */
EAPI void elm_atspi_bridge_utils_pause_resume(Eina_Bool pause);

/**
 * @brief Cancels anything, screen-reader is reading / has quened to read
 *
 * @param non_discardable_also true, when you want to cancel all readings
 *     false when you want to limit cancellings only to discardable readings
 */
EAPI void elm_atspi_bridge_utils_stop_reading(Eina_Bool non_discardable_also);

//
//TIZEN_ONLY(20161027) - Export elm_atspi_bridge_utils_is_screen_reader_enabled
/**
 * @internal
 * Determines if screen reader is enabled
 * @ingroup Elm_Atspi_Bridge
 */
EAPI Eina_Bool elm_atspi_bridge_utils_is_screen_reader_enabled(void);
//

//TIZEN_ONLY(20200515) - Export elm_atspi_bridge_highlighted_object_read
/**
 * @brief Reads highlighted object text
 *
 * Reads the highlighted object text based on reading
 * information set on object, this API can be called
 * when information of a highlighted object needs
 * to be read again
 *
 * @ingroup Elm_Atspi_Bridge
 * @since_tizen 5.5
 */
EAPI void elm_atspi_bridge_highlighted_object_read(void);
//

//TIZEN_ONLY(20210416) - Suppress screen reader
/**
 * @brief Sets screen reader suppress
 *
 * Because there is legacy code for screen-reader, an application could want
 * to keep previous behavior of the screen-reader even though the ATSPI is
 * enabled. If you want to make screen-reader AT client do not work for your
 * application, then set @c suppress to @c EINA_TRUE.
 * Default suppress is @c EINA_FALSE.
 *
 * @internal
 * @ingroup Elm_Atspi_Bridge
 */
EAPI Eina_Bool elm_atspi_bridge_utils_suppress_screen_reader_set(Eina_Bool suppress);

/**
 * @brief Gets screen reader suppress
 * @see elm_atspi_bridge_utils_suppress_screen_reader_set
 *
 * @internal
 * @ingroup Elm_Atspi_Bridge
 */
EAPI Eina_Bool elm_atspi_bridge_utils_suppress_screen_reader_get(void);
//
