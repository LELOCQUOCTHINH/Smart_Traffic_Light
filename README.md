# Smart Traffic Light System

A web-controlled traffic light management system built with ESP32 and STM32 microcontrollers, featuring automatic timing sequences, manual control, and configurable parameters through a responsive web interface.

## System Architecture

The system implements a three-tier architecture:

- **Web Interface Tier**: HTML/CSS/JavaScript frontend with Bootstrap framework [1](#0-0) 
- **ESP32 Controller Tier**: Middleware handling web requests and finite state machine logic [2](#0-1) 
- **STM32 Hardware Tier**: Physical traffic light control and LCD display [3](#0-2) 

## Features

### Operational Modes

1. **Automatic Mode**: Timer-based traffic light sequences with configurable durations
2. **Manual Mode**: Direct control of odd/even road configurations
3. **Setting Mode**: Configuration interface for timing parameters (0-999 seconds)

### Web Interface

The frontend provides four main pages:
- `index.html`: Main mode selection interface [4](#0-3) 
- `auto_mode.html`: Real-time countdown display [5](#0-4) 
- `manual_mode.html`: Manual control buttons [6](#0-5) 
- `setting_mode.html`: Timer configuration interface

## Hardware Requirements

- ESP32 microcontroller (web server and communication)
- STM32F103RBTx microcontroller (traffic light control) [7](#0-6) 
- LCD display (I2C interface)
- LED traffic lights for odd/even roads
- UART communication between ESP32 and STM32

## Communication Protocol

The system uses UART commands for ESP32-STM32 communication:

| Command | Purpose |
|---------|---------|
| `CHMOD` | Check mode status |
| `AUTO` | Enable automatic mode |
| `MAN` | Enable manual mode |
| `SETTI` | Enable setting mode |
| `REDOD`/`REDEV` | Manual road control |
| `SET###` | Set timing duration | [8](#0-7) 

## Project Structure

```
├── ESP32&WEB/
│   ├── FrontEndWeb/          # Web interface files
│   │   ├── index.html        # Main page
│   │   ├── auto_mode.html    # Automatic mode
│   │   ├── manual_mode.html  # Manual control
│   │   ├── setting_mode.html # Configuration
│   │   └── styles.css        # Styling
│   └── BackEndWeb&ESP32/     # ESP32 backend
│       ├── backendESP32.ino  # Main ESP32 code
│       └── fsm.cpp           # Finite state machine
└── STM32/                    # STM32 project
    └── Core/
        ├── Inc/              # Header files
        └── Src/              # Source files
```

## Getting Started

### ESP32 Setup
1. Load the ESP32 code from `ESP32&WEB/BackEndWeb&ESP32/`
2. Configure WiFi credentials
3. Upload the web interface files to ESP32 filesystem

### STM32 Setup
1. Open the STM32CubeIDE project from `STM32/`
2. Configure GPIO pins for traffic lights and LCD
3. Set up UART communication (115200 baud rate) [9](#0-8) 

### Usage
1. Connect to the ESP32 web server
2. Select desired operation mode from the main interface
3. Configure timing parameters in Setting Mode
4. Monitor traffic light status through the web interface

## Technical Details

- **Timer Resolution**: 10ms tick rate [10](#0-9) 
- **LED States**: OFF (00), YELLOW (01), GREEN (10), RED (11) [11](#0-10) 
- **Web Framework**: Bootstrap 5.3.3 with Font Awesome icons
- **Communication**: HTTP requests with fetch API for ESP32 communication

## License

© Microprocessors & Microcontrollers Assignment. All Rights Reserved.

## Notes

This project was developed as part of a microprocessors and microcontrollers course assignment, demonstrating embedded systems integration, web-based control interfaces, and real-time traffic management concepts. The system supports educational applications and small-scale traffic management implementations.

Wiki pages you might want to explore:
- [Smart Traffic Light System Overview (LELOCQUOCTHINH/Smart_Traffic_Light)](https://deepwiki.com/LELOCQUOCTHINH/Smart_Traffic_Light)
