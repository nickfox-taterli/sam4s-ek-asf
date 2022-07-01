#include "compiler.h"
#include "board.h"
#include "gpio.h"
#include "smc.h"
#include "ioport.h"
#include "pio_handler.h"

void board_init(void)
{
	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;

	/* GPIO has been deprecated, the old code just keeps it for compatibility.
	 * In new designs IOPORT is used instead.
	 * Here IOPORT must be initialized for others to use before setting up IO.
	 */
	ioport_init();

	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_PIOC);

	/* Configure LED pins as GPIO */
	gpio_configure_pin(PIO_PA19_IDX, PIO_OUTPUT_1 | PIO_DEFAULT); /* LED0 */
	gpio_configure_pin(PIO_PA20_IDX, PIO_OUTPUT_1 | PIO_DEFAULT); /* LED1 */
	gpio_configure_pin(PIO_PC20_IDX, PIO_OUTPUT_1 | PIO_DEFAULT); /* LED2 */

	/* Configure LED pins as PWM */
	// gpio_configure_pin(PIO_PA19_IDX, PIO_OUTPUT_1 | PIO_PERIPH_B | PIO_DEFAULT); /* LED0 */
	// gpio_configure_pin(PIO_PA20_IDX, PIO_OUTPUT_1 | PIO_PERIPH_B | PIO_DEFAULT); /* LED1 */
	// gpio_configure_pin(PIO_PC20_IDX, PIO_OUTPUT_1 | PIO_PERIPH_B | PIO_DEFAULT); /* LED2 */

	/* Configure Push Button pins */
	gpio_configure_pin(PIO_PB3_IDX, PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE);	/* PB1 */
	gpio_configure_pin(PIO_PC12_IDX, PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE); /* PB2 */

	/* Configure UART pins */
	gpio_configure_group(PIOA, PIO_PA9A_URXD0 | PIO_PA10A_UTXD0, PIO_PERIPH_A | PIO_DEFAULT);

	/* Configure SPI pins */
	gpio_configure_pin(PIO_PA12_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* MISO */
	gpio_configure_pin(PIO_PA13_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* MOSI */
	gpio_configure_pin(PIO_PA14_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* SCLK */

	/**
	 * For NPCS 1, 2, and 3, different PINs can be used to access the same NPCS line.
	 * Depending on the application requirements, the default PIN may not be available.
	 * Hence a different PIN should be selected using the CONF_BOARD_SPI_NPCS_GPIO and
	 * CONF_BOARD_SPI_NPCS_FLAGS macros.
	 */

	/* Configure SPI CS pins */
	gpio_configure_pin(PIO_PA11_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* CS0 - Touch CS */

	/* Configure USART pins */
	gpio_configure_pin(PIO_PA21_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* RXD */
	gpio_configure_pin(PIO_PA22_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* TXD */

	gpio_configure_pin(PIO_PA23_IDX, PIO_OUTPUT_0 | PIO_DEFAULT); /* DE */
	gpio_configure_pin(PIO_PA24_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* RTS */
	gpio_configure_pin(PIO_PA25_IDX, PIO_PERIPH_A | PIO_DEFAULT); /* CTS */
	gpio_set_pin_low(PIO_PA23_IDX);

	/* Configure LCD/NAND EBI pins */
	gpio_configure_pin(PIO_PC0_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PC1_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PC2_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PC3_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PC4_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PC5_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PC6_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PC7_IDX, PIO_PERIPH_A | PIO_PULLUP);

	/* Configure LCD pins */
	gpio_configure_pin(PIO_PC11_IDX, PIO_PERIPH_A | PIO_PULLUP); /* NRD */
	gpio_configure_pin(PIO_PC8_IDX, PIO_PERIPH_A | PIO_PULLUP);	 /* NWE */
	gpio_configure_pin(PIO_PC15_IDX, PIO_PERIPH_A | PIO_PULLUP); /* NCS */
	gpio_configure_pin(PIO_PC19_IDX, PIO_PERIPH_A | PIO_PULLUP); /* RS */

	/* Configure Backlight control pin */
	gpio_configure_pin(PIO_PC13_IDX, PIO_OUTPUT_0 | PIO_DEFAULT); /* BL */

	/* Configure Touchscreen SPI pins */
	gpio_configure_pin(PIO_PA16_IDX, PIO_INPUT | PIO_PULLUP); /* IRQ */
	gpio_configure_pin(PIO_PA17_IDX, PIO_INPUT | PIO_PULLUP); /* BUSY */

	gpio_configure_pin(PIO_PC9_IDX, PIO_PERIPH_A | PIO_PULLUP);			   /* OE */
	gpio_configure_pin(PIO_PC10_IDX, PIO_PERIPH_A | PIO_PULLUP);		   /* WE */
	gpio_configure_pin(PIO_PC17_IDX, PIO_PERIPH_A | PIO_PULLUP);		   /* CLE */
	gpio_configure_pin(PIO_PC16_IDX, PIO_PERIPH_A | PIO_PULLUP);		   /* ALE */
	gpio_configure_pin(PIO_PC14_IDX, PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT); /* CE */
	gpio_configure_pin(PIO_PC18_IDX, PIO_INPUT | PIO_PULLUP);			   /* RB */

	/* Configure HSMCI pins */
	gpio_configure_pin(PIO_PA28_IDX, PIO_PERIPH_C | PIO_DEFAULT); /* CMD */
	gpio_configure_pin(PIO_PA29_IDX, PIO_PERIPH_C | PIO_DEFAULT); /* CLK */
	gpio_configure_pin(PIO_PA30_IDX, PIO_PERIPH_C | PIO_DEFAULT); /* D0 */
	gpio_configure_pin(PIO_PA31_IDX, PIO_PERIPH_C | PIO_DEFAULT); /* D1 */
	gpio_configure_pin(PIO_PA26_IDX, PIO_PERIPH_C | PIO_DEFAULT); /* D2 */
	gpio_configure_pin(PIO_PA27_IDX, PIO_PERIPH_C | PIO_DEFAULT); /* D3 */

	/* Configure SD/MMC card detect pin */
	gpio_configure_pin(PIO_PA6_IDX, PIO_INPUT | PIO_PULLUP);

	/* Configure VBUS GPIO pin */
	gpio_configure_pin(PIO_PC21_IDX, PIO_INPUT | PIO_DEBOUNCE | PIO_IT_EDGE);

	/* Enable SMC clock */
	pmc_enable_periph_clk(ID_SMC);
}
