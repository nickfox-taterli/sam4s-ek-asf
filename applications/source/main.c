#include "asf.h"
#include "board.h"
#include "stdio_serial.h"

#include "conf_usb.h"

extern void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
	printf("stack overflow %x %s\r\n", xTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;)
	{
	}
}

extern void vApplicationTickHook(void)
{
	lv_tick_inc(1);
}

static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = 115200,
		.charlength = 8,
		.paritytype = UART_MR_PAR_NO,
		.stopbits = 1,
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(ID_UART0);
	stdio_serial_init(CONSOLE_UART, &uart_serial_options);
}

static void daemon(void *pvParameters)
{
	UNUSED(pvParameters);
	for (;;)
	{
		rtouch_process();
		lv_task_handler();
		vTaskDelay(1);
	}
}

static void application(void *pvParameters)
{
	UNUSED(pvParameters);

	lv_demo_widgets();
	for (;;)
	{
		vTaskDelay(10000);
	}
}

int main(void)
{
	sysclk_init();
	board_init();

	configure_console();

	lv_init();

	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();

	/* USB Device Init (Demo Keyboard) */
	udc_start();

	xTaskCreate(daemon, "daemon", 1024, NULL, 1, NULL);
	xTaskCreate(application, "app", 768, NULL, 2, NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle task. */
	return 0;
}
