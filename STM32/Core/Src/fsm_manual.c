/*
 * fsm_manual.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 84898
 */

#include "global.h"

void fsm_manual()
{
	switch(get_red_even_or_odd())
	{
	case 0: //red at odd road
		set_state_even_road(GREEN); //set to display
		set_state_odd_road(RED);
		break;
	case 1: //red at even road
		set_state_even_road(RED);
		set_state_odd_road(GREEN);
		break;
	}
}

char manual_mode_string [] = "Run manual mode...";
char red_odd [] = "red at odd road";
char red_even [] ="red at even road";
void display_manual()
{
	lcd_clear_display(); //clear monitor

	lcd_goto_XY (0, 0); //point to (0;0)
	lcd_send_string (manual_mode_string);  // display string "run manual mode.." to lcd

	lcd_goto_XY(1, 0); //point to new line on lcd
	lcd_send_string(get_red_even_or_odd() ? red_even : red_odd); //display string red odd or even
}

void run_manual()
{
	if(get_red_even_or_odd()) //red at even road
	{
		parse_touch_REDOD_bar(buffer [parser_index]);
		if(RED_AT_ODD_flag)
		{
			RED_AT_ODD_flag = 0;
			set_red_even_or_odd(0); //switch to red at odd road
			display_manual(); //display state to lcd
		}
	}
	if(!get_red_even_or_odd()) //red at odd road
	{
		parse_touch_REDEV_bar(buffer [parser_index]);
		if(RED_AT_EVEN_flag)
		{
			RED_AT_EVEN_flag = 0;
			set_red_even_or_odd(1); //switch to red at even road
			display_manual(); //display state to lcd
		}
	}
	run_led_traffic(); //turn on led traffic at all roads
	fsm_manual(); //control led traffic
}
