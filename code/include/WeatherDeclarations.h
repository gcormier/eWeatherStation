#pragma once

bool obtain_wx_data(String RequestType);
float getBatteryVoltage();
void UpdateLocalTime();
void Convert_Readings_to_Imperial();
bool DecodeWeather(Stream &json_stream, String Type);
int JulianDate(int d, int m, int y);
String WindDegToDirection(float winddirection);
void arrow(int x, int y, int asize, float aangle, int pwidth, int plength);
void DrawWind(int x, int y, float angle, float windspeed);
void DisplayWXicon(int x, int y, String IconName, bool LargeSize);
void Draw_Forecast_Section(int x, int y);
void Draw_Forecast_Weather(int x, int y, int index);
void Draw_Main_Wx(int x, int y);
String TitleCase(String text);
void Draw_Main_Weather_Section(int x, int y);
void DrawMoon(int x, int y, int dd, int mm, int yy, String hemisphere);
String MoonPhase(int d, int m, int y, String hemisphere);

#define SCREEN_WIDTH  400.0    // Set for landscape mode, don't remove the decimal place!
#define SCREEN_HEIGHT 300.0
#define BITS_PER_PIXEL 1
#define EPD_BLACK 0
#define EPD_WHITE 1
uint16_t palette[] = { 0, 1 };

// pins_arduino.h, e.g. LOLIN32 LITE
static const uint8_t EPD_BUSY = 4;
static const uint8_t EPD_SS = 5;
static const uint8_t EPD_RST = 16;
static const uint8_t EPD_DC = 17;
static const uint8_t EPD_SCK = 18;
static const uint8_t EPD_MISO = 19; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 23;
typedef WebServer WEBServer;
EPD_WaveShare42 epd(EPD_SS, EPD_RST, EPD_DC, EPD_BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);

const unsigned long UpdateInterval = (30L * 60L - 3) * 1000000L; // Delay between updates, in microseconds, WU allows 500 requests per-day maximum, set to every 15-mins or more
bool LargeIcon = true;
bool SmallIcon = false;
#define Large  10
#define Small  4
String time_str, Day_time_str, rxtext; // strings to hold time and received weather data;
int    wifisection, displaysection, MoonDay, MoonMonth, MoonYear;
int    Sunrise, Sunset;
typedef struct { // For current Day and Day 1, 2, 3, etc
  String   Dt;
  String   Period;
  String   Trend;
  String   Main0;
  String   Forecast0;
  String   Forecast1;
  String   Forecast2;
  String   Description;
  String   Time;
  String   Icon;
  String   Country;
  float    lat;
  float    lon;
  float    Temperature;
  float    Humidity;
  float    High;
  float    Low;
  float    Winddir;
  float    Windspeed;
  float    Rainfall;
  float    Snowfall;
  float    Pressure;
  int      Cloudcover;
  int      Visibility;
  int      Sunrise;
  int      Sunset;
} Forecast_record_type;

#define max_readings 24

Forecast_record_type  WxConditions[1];
Forecast_record_type  WxForecast[max_readings];

#define autoscale_on  true
#define autoscale_off false
#define barchart_on   true
#define barchart_off  false

float pressure_readings[max_readings] = { 0 };
float temperature_readings[max_readings] = { 0 };
float rain_readings[max_readings] = { 0 };
