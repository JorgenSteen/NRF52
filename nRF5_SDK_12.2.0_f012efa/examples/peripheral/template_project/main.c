/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
* @brief Example template project.
* @defgroup nrf_templates_example Example Template
*
*/


#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "nrf_delay.h"
#include "app_error.h"
#include "app_timer.h"
#include "app_util_platform.h"
#include "boards.h"
//#include "bsp.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "app_button.h"
#include "app_pwm.h"
#include "app_uart.h"
#include "nrf_temp.h"

// General application timer settings.
#define APP_TIMER_PRESCALER             15    // Value of the RTC1 PRESCALER register.
#define APP_TIMER_OP_QUEUE_SIZE         3     // Size of timer operation queues.

APP_TIMER_DEF(m_led_a_timer_id);
APP_PWM_INSTANCE(PWM1,1);

static volatile bool ready_flag;        // A flag indicating PWM status.

#if 0
#include "nrf_drv_timer.h"
const nrf_drv_timer_t MYTIMER = NRF_DRV_TIMER_INSTANCE(0);
void timer_event_handler(nrf_timer_event_t event_type, void* p_context)
{
}
#endif

// Function starting the internal LFCLK oscillator.
// This is needed by RTC1 which is used by the application timer
// (When SoftDevice is enabled the LFCLK is always running and this is not needed).
static void lfclk_request(void)
{
    uint32_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request(NULL);
}

// Timeout handler for the repeated timer
static void timer_a_handler(void * p_context)
{
    nrf_gpio_pin_toggle(LED_1);
}


// Create timers
static void create_timers()
{   
    uint32_t err_code;

    // Create timers
    err_code = app_timer_create(&m_led_a_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                timer_a_handler);
    APP_ERROR_CHECK(err_code);
	
}
// Start timer
static void start_timer()
{
		uint32_t err_code;
	
		// Start timer
		err_code = app_timer_start(m_led_a_timer_id,APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER), NULL);
		
		APP_ERROR_CHECK(err_code);
}

// Stop timer
static void stop_timer()
{
		uint32_t err_code;
	
		// Stop timer
		err_code = app_timer_stop(m_led_a_timer_id);
		
		APP_ERROR_CHECK (err_code);
	
}

static void button_handler(uint8_t pin_no,uint8_t button_action)
{
	if(pin_no == BUTTON_1 && button_action == APP_BUTTON_PUSH)
	{
		start_timer();
	}
	if(pin_no == BUTTON_2 && button_action == APP_BUTTON_PUSH)
	{
		stop_timer();
	}
	if(pin_no == BUTTON_3 && button_action == APP_BUTTON_PUSH)
	{
		while(app_pwm_channel_duty_set(&PWM1, 0, 5) == NRF_ERROR_BUSY);
	}
	if(pin_no == BUTTON_4 && button_action == APP_BUTTON_PUSH)
	{
		while(app_pwm_channel_duty_set(&PWM1, 0, 10) == NRF_ERROR_BUSY);
	}
}
static void button_init()
{
		uint32_t err_code;
	
    static app_button_cfg_t button_config[4];
    button_config[0].pin_no        = BUTTON_1 ;
    button_config[0].active_state  = APP_BUTTON_ACTIVE_LOW;
    button_config[0].pull_cfg      = NRF_GPIO_PIN_PULLUP; 
    button_config[0].button_handler = button_handler;

    button_config[1].pin_no        = BUTTON_2 ;
    button_config[1].active_state  = APP_BUTTON_ACTIVE_LOW;
    button_config[1].pull_cfg      = NRF_GPIO_PIN_PULLUP; 
    button_config[1].button_handler = button_handler;

	  button_config[2].pin_no        = BUTTON_3 ;
    button_config[2].active_state  = APP_BUTTON_ACTIVE_LOW;
    button_config[2].pull_cfg      = NRF_GPIO_PIN_PULLUP; 
    button_config[2].button_handler = button_handler;
	
	  button_config[3].pin_no        = BUTTON_4 ;
    button_config[3].active_state  = APP_BUTTON_ACTIVE_LOW;
    button_config[3].pull_cfg      = NRF_GPIO_PIN_PULLUP; 
    button_config[3].button_handler = button_handler;

		err_code = app_button_init(button_config, 4, 10);
		APP_ERROR_CHECK (err_code);
		
		err_code = app_button_enable();
		APP_ERROR_CHECK (err_code);
		
}

// PWM Ready call-back
void pwm_ready_callback(uint32_t pwm_id)
{
	ready_flag = true;
}	

static void pwm_init()
{
	app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(20000L, 4);
	
	pwm_config.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
	
	/* Initialize and enable PWM */
	uint32_t err_code;
	err_code = app_pwm_init(&PWM1,&pwm_config,pwm_ready_callback);
	APP_ERROR_CHECK (err_code);
	app_pwm_enable(&PWM1);
	
}
/**
 * @brief Function for application main entry.
 */

int main(void)
{
		//uint32_t err_code;
		
		// Set LED_1 pin as output
		nrf_gpio_cfg_output(LED_1);
		nrf_gpio_pin_set(LED_1);
	
		// Request LF clock.
    lfclk_request();
	
	  // Initialize the application timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
		
		// Create application timers.
		create_timers();
	
		//start_timer();
	
		button_init();
	
		pwm_init();
	
		
		// Led 1 blinks with 200 clicks interval
		//err_code = app_timer_start(m_led_a_timer_id, APP_TIMER_TICKS(200, APP_TIMER_PRESCALER), NULL);
    //APP_ERROR_CHECK(err_code);	
	
		while (true)
    {
		
		   // Do nothing.
    }
}
/** @} */
