#include "fsm.h"
static bool sendOdd_flag = false;
static bool sendEven_flag = false;
static bool setTimeButton_flag = false;
int index_duration;
char message[5];
static int ledDuration[3] = {0};

void fsm_run()
{
  switch(fsm_status)
  {
    case INIT:
      Serial.begin(115200);
      setupWiFi();
      setupWebServer();
      init_ua();
      setTimer(10000);
      fsm_status = MAIN_PAGE;
      break;
    case MAIN_PAGE:
      if(timer_flag)
      {
        uart_sendMessage("CHMOD");
        Serial.println("FSM: MAIN_PAGE sent CHMOD to STM32");
      }
      if(isAutoMode())
      {
        fsm_status = AUTO_MODE;
      }
      else if(isManualMode())
      {
        fsm_status = MAN_MODE;
      }
      else if(isSettingMode())
      {
        fsm_status = SETTI_MODE;
      }
      break;
    case AUTO_MODE:
      if (timer_flag)
      {
          // strcpy(message, "AUTO"); // Gán chuỗi "AUTO" vào message
          uart_sendMessage("AUTO");
          Serial.println("FSM: AUTO_MODE sent AUTO to STM32");
      }
      if (isMainPage())
      {
          fsm_status = MAIN_PAGE;
      }
      break;
    case MAN_MODE:
      if(timer_flag && sendOdd_flag == false && sendEven_flag == false)
      {
        uart_sendMessage("MAN");
        Serial.println("FSM: MAN_MODE sent MAN to STM32");
      }
      if(isTouchOddRedGreen())
      {
        sendOdd_flag = true;
        sendEven_flag = false;
      }
      if(isTouchEvenRedGreen())
      {
        sendOdd_flag = false;
        sendEven_flag = true;
      }
      if(timer_flag)
      {
        if(sendEven_flag)
        {
          uart_sendMessage("REDEV");
          Serial.println("FSM: sent REDEV to STM32");
        }
        else if(sendOdd_flag)
        {
          uart_sendMessage("REDOD");
          Serial.println("FSM: sent REDOD to STM32");
        }
      }
      if(isMainPage())
      {
        fsm_status = MAIN_PAGE;
        sendOdd_flag = false;
        sendEven_flag = false;
      }

      break;
    case SETTI_MODE:
      if(timer_flag && setTimeButton_flag)
      {
        //TODO
        char message[7] = {'S', 'E', 'T', '\0', '\0', '\0', '#'};
        int temp = ledDuration[index_duration];
        
        for(int i = 5 ; i>2 ; i--)
        {
          message [i] = convert_digit_to_char(temp%10);
          temp /= 10;
        }
        // for(int i = 0 ; i < 7 ; i++)
          uart_sendMessage(message);
        Serial.println(message);
      }

      if(timer_flag && setTimeButton_flag == false)
      {
        uart_sendMessage("SETTI");
        Serial.println("FSM: SETTI_MODE sent SETTI to STM32");
      }
      if(isMainPage())
      {
        fsm_status = MAIN_PAGE;
        //TODO
        setTimeButton_flag = false;

      }
      if(isTouchSetRed())
      {
        fsm_status = TOUCH_RED;
        setTimeButton_flag = false;
      }
      if(isTouchSetYellow())
      {
        fsm_status = TOUCH_YELLOW;
        setTimeButton_flag = false;
      }

      if(isTouchSetGreen())
      {
        fsm_status = TOUCH_GREEN;
        setTimeButton_flag = false;
      }

      break;
    case TOUCH_RED:
      if(isTouchSetTime())
      {
          setTimeButton_flag = true;
          ledDuration[0] = server.arg("red").toInt();
          index_duration = 0;
          fsm_status = SETTI_MODE;
      }

      if(isTouchSetYellow())
      {
        fsm_status = TOUCH_YELLOW;
        setTimeButton_flag = false;
      }

      if(isTouchSetGreen())
      {
        fsm_status = TOUCH_GREEN;
        setTimeButton_flag = false;
      }

      if(timer_flag && !isTouchSetTime())
      {
        uart_sendMessage("RED");
        Serial.println("FSM: TOUCH_RED sent RED to STM32");
      }

      if(!setTimeButton_flag)
      {
        if (isTouchSetFinish() && !isTouchSetTime()) 
        {
          fsm_status = SETTI_MODE;
        }
      }

      break;
    case TOUCH_YELLOW:
      if(isTouchSetTime())
      {
          setTimeButton_flag = true;
          ledDuration[2] = server.arg("yellow").toInt();
          index_duration = 2;
          fsm_status = SETTI_MODE;
      }

      if(timer_flag && !isTouchSetTime())
      {
        uart_sendMessage("AMBER");
        Serial.println("FSM: TOUCH_YELLOW sent AMBER to STM32");
      }

      if(isTouchSetRed())
      {
        fsm_status = TOUCH_RED;
        setTimeButton_flag = false;
      }

      if(isTouchSetGreen())
      {
        fsm_status = TOUCH_GREEN;
        setTimeButton_flag = false;
      }

      if(!setTimeButton_flag)
      {
        if (isTouchSetFinish() && !isTouchSetTime()) 
        {
          fsm_status = SETTI_MODE;
        }
      }

      break;
    case TOUCH_GREEN:
      if(isTouchSetTime())
      {
          setTimeButton_flag = true;
          ledDuration[1] = server.arg("green").toInt();
          index_duration = 1;
          fsm_status = SETTI_MODE;
      }
      if(timer_flag && !isTouchSetTime())
      {
        uart_sendMessage("GREEN");
        Serial.println("FSM: TOUCH_GREEN sent GREEN to STM32");
      }

      if(isTouchSetYellow())
      {
        fsm_status = TOUCH_YELLOW;
        setTimeButton_flag = false;
      }

      if(isTouchSetRed())
      {
        fsm_status = TOUCH_RED;
        setTimeButton_flag = false;
      }
      
      if(!setTimeButton_flag)
      {
        if (isTouchSetFinish() && !isTouchSetTime()) 
        {
          fsm_status = SETTI_MODE;
        }
      }
      break;
    // case TOUCH_SET:
    //   value = getInputTime();

    //   if(value >= 0)
    //   {
    //     String messageSET = "SET" + String(value) + "#";
    //     if(timer_flag)
    //     {
    //       uart_sendMessage(messageSET.c_str());
    //       Serial.println("FSM: TOUCH_SET sent " + messageSET + " to STM32");
    //     }
    //   }
    //   if(isTouchSetFinish())
    //   {
    //     fsm_status = SETTI_MODE;
    //   }
    default:
      break;
  }
}