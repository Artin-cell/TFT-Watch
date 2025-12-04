#include <Adafruit_GFX.h> // Библиотека графики
#include <Adafruit_ST7735.h>
#include <TFT.h>
// #include <get_ip.h>

// Функция для расчета коэффициентов кубического сплайна

// Цвета для удобства
#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF

// Размеры дисплея
#define TFT_WIDTH 128
#define TFT_HEIGHT 160

void drawUniformLine(int x0, int y0, int x1, int y1, uint16_t color, int type)
{
  if (x0 == x1)
  { // Вертикальная линия
    tft.drawLine(x0, y0, x1, y1, color);
    return;
  }

  float k = (float)(y1 - y0) / (x1 - x0); // Угловой коэффициент

  for (int x = x0; x <= x1; x++)
  {
    int y = round(y0 + k * (x - x0)); // Основная формула

    if (type == 2) // Влажность
    {

      if ((x % 7 != 0) && ((x + 1) % 7 != 0))
      {
        // Рисуем точку
        tft.drawPixel(x, y, tft.color565(0, 128, 222));
        tft.drawPixel(x, y + 1, tft.color565(0, 128, 222));
      }
    }

    if (type == 1) // Температура
    {
      // Рисуем точку
      tft.fillCircle(x, y, 2, tft.color565(255, 103, 0));
    }
    if (type == 3) // Осадки
    {
      // Рисуем точку
      tft.drawLine(x, y, x, y + 100, tft.color565(0, 128, 222));
      tft.fillCircle(x, y, 1, tft.color565(0, 128, 222));
    }
    // tft.fillCircle(x, y, 2, color);
  }
}

void DrawWeather(String data, uint16_t color, int type)
{
  // Быстро парсим строку в массив
  int pointsy[24];
  int index = 0;
  int startPos = 0;
  // tft.drawLine(32, 0, 32, 160, ST7735_WHITE);
  // tft.drawLine(64, 0, 64, 160, ST7735_WHITE);
  // tft.drawLine(96, 0, 96, 160, ST7735_WHITE);

  while (index < 24)
  {
    int commaPos = data.indexOf(',', startPos);
    if (commaPos == -1)
      commaPos = data.length();

    String numStr = data.substring(startPos, commaPos);
    numStr.trim(); // убираем пробелы
    pointsy[index] = numStr.toInt();

    startPos = commaPos + 1;
    index++;
  }

  int pointsx[] = {5, 10, 15, 20, 25, 30, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86, 91, 97, 102, 107, 112, 117, 122};

  for (int i = 0; i < 23; i++)
  {
    drawUniformLine(pointsx[i], pointsy[i], pointsx[i + 1], pointsy[i + 1], color, type);
  }
}
