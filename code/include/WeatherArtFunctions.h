#pragma once

#include <WeatherDeclarations.h>

void addcloud(int x, int y, int scale, int linesize);
String ConvertUnixTime(int unix_time);
void addrain(int x, int y, int scale);
void addsnow(int x, int y, int scale);
void addtstorm(int x, int y, int scale);
void addsun(int x, int y, int scale);
void addfog(int x, int y, int scale, int linesize);
void MostlyCloudy(int x, int y, bool LargeSize, String IconName);
void MostlySunny(int x, int y, bool LargeSize, String IconName);
void Rain(int x, int y, bool LargeSize, String IconName);
void Cloudy(int x, int y, bool LargeSize, String IconName);
void Sunny(int x, int y, bool LargeSize, String IconName);
void ExpectRain(int x, int y, bool LargeSize, String IconName);
void ChanceRain(int x, int y, bool LargeSize, String IconName);
void Tstorms(int x, int y, bool LargeSize, String IconName);
void Snow(int x, int y, bool LargeSize, String IconName);
void Fog(int x, int y, bool LargeSize, String IconName);
void Haze(int x, int y, bool LargeSize, String IconName);
void addmoon(int x, int y, int scale);


String ConvertUnixTime(int unix_time)
{
  struct tm *now_tm;
  int hour, min, second, day, month, year, wday;
  // timeval tv = {unix_time,0};
  time_t tm = unix_time;
  now_tm = localtime(&tm);
  hour = now_tm->tm_hour;
  min = now_tm->tm_min;
  second = now_tm->tm_sec;
  wday = now_tm->tm_wday;
  day = now_tm->tm_mday;
  month = now_tm->tm_mon + 1;
  year = 1900 + now_tm->tm_year; // To get just YY information
  MoonDay = day;
  MoonMonth = month;
  MoonYear = year;
  if (Units == "M")
  {
    time_str = (hour < 10 ? "0" + String(hour) : String(hour)) + ":" + (min < 10 ? "0" + String(min) : String(min)) + ":" + "  ";                     // HH:MM   05/07/17
    time_str += (day < 10 ? "0" + String(day) : String(day)) + "/" + (month < 10 ? "0" + String(month) : String(month)) + "/" + (year < 10 ? "0" + String(year) : String(year)); // HH:MM   05/07/17
  }
  else
  {
    String ampm = "am";
    if (hour > 11) ampm = "pm";
    hour = hour % 12; if (hour == 0) hour = 12;
    time_str = (hour % 12 < 10 ? "0" + String(hour % 12) : String(hour % 12)) + ":" + (min < 10 ? "0" + String(min) : String(min)) + ampm + " ";      // HH:MMam 07/05/17
    time_str += (month < 10 ? "0" + String(month) : String(month)) + "/" + (day < 10 ? "0" + String(day) : String(day)) + "/" + "/" + (year < 10 ? "0" + String(year) : String(year)); // HH:MMpm 07/05/17
  }
  // Returns either '21:12  ' or ' 09:12pm' depending on Units
  //Serial.println(time_str);
  return time_str;
}
// Symbols are drawn on a relative 10x10grid and 1 scale unit = 1 drawing unit
void addcloud(int x, int y, int scale, int linesize)
{
  //Draw cloud outer
  gfx.fillCircle(x - scale * 3, y, scale);                       // Left most circle
  gfx.fillCircle(x + scale * 3, y, scale);                       // Right most circle
  gfx.fillCircle(x - scale, y - scale, scale * 1.4);             // left middle upper circle
  gfx.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75); // Right middle upper circle
  gfx.fillRect(x - scale * 3 - 1, y - scale, scale * 6, scale * 2 + 1); // Upper and lower lines
  //Clear cloud inner
  gfx.setColor(EPD_WHITE);
  gfx.fillCircle(x - scale * 3, y, scale - linesize);            // Clear left most circle
  gfx.fillCircle(x + scale * 3, y, scale - linesize);            // Clear right most circle
  gfx.fillCircle(x - scale, y - scale, scale * 1.4 - linesize);  // left middle upper circle
  gfx.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75 - linesize); // Right middle upper circle
  gfx.fillRect(x - scale * 3 + 2, y - scale + linesize - 1, scale * 5.9, scale * 2 - linesize * 2 + 2); // Upper and lower lines
  gfx.setColor(EPD_BLACK);
}
void addrain(int x, int y, int scale)
{
  y = y + scale / 2;
  for (int i = 0; i < 6; i++)
  {
    gfx.drawLine(x - scale * 4 + scale * i * 1.3 + 0, y + scale * 1.9, x - scale * 3.5 + scale * i * 1.3 + 0, y + scale);
    if (scale != Small)
    {
      gfx.drawLine(x - scale * 4 + scale * i * 1.3 + 1, y + scale * 1.9, x - scale * 3.5 + scale * i * 1.3 + 1, y + scale);
      gfx.drawLine(x - scale * 4 + scale * i * 1.3 + 2, y + scale * 1.9, x - scale * 3.5 + scale * i * 1.3 + 2, y + scale);
    }
  }
}
void addsnow(int x, int y, int scale)
{
  int dxo, dyo, dxi, dyi;
  for (int flakes = 0; flakes < 5; flakes++)
  {
    for (int i = 0; i < 360; i = i + 45)
    {
      dxo = 0.5 * scale * cos((i - 90) * 3.14 / 180); dxi = dxo * 0.1;
      dyo = 0.5 * scale * sin((i - 90) * 3.14 / 180); dyi = dyo * 0.1;
      gfx.drawLine(dxo + x + 0 + flakes * 1.5 * scale - scale * 3, dyo + y + scale * 2, dxi + x + 0 + flakes * 1.5 * scale - scale * 3, dyi + y + scale * 2);
    }
  }
}
void addtstorm(int x, int y, int scale)
{
  y = y + scale / 2;
  for (int i = 0; i < 5; i++)
  {
    gfx.drawLine(x - scale * 4 + scale * i * 1.5 + 0, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 0, y + scale);
    if (scale != Small)
    {
      gfx.drawLine(x - scale * 4 + scale * i * 1.5 + 1, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 1, y + scale);
      gfx.drawLine(x - scale * 4 + scale * i * 1.5 + 2, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 2, y + scale);
    }
    gfx.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 0, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 0);
    if (scale != Small)
    {
      gfx.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 1, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 1);
      gfx.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 2, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 2);
    }
    gfx.drawLine(x - scale * 3.5 + scale * i * 1.4 + 0, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5);
    if (scale != Small)
    {
      gfx.drawLine(x - scale * 3.5 + scale * i * 1.4 + 1, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 1, y + scale * 1.5);
      gfx.drawLine(x - scale * 3.5 + scale * i * 1.4 + 2, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 2, y + scale * 1.5);
    }
  }
}
void addsun(int x, int y, int scale)
{
  int linesize = 3;
  if (scale == Small) linesize = 1;
  int dxo, dyo, dxi, dyi;
  gfx.fillCircle(x, y, scale);
  gfx.setColor(EPD_WHITE);
  gfx.fillCircle(x, y, scale - linesize);
  gfx.setColor(EPD_BLACK);
  for (float i = 0; i < 360; i = i + 45)
  {
    dxo = 2.2 * scale * cos((i - 90) * 3.14 / 180); dxi = dxo * 0.6;
    dyo = 2.2 * scale * sin((i - 90) * 3.14 / 180); dyi = dyo * 0.6;
    if (i == 0 || i == 180)
    {
      gfx.drawLine(dxo + x - 1, dyo + y, dxi + x - 1, dyi + y);
      if (scale != Small)
      {
        gfx.drawLine(dxo + x + 0, dyo + y, dxi + x + 0, dyi + y);
        gfx.drawLine(dxo + x + 1, dyo + y, dxi + x + 1, dyi + y);
      }
    }
    if (i == 90 || i == 270)
    {
      gfx.drawLine(dxo + x, dyo + y - 1, dxi + x, dyi + y - 1);
      if (scale != Small)
      {
        gfx.drawLine(dxo + x, dyo + y + 0, dxi + x, dyi + y + 0);
        gfx.drawLine(dxo + x, dyo + y + 1, dxi + x, dyi + y + 1);
      }
    }
    if (i == 45 || i == 135 || i == 225 || i == 315)
    {
      gfx.drawLine(dxo + x - 1, dyo + y, dxi + x - 1, dyi + y);
      if (scale != Small)
      {
        gfx.drawLine(dxo + x + 0, dyo + y, dxi + x + 0, dyi + y);
        gfx.drawLine(dxo + x + 1, dyo + y, dxi + x + 1, dyi + y);
      }
    }
  }
}
void addfog(int x, int y, int scale, int linesize)
{
  if (scale == Small) y -= 10;
  if (scale == Small) linesize = 1;
  for (int i = 0; i < 6; i++)
  {
    gfx.fillRect(x - scale * 3, y + scale * 1.5, scale * 6, linesize);
    gfx.fillRect(x - scale * 3, y + scale * 2.0, scale * 6, linesize);
    gfx.fillRect(x - scale * 3, y + scale * 2.7, scale * 6, linesize);
  }
}
void MostlyCloudy(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addcloud(x, y, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale);
  addcloud(x, y, scale, linesize);
}
void MostlySunny(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addcloud(x, y, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale);
}

