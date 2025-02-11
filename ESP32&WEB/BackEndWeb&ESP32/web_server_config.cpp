#include "HardwareSerial.h"
#include "web_server_config.h"

WebServer server(80);
String currentLed = "";

void serveFile(const char* path, const char* contentType) {
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "File Not Found");
  }
}

void setupWebServer() {
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Route for serving the main page
   server.on("/", HTTP_GET, []() {
    serveFile("/index.html", "text/html");
    Serial.println("Main page is running");
    uart_sendMessage("CHMOD"); //send message to STM32
    currentMode = "main_page";
  });

  //Route for serving the CSS file
  server.on("/styles.css", HTTP_GET, []() {
    serveFile("/styles.css", "text/css");
  });

  // Route for serving the JavaScript file
  server.on("/script.js", HTTP_GET, []() {
    serveFile("/script.js", "application/javascript");
  });
  

  server.on("/manual_mode.html", HTTP_GET, []() {
    serveFile("/manual_mode.html", "text/html");
  });

  server.on("/auto_mode.html", HTTP_GET, []() {
    serveFile("/auto_mode.html", "text/html");
  });

  server.on("/setting_mode.html", HTTP_GET, []() {
    serveFile("/setting_mode.html", "text/html");
  });

  server.on("/images/favicon-32x32.png", HTTP_GET, []() {
    serveFile("/images/favicon-32x32.png", "image/png");
});

  server.on("/images/logoBK.png", HTTP_GET, []() {
    serveFile("/images/logoBK.png", "image/png");
});

  server.on("/mode", HTTP_GET, []() {
    if (server.hasArg("type")) {
        String mode = server.arg("type");
        currentMode = mode;

        // Print the corresponding message to Serial Monitor
        if (mode == "auto") {
            Serial.println("Automatic mode is running");
            uart_sendMessage("AUTO"); //send message to STM32

        } else if (mode == "manual") {
            Serial.println("Manual mode is running");
            uart_sendMessage("MAN"); //send message to STM32
        } else if (mode == "setting") {
            Serial.println("Setting mode is running");
            uart_sendMessage("SETTI"); //send message to STM32
        } else {
            Serial.println("Unknown mode received");
        }

        // Respond to the client
        server.send(200, "text/plain", "Mode received: " + mode);
    } else {
        server.send(400, "text/plain", "No mode specified");
    }
});

//   server.on("/led", HTTP_GET, []() {
//     if (server.hasArg("type")) {
//         String ledType = server.arg("type");

//         // Gửi tín hiệu qua UART
//         if (ledType == "RED") {
//             uart_sendMessage("RED");
//             Serial.println("UART_100ms: RED");
//         } else if (ledType == "GREEN") {
//             uart_sendMessage("GREEN");
//             Serial.println("UART_100ms: GREEN");
//         } else if (ledType == "YELLOW") {
//             uart_sendMessage("YELLOW");
//             Serial.println("UART_100ms: YELLOW");
//         }

//         server.send(200, "text/plain", "LED selected: " + ledType);
//     } else {
//         server.send(400, "text/plain", "No LED type specified");
//     }
// });

  server.begin();
  Serial.println("Web server started");
}

void handleWebServer() {
  server.handleClient();
}
