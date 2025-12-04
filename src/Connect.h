#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <GyverPortal.h>
#include "get_info.h" // Ensure get_info.h is included

#include <NTPClient.h>
#include "Draw.h"
#include "TFT.h"

#include "config_secret.h"

#define TFT_CS D3
#define TFT_DC D8
#define TFT_RST D6

#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

void settings() {
   // Инициализация дисплея
  tft.initR(INITR_BLACKTAB);   // Инициализация с черной вкладкой
  tft.fillScreen(ST7735_BLACK); // Очистка экрана черным цветом

    // Установка цвета текста
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(TSize);

  // Вывод текста на дисплей
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  tft.print("Connect to WI-FI" ); // Display the formatted time

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tft.print(".");
  }
  tft.println(".");
  tft.println(WiFi.localIP()); // Вывод текста
  Serial.println(WiFi.localIP());

  Serial.println("Завершение функции settings()");
}
#endif // MY_FUNCTIONS_H
