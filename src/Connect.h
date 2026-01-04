#ifndef CONNECT_H
#define CONNECT_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <GyverPortal.h>
#include <EEPROM.h>
#include <NTPClient.h>
#include "Draw.h"
#include "TFT.h"
#include "config_secret.h"

// Объявление экземпляра TFT
extern Adafruit_ST7735 tft;

// Глобальные переменные для Wi-Fi
String wifiSSID = "";
String wifiPassword = "";
bool wifiConfigured = false;

// Глобальный массив для Wi-Fi сетей
const int MAX_NETWORKS = 50;  // ← Размер массива
extern String wifiNetworks[MAX_NETWORKS];  // ← Объявление массива
extern int networkCount;  // ← Фактическое количество найденных сетей

// переменные
bool valCheck;
bool valSwitch;
String valText;
int valNum;
char valPass[10];
float valSpin;
int valSlider;
GPdate valDate;
GPtime valTime;
GPcolor valCol;
int valSelect;
int valRad;
String wifiListStr = "Нет сетей";

GyverPortal ui;

// В конце файла (перед #endif) нужно добавить определение массива:
String wifiNetworks[MAX_NETWORKS] = {""};  // Инициализация пустыми строками
int networkCount = 0;

String getWiFiList()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  String result = "";

  // Используем глобальные переменные
  networkCount = (n > MAX_NETWORKS) ? MAX_NETWORKS : n;

  for (int i = 0; i < networkCount; i++)
  {
    wifiNetworks[i] = WiFi.SSID(i); // Сохраняем в глобальный массив

    if (i > 0)
      result += ",";
    result += wifiNetworks[i];       // Добавляем к строке для SELECT
    Serial.println(wifiNetworks[i]); // Выводим в Serial
  }

  WiFi.scanDelete();

  // Теперь у тебя есть:
  // 1. wifiNetworks[] - массив с названиями сетей (глобальный)
  // 2. networkCount - количество найденных сетей (глобальный)
  // 3. result - строка для SELECT в формате "сеть1,сеть2,..."

  wifiListStr = (n == 0) ? "Нет сетей" : result;
 
  return wifiListStr;
}

// Функция для получения названия сети по индексу
String getNetworkName(int index) {
  if (index >= 0 && index < networkCount) {
    Serial.print(wifiNetworks[index]);
    return wifiNetworks[index];
  }
  return "Нет сети";
}

void build()
{
  GP.BUILD_BEGIN(GP_DARK);

  GP.TITLE("Настрока часов", "t1");
  GP.HR();
  GP.LABEL("Wi-Fi", "val");
  GP.BREAK();
  GP.SELECT("sel", wifiListStr, valSelect);
  GP.BREAK();
  GP.TEXT("txt", "Wi-Fi password", valText);
  GP.BREAK();
  GP.LABEL("UTC", "val");
  GP.NUMBER("num", "UTC", valNum);
  GP.BREAK();
  

  // ... остальные компоненты ...

  GP.BREAK();
  GP.BUTTON("btn", "APPLY");

  GP.BUILD_END();
}

void action()
{
  // был клик по компоненту
  if (ui.click())
  {
    // проверяем компоненты и обновляем переменные

    // ... другие компоненты ...

    if (ui.clickInt("sel", valSelect))
    {
      Serial.print("Select: ");
      Serial.println(valSelect);
      
      // Используем функцию getNetworkName или напрямую массив
      if (valSelect >= 0 && valSelect < networkCount) {
        String selectedNetwork = wifiNetworks[valSelect];
        Serial.print("Выбранная сеть: ");
        Serial.println(selectedNetwork);

        // Сохраняем выбранный SSID для подключения
        wifiSSID = selectedNetwork;
      } else {
        Serial.println("Неверный индекс сети");
      }
    }

    if (ui.clickString("txt", valText))
    {
      Serial.print("Text: ");
      Serial.println(valText);
    }

     if (ui.clickInt("num", valNum))
    {
      Serial.print("Number: ");
      Serial.println(valNum);
    }

    if (ui.click("btn"))
      Serial.println("Button click");
  }
}

// Основная функция настройки
void settings()
{
  // Инициализация дисплея
  tft.initR(INITR_BLACKTAB);    // Инициализация с черной вкладкой
  tft.fillScreen(ST7735_BLACK); // Очистка экрана черным цветом

  // Установка цвета текста
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(TSize);

  // Вывод текста на дисплей
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  tft.print("Connect to WI-FI"); // Display the formatted time

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    tft.print(".");
  }
  tft.println(".");
  tft.println(WiFi.localIP()); // Вывод текста
  Serial.println(WiFi.localIP());

  Serial.println("Завершение функции settings()");
}
#endif // CONNECT_H