#ifndef _ECORE_INPUT_DEVICE_EO_H_
#define _ECORE_INPUT_DEVICE_EO_H_

#ifndef _ECORE_INPUT_DEVICE_EO_CLASS_TYPE
#define _ECORE_INPUT_DEVICE_EO_CLASS_TYPE

typedef Eo Ecore_Input_Device;

#endif

#ifndef _ECORE_INPUT_DEVICE_EO_TYPES
#define _ECORE_INPUT_DEVICE_EO_TYPES

/** General type of input device.
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device_Type
 */
typedef enum
{
  ECORE_INPUT_DEVICE_TYPE_NONE = 0, /**< Not a device.
                                     *
                                     * @since 1.23 */
  ECORE_INPUT_DEVICE_TYPE_SEAT, /**< The user/seat (the user themselves).
                                 *
                                 * @since 1.23 */
  ECORE_INPUT_DEVICE_TYPE_KEYBOARD, /**< A regular keyboard, numberpad or
                                     * attached buttons.
                                     *
                                     * @since 1.23 */
  ECORE_INPUT_DEVICE_TYPE_MOUSE, /**< A mouse, trackball or touchpad relative
                                  * motion device.
                                  *
                                  * @since 1.23 */
  ECORE_INPUT_DEVICE_TYPE_TOUCH, /**< A touchscreen with fingers or stylus.
                                  *
                                  * @since 1.23 */
  ECORE_INPUT_DEVICE_TYPE_PEN, /**< A special pen device.
                                *
                                * @since 1.23 */
  ECORE_INPUT_DEVICE_TYPE_WAND, /**< A laser pointer, wii-style or "Minority
                                 * Report" pointing device.
                                 *
                                 * @since 1.23 */
  ECORE_INPUT_DEVICE_TYPE_GAMEPAD /**< A gamepad controller or joystick.
                                   *
                                   * @since 1.23 */
} Ecore_Input_Device_Type;

/** General type of input device.
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device_Subtype
 */
typedef enum
{
  ECORE_INPUT_DEVICE_SUBTYPE_NONE = 0, /**< Not a device.
                                        *
                                        * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_FINGER, /**< The normal flat of your finger.
                                      *
                                      * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_FINGERNAIL, /**< A fingernai.
                                          *
                                          * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_KNUCKLE, /**< A Knuckle.
                                       *
                                       * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_PALM, /**< The palm of a users hand.
                                    *
                                    * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_HAND_SIZE, /**< The side of your hand.
                                         *
                                         * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_HAND_FLAT, /**< The flat of your hand.
                                         *
                                         * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_PEN_TIP, /**< The tip of a pen.
                                       *
                                       * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_TRACKPAD, /**< A trackpad style mouse.
                                        *
                                        * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_TRACKPOINT, /**< A trackpoint style mouse.
                                          *
                                          * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_TRACKBALL, /**< A trackball style mouse.
                                         *
                                         * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_REMOCON, /**< A remote controller.
                                       *
                                       * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_VIRTUAL_KEYBOARD, /**< A virtual keyboard.
                                                *
                                                * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_VIRTUAL_REMOCON, /**< A virtual remote controller.
                                               *
                                               * @since 1.23 */
  ECORE_INPUT_DEVICE_SUBTYPE_VIRTUAL_MOUSE /**< A virtual mouse.
                                            *
                                            * @since 1.23 */
} Ecore_Input_Device_Subtype;


#endif
/** Represents a pointing device such as a touch finger, pen or mouse.
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
#define ECORE_INPUT_DEVICE_CLASS ecore_input_device_class_get()

EWAPI const Efl_Class *ecore_input_device_class_get(void) EINA_CONST;

/**
 * @brief Device type property
 *
 * @param[in] obj The object.
 * @param[in] klass Input device class
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI void ecore_input_device_type_set(Eo *obj, Ecore_Input_Device_Type klass);

/**
 * @brief Device type property
 *
 * @param[in] obj The object.
 *
 * @return Input device class
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI Ecore_Input_Device_Type ecore_input_device_type_get(const Eo *obj);

/**
 * @brief Device subclass property
 *
 * @param[in] obj The object.
 * @param[in] klass Input device subclass
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI void ecore_input_device_subclass_set(Eo *obj, Ecore_Input_Device_Subtype klass);

/**
 * @brief Device subclass property
 *
 * @param[in] obj The object.
 *
 * @return Input device subclass
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI Ecore_Input_Device_Subtype ecore_input_device_subclass_get(const Eo *obj);

/**
 * @brief Device source property
 *
 * @param[in] obj The object.
 * @param[in] src Input device
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI void ecore_input_device_source_set(Eo *obj, Ecore_Input_Device *src);

/**
 * @brief Device source property
 *
 * @param[in] obj The object.
 *
 * @return Input device
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI Ecore_Input_Device *ecore_input_device_source_get(const Eo *obj);

/**
 * @brief Get the @ref Efl_Input_Device that represents a seat.
 *
 * This method will find the seat the device belongs to.
 *
 * For this, it walk through device's parents looking for a device with
 * @ref EFL_INPUT_DEVICE_TYPE_SEAT. It may be the device itself.
 *
 * In case no seat is found, @c null is returned.
 *
 * @param[in] obj The object.
 *
 * @return The seat this device belongs to.
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI Ecore_Input_Device *ecore_input_device_seat_get(const Eo *obj);

/**
 * @brief Seat id number
 *
 * @param[in] obj The object.
 * @param[in] id The id of the seat
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI void ecore_input_device_seat_id_set(Eo *obj, unsigned int id);

/**
 * @brief Seat id number
 *
 * @param[in] obj The object.
 *
 * @return The id of the seat
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI unsigned int ecore_input_device_seat_id_get(const Eo *obj);

/**
 * @brief Lists the children attached to this device.
 *
 * This is only meaningful with seat devices, as they are groups of real input
 * devices.
 *
 * @param[in] obj The object.
 *
 * @return List of device children
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI Eina_Iterator *ecore_input_device_children_iterate(Eo *obj) EFL_TRANSFER_OWNERSHIP EINA_WARN_UNUSED_RESULT;

/**
 * @brief The number of pointer devices in this seat.
 *
 * Pointer devices are the ones whose @ref ecore_input_device_type_get is
 * @c mouse, @c pen, @c touch or @c wand. In case this device is not of the
 * type @c seat, -1 is returned.
 *
 * @param[in] obj The object.
 *
 * @return The number of pointer devices.
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI int ecore_input_device_pointer_device_count_get(const Eo *obj);

/**
 * @brief @c true if @ref ecore_input_device_type_get is @c mouse, @c pen,
 * @c touch or @c wand.
 *
 * @param[in] obj The object.
 *
 * @return @c true if the device has pointing capabilities.
 *
 * @since 1.23
 *
 * @ingroup Ecore_Input_Device
 */
EOAPI Eina_Bool ecore_input_device_is_pointer_type_get(const Eo *obj);

#endif
