/*****************************************************************************
 *
 * \file
 *
 * \brief Clock configuration file with a 14.7456MHz crystal on OSC0 and 11.2896MHz
 * crystal on OSC1.
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
 ******************************************************************************/
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <avr32/io.h>
#include "board.h"
#include "clocks.h"
#include "conf_audio_player.h"
#include "conf_audio_mixer.h"

#include "pm.h"
#include "flashc.h"
#include "gpio.h"

#if DEFAULT_DACS == AUDIO_MIXER_DAC_ABDAC
  #include "abdac.h"
  #include "tpa6130.h"
  #include "conf_tpa6130.h"
#elif DEFAULT_DACS == AUDIO_MIXER_DAC_AIC23B
  #include "tlv320aic23b.h"
  #include "conf_tlv320aic23b.h"
#endif

// Check clock configuration

#warning This crystal configuration is not recommended to decode high-bitrate audio files.

#if FOSC0 != 14745600 || FOSC1 != 11289600
  #error FOSC0 must be equal to 14.7456MHz and FOSC1 to 11.2896MHz
#endif

#if DEFAULT_DACS == AUDIO_MIXER_DAC_AIC23B
  #if !defined(AIC23B_DAC_USE_RX_CLOCK) || AIC23B_DAC_USE_RX_CLOCK == DISABLED
    #error AIC23B_DAC_USE_RX_CLOCK must be enabled.
  #endif
#endif

#if FCPU_HZ != 55296000 || \
    FHSB_HZ != 55296000 || \
    FPBB_HZ != 55296000 || \
    FPBA_HZ != 55296000
  #error FCPU_HZ, FHSB_HZ, FPBB_HZ and FPBA_HZ must be equal to 55.296MHz
#endif

static void init_usb_clock(void);
static void init_codec_gclk(void);
       void set_gclk2_freq(int freq_hz);

/*! \brief Initializes the MCU system clocks.
 */
void init_sys_clocks(void)
{
  // Switch to OSC0 to speed up the booting
  pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

  // Start oscillator1
  pm_enable_osc1_crystal(&AVR32_PM, FOSC0);
  //
  pm_enable_clk1(&AVR32_PM, OSC0_STARTUP);

  // Set PLL0 (fed from OSC0 = 14.7456 MHz) to 110.592 MHz
  // 14.7456*15/2
  pm_pll_setup(&AVR32_PM, 0,  // pll.
    14,  // mul.
    2,   // div.
    0,   // osc.
    16); // lockcount.

  // Set PLL operating range and divider (fpll = fvco)
  // -> PLL0 output = 110.592 MHz
  pm_pll_set_option(&AVR32_PM, 0, // pll.
    1,  // pll_freq.
    0,  // pll_div2.
    0); // pll_wbwdisable.

  // start PLL0 and wait for the lock
  pm_pll_enable(&AVR32_PM, 0);
  pm_wait_for_pll0_locked(&AVR32_PM);
  // Set all peripheral clocks to run at master clock rate
  pm_cksel(&AVR32_PM,
    1,   // pbadiv.
    0,   // pbasel.
    1,   // pbbdiv.
    0,   // pbbsel.
    1,   // hsbdiv.
    0);  // hsbsel.

  // Set one waitstate for the flash
  flashc_set_wait_state(1);

  // Switch to PLL0 as the master clock
  pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCCTRL_MCSEL_PLL0);

  init_usb_clock();
  init_codec_gclk();
}

/*! \brief Initializes the USB clock.
 */
static void init_usb_clock(void)
{
  // Set PLL1 (fed from OSC0 = 14.7456 MHz) to 95.8464 MHz
  pm_pll_setup(&AVR32_PM, 1,  // pll.
	  12,   // mul.
	  2,   // div.
	  0,   // osc.
	  16); // lockcount.

  pm_pll_set_option(&AVR32_PM, 1, // pll.
	  1,  // pll_freq: choose the range 80-180MHz.
	  1,  // pll_div2.
	  0); // pll_wbwdisable.

  // start PLL1 and wait forl lock
  pm_pll_enable(&AVR32_PM, 1);

  // Wait for PLL1 locked.
  pm_wait_for_pll1_locked(&AVR32_PM);

  pm_gc_setup(&AVR32_PM, AVR32_PM_GCLK_USBB,  // gc.
            1,  // osc_or_pll: use Osc (if 0) or PLL (if 1).
            1,  // pll_osc: select Osc0/PLL0 or Osc1/PLL1.
            0,  // diven.
            0); // div.
  pm_gc_enable(&AVR32_PM, AVR32_PM_GCLK_USBB);
}

