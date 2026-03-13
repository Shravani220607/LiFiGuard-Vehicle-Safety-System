# LiFiGuard – Vehicle Communication and Safety System

## Overview
LiFiGuard is a vehicle-to-vehicle communication system that uses LiFi (Light Fidelity) technology to transmit real-time safety alerts between vehicles. 
The system detects dangerous conditions such as collision, alcohol presence, and obstacles, and sends alerts to nearby vehicles using LED-based LiFi communication.

## Key Features
- Obstacle detection using ultrasonic sensor
- Collision detection using vibration sensor
- Alcohol detection using MQ-3 gas sensor
- Real-time vehicle-to-vehicle alert communication using LiFi
- LCD display and buzzer alerts for warning notifications

## Technologies Used
- Arduino Uno
- LiFi Communication
- Embedded C / Arduino Programming
- Sensors (MQ-3, SW-420, HC-SR04)
- LCD Display
- Solar Panel Receiver

## System Architecture
![System Architecture](Diagrams/system_architecture.png)

## Hardware Setup
![Project Setup](Images/setup.jpg)

## Transmitter Module
![Transmitter](Images/transmitter.jpg)

## Receiver Module
![Receiver](Images/receiver.jpg)

## Project Structure

LiFiGuard-Vehicle-Safety-System
│
├── README.md
├── Project_Report.pdf
├── LiFiGuard_Presentation.pptx
│
├── Code
│   ├── transmitter_code.ino
│   └── receiver_code.ino
│
├── Images
│   ├── setup.jpg
│   ├── transmitter.jpg
│   └── receiver.jpg
│
├── Diagrams
│   ├── system_architecture.png
│   ├── transmitter_circuit.png
│   └── receiver_circuit.png
│
└── Videos
    └── demo.mp4

## How the System Works
1. Sensors detect conditions such as obstacles, vibration (collision), or alcohol presence.
2. Arduino processes the sensor data.
3. Data is encoded and transmitted using LiFi LED light.
4. The receiver solar panel detects the light signal.
5. The receiver Arduino decodes the signal.
6. Alerts are displayed on the LCD and buzzer is activated if needed.

## Demonstration
The working demonstration video is available in the **Videos** folder.

## Contributors
- Karthikeya
- Ram Charan
- Shravani
- Haritha
- Madhava Rao
