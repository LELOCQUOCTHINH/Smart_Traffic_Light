/*
 * fsm_auto.h
 *
 *  Created on: Dec 18, 2024
 *      Author: 84898
 */

#ifndef INC_FSM_AUTO_H_
#define INC_FSM_AUTO_H_

uint16_t get_static_red_timer();
uint16_t get_static_yellow_timer();
uint16_t get_static_green_timer();
void set_static_red_timer(uint16_t redtimer_for_set);
void set_static_yellow_timer(uint16_t yellowtimer_for_set);
void set_static_green_timer(uint16_t greentimer_for_set);

void fsm_auto_init();
void run_auto();
void auto_mode_display();

#endif /* INC_FSM_AUTO_H_ */
