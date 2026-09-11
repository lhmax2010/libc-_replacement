/*
 * Copyright (c) 2016-2018 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_SYSTEM_PERIPHERAL_IO_H__
#define __TIZEN_SYSTEM_PERIPHERAL_IO_H__

#include <stdint.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_MODULE
 * @{
 */

/**
 * @file peripheral_io.h
 * @brief This file contains the public peripheral-io interface.
 * @details Useful for controlling peripheral devices like sensors, servomotors and whatnot.
 */

/**
 * @brief Enumeration for error codes returned by the peripheral-io API.
 * @details Most peripheral-io API functions can return one of these codes to specify an error.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_ERROR_NONE                = TIZEN_ERROR_NONE,                /**< Successful */
	PERIPHERAL_ERROR_IO_ERROR            = TIZEN_ERROR_IO_ERROR,            /**< I/O error */
	PERIPHERAL_ERROR_NO_DEVICE           = TIZEN_ERROR_NO_SUCH_DEVICE,      /**< No such device */
	PERIPHERAL_ERROR_TRY_AGAIN           = TIZEN_ERROR_TRY_AGAIN,           /**< Try again */
	PERIPHERAL_ERROR_OUT_OF_MEMORY       = TIZEN_ERROR_OUT_OF_MEMORY,       /**< Out of memory */
	PERIPHERAL_ERROR_PERMISSION_DENIED   = TIZEN_ERROR_PERMISSION_DENIED,   /**< Permission denied */
	PERIPHERAL_ERROR_RESOURCE_BUSY       = TIZEN_ERROR_RESOURCE_BUSY,       /**< Device or resource busy */
	PERIPHERAL_ERROR_INVALID_PARAMETER   = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	PERIPHERAL_ERROR_NOT_SUPPORTED       = TIZEN_ERROR_NOT_SUPPORTED,       /**< Not supported */
	PERIPHERAL_ERROR_UNKNOWN             = TIZEN_ERROR_UNKNOWN,             /**< Unknown error */
} peripheral_error_e;

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_GPIO_MODULE
 * @{
 */

/**
 * @brief Enumeration representing GPIO device direction options.
 * @details Some interfaces require you to specify whether data goes in or out.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_GPIO_DIRECTION_IN = 0,              /**< Input Mode */
	PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_HIGH,  /**< Output mode with high value */
	PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW,   /**< Output mode with low value */
} peripheral_gpio_direction_e;

/**
 * @brief Enumeration for edge types for the GPIO interrupt.
 * @details Some interfaces require you to specify what kind of signal edge produces an interrupt.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_GPIO_EDGE_NONE = 0,  /**< No interrupt on GPIO */
	PERIPHERAL_GPIO_EDGE_RISING,    /**< Interrupt on rising only */
	PERIPHERAL_GPIO_EDGE_FALLING,   /**< Interrupt on falling only */
	PERIPHERAL_GPIO_EDGE_BOTH,      /**< Interrupt on rising & falling */
} peripheral_gpio_edge_e;

/**
 * @brief An opaque handle to an object representing a GPIO pin.
 * @details A handle to a single GPIO pin, for use with most GPIO interfaces
 * @since_tizen 4.0
 */
typedef struct _peripheral_gpio_s *peripheral_gpio_h;

/**
 * @platform
 * @brief Opens a GPIO pin and returns a handle representing it
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a gpio should be released with peripheral_gpio_close()
 *
 * @param[in] gpio_pin The GPIO pin number
 * @param[out] gpio The GPIO handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_gpio_close()
 *
 * @code
#include <peripheral_io.h>

#define PIN_NUMBER 7

int main(void)
{
	int ret;
	peripheral_gpio_h gpio;

	ret = peripheral_gpio_open(PIN_NUMBER, &gpio);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the pin here

	ret = peripheral_gpio_close(gpio);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_open(int gpio_pin, peripheral_gpio_h *gpio);

/**
 * @platform
 * @brief Closes a GPIO pin and frees the resources associated with the handle
 * @details This function should be called at the end of the object's lifetime to avoid a memory leak.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_gpio_open()
 *
 * @code
#include <peripheral_io.h>

#define PIN_NUMBER 7

int main(void)
{
	int ret;
	peripheral_gpio_h gpio;

	ret = peripheral_gpio_open(PIN_NUMBER, &gpio);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the pin here

	ret = peripheral_gpio_close(gpio);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_close(peripheral_gpio_h gpio);

/**
 * @platform
 * @brief Sets the input/output direction of given GPIO pin.
 * @details Sets whether data goes in or out on given GPIO pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks To set the direction to #PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_HIGH or #PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW, the edge mode must be set to #PERIPHERAL_GPIO_EDGE_NONE.
 *
 * @param[in] gpio The GPIO handle
 * @param[in] direction The direction of the GPIO pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_direction_e
 * @see peripheral_gpio_set_edge_mode()
 *
 * @code
#include <peripheral_io.h>

void configure_pin(peripheral_gpio_h gpio, bool input_needed, bool output_needed)
{
	int ret;

	if (input_needed)
		ret = peripheral_gpio_set_direction(gpio, PERIPHERAL_GPIO_DIRECTION_IN);
	else if (output_needed)
		ret = peripheral_gpio_set_direction(gpio, PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
	else
		handle_this_case();

	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_set_direction(peripheral_gpio_h gpio, peripheral_gpio_direction_e direction);

/**
 * @platform
 * @brief Sets the input edge mode of given GPIO pin device.
 * @details Changes which kind of edges will trigger the GPIO interrupts.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks To set the edge mode to #PERIPHERAL_GPIO_EDGE_RISING, #PERIPHERAL_GPIO_EDGE_FALLING, #PERIPHERAL_GPIO_EDGE_BOTH, the data direction must be set to the #PERIPHERAL_GPIO_DIRECTION_IN.
 *
 * @param[in] gpio The GPIO handle
 * @param[in] edge The edge mode of the GPIO pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_edge_e
 * @see peripheral_gpio_set_direction()
 *
 * @code
#include <peripheral_io.h>

void configure_pin(peripheral_gpio_h gpio, bool on_rising, bool on_falling)
{
	int ret = peripheral_gpio_set_direction(gpio, PERIPHERAL_GPIO_DIRECTION_IN);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	if (on_rising) {
		if (on_falling)
			ret = peripheral_gpio_set_edge_mode(gpio, PERIPHERAL_GPIO_EDGE_BOTH);
		else
			ret = peripheral_gpio_set_edge_mode(gpio, PERIPHERAL_GPIO_EDGE_RISING);
	} else {
		if (on_falling)
			ret = peripheral_gpio_set_edge_mode(gpio, PERIPHERAL_GPIO_EDGE_FALLING);
		else
			ret = peripheral_gpio_set_edge_mode(gpio, PERIPHERAL_GPIO_EDGE_NONE);
	}
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_set_edge_mode(peripheral_gpio_h gpio, peripheral_gpio_edge_e edge);

/**
 * @platform
 * @brief The GPIO interrupted callback called when the GPIO interrupt is triggered.
 * @details The following errors can be received: \n
 * #PERIPHERAL_ERROR_NONE Successful \n
 * #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported \n
 * #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied \n
 * #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter \n
 * #PERIPHERAL_ERROR_IO_ERROR I/O operation failed \n
 * #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed \n
 * #PERIPHERAL_ERROR_TRY_AGAIN Try again \n
 * #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed \n
 * #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use \n
 * #PERIPHERAL_ERROR_UNKNOWN Unknown internal error \n
 * @since_tizen 4.0
 *
 * @param[in] gpio The GPIO handle
 * @param[in] error The GPIO error
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see peripheral_gpio_set_interrupted_cb()
 */