#if DEFAULT_DACS == AUDIO_MIXER_DAC_ABDAC

static void init_codec_gclk(void)
{
  // Configure the ABDAC generic clock
  // We do not activate it here since it is done in the low level DRIVERS/ABDAC/abdac.c driver.
  pm_gc_setup(&AVR32_PM, AVR32_PM_GCLK_ABDAC,
    AVR32_GC_USES_OSC, AVR32_GC_USES_OSC1, 0, 0);
}

void set_gclk2_freq(int freq_hz)
{
  if (!abdac_set_dac_sample_rate(freq_hz))
    abdac_set_dac_hz(TPA6130_ABDAC, FOSC1, freq_hz);
}

#elif DEFAULT_DACS == AUDIO_MIXER_DAC_AIC23B

void set_gclk2_freq(int freq_hz)
{
  const int gc_master_clock = 0;
  const int gc_tx_clock = 2;
  int temp;

  pm_gc_disable(&AVR32_PM, gc_master_clock);
  pm_gc_disable(&AVR32_PM, gc_tx_clock);
  switch (freq_hz)
  {
  // 32000 - stereo - 16 bits -> PLL0 (110.592MHz)
  case (32000*2*16):
    // Codec master clock - 18.432MHz
    pm_gc_setup(&AVR32_PM, gc_master_clock, AVR32_GC_USES_PLL, AVR32_GC_USES_PLL0, 1, 2);
    // I2S TX clock - 110.592MHz/108 = 1.024MHz = freq_hz
    pm_gc_setup(&AVR32_PM, gc_tx_clock, AVR32_GC_USES_PLL, AVR32_GC_USES_PLL0, 1, 53);
    // Configure the DAC
    aic23b_configure_freq(18432000, 32000);
    break;
  // 44100 - stereo - 16 bits -> OSC1 (11.289600MHz)
  case (44100*2*16):
    // Codec master clock - 11.289600MHz
    pm_gc_setup(&AVR32_PM, gc_master_clock, AVR32_GC_USES_OSC, AVR32_GC_USES_OSC1, 0, 0);
    // I2S TX clock - 11.289600MHz/8 = 1.4112MHz = freq_hz
    pm_gc_setup(&AVR32_PM, gc_tx_clock, AVR32_GC_USES_OSC, AVR32_GC_USES_OSC1, 1, 3);
    // Configure the DAC
    aic23b_configure_freq(11289600, 44100);
    break;
  // 48000 - stereo - 16 bits -> PLL0 (110.592MHz)
  case (48000*2*16):
    // Codec master clock - 18.432MHz
    pm_gc_setup(&AVR32_PM, gc_master_clock, AVR32_GC_USES_PLL, AVR32_GC_USES_PLL0, 1, 2);
    // I2S TX clock - 110.592MHz/72 = 1.536MHz = freq_hz
    pm_gc_setup(&AVR32_PM, gc_tx_clock, AVR32_GC_USES_PLL, AVR32_GC_USES_PLL0, 1, 35);
    // Configure the DAC
    aic23b_configure_freq(18432000, 48000);
    break;
  default:
    // not supported! (but we try anyway! to avoid locked-up conditions)
    // Codec master clock - 11.289600MHz
    pm_gc_setup(&AVR32_PM, gc_master_clock, AVR32_GC_USES_OSC, AVR32_GC_USES_OSC1, 0, 0);
    // I2S TX clock
    temp = (FOSC1 + freq_hz) / (2*freq_hz) - 1;
    pm_gc_setup(&AVR32_PM, gc_tx_clock, AVR32_GC_USES_OSC, AVR32_GC_USES_OSC1, 1, temp);
    // Configure the DAC
    aic23b_configure_freq(FOSC1, freq_hz / (2*16));
  }
  pm_gc_enable(&AVR32_PM, gc_tx_clock);
  pm_gc_enable(&AVR32_PM, gc_master_clock);
}

/*! \brief Sets up generic clock for the audio codec.
 */
static void init_codec_gclk(void)
{
  // GCLK to supply the I2S TX clock
  gpio_enable_module_pin(TLV320_PM_GCLK_RX_PIN, TLV320_PM_GCLK_RX_FUNCTION);
  gpio_enable_module_pin(TLV320_PM_GCLK_PIN, TLV320_PM_GCLK_FUNCTION);
}

#else

static void init_codec_gclk(void)
{
}

void set_gclk2_freq(int freq_hz)
{
}

#endif
