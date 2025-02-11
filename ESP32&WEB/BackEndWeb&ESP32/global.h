#ifndef GLOBAL_H
#define GLOBAL_H
#include "uart_to_STM32.h"
#include "fsm.h"
#include "web_server_config.h"
#include "wifi_config.h"
#include "software_timer.h"

#define DISABLE       0
#define INIT          1
#define AUTO_MODE     2
#define MAN_MODE      3
#define SETTI_MODE     4
#define TOUCH_RED     5
#define TOUCH_YELLOW  6
#define TOUCH_GREEN   7
#define TOUCH_SET     8
#define TOUCH_RED_ODD 9
#define TOUCH_RED_EVE 10
#define CANCEL        11
#define MAIN_PAGE     12

extern int fsm_status;
extern String currentMode;
extern int timerToSend[3];
extern char buffer[3];

bool isAutoMode();
bool isManualMode();
bool isSettingMode();
bool isMainPage();
bool isTouchOddRedGreen();
bool isTouchEvenRedGreen();
bool isTouchSetRed();
bool isTouchSetYellow();
bool isTouchSetGreen();
bool isTouchSetFinish();
bool isTouchSetTime();
char convert_digit_to_char(int digit);
#endif