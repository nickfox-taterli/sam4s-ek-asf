#ifndef ASF_H
#define ASF_H

// From module: Common SAM compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: ACC - Analog Comparator Controller
#include <acc.h>

// From module: ADC - Analog-to-digital Converter
#include <adc.h>

// From module: CHIPID - Chip Identifier
#include <chipid.h>

// From module: CRCCU - Cyclic Redundancy Check Calculation Unit
#include <crccu.h>

// From module: DACC - Digital-to-Analog Converter
#include <dacc.h>

// From module: SMC - Static Memory Controller
#include <smc.h>

// From module: EEFC - Enhanced Embedded Flash Controller
#include <efc.h>

// From module: HSMCI - High Speed MultiMedia Card Interface
#include <hsmci.h>

// From module: MATRIX - Bus Matrix
#include <matrix.h>

// From module: PDC - Peripheral DMA Controller Example
#include <pdc.h>


// From module: PIO - Parallel Input/Output Controller
#include <pio.h>

// From module: PMC - Power Management Controller
#include <pmc.h>
#include <sleep.h>

// From module: PWM - Pulse Width Modulation
#include <pwm.h>

// From module: RSTC - Reset Controller
#include <rstc.h>

// From module: RTC - Real Time Clock
#include <rtc.h>

// From module: RTT - Real Time Timer
#include <rtt.h>

// From module: SPI - Serial Peripheral Interface
#include <spi.h>

// From module: SSC - Synchronous Serial Controller
#include <ssc.h>

// From module: TC - Timer Counter
#include <tc.h>

// From module: TWI - Two-wire Interface
#include <twi.h>

// From module: UART - Univ. Async Rec/Trans
#include <uart.h>

// From module: USART - Univ. Syn Async Rec/Trans
#include <usart.h>

// From module: WDT - Watchdog Timer
#include <wdt.h>

// From module: Flash - SAM Flash Service API
#include <flash_efc.h>

// From module: Atmel QTouch Library 5.0 for Atmel SAM4S devices
#include <touch_api.h>

// From module: Atmel QTouch Library for QTouch Method in SAM devices
#include <touch_qt_config.h>
#include <conf_qtouch.h>

// From module: Delay routines
#include <delay.h>

// From module: FatFS file system
#include <diskio.h>
#include <ff.h>
#include <ffconf.h>

// From module: Memory Control Access Interface
#include <ctrl_access.h>

// From module: NAND Flash on EBI
#include <nand_flash_common.h>
#include <nand_flash_ecc.h>
#include <nand_flash_management.h>
#include <nand_flash_mem.h>
#include <nand_flash_model.h>
#include <nand_flash_raw.h>
#include <nand_flash_spare_scheme.h>
#include <nand_flash_translation.h>

// From module: FreeRTOS
#include <FreeRTOS.h>
#include <croutine.h>
#include <deprecated_definitions.h>
#include <event_groups.h>
#include <list.h>
#include <message_buffer.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <stack_macros.h>
#include <stream_buffer.h>
#include <task.h>
#include <timers.h>

// From module: LVGL
#include "lvgl.h"
#include "porting/lv_port_disp.h"
#include "porting/lv_port_indev.h"
#include "porting/lv_port_fs.h"
#include "demos/lv_demos.h"

// From module: Display - AAT31XX Backlight Controller
#include <aat31xx.h>

// From module: Display - ADS7843 Touch Controller
#include <ads7843.h>

// From module: Display - Generic Resistive Touch Service
#include <rtouch.h>
#include <rtouch_ads7843.h>

// From module: Display - ILI9325 LCD Controller
#include <ili9325.h>

// From module: GPIO - General purpose Input/Output
#include <gpio.h>

// From module: Generic board support
#include <board.h>

// From module: IOPORT - General purpose I/O service
#include <ioport.h>

// From module: Interrupt management - SAM implementation
#include <interrupt.h>

// From module: Part identification macros
#include <parts.h>

// From module: SPI - SAM Implementation
#include <spi_master.h>
#include <spi_master.h>

// From module: SD/MMC Memory Control Access - Enable
#include <sd_mmc_mem.h>

// From module: SD/MMC stack on Multimedia Card interface
#include <sd_mmc.h>

// From module: Standard serial I/O (stdio) - SAM implementation
#include <stdio_serial.h>

// From module: System Clock Control - SAM4S implementation
#include <sysclk.h>

// From module: USART - Serial interface - SAM implementation for devices with both UART and USART
#include <serial.h>

// From module: pio_handler support enabled
#include <pio_handler.h>

// From module: USB Device HID Keyboard (Single Interface Device)
#include <udi_hid_kbd.h>

// From module: USB Device Stack Core (Common API)
#include <udc.h>
#include <udd.h>

// From module: USB HID Device protocol
#include <usb_protocol_hid.h>

// From module: USB HID Library (Device)
#include <udi_hid.h>

// From module: Sleep manager - SAM implementation
#include <sam/sleepmgr.h>
#include <sleepmgr.h>

#endif // ASF_H
