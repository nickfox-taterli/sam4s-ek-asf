/*****************************************************************************
 *
 * \file
 *
 * \brief AVR32 UC3 Control Panel potentiometer sensor module.
 *
 * - Supported boards:   EVK1100
 *
 *
 * Copyright (c) 2009-2018 Microchip Technology Inc. and its subsidiaries.
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
 *****************************************************************************/
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

//_____  I N C L U D E S ___________________________________________________
#include <string.h>

#include "compiler.h"
#include "gpio.h" // Have to include gpio.h before FreeRTOS.h as long as FreeRTOS
                  // redefines the inline keyword to empty.
#include "FreeRTOS.h"

#include "potentiometer.h"

#include "datalog.h"
#include "shell.h"
#include "board.h"
#include "adc.h"
#include "sensor.h"
#include "config_file.h"
#include "fsaccess.h"

#include "BasicSMTP.h"

//_____ M A C R O S ________________________________________________________

/*! Max string length of a get config. */
#define POT_GETCONF_MAXLEN   64

/*! Path for the config file */
#define SENSOR_POT_CONFIG_FILE   "A:/CFG/POT.TXT"

//_____ D E F I N I T I O N S ______________________________________________
/*! Value for the log rate. */
unsigned int ul_pot_lograte = 5;

/*! Min value for alarm. */
static unsigned int ul_pot_min = 0;

/*! Max Value for alarm. */
static unsigned int ul_pot_max = 100;

/*! Alarm upon event. */
static bool b_pot_alarm = false;
/*! Alarm for max is pending ? */
static bool b_pot_alarm_max = pdFALSE;
/*! Alarm for min is pending ? */
static bool b_pot_alarm_min = pdFALSE;

/*! ADC cell. */
static volatile avr32_adc_t * adc= (volatile avr32_adc_t *) &AVR32_ADC;

/*! The CFG system mutex. */
extern xSemaphoreHandle   xCFGMutex;

//_____ D E C L A R A T I O N S ____________________________________________
extern int sprintf(char *out, const char *format, ...);  // From printf-stdarg.c



/*!
 *  \brief Init the potentiometer channel.
 *
 *  \return true upon success, false if error.
 */
bool b_potentiometer_init ( void )
{
portCHAR token[6];
portCHAR * percent;

   // Get the xCFGMutex.
   if( pdTRUE == x_supervisor_SemaphoreTake( xCFGMutex, 0 ) )
   {
       // get the field
       if (config_file_get_value(SENSOR_POT_CONFIG_FILE, "alarm" , token) >= 0)
       {
         // update value
         if (!strcmp(token, "on"))
         {
           b_pot_alarm = pdTRUE;
         }
       }
       if (config_file_get_value(SENSOR_POT_CONFIG_FILE, "min" , token) >= 0)
       {
         percent = strpbrk(token , "%");
         if (percent != NULL)
         {
           *percent = '\0';
         }
         sscanf(token, "%u", &ul_pot_min);
       }
       if (config_file_get_value(SENSOR_POT_CONFIG_FILE, "max" , token) >= 0)
       {
         percent = strpbrk(token , "%");
         if (percent != NULL)
         {
           *percent = '\0';
         }
         sscanf(token, "%u", &ul_pot_max);
       }
       if (config_file_get_value(SENSOR_POT_CONFIG_FILE, "lograte" , token) >= 0)
       {
         sscanf(token, "%u", &ul_pot_lograte);
       }
     // Release the xCFGMutex.
     x_supervisor_SemaphoreGive( xCFGMutex );
   }
   /* enable pin for sensor */
   gpio_enable_module_pin( ADC_POTENTIOMETER_PIN , ADC_POTENTIOMETER_FUNCTION );

   return (true);
}

/*!
 *  \brief Get the potentiometer sensor config.
 *
 *  \param ppcStringReply Input/Output. The response string. NEVER NULL AS INPUT.
 *                        A malloc for the response string is performed here;
 *                        the caller must free this string.
 *
 *  \return the status of the command execution.
 */
eExecStatus e_potentiometer_get_config( signed portCHAR **ppcStringReply )
{
   // Alloc space for the reply.
   *ppcStringReply = (signed portCHAR *)pvPortMalloc( POT_GETCONF_MAXLEN );
   if( NULL == *ppcStringReply )
   {
      *ppcStringReply = (signed portCHAR *)SHELL_ERRMSG_MEMALLOC;
      return( SHELL_EXECSTATUS_KO );
   }

   // Build the string.
   sprintf( (char *)*ppcStringReply, "lograte=%d\r\n""min=%d%%\r\n""max=%d%%\r\n""alarm=%s\r\n",
            ul_pot_lograte, ul_pot_min, ul_pot_max, ((b_pot_alarm == pdTRUE) ? "on" : "off"));

   return( SHELL_EXECSTATUS_OK );
}

/*!
 *  \brief Set the sensor config.
 *
 *  \param ppcStringReply Input/Output. The response string. NEVER NULL AS INPUT.
 *  \param ac             Input. Number of args
 *  \param av             Input. pointer to args
 *
 *  \return the status of the command execution.
 */
