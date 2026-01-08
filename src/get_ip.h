#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Weather.h>
#include <weather_icons.h>
#include "config_secret.h"
#include "Connect.h"

static String google_ip_url;
static const char *ipScriptUrl = nullptr;
// Основной URL для отправки IP

static void initGoogleipScriptUrl()
{
  if (ipScriptUrl != nullptr)
    return;

  String scriptId = getString("id_script");

  if (scriptId.length() == 0)
  {
    Serial.println(F("ERROR: Google Script ID is empty"));
    return;
  }

  google_ip_url =
      "https://script.google.com/macros/s/" +
      scriptId +
      "/exec?action=IP&value=";

  ipScriptUrl = google_ip_url.c_str();

  Serial.print(F("Google IP Script URL: "));
  Serial.println(google_ip_url);
}

// Вспомогательная функция для парсинга строк с числами через запятую
void parseNumberString(const String &numberStr, int *outputArray, int size)
{
  String tempStr = numberStr;
  int index = 0;
  int startPos = 0;
  int commaPos = 0;

  while (index < size && commaPos >= 0)
  {
    commaPos = tempStr.indexOf(',', startPos);
    String valueStr;

    if (commaPos == -1)
    {
      valueStr = tempStr.substring(startPos);
    }
    else
    {
      valueStr = tempStr.substring(startPos, commaPos);
    }

    outputArray[index] = valueStr.toInt();
    startPos = commaPos + 1;
    index++;
  }
}

