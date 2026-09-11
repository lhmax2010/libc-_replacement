#ifndef __TIZEN_SYSTEM_DISPLAY_ENUM_H__
#define __TIZEN_SYSTEM_DISPLAY_ENUM_H__

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @addtogroup CAPI_SYSTEM_DEVICE_DISPLAY_MODULE
 * @{
 */
/**
 * @brief Enumeration for the available display states.
 * @since_tizen 2.3
 * @remarks #DISPLAY_STATE_SCREEN_DIM may be ignored if the DIM state is disabled on the platform.
 */

typedef enum
{
    DISPLAY_STATE_NORMAL, /**< Normal state */
    DISPLAY_STATE_SCREEN_DIM, /**< Screen dim state */
    DISPLAY_STATE_SCREEN_OFF, /**< Screen off state */
} display_state_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_SYSTEM_DISPLAY_ENUM_H__