typedef void(*peripheral_gpio_interrupted_cb)(peripheral_gpio_h gpio, peripheral_error_e error, void *user_data);

/**
 * @platform
 * @brief Sets the GPIO interrupted callback to the provided function.
 * @details The callback will be called when the GPIO interrupt is triggered.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks The interrupted callback is unset when peripheral_gpio_unset_interrupted_cb() is called, when another callback is registered for the same #peripheral_gpio_h handle (it is not possible to have multiple callbacks at the same time) or when callback receives an error value other than #PERIPHERAL_ERROR_NONE. Additionally, the callback cannot modify the provided gpio handle in anyway, it should consider it read-only.
 *
 * @param[in] gpio The GPIO handle
 * @param[in] callback The GPIO interrupted callback function to set
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 *
 * @post peripheral_gpio_unset_interrupted_cb()
 * @see peripheral_gpio_set_edge_mode()
 *
 * @code
#include <peripheral_io.h>

void callback(peripheral_gpio_h gpio, peripheral_error_e error, void *user_data)
{
	struct our_data_type *callback_data = user_data;
	int ret;
	uint32_t pin_value;

	if (error != PERIPHERAL_ERROR_NONE)
		handle_error(error);

	ret = peripheral_gpio_read(gpio, &pin_value);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	process_value(callback_data, pin_value);
}

void set_callback(peripheral_gpio_h gpio)
{
	struct our_data_type *callback_data = create_callback_data();

	int ret = peripheral_gpio_set_interrupted_cb(gpio, callback, callback_data);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_set_interrupted_cb(peripheral_gpio_h gpio, peripheral_gpio_interrupted_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the previously set GPIO interrupted callback.
 * @details The callback will no longer be invoked when the GPIO interrupt is triggered.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks If there is no callback registered, the function will be successful anyway, so it's ok to call it unconditionally regardless of peripheral_gpio_set_interrupted_cb() success. After this function exits, the callback will not be called anymore.
 *
 * @param[in] gpio The GPIO handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre peripheral_gpio_set_interrupted_cb()
 *
 * @code
#include <peripheral_io.h>

void unset_callback(peripheral_gpio_h gpio, struct our_data_type *callback_data)
{
	int ret = peripheral_gpio_unset_interrupted_cb(gpio);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Make sure the callback is not running in parallel!
	// The callback will run on the GLib main context,
	// if you are also running this on the main context too,
	// which is probably the case if your app is single-threaded,
	// it is ok to immediately deallocate.
	destroy_callback_data(callback_data);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_unset_interrupted_cb(peripheral_gpio_h gpio);

/**
 * @platform
 * @brief Gets the current value being exposed on the GPIO pin.
 * @details Reads data from the GPIO pin to the specified address.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 * @param[out] value The value to get
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_write()
 *
 * @code
#include <peripheral_io.h>
#include <glib.h>
#include <stdio.h>

gboolean loop_iteration(gpointer user_data)
{
	peripheral_gpio_h gpio = user_data;
	int ret;
	uint32_t data;

	ret = peripheral_gpio_read(gpio, &data);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	printf("Current GPIO pin value: %d\n", (int)data);

	return G_SOURCE_CONTINUE;
}

guint read_in_a_loop(peripheral_gpio_h gpio)
{
	int ret = peripheral_gpio_set_direction(gpio, PERIPHERAL_GPIO_DIRECTION_IN);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	return g_timeout_add_seconds(1, loop_iteration, gpio);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_read(peripheral_gpio_h gpio, uint32_t *value);

/**
 * @platform
 * @brief Sets the value to be exposed on the given GPIO pin.
 * @details Writes data into the GPIO pin from the specified address.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks To write binary data, the direction must be set to #PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_HIGH or #PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW.
 *
 * @param[in] gpio The GPIO handle
 * @param[in] value The value to set (must be 0 or 1)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_read()
 * @see peripheral_gpio_set_direction()
 *
 * @code
#include <peripheral_io.h>
#include <glib.h>
#include <stdio.h>

gboolean loop_iteration(gpointer user_data)
{
	peripheral_gpio_h gpio = user_data;
	int ret;

	ret = peripheral_gpio_write(gpio, g_random_int_range(0, 2));
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	printf("Current GPIO pin value: %d\n", (int)data);

	return G_SOURCE_CONTINUE;
}

guint read_in_a_loop(peripheral_gpio_h gpio)
{
	int ret = peripheral_gpio_set_direction(gpio, PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	return g_timeout_add_seconds(1, loop_iteration, gpio);
}
 * @endcode
 */
