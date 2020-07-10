#include <Arduino.h>
#include <EPD_WaveShare.h>     // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "EPD_WaveShare_42.h"  // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include <MiniGrafx.h>         // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "DisplayDriver.h"     // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "ArialRounded.h"      // Copyright (c) 2017 by Daniel Eichhorn https://github.com/ThingPulse/minigrafx
#include "owm_credentials2.h"  // See 'owm_credentials' tab and enter your OWM API key and set the Wifi SSID and PASSWORD
#include <ArduinoJson.h>       // https://github.com/bblanchon/ArduinoJson NOTE: *** MUST BE Version-6 or above ***

#include <Adafruit_Si7021.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <SPIFFS.h>

#include "accurite.h"
#include <WeatherArtFunctions.h>

/********Weather headers********/

WiFiClient client; // wifi client object
/*******************/

#define SENSOR_ID_FILTER    0x1023

const char* ssid = "aaaa";
const char* password = "aaaa";

RTC_DATA_ATTR unsigned long currentMillis = 0;
RTC_DATA_ATTR unsigned long lastLocalMillis = 0, lastRemoteMillis = 0, lastNetMillis = 0;
unsigned long waitingRemoteWeather = 0;

const char HelloWorld[] = "Hello World!";
const char HelloArduino[] = "Hello Arduino!";

#define HOST_NAME "eWeather"

// Added to the battery reading for fine tuning
#define BATTERY_OFFSET  0.18f

// How long to sleep between reading measurements
#define INTERVAL_SLEEP      15
#define INTERVAL_LOCAL      60
#define INTERVAL_REMOTE     120
#define INTERVAL_NET        30
#define INTERVAL_REMOTE_TIMEOUT 45    // How many seconds until we timeout waiting for the remote sensor to transmit

#define PIN_PAPER_BUSY	4
#define PIN_PAPER_RST		16
#define PIN_PAPER_DC		17
#define PIN_PAPER_CLK		18
#define PIN_PAPER_CS		5
#define PIN_PAPER_DIN		23

#define PIN_CHGSTAT			14
#define PIN_ADC_BATT		32

#define PIN_LED1        13
#define PIN_LED2        15
#define PIN_BUZZER      27

#define PIN_RADIO       25
#define PIN_RADIO_EN    26

/* RF SECTION*/
// sync states
#define RESET     0   // no sync yet
#define INSYNC    1   // sync pulses detected 
#define SYNCDONE  2   // complete sync header received 

#define MAXBITS      55

// pulse timings
// SYNC
#define SYNC_HI      700
#define SYNC_LO      550

// HIGH == 1
#define LONG_HI      475
#define LONG_LO      330

// SHORT == 0
#define SHORT_HI     275
#define SHORT_LO     150

#define RESETTIME    2000  // Probably can go to 2000 once debugging complete

volatile unsigned int    pulsecnt = 0;
volatile unsigned long   risets = 0;     // track rising edge time
volatile unsigned int    syncpulses = 0; // track sync pulses
volatile byte            state = RESET;
volatile byte            buf[7] = { 0,0,0,0,0,0,0 };  // msg frame buffer
volatile bool            haveReading = false;            // have valid reading
/* END RF SECTION*/

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
void fallingPulseHandler();
void updateScreen();
void sleepyTime();
void startWeather();
int StartWiFi();
void SetupTime();
void StopWiFi();
void Draw_Heading_Section();
void Display_Weather();

void setup()
{
  pinMode(19, INPUT);   // Hack until PCB revision 3

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

  if (currentMillis == 0) // First time starting up
  {
    updateLocalWeather();
    updateNetWeather();
    updateRemoteWeather();
  }

}

// 75uA during deep sleep without screen
// 210uA during deep sleep with screen
// 35mA when checking remote
// 25mA when checking local

// Do we have to do anything?
void checkForActivity()
{
  // TODO: BUG HERE, need to up millis by factor of 10?!?!?!
  currentMillis += millis() * 10UL;
  currentMillis += INTERVAL_SLEEP * 1000UL; // Need to add all the time we slept too

  Serial.println(currentMillis);


  if (currentMillis - lastLocalMillis > (INTERVAL_LOCAL * 1000UL))
  {
    updateLocalWeather();
    screenUpdate = true;
  }

  if (currentMillis - lastRemoteMillis > (INTERVAL_REMOTE * 1000UL))
  {
    updateRemoteWeather();
  }

  if (currentMillis - lastNetMillis > (INTERVAL_NET * 1000UL))
  {
    updateNetWeather();
    screenUpdate = true;
  }
}


