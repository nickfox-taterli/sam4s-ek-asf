/**
 * \file
 *
 * \brief AT32UC3A3 RZ600 board LEDs support package.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <avr32/io.h>
#include "preprocessor.h"
#include "compiler.h"
#include "rz600.h"
#include "led.h"


//! Structure describing LED hardware connections.
typedef const struct
{
  struct
  {
    U32 PORT;     //!< LED GPIO port.
    U32 PIN_MASK; //!< Bit-mask of LED pin in GPIO port.
  } GPIO; //!< LED GPIO descriptor.
} tLED_DESCRIPTOR;


//! Hardware descriptors of all LEDs.
static tLED_DESCRIPTOR LED_DESCRIPTOR[LED_COUNT] =
{
#define INSERT_LED_DESCRIPTOR(LED_NO, unused)                 \
  {                                                           \
    {LED##LED_NO##_GPIO / 32, 1 << (LED##LED_NO##_GPIO % 32)}\
  },
  MREPEAT(LED_COUNT, INSERT_LED_DESCRIPTOR, ~)
#undef INSERT_LED_DESCRIPTOR
};


//! Saved state of all LEDs.
static volatile U32 LED_State = (1 << LED_COUNT) - 1;


U32 LED_Read_Display(void)
{
  return LED_State;
}


void LED_Display(U32 leds)
{
  // Use the LED descriptors to get the connections of a given LED to the MCU.
  tLED_DESCRIPTOR *led_descriptor;
  volatile avr32_gpio_port_t *led_gpio_port;

  // Make sure only existing LEDs are specified.
  leds &= (1 << LED_COUNT) - 1;

  // Update the saved state of all LEDs with the requested changes.
  LED_State = leds;

  // For all LEDs...
  for (led_descriptor = &LED_DESCRIPTOR[0];
       led_descriptor < LED_DESCRIPTOR + LED_COUNT;
       led_descriptor++)
  {
    // Set the LED to the requested state.
    led_gpio_port = &AVR32_GPIO.port[led_descriptor->GPIO.PORT];
    if (leds & 1)
    {
      led_gpio_port->ovrc  = led_descriptor->GPIO.PIN_MASK;
    }
    else
    {
      led_gpio_port->ovrs  = led_descriptor->GPIO.PIN_MASK;
    }
    led_gpio_port->oders = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->gpers = led_descriptor->GPIO.PIN_MASK;
    leds >>= 1;
  }
}


U32 LED_Read_Display_Mask(U32 mask)
{
  return Rd_bits(LED_State, mask);
}


void LED_Display_Mask(U32 mask, U32 leds)
{
  // Use the LED descriptors to get the connections of a given LED to the MCU.
  tLED_DESCRIPTOR *led_descriptor = &LED_DESCRIPTOR[0] - 1;
  volatile avr32_gpio_port_t *led_gpio_port;
  U8 led_shift;

  // Make sure only existing LEDs are specified.
  mask &= (1 << LED_COUNT) - 1;

  // Update the saved state of all LEDs with the requested changes.
  Wr_bits(LED_State, mask, leds);

  // While there are specified LEDs left to manage...
  while (mask)
  {
    // Select the next specified LED and set it to the requested state.
    led_shift = 1 + ctz(mask);
    led_descriptor += led_shift;
    led_gpio_port = &AVR32_GPIO.port[led_descriptor->GPIO.PORT];
    leds >>= led_shift - 1;
    if (leds & 1)
    {
      led_gpio_port->ovrc  = led_descriptor->GPIO.PIN_MASK;
    }
    else
    {
      led_gpio_port->ovrs  = led_descriptor->GPIO.PIN_MASK;
    }
    led_gpio_port->oders = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->gpers = led_descriptor->GPIO.PIN_MASK;
    leds >>= 1;
    mask >>= led_shift;
  }
}


bool LED_Test(U32 leds)
{
  return Tst_bits(LED_State, leds);
}


void LED_Off(U32 leds)
{
  // Use the LED descriptors to get the connections of a given LED to the MCU.
  tLED_DESCRIPTOR *led_descriptor = &LED_DESCRIPTOR[0] - 1;
  volatile avr32_gpio_port_t *led_gpio_port;
  U8 led_shift;

  // Make sure only existing LEDs are specified.
  leds &= (1 << LED_COUNT) - 1;

  // Update the saved state of all LEDs with the requested changes.
  Clr_bits(LED_State, leds);

  // While there are specified LEDs left to manage...
  while (leds)
  {
    // Select the next specified LED and turn it off.
    led_shift = 1 + ctz(leds);
    led_descriptor += led_shift;
    led_gpio_port = &AVR32_GPIO.port[led_descriptor->GPIO.PORT];
    led_gpio_port->ovrs  = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->oders = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->gpers = led_descriptor->GPIO.PIN_MASK;
    leds >>= led_shift;
  }
}


void LED_On(U32 leds)
{
  // Use the LED descriptors to get the connections of a given LED to the MCU.
  tLED_DESCRIPTOR *led_descriptor = &LED_DESCRIPTOR[0] - 1;
  volatile avr32_gpio_port_t *led_gpio_port;
  U8 led_shift;

  // Make sure only existing LEDs are specified.
  leds &= (1 << LED_COUNT) - 1;

  // Update the saved state of all LEDs with the requested changes.
  Set_bits(LED_State, leds);

  // While there are specified LEDs left to manage...
  while (leds)
  {
    // Select the next specified LED and turn it on.
    led_shift = 1 + ctz(leds);
    led_descriptor += led_shift;
    led_gpio_port = &AVR32_GPIO.port[led_descriptor->GPIO.PORT];
    led_gpio_port->ovrc  = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->oders = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->gpers = led_descriptor->GPIO.PIN_MASK;
    leds >>= led_shift;
  }
}


void LED_Toggle(U32 leds)
{
  // Use the LED descriptors to get the connections of a given LED to the MCU.
  tLED_DESCRIPTOR *led_descriptor = &LED_DESCRIPTOR[0] - 1;
  volatile avr32_gpio_port_t *led_gpio_port;
  U8 led_shift;

  // Make sure only existing LEDs are specified.
  leds &= (1 << LED_COUNT) - 1;

  // Update the saved state of all LEDs with the requested changes.
  Tgl_bits(LED_State, leds);

  // While there are specified LEDs left to manage...
  while (leds)
  {
    // Select the next specified LED and toggle it.
    led_shift = 1 + ctz(leds);
    led_descriptor += led_shift;
    led_gpio_port = &AVR32_GPIO.port[led_descriptor->GPIO.PORT];
    led_gpio_port->ovrt  = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->oders = led_descriptor->GPIO.PIN_MASK;
    led_gpio_port->gpers = led_descriptor->GPIO.PIN_MASK;
    leds >>= led_shift;
  }
}


U32 LED_Read_Display_Field(U32 field)
{
  return Rd_bitfield(LED_State, field);
}


void LED_Display_Field(U32 field, U32 leds)
{
  // Move the bit-field to the appropriate position for the bit-mask.
  LED_Display_Mask(field, leds << ctz(field));
}

