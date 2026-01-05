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
#include <Arduino.h>
#include <map>

std::map<String, String> stringStorage;

// Объявление экземпляра TFT
extern Adafruit_ST7735 tft;

// Глобальные переменные для Wi-Fi
String wifiSSID = "";
String wifiPassword = "";
bool wifiConfigured = false;

// Глобальный массив для Wi-Fi сетей
const int MAX_NETWORKS = 50;              // ← Размер массива
extern String wifiNetworks[MAX_NETWORKS]; // ← Объявление массива
extern int networkCount;                  // ← Фактическое количество найденных сетей

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
int btn = 0;

GyverPortal ui;

String wifiNetworks[MAX_NETWORKS] = {""}; // Инициализация пустыми строками
int networkCount = 0;

// Структура для хранения данных в EEPROM
struct WiFiConfig {
    char ssid[32];
    char password[64];
};

// Адреса в EEPROM
const int EEPROM_SIZE = 512;
const int WIFI_CONFIG_ADDR = 0;

void saveString(const String &key, const String &value)
{
    if (key == "wifi_ssid") {
        // Сохраняем SSID в EEPROM
        WiFiConfig config;
        EEPROM.begin(EEPROM_SIZE);
        EEPROM.get(WIFI_CONFIG_ADDR, config);
        
        // Копируем строку в структуру
        strncpy(config.ssid, value.c_str(), sizeof(config.ssid) - 1);
        config.ssid[sizeof(config.ssid) - 1] = '\0'; // Гарантируем завершающий ноль
        
        EEPROM.put(WIFI_CONFIG_ADDR, config);
        EEPROM.commit();
        EEPROM.end();
        
        Serial.print(F("Saved SSID to EEPROM: "));
        Serial.println(config.ssid);
    }
    else if (key == "wifi_pass") {
        // Сохраняем пароль в EEPROM
        WiFiConfig config;
        EEPROM.begin(EEPROM_SIZE);
        EEPROM.get(WIFI_CONFIG_ADDR, config);
        
        // Копируем строку в структуру
        strncpy(config.password, value.c_str(), sizeof(config.password) - 1);
        config.password[sizeof(config.password) - 1] = '\0'; // Гарантируем завершающий ноль
        
        EEPROM.put(WIFI_CONFIG_ADDR, config);
        EEPROM.commit();
        EEPROM.end();
        
        Serial.print(F("Saved password to EEPROM: "));
        Serial.println("********"); // Не выводим пароль в лог
    }
    else {
        // Для других ключей используем map (временное хранение)
        stringStorage[key] = value;
        Serial.print(F("Saved to map: "));
        Serial.print(key);
        Serial.print(F(" = "));
        Serial.println(value);
    }
}

String getString(const String &key)
{
    if (key == "wifi_ssid" || key == "wifi_pass") {
        // Читаем из EEPROM
        WiFiConfig config;
        EEPROM.begin(EEPROM_SIZE);
        EEPROM.get(WIFI_CONFIG_ADDR, config);
        EEPROM.end();
        
        if (key == "wifi_ssid") {
            Serial.print(F("Read SSID from EEPROM: "));
            Serial.println(config.ssid);
            return String(config.ssid);
        } else if (key == "wifi_pass") {
            Serial.println(F("Read password from EEPROM"));
            return String(config.password);
        }
    }
    
    // Для других ключей ищем в map
    if (stringStorage.find(key) != stringStorage.end()) {
        return stringStorage[key];
    }
    
    return String(); // Пустая строка если не найдено
}

// Функция для очистки сохраненных данных WiFi
void clearWiFiConfig() {
    WiFiConfig config;
    memset(&config, 0, sizeof(config));
    
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(WIFI_CONFIG_ADDR, config);
    EEPROM.commit();
    EEPROM.end();
    
    Serial.println(F("WiFi config cleared from EEPROM"));
}

// Функция для инициализации EEPROM в setup()
void initEEPROM() {
    EEPROM.begin(EEPROM_SIZE);
    // Проверяем, есть ли уже данные в EEPROM
    WiFiConfig config;
    EEPROM.get(WIFI_CONFIG_ADDR, config);
    
    if (config.ssid[0] == '\0') {
        Serial.println(F("EEPROM: No saved WiFi config found"));
    } else {
        Serial.print(F("EEPROM: Found saved SSID: "));
        Serial.println(config.ssid);
    }
    
    EEPROM.end();
}


void again()
{
  btn = 0;
  Serial.println("again() called - resetting btn flag");
}

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
String getNetworkName(int index)
{
  if (index >= 0 && index < networkCount)
  {
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
      if (valSelect >= 0 && valSelect < networkCount)
      {
        String selectedNetwork = wifiNetworks[valSelect];
        Serial.print("Выбранная сеть: ");
        Serial.println(selectedNetwork);

        saveString("wifi_ssid", selectedNetwork);
      }
      else
      {
        Serial.println("Неверный индекс сети");
      }
    }

    if (ui.clickString("txt", valText))
    {
      Serial.print("Text: ");
      Serial.println(valText);
      saveString("wifi_pass", valText);
    }

    if (ui.clickInt("num", valNum))
    {
      Serial.print("Number: ");
      Serial.println(valNum);
    }

    if (ui.click("btn"))
    {
      Serial.println("Button click");
      btn = 1;
    }
    // else не нужен, так как btn уже инициализирован как 0
  }
}

// Основная функция настройки
// Основная функция настройки
void settings()
{
  // Инициализация дисплея
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);

  // Установка цвета текста
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(TSize);

  String ssid1 = getString("wifi_ssid");
  String pass1 = getString("wifi_pass");
  Serial.println(ssid1);
  Serial.println(pass1);

  tft.setCursor(0, 0);

  // Если есть сохраненные SSID и пароль, пробуем подключиться
  if (ssid1.length() > 0)
  {
    tft.print("Connecting to:");
    tft.println(ssid1);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid1, pass1);
    tft.print("Connecting");

    int ticks = 0;
    while (WiFi.status() != WL_CONNECTED && ticks < 50)
    {
      delay(500);
      Serial.print(".");
      Serial.print(ticks);
      Serial.print(" ");
      tft.print(".");
      ticks++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      // Успешное подключение
      tft.println("");
      tft.println("Connected!");
      tft.print("IP: ");
      tft.println(WiFi.localIP());
      Serial.println("");
      Serial.println("WiFi connected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      return; // Выходим из функции - WiFi подключен
    }
  }

  // Если дошли сюда - либо нет сохраненных данных, либо подключение не удалось
  tft.println("Setup WiFi");
  tft.println("Connect to AP:");
  tft.println("ESP_Config");
  tft.println("Pass: 12345678");

  // Запускаем точку доступа
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP_Config", "12345678");

  // Настраиваем веб-интерфейс
  ui.attachBuild(build);
  ui.attach(action);
  ui.start();

  // Сбрасываем флаг кнопки
  btn = 0;

  tft.print("AP IP: ");
  tft.println(WiFi.softAPIP());
  tft.println("Waiting for config...");

  // Ждем нажатия кнопки APPLY
  while (btn == 0)
  {
    ui.tick(); // Обрабатываем UI
    delay(10);
  }

  // После нажатия кнопки снова пытаемся подключиться
  settings(); // Рекурсивный вызов с новыми данными
}

#endif // CONNECT_H