#include "ESP8266WiFi.h"
#include <Wire.h>

char buffer[20];
char* password = "0113216200";
char* ssid     = "LCC-WiFi";
String MyNetworkSSID = "LCC-WiFi"; // SSID you want to connect to Same as SSID
bool Fl_MyNetwork = false; // Used to flag specific network has been found
bool Fl_NetworkUP = false; // Used to flag network connected and operational.

extern "C" {
#include "user_interface.h"
}


void setup() {
  Serial.begin(115200);
  delay(2000); // wait for uart to settle and print Espressif blurb..
  // print out all system information
  Serial.print("Heap: "); Serial.println(system_get_free_heap_size());
  Serial.print("Boot Vers: "); Serial.println(system_get_boot_version());
  Serial.print("CPU: "); Serial.println(system_get_cpu_freq());
  Serial.println();
  Serial.println("OLED network Acquire Application Started....");
  //Wire.pins(int sda, int scl), etc
  Wire.pins(0, 2); //on ESP-01.
  Wire.begin();
  StartUp_OLED(); // Init Oled and fire up!
  Serial.println("OLED Init...");
  clear_display();
  sendStrXY(" DANBICKS WIFI ", 0, 1); // 16 Character max per line with font set
  sendStrXY("   SCANNER     ", 2, 1);
  sendStrXY("START-UP ....  ", 4, 1);
  delay(4000);
  Serial.println("Setup done");
}


void loop()
{
  if (!Fl_NetworkUP)
  {
    Serial.println("Starting Process Scanning...");
    Scan_Wifi_Networks();
    Draw_WAVES();
    delay(2000);

    if (Fl_MyNetwork)
    {
      // Yep we have our network lets try and connect to it..
      Serial.println("MyNetwork has been Found....");
      Serial.println("Attempting Connection to Network..");
      Do_Connect();

      if (Fl_NetworkUP)
      {
        // Connection success
        Serial.println("Connected OK");
        Draw_WIFI();
        delay(4000);
        clear_display(); // Clear OLED
        IPAddress ip = WiFi.localIP(); // // Convert IP Here
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        ipStr.toCharArray(buffer, 20);
        sendStrXY("NETWORK DETAILS", 0, 1);
        sendStrXY("NET: ", 1, 1);
        sendStrXY((ssid), 1, 6);
        sendStrXY("INT IP: ", 2, 1);
        sendStrXY((buffer), 3, 1); // Print IP to yellow part OLED

        WiFiClient ExtIPClient;
        char server[] = "www.icanhazip.com";
        char ExtIP;
        
        if(ExtIPClient.connect(server, 80)){
          Serial.println("Connected to icanhazip");
          ExtIPClient.println("GET /");
          ExtIPClient.println();

          delay(1000);
          String ExternalIP = "";
          int i = 1;
          while (ExtIPClient.available()) {
            char c;
            c = ExtIPClient.read();
            Serial.print(c);
            //sendStrXY(&c, 3, i);
            //i++;
            ExternalIP += c;
          }
          ExternalIP.toCharArray(&ExtIP, ExternalIP.length());
          Serial.print("EXT IP: ");
          Serial.println(ExternalIP);
          sendStrXY("EXT IP:", 4, 1);
          sendStrXY(&ExtIP, 5, 1);
        }
      }
      else
      {
        // Connection failure
        Serial.println("Not Connected");
        clear_display(); // Clear OLED
        sendStrXY("CHECK   NETWORK", 0, 1);
        sendStrXY("   DETAILS     ", 2, 1);
        sendStrXY("NO CONNECTION..", 6, 1); // YELLOW LINE DISPLAY
        delay(3000);
      }
    }
    else
    {
      // Nope my network not identified in Scan
      Serial.println("Not Connected");
      clear_display(); // Clear OLED
      sendStrXY("  MY  NETWORK  ", 0, 1);
      sendStrXY("  NOT FOUND IN ", 2, 1);
      sendStrXY("     SCAN      ", 4, 1);
      delay(3000);
    }
  }
  delay(5000);    // Wait a little before trying again
}
