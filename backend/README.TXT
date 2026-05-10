Backend File Description

This folder contains the backend logic used for rover navigation, MQTT communication, and command simulation for the Indoor RSSI Based Rover Localization System.

The backend is developed using Node.js and MQTT for real-time communication between the rover, frontend, and control system.

---

1. server.js

- Main backend navigation controller.
- Connects to the local MQTT broker using Node.js.
- Receives target grid locations from the frontend or user commands.
- Calculates rover movement path inside the 4×4 grid system.
- Generates navigation commands including turning angle and drive duration.
- Publishes movement commands to the rover through MQTT topics.
- Tracks rover position, heading, and movement state.
- Publishes live rover status updates for monitoring.
- Handles rover reset functionality.

Main Features:
- MQTT communication
- Grid-based navigation
- Rover path planning
- Real-time rover status updates
- Autonomous movement control

MQTT Topics Used:
- rover/travel/target
- rover/cmd
- rover/status
- rover/reset

---

2. test_send.js

- Testing and simulation script for backend validation.
- Simulates frontend target grid selection.
- Sends test navigation targets to the backend server.
- Monitors commands being sent to the ESP32 rover.
- Displays rover status updates in the terminal.
- Used for debugging and backend testing without requiring the actual frontend.

Main Features:
- MQTT test client
- Target simulation
- Command monitoring
- Backend debugging
- Status verification

---

Technologies Used

- Node.js
- MQTT
- JavaScript
- Mosquitto MQTT Broker

---

Purpose of the Backend

The backend acts as the central control layer of the system by:

- Receiving target destinations
- Planning rover movement
- Generating navigation commands
- Managing rover state
- Providing real-time communication
- Enabling autonomous indoor navigation
