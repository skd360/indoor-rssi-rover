Frontend File Description

This folder contains the frontend dashboard interface for the Indoor RSSI Based Rover Localization System.

The frontend is developed using HTML, CSS, and JavaScript and provides a real-time interactive dashboard for RSSI logging, rover monitoring, and autonomous navigation control.

The interface communicates with the backend using MQTT over WebSockets and provides visual feedback for rover movement and localization.

---

1. rover_dashboard.html

- Main frontend dashboard file.
- Contains the complete user interface, styling, rover visualization, and MQTT communication logic.
- Acts as the control and monitoring center for the entire system.

---

Main Features

1. RSSI Logging Mode

- Allows users to log RSSI values for each grid.
- Supports a 4×4 indoor grid mapping system.
- Users can manually enter RSSI values from four ESP32 nodes.
- Stores RSSI fingerprints for localization purposes.
- Displays mapped and unmapped zones.
- Calculates grid coverage statistics.

Features Included:
- RSSI input system
- Fingerprint storage
- Zone selection
- Coverage statistics
- Confidence estimation
- Local storage support

---

2. Travel Mode

- Allows users to select a destination grid for rover navigation.
- Simulates rover movement inside the mapped environment.
- Shows rover position and heading in real time.
- Displays rover path trail and movement logs.
- Provides animated rover movement visualization.

Features Included:
- Grid-based navigation
- Autonomous target selection
- Real-time rover animation
- Path logging
- Rover heading visualization
- Rover reset controls

---

3. MQTT Integration

The frontend communicates with the backend server using MQTT over WebSockets.

MQTT Features:
- Sends rover destination targets
- Receives rover status updates
- Displays backend connection status
- Handles rover reset commands
- Supports real-time communication

MQTT Topics Used:
- rover/travel/target
- rover/status
- rover/reset

---

4. Rover Visualization System

The dashboard contains a graphical rover simulation system using HTML Canvas.

Visualization Features:
- Real-time rover position tracking
- Direction and heading display
- Rover movement trail
- Animated grid transitions
- Target zone highlighting
- Navigation feedback

---

5. User Interface Features

The frontend dashboard includes a futuristic monitoring interface designed for robotics and IoT visualization.

UI Features:
- Interactive 4×4 grid system
- Real-time status indicators
- Responsive control panels
- Dynamic rover animation
- Live toast notifications
- RSSI confidence visualization
- Path movement logs

---

6. Localization Workflow

The frontend supports RSSI fingerprint-based indoor localization.

Workflow:
- User selects a grid zone
- RSSI values are logged for that zone
- Fingerprints are stored locally
- Travel mode uses the mapped grid system
- Rover movement and navigation are visualized in real time

---

Technologies Used

- HTML5
- CSS3
- JavaScript
- MQTT.js
- WebSockets
- HTML Canvas

---

Purpose of the Frontend

The frontend acts as the visualization and control layer of the system by:

- Logging RSSI fingerprints
- Monitoring rover movement
- Providing real-time navigation controls
- Displaying rover position and heading
- Enabling target-based navigation
- Providing backend communication visualization
- Simulating indoor autonomous navigation

---

System Role

The frontend serves as the primary Human-Machine Interface (HMI) for the project and allows users to interact with the indoor localization and navigation system in real time.