EXPORT_API int peripheral_gpio_write(peripheral_gpio_h gpio, uint32_t value);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_I2C_MODULE
 * @{
 */

/**
 * @brief An opaque handle representing the I2C slave device.
 * @details A handle to a single I2C slave device, for use with most I2C slave interfaces
 * @since_tizen 4.0
 */
typedef struct _peripheral_i2c_s *peripheral_i2c_h;

/**
 * @platform
 * @brief Opens an I2C slave device and returns a handle representing it
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a i2c should be released with peripheral_i2c_close()
 *
 * @param[in] bus The I2C bus number that the slave device is connected
 * @param[in] address The address of the slave device
 * @param[out] i2c The I2C handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_i2c_close()
 *
 * @code
#include <peripheral_io.h>

#define I2C_BUS 1
#define I2C_ADDRESS 0x07

int main(void)
{
	int ret;
	peripheral_i2c_h i2c;

	ret = peripheral_i2c_open(I2C_BUS, I2C_ADDRESS, &i2c);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_i2c_close(i2c);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_open(int bus, int address, peripheral_i2c_h *i2c);

/**
 * @brief Enumeration for open flags (bitmask) for an i2c device.
 * @since_tizen 6.5
 *
 * @details Enum values are supposed to be used as bitmask, where only one
 * value can be specified for following flag groups:
 * - locking mode - either #PERIPHERAL_OPEN_FLAGS_PRIVATE or #PERIPHERAL_OPEN_FLAGS_SHARED can be used
 *
 * The #PERIPHERAL_OPEN_FLAGS_NONBLOCK can be used with all other available flags.
 */
typedef enum {
	PERIPHERAL_OPEN_FLAGS_PRIVATE = 0,          /**< Exclusive access to device */
	PERIPHERAL_OPEN_FLAGS_SHARED  = 1,          /**< Shared access to device */
	PERIPHERAL_OPEN_FLAGS_NONBLOCK = 2,         /**< Nonblocking read/write flag,
							available for only uart (Since 8.0) */
} peripheral_open_flags_e;

/**
 * @platform
 * @brief Opens an I2C slave device and returns a representing handle
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a i2c should be released with peripheral_i2c_close()
 *
 * @param[in] bus The I2C bus number that the slave device is connected
 * @param[in] address The address of the slave device
 * @param[in] flags The flags to open call
 * @param[out] i2c The I2C handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_i2c_close()
 *
 * @code
#include <peripheral_io.h>

#define I2C_BUS 1
#define I2C_ADDRESS 0x07

int main(void)
{
	int ret;
	peripheral_i2c_h i2c;

	ret = peripheral_i2c_open_flags(I2C_BUS, I2C_ADDRESS,
		PERIPHERAL_OPEN_FLAGS_SHARED | PERIPHERAL_OPEN_FLAGS_NONBLOCK,
		&i2c);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_i2c_close(i2c);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_open_flags(int bus, int address, peripheral_open_flags_e flags, peripheral_i2c_h *i2c);

/**
 * @platform
 * @brief Closes an I2C slave device and frees resources associated with the handle.
 * @details This function should be called at the end of the object's lifetime to avoid a memory leak.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_i2c_open()
 *
 * @code
#include <peripheral_io.h>

#define I2C_BUS 1
#define I2C_ADDRESS 0x07

int main(void)
{
	int ret;
	peripheral_i2c_h i2c;

	ret = peripheral_i2c_open(I2C_BUS, I2C_ADDRESS, &i2c);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_i2c_close(i2c);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_close(peripheral_i2c_h i2c);

/**
 * @platform
 * @brief Reads the bytes data from the I2C slave device into a buffer
 * @details Fills the buffer with data read from the provided device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[out] data The data buffer to read
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_write()
 *
 * @code
#include <peripheral_io.h>
#include <stdio.h>

#define READ_SIZE 4

void read_bytes(peripheral_i2c_h i2c)
{
	int ret;
	uint8_t data[READ_SIZE];

	ret = peripheral_i2c_read(i2c, data, READ_SIZE);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	for (int i = 0; i < BUFFER_SIZE; ++i)
		printf("%02x", data[i]);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_read(peripheral_i2c_h i2c, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Writes the bytes data from a buffer to the I2C slave device.
 * @details Reads the data from the buffer and sends it into the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] data The data buffer to write
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_read()
 *
 * @code
#include <peripheral_io.h>

static uint8_t data_to_write[] = { 0x1, 0x2, 0x3, 0x4 };

void write_bytes(peripheral_i2c_h i2c)
{
	int ret;

	ret = peripheral_i2c_write(i2c, data_to_write,
		sizeof(data_to_write) / sizeof(data_to_write[0]));
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_write(peripheral_i2c_h i2c, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Reads byte data from the register of the I2C slave device.
 * @details Will only read a single byte from the provided register.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to read
 * @param[out] data The single byte data to read
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_write_register_byte()
 *
 * @code
#include <peripheral_io.h>
#include <stdio.h>

#define REGISTER 0x7

void read_byte(peripheral_i2c_h i2c)
{
	int ret;
	uint8_t data;

	ret = peripheral_i2c_read_register_byte(i2c, REGISTER, &data);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	printf("%02x", data);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_read_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t *data);

/**
 * @platform
 * @brief Writes byte data to the register of the I2C slave device.
 * @details Will only write a single byte to the provided register.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to write
 * @param[in] data The single byte data to write
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_read_register_byte()
 *
 * @code
#include <peripheral_io.h>

#define REGISTER 0x7
#define DATA 0x11

void write_byte(peripheral_i2c_h i2c)
{
	int ret;

	ret = peripheral_i2c_write_register_byte(i2c, REGISTER, DATA);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_write_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t data);

/**
 * @platform
 * @brief Reads word data from the register of the I2C slave device.
 * @details Will read two bytes (a word) from the provided register.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to read
 * @param[out] data The word(2 bytes) data to read
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_write_register_word()
 *
 * @code
#include <peripheral_io.h>
#include <stdio.h>

#define REGISTER 0x7

void read_word(peripheral_i2c_h i2c)
{
	int ret;
	uint16_t data;

	ret = peripheral_i2c_read_register_word(i2c, REGISTER, &data);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	printf("%04x", data);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_read_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t *data);

/**
 * @platform
 * @brief Writes word data to the register of the I2C slave device.
 * @details Will write two bytes (a word) to the provided register.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to write
 * @param[in] data The word(2 bytes) data to write
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_read_register_word()
 *
 * @code
#include <peripheral_io.h>

#define REGISTER 0x7
#define DATA 0x1234

void write_word(peripheral_i2c_h i2c)
{
	int ret;

	ret = peripheral_i2c_write_register_word(i2c, REGISTER, DATA);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_i2c_write_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t data);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_PWM_MODULE
 * @{
 */

/**
 * @brief An opaque handle representing a PWM peripherals pin.
 * @details A handle to a single PWM peripherals pin, for use with most PWM interfaces
 * @since_tizen 4.0
 */
typedef struct _peripheral_pwm_s *peripheral_pwm_h;

/**
 * @brief Enumeration for representing PWN active signal Polarity.
 * @details Controls whether an active PWM signal has high or low/inversed polarity.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_PWM_POLARITY_ACTIVE_HIGH = 0, /**< PWM signal start in the active high state (Normal) */
	PERIPHERAL_PWM_POLARITY_ACTIVE_LOW,      /**< PWM signal start in the active low state (Inversed) */
} peripheral_pwm_polarity_e;

