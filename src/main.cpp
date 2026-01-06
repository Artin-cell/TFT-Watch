#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "myImage.h"
#include "Draw.h"
#include "get_info.h"
#include <TFT.h>
#include <Connect.h>
#include "draw_time.h"
#include "get_events.h"
#include <get_ip.h>
#include <cmath>

int sec = 0;
extern int btn;
extern int swt_time;
extern int swt_cal;
extern int swt_weat;
extern int spn_time;
extern int spn_cal;
extern int spn_weat;
uint16_t rgbTo565(uint8_t r, uint8_t g, uint8_t b)
{
  return (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
}

bool jumperCounting = false;
unsigned long jumperStart = 0;

// Супер-простая версия
// void checkJumperReset() {
//     // Настройка пинов
//     static bool pinsSetup = false;
//     if (!pinsSetup) {
//         pinMode(D1, OUTPUT);
//         digitalWrite(D1, HIGH);
//         pinMode(D2, INPUT_PULLUP);
//         pinsSetup = true;
//     }

//     // Если перемычка замкнута
//     if (digitalRead(D2) == HIGH) {
//         Serial.println("Reset jumper detected, checking...");

//         int u = 0;

//         // Отсчитываем 5 секунд
//         for (int i = 0; i < 5; i++) {
//             delay(1000);  // Ждем 1 секунду

//             // Проверяем, все ли еще замкнуто
//             if (digitalRead(D2) == LOW) {
//                 Serial.println("Jumper released early");
//                 return;  // Перемычку убрали
//             }

//             u++;  // Считаем секунды
//         }

//         // Если u == 5, значит держали все 5 секунд
//         if (u >= 5) {
//             Serial.println("Clearing WiFi config!");
//             clearWiFiConfig();

//             // Можно сразу перезагрузить
//             delay(1000);
//             ESP.restart();
//         }
//     }
// }

int stringToInt(String str) {
  return str.toInt();
}

void inic()
{
  // Инициализация последовательного порта
  Serial.begin(115200);
  Serial.println("Инициализация дисплея...");

  // Инициализация дисплея
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK); // Очищаем экран
}

void setup()
{
  inic();
  initEEPROM();
  tft.fillScreen(ST77XX_BLACK);
  // checkJumperReset();
  // tft.setRotation(0);
  getWiFiList();

  settings();
}
// Использовать в будущем неблокируеммый таймер

void loop()
{
  get_time();
  Serial.println(stringToInt(getString("swt_time_mem")));
  Serial.println(stringToInt(getString("swt_cal_mem")));
  Serial.println(stringToInt(getString("swt_weat_mem")));
  Serial.println(stringToInt(getString("spn_time_mem")));
  Serial.println(stringToInt(getString("spn_cal_mem")));
Serial.println(stringToInt(getString("spn_weat_mem")));

  if (stringToInt(getString("swt_time_mem")) == 1)
  {
    
    draw_time();
    int spn_time1 = stringToInt(getString("spn_time_mem"));
    delay(spn_time1 * 1000);
  }

  if (stringToInt(getString("swt_cal_mem")) == 1)
  {
    fetchCalendarEvents();
    int spn_cal1 = stringToInt(getString("spn_cal_mem"));
    delay(spn_cal1 * 1000);
  }

  if (stringToInt(getString("swt_weat_mem")) == 1)
  {
    getExternalIP();
    int spn_weat1 = stringToInt(getString("spn_weat_mem"));
    delay(spn_weat1 * 1000);
  }
}
