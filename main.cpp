/**
 * CO2-TVOC-Sensor is s sensor to report carbon dioxide and volatile organic compound levels to MQTT and an ESP32's display.
 *
 * Copyright 2020 Benjamin Dahlhoff 
 * https://github.com/bedawi/CO2-TVOC-Sensor
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

/*
 * Libraries needed:
 * Adafruit BusIO
 * Adafruit SSD1306
 * Adafruit GFX Library
 * Heltec ESP32 Dev-Boards by Heltec Automation
 * Adafruit CCS811 Library by Adafruit
 * IotWebConf by Balazs Kelemen // Warning: The version 2.2.3 on PlatformIO is outdated/missing files. Download from original repo!
 * MQTT by Joel Gaehwiler
 * Adafruit BME280 Library by Adafriut
*/

#include <Arduino.h>
#include "SensorTimer.h"
#include "ScreenHandler.h"
#include "ScreenPage.h"
#include <IotWebConf.h>
#include <IotWebConfUsing.h> // This loads aliases for easier class names.
#include <MQTT.h>
#include "config.h"

// Configuration for IotWebConf
const int STRING_LEN = 128;
#define NUMBER_LEN 32

// -- Configuration specific key. The value should be modified if config structure was changed.
#define CONFIG_VERSION "2021-01-01"

// -- When CONFIG_PIN is pulled to ground on startup, the Thing will use the initial
//      password to buld an AP. (E.g. in case of lost password)
#define CONFIG_PIN 33 // was 36

// -- Status indicator pin.
//      First it will light up (kept LOW), on Wifi connection it will blink,
//      when connected to the Wifi it will turn off (kept HIGH).
#define STATUS_PIN LED_BUILTIN

// -- Callback method declarations (IotWebConf)
void wifiConnected();
void configSaved();
bool formValidator();

DNSServer dnsServer;
WebServer server(80);
WiFiClient net;
MQTTClient mqttClient;

char mqttServerValue[STRING_LEN];
char mqttUserNameValue[STRING_LEN];
char mqttUserPasswordValue[STRING_LEN];
char mqttTopicValue[STRING_LEN];

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword, CONFIG_VERSION);
// -- You can also use namespace formats e.g.: iotwebconf::ParameterGroup
IotWebConfParameterGroup mqttGroup = IotWebConfParameterGroup("MQTT configuration");
IotWebConfTextParameter mqttServerParam = IotWebConfTextParameter("MQTT server", "mqttServer", mqttServerValue, STRING_LEN);
IotWebConfTextParameter mqttUserNameParam = IotWebConfTextParameter("MQTT user", "mqttUser", mqttUserNameValue, STRING_LEN);
IotWebConfPasswordParameter mqttUserPasswordParam = IotWebConfPasswordParameter("MQTT password", "mqttPass", mqttUserPasswordValue, STRING_LEN);
IotWebConfTextParameter mqttTopicParam = IotWebConfTextParameter("MQTT topic", "mqttTopic", mqttTopicValue, STRING_LEN);

// Includes for SSD1306 Display
#include <heltec.h>

// Includes and instanciations for CCS811 Sensor
#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;
SensorTimer ccstimer;
uint16_t CO2, TVOC;

// Includes and instanciations for BME/BMP280 Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;
SensorTimer bmetimer;
float g_bme_temperature;
float g_bme_humidity;
float g_bme_pressure;

// other global variables
int lastScreenUpdate = millis();
//int currentScreenPage = 1;
int changeScreenSeconds = 4;
boolean needMqttConnect = false;
boolean needReset = false;
int pinState = HIGH;
unsigned long lastReport = 0;
unsigned long lastMqttConnectionAttempt = 0;

// ScreenHandler
static ScreenHandler myScreenHandler;
// Screen Pages
ScreenPage screen0_info("Air Quality Sensor", CONFIG_VERSION);
ScreenPage screen1_co2("CO2", "ppm", "parts per million");
ScreenPage screen2_tvoc("TVOC", "ppb", "parts per billion");
ScreenPage screen3_temperature("Temperature", "°C", "");
ScreenPage screen4_humidity("Humidity", "%", "");
ScreenPage screen5_pressure("Pressure", "hpa", "");
ScreenPage screen6_wifiAPSSID("AP-Mode, SSID:", thingName);
ScreenPage screen7_wifiAPPassword("AP-Mode, PWD:", wifiInitialApPassword);
ScreenPage screen8_ccs811error("Sensor error:", "CCS811");

