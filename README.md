# Arduino Sensor Integration

This Arduino project integrates a PIR (Passive Infrared) sensor, an LDR (Light-Dependent Resistor), and a current sensor (ACS). The system adjusts the brightness of an LED based on the LDR reading, calculates the current flowing through the LED, and sends the current data, along with the LED brightness percentage, to a NodeMCU using serial communication.

## Components Used

- PIR Sensor
- LDR
- ACS (Current Sensor)
- LED
- Arduino board
- NodeMCU

## Pin Configuration

- PIR Sensor: Digital Pin 2
- LDR: Analog Pin A0
- ACS: Analog Pin A1
- LED: Digital Pin 3

## Constants

- `MAX_PWM` and `MIN_PWM`: Define the PWM range for LED brightness control.
- `MAX_PIR` and `MIN_PIR`: Define the range of PIR sensor readings.
- `MIN_PERSON_TIME`: Minimum duration for which the PIR sensor keeps the detection state.
- `START`: Start byte for serial communication.

## Variables

- `_pirEnabled`: Flag indicating whether a person has been detected by the PIR sensor.
- `_now`: Keeps track of the current time using `millis()`.
- `_pirTime`: Records the time when a person is detected by the PIR sensor.
- `_current`: Current flowing through the LED.
- `_voltageResistor`: Voltage across the resistor in the current calculation.
- `_pwm`: Pulse Width Modulation value for LED brightness control.
- `_percentage`: LED brightness percentage.
- `buffer`: Data buffer for serial communication.

## Setup

- Configure pin modes and set up serial communication.
- Initialize the LED state.

## Main Loop

- Continuously reads data from the sensors.
- Adjusts the LED brightness based on LDR readings.
- Calculates the current flowing through the LED using the ACS.
- Sends current and LED brightness percentage data to the NodeMCU through serial communication.

## Calculate Current

- The `calculateCurrent` function estimates the current flowing through the LED based on the PWM value.

## Interrupt Service Routine (ISR)

- An interrupt service routine (`pir_process`) is triggered on a rising edge of the PIR sensor signal. This routine updates `_pirTime` and sets `_pirEnabled` to true.

## Usage

1. Upload the code to your Arduino board.
2. Connect the sensors and components as per the provided pin configuration.
3. Ensure the NodeMCU code correctly expects and interprets the data format sent by the Arduino.

## NodeMCU

An ESP8266 module and facilitates communication with an MQTT broker. It connects to a specified Wi-Fi network, establishes a connection with the MQTT broker, and publishes data received from a serial device in JSON format.

## Libraries Used

- `ESP8266WiFi`
- `WiFiClient`
- `SoftwareSerial`
- `PubSubClient`
- `ArduinoJson`
