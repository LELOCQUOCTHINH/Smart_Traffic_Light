/*
 * global.h
 *
 *  Created on: Oct 28, 2024
 *      Author: 84898
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define TICK 10
#define Led_OFF 00
#define YELLOW 01
#define GREEN 10
#define RED 11

#define true 1
#define false 0
typedef uint8_t bool;

void blink_led(); //blink red led at pin pa5 each second
void start_timerRun();
void timerRun(); //run timer and set flag if one second passes
bool IsOneSecondPassed(); //indicate one second passes
void clearOneSecondFlag(); //clear one second passed flag
void run_led_traffic(); //run led traffic at all road
void set_state_odd_road(uint8_t value_for_set); //set state of led traffic at odd road
void set_state_even_road(uint8_t value_for_set); //set state of led traffic at even road
uint8_t get_red_even_or_odd(); //indicate led red at even or odd road
void set_red_even_or_odd(uint8_t value_for_set); //set red led at even or odd road
void off_state(); //turn off led traffic at all road
char * get_red_timer_string();
char * get_green_timer_string();
char * get_yellow_timer_string();
void blink_traffic_light_following_desire(uint8_t color_u_want);
void display_choose_mode();

#include "fsm_auto.h"
#include "fsm_manual.h"
#include "fsm_setting.h"
#include "Scheduler.h"
#include "i2c-lcd.h"
#include "uart.h"

void fsm_main();
#endif /* INC_GLOBAL_H_ */
