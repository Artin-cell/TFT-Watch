#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <TFT.h>
#include <myImage.h>

#ifndef DRAW_H
#define DRAW_H
// Функция для вывода изображения на дисплей  myImage, 128, 160
void drawImage(const uint16_t *img, int x, int y, int srcWidth, int srcHeight, int destWidth, int destHeight, int hold)
{
    if (destWidth <= 0 || destHeight <= 0)
        return;

    float scaleX = (float)srcWidth / destWidth;
    float scaleY = (float)srcHeight / destHeight;

    for (int j = 0; j < destHeight; j++)
    {
        for (int i = 0; i < destWidth; i++)
        {
            // Вычисляем координаты в исходном изображении
            int srcX = (int)(i * scaleX);
            int srcY = (int)(j * scaleY);

            // Чтение цвета из PROGMEM
            uint16_t color = pgm_read_word(&img[srcY * srcWidth + srcX]);
            // Пропускаем очень темные пиксели (фон)
            if (color > hold)
            {
                tft.drawPixel(x + i, y + j, color);
            }
        }
    }
}


#endif // DRAW_H