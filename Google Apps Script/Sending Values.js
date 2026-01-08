function sendCals() {
  fetchCalendarEvents();
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Send");
  const data = sheet.getRange("A1:F81").getValues(); // Your range

  // Convert array to string
  let formattedText = JSON.stringify(data);

  // Remove unnecessary characters
  const a = formattedText.replace(/\[\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*\],?/g, '');

  Logger.log(a);
  
  // Return result
  return ContentService.createTextOutput(a);
}

function sendWeather() {
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Weather");
  const data = sheet.getRange("A1:B11").getValues(); // Your range

  // Convert array to string
  let formattedText = JSON.stringify(data);

  // Remove unnecessary characters
  const w = formattedText.replace(/\[\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*\],?/g, '');

  Logger.log(w);
  
  // Return result
  return ContentService.createTextOutput(w);
}