// -- Method declarations.
void setup();
void takeReadings();
void display();
void reportReadings();
String tvocBbpToIAQ(uint16_t tvocbbm);
void handleRoot();
void mqttMessageReceived(String &topic, String &payload);
bool connectMqtt();
bool connectMqttOptions();

// -- Setup
void setup()
{
  // A second of delay helps debugging. In VS Code the serial monitor needs a second to start after upload of firmware.
  delay(1000);

  // Attaching screens to handler class
  myScreenHandler.attachScreen(&screen0_info);
  myScreenHandler.attachScreen(&screen1_co2);
  myScreenHandler.attachScreen(&screen2_tvoc);
  myScreenHandler.attachScreen(&screen3_temperature);
  myScreenHandler.attachScreen(&screen4_humidity);
  myScreenHandler.attachScreen(&screen5_pressure);
  myScreenHandler.attachScreen(&screen6_wifiAPSSID);
  myScreenHandler.attachScreen(&screen7_wifiAPPassword);
  myScreenHandler.attachScreen(&screen8_ccs811error);

  // Defining mode of PINs (Sensor CCS811 has a WAKE-PIN to control sleep)
  pinMode(g_CCS811_wake_pin, OUTPUT);
  digitalWrite(g_CCS811_wake_pin, LOW);

  // setting up display
  Heltec.display->init();
  Heltec.display->flipScreenVertically();

  // setting up WiFi (IoT Web)
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  // Setting priority of WiFi info screens to high, will be set to 0 when connection is established.
  screen6_wifiAPSSID.setPriority(2);
  screen7_wifiAPPassword.setPriority(2);

  mqttGroup.addItem(&mqttServerParam);
  mqttGroup.addItem(&mqttUserNameParam);
  mqttGroup.addItem(&mqttUserPasswordParam);
  mqttGroup.addItem(&mqttTopicParam);

  iotWebConf.setStatusPin(STATUS_PIN);
  pinMode(CONFIG_PIN, INPUT_PULLUP);
  iotWebConf.setConfigPin(CONFIG_PIN);
  iotWebConf.addParameterGroup(&mqttGroup);
  iotWebConf.setConfigSavedCallback(&configSaved);
  iotWebConf.setFormValidator(&formValidator);
  iotWebConf.setWifiConnectionCallback(&wifiConnected);

  // -- Initializing the configuration.
  bool validConfig = iotWebConf.init();
  if (!validConfig)
  {
    Serial.println("Found no valid IoTWebConf/MQTT config in EEPROM");
    mqttServerValue[0] = '\x00';
    mqttUserNameValue[0] = '\x00';
    mqttUserPasswordValue[0] = '\x00';
    strcpy(mqttTopicValue, "/tele/sensorname/");
  }
  else
  {
    Serial.println("Found valid IoTWebConf/MQTT config in EEPROM");
  }

  // -- Set up required URL handlers on the web server.
  server.on("/", handleRoot);
  server.on("/config", [] { iotWebConf.handleConfig(); });
  server.onNotFound([]() { iotWebConf.handleNotFound(); });

  mqttClient.begin(mqttServerValue, net);
  mqttClient.onMessage(mqttMessageReceived);

  // -- CCS811 Sensor

  if (!ccs.begin())
  {
    Serial.println("Failed to start sensor! Please check your wiring.");
    ccstimer.setAvailable(false);
    screen8_ccs811error.setPriority(1);
  }
  else
  {
    Serial.println("CCS811 sensor is online.");
    screen8_ccs811error.setPriority(0);
    screen8_ccs811error.setPriority(1);
    ccstimer.setAvailable(true);
    screen8_ccs811error.setPriority(0);
    ccstimer.setRepeatTime(g_CCS811_report_period);
    ccstimer.setWarmupTime(0);
    ccstimer.setColdstartTime(g_CCS811_coldstart_period);
    ccstimer.skipWaiting();
  }

  screen1_co2.setPriority(0);
  screen2_tvoc.setPriority(0);

  // Starting BME/BMP280 sensor
  if (!bme.begin(g_bme_i2c_address, &Wire))
  {
    Serial.println("Could not start BME/BMP280 sensor.");
    bmetimer.setAvailable(false);
  }
  else
  {
    Serial.println("BME/BMP280 sensor is online.");
    g_bme_humidity = bme.readHumidity();
    g_bme_temperature = bme.readTemperature();
    g_bme_pressure = bme.readPressure();
    screen3_temperature.setValue(g_bme_temperature);
    screen4_humidity.setValue(g_bme_humidity);
    screen5_pressure.setValue(g_bme_pressure);
    bmetimer.setAvailable(true);
    bmetimer.setRepeatTime(g_bme_report_period);
    bmetimer.setWarmupTime(0);
    bmetimer.setColdstartTime(0);
    bmetimer.skipWaiting();
  }
}

