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
int debug_mod = 0;
bool valCheck;
bool valSwitch;
String valText;
String valText1;
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
int swt_time = 0;
int swt_cal = 0;
int swt_weat = 0;
int spn_time = 0;
int spn_cal = 0;
int spn_weat = 0;

GyverPortal ui;

String wifiNetworks[MAX_NETWORKS] = {""}; // Инициализация пустыми строками
int networkCount = 0;

// Структура для хранения данных в EEPROM
struct WiFiConfig
{
  char ssid[32];
  char password[64];
};

// Адреса в EEPROM
const int EEPROM_SIZE = 512;
const int WIFI_CONFIG_ADDR = 0;

void saveString(const String &key, const String &value)
{
  Serial.print(F("Saving: "));
  Serial.print(key);
  Serial.print(F(" = "));
  Serial.println(value);

  if (key == "wifi_ssid")
  {
    // Сохраняем SSID в EEPROM
    WiFiConfig config;
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(WIFI_CONFIG_ADDR, config);

    strncpy(config.ssid, value.c_str(), sizeof(config.ssid) - 1);
    config.ssid[sizeof(config.ssid) - 1] = '\0';

    EEPROM.put(WIFI_CONFIG_ADDR, config);
    EEPROM.commit();
    EEPROM.end();

    Serial.print(F("Saved SSID to EEPROM: "));
    Serial.println(config.ssid);
  }
  else if (key == "wifi_pass")
  {
    // Сохраняем пароль в EEPROM
    WiFiConfig config;
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(WIFI_CONFIG_ADDR, config);

    strncpy(config.password, value.c_str(), sizeof(config.password) - 1);
    config.password[sizeof(config.password) - 1] = '\0';

    EEPROM.put(WIFI_CONFIG_ADDR, config);
    EEPROM.commit();
    EEPROM.end();

    Serial.println(F("Saved password to EEPROM"));
  }
  else
  {
    // Для остальных настроек сохраняем в отдельную область EEPROM
    EEPROM.begin(EEPROM_SIZE);

    // Определяем адрес для каждой настройки
    int address = 100; // Начальный адрес для других настроек (после WiFi)

    if (key == "swt_time_mem")
      address = 100;
    else if (key == "swt_cal_mem")
      address = 110;
    else if (key == "swt_weat_mem")
      address = 120;
    else if (key == "spn_time_mem")
      address = 130;
    else if (key == "spn_cal_mem")
      address = 140;
    else if (key == "spn_weat_mem")
      address = 150;
    else if (key == "id_script")
      address = 160;
    else
    {
      // Для неизвестных ключей используем map
      stringStorage[key] = value;
      Serial.print(F("Saved to map: "));
      Serial.print(key);
      Serial.print(F(" = "));
      Serial.println(value);
      EEPROM.end();
      return;
    }

    // Сохраняем строку в EEPROM
    for (size_t i = 0; i < value.length(); i++)
    {
      EEPROM.write(address + i, value[i]);
    }
    EEPROM.write(address + value.length(), '\0'); // Завершающий ноль

    EEPROM.commit();
    EEPROM.end();

    Serial.print(F("Saved to EEPROM address "));
    Serial.print(address);
    Serial.print(F(": "));
    Serial.println(value);
  }
}

String getString(const String &key)
{
  if (key == "wifi_ssid" || key == "wifi_pass")
  {
    // Читаем из EEPROM
    WiFiConfig config;
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(WIFI_CONFIG_ADDR, config);
    EEPROM.end();

    if (key == "wifi_ssid")
    {
      return String(config.ssid);
    }
    else if (key == "wifi_pass")
    {
      return String(config.password);
    }
  }

  // Читаем другие настройки из EEPROM
  EEPROM.begin(EEPROM_SIZE);

  int address = 100;
  if (key == "swt_time_mem")
    address = 100;
  else if (key == "swt_cal_mem")
    address = 110;
  else if (key == "swt_weat_mem")
    address = 120;
  else if (key == "spn_time_mem")
    address = 130;
  else if (key == "spn_cal_mem")
    address = 140;
  else if (key == "spn_weat_mem")
    address = 150;
  else if (key == "id_script")
    address = 160;
  else
  {
    // Для других ключей ищем в map
    EEPROM.end();
    if (stringStorage.find(key) != stringStorage.end())
    {
      return stringStorage[key];
    }
    return String(); // Пустая строка если не найдено
  }

  // Читаем строку из EEPROM
  String result = "";
  char ch;
  int i = 0;
  while (i < 100)
  { // Максимальная длина строки
    ch = EEPROM.read(address + i);
    if (ch == '\0' || ch == 255)
      break;
    result += ch;
    i++;
  }

  EEPROM.end();

  // Если строка пустая, возвращаем значения по умолчанию
  if (result.length() == 0)
  {
    if (key == "swt_time_mem")
      result = "1";
    else if (key == "swt_cal_mem")
      result = "1";
    else if (key == "swt_weat_mem")
      result = "1";
    else if (key == "spn_time_mem")
      result = "5";
    else if (key == "spn_cal_mem")
      result = "10";
    else if (key == "spn_weat_mem")
      result = "15";
    else if (key == "id_script")
      result = "1";
  }

  Serial.print(F("Read from EEPROM: "));
  Serial.print(key);
  Serial.print(F(" = "));
  Serial.println(result);

  return result;
}