void enableRadio()
{
  digitalWrite(PIN_RADIO_EN, HIGH);
  attachInterrupt(PIN_RADIO, fallingPulseHandler, CHANGE);
}

void loop()
{


  while (waitingRemoteWeather > 0)
  {
    unsigned int sensorID;

    if (millis() - waitingRemoteWeather > (INTERVAL_REMOTE_TIMEOUT * 1000UL)) // Timed out waiting for something
    {
      waitingRemoteWeather = 0;
      lastRemoteMillis = currentMillis + (millis() * 10);
    }

    if (haveReading)
    {
      noInterrupts();
      if (acurite_crc(buf, sizeof(buf)))
      {
        if (getMessageType(buf[2]) == 4)
        {
          remoteTemp = getTempC(buf[4], buf[5]);
          remoteHumidity = getHumidity(buf[3]);
          remoteBatteryLow = getLowBatteryStatus(buf[2]);
          sensorID = getSensorID(buf[0], buf[1]);
        }
      }

      haveReading = false;
      if (sensorID == SENSOR_ID_FILTER)
      {
        screenUpdate = true;
        waitingRemoteWeather = 0;
        lastRemoteMillis = currentMillis + (millis() * 10);
        digitalWrite(PIN_RADIO_EN, LOW);
      }
      interrupts();
    }
  }

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

void sleepyTime()
{
  esp_sleep_enable_timer_wakeup(INTERVAL_SLEEP * 1000000);
  esp_deep_sleep_start();
}

// Update local weather
void updateLocalWeather()
{
  Serial.println("Update local...");

  if (!sensor.begin())
    return;

  localTemp = sensor.readTemperature();
  localHumidity = sensor.readHumidity();
  lastLocalMillis = currentMillis;
  Serial.println(localTemp);
  Serial.println(localHumidity);
}

// Update remote weather
void updateRemoteWeather()
{
  Serial.println("Update remote...");

  waitingRemoteWeather = millis();
  enableRadio();
}

// Update weather from the internet
void updateNetWeather()
{
  Serial.println("Update net...");
  lastNetMillis = currentMillis;
  startWeather();
}


// Update e-ink display with new weather values
void updateScreen()
{
  Serial.println("updateScreen()");

}

void enableWifi()
{
  if (WiFi.isConnected())
    return;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}

void setupOTA()
{
  Serial.println("Setup OTA...");
  enableWifi();

  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }

  ArduinoOTA.setHostname("esp32-weather");

  ArduinoOTA
    .onStart([]() {

    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);

    // Detach interrupt so it doesn't interrupt the OTA flashing.
    detachInterrupt(PIN_RADIO);
  })
    .onEnd([]() {
    Serial.println("\nEnd");
  })
    .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
  })
    .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.begin();
}

void setupPaper()
{
  Serial.println("Setup e-ink...");

}


