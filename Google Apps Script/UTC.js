function sendUTC() {
  // Get active spreadsheet
  var spreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  
  // Get sheet by name
  var sheet = spreadsheet.getSheetByName("weatherData");
  
  // Get value from cell C5
  var data = sheet.getRange("C5").getValue();
  
  // Log the value
  Logger.log("Value from cell C5: " + data);
  
  let formattedText = JSON.stringify(data);
  Logger.log(formattedText);

  // Remove unnecessary characters (if needed)
  const u = formattedText.replace(/\[\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*,\s*""\s*\],?/g, '');

  Logger.log(u);
  
  // Return result
  return ContentService.createTextOutput(u);
}