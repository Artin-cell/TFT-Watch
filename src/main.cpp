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
#include <GyverPortal.h>
#include <get_ip.h>
#include <cmath>

GyverPortal ui;
int sec = 0;
uint16_t rgbTo565(uint8_t r, uint8_t g, uint8_t b)
{
  return (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
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

  // Вывод изображения на дисплей
  settings();
  delay(1000);

  // inic();
  tft.fillScreen(ST77XX_BLACK);

  // tft.setRotation(0);
  // getExternalIP();

  // ui.attachBuild(build);
  // ui.attach(action);
  // ui.start();

  
}

// Использовать в будущем неблокируеммый таймер

void loop()
{

  // ui.tick();


  draw_time();

  delay(8000);

  fetchCalendarEvents();

  delay(8000);


  getExternalIP();

  delay(5000);
}
