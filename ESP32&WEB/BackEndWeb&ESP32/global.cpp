#include "global.h"

int fsm_status = INIT;
int counter = 100;
char buffer[3];

String currentMode = "main_page";

bool isAutoMode()
{
  return currentMode == "auto";
}
bool isManualMode()
{
  return currentMode == "manual";
}
bool isSettingMode()
{
  return currentMode == "setting";
}

bool isMainPage()
{
  return currentMode == "main_page";
}

bool isTouchOddRedGreen()
{
  return server.hasArg("type") && server.arg("type") == "REDODDGREEN";
}
bool isTouchEvenRedGreen()
{
  return server.hasArg("type") && server.arg("type") == "REDEVENGREEN";
}

bool isTouchSetRed()
{
  return server.hasArg("type") && server.arg("type") == "SETTIRED";
}

bool isTouchSetYellow()
{
  return server.hasArg("type") && server.arg("type") == "SETTIYELLOW";
}

bool isTouchSetGreen()
{
  return server.hasArg("type") && server.arg("type") == "SETTIGREEN";
}

bool isTouchSetFinish() {
    // Kiểm tra tín hiệu từ ESP32
    if(!isTouchSetRed() && !isTouchSetGreen() && !isTouchSetYellow())
    {
      counter--;
      if(counter <= 0)
      {
        counter = 100;
        return true;
      }
    }
    else counter = 100;
    return false;
}

bool isTouchSetTime()
{
  return server.hasArg("type") && server.arg("type") == "SETTIME";
}

char convert_digit_to_char(int digit) {
  switch (digit)
  {
    case 0:
      return '0';
    case 1:
      return '1';
    case 2: 
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    default:
      return '9';
  }
}
