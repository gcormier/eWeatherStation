#include <Arduino.h>
#include <EPD_WaveShare.h>    // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "EPD_WaveShare_42.h" // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include <MiniGrafx.h>        // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "DisplayDriver.h"    // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "ArialRounded.h"     // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "owm_credentials2.h" // See 'owm_credentials' tab and enter your OWM API key and set the Wifi SSID and PASSWORD
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson NOTE: *** MUST BE Version-6 or above ***

#include <Adafruit_Si7021.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <AsyncMqttClient.h>

/********Weather headers********/

WiFiClient client; // wifi client object
/*******************/

const char *ssid = "jigglypop";
const char *password = "r1cerb0y";

#define MQTT_HOST IPAddress(192, 168, 1, 33)
#define MQTT_PORT 1883
const char *MQTT_USER = "mqtt";
const char *MQTT_PASS = "mqtt123";

AsyncMqttClient mqttClient;

#define HOST_NAME "eWeather"

// Added to the battery reading for fine tuning
#define BATTERY_OFFSET 0.18f

// How long to sleep between reading measurements
#define INTERVAL_SLEEP 15
#define INTERVAL_LOCAL 60
#define INTERVAL_REMOTE 120
#define INTERVAL_NET 30
#define INTERVAL_REMOTE_TIMEOUT 45 // How many seconds until we timeout waiting for the remote sensor to transmit

#define PIN_PAPER_BUSY 4
#define PIN_PAPER_RST 16
#define PIN_PAPER_DC 17
#define PIN_PAPER_CLK 18
#define PIN_PAPER_CS 5
#define PIN_PAPER_DIN 23

#define PIN_CHGSTAT 14
#define PIN_ADC_BATT 32

#define PIN_LED1 13
#define PIN_LED2 15
#define PIN_BUZZER 27

bool screenUpdate = false;

bool batteryStatus;

Adafruit_Si7021 sensor = Adafruit_Si7021();

int remoteTemp;
float remoteHumidity;

double lastTemp, lastHumidity;


void updateLocalWeather();
void drawScreen();
void sleepyTime();
void startWeather();
int StartWiFi();
void SetupTime();
void StopWiFi();
void Draw_Heading_Section();
void Display_Weather();

void initLocalSensor()
{
  if (!sensor.begin())
  {
    Serial.println("Did not find Si7021 sensor!");
    return;
  }
  switch (sensor.getModel())
  {
  case SI_Engineering_Samples:
    Serial.print("SI engineering samples");
    break;
  case SI_7013:
    Serial.print("Si7013");
    break;
  case SI_7020:
    Serial.print("Si7020");
    break;
  case SI_7021:
    Serial.print("Si7021");
    break;
  case SI_UNKNOWN:
  default:
    Serial.print("Unknown");
  }
  Serial.print(" Rev(");
  Serial.print(sensor.getRevision());
  Serial.print(")");
  Serial.print(" Serial #");
  Serial.print(sensor.sernum_a, HEX);
  Serial.println(sensor.sernum_b, HEX);
}

void getLocalReadings()
{
  //localTemp = sensor.readTemperature();
  //localHumidity = sensor.readHumidity();
}

void setRemoteHumidity(int humidity)
{
  Serial.print("Setting remote humidity to ");
  Serial.println(humidity);
  remoteHumidity = humidity;
  lastHumidity = millis();
}

void setRemoteTemperature(float temperature)
{
  Serial.print("Setting remote temperature to ");
  Serial.println(temperature);
  remoteTemp = temperature;
  lastTemp = millis();
}

void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent)
{
  Serial.println("Connected to MQTT.");
  mqttClient.subscribe("rtl_433/cba70ce2a24f/devices/Acurite-Tower/A/4131/humidity", 1);
  mqttClient.subscribe("rtl_433/cba70ce2a24f/devices/Acurite-Tower/A/4131/temperature_C", 1);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);

  Serial.println(payload);
  String tp = String(topic);
  
  if (tp.indexOf("humidity") > 0)
    setRemoteHumidity(String(payload).toInt());
  else if (tp.indexOf("temperature") > 0)
    setRemoteTemperature(String(payload).toFloat());
}

void setup()
{
  pinMode(19, INPUT); // Hack until PCB revision 3

  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_ADC_BATT, INPUT);
  pinMode(PIN_CHGSTAT, INPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  Serial.begin(115200);

  delay(25);
  // 75uA during deep sleep without screen
  // 210uA during deep sleep with screen
  // 35mA when checking remote
  // 25mA when checking local

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCredentials(MQTT_USER, MQTT_PASS);
  
  StartWiFi();
  delay(250);
  Serial.println("Connecting to MQTT...");
  connectToMqtt();
  Serial.println("Done.");
}
void loop()
{

}

float getBatteryVoltage()
{
  analogReadResolution(12);
  analogSetAttenuation(ADC_0db);
  analogSetWidth(12);
  adcAttachPin(PIN_ADC_BATT);

  uint16_t val = analogRead(PIN_ADC_BATT);
  float conv = val / 4096.0;

  return ((conv * 620) / 150) + BATTERY_OFFSET;
}

int StartWiFi()
{
  int connAttempts = 0;
  Serial.print(F("\r\nConnecting to: "));
  Serial.println(String(ssid1));
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid1, password1);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (connAttempts > 20)
    {
      WiFi.disconnect();
      sleepyTime();
    }
    connAttempts++;
  }
  Serial.println("WiFi connected at: " + String(WiFi.localIP()));
  return 1;
}

void StopWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void sleepyTime()
{

}

void drawScreen()
{
  //sensor.readTemperature
  //sensor.readHumidity
  //remoteTemp, remoteHumidity
  float voltage = getBatteryVoltage();

  Serial.println(sensor.readTemperature());
  Serial.println(sensor.readHumidity());
  
  Serial.println(remoteTemp);
  Serial.println(remoteHumidity);

  Serial.println(voltage);
}