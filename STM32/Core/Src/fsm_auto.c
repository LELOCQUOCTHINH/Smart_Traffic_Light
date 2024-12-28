/*
 * fsm_auto.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 84898
 */
#include "global.h"

int16_t red_timer = 0;
int16_t yellow_timer = 0;
int16_t green_timer = 0;

uint16_t static_red_timer = 0;
uint16_t static_yellow_timer = 0;
uint16_t static_green_timer = 0;

uint16_t get_static_red_timer()
{
	return static_red_timer;
}

uint16_t get_static_yellow_timer()
{
	return static_yellow_timer;
}

uint16_t get_static_green_timer()
{
	return static_green_timer;
}

void set_static_red_timer(uint16_t redtimer_for_set)
{
	static_red_timer = redtimer_for_set;
}

void set_static_yellow_timer(uint16_t yellowtimer_for_set)
{
	static_yellow_timer = yellowtimer_for_set;
}

void set_static_green_timer(uint16_t greentimer_for_set)
{
	static_green_timer = greentimer_for_set;
}

void fsm_auto_init()
{
	green_timer = static_green_timer;
	red_timer = static_red_timer;
	yellow_timer = static_yellow_timer;
}

void decrease_led_timer_at_each_road() //just decrease if one second passed
{
	if(green_timer <= 0)
		yellow_timer--;
	else
		green_timer --;

	red_timer--;
}

void fsm_auto()
{
	switch (get_red_even_or_odd())
	{
	case 1: //red at even road
		set_state_even_road(RED); //even road switch to red led
		if(green_timer <= 0)
			set_state_odd_road(YELLOW); //switch odd road to yellow
		else
			set_state_odd_road(GREEN); //odd road switch to green led

		if(red_timer <= 0)
		{
			set_red_even_or_odd(0); //switch red led at odd road
			fsm_auto_init(); //reset timer for each led
		}
		break;
	case 0:
		set_state_odd_road(RED); //odd road switch to red led
		if(green_timer <= 0) //when green = 0
			set_state_even_road(YELLOW); //switch even road to yellow
		else
			set_state_even_road(GREEN); //even road switch to green led

		if(red_timer <= 0)
		{
			set_red_even_or_odd(1); //switch red led at even road
			fsm_auto_init(); //reset timer for each led
		}
		break;
	}
}

char auto_mode_string [] = "Run auto mode...";
char value_of_timer [4];
void auto_mode_display()
{
	lcd_clear_display(); //clear monitor of lcd

	lcd_goto_XY (0, 0); //point to (0;0)
	lcd_send_string (auto_mode_string);  // display string "run auto mode.." to lcd

	lcd_goto_XY(1, 0); //point to new line on lcd
	lcd_send_string(get_red_timer_string()); //display string "RED="

	lcd_goto_XY(1, strlen(get_red_timer_string())); //point to afterward RED= to display timer of red led
	sprintf(value_of_timer, "%d", red_timer); //convert red timer to string
	lcd_send_string(value_of_timer); //send timer of red led

	lcd_goto_XY(1, strlen(get_red_timer_string()) + strlen(value_of_timer)); //point to afterward timer of red led
	if(green_timer <= 0) //yellow mod
		lcd_send_string(get_yellow_timer_string()); //display string "AMBER="
	else //green
		lcd_send_string(get_green_timer_string()); //display string "GREEN="

	lcd_goto_XY(1, strlen(get_red_timer_string()) + strlen(value_of_timer) + strlen(get_green_timer_string()));
	//point to afterward "AMBER=" or "GREEN="
	if(green_timer <= 0)
	{
		sprintf(value_of_timer, "%d", yellow_timer); //convert yellow timer to string
		lcd_send_string(value_of_timer); //send timer of yellow led
	}
	else
	{
		sprintf(value_of_timer, "%d", green_timer); //convert green timer to string
		lcd_send_string(value_of_timer); //send timer of green led
	}
}
void run_auto()
{
	if(static_red_timer > 0)
		fsm_auto();
	run_led_traffic();
	if(IsOneSecondPassed())
	{
		decrease_led_timer_at_each_road();
		clearOneSecondFlag();
		if(static_red_timer > 0) //unsigned int so very much therefore it shouldn't be transmitted to lcd
			auto_mode_display();
		else //static_red_timer <= 0
			fsm_auto();
	}
}
