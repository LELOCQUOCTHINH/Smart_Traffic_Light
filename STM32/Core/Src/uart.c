/*
 * uart.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 84898
 */
#include "global.h"

uint8_t CHOOSING_MODE_flag = 0; //also cancel flag
uint8_t AUTO_MODE_flag = 0;
uint8_t SETTING_MODE_flag = 0;
uint8_t CHANGING_RED_TIMER_flag = 0;
uint8_t CHANGING_YELLOW_TIMER_flag = 0;
uint8_t CHANGING_GREEN_TIMER_flag = 0;
uint8_t SET_TIMER_flag = 0;
uint8_t MANUAL_MODE_flag = 0;
uint8_t RED_AT_ODD_flag = 0;
uint8_t RED_AT_EVEN_flag = 0;

uint8_t buffer [MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t parser_index = 0;

//#define CHOOSE_MODE "CHMOD"
// Define the states of the FSM
typedef enum {
    WAITING_FOR_CHOOSE_C,
    GOT_CHOOSE_C,
    GOT_CHOOSE_H,
    GOT_CHOOSE_M,
    GOT_CHOOSE_O,
} Choose_mode_parser;

Choose_mode_parser choose_mode_parser = WAITING_FOR_CHOOSE_C;

void reset_parsing_choose_mode() {
    choose_mode_parser = WAITING_FOR_CHOOSE_C;
}

void parse_choose_mode(char c) {
    switch (choose_mode_parser) {
        case WAITING_FOR_CHOOSE_C:
            if (c == 'C') {
                choose_mode_parser = GOT_CHOOSE_C;
            } else {
                reset_parsing_choose_mode();  // Reset on unexpected character
            }
            break;
        case GOT_CHOOSE_C:
            if (c == 'H') {
                choose_mode_parser = GOT_CHOOSE_H;
            } else {
                reset_parsing_choose_mode();  // Reset on unexpected character
            }
            break;
        case GOT_CHOOSE_H:
            if (c == 'M') {
                choose_mode_parser = GOT_CHOOSE_M;
            } else {
                reset_parsing_choose_mode();  // Reset on unexpected character
            }
            break;
        case GOT_CHOOSE_M:
            if (c == 'O') {
                choose_mode_parser = GOT_CHOOSE_O;
            } else {
                reset_parsing_choose_mode();  // Reset on unexpected character
            }
            break;
        case GOT_CHOOSE_O:
            if (c == 'D') {
                CHOOSING_MODE_flag = 1;
            }
            reset_parsing_choose_mode();
            break;
    }
}

typedef enum {
    WAITING_FOR_AUTO_A,
    GOT_AUTO_A,
    GOT_AUTO_U,
    GOT_AUTO_T
} Auto_mode_parser;

Auto_mode_parser auto_mode_parser = WAITING_FOR_AUTO_A;

void reset_parsing_auto_mode() {
    auto_mode_parser = WAITING_FOR_AUTO_A;
}

void parse_auto_mode(char c) {
    switch (auto_mode_parser) {
        case WAITING_FOR_AUTO_A:
            if (c == 'A') {
                auto_mode_parser = GOT_AUTO_A;
            } else {
                reset_parsing_auto_mode();  // Reset on unexpected character
            }
            break;
        case GOT_AUTO_A:
            if (c == 'U') {
                auto_mode_parser = GOT_AUTO_U;
            } else {
                reset_parsing_auto_mode();  // Reset on unexpected character
            }
            break;
        case GOT_AUTO_U:
            if (c == 'T') {
                auto_mode_parser = GOT_AUTO_T;
            } else {
                reset_parsing_auto_mode();  // Reset on unexpected character
            }
            break;
        case GOT_AUTO_T:
            if (c == 'O') {
                AUTO_MODE_flag = 1;
            }
            reset_parsing_auto_mode();
            break;
    }
}

typedef enum {
    WAITING_FOR_SETTING_S,
    GOT_SETTING_S,
    GOT_SETTING_E,
    GOT_SETTING_T1,
    GOT_SETTING_T2
} Setting_mode_parser;

Setting_mode_parser setting_mode_parser = WAITING_FOR_SETTING_S;

void reset_parsing_setting_mode() {
    setting_mode_parser = WAITING_FOR_SETTING_S;
}

void parse_setting_mode(char c) {
    switch (setting_mode_parser) {
        case WAITING_FOR_SETTING_S:
            if (c == 'S') {
                setting_mode_parser = GOT_SETTING_S;
            } else {
                reset_parsing_setting_mode();  // Reset on unexpected character
            }
            break;
        case GOT_SETTING_S:
            if (c == 'E') {
                setting_mode_parser = GOT_SETTING_E;
            } else {
                reset_parsing_setting_mode();  // Reset on unexpected character
            }
            break;
        case GOT_SETTING_E:
            if (c == 'T') {
                setting_mode_parser = GOT_SETTING_T1;
            } else {
                reset_parsing_setting_mode();  // Reset on unexpected character
            }
            break;
        case GOT_SETTING_T1:
            if (c == 'T') {
                setting_mode_parser = GOT_SETTING_T2;
            } else {
                reset_parsing_setting_mode();  // Reset on unexpected character
            }
            break;
        case GOT_SETTING_T2:
            if (c == 'I') {
                SETTING_MODE_flag = 1;
            }
            reset_parsing_setting_mode();
            break;
    }
}

typedef enum {
    WAITING_FOR_MANUAL_M,
    GOT_MANUAL_M,
    GOT_MANUAL_A
} Manual_mode_parser;

Manual_mode_parser manual_mode_parser = WAITING_FOR_MANUAL_M;

void reset_parsing_manual_mode() {
    manual_mode_parser = WAITING_FOR_MANUAL_M;
}

void parse_manual_mode(char c) { // parses phrase "MAN"
    switch (manual_mode_parser) {
        case WAITING_FOR_MANUAL_M:
            if (c == 'M') {
                manual_mode_parser = GOT_MANUAL_M;
            } else {
                reset_parsing_manual_mode();  // Reset on unexpected character
            }
            break;
        case GOT_MANUAL_M:
            if (c == 'A') {
                manual_mode_parser = GOT_MANUAL_A;
            } else {
                reset_parsing_manual_mode();  // Reset on unexpected character
            }
            break;
        case GOT_MANUAL_A:
            if (c == 'N') {
                MANUAL_MODE_flag = 1;
            }
            reset_parsing_manual_mode();  // Reset on unexpected character
            break;
    }
}

typedef enum {
    WAITING_FOR_RED_R,
    GOT_RED_R,
    GOT_RED_E
} Touch_RED_bar_parser;

Touch_RED_bar_parser touch_RED_bar_parser = WAITING_FOR_RED_R;

void reset_parsing_touch_red_bar() {
    touch_RED_bar_parser = WAITING_FOR_RED_R;
}

void parse_touch_RED_bar(char c) { // parses phrase "RED"
    switch (touch_RED_bar_parser) {
        case WAITING_FOR_RED_R:
            if (c == 'R') {
                touch_RED_bar_parser = GOT_RED_R;
            } else {
                reset_parsing_touch_red_bar();  // Reset on unexpected character
            }
            break;
        case GOT_RED_R:
            if (c == 'E') {
                touch_RED_bar_parser = GOT_RED_E;
            } else {
                reset_parsing_touch_red_bar();  // Reset on unexpected character
            }
            break;
        case GOT_RED_E:
            if (c == 'D') {
                CHANGING_RED_TIMER_flag = 1;
            }
            reset_parsing_touch_red_bar();  // Reset on unexpected character
            break;
    }
}

// Define the states of the FSM
typedef enum {
    WAITING_FOR_YELLOW_A,
    GOT_YELLOW_A,
    GOT_YELLOW_M,
    GOT_YELLOW_B,
    GOT_YELLOW_E
} Touch_YELLOW_bar_parser;

Touch_YELLOW_bar_parser touch_YELLOW_bar_parser = WAITING_FOR_YELLOW_A;

void reset_parsing_touch_YELLOW_bar() {
    touch_YELLOW_bar_parser = WAITING_FOR_YELLOW_A;
}

void parse_touch_YELLOW_bar(char c) { // parses phrase "AMBER"
    switch (touch_YELLOW_bar_parser) {
        case WAITING_FOR_YELLOW_A:
            if (c == 'A') {
                touch_YELLOW_bar_parser = GOT_YELLOW_A;
            } else {
                reset_parsing_touch_YELLOW_bar();  // Reset on unexpected character
            }
            break;
        case GOT_YELLOW_A:
            if (c == 'M') {
                touch_YELLOW_bar_parser = GOT_YELLOW_M;
            } else {
                reset_parsing_touch_YELLOW_bar();  // Reset on unexpected character
            }
            break;
        case GOT_YELLOW_M:
            if (c == 'B') {
                touch_YELLOW_bar_parser = GOT_YELLOW_B;
            } else {
                reset_parsing_touch_YELLOW_bar();  // Reset on unexpected character
            }
            break;
        case GOT_YELLOW_B:
            if (c == 'E') {
                touch_YELLOW_bar_parser = GOT_YELLOW_E;
            } else {
                reset_parsing_touch_YELLOW_bar();  // Reset on unexpected character
            }
            break;
        case GOT_YELLOW_E:
            if (c == 'R') {
                CHANGING_YELLOW_TIMER_flag = 1;
            }
            reset_parsing_touch_YELLOW_bar();  // Reset on unexpected character
            break;
    }
}


typedef enum {
    WAITING_FOR_GREEN_G,
    GOT_GREEN_G,
    GOT_GREEN_R,
    GOT_GREEN_E1,
    GOT_GREEN_E2
} Touch_GREEN_bar_parser;

Touch_GREEN_bar_parser touch_GREEN_bar_parser = WAITING_FOR_GREEN_G;

void reset_parsing_touch_GREEN_bar() {
    touch_GREEN_bar_parser = WAITING_FOR_GREEN_G;
}

void parse_touch_GREEN_bar(char c) { // parses phrase "GREEN"
    switch (touch_GREEN_bar_parser) {
        case WAITING_FOR_GREEN_G:
            if (c == 'G') {
                touch_GREEN_bar_parser = GOT_GREEN_G;
            } else {
                reset_parsing_touch_GREEN_bar();  // Reset on unexpected character
            }
            break;
        case GOT_GREEN_G:
            if (c == 'R') {
                touch_GREEN_bar_parser = GOT_GREEN_R;
            } else {
                reset_parsing_touch_GREEN_bar();  // Reset on unexpected character
            }
            break;
        case GOT_GREEN_R:
            if (c == 'E') {
                touch_GREEN_bar_parser = GOT_GREEN_E1;
            } else {
                reset_parsing_touch_GREEN_bar();  // Reset on unexpected character
            }
            break;
        case GOT_GREEN_E1:
            if (c == 'E') {
                touch_GREEN_bar_parser = GOT_GREEN_E2;
            } else {
                reset_parsing_touch_GREEN_bar();  // Reset on unexpected character
            }
            break;
        case GOT_GREEN_E2:
            if (c == 'N') {
                CHANGING_GREEN_TIMER_flag = 1;
            }
            reset_parsing_touch_GREEN_bar();  // Reset on unexpected character
            break;
    }
}


typedef enum {
    WAITING_FOR_SET_S,
    GOT_SET_S,
    GOT_SET_E
} Set_timer_parser;

Set_timer_parser set_timer_parser = WAITING_FOR_SET_S;

void reset_parsing_SET_timer() {
    set_timer_parser = WAITING_FOR_SET_S;
}

void parse_SET_timer(char c) {
    switch (set_timer_parser) {
        case WAITING_FOR_SET_S:
            if (c == 'S') {
                set_timer_parser = GOT_SET_S;
            } else {
                reset_parsing_SET_timer();  // Reset on unexpected character
            }
            break;
        case GOT_SET_S:
            if (c == 'E') {
                set_timer_parser = GOT_SET_E;
            } else {
                reset_parsing_SET_timer();  // Reset on unexpected character
            }
            break;
        case GOT_SET_E:
            if (c == 'T') {
                SET_TIMER_flag = 1;
            }
            reset_parsing_SET_timer();  // Reset on unexpected character
            break;
    }
}


typedef enum {
    WAITING_FOR_DIGIT,
    GOT_DIGIT,
    GOT_SECOND_DIGIT,
    GOT_THIRD_DIGIT,
    GOT_HASH
} NumberParserState;

NumberParserState number_parser_state = WAITING_FOR_DIGIT;
uint16_t parsed_number = 0;  // Variable to save the parsed number
uint16_t user_timer = 0;
uint8_t HASH_flag = 0;

void reset_number_parser() {
    number_parser_state = WAITING_FOR_DIGIT;
    parsed_number = 0;  // Reset the parsed number
}

void parse_number(char c) {
    switch (number_parser_state) {
        case WAITING_FOR_DIGIT:
            if (c >= '0' && c <= '9') {
                parsed_number = c - '0';  // Store the first digit
                number_parser_state = GOT_DIGIT;
            } else {
                reset_number_parser();  // Reset on unexpected character
            }
            break;
        case GOT_DIGIT:
            if (c >= '0' && c <= '9') {
                parsed_number = parsed_number * 10 + (c - '0');  // Store the second digit
                number_parser_state = GOT_SECOND_DIGIT;
            } else if (c == '#') {
                number_parser_state = GOT_HASH;
            } else {
                reset_number_parser();  // Reset on unexpected character
            }
            break;
        case GOT_SECOND_DIGIT:
            if (c >= '0' && c <= '9') {
                parsed_number = parsed_number * 10 + (c - '0');  // Store the third digit
                number_parser_state = GOT_THIRD_DIGIT;
            } else if (c == '#') {
                number_parser_state = GOT_HASH;
            } else {
                reset_number_parser();  // Reset on unexpected character
            }
            break;
        case GOT_THIRD_DIGIT:
            if (c == '#') {
                number_parser_state = GOT_HASH;
            } else {
                reset_number_parser();  // Reset on unexpected character
            }
            break;
        case GOT_HASH: //do nothing because we process it below in if
        	break;
    }

    if(number_parser_state == GOT_HASH)
    {
    	HASH_flag = 1;
    	user_timer = parsed_number;
    	reset_number_parser();
    }
}

typedef enum {
    WAITING_FOR_REDOD_R,
    GOT_REDOD_R,
    GOT_REDOD_E,
    GOT_REDOD_D1,
    GOT_REDOD_O
} Touch_REDOD_bar_parser;

Touch_REDOD_bar_parser touch_REDOD_bar_parser = WAITING_FOR_REDOD_R;

void reset_parsing_touch_REDOD_bar() {
    touch_REDOD_bar_parser = WAITING_FOR_REDOD_R;
}

void parse_touch_REDOD_bar(char c) { // parses phrase "REDOD"
    switch (touch_REDOD_bar_parser) {
        case WAITING_FOR_REDOD_R:
            if (c == 'R') {
                touch_REDOD_bar_parser = GOT_REDOD_R;
            } else {
                reset_parsing_touch_REDOD_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDOD_R:
            if (c == 'E') {
                touch_REDOD_bar_parser = GOT_REDOD_E;
            } else {
                reset_parsing_touch_REDOD_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDOD_E:
            if (c == 'D') {
                touch_REDOD_bar_parser = GOT_REDOD_D1;
            } else {
                reset_parsing_touch_REDOD_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDOD_D1:
            if (c == 'O') {
                touch_REDOD_bar_parser = GOT_REDOD_O;
            } else {
                reset_parsing_touch_REDOD_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDOD_O:
            if (c == 'D') {
            	RED_AT_ODD_flag = 1;
            }
                reset_parsing_touch_REDOD_bar();  // Reset on unexpected character
            break;
    }
}

typedef enum {
    WAITING_FOR_REDEV_R,
    GOT_REDEV_R,
    GOT_REDEV_E,
    GOT_REDEV_D1,
    GOT_REDEV_E2
} Touch_REDEV_bar_parser;

Touch_REDEV_bar_parser touch_REDEV_bar_parser = WAITING_FOR_REDEV_R;

void reset_parsing_touch_REDEV_bar() {
    touch_REDEV_bar_parser = WAITING_FOR_REDEV_R;
}

void parse_touch_REDEV_bar(char c) { // parses phrase "REDEV"
    switch (touch_REDEV_bar_parser) {
        case WAITING_FOR_REDEV_R:
            if (c == 'R') {
                touch_REDEV_bar_parser = GOT_REDEV_R;
            } else {
                reset_parsing_touch_REDEV_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDEV_R:
            if (c == 'E') {
                touch_REDEV_bar_parser = GOT_REDEV_E;
            } else {
                reset_parsing_touch_REDEV_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDEV_E:
            if (c == 'D') {
                touch_REDEV_bar_parser = GOT_REDEV_D1;
            } else {
                reset_parsing_touch_REDEV_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDEV_D1:
            if (c == 'E') {
                touch_REDEV_bar_parser = GOT_REDEV_E2;
            } else {
                reset_parsing_touch_REDEV_bar();  // Reset on unexpected character
            }
            break;
        case GOT_REDEV_E2:
            if (c == 'V') {
            	RED_AT_EVEN_flag = 1; //set flag for RED at EVEN road
            }
                reset_parsing_touch_REDEV_bar();  // Reset on unexpected character
            break;
    }
}
