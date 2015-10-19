#include "ESP8266WiFi.h"
#include <Wire.h>

extern "C" {
#include "user_interface.h"
}

const char* CURRENT_SSID_CHAR_FULL; //Full SSID
char* CURRENT_SSID_CHAR; //Truncated SSID
      
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Heap: "); Serial.println(system_get_free_heap_size()); //Print some basic information
  Serial.print("Boot version: "); Serial.println(system_get_boot_version());
  Serial.print("CPU frequency: "); Serial.println(system_get_cpu_freq());
  Serial.println();

  Wire.pins(0, 2); //Configure and enable I2C
  Wire.begin();

  startOLED();
  clearDisplay();
  sendStrXY("WiFi Network Scan", 0, 1);
  delay(4000);
}

void loop() {
  clearDisplay();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); //Disconnect from any AP, just to be sure
  
  delay(100);
  
  int NETWORKS_FOUND = WiFi.scanNetworks();
  Serial.println("WiFi network scan complete");
  Serial.print("No. of networks found: "); Serial.println(NETWORKS_FOUND);

  if (NETWORKS_FOUND != 0) {
    for (int i = 0; i < NETWORKS_FOUND; i++) {
      if (i > 5) {
        break; //We only want 5 on the OLED
      }

      CURRENT_SSID_CHAR_FULL = WiFi.SSID(i); //Copy to variable

      Serial.println(CURRENT_SSID_CHAR_FULL); //Prints SSID fine
      
      os_strncpy(CURRENT_SSID_CHAR, CURRENT_SSID_CHAR_FULL, 15); //This is where the code fails with an exception, trying to truncate the string to 15 chars
      
      Serial.print("SSID #"); Serial.print(i); Serial.print(": "); Serial.println(CURRENT_SSID_CHAR);
      //sendStrXY(CURRENT_SSID_CHAR, i, 0);
      delay(10);
    }
  }

  delay(10000); //update every 10 seconds
}
