/*
 * fsm_setting.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 84898
 */

#include "global.h"

typedef enum {
WATING_CHOOSES_LED,
RED_LED,
YELLOW_LED,
GREEN_LED
}State_of_setting;

State_of_setting state_of_setting = WATING_CHOOSES_LED;

void fsm_setting()
{
	switch(state_of_setting)
	{
	case WATING_CHOOSES_LED:
		blink_traffic_light_following_desire(Led_OFF);
		parse_touch_RED_bar(buffer[parser_index]);
		parse_touch_YELLOW_bar(buffer[parser_index]);
		parse_touch_GREEN_bar(buffer[parser_index]);
		if(CHANGING_RED_TIMER_flag)
		{
			state_of_setting = RED_LED;
			CHANGING_RED_TIMER_flag = 0;
			display_setting();
//			memset(buffer, '0', MAX_BUFFER_SIZE);
		}
		else if(CHANGING_YELLOW_TIMER_flag)
		{
			state_of_setting = YELLOW_LED;
			CHANGING_YELLOW_TIMER_flag = 0;
			display_setting();
//			memset(buffer, '0', MAX_BUFFER_SIZE);
		}
		else if(CHANGING_GREEN_TIMER_flag)
		{
			state_of_setting = GREEN_LED;
			CHANGING_GREEN_TIMER_flag = 0;
			display_setting();
//			memset(buffer, '0', MAX_BUFFER_SIZE);
		}

//		SET_TIMER_flag = 0; //reset set timer flag because overlap "SET" signal with "SETTI" signal
		break;
	case RED_LED:
		blink_traffic_light_following_desire(RED);
		parse_SET_timer(buffer[parser_index]);
		if(SET_TIMER_flag)
		{
			parse_number(buffer[parser_index]);
			if(HASH_flag)
			{
				SET_TIMER_flag = 0;
				HASH_flag = 0;
				set_static_red_timer(user_timer); //static red timer = user timer
				state_of_setting = WATING_CHOOSES_LED;
				display_setting();
			}
		}
		else
		{
			parse_touch_YELLOW_bar(buffer[parser_index]);
			parse_touch_GREEN_bar(buffer[parser_index]);
			if(CHANGING_YELLOW_TIMER_flag)
			{
				state_of_setting = YELLOW_LED;
				CHANGING_YELLOW_TIMER_flag = 0;
				display_setting();
			}
			if(CHANGING_GREEN_TIMER_flag)
			{
				state_of_setting = GREEN_LED;
				CHANGING_GREEN_TIMER_flag = 0;
				display_setting();
			}
		}
		break;
	case YELLOW_LED:
		blink_traffic_light_following_desire(YELLOW);
		parse_SET_timer(buffer[parser_index]);
		if(SET_TIMER_flag)
		{
			parse_number(buffer[parser_index]);
			if(HASH_flag)
			{
				SET_TIMER_flag = 0;
				HASH_flag = 0;
				set_static_yellow_timer(user_timer); //static YELLOW timer = user timer
				state_of_setting = WATING_CHOOSES_LED;
				display_setting();
			}
		}
		else
		{
			parse_touch_RED_bar(buffer[parser_index]);
			parse_touch_GREEN_bar(buffer[parser_index]);
			if(CHANGING_RED_TIMER_flag)
			{
				state_of_setting = RED_LED;
				CHANGING_RED_TIMER_flag = 0;
				display_setting();
			}
			if(CHANGING_GREEN_TIMER_flag)
			{
				state_of_setting = GREEN_LED;
				CHANGING_GREEN_TIMER_flag = 0;
				display_setting();
			}
		}
		break;
	case GREEN_LED:
		blink_traffic_light_following_desire(GREEN);
		parse_SET_timer(buffer[parser_index]);
		if(SET_TIMER_flag)
		{
			parse_number(buffer[parser_index]);
			if(HASH_flag)
			{
				SET_TIMER_flag = 0;
				HASH_flag = 0;
				set_static_green_timer(user_timer); //static red timer = user timer
				state_of_setting = WATING_CHOOSES_LED;
				display_setting();
			}
		}
		else
		{
			parse_touch_YELLOW_bar(buffer[parser_index]);
			parse_touch_RED_bar(buffer[parser_index]);
			if(CHANGING_YELLOW_TIMER_flag)
			{
				state_of_setting = YELLOW_LED;
				CHANGING_YELLOW_TIMER_flag = 0;
				display_setting();
			}
			if(CHANGING_RED_TIMER_flag)
			{
				state_of_setting = RED_LED;
				CHANGING_RED_TIMER_flag = 0;
				display_setting();
			}
		}
		break;
	}

	parse_setting_mode(buffer[parser_index]);

	if(state_of_setting != WATING_CHOOSES_LED && SETTING_MODE_flag)
	{
		state_of_setting = WATING_CHOOSES_LED;
		SETTING_MODE_flag = 0;
		SET_TIMER_flag = 0; //reset set timer flag because overlap "SET" signal with "SETTI" signal
		display_setting();
	}
}

void reset_fsm_setting()
{
	state_of_setting = WATING_CHOOSES_LED;
}

void validate_timer()
{
	if(get_static_red_timer() != get_static_green_timer() + get_static_yellow_timer())
	{
		set_static_red_timer(0);
		set_static_green_timer(0);
		set_static_yellow_timer(0);
	}
}

void display_setting()
{
	lcd_clear_display();
	switch (state_of_setting)
	{
	case WATING_CHOOSES_LED:
		lcd_goto_XY(0, 0);
		lcd_send_string("Run setting mode...");
		lcd_goto_XY(1, 0);
		lcd_send_string("Choice color...");
		break;
	case RED_LED:
		lcd_goto_XY(0, 0);
		lcd_send_string("typing timer for");
		lcd_goto_XY(1, 0);
		lcd_send_string("RED LED...");
		break;
	case YELLOW_LED:
		lcd_goto_XY(0, 0);
		lcd_send_string("typing timer for");
		lcd_goto_XY(1, 0);
		lcd_send_string("YELLOW LED...");
		break;
	case GREEN_LED:
		lcd_goto_XY(0, 0);
		lcd_send_string("typing timer for");
		lcd_goto_XY(1, 0);
		lcd_send_string("GREEN LED...");
		break;
	}
}

void run_setting()
{
	fsm_setting();
}
