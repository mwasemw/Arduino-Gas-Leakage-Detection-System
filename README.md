# Arduino-Based Gas Leakage Detection System

**Group 10 | Embedded System Design Project**

## 1. Project Overview

This project involves the design and simulation of an Arduino-based gas leakage detection and warning system. The system uses an MQ-2 gas sensor to detect the presence of gas and an Arduino UNO to process the input and control the output devices.

When gas is detected, the system activates an audible alarm and a ventilation fan while displaying a warning message on the LCD. A push button is also provided for manually testing the alarm and fan.

## 2. Project Objectives

- To design an embedded system for gas leakage detection.
- To interface a gas sensor with an Arduino UNO.
- To provide audible and visual warnings when gas is detected.
- To implement automatic fan control.
- To provide a manual testing facility.
- To simulate and verify system operation using Proteus.

## 3. Hardware Components

| Component | Function |
|---|---|
| Arduino UNO | Central controller |
| MQ-2 Gas Sensor | Detects the presence of combustible gases |
| 16×2 LCD | Displays system status |
| Buzzer | Produces an audible alarm |
| 12V DC Fan | Provides ventilation |
| Resistors | Limit current and support switching circuits |
| NPN Transistor | Controls the buzzer |
| MOSFET | Switches the 12V fan |
| Flyback Diode | Protects the fan switching circuit |
| Push Button | Provides manual testing input |

## 4. Pin Configuration

| Component | Arduino Pin |
|---|---|
| MQ-2 sensor signal | A0 |
| LCD RS | D7 |
| LCD Enable | D6 |
| LCD D4 | D5 |
| LCD D5 | D4 |
| LCD D6 | D3 |
| LCD D7 | D2 |
| Buzzer | D8 |
| Fan control (MOSFET) | D9 |
| Push Button | D10 |

## 5. System Operation

The system continuously monitors the gas sensor input. When gas is detected, the Arduino activates the buzzer and ventilation fan and displays "GAS DETECTED!" on the LCD.

When no gas is detected, the buzzer and fan remain OFF and the LCD displays "SYSTEM NORMAL".

The push button provides a manual testing facility, allowing the fan and buzzer to be activated when required. Gas detection takes priority over manual testing.

System information is also transmitted through serial communication at 9600 baud.

## 6. Software Implementation

The system is programmed using the Arduino programming language (C/C++) and the LiquidCrystal library.

The program is organised into modular functions responsible for:

- Reading the gas sensor.
- Reading and debouncing the push button.
- Controlling the output devices.
- Displaying system status on the LCD.
- Monitoring system information through serial communication.

**Proteus simulation note:** The gas detection signal is represented by a LogicToggle. A HIGH signal represents gas detected, while LOW represents no gas detected. The current implementation therefore reads the simulated sensor signal digitally.

## 7. Project Structure

- `Arduino_Code/` – Arduino source code.
- `Proteus_Simulation/` – Proteus project and circuit schematic.
- `Documentation/` – System diagrams and project documentation.
- `Images/` – Circuit and simulation screenshots.

## 8. Software and Tools

- Arduino IDE
- Proteus Design Suite 9
- LiquidCrystal Arduino library
- GitHub

## 10. Disclaimer

This project is an educational prototype developed for embedded system design and simulation. It is not certified for real-world combustible gas safety applications.
