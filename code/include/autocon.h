#pragma once

// Redirects from root to the hello page.
void onRoot()
{
  WEBServer& webServer = portal.host();
  webServer.sendHeader("Location", String("http://") + webServer.client().localIP().toString() + String(HELLO_URI));
  webServer.send(302, "text/plain", "");
  webServer.client().flush();
  webServer.client().stop();
}

// Load the attribute of the element to modify at runtime from external.
String onHello(AutoConnectAux& aux, PageArgument& args)
{
  aux.loadElement(ElementJson);
  return String();
}

// Load the element from specified file in SPIFFS.
void loadParam(const char* fileName)
{
  SPIFFS.begin();
  File param = SPIFFS.open(fileName, "r");
  if (param)
  {
    ElementJson = param.readString();
    param.close();
  }
  SPIFFS.end();
}