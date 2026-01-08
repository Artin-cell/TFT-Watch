function sendWeather() {
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Weather");
  const data = sheet.getRange("B1:B11").getValues();

  // Convert array to string
  let formattedText = JSON.stringify(data);

  // Remove unnecessary characters
  const a = formattedText.replace(/\[\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*\],?/g, '');

  Logger.log(a);
  
  // Return result
  return ContentService.createTextOutput(a);
}