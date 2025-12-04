#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#ifndef TFT_H
#define TFT_H

// Определение пинов для дисплея
#define TFT_CS D3
#define TFT_DC D8
#define TFT_RST D6

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

int TSize = 1; //Размер текста
int HText = 8 * TSize; //Высота текста

#endif // TFT_H
