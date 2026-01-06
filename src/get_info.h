#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TFT.h>
#include "config_secret.h"



#ifndef WI_FI
#define WI_FI


int utc = TIMEZONE_OFFSET;
// Создайте экземпляры WiFiUDP и NTPClient
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utc);

#endif

#ifndef UPTIME
#define UPTIME
// Ensure formattedTime is declared before usage

String formattedTime; // Define formattedTime as a global variable
void uptime() {
  // Обновите время
  timeClient.update();

  // Получите текущее время
  formattedTime = timeClient.getFormattedTime(); // Update the global variable
  
}
  

  

#endif
