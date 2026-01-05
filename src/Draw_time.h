#include "myImage.h"
#include "Draw.h"
#include "get_info.h"

const int ledPin = D1;
int x_num = 0;
int y_num = 0;
extern String formattedTime;
const uint16_t *name = b0;

void draw_time()
{
  tft.fillScreen(ST77XX_BLACK);
  uptime();
  String time_now = formattedTime;
  Serial.println("Текущее время: " + time_now);

  // Extract each character as a separate variable
  char hour1 = time_now[0];
  char hour2 = time_now[1];
  char minute1 = time_now[3];
  char minute2 = time_now[4];
  char second1 = time_now[6];
  char second2 = time_now[7];
  Serial.println(hour1);
  Serial.println(hour2);
  Serial.println(minute1);
  Serial.println(minute2);
  Serial.println(second1);
  Serial.println(second2);
  int hours = (hour1 - '0') * 10 + (hour2 - '0');
  if ((hours >= 8) && (hours < 22))
  {
    analogWrite(ledPin, 255); // Яркий свет днем
  }
  else
  {
    analogWrite(ledPin, 15); // Тусклый свет ночью
  }

  // Предполагаем, что b0, b1, ..., b9 объявлены как массивы типа uint16_t
  const uint16_t *b[] = {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9};

  // Получаем время и выводим на дисплей
  //  Выносим общие переменные
  int width = 37;
  int height = 48;

  // Координаты для каждой цифры
  int x_coords[] = {27, 64, 27, 64};
  int y_coords[] = {32, 32, 80, 80};
  char digits[] = {hour1, hour2, minute1, minute2};

  // Отрисовываем все 4 цифры в цикле
  for (int i = 0; i < 4; i++)
  {
    switch (digits[i])
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
      int index = digits[i] - '0';
      const uint16_t *name = b[index];
      drawImage(name, x_coords[i], y_coords[i], width, height, width, height, 0);
    }
    break;
    }
  }

  // Optional: Print each character to verify
  Serial.println("Часы: " + String(hour1) + String(hour2) + ", Минуты: " + String(minute1) + String(minute2) + ", Секунды: " + String(second1) + String(second2));

  // delay(5000);
}