void parseWeatherData(const String &payload)
{
  tft.setFont(NULL);
  tft.setTextColor(ST77XX_WHITE);
  // Создаем JSON-документ
  const size_t capacity = JSON_ARRAY_SIZE(12) + 500;
  DynamicJsonDocument doc(capacity);

  // Парсим JSON
  DeserializationError error = deserializeJson(doc, payload);
  if (error)
  {
    Serial.print(F("Ошибка парсинга JSON: "));
    Serial.println(error.f_str());
    return;
  }

  // Получаем корневой массив
  JsonArray root = doc.as<JsonArray>();

  // Рисуем столбики и линию
  tft.fillRect(5, 67, 29, 90, tft.color565(85, 85, 85));
  tft.fillRect(34, 67, 30, 90, tft.color565(55, 55, 55));
  tft.fillRect(64, 67, 29, 90, tft.color565(85, 85, 85));
  tft.fillRect(92, 67, 30, 90, tft.color565(55, 55, 55));
  tft.drawLine(0, 126, 128, 126, tft.color565(0, 0, 0));

  // 5. Осадки график (24 значения)
  String precipChartStr = root[5][0].as<String>();
  int precipChart[24];
  parseNumberString(precipChartStr, precipChart, 24);
  DrawWeather(precipChartStr, tft.color565(209, 161, 255), 3);

  // 6. Влажность график (24 значения)
  String humidityChartStr = root[6][0].as<String>();
  int humidityChart[24];
  parseNumberString(humidityChartStr, humidityChart, 24);
  DrawWeather(humidityChartStr, tft.color565(132, 0, 255), 2);

  // 7. Температура график (24 значения)
  String tempChartStr = root[4][0].as<String>();
  int tempChart[24];
  parseNumberString(tempChartStr, tempChart, 24);
  DrawWeather(tempChartStr, tft.color565(255, 38, 38), 1);

  int x0 = 5; // начальная координата
  int y0 = 67;
  int w = 118; // начальная ширина
  int h = 90;  // начальная высота
  int radius = 13;

  for (int j = 0; j < 12; j++)
  {

    tft.drawRoundRect(x0, y0, w, h, radius, tft.color565(0, 0, 0));

    // Увеличить прямоугольник на 1 пиксель со всех сторон:
    x0 -= 1; // сдвинуть влево
    y0 -= 1; // сдвинуть вверх
    w += 2;  // добавить 1 пиксель справа и 1 слева
    h += 2;  // добавить 1 пиксель сверху и 1 снизу

    radius++; // опционально
  }
  tft.fillCircle(5, 157, 6, tft.color565(0, 0, 0));
  tft.fillCircle(123, 157, 7, tft.color565(0, 0, 0));
  tft.fillCircle(5, 65, 7, tft.color565(0, 0, 0));
  tft.fillCircle(123, 65, 7, tft.color565(0, 0, 0));
  int x1 = 5; // начальная координата
  int y1 = 67;
  int w1 = 118; // начальная ширина
  int h1 = 90;  // начальная высота
  int radius1 = 13;
  tft.drawRoundRect(x1, y1, w1, h1, radius1, tft.color565(0, 0, 0));
  // tft.drawRoundRect(x1-1, y1-1, w1+2, h1+2, radius1+1, tft.color565(97, 97, 97));
  tft.drawPixel(14, 159, tft.color565(0, 0, 0));
  tft.drawPixel(113, 159, tft.color565(0, 0, 0));

  // 1. Город
  String city = root[0][0].as<String>();
  Serial.print("Город: ");
  Serial.println(city);
  tft.setCursor(8, 10);
  tft.setTextSize(1);
  tft.print(city);

  // 2. Средняя температура
  String avgTemp = root[1][0].as<String>();
  int daAvgTemps[4];
  parseNumberString(avgTemp, daAvgTemps, 4);
  Serial.print("Распарсенные температуры: ");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(daAvgTemps[i]);
    if (i < 3)
      Serial.print(", ");
  }
  Serial.println();

  // 3. Основной код погоды
  int weatherCode = root[2][0].as<int>();
  Serial.print("Код погоды: ");
  Serial.println(weatherCode);

  int x_num = 79; // Задаем координаты
  int y_num = 9;
  int w_icons = 43;
  switch (weatherCode)
  {
  case 1:
  {
    drawImage(b1_0, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 2:
  {
    drawImage(b2_0, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 3:
  {
    drawImage(b3_0, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 45:
  {
    drawImage(b45, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 48:
  {
    drawImage(b48, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 51:
  {
    drawImage(b51, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 53:
  {
    drawImage(b53, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 55:
  {
    drawImage(b55, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 56:
  {
    drawImage(b56, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 57:
  {
    drawImage(b57, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 61:
  {
    drawImage(b61, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 63:
  {
    drawImage(b63, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 66:
  {
    drawImage(b66, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 67:
  {
    drawImage(b67, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 71:
  {
    drawImage(b71, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 73:
  {
    drawImage(b73, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 75:
  {
    drawImage(b75, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 77:
  {
    drawImage(b77, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 80:
  {
    drawImage(b80, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 82:
  {
    drawImage(b82, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 85:
  {
    drawImage(b85, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 86:
  {
    drawImage(b86, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 95:
  {
    drawImage(b95, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 96:
  {
    drawImage(b96, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  case 99:
  {
    drawImage(b99, x_num, y_num, w_icons, w_icons, w_icons, w_icons, 0);
  }
  break;
  }

  // 4. Дата
  String date = root[3][0].as<String>();
  Serial.print("Дата: ");
  Serial.println(date);
  tft.setCursor(10, 52);
  tft.setTextSize(1);
  tft.print(date);

  // 10. Мин температура
  int minTemp = root[9][0].as<int>();
  Serial.print("Мин температура: ");
  Serial.println(minTemp);

  // 11. Макс температура
  int maxTemp = root[10][0].as<int>();
  Serial.print("Макс температура: ");
  Serial.println(maxTemp);

  int medTemp = (maxTemp + minTemp) / 2;

  int pos_y = 28;
  tft.setCursor(8, pos_y);
  tft.setTextSize(2);
  tft.print(String(medTemp));

  // tft.drawLine(tft.getCursorX() + 1, pos_y + 12, tft.getCursorX() + 4, pos_y, TFT_WHITE);
  // tft.drawLine(tft.getCursorX() + 2, pos_y + 12, tft.getCursorX() + 5, pos_y, TFT_WHITE);

  // tft.setCursor(tft.getCursorX() + 10, pos_y);
  // tft.print(String(maxTemp));

  tft.fillCircle(tft.getCursorX() + 5, pos_y + 3, 3.5, TFT_WHITE);
  tft.fillCircle(tft.getCursorX() + 5, pos_y + 3, 1.25, TFT_BLACK);
  tft.drawPixel(tft.getCursorX() + 5 - 1, pos_y + 2, tft.color565(127, 127, 127));
  tft.drawPixel(tft.getCursorX() + 5 - 1, pos_y + 4, tft.color565(127, 127, 127));
  tft.drawPixel(tft.getCursorX() + 5 + 1, pos_y + 2, tft.color565(127, 127, 127));
  tft.drawPixel(tft.getCursorX() + 5 + 1, pos_y + 4, tft.color565(127, 127, 127));
  tft.setCursor(tft.getCursorX() + 11, pos_y);
  tft.print("C");

  // 9. Средние температуры за день (4 значения)
  String dailyAvgTempsStr = root[8][0].as<String>();
  int dailyAvgTemps[4];
  parseNumberString(dailyAvgTempsStr, dailyAvgTemps, 4);
  Serial.print("Средние температуры за день: ");
  Serial.println(dailyAvgTempsStr); // Выводим исходную строку с запятыми
  for (int i = 0; i < 4; i++)
  {
    int AvgTempsTft = round(maxTemp - ((maxTemp / 100.0) * (0.52 * (dailyAvgTemps[i] - 68))));
    Serial.println(AvgTempsTft);
    tft.setTextSize(1);
    switch (i)
    {
    case 0:
    {
      tft.setCursor(0, -10);
      int startX = tft.getCursorX();
      tft.setTextColor(TFT_BLACK);
      tft.print(daAvgTemps[0]);
      tft.fillCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_BLACK);
      tft.print("C");
      int endX = tft.getCursorX();
      int textWidth = endX - startX;
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(19 - textWidth / 2, dailyAvgTemps[0] - 11);
      tft.print(daAvgTemps[0]);
      tft.drawRect(tft.getCursorX(), tft.getCursorY() - 1, 3, 3, tft.color565(170, 170, 170));
      tft.drawCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_WHITE);
      tft.setCursor(tft.getCursorX() + 4, tft.getCursorY());
      tft.print("C");
      break;
    }
    case 1:
    {
      tft.setCursor(0, -10);
      int startX1 = tft.getCursorX();
      tft.setTextColor(TFT_BLACK);
      tft.print(daAvgTemps[1]);
      tft.fillCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_BLACK);
      tft.print("C");
      int endX1 = tft.getCursorX();
      int textWidth1 = endX1 - startX1;
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(47 - textWidth1 / 2, dailyAvgTemps[1] - 11);
      tft.print(daAvgTemps[1]);
      tft.drawRect(tft.getCursorX(), tft.getCursorY() - 1, 3, 3, tft.color565(155, 155, 155));
      tft.drawCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_WHITE);
      tft.setCursor(tft.getCursorX() + 4, tft.getCursorY());
      tft.print("C");
      break;
    }
    case 2:
    {
      tft.setCursor(0, -10);
      int startX2 = tft.getCursorX();
      tft.setTextColor(TFT_BLACK);
      tft.print(daAvgTemps[2]);
      tft.fillCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_BLACK);
      tft.print("C");
      int endX2 = tft.getCursorX();
      int textWidth2 = endX2 - startX2;
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(76 - textWidth2 / 2, dailyAvgTemps[2] - 11);
      tft.print(daAvgTemps[2]);
      tft.drawRect(tft.getCursorX(), tft.getCursorY() - 1, 3, 3, tft.color565(170, 170, 170));
      tft.drawCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_WHITE);
      tft.setCursor(tft.getCursorX() + 4, tft.getCursorY());
      tft.print("C");
      break;
    }
    case 3:
    {
      tft.setCursor(0, -10);
      int startX3 = tft.getCursorX();
      tft.setTextColor(TFT_BLACK);
      tft.print(daAvgTemps[3]);
      tft.fillCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_BLACK);
      tft.print("C");
      int endX3 = tft.getCursorX();
      int textWidth3 = endX3 - startX3;
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(107 - textWidth3 / 2, dailyAvgTemps[3] - 11);
      tft.print(daAvgTemps[3]);
      tft.drawRect(tft.getCursorX(), tft.getCursorY() - 1, 3, 3, tft.color565(155, 155, 155));
      tft.drawCircle(tft.getCursorX() + 1, tft.getCursorY(), 1, TFT_WHITE);
      tft.setCursor(tft.getCursorX() + 4, tft.getCursorY());
      tft.print("C");
      break;
    }
    }
  }

  // 8. Код погоды график (4 значения)
  String weatherCodeChartStr = root[7][0].as<String>();
  int weatherCodeChart[5];
  parseNumberString(weatherCodeChartStr, weatherCodeChart, 5);
  Serial.print("Код погоды график: ");
  Serial.println(weatherCodeChartStr); // Выводим исходную строку с запятыми

  // Массив координат для 4 позиций
  int positions[4][3] = {
      {9, 128, 43},  // x, y, w для позиции 1
      {38, 128, 43}, // x, y, w для позиции 2
      {68, 128, 43}, // x, y, w для позиции 3
      {96, 128, 43}  // x, y, w для позиции 4
  };

  // Отрисовка всех 4 иконок в цикле
  for (int i = 0; i < 4; i++)
  {
    int weatherCode = weatherCodeChart[i];
    const uint16_t *image = nullptr;

    // Определяем изображение прямо здесь
    switch (weatherCode)
    {
    case 1:
      image = b1_0;
      break;
    case 2:
      image = b2_0;
      break;
    case 3:
      image = b3_0;
      break;
    case 45:
      image = b45;
      break;
    case 48:
      image = b48;
      break;
    case 51:
      image = b51;
      break;
    case 53:
      image = b53;
      break;
    case 55:
      image = b55;
      break;
    case 56:
      image = b56;
      break;
    case 57:
      image = b57;
      break;
    case 61:
      image = b61;
      break;
    case 63:
      image = b63;
      break;
    case 66:
      image = b66;
      break;
    case 67:
      image = b67;
      break;
    case 71:
      image = b71;
      break;
    case 73:
      image = b73;
      break;
    case 75:
      image = b75;
      break;
    case 77:
      image = b77;
      break;
    case 80:
      image = b80;
      break;
    case 82:
      image = b82;
      break;
    case 85:
      image = b85;
      break;
    case 86:
      image = b86;
      break;
    case 95:
      image = b95;
      break;
    case 96:
      image = b96;
      break;
    case 99:
      image = b99;
      break;
    }

    int colorTreshHold;
    if (i == 0 || i == 2)
    {
      colorTreshHold = 33808;
    }
    else
    {
      colorTreshHold = 22102;
    }
    if (image != nullptr)
    {
      drawImage(image, positions[i][0], positions[i][1], positions[i][2], positions[i][2], 22, 22, colorTreshHold);
    }
  }
};

String getExternalIP()
{
  initGoogleipScriptUrl();

  if (ipScriptUrl == nullptr)
  {
    Serial.println(F("ERROR: ipScriptUrl not initialized"));
    return "";
  }

  WiFiClient client;
  HTTPClient http;

  // Получаем внешний IP
  http.begin(client, "http://api.ipify.org/");
  http.setTimeout(10000);

  int httpCode = http.GET();
  String externalIP = "";

  if (httpCode == HTTP_CODE_OK)
  {
    externalIP = http.getString();
    Serial.print("External IP: ");
    Serial.println(externalIP);
  }
  else
  {
    Serial.printf("Failed to get IP, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return "";
  }
  http.end();

  // Используем подход из вашего рабочего кода
  WiFiClientSecure secureClient;
  HTTPClient https;

  secureClient.setInsecure();
  https.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  String fullUrl = google_ip_url + externalIP;
  Serial.println("Sending to: " + fullUrl);
  // String data = "[[\"Kazan'\"],[2],[3],[\"02.11.2025\"],[\"124, 124, 124, 124, 140, 140, 140, 140, 140, 140, 140, 140, 124, 124, 109, 109, 124, 124, 124, 124, 124, 124, 140, 140\"],[\"155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155\"],[\"109, 109, 110, 111, 111, 111, 111, 111, 113, 114, 118, 119, 120, 120, 121, 124, 122, 120, 117, 117, 116, 116, 116, 120\"],[\"3, 2, 3, 2, 3\"],[\"129, 140, 119, 129\"],[0],[3]]";
  // parseWeatherData(data);
  if (https.begin(secureClient, fullUrl))
  {
    https.setTimeout(15000);

    httpCode = https.GET();

    if (httpCode == HTTP_CODE_OK)
    {
      String response = https.getString();
      Serial.println("Success! Response: " + response);
      tft.fillScreen(ST77XX_BLACK);

      parseWeatherData(response);
    }
    else
    {
      Serial.printf("Failed to send IP, error: %s\n", https.errorToString(httpCode).c_str());
      Serial.println("Response: " + https.getString());
    }

    https.end();
  }
  else
  {
    Serial.println("Connection failed");
  }

  return externalIP;
}