/**
 * @platform
 * @brief Opens the PWM pin and returns a handle representing it
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a pwm should be released with peripheral_pwm_close()
 *
 * @param[in] chip The PWM chip number
 * @param[in] pin The PWM pin(channel) number to control
 * @param[out] pwm The PWM handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_pwm_close()
 *
 * @code
#include <peripheral_io.h>

#define PWM_CHIP 0
#define PWM_PIN 1

int main(void)
{
	int ret;
	peripheral_pwm_h pwm;

	ret = peripheral_pwm_open(PWM_CHIP, PWM_PIN, &pwm);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_pwm_close(pwm);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_pwm_open(int chip, int pin, peripheral_pwm_h *pwm);

/**
 * @platform
 * @brief Closes a PWM pin and frees resources associated with the handle.
 * @details This function should be called at the end of the object's lifetime to avoid a memory leak.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_pwm_open()
 *
 * @code
#include <peripheral_io.h>

#define PWM_CHIP 0
#define PWM_PIN 1

int main(void)
{
	int ret;
	peripheral_pwm_h pwm;

	ret = peripheral_pwm_open(PWM_CHIP, PWM_PIN, &pwm);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_pwm_close(pwm);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_pwm_close(peripheral_pwm_h pwm);

/**
 * @platform
 * @brief Sets the period length in nanoseconds for the given PWM pin.
 * @details Increasing this decreases the average signal value, since the pulse width is kept the same.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] period_ns The total period of the PWM pin (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>

void manipulate_period(peripheral_pwm_h pwm)
{
	int ret;

	ret = peripheral_pwm_set_duty_cycle(pwm, 1 * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
	ret = peripheral_pwm_set_period(pwm, 10 * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Now the PWM pin is high for 1 ms of every 10 ms, i.e. 10% of the time.

	1et = peripheral_pwm_set_period(pwm, 20 * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Now the PWM pin is high for 1 ms of every 20 ms, i.e. 5% of the time.
}
 * @endcode
 */
EXPORT_API int peripheral_pwm_set_period(peripheral_pwm_h pwm, uint32_t period_ns);

/**
 * @platform
 * @brief Sets the length of a duty cycle (i.e. the pulse width or the active time) in nanoseconds of the given PWM pin.
 * @details Increasing this increases the average signal value, since the period length is kept the same.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] duty_cycle_ns The duty cycle of the PWM pin (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>

void manipulate_duty_cycle(peripheral_pwm_h pwm)
{
	int ret;

	ret = peripheral_pwm_set_duty_cycle(pwm, 1 * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
	ret = peripheral_pwm_set_period(pwm, 10 * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Now the PWM pin is high for 1 ms of every 10 ms, i.e. 10% of the time.

	1et = peripheral_pwm_set_duty_cycle(pwm, 2 * 1000 * 1000);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Now the PWM pin is high for 2 ms of every 10 ms, i.e. 20% of the time.
}
 * @endcode
 */
EXPORT_API int peripheral_pwm_set_duty_cycle(peripheral_pwm_h pwm, uint32_t duty_cycle_ns);

