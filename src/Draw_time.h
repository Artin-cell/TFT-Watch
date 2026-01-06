#include "myImage.h"
#include "Draw.h"
#include "get_info.h"

const int ledPin = D1;
int x_num = 0;
int y_num = 0;
extern String formattedTime;

// Объявляем переменные времени как глобальные, чтобы они были доступны в обеих функциях
char hour1, hour2, minute1, minute2, second1, second2;

// Объявляем массивы цифр
extern const uint16_t b0[];
extern const uint16_t b1[];
extern const uint16_t b2[];
extern const uint16_t b3[];
extern const uint16_t b4[];
extern const uint16_t b5[];
extern const uint16_t b6[];
extern const uint16_t b7[];
extern const uint16_t b8[];
extern const uint16_t b9[];

void get_time()
{
  tft.fillScreen(ST77XX_BLACK);
  uptime();
  String time_now = formattedTime;

  // Проверяем длину строки времени перед доступом к символам
  if (time_now.length() >= 8)
  {
    // Extract each character as a separate variable
    hour1 = time_now[0];
    hour2 = time_now[1];
    minute1 = time_now[3];
    minute2 = time_now[4];
    second1 = time_now[6];
    second2 = time_now[7];
  }
  else
  {
    // Значения по умолчанию, если строка времени некорректна
    hour1 = '0';
    hour2 = '0';
    minute1 = '0';
    minute2 = '0';
    second1 = '0';
    second2 = '0';
    Serial.println("Ошибка: некорректный формат времени");
  }

  // Преобразуем часы в число
  int hours = (hour1 - '0') * 10 + (hour2 - '0');

  // Управляем яркостью светодиода в зависимости от времени
  if ((hours >= 8) && (hours < 22))
  {
    analogWrite(ledPin, 255); // Яркий свет днем
  }
  else
  {
    analogWrite(ledPin, 15); // Тусклый свет ночью
  }
}

void draw_time()
{
  get_time(); // Получаем актуальное время

  // Массив указателей на массивы с изображениями цифр
  const uint16_t *b[] = {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9};

  // Размеры изображений цифр
  int width = 37;
  int height = 48;

  // Координаты для каждой цифры (часы: 0-1, минуты: 2-3)
  int x_coords[] = {27, 64, 27, 64};
  int y_coords[] = {32, 32, 80, 80};

  // Массив цифр для отрисовки
  char digits[] = {hour1, hour2, minute1, minute2};

  // Отрисовываем все 4 цифры в цикле
  for (int i = 0; i < 4; i++)
  {
    if (digits[i] >= '0' && digits[i] <= '9')
    {
      int index = digits[i] - '0';
      if (index >= 0 && index <= 9)
      {
        const uint16_t *name = b[index];
        drawImage(name, x_coords[i], y_coords[i], width, height, width, height, 0);
      }
    }
  }

  // Небольшая задержка перед обновлением (по желанию)
  // delay(1000);
}