void Rain(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale);
}

void Cloudy(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small)
  {
    if (IconName.endsWith("n")) addmoon(x, y, scale);
    linesize = 1;
    addcloud(x, y, scale, linesize);
  }
  else
  {
    y += 25;
    if (IconName.endsWith("n")) addmoon(x, y - 15, scale);
    addcloud(x + 30, y - 35, 4, linesize); // Cloud top right
    addcloud(x - 20, y - 25, 6, linesize); // Cloud top left
    addcloud(x, y, scale, linesize);   // Main cloud
  }
}

void Sunny(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  scale = scale * 1.5;
  addsun(x, y, scale);
}

void ExpectRain(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addsun(x - scale * 1.8, y - scale * 1.8, scale);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale);
}

void ChanceRain(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addsun(x - scale * 1.8, y - scale * 1.8, scale);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale);
}

void Tstorms(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addcloud(x, y, scale, linesize);
  addtstorm(x, y, scale);
}

void Snow(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addcloud(x, y, scale, linesize);
  addsnow(x, y, scale);
}

void Fog(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addcloud(x, y, scale, linesize);
  addfog(x, y, scale, linesize);
}

void Haze(int x, int y, bool LargeSize, String IconName)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  int linesize = 3;
  if (scale == Small) linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y, scale);
  addsun(x, y, scale*1.4);
  addfog(x, y, scale*1.4, linesize);
}