/**
 * @platform
 * @brief Sets the active signal polarity of the given PWM pin.
 * @details Sets the signal polarity between normal and inversed.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] polarity The polarity of the PWM pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_pwm_polarity_e
 *
 * @code
#include <peripheral_io.h>

void set_pwm_polarity_active_high(peripheral_pwm_h pwm)
{
	int ret = peripheral_pwm_set_polarity(pwm, PERIPHERAL_PWM_POLARITY_ACTIVE_HIGH);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_pwm_set_polarity(peripheral_pwm_h pwm, peripheral_pwm_polarity_e polarity);

/**
 * @platform
 * @brief Toggles whether given PWM peripherals pin is enabled.
 * @details Enables or disables the given PWM peripherals pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] enabled Enable/disable the PWM pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>

void disable_pwm(peripheral_pwm_h pwm)
{
	int ret = peripheral_pwm_set_enabled(pwm, false);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_pwm_set_enabled(peripheral_pwm_h pwm, bool enabled);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_ADC_MODULE
 * @{
 */

/**
 * @brief An opaque handle representing an ADC peripherals pin.
 * @details A handle to a single ADC pin, for use with most ADC interfaces
 * @since_tizen 5.0
 */
typedef struct _peripheral_adc_s *peripheral_adc_h;

/**
 * @platform
 * @brief Opens the ADC pin and returns a handle representing it
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a adc should be released with peripheral_adc_close()
 *
 * @param[in] device The ADC device number
 * @param[in] channel The ADC channel number to control
 * @param[out] adc The ADC handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_adc_close()
 *
 * @code
#include <peripheral_io.h>

#define ADC_DEVICE 0
#define ADC_CHANNEL 1

int main(void)
{
	int ret;
	peripheral_adc_h adc;

	ret = peripheral_adc_open(ADC_DEVICE, ADC_CHANNEL, &adc);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_adc_close(adc);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_adc_open(int device, int channel, peripheral_adc_h *adc);

/**
 * @platform
 * @brief Closes the ADC pin and frees the resources associated with it.
 * @details This function should be called at the end of the object's lifetime to avoid a memory leak.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] adc The ADC handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_adc_open()
 *
 * @code
#include <peripheral_io.h>

#define ADC_DEVICE 0
#define ADC_CHANNEL 1

int main(void)
{
	int ret;
	peripheral_adc_h adc;

	ret = peripheral_adc_open(ADC_DEVICE, ADC_CHANNEL, &adc);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_adc_close(adc);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_adc_close(peripheral_adc_h adc);

/**
 * @platform
 * @brief Gets the current value of the ADC peripherals pin.
 * @details Reads the pin value into the provided pointer parameter.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks The value is returned in arbitrary units. It should be interpreted either experimentally, or by investigating the multiplier provided inside kernel's sys tree.
 *
 * @param[in] adc The ADC handle
 * @param[out] value The value to get
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>
#include <glib.h>
#include <stdio.h>

gboolean loop_iteration(gpointer user_data)
{
	peripheral_adc_h adc = user_data;
	int ret;
	uint32_t data;

	ret = peripheral_adc_read(adc, &data);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	printf("Current ADC pin value: %d\n", (int)data);

	return G_SOURCE_CONTINUE;
}

guint read_in_a_loop(peripheral_adc_h adc)
{
	return g_timeout_add_seconds(1, loop_iteration, adc);
}
 * @endcode
 */
EXPORT_API int peripheral_adc_read(peripheral_adc_h adc, uint32_t *value);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_UART_MODULE
 * @{
 */

/**
 * @brief An opaque handle representing a UART peripherals device.
 * @details A handle to a single UART slave device, for use with most UART peripherals interfaces
 * @since_tizen 4.0
 */
typedef struct _peripheral_uart_s *peripheral_uart_h;

/**
 * @brief Enumeration for signal baud rate for UART slave devices.
 * @details Controls the baud rate, i.e. signals per second on a UART device
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_BAUD_RATE_0 = 0,  /**< The number of signal in one second is 0 */
	PERIPHERAL_UART_BAUD_RATE_50,     /**< The number of signal in one second is 50 */
	PERIPHERAL_UART_BAUD_RATE_75,     /**< The number of signal in one second is 75 */
	PERIPHERAL_UART_BAUD_RATE_110,    /**< The number of signal in one second is 110 */
	PERIPHERAL_UART_BAUD_RATE_134,    /**< The number of signal in one second is 134 */
	PERIPHERAL_UART_BAUD_RATE_150,    /**< The number of signal in one second is 150 */
	PERIPHERAL_UART_BAUD_RATE_200,    /**< The number of signal in one second is 200 */
	PERIPHERAL_UART_BAUD_RATE_300,    /**< The number of signal in one second is 300 */
	PERIPHERAL_UART_BAUD_RATE_600,    /**< The number of signal in one second is 600 */
	PERIPHERAL_UART_BAUD_RATE_1200,   /**< The number of signal in one second is 1200 */
	PERIPHERAL_UART_BAUD_RATE_1800,   /**< The number of signal in one second is 1800 */
	PERIPHERAL_UART_BAUD_RATE_2400,   /**< The number of signal in one second is 2400 */
	PERIPHERAL_UART_BAUD_RATE_4800,   /**< The number of signal in one second is 4800 */
	PERIPHERAL_UART_BAUD_RATE_9600,   /**< The number of signal in one second is 9600 */
	PERIPHERAL_UART_BAUD_RATE_19200,  /**< The number of signal in one second is 19200 */
	PERIPHERAL_UART_BAUD_RATE_38400,  /**< The number of signal in one second is 38400 */
	PERIPHERAL_UART_BAUD_RATE_57600,  /**< The number of signal in one second is 57600 */
	PERIPHERAL_UART_BAUD_RATE_115200, /**< The number of signal in one second is 115200 */
	PERIPHERAL_UART_BAUD_RATE_230400, /**< The number of signal in one second is 230400 */
} peripheral_uart_baud_rate_e;

