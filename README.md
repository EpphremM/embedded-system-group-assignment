# Temperature Regulated Fan System

## Table of Contents

- [Temperature Regulated Fan System](#temperature-regulated-fan-system)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
  - [Key Features](#key-features)
  - [Components List](#components-list)
  - [System Operation Guide](#system-operation-guide)
    - [Temperature Response Modes](#temperature-response-modes)
    - [Serial Monitor Output](#serial-monitor-output)
  - [Setup Instructions](#setup-instructions)
  - [Customization](#customization)
  - [Troubleshooting](#troubleshooting)
  - [Pin Configuration](#pin-configuration)
  - [Temperature Thresholds](#temperature-thresholds)
  - [Logic Description](#logic-description)
    - [Setup](#setup)
    - [Loop](#loop)
    - [1. Reading Temperature](#1-reading-temperature)
    - [2. Output Behavior](#2-output-behavior)
      - [Cool Zone (\< 25°C)](#cool-zone--25c)
      - [Normal Zone (25°C - 34.9°C)](#normal-zone-25c---349c)
      - [Warm Zone (35°C - 46.9°C)](#warm-zone-35c---469c)
      - [Hot Zone (≥ 47°C)](#hot-zone--47c)

## Project Overview

This Arduino-based temperature monitoring and control system automatically regulates cooling based on ambient temperature readings. It provides visual and auditory feedback through LEDs and a buzzer, making it ideal for applications like PC cooling, greenhouse monitoring, or kitchen safety.

## Key Features

- Real-time temperature monitoring with LM35 sensor
- Three-tier visual feedback system (Green/Yellow/Red LEDs)
- Automatic fan speed control via PWM
- Audible alarm for critical temperatures
- Serial monitor output for debugging

## Components List

| Component | Quantity | Description |
|-----------|----------|-------------|
| Arduino Uno R3 | 1 | Main microcontroller board |
| LM35 Temperature Sensor | 1 | Precision analog temperature sensor (0-100°C) |
| 5mm LEDs (Green, Yellow, Red) | 1 each | Visual status indicators |
| 5V Active Buzzer | 1 | Audible alarm (85dB) |
| 5V DC Cooling Fan | 1 | 40mm brushless fan |
| Breadboard | 1 | Prototyping platform |
| Jumper Wires | As needed | Male-to-male connectors |
| 220Ω Resistors | 3 | Current limiters for LEDs |
| USB Cable (Type A to B) | 1 | For power and programming |

## System Operation Guide

### Temperature Response Modes

1. **Normal Mode (< 25°C)**
   - Green LED ON (50% brightness)
   - Fan OFF
   - Buzzer OFF

2. **Elevated Temperature (25-34.9°C)**
   - Yellow LED ON (50% brightness)
   - Fan OFF
   - Buzzer OFF

3. **Warning State (35-46.9°C)**
   - Yellow LED ON (100% brightness)
   - Fan at 50% speed
   - Buzzer OFF

4. **Critical State (≥ 47°C)**
   - Red LED blinking (fast)
   - Fan at 100% speed
   - Buzzer ON (1kHz tone)

### Serial Monitor Output

The system continuously outputs temperature readings in the format:

```txt
Temperature: XX.X °C
```

## Setup Instructions

1. Connect all components as per the circuit diagram
2. Upload the provided Arduino code
3. Open Serial Monitor (9600 baud) to view temperature readings
4. System will automatically begin temperature regulation

## Customization

Adjust these constants in the code to modify temperature thresholds:

```arduino
const float lowTemp = 25.0;   // Cool range upper limit
const float medTemp = 34.9;   // Normal range upper limit
const float highTemp = 46.9;  // Danger threshold
```

## Troubleshooting

- Ensure all components are properly connected
- Verify correct pin assignments in the code
- Check Serial Monitor for temperature readings
- Confirm power supply is stable (5V)


## Pin Configuration

| Component     | Arduino Pin | Notes                       |
|---------------|-------------|-----------------------------|
| LM35 Sensor   | A0          | Analog temperature input    |
| Green LED     | 9           | PWM capable                 |
| Yellow LED    | 10          | PWM capable                 |
| Red LED       | 11          | PWM capable                 |
| Buzzer        | 5           | Digital output              |
| Fan           | 6           | PWM capable for speed control |

---

## Temperature Thresholds

| Zone       | Temperature Range     | Response                                     |
|------------|-----------------------|----------------------------------------------|
| Cool       | Below 25°C            | Green LED (dim), fan off, no buzzer          |
| Normal     | 25°C – 34.9°C         | Yellow LED (dim), fan off, no buzzer         |
| Warm       | 35°C – 46.9°C         | Yellow LED (bright), fan 50%, no buzzer      |
| Hot        | 47°C and above        | Red LED blinking, fan 100%, buzzer on        |

---

## Logic Description

### Setup

```cpp
void setup() {
  Serial.begin(9600); // Initialize serial monitor
  
  // Set all component pins as outputs
  pinMode(greenLed, OUTPUT); // Green light for low temperature
  pinMode(yellowLed, OUTPUT); // Yellow for medium temperature
  pinMode(redLed, OUTPUT); // Red for hot temperature
  pinMode(buzzer, OUTPUT); // Buzzer to indicate hot temperature
  pinMode(fan, OUTPUT); // The actual fan

  // Initialize all outputs to off
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(fan, LOW);

  Serial.println("Temperature Monitoring System Started (No Resistors Version)");
}
```

### Loop

### 1. Reading Temperature

```cpp
int tempValue = analogRead(tempPin); // Read the temperature value in analog form
float temperature = (tempValue * 5.0 / 1024.0) * 100.0; // Convert the signal to celcius
```

- Reads analog input from LM35
- Converts it into temperature in Celsius using:
  - Voltage = (ADC Value / 1024) × 5V
  - LM35 gives 10mV per °C → Temperature = Voltage × 100

### 2. Output Behavior

#### Cool Zone (< 25°C)

```cpp
analogWrite(greenLed, 128);  // Green LED at 50%
analogWrite(yellowLed, 0);
analogWrite(redLed, 0);
analogWrite(fan, 0);         // Fan off
noTone(buzzer);              // Buzzer off
```

#### Normal Zone (25°C - 34.9°C)

```cpp
analogWrite(greenLed, 0);
analogWrite(yellowLed, 128); // Yellow LED at 50%
analogWrite(redLed, 0);
analogWrite(fan, 0);
noTone(buzzer);
```

#### Warm Zone (35°C - 46.9°C)

```cpp
analogWrite(greenLed, 0);
analogWrite(yellowLed, 255); // Yellow LED at 100%
analogWrite(redLed, 0);
analogWrite(fan, 128);       // Fan at 50% speed
noTone(buzzer);
```

#### Hot Zone (≥ 47°C)

```cpp
analogWrite(greenLed, 0);
analogWrite(yellowLed, 0);
analogWrite(redLed, millis() % 200 < 100 ? 255 : 0); // Blink
analogWrite(fan, 255);       // Fan at full speed
tone(buzzer, 1000);          // Buzzer at 1kHz
```
