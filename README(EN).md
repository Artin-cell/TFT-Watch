# ⌚ TFT-Watch (EN)

<div align="center">

![ESP8266](https://img.shields.io/badge/ESP8266-TFT%20Watch-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-green)
![Status](https://img.shields.io/badge/Status-In%20Development-yellow)

**Smart TFT Watch based on ESP8266** with a 128×160 color display showing time, calendar and weather with charts.

</div>

---

## 🎯 Features

| Feature            | Description                          | Icon |
| ------------------ | ------------------------------------ | ---- |
| **Time**           | Accurate NTP-synchronized time       | 🕐   |
| **Calendar**       | Google Calendar events display       | 📅   |
| **Weather**        | Current weather and forecast         | 🌦️  |
| **Weather Charts** | Temperature, humidity, precipitation | 📊   |
| **Weather Icons**  | WMO weather codes                    | ⛅    |
| **Wi-Fi**          | Internet-based data                  | 🌐   |

---

## 📖 Project Overview

**TFT-Watch** is an ESP8266-based smart watch / desk display project using a color TFT screen.
The device connects to Wi-Fi, fetches online data and displays it in a compact, graphical interface.

The main goals of the project are:

* clarity,
* rich visualization,
* autonomous operation.

---

## 📊 Weather Visualization

The project includes **graph-based weather visualization**:

* daily temperature changes
* humidity levels
* precipitation

This allows quick and intuitive analysis of weather trends.

---

## 🧩 Planned Features

* 🌡 **Indoor Climate Monitoring**

  * temperature & humidity sensor
  * indoor climate visualization
  * data analysis

*(Planned, not yet implemented)*

---

## 🖥 Hardware

* ESP8266
* 128×160 TFT display (ST7735)
* Wi-Fi connection

---

## 📁 Project Structure

```
TFT-Watch/
├── src/
│   ├── main.cpp
│   ├── Connect.h
│   ├── TFT.h
│   ├── Draw.h
│   ├── Draw_time.h
│   ├── get_info.h
│   ├── get_ip.h
│   ├── get_events.h
│   ├── Weather.h
│   ├── myImage.h
│   └── weather_icons.h
├── platformio.ini
└── README.md
```

---

## 📸 Screenshots & Photos

![Uploading photo_2026-01-07_01-40-12.jpg…]()


*(placeholder for screenshots)*

### 📷 Device Photos

*(placeholder for hardware photos)*

---

## 🚀 Project Status

The project is **under active development** and continues to evolve.
