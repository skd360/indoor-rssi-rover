# indoor-rssi-rover
# Indoor RSSI Based Rover Localization System
<img width="500" alt="Project Architecture" src="https://github.com/user-attachments/assets/8a14f371-2beb-406a-809d-fb7ef19b473b" />

## Overview

This project is a low-cost indoor localization and navigation system designed for small mobile robots operating inside environments such as warehouses and factories where GPS cannot function properly.

The system uses four ESP32 nodes placed at the corners of a selected area. These nodes continuously transmit Wi-Fi packets, which are received by the rover. The rover analyzes the RSSI (Received Signal Strength Indicator) values from each ESP32 node to estimate its approximate location within a 4×4 grid system.

The collected RSSI data is sent to the backend server for processing and storage, while the frontend dashboard provides real-time monitoring, RSSI logging, and target-based rover navigation.

---

# Features

- Indoor robot localization without GPS
- Low-cost alternative to UWB and LiDAR systems
- 4×4 grid-based indoor mapping system
- RSSI-based approximate positioning
- Real-time frontend monitoring dashboard
- Backend logging and RSSI analysis
- Target grid selection for autonomous rover navigation
- Wireless communication using ESP32 modules

---

# How It Works

- Four ESP32 nodes are placed at the corners of the area to be mapped.
- The area is divided into a 4×4 grid system.
- Each ESP32 node continuously transmits Wi-Fi packets.
- The rover scans and collects RSSI values from all four nodes.
- RSSI patterns for each grid are stored in the backend database.
- By comparing the RSSI gradients, the rover’s approximate position inside the grid can be estimated.
- Users can monitor the rover and select target grids from the frontend dashboard.

---

# Tech Stack

## Hardware
- ESP32
- Mobile Rover Platform
- DC Motors
- Motor Driver

## Software
- ESP-IDF
- Node.js
- Express.js
- MQTT/WebSockets
- HTML/CSS/JavaScript

---

# Project Structure

```text
indoor-rssi-rover/
│
├── firmware/
├── backend/
├── frontend/
└── README.md
```

---

# Applications

- Warehouse robot tracking
- Indoor autonomous navigation
- Factory automation systems
- Educational robotics projects
- Smart indoor IoT systems
- Low-cost research platforms

---

# IoT Relevance

This project fits strongly into the Internet of Things (IoT) domain because it involves interconnected ESP32 devices communicating and exchanging real-time data over a network for intelligent automation and monitoring. The system demonstrates wireless communication, distributed sensor nodes, backend connectivity, real-time data processing, and autonomous robotic control using low-cost IoT hardware.

---

# Future Improvements

- Dynamic grid scaling
- Improved localization algorithms
- Machine learning based RSSI prediction
- Multi-rover support
- Mobile application integration
- Cloud-based analytics dashboard

---

# Setup Instructions

## Clone Repository

```bash
git clone https://github.com/your-username/your-repository-name.git
```

## Backend Setup

```bash
cd backend
npm install
npm start
```

## Frontend Setup

```bash
cd frontend
npm install
npm start
```

## ESP32 Firmware

- Open firmware in ESP-IDF
- Configure Wi-Fi credentials
- Flash firmware to ESP32 devices

---

# Team

- Sanath Kumar
- Team Members

---

# License

This project is developed for educational and research purposes.