void fallingPulseHandler()
{
  unsigned long timestamp = micros();
  if (digitalRead(PIN_RADIO) == HIGH)
  {
    // going high, start timing
    if (timestamp - risets > RESETTIME)
    {
      // detect reset condition
      state = RESET;
      syncpulses = 0;
      pulsecnt = 0;
    }
    risets = timestamp;
    return;
  }

  // going low
  unsigned long duration = timestamp - risets;

  if (state == RESET || state == INSYNC)
  {
    // looking for sync pulses
    if ((SYNC_LO) < duration && duration < (SYNC_HI))
    {
      // start counting sync pulses
      state = INSYNC;
      syncpulses++;
      if (syncpulses > 3)
      {
        // found complete sync header
        state = SYNCDONE;
        syncpulses = 0;
        pulsecnt = 0;
      }
      return;

    }
    else
    {
      // not interested, reset  
      syncpulses = 0;
      pulsecnt = 0;
      state = RESET;

      return;
    }
  }
  else
  {
    // SYNCDONE, now look for message 
    // detect if finished here
    if (pulsecnt > MAXBITS)
    {
      state = RESET;
      pulsecnt = 0;
      haveReading = true;
      return;
    }
    // stuff buffer with message
    byte bytepos = pulsecnt / 8;
    byte bitpos = 7 - (pulsecnt % 8); // reverse bitorder
    if (LONG_LO < duration && duration < LONG_HI)
    {
      bitSet(buf[bytepos], bitpos);
      pulsecnt++;
    }
    else if (SHORT_LO < duration && duration < SHORT_HI)
    {
      bitClear(buf[bytepos], bitpos);
      pulsecnt++;
    }
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

void Display_Weather()
{
  Draw_Heading_Section();             // Top line of the display
  Draw_Main_Weather_Section(170, 70); // Centre section of display for Location, temperature, Weather report, current Wx Symbol and wind direction
  //Draw_Forecast_Section(230, 18);     // 3hr forecast boxes
  Draw_Astronomy_Section(230, 20);    // Astronomy section Sun rise/set, Moon phase and Moon icon

  gfx.drawString(10, 200, String(remoteTemp));
  gfx.drawString(10, 230, String(remoteHumidity));

  gfx.drawString(10, 260, String(localTemp));
  gfx.drawString(10, 290, String(localHumidity));


}

void Draw_Heading_Section()
{
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(SCREEN_WIDTH / 2, -2, City);
  gfx.setFont(ArialMT_Plain_10);
  gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
  gfx.drawString(SCREEN_WIDTH - 3, 0, Day_time_str);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(5, 0, time_str);
  gfx.drawLine(0, 15, SCREEN_WIDTH, 15);
}

void Draw_Main_Weather_Section(int x, int y)
{
  DisplayWXicon(x + 5, y - 5, WxConditions[0].Icon, LargeIcon);
  gfx.setFont(ArialRoundedMTBold_14);
  DrawPressureTrend(x, y + 50, WxConditions[0].Pressure, WxConditions[0].Trend);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  Draw_Rain(x - 100, y + 35);
  gfx.setFont(ArialMT_Plain_24);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  String Wx_Description = WxConditions[0].Main0;
  if (WxConditions[0].Forecast0 != "")
  {
    Wx_Description += " (" + WxConditions[0].Forecast0;
    if (WxConditions[0].Forecast1 != "" && WxConditions[0].Forecast1 != WxConditions[0].Forecast2) Wx_Description += " & " + WxConditions[0].Forecast1 + ")";
    else Wx_Description += ")";
  }
  gfx.drawString(x - 170, y + 70, TitleCase(Wx_Description));
  Draw_Main_Wx(x - 98, y - 1);
  gfx.drawLine(0, y + 68, SCREEN_WIDTH, y + 68);
}

String TitleCase(String text)
{
  if (text.length() > 0)
  {
    String temp_text = text.substring(0, 1);
    temp_text.toUpperCase();
    return temp_text + text.substring(1); // Title-case the string
  }
  return "";
}

void Draw_Forecast_Section(int x, int y)
{
  gfx.setFont(ArialMT_Plain_10);
  Draw_Forecast_Weather(x, y, 0);
  Draw_Forecast_Weather(x + 56, y, 1);
  Draw_Forecast_Weather(x + 112, y, 2);
  //       (x,y,width,height,MinValue, MaxValue, Title, Data Array, AutoScale, ChartMode)
  for (int r = 1; r <= max_readings; r++)
  {
    if (Units == "I") pressure_readings[r] = WxForecast[r].Pressure * 0.02953;
    else              pressure_readings[r] = WxForecast[r].Pressure;
    temperature_readings[r] = WxForecast[r].Temperature;
    if (Units == "I") rain_readings[r] = WxForecast[r].Rainfall * 0.0393701;
    else              rain_readings[r] = WxForecast[r].Rainfall;
  }
  gfx.drawLine(0, y + 173, SCREEN_WIDTH, y + 173);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.drawString(x - 40, y + 173, "3-Day Forecast Values");
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setFont(ArialMT_Plain_10);
  DrawGraph(SCREEN_WIDTH / 400 * 30, SCREEN_HEIGHT / 300 * 222, SCREEN_WIDTH / 400 * 100, SCREEN_HEIGHT / 300 * 60, 900, 1050, "Pressure", pressure_readings, max_readings, autoscale_on, barchart_off);
  DrawGraph(SCREEN_WIDTH / 400 * 158, SCREEN_HEIGHT / 300 * 222, SCREEN_WIDTH / 400 * 100, SCREEN_HEIGHT / 300 * 60, 10, 30, "Temperature", temperature_readings, max_readings, autoscale_on, barchart_off);
  DrawGraph(SCREEN_WIDTH / 400 * 288, SCREEN_HEIGHT / 300 * 222, SCREEN_WIDTH / 400 * 100, SCREEN_HEIGHT / 300 * 60, 0, 30, "Rainfall", rain_readings, max_readings, autoscale_on, barchart_on);
}

void Draw_Forecast_Weather(int x, int y, int index)
{
  gfx.setFont(ArialMT_Plain_10);
  gfx.setColor(EPD_BLACK); // Sometimes gets set to WHITE, so change back
  gfx.drawRect(x, y, 55, 65);
  gfx.drawLine(x + 1, y + 13, x + 55, y + 13);
  DisplayWXicon(x + 28, y + 35, WxForecast[index].Icon, SmallIcon);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setFont(ArialMT_Plain_10);
  gfx.drawString(x + 28, y, String(WxForecast[index].Period.substring(11, 16)));
  gfx.drawString(x + 28, y + 50, String(WxForecast[index].High, 0) + "� / " + String(WxForecast[index].Low, 0) + "�");
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

void DrawWind(int x, int y, float angle, float windspeed)
{
  int Cradius = 44;
  //float dx = Cradius * cos((angle - 90) * PI / 180) + x; // calculate X position
  //float dy = Cradius * sin((angle - 90) * PI / 180) + y; // calculate Y position
  arrow(x, y, Cradius - 3, angle, 15, 15); // Show wind direction on outer circle
  gfx.drawCircle(x, y, Cradius + 2);
  gfx.drawCircle(x, y, Cradius + 3);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.drawString(x, y + Cradius - 25, WindDegToDirection(angle));
  gfx.setFont(ArialMT_Plain_10);
  gfx.drawString(x - Cradius + 3, y - Cradius - 6, String(windspeed, 1) + (Units == "M" ? " m/s" : " mph"));
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
}

String WindDegToDirection(float winddirection)
{
  if (winddirection >= 348.75 || winddirection < 11.25)  return "N";
  if (winddirection >= 11.25 && winddirection < 33.75)  return "NNE";
  if (winddirection >= 33.75 && winddirection < 56.25)  return "NE";
  if (winddirection >= 56.25 && winddirection < 78.75)  return "ENE";
  if (winddirection >= 78.75 && winddirection < 101.25) return "E";
  if (winddirection >= 101.25 && winddirection < 123.75) return "ESE";
  if (winddirection >= 123.75 && winddirection < 146.25) return "SE";
  if (winddirection >= 146.25 && winddirection < 168.75) return "SSE";
  if (winddirection >= 168.75 && winddirection < 191.25) return "S";
  if (winddirection >= 191.25 && winddirection < 213.75) return "SSW";
  if (winddirection >= 213.75 && winddirection < 236.25) return "SW";
  if (winddirection >= 236.25 && winddirection < 258.75) return "WSW";
  if (winddirection >= 258.75 && winddirection < 281.25) return "W";
  if (winddirection >= 281.25 && winddirection < 303.75) return "WNW";
  if (winddirection >= 303.75 && winddirection < 326.25) return "NW";
  if (winddirection >= 326.25 && winddirection < 348.75) return "NNW";
  return "?";
}


int JulianDate(int d, int m, int y)
{
  int mm, yy, k1, k2, k3, j;
  yy = y - (int)((12 - m) / 10);
  mm = m + 9;
  if (mm >= 12) mm = mm - 12;
  k1 = (int)(365.25 * (yy + 4712));
  k2 = (int)(30.6001 * mm + 0.5);
  k3 = (int)((int)((yy / 100) + 49) * 0.75) - 38;
  // 'j' for dates in Julian calendar:
  j = k1 + k2 + d + 59 + 1;
  if (j > 2299160) j = j - k3; // 'j' is the Julian date at 12h UT (Universal Time) For Gregorian calendar:
  return j;
}

String MoonPhase(int d, int m, int y, String hemisphere)
{
  int c, e;
  double jd;
  int b;
  if (m < 3)
  {
    y--;
    m += 12;
  }
  ++m;
  c = 365.25 * y;
  e = 30.6 * m;
  jd = c + e + d - 694039.09;           /* jd is total days elapsed */
  jd /= 29.53059;                        /* divide by the moon cycle (29.53 days) */
  b = jd;                              /* int(jd) -> b, take integer part of jd */
  jd -= b;                               /* subtract integer part to leave fractional part of original jd */
  b = jd * 8 + 0.5;                    /* scale fraction from 0-8 and round by adding 0.5 */
  b = b & 7;                           /* 0 and 8 are the same phase so modulo 8 for 0 */
  if (hemisphere == "south") b = 7 - b;
  if (b == 0) return "New";              // New; 0% illuminated
  if (b == 1) return "Waxing crescent";  // Waxing crescent; 25% illuminated
  if (b == 2) return "First quarter";    // First quarter; 50% illuminated
  if (b == 3) return "Waxing gibbous";   // Waxing gibbous; 75% illuminated
  if (b == 4) return "Full";             // Full; 100% illuminated
  if (b == 5) return "Waning gibbous";   // Waning gibbous; 75% illuminated
  if (b == 6) return "Third quarter";    // Last quarter; 50% illuminated
  if (b == 7) return "Waning crescent";  // Waning crescent; 25% illuminated
  return "";
}

void DrawCircle(int x, int y, int Cstart, int Cend, int Cradius, int Coffset_radius, int Coffset)
{
  gfx.setColor(EPD_BLACK);
  float dx, dy;
  for (int i = Cstart; i < Cend; i++)
  {
    dx = (Cradius + Coffset_radius) * cos((i - 90) * PI / 180) + x + Coffset / 2; // calculate X position
    dy = (Cradius + Coffset_radius) * sin((i - 90) * PI / 180) + y; // calculate Y position
    gfx.setPixel(dx, dy);
  }
}

void arrow(int x, int y, int asize, float aangle, int pwidth, int plength)
{
  float dx = (asize - 10) * cos((aangle - 90) * PI / 180) + x; // calculate X position
  float dy = (asize - 10) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
  float x1 = 0;         float y1 = plength;
  float x2 = pwidth / 2;  float y2 = pwidth / 2;
  float x3 = -pwidth / 2; float y3 = pwidth / 2;
  float angle = aangle * PI / 180 - 135;
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
  gfx.fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2);
}

void DisplayWXicon(int x, int y, String IconName, bool LargeSize)
{
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  Serial.println(IconName);
  if (IconName == "01d" || IconName == "01n")  Sunny(x, y, LargeSize, IconName);
  else if (IconName == "02d" || IconName == "02n")  MostlySunny(x, y, LargeSize, IconName);
  else if (IconName == "03d" || IconName == "03n")  Cloudy(x, y, LargeSize, IconName);
  else if (IconName == "04d" || IconName == "04n")  MostlySunny(x, y, LargeSize, IconName);
  else if (IconName == "09d" || IconName == "09n")  ChanceRain(x, y, LargeSize, IconName);
  else if (IconName == "10d" || IconName == "10n")  Rain(x, y, LargeSize, IconName);
  else if (IconName == "11d" || IconName == "11n")  Tstorms(x, y, LargeSize, IconName);
  else if (IconName == "13d" || IconName == "13n")  Snow(x, y, LargeSize, IconName);
  else if (IconName == "50d")                       Haze(x, y - 5, LargeSize, IconName);
  else if (IconName == "50n")                       Fog(x, y - 5, LargeSize, IconName);
  else                                              Nodata(x, y, LargeSize);
}

bool obtain_wx_data(String RequestType)
{
  rxtext = "";
  String units = (Units == "M" ? "metric" : "imperial");
  client.stop(); // close connection before sending a new request
  if (client.connect(server, 80))
  { // if the connection succeeds
// Serial.println("connecting...");
// send the HTTP PUT request:
    if (RequestType == "weather")
      client.println("GET /data/2.5/" + RequestType + "?q=" + City + "," + Country + "&APPID=" + apikey + "&mode=json&units=" + units + "&lang=" + Language + " HTTP/1.1");
    else
      client.println("GET /data/2.5/" + RequestType + "?q=" + City + "," + Country + "&APPID=" + apikey + "&mode=json&units=" + units + "&lang=" + Language + "&cnt=24 HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ESP OWM Receiver/1.1");
    client.println("Connection: close");
    client.println();
    // Check HTTP status
    char status[32] = { 0 };
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") != 0)
    {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return 0;
    }
    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
      Serial.println(F("Invalid response"));
      return 0;
    }
    DecodeWeather(client, RequestType);
    client.stop();
    return true;
  }
  else
  { // if no connection was made
    Serial.println("connection failed");
    return false;
  }
  return true;
}

// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/
bool DecodeWeather(Stream &json_stream, String Type)
{
  Serial.print("Creating object and ");
  DynamicJsonDocument doc(5 * 1024);
  DeserializationError error = deserializeJson(doc, json_stream);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // Extract values
  JsonObject root = doc.as<JsonObject>();
  Serial.println(" Decoding " + Type + " data");
  if (Type == "weather")
  {
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed 
    WxConditions[0].lon = root["coord"]["lon"].as<float>();              Serial.println(WxConditions[0].lon);
    WxConditions[0].lat = root["coord"]["lat"].as<float>();              Serial.println(WxConditions[0].lat);
    WxConditions[0].Main0 = root["weather"][0]["main"].as<char*>();        Serial.println(WxConditions[0].Main0);
    WxConditions[0].Forecast0 = root["weather"][0]["description"].as<char*>(); Serial.println(WxConditions[0].Forecast0);
    WxConditions[0].Icon = root["weather"][0]["icon"].as<char*>();        Serial.println(WxConditions[0].Icon);
    WxConditions[0].Forecast1 = root["weather"][1]["main"].as<char*>();        Serial.println(WxConditions[0].Forecast1);
    WxConditions[0].Forecast2 = root["weather"][2]["main"].as<char*>();        Serial.println(WxConditions[0].Forecast2);
    WxConditions[0].Temperature = root["main"]["temp"].as<float>();              Serial.println(WxConditions[0].Temperature);
    WxConditions[0].Pressure = root["main"]["pressure"].as<float>();          Serial.println(WxConditions[0].Pressure);
    WxConditions[0].Humidity = root["main"]["humidity"].as<float>();          Serial.println(WxConditions[0].Humidity);
    WxConditions[0].Low = root["main"]["temp_min"].as<float>();          Serial.println(WxConditions[0].Low);
    WxConditions[0].High = root["main"]["temp_max"].as<float>();          Serial.println(WxConditions[0].High);
    WxConditions[0].Windspeed = root["wind"]["speed"].as<float>();             Serial.println(WxConditions[0].Windspeed);
    WxConditions[0].Winddir = root["wind"]["deg"].as<float>();               Serial.println(WxConditions[0].Winddir);
    WxConditions[0].Cloudcover = root["clouds"]["all"].as<int>();               Serial.println(WxConditions[0].Cloudcover); // in % of cloud cover
    WxConditions[0].Rainfall = root["rain"]["3h"].as<float>();                Serial.println(WxConditions[0].Rainfall);   // in mm of rain
    WxConditions[0].Snowfall = root["snow"]["3h"].as<float>();                Serial.println(WxConditions[0].Snowfall);   // in mm of snow
    WxConditions[0].Visibility = root["visibility"].as<int>();                  Serial.println(WxConditions[0].Visibility); // in metres
    WxConditions[0].Country = root["sys"]["country"].as<char*>();            Serial.println(WxConditions[0].Country);
    WxConditions[0].Sunrise = root["sys"]["sunrise"].as<int>();              Serial.println(WxConditions[0].Sunrise);
    WxConditions[0].Sunset = root["sys"]["sunset"].as<int>();               Serial.println(WxConditions[0].Sunset);
  }
  if (Type == "forecast")
  {
    //Serial.println(json);
    //const char* cod = root["cod"]; // "200"
    //float message = root["message"];
    //int cnt = root["cnt"];
    JsonArray list = root["list"];
    Serial.print("\nReceiving Forecast period - "); //------------------------------------------------
    for (byte r = 0; r < max_readings; r++)
    {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      WxForecast[r].Dt = list[r]["dt"].as<char*>();
      WxForecast[r].Temperature = list[r]["main"]["temp"].as<float>();              Serial.println(WxForecast[r].Temperature);
      WxForecast[r].Low = list[r]["main"]["temp_min"].as<float>();          Serial.println(WxForecast[r].Low);
      WxForecast[r].High = list[r]["main"]["temp_max"].as<float>();          Serial.println(WxForecast[r].High);
      WxForecast[r].Pressure = list[r]["main"]["pressure"].as<float>();          Serial.println(WxForecast[r].Pressure);
      WxForecast[r].Humidity = list[r]["main"]["humidity"].as<float>();          Serial.println(WxForecast[r].Humidity);
      WxForecast[r].Forecast0 = list[r]["weather"][0]["main"].as<char*>();        Serial.println(WxForecast[r].Forecast0);
      WxForecast[r].Forecast0 = list[r]["weather"][1]["main"].as<char*>();        Serial.println(WxForecast[r].Forecast1);
      WxForecast[r].Forecast0 = list[r]["weather"][2]["main"].as<char*>();        Serial.println(WxForecast[r].Forecast2);
      WxForecast[r].Description = list[r]["weather"][0]["description"].as<char*>(); Serial.println(WxForecast[r].Description);
      WxForecast[r].Icon = list[r]["weather"][0]["icon"].as<char*>();        Serial.println(WxForecast[r].Icon);
      WxForecast[r].Cloudcover = list[r]["clouds"]["all"].as<int>();               Serial.println(WxForecast[0].Cloudcover); // in % of cloud cover
      WxForecast[r].Windspeed = list[r]["wind"]["speed"].as<float>();             Serial.println(WxForecast[r].Windspeed);
      WxForecast[r].Winddir = list[r]["wind"]["deg"].as<float>();               Serial.println(WxForecast[r].Winddir);
      WxForecast[r].Rainfall = list[r]["rain"]["3h"].as<float>();                Serial.println(WxForecast[r].Rainfall);
      WxForecast[r].Snowfall = list[r]["snow"]["3h"].as<float>();                Serial.println(WxForecast[r].Snowfall);
      WxForecast[r].Period = list[r]["dt_txt"].as<char*>();                    Serial.println(WxForecast[r].Period);
    }
    //------------------------------------------
    float pressure_trend = WxForecast[0].Pressure - WxForecast[1].Pressure; // Measure pressure slope between ~now and later
    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
    WxConditions[0].Trend = "0";
    if (pressure_trend > 0)  WxConditions[0].Trend = "+";
    if (pressure_trend < 0)  WxConditions[0].Trend = "-";
    if (pressure_trend == 0) WxConditions[0].Trend = "0";

    if (Units == "I") Convert_Readings_to_Imperial();
  }
  return true;
}

void Convert_Readings_to_Imperial()
{
  WxConditions[0].Pressure = WxConditions[0].Pressure * 0.02953; //  hPa to ins
  WxForecast[1].Rainfall = WxForecast[1].Rainfall * 0.0393701; // mm to inches of rainfall
}

int StartWiFi()
{
  int connAttempts = 0;
  Serial.print(F("\r\nConnecting to: ")); Serial.println(String(ssid1));
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid1, password1);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500); Serial.print(".");
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

void SetupTime()
{
  configTime(0, 0, "0.uk.pool.ntp.org", "time.nist.gov");
  setenv("TZ", Timezone, 1);
  delay(500);
  UpdateLocalTime();
}

void UpdateLocalTime()
{
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo))
  {
    Serial.println(F("Failed to obtain time"));
  }
  //See http://www.cplusplus.com/reference/ctime/strftime/
  //Serial.println(&timeinfo, "%a %b %d %Y   %H:%M:%S");     // Displays: Saturday, June 24 2017 14:05:49
  Serial.println(&timeinfo, "%H:%M:%S");                     // Displays: 14:05:49
  char output[30], day_output[30];
  if (Units == "M")
  {
    strftime(day_output, 30, "%a  %d-%b", &timeinfo);     // Displays: Sat 24-Jun-17
    strftime(output, 30, "@ %H:%M", &timeinfo);        // Creates: '@ 14:05'
  }
  else
  {
    strftime(day_output, 30, "%a  %b-%d-%y", &timeinfo);     // Creates: Sat Jun-24-17
    strftime(output, 30, "(@ %r )", &timeinfo);              // Creates: '@ 2:05:49pm'
  }
  Day_time_str = day_output;
  time_str = output;
}
