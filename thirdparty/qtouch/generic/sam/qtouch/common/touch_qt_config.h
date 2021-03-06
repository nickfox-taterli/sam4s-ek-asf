/**
 * \file
 *
 * \brief Qtouch config for SAM.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _TOUCH_QT_CONFIG_ARM_H_
#define _TOUCH_QT_CONFIG_ARM_H_

#define _SNS1_SNSK1_SAME_PORT_
#define _SNS2_SNSK2_SAME_PORT_

#define QT_DELAY_CYCLES 5

#define SNSK1 C
#define SNSK2 A
#define SNS1 C
#define SNS2 A

#define _QTOUCH_
#define QT_NUM_CHANNELS 32

/*
 * Default configuration for SAM EK board
 */

#define _TOUCH_ARM_

/*
* Number of Channels(dependent on the library used).Please refer to the user guide
* more information on selecting the number of channels.
*
* Possible values: 32.
*/
#ifndef QT_NUM_CHANNELS
    #define QT_NUM_CHANNELS 32
#endif

/*
* Delay cycles that determine the capacitance charge pulse width.
*
* Possible values: 3,4,5,10,25,50
*/
#ifndef QT_DELAY_CYCLES
    #define QT_DELAY_CYCLES 3
#endif

/*
* Enabling the _ROTOR_SLIDER_ constant will link the library need for using rotors
* and sliders.
*
* Possible values: comment/uncomment the define
*/
#ifndef _ROTOR_SLIDER_
    #define _ROTOR_SLIDER_
#endif

/*
* Define the ports to be used for SNS1 and SNSK1 pins. SNS1 and SNSK1 port pins
* can be available on the same port or on different ports
*
* Possible values: refer to the device data sheet and QTouch libraries user guide.
*/

/*
* Define the ports to be used for SNS2 and SNSK2 pins. SNS2 and SNSK2 port pins
* can be available on the same port or on different ports
*
* Possible values: refer to the device data sheet and QTouch libraries user guide.
*/
/* Should be defined if required */

/**************************************************************/
/* Please do not change any part of the code below this line. */
/**************************************************************/
#if (QT_DELAY_CYCLES == 0)
#error 'QT_DELAY_CYCLES can only have values in the range 1-255.'
#endif

#define _STATIC_PORT_PIN_CONF_ 1

#ifdef _SNS2_SNSK2_SAME_PORT_
    #define QTOUCH_SNS_PORT_COUNT 2
#else
    #define QTOUCH_SNS_PORT_COUNT 1
#endif

#ifdef _SNS1_SNSK1_SAME_PORT_
    #define INTRABURST_1 1
#else
    #define INTRABURST_1 0
#endif

#ifdef _SNS2_SNSK2_SAME_PORT_
    #define INTRABURST_2 1
#else
    #define INTRABURST_2 0
#endif


#ifndef _DEBUG_INTERFACE_
    /* #define _DEBUG_INTERFACE_ */
#endif
#ifndef _QTOUCH_
    #define _QTOUCH_
#endif

#endif /* _TOUCH_QT_CONFIG_ARM_H_ */