/**
 * @brief Enumeration for data byte size for UART slave devices.
 * @details Controls how many bits per byte are there for data, on UART slave devices.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_BYTE_SIZE_5BIT = 0, /**< 5 data bits */
	PERIPHERAL_UART_BYTE_SIZE_6BIT,     /**< 6 data bits */
	PERIPHERAL_UART_BYTE_SIZE_7BIT,     /**< 7 data bits */
	PERIPHERAL_UART_BYTE_SIZE_8BIT,     /**< 8 data bits */
} peripheral_uart_byte_size_e;

/**
 * @brief Enumeration for parity bit for UART slave devices.
 * @details Controls whether parity is even, odd, or none for UART devices
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_PARITY_NONE = 0, /**< No parity is used */
	PERIPHERAL_UART_PARITY_EVEN,     /**< Even parity is used */
	PERIPHERAL_UART_PARITY_ODD,      /**< ODD parity is used */
} peripheral_uart_parity_e;

/**
 * @brief Enumeration for the number of stop bits for UART slave devices.
 * @details Controls whether there is one or two stop bits for UART devices
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_STOP_BITS_1BIT = 0, /**< One stop bit */
	PERIPHERAL_UART_STOP_BITS_2BIT,     /**< Two stop bits */
} peripheral_uart_stop_bits_e;

/**
 * @brief Enumeration for hardware flow control for UART slave devices.
 * @details Controls details of the hardware flow control on UART devices
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_HARDWARE_FLOW_CONTROL_NONE = 0,    /**< No hardware flow control */
	PERIPHERAL_UART_HARDWARE_FLOW_CONTROL_AUTO_RTSCTS, /**< Automatic RTS/CTS hardware flow control*/
} peripheral_uart_hardware_flow_control_e;

/**
 * @brief Enumeration for software flow control for UART slave devices.
 * @details Controls details of the software flow control on UART devices
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_SOFTWARE_FLOW_CONTROL_NONE = 0,    /**< No software flow control */
	PERIPHERAL_UART_SOFTWARE_FLOW_CONTROL_XONXOFF,     /**< XON/XOFF software flow control */
} peripheral_uart_software_flow_control_e;

