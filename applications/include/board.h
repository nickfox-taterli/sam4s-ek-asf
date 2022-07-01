#ifndef _BOARD_H_
#define _BOARD_H_

#include "compiler.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "system_sam4s.h"

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL (32768U)
#define BOARD_FREQ_SLCK_BYPASS (32768U)
#define BOARD_FREQ_MAINCK_XTAL (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS (12000000U)

/** board main clock xtal startup time */
#define BOARD_OSC_STARTUP_US 15625

/** board main clock xtal startup time */
#define BOARD_OSC_STARTUP_US 15625

// ===== System Clock (MCK) Source Options
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_XTAL
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_BYPASS
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_4M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_8M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_12M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_XTAL
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_BYPASS
#define CONFIG_SYSCLK_SOURCE SYSCLK_SRC_PLLACK
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLBCK

// ===== System Clock (MCK) Prescaler Options   (Fmck = Fsys / (SYSCLK_PRES))
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
#define CONFIG_SYSCLK_PRES SYSCLK_PRES_2
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_4
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_8
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_16
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_32
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_64
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_3

// ===== PLL0 (A) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
#define CONFIG_PLL0_SOURCE PLL_SRC_MAINCK_XTAL
#define CONFIG_PLL0_MUL 20
#define CONFIG_PLL0_DIV 1

// ===== PLL1 (B) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
// Use mul and div effective values here.
#define CONFIG_PLL1_SOURCE          PLL_SRC_MAINCK_XTAL
#define CONFIG_PLL1_MUL             16
#define CONFIG_PLL1_DIV             2

// ===== USB Clock Source Options   (Fusb = FpllX / USB_div)
// Use div effective value here.
//#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL0
#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL1
#define CONFIG_USBCLK_DIV           2

#define CONSOLE_UART UART0
#define CONSOLE_UART_ID ID_UART0

/** Address for transferring command bytes to the nandflash. */
#define BOARD_NF_COMMAND_ADDR 0x60400000
/** Address for transferring address bytes to the nandflash. */
#define BOARD_NF_ADDRESS_ADDR 0x60200000
/** Address for transferring data bytes to the nandflash. */
#define BOARD_NF_DATA_ADDR 0x60000000
/** Bus width for NAND */
#define CONF_NF_BUSWIDTH 8
/** Access timing for NAND */
#define CONF_NF_SETUP_TIMING (SMC_SETUP_NWE_SETUP(0) | SMC_SETUP_NCS_WR_SETUP(1) | SMC_SETUP_NRD_SETUP(0) | SMC_SETUP_NCS_RD_SETUP(1))
#define CONF_NF_PULSE_TIMING (SMC_PULSE_NWE_PULSE(2) | SMC_PULSE_NCS_WR_PULSE(3) | SMC_PULSE_NRD_PULSE(4) | SMC_PULSE_NCS_RD_PULSE(4))
#define CONF_NF_CYCLE_TIMING (SMC_CYCLE_NWE_CYCLE(4) | SMC_CYCLE_NRD_CYCLE(7))

/** Chip select number for nand */
#define BOARD_NAND_CS 0

/** Nandflash chip enable pin definition. */
#define PIN_NF_CE_IDX (PIO_PC14_IDX)

/** Nandflash ready/busy pin definition. */
#define PIN_NF_RB_IDX (PIO_PC18_IDX)

/** Base address of SPI peripheral connected to the touchscreen controller. */
#define BOARD_ADS7843_SPI_BASE SPI
/** Chip select connected to the touchscreen controller. */
#define BOARD_ADS7843_SPI_NPCS 0
/** Touchscreen controller IRQ & Busy pin definition. */
#define BOARD_ADS7843_IRQ_GPIO PIO_PA16_IDX
#define BOARD_ADS7843_IRQ_FLAGS PIO_INPUT | PIO_PULLUP

/** Define ILI9325 base address. */
#define BOARD_ILI9325_ADDR 0x61000000
/** Define ILI9325 register select signal. */
#define BOARD_ILI9325_RS (1 << 1)

/** Backlight pin definition. */
#define BOARD_AAT31XX_SET_GPIO PIO_PC13_IDX

//! Default Config Spi Master Delay BCS
#define CONFIG_SPI_MASTER_DELAY_BCS 0

//! Default Config Spi Master Bits per Transfer Definition
#define CONFIG_SPI_MASTER_BITS_PER_TRANSFER 8

//! Default Config Spi Master Delay BCT
#define CONFIG_SPI_MASTER_DELAY_BCT 0

//! Default Config Spi Master Delay BS
#define CONFIG_SPI_MASTER_DELAY_BS 0

//! Default Config Spi Master Dummy Field
#define CONFIG_SPI_MASTER_DUMMY 0xFF

/** HSMCI pins definition. */
/*! Number of slot connected on HSMCI interface */
#define SD_MMC_HSMCI_MEM_CNT 1
#define SD_MMC_HSMCI_SLOT_0_SIZE 4

/** Enable SD MMC interface pins through HSMCI */
#define CONF_BOARD_SD_MMC_HSMCI

#define SD_MMC_0_CD_PIO_ID          ID_PIOA
#define SD_MMC_0_CD_GPIO            (PIO_PA6_IDX)
#define SD_MMC_0_CD_DETECT_VALUE    0

/**
* Initialization values for the Qtouch library parameters.
*/

/**
  * Sensor detect integration (DI) limit.
  * Default value: 4.
  */
#define DEF_QT_DI                      4u

/**
  * Sensor negative drift rate.
  *
  * Units: 200ms
  * Default value: 20 (4 seconds per LSB).
  */
#define DEF_QT_NEG_DRIFT_RATE          20	/* 4s per LSB */

/**
  * Sensor positive drift rate.
  *
  * Units: 200ms
  * Default value: 5 (1 second per LSB).
  */
#define DEF_QT_POS_DRIFT_RATE          5	/* 1s per LSB */

/**
  * Sensor drift hold time.
  *
  * Units: 200ms
  * Default value: 20 (hold off drifting for 4 seconds after leaving detect).
  */
#define DEF_QT_DRIFT_HOLD_TIME         20	/* 4s */

/**
  * Sensor maximum on duration.
  *
  * Units: 200ms (e.g., 150 = recalibrate after 30s). 0 = no recalibration.
  * Default value: 0 (recalibration disabled).
  */
#define DEF_QT_MAX_ON_DURATION         0	/* disabled */

/**
  * Sensor recalibration threshold.
  *
  * Default: RECAL_50 (recalibration threshold = 50% of detection threshold).
  */
#define DEF_QT_RECAL_THRESHOLD         RECAL_50	/* recal threshold = 50% of detect */

/**
  * Positive recalibration delay.
  *
  * Default: 3
  */
#define DEF_QT_POS_RECAL_DELAY         3u

#define GET_SENSOR_STATE(SENSOR_NUMBER) (qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] & (1 << (SENSOR_NUMBER % 8)))
#define GET_ROTOR_SLIDER_POSITION(ROTOR_SLIDER_NUMBER) qt_measure_data.qt_touch_status.rotor_slider_values[ROTOR_SLIDER_NUMBER]

#define CONF_BOARD_AAT3155
  
/*! \brief This function initializes the board target resources
 *
 * This function should be called to ensure proper initialization of the target
 * board hardware connected to the part.
 */
extern void board_init(void);

#ifdef __cplusplus
}
#endif

/**
 * \}
 */

#endif // _BOARD_H_
