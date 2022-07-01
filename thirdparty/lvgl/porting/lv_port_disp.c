/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
// static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//         const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                             /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);   /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = ILI9325_LCD_WIDTH;
    disp_drv.ver_res = ILI9325_LCD_HEIGHT;
    // disp_drv.sw_rotate = 1;
    // disp_drv.rotated = LV_DISP_ROT_90;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for Example 3)*/
    // disp_drv.full_refresh = 1

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    // disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    struct ili9325_opt_t g_display_opt;

    /* Switch off backlight */
    aat31xx_disable_backlight();

    /* Enable SMC clock */
    pmc_enable_periph_clk(ID_SMC);

    smc_set_setup_timing(SMC, 1, SMC_SETUP_NWE_SETUP(2) | SMC_SETUP_NCS_WR_SETUP(2) | SMC_SETUP_NRD_SETUP(2) | SMC_SETUP_NCS_RD_SETUP(2));
    smc_set_pulse_timing(SMC, 1, SMC_PULSE_NWE_PULSE(4) | SMC_PULSE_NCS_WR_PULSE(4) | SMC_PULSE_NRD_PULSE(10) | SMC_PULSE_NCS_RD_PULSE(10));
    smc_set_cycle_timing(SMC, 1, SMC_CYCLE_NWE_CYCLE(10) | SMC_CYCLE_NRD_CYCLE(22));
    smc_set_mode(SMC, 1, SMC_MODE_READ_MODE | SMC_MODE_WRITE_MODE);

    g_display_opt.ul_width = ILI9325_LCD_WIDTH;
    g_display_opt.ul_height = ILI9325_LCD_HEIGHT;
    g_display_opt.foreground_color = COLOR_WHITE;
    g_display_opt.background_color = COLOR_BLACK;

    /* Initialize ili9325 */
    ili9325_init(&g_display_opt);

    ili9325_draw_filled_rectangle(0, 0, ILI9325_LCD_WIDTH, ILI9325_LCD_HEIGHT);

    /* Turn on LCD */
    ili9325_display_on();
    /* Switch on backlight */
    aat31xx_set_backlight(AAT31XX_MAX_BACKLIGHT_LEVEL);
}

#define RGB_16_TO_24BIT_B(RGB) ((RGB)&0x1F) << 3
#define RGB_16_TO_24BIT_G(RGB) ((RGB >> 5)) << 2
#define RGB_16_TO_24BIT_R(RGB) ((RGB >> 11)) << 3

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    int32_t x;
    int32_t y;

    for (y = area->y1; y <= area->y2; y++)
    {
        LCD_IR(0);
        LCD_IR(ILI9325_VERTICAL_GRAM_ADDR_SET);
        LCD_WD((y >> 8) & 0xFF);
        LCD_WD(y & 0xFF);

        for (x = area->x1; x <= area->x2; x++)
        {
            LCD_IR(0);
            LCD_IR(ILI9325_HORIZONTAL_GRAM_ADDR_SET);
            LCD_WD((x >> 8) & 0xFF);
            LCD_WD(x & 0xFF);

            /* Prepare to write in GRAM */
            LCD_IR(0);
            LCD_IR(ILI9325_GRAM_DATA_REG); /* Write Data to GRAM (R22h) */

            LCD_WD(RGB_16_TO_24BIT_R(*(uint32_t *)color_p));
            LCD_WD(RGB_16_TO_24BIT_G(*(uint32_t *)color_p));
            LCD_WD(RGB_16_TO_24BIT_B(*(uint32_t *)color_p));

            color_p++;
        }
    }

    /* Reset the refresh window area */
    ili9325_set_window(0, 0, ILI9325_LCD_WIDTH, ILI9325_LCD_HEIGHT);

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
// static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                     const lv_area_t * fill_area, lv_color_t color)
//{
//     /*It's an example code which should be done by your GPU*/
//     int32_t x, y;
//     dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//     for(y = fill_area->y1; y <= fill_area->y2; y++) {
//         for(x = fill_area->x1; x <= fill_area->x2; x++) {
//             dest_buf[x] = color;
//         }
//         dest_buf+=dest_width;    /*Go to the next line*/
//     }
// }

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
