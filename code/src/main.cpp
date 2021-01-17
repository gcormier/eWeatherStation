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

/********Weather headers********/

WiFiClient client; // wifi client object
/*******************/

const char *ssid = "jigglypop";
const char *password = "r1cerb0y";

RTC_DATA_ATTR unsigned long currentMillis = 0;
RTC_DATA_ATTR unsigned long lastLocalMillis = 0, lastRemoteMillis = 0, lastNetMillis = 0;
unsigned long waitingRemoteWeather = 0;

const char HelloWorld[] = "Hello World!";
const char HelloArduino[] = "Hello Arduino!";

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

#define PIN_RADIO 25
#define PIN_RADIO_EN 26

bool screenUpdate = false;

byte currentTemp;
byte currentHumidity;
bool batteryStatus;

Adafruit_Si7021 sensor = Adafruit_Si7021();

RTC_DATA_ATTR float localTemp, localHumidity;
RTC_DATA_ATTR float remoteTemp, remoteHumidity;
RTC_DATA_ATTR bool remoteBatteryLow, localBatteryLow;

void updateLocalWeather();
void updateNetWeather();
void updateRemoteWeather();
void pulseISR();
void updateScreen();
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
  localTemp = sensor.readTemperature();
  localHumidity = sensor.readHumidity();
}

void getRemoteReadings()
{
}

void setup()
{
  pinMode(19, INPUT); // Hack until PCB revision 3

  pinMode(PIN_RADIO_EN, OUTPUT);
  pinMode(PIN_RADIO, INPUT);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_ADC_BATT, INPUT);
  pinMode(PIN_CHGSTAT, INPUT);

  digitalWrite(PIN_RADIO_EN, LOW);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  Serial.begin(115200);

  delay(25);
  // 75uA during deep sleep without screen
  // 210uA during deep sleep with screen
  // 35mA when checking remote
  // 25mA when checking local

  void loop()
  {
    checkForActivity();

    if (screenUpdate)
    {
      screenUpdate = false;
      updateScreen();
    }

    // Check if we can go to sleep
    if (waitingRemoteWeather == 0)
    {
      sleepyTime();
    }
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

  /* ****
  Weather code below
  */

  void startWeather()
  {
    StartWiFi();
    SetupTime();
    bool Received_WxData_OK = false;
    Received_WxData_OK = (obtain_wx_data("weather") && obtain_wx_data("forecast"));
    // Now only refresh the screen if all the data was received OK, otherwise wait until the next timed check otherwise wait until the next timed check
    if (Received_WxData_OK)
    {
      StopWiFi(); // Reduces power consumption
      gfx.init();
      gfx.setRotation(0);
      gfx.setColor(EPD_BLACK);
      gfx.fillBuffer(EPD_WHITE);
      gfx.setTextAlignment(TEXT_ALIGN_LEFT);
      Display_Weather();
      DrawBattery(SCREEN_WIDTH - 65, 0);
      gfx.commit();
      delay(2000);
    }
  }

  void Draw_Main_Wx(int x, int y)
  {
    DrawWind(x, y, WxConditions[0].Winddir, WxConditions[0].Windspeed);
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    gfx.setFont(ArialRoundedMTBold_14);
    gfx.drawString(x, y - 28, String(WxConditions[0].High, 0) + "� | " + String(WxConditions[0].Low, 0) + "�"); // Show forecast high and Low
    gfx.setFont(ArialMT_Plain_24);
    gfx.drawString(x - 5, y - 10, String(WxConditions[0].Temperature, 1) + "�"); // Show current Temperature
    gfx.setFont(ArialRoundedMTBold_14);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(x + String(WxConditions[0].Temperature, 1).length() * 11 / 2, y - 9, Units == "M" ? "C" : "F"); // Add in smaller Temperature unit
    gfx.setFont(ArialRoundedMTBold_14);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
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
    wifisection = millis() - wifisection;
  }
