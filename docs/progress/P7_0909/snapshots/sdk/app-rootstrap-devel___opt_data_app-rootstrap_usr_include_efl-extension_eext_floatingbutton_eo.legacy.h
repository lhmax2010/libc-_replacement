#ifndef _EEXT_FLOATINGBUTTON_EO_LEGACY_H_
#define _EEXT_FLOATINGBUTTON_EO_LEGACY_H_

#ifndef _EEXT_FLOATINGBUTTON_EO_CLASS_TYPE
#define _EEXT_FLOATINGBUTTON_EO_CLASS_TYPE

typedef Eo Eext_Floatingbutton;

#endif

#ifndef _EEXT_FLOATINGBUTTON_EO_TYPES
#define _EEXT_FLOATINGBUTTON_EO_TYPES

/** Floating button position
 *
 * @ingroup Eext_Floatingbutton
 */
typedef enum
{
  EEXT_FLOATINGBUTTON_LEFT_OUT = 0, /**< hides in the left, but small handler
                                     * will show only */
  EEXT_FLOATINGBUTTON_LEFT, /**< shows all of buttons, but lies on the left */
  EEXT_FLOATINGBUTTON_CENTER, /**< shows all of buttons, but lies on the center
                               */
  EEXT_FLOATINGBUTTON_RIGHT, /**< shows all of buttons, but lies on the right */
  EEXT_FLOATINGBUTTON_RIGHT_OUT, /**< hides in the right, but small handler will
                                  * show only */
  EEXT_FLOATINGBUTTON_LAST /**< indicates the last, do not use this */
} Eext_Floatingbutton_Pos;

/** Floating button mode
 *
 * @ingroup Eext_Floatingbutton
 */
typedef enum
{
  EEXT_FLOATINGBUTTON_MODE_DEFAULT = 0, /**< allows all positions */
  EEXT_FLOATINGBUTTON_MODE_BOTH_SIDES, /**< allows LEFT and RIGHT positions only
                                        */
  EEXT_FLOATINGBUTTON_MODE_LAST /**< indicates the last, do not use this */
} Eext_Floatingbutton_Mode;


#endif

#endif