eExecStatus e_potentiometer_set_config( signed portCHAR **ppcStringReply, int ac, signed portCHAR *av[] )
{
portCHAR * token;

  if (config_file_set_value(SENSOR_POT_CONFIG_FILE, ac, av) != 0)
  {
    *ppcStringReply = (signed portCHAR *)SHELL_ERRMSG_CONFIGERROR;
    // return error
    return( SHELL_EXECSTATUS_KO );
  }
  if (!strcmp((char *)av[0] , "alarm"))
  {
    if (!strcmp((char *)av[1] , "on"))
    {
      b_pot_alarm = pdTRUE;
      *ppcStringReply = (signed portCHAR *)SENSOR_MSG_ALARM_ON;
      return( SHELL_EXECSTATUS_OK_NO_FREE );
    }
    else if (!strcmp( (char *)av[1], "off"))
    {
      b_pot_alarm = pdFALSE;
      *ppcStringReply = (signed portCHAR *)SENSOR_MSG_ALARM_OFF;
      return( SHELL_EXECSTATUS_OK_NO_FREE );
    }
    else
    {
      *ppcStringReply = (signed portCHAR *)SHELL_ERRMSG_CONFIGERROR;
      return( SHELL_EXECSTATUS_KO );
    }
  }
  else if (!strcmp((char *)av[0] , "lograte"))
  {
    ul_pot_lograte = atoi((char *)av[1]);
    *ppcStringReply = (signed portCHAR *)SENSOR_MSG_CONFIG_SET;
    return( SHELL_EXECSTATUS_OK_NO_FREE );
  }
  else if (!strcmp((char *)av[0] , "min"))
  {
    token = strpbrk((char *)av[1] , "%");
    if (token != NULL)
    {
      *token = '\0';
    }
    ul_pot_min = atoi((char *)av[1]);
    *ppcStringReply = (signed portCHAR *)SENSOR_MSG_CONFIG_SET;
    return( SHELL_EXECSTATUS_OK_NO_FREE );
  }
  else if (!strcmp((char *)av[0] , "max"))
  {
    token = strpbrk((char *)av[1] , "%");
    if (token != NULL)
    {
      *token = '\0';
    }
    ul_pot_max = atoi((char *)av[1]);
    *ppcStringReply = (signed portCHAR *)SENSOR_MSG_CONFIG_SET;
    return( SHELL_EXECSTATUS_OK_NO_FREE );
  }
  else

  {
    *ppcStringReply = (signed portCHAR *)SHELL_ERRMSG_CONFIGERROR;
    return( SHELL_EXECSTATUS_KO );
  }
}

/*!
 *  \brief Get the current potentiometer value.
 *
 *  \param pxLog a Log structure.
 *
 *  \return true upon success, false if error.
 */
bool b_potentiometer_get_value( xLogDef *pxLog )
{
   int i_current_val;


   /* enable channel for sensor */
   adc_enable( adc, ADC_POTENTIOMETER_CHANNEL );
   /* start conversion */
   adc_start( adc );
   /* get value for sensor */
   i_current_val = adc_get_value( adc, ADC_POTENTIOMETER_CHANNEL ) * 100 / ADC_MAX_VALUE;
   /* Disable channel for sensor */
   adc_disable( adc, ADC_POTENTIOMETER_CHANNEL );

   // Alloc memory for the log string.
   pxLog->pcStringLog = pvPortMalloc( 16*sizeof( char ) );
   if( NULL == pxLog->pcStringLog )
   {
      return( false );
   }
   pxLog->pfFreeStringLog = vPortFree; // Because pvPortMalloc() was used to
                                       // alloc the log string.

   // Build the log string.
   if( i_current_val <= ul_pot_min )
   {
      sprintf( pxLog->pcStringLog, "%3d%% | min", i_current_val );
      // if alarms have to be checked and no alarm for min was pending
      if (( b_pot_alarm == pdTRUE ) && ( b_pot_alarm_min == pdFALSE ))
      {
        // alarm has been taken into account,
        // don't reenter this test before leaving min area
        b_pot_alarm_min = pdTRUE;
        // allow alarm if max is reached
        b_pot_alarm_max = pdFALSE;
        // post alarm to SMTP task
        v_SMTP_Post("Min Potentiometer Alarm", NULL);
      }
   }
   else if( i_current_val >= ul_pot_max )
   {
      sprintf( pxLog->pcStringLog, "%3d%% | max", i_current_val );
      // if alarms have to be checked and no alarm for max was pending
      if (( b_pot_alarm == pdTRUE ) && ( b_pot_alarm_max == pdFALSE ))
      {
        // alarm has been taken into account,
        // don't reenter this test before leaving max area
        b_pot_alarm_max = pdTRUE;
        // allow alarm if min is reached
        b_pot_alarm_min = pdFALSE;
        // post alarm to SMTP task
        v_SMTP_Post("Max Potentiometer Alarm", NULL);
      }
   }
   else
   {
      sprintf( pxLog->pcStringLog, "%3d%%", i_current_val );
      // if alarms have to be checked
      if ( b_pot_alarm == pdTRUE )
      {
        // no alarm is pending
        b_pot_alarm_max = pdFALSE;
        b_pot_alarm_min = pdFALSE;
      }
   }

   return( true );
}
