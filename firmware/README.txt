Firmware File Description

1. main.c
- Main entry point of the firmware.
- Initializes Wi-Fi, ESP-NOW communication, and system components.
- Handles overall rover workflow and task execution.
- Coordinates communication between localization, RSSI analysis, and navigation modules.

2. espnow_rx.c
- Handles ESP-NOW packet reception.
- Receives wireless data transmitted between ESP32 devices.
- Processes incoming RSSI-related communication data.
- Used for real-time wireless communication between nodes and rover.

3. espnow_rx.h
- Header file for espnow_rx.c.
- Contains function declarations, structures, and shared definitions related to ESP-NOW reception.

4. web_server.c
- Creates a web server directly on the ESP32 rover.
- Allows external devices such as laptops or phones to connect directly to the ESP32 access point.
- Displays live rover data, RSSI values, and localization information through a browser interface.
- Enables wireless monitoring of rover data while the rover is moving.

5. web_server.h
- Header file for web_server.c.
- Contains function declarations and shared definitions for web server operations.

6. CMakeLists.txt
- ESP-IDF build configuration file.
- Defines source files and build settings required for firmware compilation.

7. README.txt
- Documentation file explaining firmware structure, functionality, and module descriptions.
