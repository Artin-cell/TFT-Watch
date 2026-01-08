#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "Connect.h"
#include <get_info.h>
#include <StringUtils.h>
#include "TimesNRCyr6.h"
#include "config_secret.h"

// =====================
// Глобальные переменные
// =====================

static String google_url;
static const char* scriptUrl = nullptr;
const int displayWidth = 128;

// =====================
// Инициализация URL
// =====================

static void initGoogleScriptUrl()
{
  if (scriptUrl != nullptr) return; // Уже инициализировано

  String scriptId = getString("id_script");

  if (scriptId.length() == 0) {
    Serial.println(F("ERROR: Google Script ID is empty"));
    return;
  }

  google_url = "https://script.google.com/macros/s/" +
               scriptId +
               "/exec?action=sendCals";

  scriptUrl = google_url.c_str();

  Serial.print(F("Google Script URL: "));
  Serial.println(google_url);
}

// =====================
// Парсинг ответа
// =====================

void parseResponse(const String &payload)
{
  String trimmedResponse = payload;

  trimmedResponse.remove(0, 1);
  trimmedResponse.remove(trimmedResponse.length() - 2, 2);

  const size_t capacity = JSON_ARRAY_SIZE(10) + JSON_ARRAY_SIZE(6) * 10;
  DynamicJsonDocument doc(capacity);

  DeserializationError error = deserializeJson(doc, "[" + trimmedResponse + "]");
  if (error) {
    Serial.print(F("JSON error: "));
    Serial.println(error.f_str());
    return;
  }

  JsonArray events = doc.as<JsonArray>();

  int y = 0;
  int count = 0;

  for (JsonArray event : events)
  {
    if (count >= 5) break;

    String eventName  = event[0].as<String>();
    String eventTime  = event[1].as<String>();
    String eventDate  = event[2].as<String>();
    int eventRed      = event[3].as<int>();
    int eventGreen    = event[4].as<int>();
    int eventBlue     = event[5].as<int>();

    Serial.print(F("Event: "));
    Serial.print(eventName);
    Serial.print(F(" | "));
    Serial.print(eventTime);
    Serial.print(F(" | "));
    Serial.println(eventDate);

    int h = 32;
    uint16_t color = tft.color565(eventRed, eventGreen, eventBlue);

    tft.fillRoundRect(0, y, 128, h, 8, color);
    tft.drawRoundRect(0, y, 128, h, 8, ST7735_BLACK);

    tft.setFont(&TimesNRCyr6pt8b);
    tft.setTextColor(ST7735_BLACK);

    if (eventTime == "Весь день") {
      tft.setCursor(3, y + 13);
      tft.println(eventName);
      tft.setCursor(3, y + 26);
      tft.println(eventDate);
    } else {
      tft.setCursor(3, y + 13);
      tft.println(eventName);
      tft.setCursor(3, y + 26);
      tft.print(eventTime);
      tft.print(" ");
      tft.println(eventDate);
    }

    y += h;
    count++;
  }
}

// =====================
// Загрузка событий
// =====================

void fetchCalendarEvents()
{
  initGoogleScriptUrl();

  if (scriptUrl == nullptr) {
    Serial.println(F("ERROR: scriptUrl not initialized"));
    return;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected"));
    return;
  }

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.setTimeout(10000);

  Serial.print(F("Connecting to: "));
  Serial.println(scriptUrl);

  if (!http.begin(client, scriptUrl)) {
    Serial.println(F("HTTP begin failed"));
    return;
  }

  int httpCode = http.GET();
  Serial.printf("HTTP code: %d\n", httpCode);


  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    parseResponse(payload);
  } else {
    Serial.printf("HTTP error %d: %s\n",
                  httpCode,
                  http.errorToString(httpCode).c_str());
  }

  http.end();
}
