function fetchCalendarEvents() {
  const infoSheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Info");
  const calendarSheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Calendar");

  // Get list of calendars
  const calendarIds = infoSheet.getRange("A2:A20").getValues().flat().filter(id => id);

  // Array to store calendar colors
  const calendarColors = [];
  const eventsData = [];
  const dateToday = new Date();

  // Process each calendar
  calendarIds.forEach(calendarId => {
    const calendar = CalendarApp.getCalendarById(calendarId);

    if (!calendar) {
      eventsData.push([`Calendar with ID ${calendarId} not found`, "", "", calendarId, ""]);
      calendarColors.push([""]); // Empty cell for non-existent calendar
      return;
    }

    // Get calendar color and convert to RGB
    const hexColor = calendar.getColor();
    const rgbColor = hexToRgb(hexColor);
    calendarColors.push([rgbColor]);

    Logger.log(`Calendar: "${calendar.getName()}", HEX: ${hexColor}, RGB: ${rgbColor}`);

    // Get calendar events
    const events = calendar.getEvents(dateToday, new Date(dateToday.getFullYear() + 1, dateToday.getMonth(), dateToday.getDate()));
    const upcomingEvents = events.slice(0, 4);
    
    upcomingEvents.forEach(event => {
      const summary = event.getTitle() || "No title";
      const startDateTime = event.getStartTime();
      const endDateTime = event.getEndTime();
Logger.log(startDateTime);
      if (event.isAllDayEvent()) {
        eventsData.push([summary, "All day", formatDate(startDateTime), calendarId, calendar.getName()]);
      } else {
        eventsData.push([
          summary,
          `${formatTime(startDateTime)}-${formatTime(endDateTime)}`,
          formatDate(startDateTime),
          calendarId,
          calendar.getName()
        ]);
      }
    });
  });

  // Write calendar colors to column C in "Info" sheet
  if (calendarColors.length > 0) {
    infoSheet.getRange(2, 3, calendarColors.length, 1).setValues(calendarColors);
  }

  // Update events data in "Calendar" sheet
  const calendarRange = calendarSheet.getRange("A2:E82");
  calendarRange.clearContent();

  if (eventsData.length > 0) {
    calendarSheet.getRange(2, 1, eventsData.length, 5).setValues(eventsData);
  }
}

function hexToRgb(hex) {
  hex = hex.replace('#', '');
  const r = parseInt(hex.substring(0, 2), 16);
  const g = parseInt(hex.substring(2, 4), 16);
  const b = parseInt(hex.substring(4, 6), 16);
  return `${r}, ${g}, ${b}`;
}

function formatDate(date) {
  const options = { year: 'numeric', month: '2-digit', day: '2-digit' };
  return date.toLocaleDateString('ru-RU', options).replace(/\//g, '.');
}

function formatTime(date) {
  const options = { hour: '2-digit', minute: '2-digit', hour12: false };
  return date.toLocaleTimeString('ru-RU', options).slice(0, 5);
}