void addmoon(int x, int y, int scale)
{
  if (scale == Large)
  {
    gfx.fillCircle(x - 37, y - 33, scale);
    gfx.setColor(EPD_WHITE);
    gfx.fillCircle(x - 27, y - 33, scale*1.6);
    gfx.setColor(EPD_BLACK);
  }
  else
  {
    gfx.fillCircle(x - 20, y - 15, scale);
    gfx.setColor(EPD_WHITE);
    gfx.fillCircle(x - 15, y - 15, scale*1.6);
    gfx.setColor(EPD_BLACK);
  }
}

void Nodata(int x, int y, bool LargeSize)
{
  int scale = Small;
  if (LargeSize) scale = Large;
  if (scale == Large) gfx.setFont(ArialMT_Plain_24); else gfx.setFont(ArialMT_Plain_16);
  gfx.drawString(x, y - 10, "N/A");
}

void DrawBattery(int x, int y)
{
  uint8_t percentage = 100;
  float voltage = getBatteryVoltage();
  
  if (voltage > 1)
  {
    if (voltage >= 4.20) percentage = 100;
    else if (voltage < 3.70) percentage = 0;
    else percentage = (voltage - 3.70) * 100 / (4.20 - 3.70);
    gfx.setColor(EPD_BLACK);
    gfx.setFont(ArialMT_Plain_10);
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    gfx.drawString(x - 25, y, String(voltage, 2) + "V");
    gfx.drawRect(x - 22, y + 2, 19, 9);
    gfx.fillRect(x - 2, y + 4, 3, 5);
    gfx.fillRect(x - 20, y + 4, 17 * percentage / 100.0, 6);
  }
}

