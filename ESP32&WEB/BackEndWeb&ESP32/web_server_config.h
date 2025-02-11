#ifndef WEB_SERVER_CONFIG_H
#define WEB_SERVER_CONFIG_H

#include <LittleFS.h>

#include <HTTPClient.h>
#include "global.h"
#include <WebServer.h>

extern WebServer server;
extern String currentLed;

String getContentType(String filename);
void setupWebServer();
void handleWebServer();

#endif