/**
 * \file
 *
 * \brief Audio player configuration file.
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
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _CONF_AUDIO_PLAYER_H_
#define _CONF_AUDIO_PLAYER_H_

#include <avr32/io.h>
#include "board.h"
#include "compiler.h"

/*! \name System Clock Frequencies
 */
//! @{
#define FMCK_HZ                           62092800
#define FCPU_HZ                           FMCK_HZ
#define FHSB_HZ                           FCPU_HZ
#define FPBB_HZ                           FMCK_HZ
#define FPBA_HZ                           FMCK_HZ
//! @}

//! Default heap initialization word.
#define DEFAULT_HEAP_INIT_WORD            0xA5A5A5A5

/*! \name Default DAC Settings
 * Please choose between:
 * - AUDIO_MIXER_DAC_AIC23B  (external DAC mounted on EVK1105 board)
 * - AUDIO_MIXER_DAC_ABDAC (internal ABDAC with external amplifier tap6130
 *   on EVK1105 and EVK1104)
 */
//! @{
#define DEFAULT_DACS                      AUDIO_MIXER_DAC_ABDAC
#define DEFAULT_DAC_SAMPLE_RATE_HZ        44100
#define DEFAULT_DAC_NUM_CHANNELS          2
#define DEFAULT_DAC_BITS_PER_SAMPLE       16
#define DEFAULT_DAC_SWAP_CHANNELS         false
//! @}

/*! \name Audio player sample bufferization.
 */
#define USE_AUDIO_PLAYER_BUFFERIZATION    true

/*! \name Audio player features.
 */
//! @{
#define SUPPORT_WAV                       true
#define SUPPORT_PLAYLISTS                 true
#define SUPPORT_EMBEDDED_COVER_ARTS       false
//! @}

/******* Should not be changed, used to support features *******/

/******* Set audio player configuration ************************/

#define SUPPORT_USB_MS                true
#define SUPPORT_SD_MMC                true

// ***** SD/MMC Card support ***************************************
#if defined(SUPPORT_SD_MMC) && SUPPORT_SD_MMC == true
  #define SD_MMC_INCLUDED                         true
  #define SUPPORT_SD_MMC_MCI                      true
#endif

// ***** Mass Storage Support **********************************
#if defined(SUPPORT_USB_MS) && SUPPORT_USB_MS == true
  #define USB_MS_INCLUDED                         true
  #define task_init_usb_ms()                      \
  {                                               \
    host_mass_storage_task_init();                \
  }
  #define task_usb_ms()                           \
  {                                               \
    host_mass_storage_task();                     \
  }
  #define USB_MS_VID_PID_TABLE                    ATMEL_VID, 1, MS_EXAMPLE_PID,
  #define USB_MS_SUBCLASS_PROTOCOL                MS_CLASS, SFF8020I_MMC2_SUBCLASS, BULK_PROTOCOL,  \
                                                  MS_CLASS, SFF8070I_SUBCLASS, BULK_PROTOCOL,       \
                                                  MS_CLASS, SCSI_SUBCLASS, BULK_PROTOCOL,
  #define USB_MS_MAX_INTERFACE_SUPPORTED          3
  #define usb_ms_disconnection_action()           \
  {                                               \
    ms_new_device_connected = false;              \
    ms_connected = false;                         \
  }
  #define usb_ms_new_device_connection_action()   \
  {                                               \
    ms_new_device_connected = true;               \
  }
  extern volatile bool ms_new_device_connected;
  extern volatile bool ms_connected;
  extern void host_suspend_action(void);
#else
  #define USB_MS_INCLUDED                         false
  #define task_init_usb_ms()
  #define task_usb_ms()
  #define USB_MS_VID_PID_TABLE
  #define USB_MS_SUBCLASS_PROTOCOL
  #define USB_MS_MAX_INTERFACE_SUPPORTED          0
  #define usb_ms_disconnection_action()
  #define usb_ms_new_device_connection_action()
#endif


// ***** USB Support *******************************************
#if UC3A3
  #define USB_HIGH_SPEED_SUPPORT      true
#endif
#if (defined(SUPPORT_USB_MS) && SUPPORT_USB_MS == true)

  // USB Device / Host features
  #define USB_HOST_FEATURE            true
  #define USB_DEVICE_FEATURE          false

  #define VID_PID_TABLE                         \
  {                                             \
    USB_MS_VID_PID_TABLE                        \
  }
  #define CLASS_SUBCLASS_PROTOCOL               \
  {                                             \
    USB_MS_SUBCLASS_PROTOCOL                    \
  }
  #define Host_device_disconnection_action()    \
  {                                             \
    usb_ms_disconnection_action();              \
  }
  #define Host_new_device_connection_action()   \
  {                                             \
    usb_ms_new_device_connection_action();      \
  }
  #define Host_user_check_class_action(x)       \
  {                                             \
  }
  #define MAX_INTERFACE_SUPPORTED               USB_MS_MAX_INTERFACE_SUPPORTED
  #if defined(SUPPORT_USB_MS) && SUPPORT_USB_MS == true
    #include "host_mass_storage_task.h"
  #endif
  #include "usb_task.h"
  #define task_init_usb()                       \
  {                                             \
    usb_task_init();                            \
  }
  #define task_usb()                            \
  {                                             \
    usb_task();                                 \
  }
#else
  #define task_init_usb()
  #define task_usb()
#endif


// ***** WAV Support *******************************************
#if defined(SUPPORT_WAV) && SUPPORT_WAV == true
  #include "ai_usb_ms_wav_support.h"
  #define task_wav()                  \
  {                                   \
    extern void codec_wav_task(void); \
    codec_wav_task();                 \
  }
  #define file_format_wav             WAV_FILE_FORMAT,
#else
  #define task_wav()
  #define file_format_wav
#endif


// *************************************************************

#define task_init()                   \
{                                     \
  task_init_usb();                    \
  task_init_usb_ms();                 \
}

#define task()                        \
{                                     \
  task_usb();                         \
  task_usb_ms();                      \
  task_wav();                         \
}

#define SUPPORTED_FILE_FORMATS        \
{                                     \
  file_format_wav                     \
}

#undef AI_SYNC_TASK_CALL_BACK
#define AI_SYNC_TASK_CALL_BACK()      \
{                                     \
  task_usb();                         \
}

#endif  // _CONF_AUDIO_PLAYER_H_