void takeReadings()
{
  if (bmetimer.isAvailable() && bmetimer.isReady())
  {
    Serial.println("Taking reading from BME");
    g_bme_humidity = bme.readHumidity();
    g_bme_temperature = bme.readTemperature();
    g_bme_pressure = bme.readPressure();
    screen3_temperature.setValue(g_bme_temperature);
    screen3_temperature.setPriority(1);
    screen4_humidity.setValue(g_bme_humidity);
    screen4_humidity.setPriority(1);
    screen5_pressure.setValue(g_bme_pressure);
    screen5_pressure.setPriority(1);
    bmetimer.startover();
    bmetimer.readingsTaken();
  }

  if (ccs.available() && ccstimer.isAvailable() && ccstimer.isReady())
  {
    Serial.println("Taking reading from CCS811");
    if (!ccs.readData())
    {
      if (bmetimer.isAvailable())
      {
        g_bme_humidity = bme.readHumidity();
        g_bme_temperature = bme.readTemperature();
        g_bme_pressure = bme.readPressure();
        ccs.setEnvironmentalData(g_bme_humidity, g_bme_temperature);
      }
      CO2 = ccs.geteCO2();
      screen1_co2.setValue(CO2);
      TVOC = ccs.getTVOC();
      screen2_tvoc.setValue(TVOC);
      screen2_tvoc.setComment(tvocBbpToIAQ(TVOC));
      screen1_co2.setPriority(1);
      screen2_tvoc.setPriority(1);
      ccstimer.readingsTaken();
      ccstimer.startover();
      digitalWrite(g_CCS811_wake_pin, HIGH); // Sending Sensor to sleep...
    }
    else
    {
      // Reading failed
    }
  }
  else
  {
    // Sensor unavailable
  }
}

void display()
{
  if ((lastScreenUpdate + (changeScreenSeconds * 1000)) < millis())
  {
    ScreenPage *currentScreenPage = myScreenHandler.returnNextScreen();
    Heltec.display->clear();
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0, currentScreenPage->getLine1());
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(10, 20, currentScreenPage->getLine2());
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 50, currentScreenPage->getLine3());
    Heltec.display->display();
    lastScreenUpdate = millis();
  }
}

void reportReadings()
{
  String topic = mqttTopicValue;
  if (ccstimer.readingsWaiting())
  {
    // reporting to serial console
    Serial.print("CO2: ");
    Serial.print(CO2);
    Serial.print("ppm, TVOC: ");
    Serial.println(TVOC);

    // reporting to MQTT
    topic = mqttTopicValue;
    topic.concat("CO2");
    if (mqttClient.publish(topic.c_str(), String(CO2)))
    {
      Serial.print("CO2 value published: ");
      Serial.println(CO2);
    }

    topic = mqttTopicValue;
    topic.concat("TVOC");
    if (mqttClient.publish(topic.c_str(), String(TVOC)))
    {
      Serial.print("TVOC value published: ");
      Serial.println(TVOC);
    }
    ccstimer.readingsReported();
  }
  if (bmetimer.readingsWaiting())
  {
    topic = mqttTopicValue;
    topic.concat("humidity");
    if (mqttClient.publish(topic.c_str(), String(g_bme_humidity)))
    {
      Serial.print("Humidity value published");
      Serial.println(g_bme_humidity);
    }
    topic = mqttTopicValue;
    topic.concat("temperature");
    if (mqttClient.publish(topic.c_str(), String(g_bme_temperature)))
    {
      Serial.println("Temperature value published.");
    }
    topic = mqttTopicValue;
    topic.concat("pressure");
    if (mqttClient.publish(topic.c_str(), String(g_bme_pressure)))
    {
      Serial.println("Pressure value published.");
    }
    bmetimer.readingsReported();
  }
}

