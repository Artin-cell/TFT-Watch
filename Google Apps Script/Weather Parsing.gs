function getWeatherData() {
  const ss = SpreadsheetApp.getActiveSpreadsheet();
  const infosheet = ss.getSheetByName("Info");

  // Get IP from cell E3
  const ipAddress = infosheet.getRange("E3").getValue();

  // Form URL for request
  const apiUrl1 = `http://ip-api.com/json/${ipAddress}?fields=city,lat,lon`;
  Logger.log(apiUrl1);
  
  const response = UrlFetchApp.fetch(apiUrl1);
  const jsonData = JSON.parse(response.getContentText());

  const latitude = jsonData.lat;
  const longitude = jsonData.lon;
  const city = jsonData.city;

  const params = {
    latitude: latitude,
    longitude: longitude,
    daily: "temperature_2m_max,temperature_2m_min,weather_code",
    hourly: "temperature_2m,precipitation_probability,relative_humidity_2m,weather_code",
    timezone: "auto",
    forecast_days: 1
  };

  const baseUrl = "https://api.open-meteo.com/v1/forecast";
  const queryString = Object.entries(params)
    .map(([key, value]) => `${key}=${encodeURIComponent(value)}`)
    .join("&");
  const apiUrl = `${baseUrl}?${queryString}`;
  Logger.log(apiUrl);

  try {
    const response = UrlFetchApp.fetch(apiUrl, { muteHttpExceptions: true });
    const jsonData = JSON.parse(response.getContentText());

    writeWeatherToSheet(jsonData, city);

    return jsonData;

  } catch (error) {
    console.error("Error:", error);
    return null;
  }

}

function writeWeatherToSheet(weatherData, city) {
  const ss = SpreadsheetApp.getActiveSpreadsheet();
  let sheet = ss.getSheetByName("WeatherData");

  sheet.getRange("A5").setValue(city);

  sheet.getRange("A1:D1").setValues([["Date", "Max temp", "Min temp", "Weather code"]]);
  sheet.getRange("A2:D2").setValues([
    [
      weatherData.daily.time[0],
      Math.ceil(weatherData.daily.temperature_2m_max[0]),
      Math.floor(weatherData.daily.temperature_2m_min[0]),
      weatherData.daily.weather_code[0]
    ]
  ]);

  sheet.getRange("A4:D4").setValues([["City", "GMT", "UTC", "Elevation"]]);
  sheet.getRange("B5:D5").setValues([
    [

      weatherData.timezone_abbreviation,
      weatherData.utc_offset_seconds,
      weatherData.elevation
    ]
  ]);

  sheet.getRange("F1:J1").setValues([["Time", "Temperature", "Precipitation", "Humidity", "Weather code"]]);

  const hourly = weatherData.hourly;
  const hourlyData = [];
  for (let i = 0; i < hourly.time.length; i++) {
    hourlyData.push([
      hourly.time[i].replace("T", " "),
      Math.round(hourly.temperature_2m[i]),
      hourly.precipitation_probability[i],
      hourly.relative_humidity_2m[i],
      hourly.weather_code[i]
    ]);
  }

  sheet.getRange(2, 6, hourlyData.length, 5).setValues(hourlyData);

  sheet.autoResizeColumns(1, 10);
}