/**
 * @platform
 * @brief Opens the UART slave device and returns a handle representing it
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a uart should be released with peripheral_uart_close()
 *
 * @param[in] port The UART port number that the slave device is connected
 * @param[out] uart The UART handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_uart_close()
 *
 * @code
#include <peripheral_io.h>

#define UART_PORT 3

int main(void)
{
	int ret;
	peripheral_uart_h uart;

	ret = peripheral_uart_open(UART_PORT, &uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Set configuration and then use the connection

	ret = peripheral_uart_close(uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_open(int port, peripheral_uart_h *uart);

/**
 * @platform
 * @brief Opens the UART slave device using specified open flags, and returns a handle representing it
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a uart should be released with peripheral_uart_close()
 *
 * @param[in] port The UART port number that the slave device is connected
 * @param[in] flags The flags to open call
 * @param[out] uart The UART handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_uart_close()
 *
 * @code
#include <peripheral_io.h>

#define UART_PORT 3

int main(void)
{
	int ret;
	peripheral_uart_h uart;

	ret = peripheral_uart_open_flags(UART_PORT,
		PERIPHERAL_OPEN_FLAGS_SHARED | PERIPHERAL_OPEN_FLAGS_NONBLOCK,
		&uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Set configuration and then use the connection

	ret = peripheral_uart_close(uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_open_flags(int port, peripheral_open_flags_e flags, peripheral_uart_h *uart);

/**
 * @platform
 * @brief Closes the UART slave device and frees resources associated with the handle.
 * @details This function should be called at the end of the object's lifetime to avoid a memory leak.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_uart_open()
 *
 * @code
#include <peripheral_io.h>

#define UART_PORT 3

int main(void)
{
	int ret;
	peripheral_uart_h uart;

	ret = peripheral_uart_open(UART_PORT, &uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Set configuration and then use the connection

	ret = peripheral_uart_close(uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_close(peripheral_uart_h uart);

/**
 * @platform
 * @brief Sets the baud rate for the given UART slave device.
 * @details Allows to choose the baud rate expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] baud Baud rate of the UART slave device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_baud_rate_e
 *
 * @code
#include <peripheral_io.h>

void set_uart_9600_bps(peripheral_uart_h uart)
{
	int ret = peripheral_uart_set_baud_rate(uart, PERIPHERAL_UART_BAUD_RATE_9600);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_set_baud_rate(peripheral_uart_h uart, peripheral_uart_baud_rate_e baud);

/**
 * @platform
 * @brief Sets data byte size for the given UART slave device.
 * @details Allows to choose the data size expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] byte_size Byte size of the UART slave device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_byte_size_e
 *
 * @code
#include <peripheral_io.h>

void set_uart_8_bits(peripheral_uart_h uart)
{
	int ret = peripheral_uart_set_bits_per_word(uart, PERIPHERAL_UART_BYTE_SIZE_8BIT);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_set_byte_size(peripheral_uart_h uart, peripheral_uart_byte_size_e byte_size);

/**
 * @platform
 * @brief Sets the parity bit for the given UART slave device.
 * @details Allows to choose the parity setting expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] parity Parity bit of the UART slave device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_parity_e
 *
 * @code
#include <peripheral_io.h>

void disable_uart_parity(peripheral_uart_h uart)
{
	int ret = peripheral_uart_set_parity(uart, PERIPHERAL_UART_PARITY_NONE);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_set_parity(peripheral_uart_h uart, peripheral_uart_parity_e parity);

/**
 * @platform
 * @brief Sets the number of stop bits for the UART slave device.
 * @details Allows to choose the number of stop bits expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] stop_bits Stop bits of the UART slave device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_stop_bits_e
 *
 * @code
#include <peripheral_io.h>

void set_uart_1_stop_bit(peripheral_uart_h uart)
{
	int ret = peripheral_uart_set_stop_bits(uart, PERIPHERAL_UART_STOP_BITS_1BIT);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_set_stop_bits(peripheral_uart_h uart, peripheral_uart_stop_bits_e stop_bits);

/**
 * @platform
 * @brief Sets hardware and software flow control of the UART slave device.
 * @details Allows to choose the flow control settings expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] sw_flow_control Software flow control (Turns a transmitter on or off)
 * @param[in] hw_flow_control Hardware flow control (Turns "Request to Send/Clear to Send" on or off)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_software_flow_control_e
 * @see peripheral_uart_hardware_flow_control_e
 *
 * @code
#include <peripheral_io.h>

void set_uart_hw_control_only(peripheral_uart_h uart)
{
	int ret = peripheral_uart_set_flow_control(uart,
		PERIPHERAL_UART_SOFTWARE_FLOW_CONTROL_NONE,
		PERIPHERAL_UART_HARDWARE_FLOW_CONTROL_AUTO_RTSCTS);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_set_flow_control(peripheral_uart_h uart,
			peripheral_uart_software_flow_control_e sw_flow_control,
			peripheral_uart_hardware_flow_control_e hw_flow_control);

/**
 * @platform
 * @brief Reads data from the UART slave device into a local buffer.
 * @details Fills the buffer with data read from the provided device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[out] data The buffer to read
 * @param[in] length The size of buffer (in bytes)
 *
 * @return the number of bytes read on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_TRY_AGAIN Try again
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_write()
 *
 * @code
#include <peripheral_io.h>
#include <stdio.h>

#define BUFFER_SIZE 64

void print_uart_data(peripheral_uart_h uart)
{
	int ret;
	uint8_t data[BUFFER_SIZE];

	ret = peripheral_uart_read(uart, data, BUFFER_SIZE);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	for (unsigned i = 0; i < BUFFER_SIZE; ++i)
		printf("%02x", data[i]);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_read(peripheral_uart_h uart, uint8_t *data, uint32_t length);


/**
 * @platform
 * @brief Discards data queued for writing to UART slave device, but not yet transmitted.
 * @details After this call, there will be no data pending, but some of it might be dropped.
 * @since_tizen 8.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 *
 * @return #PERIPHERAL_ERROR_NONE on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_TRY_AGAIN Try again
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>

void discard_unwritten_data(peripheral_uart_h uart)
{
	int ret = peripheral_uart_flush(uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_flush(peripheral_uart_h uart);

/**
 * @platform
 * @brief Waits for all data queued for UART to be transmitted.
 * @details After this call, there will be no data pending and all of it will be written.
 * @since_tizen 8.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 *
 * @return #PERIPHERAL_ERROR_NONE on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_TRY_AGAIN Try again
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>

void wait_for_data_writing(peripheral_uart_h uart)
{
	int ret = peripheral_uart_drain(uart);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
	// This will block, of course :)
}
 * @endcode
 */
EXPORT_API int peripheral_uart_drain(peripheral_uart_h uart);

/**
 * @platform
 * @brief Writes data from a buffer to the UART slave device.
 * @details Reads the data from the buffer and sends it into the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] data The buffer to write
 * @param[in] length The size of buffer (in bytes)
 *
 * @return the number of bytes write on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_TRY_AGAIN Try again
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_read()
 *
 * @code
#include <peripheral_io.h>

static uint8_t data_to_write[] = { 0x1, 0x2, 0x3, 0x4 };

void write_bytes(peripheral_uart_h uart)
{
	int ret;

	ret = peripheral_uart_write(uart, data_to_write,
		sizeof(data_to_write) / sizeof(data_to_write[0]));
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_uart_write(peripheral_uart_h uart, uint8_t *data, uint32_t length);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_SPI_MODULE
 * @{
 */

/**
 * @brief An opaque handle representing a SPI peripherals slave device.
 * @details A handle to a single SPI slave device, for use with most SPI interfaces
 * @since_tizen 4.0
 */
typedef struct _peripheral_spi_s *peripheral_spi_h;

/**
 * @brief Enumeration of transfer modes for SPI slave devices.
 * @details Use this to control CPOL and CPHa modes on the device.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_SPI_MODE_0 = 0,   /**< CPOL = 0, CPHa = 0 Mode */
	PERIPHERAL_SPI_MODE_1,       /**< CPOL = 0, CPHa = 1 Mode */
	PERIPHERAL_SPI_MODE_2,       /**< CPOL = 1, CPHa = 0 Mode */
	PERIPHERAL_SPI_MODE_3,       /**< CPOL = 1, CPHa = 1 Mode */
} peripheral_spi_mode_e;

/**
 * @brief Enumeration of bit orders for SPI slave device interfaces.
 * @details Sets whether the most or the least significant bit goes first
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_SPI_BIT_ORDER_MSB = 0, /**< Use most significant bit first */
	PERIPHERAL_SPI_BIT_ORDER_LSB,     /**< Use least significant bit first */
} peripheral_spi_bit_order_e;

