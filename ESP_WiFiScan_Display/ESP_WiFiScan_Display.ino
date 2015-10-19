#include "ESP8266WiFi.h"
#include <Wire.h>

extern "C" {
#include "user_interface.h"
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Heap: "); Serial.println(system_get_free_heap_size());
  Serial.print("Boot version: "); Serial.println(system_get_boot_version());
  Serial.print("CPU frequency: "); Serial.println(system_get_cpu_freq());
  Serial.println();

  Wire.pins(0, 2);
  Wire.begin();

  startOLED();
  clearDisplay();
  sendStrXY("WiFi Network Scan", 0, 1);
  delay(4000);
}

void loop() {
  clearDisplay();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int NETWORKS_FOUND = WiFi.scanNetworks();
  Serial.println("WiFi network scan complete");
  Serial.print("No. of networks found: "); Serial.println(NETWORKS_FOUND);

  if (NETWORKS_FOUND != 0) {
    for (int i = 0; i < NETWORKS_FOUND; i++) {
      if (i > 5) {
        break;
      }

      String CURRENT_SSID_STRING = WiFi.SSID(i);
      char* CURRENT_SSID_CHAR;
      
      //CURRENT_SSID_STRING.toCharArray(CURRENT_SSID_CHAR, CURRENT_SSID_STRING.length());
      
      Serial.print("SSID #"); Serial.print(i); Serial.print(": "); Serial.println(WiFi.SSID(i));
      //sendStrXY(CURRENT_SSID_CHAR, i, 0);
    }
  }

  delay(10000); //update every 10 seconds
}
