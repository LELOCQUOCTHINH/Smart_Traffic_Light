/*
 * global.c
 *
 *  Created on: Oct 28, 2024
 *      Author: 84898
 */
#include "global.h"

#define TIME_BLINK 1000

int counter_blinkLed = TIME_BLINK/TICK;
uint8_t state_even_road = Led_OFF;
uint8_t state_odd_road = Led_OFF;
uint8_t red_even_or_odd = 0;// 1 for red at even road, 0 for red at odd road

char red_timer_string [] = "RED=";
char green_timer_string [] = "GREEN=";
char yellow_timer_string [] = "AMBER=";

void blink_led()
{
	counter_blinkLed--;

	if(counter_blinkLed <= 0)
	{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		counter_blinkLed = TIME_BLINK/TICK;
	}
}

int counterOneSecond = 1000/TICK;
bool OneSecondPass = 0;

void start_timerRun()
{
	counterOneSecond = 1000/TICK;
	OneSecondPass = 0;
}

void timerRun()
{
	counterOneSecond--;
	if(counterOneSecond <= 0)
	{
		OneSecondPass = 1;
		counterOneSecond = 1000/TICK;
	}
}

bool IsOneSecondPassed()
{
	return OneSecondPass;
}

uint8_t current_state = Led_OFF;
void blink_traffic_light_following_desire(uint8_t color_u_want) //Green or Red or Amber
//period is 1s
{
	if(!current_state) //current state = 0 = Led_OFF
	{
		off_state();
		if(IsOneSecondPassed())
		{
			clearOneSecondFlag();
			current_state = color_u_want;
		}
	}
	if(current_state) //current state != 0
	{
		set_state_odd_road(color_u_want);
		set_state_even_road(color_u_want);
		if(IsOneSecondPassed())
		{
			clearOneSecondFlag();
			current_state = Led_OFF;
		}
	}

	run_led_traffic();
}

void clearOneSecondFlag()
{
	OneSecondPass = 0;
}

void run_led_traffic()
{
	HAL_GPIO_WritePin(MSB_Even_GPIO_Port, MSB_Even_Pin, state_even_road / 10); //00 is 0, 10  is 1, 11 is 1
	HAL_GPIO_WritePin(LSB_Even_GPIO_Port, LSB_Even_Pin, state_even_road % 10); //00 is 0, 10 is 0, 11 is 1

	HAL_GPIO_WritePin(MSB_Odd_GPIO_Port, MSB_Odd_Pin, state_odd_road / 10); //00 is 0, 10  is 1, 11 is 1
	HAL_GPIO_WritePin(LSB_Odd_GPIO_Port, LSB_Odd_Pin, state_odd_road % 10); //00 is 0, 10 is 0, 11 is 1'
}
void set_state_odd_road(uint8_t value_for_set)
{
	state_odd_road = value_for_set;
}
void set_state_even_road(uint8_t value_for_set)
{
	state_even_road = value_for_set;
}

uint8_t get_red_even_or_odd()
{
	return red_even_or_odd;
}
void set_red_even_or_odd(uint8_t value_for_set)
{
	red_even_or_odd = value_for_set;
}

void off_state()
{
	state_even_road = Led_OFF;
	state_odd_road = Led_OFF;
}

char * get_red_timer_string()
{
	return red_timer_string;
}
char * get_green_timer_string()
{
	return green_timer_string;
}
char * get_yellow_timer_string()
{
	return yellow_timer_string;
}

void display_choose_mode()
{
	lcd_clear_display();
	lcd_goto_XY(0, 0);
	lcd_send_string("please choose a");
	lcd_goto_XY(1, 0);
	lcd_send_string("mode that u want");
}

typedef enum{
	CHOOSE_MODE,
	AUTO_MODE,
	SETTING_MODE,
	MANUAL_MODE
}Control_mode;

Control_mode control_mode = CHOOSE_MODE;

void fsm_main()
{
	switch (control_mode)
	{
	case CHOOSE_MODE:
		blink_traffic_light_following_desire(YELLOW); //blink yellow led every second
		parse_auto_mode(buffer[parser_index]);
		parse_setting_mode(buffer[parser_index]);
		parse_manual_mode(buffer[parser_index]);
		if(AUTO_MODE_flag)
		{
			start_timerRun();//START TIMER FROM ZERO
			fsm_auto_init();
			control_mode = AUTO_MODE;
			AUTO_MODE_flag = 0;
			auto_mode_display();
		}
		else if(SETTING_MODE_flag)
		{
			start_timerRun();//START TIMER FROM ZERO
			control_mode = SETTING_MODE;
			SETTING_MODE_flag = 0;
			display_setting();
		}
		else if(MANUAL_MODE_flag)
		{
			control_mode = MANUAL_MODE;
			MANUAL_MODE_flag = 0;
			display_manual();
		}
		break;
	case AUTO_MODE:
		run_auto();
		break;
	case SETTING_MODE:
		run_setting();
		break;
	case MANUAL_MODE:
		run_manual();
		break;
	}

	parse_choose_mode(buffer[parser_index]);

	if(CHOOSING_MODE_flag)
	{
		if(control_mode != CHOOSE_MODE)
			display_choose_mode();

		if(control_mode == SETTING_MODE)
		{
			validate_timer();
			reset_fsm_setting();
		}
		control_mode = CHOOSE_MODE;
		CHOOSING_MODE_flag = 0;
	}

	if(++parser_index >= 7)
		parser_index = 0;
}
