# ⌚ TFT-Watch

<div align="center">

![ESP8266](https://img.shields.io/badge/ESP8266-TFT%20Watch-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-green)
![Status](https://img.shields.io/badge/Status-In%20Development-yellow)

**Smart TFT Watch on ESP8266** with a 128×160 pixel color display, showing time, calendar, and weather with graphs.

</div>

---

## 🎯 Features

| Feature            | Description                                           | Icon |
| ------------------ | ---------------------------------------------------- | ------ |
| **Time**           | Accurate time via NTP, automatic synchronization     | 🕐     |
| **Calendar**       | Display events from Google Calendar                  | 📅     |
| **Weather**        | Current weather and forecast                         | 🌦️    |
| **Weather Graphs** | Temperature, humidity, precipitation                 | 📊     |
| **Weather Icons**  | WMO codes, visual weather conditions                 | ⛅      |
| **Wi-Fi**          | Internet connectivity                                | 🌐     |

---

## 📋 Table of Contents
* [**Google Calendar Setup**](#-google-calendar-setup)
* [Project Overview](#-project-overview)
* [Features](#-features)
* [Planned Features](#-planned-features)
* [Hardware](#-hardware)
* [Project Structure](#-project-structure)
* [Project Status](#-project-status)

---

## 📖 Project Overview

**TFT-Watch** is a smart watch/desktop display project based on **ESP8266** and a TFT screen.
The device connects to Wi-Fi, retrieves data from the internet, and displays it in a compact and visually rich interface.

Main focus is on:

* visual clarity,
* graphical data representation,
* autonomous operation.

---

## 📊 Weather and Graphs

The project displays not only textual weather information but also **graphs**:

* temperature throughout the day
* humidity
* precipitation

This allows for quick assessment of weather changes without reading numbers.

---

## 🧩 Planned Features

* 🌡 **Indoor Climate**

  * temperature and humidity sensor
  * display of indoor microclimate
  * data analysis and visualization

*(Functionality is planned but not yet implemented)*

---

## 🖥 Hardware

* ESP8266
* 128×160 TFT display (ST7735)
* Wi-Fi connection

---

## 🔧 Google Calendar Setup

To enable calendar functionality, you need to set up a proxy server via Google Sheets and Apps Script.

### 📋 Step-by-Step Instructions:

1. **Create a copy of the spreadsheet**
   * Go to: [Google Sheets Template](https://docs.google.com/spreadsheets/d/1YqFB65UOZ60psgS9BrHtdAYj01zJ0-8Ke89qvLWMD4A/edit?usp=sharing)
   * Click **"File" → "Make a copy"** in the menu.

2. **Fill in Calendar IDs**
   * In the **Calendar ID** column, enter the IDs of your Google Calendars.
   * **How to get Calendar ID?** Follow [this guide](https://docs.google.com/document/d/1-kSKf3dalrVCVa4vFMGlrWu32HrDFOZySEYf-QK9yPc/edit?usp=sharing).

3. **Configure and Deploy Apps Script**
   * In the spreadsheet menu, open **"Extensions" → "Apps Script"**.
   * In the opened script window, click **"Deploy" → "New deployment"**.
   * **"Deployment description"**: enter any description (e.g., `TFT-Watch WebApp`).
   * **"Execute as"**: select **"Me (your_email@gmail.com)"**.
   * **"Who has access"**: select **"Anyone"**.
   * Click **"Deploy"**.

4. **Get the web app URL**
   * After deployment, copy the **web app URL** (a long link like `https://script.google.com/macros/s/.../exec`).
   * **This specific URL is what you need for the project.**

5. **Configure the project**
   * In the project configuration file (`src/config.h` or similar), specify the obtained web app URL in the corresponding variable (e.g., `GOOGLE_CALENDAR_WEBAPP_URL`).

---

## 📁 Project Structure

```
TFT-Watch/
├── src/
│   ├── main.cpp          # Main program loop
│   ├── Connect.h         # Wi-Fi connection
│   ├── TFT.h             # Display initialization
│   ├── Draw.h            # Graphics rendering
│   ├── Draw_time.h       # Time display
│   ├── get_info.h        # Time retrieval (NTP)
│   ├── get_ip.h          # Weather and network data
│   ├── get_events.h      # Calendar (uses Google Sheets WebApp)
│   ├── Weather.h         # Weather graphs
│   ├── myImage.h         # Graphical resources
│   └── weather_icons.h   # Weather icons
├── platformio.ini
└── README.md
```

## 🚀 Project Status

The project is in **active development**.
Functionality is being expanded, and the interface is being refined.
