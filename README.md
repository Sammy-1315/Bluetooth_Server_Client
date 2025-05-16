# Bluetooth_Server_Client
This project demonstrates a Bluetooth Low Energy (BLE) communication setup between an ESP32 microcontroller and a Python client. The ESP32 acts as a BLE server, periodically sending data, while the Python script functions as a BLE client, receiving and displaying notifications.

## Components
ESP32 Microcontroller: Configured as a BLE server using Arduino.

Python Client: Utilizes the bleak library to connect and receive data from the ESP32.

## Future usage
Was built with intention of integrating with a prosthetic device utilizing esp32 technology

Can potentially be used to send muscle emg data from esp32 to a GUI

## Disclaimer
This repository was made to showcase the code, the process of actually running this architecture involves configuring an IDE(VSCode or ArduinoIDE) that can upload code to a valid esp32 device.