/* (C) D L BIRD
    This function will draw a graph on a ePaper/TFT/LCD display using data from an array containing data to be graphed.
    The variable 'max_readings' determines the maximum number of data elements for each array. Call it with the following parametric data:
    x_pos - the x axis top-left position of the graph
    y_pos - the y-axis top-left position of the graph, e.g. 100, 200 would draw the graph 100 pixels along and 200 pixels down from the top-left of the screen
    width - the width of the graph in pixels
    height - height of the graph in pixels
    Y1_Max - sets the scale of plotted data, for example 5000 would scale all data to a Y-axis of 5000 maximum
    data_array1 is parsed by value, externally they can be called anything else, e.g. within the routine it is called data_array1, but externally could be temperature_readings
    auto_scale - a logical value (TRUE or FALSE) that switches the Y-axis autoscale On or Off
    barchart_on - a logical value (TRUE or FALSE) that switches the drawing mode between barhcart and line graph
    barchart_colour - a sets the title and graph plotting colour
    If called with Y!_Max value of 500 and the data never goes above 500, then autoscale will retain a 0-500 Y scale, if on, the scale increases/decreases to match the data.
    auto_scale_margin, e.g. if set to 1000 then autoscale increments the scale by 1000 steps.
*/
void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode)
{
#define auto_scale_margin 0 // Sets the autoscale increment, so axis steps up in units of e.g. 3
#define y_minor_axis 5      // 5 y-axis division markers
  int maxYscale = -10000;
  int minYscale = 10000;
  int last_x, last_y;
  float x1, y1, x2, y2;
  if (auto_scale == true)
  {
    for (int i = 1; i < readings; i++)
    {
      if (DataArray[i] >= maxYscale) maxYscale = DataArray[i];
      if (DataArray[i] <= minYscale) minYscale = DataArray[i];
    }
    maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
    Y1Max = round(maxYscale + 0.5);
    if (minYscale != 0) minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
    Y1Min = round(minYscale);
  }
  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[1], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight;
  gfx.setColor(EPD_BLACK);
  gfx.drawRect(x_pos, y_pos, gwidth + 3, gheight + 2);
  gfx.setFont(ArialMT_Plain_10);
  //gfx.setFont(ArialRoundedMTBold_14);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(x_pos + gwidth / 2, y_pos - 17, title);
  gfx.setFont(ArialMT_Plain_10);
  gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
  // Draw the data
  for (int gx = 1; gx < readings; gx++)
  {
    x1 = last_x;
    y1 = last_y;
    x2 = x_pos + gx * gwidth / (readings - 1) - 1; // max_readings is the global variable that sets the maximum data that can be plotted
    y2 = y_pos + (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
    if (barchart_mode)
    {
      gfx.fillRect(x2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 1);
    }
    else
    {
      gfx.drawLine(last_x, last_y, x2, y2);
    }
    last_x = x2;
    last_y = y2;
  }
  //Draw the Y-axis scale
  for (int spacing = 0; spacing <= y_minor_axis; spacing++)
  {
#define number_of_dashes 20
    for (int j = 0; j < number_of_dashes; j++)
    { // Draw dashed graph grid lines
      if (spacing < y_minor_axis) gfx.drawHorizontalLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes));
    }
    if ((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing) < 10) { gfx.drawString(x_pos - 2, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1)); }
    else
    {
      if (Y1Min < 1) gfx.drawString(x_pos - 2, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1));
      else gfx.drawString(x_pos - 2, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 0)); // +0.01 prevents -0.00 occurring
    }
  }
  for (int i = 0; i <= 3; i++)
  {
    gfx.drawString(5 + x_pos + gwidth / 3 * i, y_pos + gheight + 3, String(i));
  }
  gfx.drawString(x_pos + gwidth / 2 + 12, y_pos + gheight + 5, "Days");
}


void DrawPressureTrend(int x, int y, float pressure, String slope)
{
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(x, y, String(pressure, 1) + (Units == "M" ? "mb" : "in"));
  x = x + 45; y = y + 8;
  if (slope == "+")
  {
    gfx.drawLine(x, y, x + 4, y - 4);
    gfx.drawLine(x + 4, y - 4, x + 8, y);
  }
  else if (slope == "0")
  {
    gfx.drawLine(x + 3, y - 4, x + 8, y);
    gfx.drawLine(x + 3, y + 4, x + 8, y);
  }
  else if (slope == "-")
  {
    gfx.drawLine(x, y, x + 4, y + 4);
    gfx.drawLine(x + 4, y + 4, x + 8, y);
  }
}

