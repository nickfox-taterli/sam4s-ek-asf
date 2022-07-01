/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

static void event_handler(rtouch_event_t const *event);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t *indev_touchpad;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
  /**
   * Here you will find example implementation of input devices supported by LittelvGL:
   *  - Touchpad
   *  - Mouse (with cursor support)
   *  - Keypad (supports GUI usage only with key)
   *  - Encoder (supports GUI usage only with: left, right, push)
   *  - Button (external buttons to press points on the screen)
   *
   *  The `..._read()` function are only examples.
   *  You should shape them according to your hardware
   */

  static lv_indev_drv_t indev_drv;

  /*------------------
   * Touchpad
   * -----------------*/

  /*Initialize your touchpad if you have*/
  touchpad_init();

  /*Register a touchpad input device*/
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  indev_touchpad = lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/
static rtouch_event_t rtouch_event;

/*Initialize your touchpad*/
static void touchpad_init(void)
{
  /* Initializes the PIO interrupt management for touchscreen driver */
  pio_handler_set_priority(PIOA, PIOA_IRQn, 0);
  pio_handler_set_priority(PIOB, PIOB_IRQn, 0);
  pio_handler_set_priority(PIOC, PIOC_IRQn, 0);

  /* Initialize touchscreen without calibration */
  rtouch_init(ILI9325_LCD_WIDTH, ILI9325_LCD_HEIGHT);
  rtouch_enable();
  rtouch_set_event_handler(event_handler);
  rtouch_set_calibration_parameter(7605, -5368, 160, 1850);
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  UNUSED(indev_drv);
  /*Save the pressed coordinates and the state*/
  switch (rtouch_event.type)
  {
  case RTOUCH_MOVE:
  case RTOUCH_PRESS:
    data->state = LV_INDEV_STATE_PR;
    break;
  case RTOUCH_RELEASE:
    data->state = LV_INDEV_STATE_REL;
    break;
  default:
    break;
  }

  /*Set the last pressed coordinates*/
  data->point.x = rtouch_event.panel.x;
  data->point.y = rtouch_event.panel.y;
}

/**
 * \brief Callback called when the pen is pressed on the touchscreen.
 *
 * \param x horizontal position (in pixel if ts calibrated).
 * \param y vertical position (in pixel if ts calibrated).
 */
static void event_handler(rtouch_event_t const *event)
{
  rtouch_event.panel.x = event->panel.x;
  rtouch_event.panel.y = event->panel.y;
  rtouch_event.type = event->type;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