String tvocBbpToIAQ(uint16_t tvocbbm)
{
  /*
  This function returns the Air Quality (AQI) rating for indoors as defined by the German Federal Environmental Agency.
  cf. https://www.renesas.com/us/en/document/whp/overview-tvoc-and-indoor-air-quality
  cf. https://www.umweltbundesamt.de/sites/default/files/medien/pdfs/Handreichung.pdf 
  Calculation: 
    0,3 mg/m3 * 0,5 = 0,15 ppm
    0,15 ppm * 1000 = 150 ppb
  Rating
    <150ppb	  Clean Hygienic Air
    <500ppb	  Good Air Quality
    <1500ppb	Noticeable Comfort Concerns
    <5000ppb	Significant Comfort Issues
    >5000	    Unacceptable Conditions
  */
  if (tvocbbm >= 5000)
  {
    return "Unacceptable Conditions";
  }
  else if (tvocbbm >= 1500)
  {
    return "Significant Comfort Issues";
  }
  else if (tvocbbm >= 500)
  {
    return "Noticeable Comfort Concerns";
  }
  else if (tvocbbm >= 150)
  {
    return "Good Air Quality";
  }
  return "Clean Hygienic Air";
}

// Main Loop
void loop()
{
  // -- IoTWebConf related
  iotWebConf.doLoop();
  mqttClient.loop();

  if (needMqttConnect)
  {
    if (connectMqtt())
    {
      needMqttConnect = false;
    }
  }
  else if ((iotWebConf.getState() == IOTWEBCONF_STATE_ONLINE) && (!mqttClient.connected()))
  {
    Serial.println("MQTT reconnect");
    connectMqtt();
  }

  if (needReset)
  {
    Serial.println("Rebooting after 1 second.");
    iotWebConf.delay(1000);
    ESP.restart();
  }

  // -- Sensor related
  if (ccstimer.isTimetoWakeup())
  {
    ccstimer.wakeUp();
    // Waking Sensor up...
    Serial.println("Waking up CCS811");
    digitalWrite(g_CCS811_wake_pin, LOW);
  }

  if (bmetimer.isTimetoWakeup())
  {
    Serial.println("Waking up BME280");
    bmetimer.wakeUp();
  }

  takeReadings();
  reportReadings();
  display();
}

/**
 * Handle web requests to "/" path.
 */
void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>CCS811 CO2-Sensor</title></head><body>";
  s += "<ul>";
  s += "<li>CO2:&nbsp:&nbsp;";
  s += CO2;
  s += " ppm";
  s += "<li>TVOC:&nbsp;";
  s += TVOC;
  s += " bbp";
  s += "<li>MQTT server: ";
  s += mqttServerValue;
  s += "</ul>";
  s += "Go to <a href='config'>configure page</a> to change values.";
  s += "</body></html>\n";

  server.send(200, "text/html", s);
}

void wifiConnected()
{
  needMqttConnect = true;
  screen6_wifiAPSSID.setPriority(0);
  screen7_wifiAPPassword.setPriority(0);
}

void configSaved()
{
  Serial.println("Configuration was updated.");
  needReset = true;
}

bool formValidator()
{
  Serial.println("Validating form.");
  bool valid = true;

  int l = server.arg(mqttServerParam.getId()).length();
  if (l < 3)
  {
    mqttServerParam.errorMessage = "Please provide at least 3 characters!";
    valid = false;
  }

  return valid;
}

bool connectMqtt()
{
  unsigned long now = millis();
  if (1000 > now - lastMqttConnectionAttempt)
  {
    // Do not repeat within 1 sec.
    return false;
  }
  Serial.println("Connecting to MQTT server...");
  if (!connectMqttOptions())
  {
    lastMqttConnectionAttempt = now;
    return false;
  }
  Serial.println("Connected!");
  return true;
}

bool connectMqttOptions()
{
  bool result;
  if (mqttUserPasswordValue[0] != '\0')
  {
    result = mqttClient.connect(iotWebConf.getThingName(), mqttUserNameValue, mqttUserPasswordValue);
  }
  else if (mqttUserNameValue[0] != '\0')
  {
    result = mqttClient.connect(iotWebConf.getThingName(), mqttUserNameValue);
  }
  else
  {
    result = mqttClient.connect(iotWebConf.getThingName());
  }
  return result;
}

void mqttMessageReceived(String &topic, String &payload)
{
  Serial.println("Incoming: " + topic + " - " + payload);
}
