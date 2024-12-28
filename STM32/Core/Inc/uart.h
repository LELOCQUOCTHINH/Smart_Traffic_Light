/*
 * uart.h
 *
 *  Created on: Dec 18, 2024
 *      Author: 84898
 */

#ifndef INC_UART_H_
#define INC_UART_H_

extern uint8_t CHOOSING_MODE_flag; //also cancel flag, switch to choose mode menu
extern uint8_t AUTO_MODE_flag; //indicator user want to switch to auto mode
extern uint8_t SETTING_MODE_flag; //indicator user want to switch to setting timer for leds
extern uint8_t CHANGING_RED_TIMER_flag; //indicator for user want to change timer at red led
extern uint8_t CHANGING_YELLOW_TIMER_flag; //indicator for user want to change timer at yellow led
extern uint8_t CHANGING_GREEN_TIMER_flag; //indicator for user want to change timer at Green Led
extern uint8_t SET_TIMER_flag; //indicator for user have tapped set timer for a LED
extern uint8_t MANUAL_MODE_flag; //indicator for user want to switch to manual mode
extern uint8_t RED_AT_ODD_flag; //indicator for RED at ODD road
extern uint8_t RED_AT_EVEN_flag; //indicator for RED at EVEN road
extern uint8_t HASH_flag; //indicator it has scanned number that user typed
extern uint16_t user_timer; //user timer

#define MAX_BUFFER_SIZE 7
extern uint8_t buffer_flag;
extern uint8_t buffer [MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t parser_index;

void parse_choose_mode(char c);
void parse_auto_mode(char c);
void parse_setting_mode(char c);
void parse_manual_mode(char c);
void parse_touch_RED_bar(char c);
void parse_touch_YELLOW_bar(char c);
void parse_touch_GREEN_bar(char c);
void parse_SET_timer(char c);
void parse_number(char c);
void parse_touch_REDOD_bar(char c);
void parse_touch_REDEV_bar(char c);

#endif /* INC_UART_H_ */
