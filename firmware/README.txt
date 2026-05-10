# Firmware

This folder contains all firmware-related code used in the Indoor RSSI Based Rover Localization System.

The firmware is developed primarily using ESP-IDF and runs on ESP32 microcontrollers used for both stationary nodes and the mobile rover.

---

# Hardware Used

- ESP32 Development Boards
- Mobile Rover Chassis
- DC Motors
- Motor Driver Module
- Power Supply/Battery Pack

---

# Firmware Components

## ESP32 Corner Nodes

The stationary ESP32 nodes are placed at the corners of the mapped area. Their main purpose is to continuously transmit Wi-Fi packets that can be detected by the rover.

### Responsibilities

- Continuous Wi-Fi beacon transmission
- Stable signal broadcasting
- Providing RSSI reference values for localization

---

## Rover Firmware

The rover firmware is responsible for collecting RSSI values from all ESP32 nodes and estimating its approximate position inside the grid system.

### Responsibilities

- Wi-Fi packet scanning
- RSSI collection and analysis
- Grid estimation logic
- Navigation control
- Motor control
- Communication with backend server
- Receiving target grid commands

---

# Localization Method

The localization system works using RSSI (Received Signal Strength Indicator) values collected from four ESP32 nodes.

The mapped environment is divided into a 4×4 grid. RSSI values from each node are collected and stored for every grid during calibration. During runtime, the rover compares live RSSI values against stored values to estimate its approximate location.

This method provides a lightweight and low-cost alternative to expensive localization systems such as UWB and LiDAR.

---

# Communication

The firmware uses wireless communication between devices and the backend server.

## Communication Features

- Wi-Fi based communication
- Real-time RSSI data transfer
- Backend connectivity
- Remote command reception
- Live rover monitoring

---

# Technologies Used

- ESP-IDF
- FreeRTOS
- Wi-Fi APIs
- MQTT/WebSocket communication
- C Programming Language

---

# Purpose of the Firmware

The firmware acts as the core intelligence layer of the system by enabling:

- Indoor localization
- Autonomous navigation
- Real-time wireless communication
- Grid-based movement
- Backend data logging
- Remote monitoring and control