void Draw_Rain(int x, int y)
{
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  if (WxForecast[1].Rainfall > 0) gfx.drawString(x, y + 14, String(WxForecast[1].Rainfall, 3) + (Units == "M" ? "mm" : "in") + " Rainfall"); // Only display rainfall total today if > 0
  gfx.setFont(ArialMT_Plain_10);
}

void Draw_Astronomy_Section(int x, int y)
{
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setFont(ArialMT_Plain_10);
  gfx.drawRect(x, y + 64, 167, 53);
  gfx.drawString(x + 4, y + 65, "Sun Rise/Set");
  gfx.drawString(x + 20, y + 75, ConvertUnixTime(WxConditions[0].Sunrise).substring(0, 5));
  gfx.drawString(x + 20, y + 86, ConvertUnixTime(WxConditions[0].Sunset).substring(0, 5));
  gfx.drawString(x + 4, y + 100, "Moon:");
  gfx.drawString(x + 35, y + 100, MoonPhase(MoonDay, MoonMonth, MoonYear, Hemisphere));
  DrawMoon(x + 103, y + 51, MoonDay, MoonMonth, MoonYear, Hemisphere);
}

void DrawMoon(int x, int y, int dd, int mm, int yy, String hemisphere)
{
  int diameter = 38;
  float Xpos, Ypos, Rpos, Xpos1, Xpos2, ag;//, ip;
  gfx.setColor(EPD_BLACK);
  for (Ypos = 0; Ypos <= 45; Ypos++)
  {
    Xpos = sqrt(45 * 45 - Ypos * Ypos);
    // Draw dark part of moon
    double pB1x = (90 - Xpos) / 90 * diameter + x;
    double pB1y = (Ypos + 90) / 90 * diameter + y;
    double pB2x = (Xpos + 90) / 90 * diameter + x;
    double pB2y = (Ypos + 90) / 90 * diameter + y;
    double pB3x = (90 - Xpos) / 90 * diameter + x;
    double pB3y = (90 - Ypos) / 90 * diameter + y;
    double pB4x = (Xpos + 90) / 90 * diameter + x;
    double pB4y = (90 - Ypos) / 90 * diameter + y;
    gfx.setColor(EPD_BLACK);
    gfx.drawLine(pB1x, pB1y, pB2x, pB2y);
    gfx.drawLine(pB3x, pB3y, pB4x, pB4y);
    // Determine the edges of the lighted part of the moon
    int j = JulianDate(dd, mm, yy);
    //Calculate the approximate phase of the moon
    double Phase = (j + 4.867) / 29.53059;
    Phase = Phase - (int)Phase;
    if (Phase < 0.5) ag = Phase * 29.53059 + 29.53059 / 2; else ag = Phase * 29.53059 - 29.53059 / 2; // Moon's age in days
    if (hemisphere == "south") Phase = 1 - Phase;
    Rpos = 2 * Xpos;
    if (Phase < 0.5)
    {
      Xpos1 = -Xpos;
      Xpos2 = (Rpos - 2 * Phase * Rpos - Xpos);
    }
    else
    {
      Xpos1 = Xpos;
      Xpos2 = (Xpos - 2 * Phase * Rpos + Rpos);
    }
    // Draw light part of moon
    double pW1x = (Xpos1 + 90) / 90 * diameter + x;
    double pW1y = (90 - Ypos) / 90 * diameter + y;
    double pW2x = (Xpos2 + 90) / 90 * diameter + x;
    double pW2y = (90 - Ypos) / 90 * diameter + y;
    double pW3x = (Xpos1 + 90) / 90 * diameter + x;
    double pW3y = (Ypos + 90) / 90 * diameter + y;
    double pW4x = (Xpos2 + 90) / 90 * diameter + x;
    double pW4y = (Ypos + 90) / 90 * diameter + y;
    gfx.setColor(EPD_WHITE);
    gfx.drawLine(pW1x, pW1y, pW2x, pW2y);
    gfx.drawLine(pW3x, pW3y, pW4x, pW4y);
  }
  gfx.setColor(EPD_BLACK);
  gfx.drawCircle(x + diameter - 1, y + diameter, diameter / 2 + 1);
}
