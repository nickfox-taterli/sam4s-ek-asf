/**************************************************************************
 *
 * \file
 *
 * \brief USB configuration file.
 *
 * This file contains the possible external configuration of the USB.
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
 ***************************************************************************/
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */


#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "board.h"
//#include "print_funcs.h"
#include "tracedump.h"
#include "supervisor.h"
#include "usb_ids.h"


//! @defgroup usb_general_conf USB application configuration
//!
//! @{

  // _________________ USB MODE CONFIGURATION ____________________________
  //
  //! @defgroup USB_op_mode USB operating modes configuration
  //! Defines to enable device or host USB operating modes
  //! supported by the application
  //! @{

    //! @brief ENABLE to activate the host software framework support
    //!
    //! Possible values true or false
#define USB_HOST_FEATURE            true

    //! @brief ENABLE to activate the device software framework support
    //!
    //! Possible values true or false
#define USB_DEVICE_FEATURE          true

  //! @}


  // _________________ HOST MODE CONFIGURATION ____________________________
  //
  //! @defgroup USB_host_mode_cfg USB host operating mode configuration
  //!
  //! @{

#if USB_HOST_FEATURE == true

    //!   @brief VID/PID supported table list
    //!
    //!   This table contains the VID/PID that are supported by the reduced-host application.
    //!
    //!   VID_PID_TABLE format definition:\n
    //!   \#define VID_PID_TABLE   {VID1, number_of_PIDs_for_VID1, PID11_value, ..., PID1X_Value,\n
    //!                             ...\n
    //!                             VIDn, number_of_PIDs_for_VIDn, PIDn1_value, ..., PIDnY_Value}
#define VID_PID_TABLE   {ATMEL_VID, 1, EVK1100_CTRL_PANEL_DEMO_PID}

    //!   @brief CLASS/SUBCLASS/PROTOCOL supported table list
    //!
    //!   This table contains the CLASS/SUBCLASS/PROTOCOL that are supported by the reduced-host application.
    //!   This table definition allows to extend the reduced application device support to an entire CLASS/
    //!   SUBCLASS/PROTOCOL instead of a simple VID/PID table list.
    //!
    //!   CLASS_SUBCLASS_PROTOCOL format definition:\n
    //!   \#define CLASS_SUBCLASS_PROTOCOL   {CLASS1, SUB_CLASS1, PROTOCOL1,\n
    //!                                       ...\n
    //!                                       CLASSn, SUB_CLASSn, PROTOCOLn}
#define CLASS_SUBCLASS_PROTOCOL   {MS_CLASS, SFF8020I_MMC2_SUBCLASS, BULK_PROTOCOL,\
                                   MS_CLASS, SFF8070I_SUBCLASS, BULK_PROTOCOL,\
                                   MS_CLASS, SCSI_SUBCLASS, BULK_PROTOCOL}

    //! The size of RAM buffer reserved for descriptor handling
#define SIZEOF_DATA_STAGE        250

    //! The address that will be assigned to the connected device
#define DEVICE_ADDRESS           0x05

    //! The maximal number of interfaces that can be supported (composite device)
#define MAX_INTERFACE_SUPPORTED  0x03

    //! The maximal number of endpoints per interface supported
#define MAX_EP_PER_INTERFACE     3

    //! The host controller will be limited to the strict VID/PID list.
    //! When enabled, if the device VID/PID does not belong to the supported list,
    //! the host controller software will not go to deeper configuration, but to error state.
#define HOST_STRICT_VID_PID_TABLE      DISABLE

    //! Try to configure the host pipe according to the device descriptors received
#define HOST_AUTO_CFG_ENDPOINT         ENABLE

    //! Host Start-of-Frame interrupt always enabled
#define HOST_CONTINUOUS_SOF_INTERRUPT  DISABLE

    //! When host error state detected, go to detached state
#define HOST_ERROR_RESTART             ENABLE

    //! USB host pipes transfers use USB communication interrupt (allows to use non-blocking functions)
#define USB_HOST_PIPE_INTERRUPT_TRANSFER  DISABLE

    //! Force CPU reset upon ID pin change