String intToString(int number)
{
  return String(number);
}

// Функция для очистки сохраненных данных WiFi
void clearWiFiConfig()
{
  WiFiConfig config;
  memset(&config, 0, sizeof(config));

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(WIFI_CONFIG_ADDR, config);
  EEPROM.commit();
  EEPROM.end();

  Serial.println(F("WiFi config cleared from EEPROM"));
  // Остальные настройки сохраняются
}

// Функция для инициализации EEPROM в setup()
void initEEPROM()
{
  EEPROM.begin(EEPROM_SIZE);
  // Проверяем, есть ли уже данные в EEPROM
  WiFiConfig config;
  EEPROM.get(WIFI_CONFIG_ADDR, config);

  if (config.ssid[0] == '\0')
  {
    Serial.println(F("EEPROM: No saved WiFi config found"));
  }
  else
  {
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
  GP.TEXT("txt", "Wi-Fi password", valText1);
  GP.BREAK();
  GP.LABEL("Id script", "val");
  GP.TEXT("id_script", "id_script", valText);
  GP.BREAK();
  GP.HR();
  GP.LABEL("Настройте функции", "val");
  GP.BREAK();
  GP.LABEL(" и их время отображения", "val");
  GP.BREAK();
  GP.LABEL("Time", "val");
  GP.SWITCH("time", valSwitch);
  GP.BREAK();
  GP.LABEL("Секунды отображения", "val");
  GP.SPINNER("spn_time", valSpin);
  GP.BREAK();
  GP.HR();

  GP.LABEL("Calendar", "val");
  GP.SWITCH("cal", valSwitch);
  GP.BREAK();
  GP.LABEL("Секунды отображения", "val");
  GP.SPINNER("spn_cal", valSpin);
  GP.BREAK();
  GP.HR();

  GP.LABEL("Weather", "val");
  GP.SWITCH("weat", valSwitch);
  GP.BREAK();
  GP.LABEL("Секунды отображения", "val");
  GP.SPINNER("spn_weat", valSpin);
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

    if (ui.clickString("txt", valText1))
    {
      String wifi_pass = valText1;
      Serial.print("Text: ");
      Serial.println(wifi_pass);
      saveString("wifi_pass", wifi_pass);
    }

    if (ui.clickString("id_script", valText))
    {
      Serial.print("Input text: ");
      Serial.println(valText);
      String id_script = valText;

      // Ожидаемый префикс и суффикс
      const String prefix = "https://script.google.com/macros/s/";
      const String suffix = "/exec";

      int start = id_script.indexOf(prefix);
      if (start >= 0)
      {
        start += prefix.length();
        int end = id_script.indexOf(suffix, start);
        if (end > start)
        {
          id_script = id_script.substring(start, end); // ← ВАЖНО: теперь id_script = ID
        }
      }

      Serial.print("Extracted script ID: ");
      Serial.println(id_script);

      saveString("id_script", id_script);
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

    if (ui.clickBool("time", valSwitch))
    {
      swt_time = valSwitch;
      Serial.print("Switch time: ");
      Serial.println(swt_time);
      saveString("swt_time_mem", intToString(swt_time));
    }

    if (ui.clickBool("cal", valSwitch))
    {
      swt_cal = valSwitch;
      Serial.print("Switch cal: ");
      Serial.println(swt_cal);
      saveString("swt_cal_mem", intToString(swt_cal));
    }

    if (ui.clickBool("weat", valSwitch))
    {
      swt_weat = valSwitch;
      Serial.print("Switch weat: ");
      Serial.println(swt_weat);
      saveString("swt_weat_mem", intToString(swt_weat));
    }
  }

  if (ui.clickFloat("spn_time", valSpin))
  {
    spn_time = valSpin;
    Serial.print("Spinner time: ");
    Serial.println(spn_time);
    saveString("spn_time_mem", intToString(spn_time));
  }

  if (ui.clickFloat("spn_cal", valSpin))
  {
    spn_cal = valSpin;
    Serial.print("Spinner cal: ");
    Serial.println(spn_cal);
    saveString("spn_cal_mem", intToString(spn_cal));
  }

  if (ui.clickFloat("spn_weat", valSpin))
  {
    spn_weat = valSpin;
    Serial.print("Spinner: ");
    Serial.println(spn_weat);
    saveString("spn_weat_mem", intToString(spn_weat));
  }
}

// Основная функция настройки
// Основная функция настройки
void settings()
{
  String ssid1 = "1";
  String pass1 = "1";
  if (debug_mod == 1)
  {
    ssid1 = "1";
    pass1 = "1";
  }
  else
  {
    ssid1 = getString("wifi_ssid");
    pass1 = getString("wifi_pass");
  }

  // Инициализация дисплея
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);

  // Установка цвета текста
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(TSize);

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
  saveString("spn_time_mem", intToString(0));
  saveString("spn_cal_mem", intToString(0));
  saveString("spn_weat_mem", intToString(0));

  // Запускаем точку доступа
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP_Config", "12345678");
  saveString("wifi_ssid", wifiNetworks[0]);
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