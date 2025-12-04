# ESP8266 TFT Weather & Calendar Display

Проект для ESP8266 с TFT-экраном, отображающий:
- Текущее время через NTP
- Погоду с графиками
- События из Google Calendar
- Внешний IP адрес

## Схема подключения
- TFT CS → D3
- TFT DC → D8  
- TFT RST → D6

## Настройка
1. Установите PlatformIO
2. Заполните WiFi настройки в `Connect.h`
3. Настройте Google Apps Script URL
4. Прошивка на ESP8266