#define ID_PIN_CHANGE_GENERATE_RESET   ENABLE

    //! Enable time-out delay for host transfer
#define TIMEOUT_DELAY_ENABLE           ENABLE

    //! Delay 1/4 s (250 ms) before time-out value
#define TIMEOUT_DELAY               10

    //! Enable cpt NAK time-out for host transfer
#define NAK_TIMEOUT_ENABLE          DISABLE

    //! Number of NAK handshakes before time-out for transmit functions (up to 0xFFFF)
#define NAK_SEND_TIMEOUT            0x0FFF

    //! Number of NAK handshakes before time-out for receive functions (up to 0xFFFF)
#define NAK_RECEIVE_TIMEOUT         0x0FFF

  #if HOST_AUTO_CFG_ENDPOINT == DISABLE
    //! If no auto configuration of EP, map here user function
#define User_configure_endpoint()
  #endif

    //! @defgroup host_cst_actions USB host custom actions
    //!
    //! @{
      // Write here the action to associate with each USB host event.
      // Be careful not to waste time in order not to disturb the functions.
#define Usb_id_transition_action()
#define Host_device_disconnection_action()        ( v_supervisor_Device_Disconnects() )
#define Host_device_connection_action()           ( v_supervisor_Device_Connects() )
#define Host_sof_action()                         host_sof_action()
#define Host_suspend_action()
#define Host_hwup_action()
#define Host_device_supported_action()
#define Host_device_not_supported_action()
#define Host_new_device_connection_action()       (ms_new_device_connected = true)
#define Host_device_class_not_supported_action()
#define Host_device_error_action()

      //! During the verification of the device class by the host (after device
      //! enumeration), this call-back function allows to look more deeply into
      //! the configuration descriptor. Thus, application-dependent specific
      //! descriptors can be detected and analyzed.
#define Host_user_check_class_action(x)
    //! @}

extern volatile bool ms_new_device_connected;
extern volatile bool ms_connected;
extern void host_sof_action(void);
extern void host_suspend_action(void);

#endif  // USB_HOST_FEATURE == true

  //! @}


  // _________________ DEVICE MODE CONFIGURATION __________________________
  //
  //! @defgroup USB_device_mode_cfg USB device operating mode configuration
  //!
  //! @{

#if USB_DEVICE_FEATURE == true

#define NB_ENDPOINTS          3 // Number of endpoints in the application including control endpoint
#define EP_MS_IN              1
#define EP_MS_OUT             2

    //! @defgroup device_cst_actions USB device custom actions
    //!
    //! @{
      // Write here the action to associate with each USB event.
      // Be careful not to waste time in order not to disturb the functions.
#define Usb_sof_action()                usb_sof_action()
#define Usb_wake_up_action()
#define Usb_resume_action()
#define Usb_suspend_action()
#define Usb_reset_action()
#define Usb_vbus_on_action()            ( v_supervisor_Host_Connects() )
#define Usb_vbus_off_action()           ( v_supervisor_Host_Disconnects() )
#define Usb_set_configuration_action()
    //! @}

#define SBC_VENDOR_ID                   "ATMEL   "          //  8 bytes only
#define SBC_PRODUCT_ID                  "UC3 CTRLPANEL   "  // 16 bytes only
#define SBC_REVISION_ID                 "0.00"              //  4 bytes only

    //! @defgroup device_scsi_cst_actions USB device SCSI custom actions
    //!
    //! @{
      // Write here the action to associate with each SCSI command.
      // Be careful not to waste time in order not to disturb the functions.
#define Scsi_start_read_action()
#define Scsi_stop_read_action()
#define Scsi_start_write_action()
#define Scsi_stop_write_action()
    //! @}

extern void usb_sof_action(void);
extern void usb_suspend_action(void);

#endif  // USB_DEVICE_FEATURE == true

  //! @}


  //! USB interrupt priority level
#define USB_INT_LEVEL                   AVR32_INTC_INT0

  //! Debug trace macro
// #define LOG_STR(str)                    print_dbg(str)
#define LOG_STR(str)                    NAKED_TRACE_COM2(str)


//! @}


#endif  // _CONF_USB_H_
