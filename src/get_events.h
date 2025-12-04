#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Connect.h"
#include <Arduino.h>
#include <get_info.h>
#include <ArduinoJson.h>
#include <StringUtils.h>
#include "TimesNRCyr6.h"

// СДЕЛАТЬ ЕЩЕ ОДНО РАЗВЕРТЫВАНИЕ
const char *scriptUrl = "https://script.google.com/macros/s/AKfycbyrc_jwRonefB7LooTT1soxCicG4nReGN0KkO9IdFHEgRFRHAuAkf62D45eMehO-c8pKg/exec?action=sendCals";
const int displayWidth = 128;

void parseResponse(const String &payload)
{
  String trimmedResponse = payload;

  trimmedResponse.remove(0, 1);                            // Удаляем первую "["
  trimmedResponse.remove(trimmedResponse.length() - 2, 2); // Удаляем последнюю "]"
  // Serial.println(trimmedResponse);
  // Создаем JSON-документ
  const size_t capacity = JSON_ARRAY_SIZE(10) + JSON_ARRAY_SIZE(4) * 10; // Примерный размер
  DynamicJsonDocument doc(capacity);
  // Парсим строку в JSON
  DeserializationError error = deserializeJson(doc, "[" + trimmedResponse + "]");
  if (error)
  {
    Serial.print(F("Ошибка парсинга JSON: "));
    Serial.println(error.f_str());
    return;
  }
  // Обрабатываем массив событий
  JsonArray events = doc.as<JsonArray>();
  int y = 0;
  int count = 0;
  for (JsonArray event : events)
  {
    if (count >= 5)
    {
      break; // Прерываем цикл, если достигли максимального количества итераций
    }
    String eventName = event[0].as<String>();
    String eventTime = event[1].as<String>();
    String eventDate = event[2].as<String>();
    int eventRed = event[3].as<int>();
    int eventGreen = event[4].as<int>();
    int eventBlue = event[5].as<int>();

    // Выводим данные
    Serial.print("Событие: ");
    Serial.print(eventName);
    Serial.print(", Время: ");
    Serial.print(eventTime);
    Serial.print(", Дата: ");
    Serial.print(eventDate);
    Serial.print(", R: ");
    Serial.print(eventRed);
    Serial.print(", G: ");
    Serial.print(eventGreen);
    Serial.print(", B: ");
    Serial.println(eventBlue);

    // Рисуем прямоугольники
    int h = 32;
    uint16_t color = tft.color565(eventRed, eventGreen, eventBlue);
    tft.fillRoundRect(0, y, 128, h, 8, color);
    tft.drawRoundRect(0, y, 128, h, 8, ST7735_BLACK);

    
    tft.setFont(&TimesNRCyr6pt8b); // выбор шрифта
    tft.setTextColor(ST7735_BLACK);

    if (eventTime == "Весь день")
    {
      tft.setCursor(3, y + 13);
      tft.println(eventName);
      tft.setCursor(3, y + 26);
      tft.println(eventDate);
    }
    else
    {
      tft.setCursor(3, y + 13);
      tft.println(eventName);
      tft.setCursor(3, y + 26);
      tft.print(eventTime);
      tft.print(" ");
      tft.println(eventDate);
    }
    count++;
    y += h;
  }
  
}

void fetchCalendarEvents() {
  Serial.println("Проверка WiFi...");
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi не подключен!");
    return;
  }

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
  // Добавляем таймауты
  
  http.setTimeout(10000);

  Serial.print("Подключаюсь к: ");
  Serial.println(scriptUrl);

  if (http.begin(client, scriptUrl)) {
    Serial.println("Запрашиваю данные...");

    int httpCode = http.GET();
    Serial.printf("HTTP код: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Данные получены успешно");
      parseResponse(payload);
    } else {
      String error = http.errorToString(httpCode);
      Serial.printf("Ошибка HTTP: %d - %s\n", httpCode, error.c_str());
      
      // Попробуйте получить тело ответа для диагностики
      if (httpCode > 0) {
        String response = http.getString();
        Serial.print("Ответ сервера: ");
        Serial.println(response);
      }
    }
    http.end();
  } else {
    Serial.println("Не удалось инициализировать HTTP соединение");
  }
}