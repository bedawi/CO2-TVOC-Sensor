# CO2-TVOC-Sensor
A sensor to report carbon dioxide and volatile organic compound levels to MQTT and an ESP32's display

![ESP32 showing carbon dioxide levels](docs/pictures/CO2-TVOC-Sensor-1.jpg)

## Functions
Currently the firmware has the following functions:

* Browser based WiFi configuration.
* Reporting of CO2 und TVOC values to Display
* Reporting of CO2 und TVOC values to MQTT

![ESP32 showing volatile organic compound levels - they are even interpreted for better understanding!](docs/pictures/CO2-TVOC-Sensor-2.jpg)

## Hardware requirements

### ESP32 with OLED Display. 
The code has been written for and tested on a [Heltec WiFi LoRa 32 (V2)](https://heltec.org/project/wifi-lora-32/) but should also work on the cheaper [WiFi Kit 32](https://heltec.org/project/wifi-kit-32/). Since the code makes use of Heltec's Arduino library to access the Display, hardware Support is currently limited to their devices.

### CCS811 Sensor
To measure carbon dioxide and volatile organic compound levels a [CCS811](https://www.sciosense.com/products/environmental-sensors/ccs811-gas-sensor-solution/) Sensor is being connected through I2C. 

The connections are:
(Sensor -> ESP32)

* GND - GND
* VCC - 3.3V
* SDA - 4
* SCL - 15
* WAKE - 3

## Software Requirements

### Compilation and Development

* Visual Studio Code
* PlatformIO
* The libraries listed in main.cpp installed

### Reporting
![Report of CO2 and TVOC levels in InfluxDB dasboard](docs/pictures/CO2-TVOC-Sensor-report.png)

Must have:

* MGTT Broker (e.g. Mosquitto)

Optional for storage and visualization of long term data:

* Node-RED
* Influx-DB
* Grafana

## What is there to improve? / Open Tasks

* Designing the firmware's integrated webpage
* Showing WiFi- and MGTT-State on OLED
* Supporting more OLED screens and other ESP32 and ESP8266 devices
* Code documentation
* LoRa/LoRaWan support
* Internationalization / translation

Your help is appreciated! Like to become a contributor?