/**
 * @platform
 * @brief Opens a SPI slave device and returns a handle representing it.
 * @details Starts the lifetime of the handle and allocates its needed resources.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks @a spi should be released with peripheral_spi_close()
 *
 * @param[in] bus The SPI bus number
 * @param[in] cs The SPI chip select number
 * @param[out] spi The SPI slave device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_spi_close()
 *
 * @code
#include <peripheral_io.h>

#define SPI_BUS 0
#define SPI_CS 1

int main(void)
{
	int ret;
	peripheral_spi_h spi;

	ret = peripheral_spi_open(SPI_BUS, SPI_CS, &spi);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_spi_close(spi);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_open(int bus, int cs, peripheral_spi_h *spi);

/**
 * @platform
 * @brief Closes the SPI slave device and frees resources associated with the handle.
 * @details This function should be called at the end of the object's lifetime to avoid a memory leak.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI slave device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_open()
 *
 * @code
#include <peripheral_io.h>

#define SPI_BUS 0
#define SPI_CS 1

int main(void)
{
	int ret;
	peripheral_spi_h spi;

	ret = peripheral_spi_open(SPI_BUS, SPI_CS, &spi);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	// Use the connection here

	ret = peripheral_spi_close(spi);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_close(peripheral_spi_h spi);

/**
 * @platform
 * @brief Sets the transfer mode for the given SPI slave device.
 * @details Allows to choose the transfer mode expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI slave device handle
 * @param[in] mode The SPI transfer mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_mode_e
 *
 * @code
#include <peripheral_io.h>

void set_spi_mode_0(peripheral_spi_h spi)
{
	int ret = peripheral_spi_set_mode(spi, PERIPHERAL_SPI_MODE_0);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_set_mode(peripheral_spi_h spi, peripheral_spi_mode_e mode);

/**
 * @platform
 * @brief Sets the transfer bit order for given SPI slave device.
 * @details Allows to choose the transfer bit order expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks ARTIK530 and Raspberry Pi 3 do not support LSB first bit order.
 *
 * @param[in] spi The SPI slave device handle
 * @param[in] bit_order The transfer bit order
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_bit_order_e
 *
 * @code
#include <peripheral_io.h>

void set_spi_msb(peripheral_spi_h spi)
{
	int ret = peripheral_spi_set_bit_order(spi, PERIPHERAL_SPI_BIT_ORDER_MSB);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_set_bit_order(peripheral_spi_h spi, peripheral_spi_bit_order_e bit_order);

/**
 * @platform
 * @brief Sets the number of bits per word for given SPI slave device.
 * @details Allows to choose the number of bits per word expected by the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI slave device handle
 * @param[in] bits The number of bits per word (in bits)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>

void set_spi_8_bits_per_word(peripheral_spi_h spi)
{
	int ret = peripheral_spi_set_bits_per_word(spi, 8);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_set_bits_per_word(peripheral_spi_h spi, uint8_t bits);

/**
 * @platform
 * @brief Sets the board-dependent frequency of the SPI bus.
 * @details Sets the frequency, in hertz, if the board supports it
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 * @remarks The frequencies supported are board dependent.
 *
 * @param[in] spi The SPI slave device handle
 * @param[in] freq_hz Frequency to set (in Hz)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @code
#include <peripheral_io.h>

void set_spi_frequency(peripheral_spi_h spi)
{
	int ret = peripheral_spi_set_frequency(spi, 8000000);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_set_frequency(peripheral_spi_h spi, uint32_t freq_hz);

/**
 * @platform
 * @brief Reads the bytes data from the SPI slave device to a local buffer.
 * @details Receives data into the specified buffer from the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI slave device handle
 * @param[out] data The data buffer to read
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_write()
 *
 * @code
#include <peripheral_io.h>
#include <stdio.h>

#define BUFFER_SIZE 4

void print_spi_data(peripheral_spi_h spi)
{
	int ret;
	uint8_t data[BUFFER_SIZE];

	ret = peripheral_spi_read(spi, data, BUFFER_SIZE);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	for (unsigned i = 0; i < BUFFER_SIZE; ++i)
		printf("%02x", data[i]);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_read(peripheral_spi_h spi, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Writes the bytes data buffer to the SPI slave device.
 * @details Puts data from the specified buffer unto the device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI slave device handle
 * @param[in] data The data buffer to write
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_read()
 *
 * @code
#include <peripheral_io.h>

static uint8_t data_to_write[] = { 0x1, 0x2, 0x3, 0x4 };

void write_bytes(peripheral_spi_h spi)
{
	int ret;

	ret = peripheral_spi_write(spi, data_to_write,
		sizeof(data_to_write) / sizeof(data_to_write[0]));
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_write(peripheral_spi_h spi, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Exchanges (read and write) the bytes data to the SPI slave device.
 * @details Performs data switcheroo. Essentially a combined write and read call.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI slave device handle
 * @param[in] txdata The data buffer to write
 * @param[out] rxdata The data buffer to read
 * @param[in] length The size of txdata and rxdata buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_read()
 * @see peripheral_spi_write()
 *
 * @code
#include <peripheral_io.h>
#include <stdio.h>

static uint8_t data_to_write[] = { 0x1, 0x2, 0x3, 0x4 };
#define BUFFER_SIZE (sizeof(data_to_write) / sizeof(data_to_write[0]))

void transfer_bytes(peripheral_spi_h spi)
{
	int ret;
	uint8_t rxdata[BUFFER_SIZE];

	ret = peripheral_spi_transfer(spi, data_to_write, rxdata, BUFFER_SIZE);
	if (ret != PERIPHERAL_ERROR_NONE)
		handle_error(ret);

	for (unsigned i = 0; i < BUFFER_SIZE; ++i)
		printf("%02x", rxdata[i]);
}
 * @endcode
 */
EXPORT_API int peripheral_spi_transfer(peripheral_spi_h spi, uint8_t *txdata, uint8_t *rxdata, uint32_t length);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_PERIPHERAL_IO_H__ */
