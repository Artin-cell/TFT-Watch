#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TFT.h>



#ifndef WI_FI
#define WI_FI
// Задайте параметры WiFi
const char* ssid = ""; // Define the WiFi SSID
const char* password = ""; // Define the WiFi password


int utc = 10800;
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
  
  // Выведите время в последовательный монитор
  //tft.println("Текущее время: " + formattedTime);
  //delay(10000);


}
  

  

#endif
