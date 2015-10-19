#include "OLEDFont.h"
#define OLED_address  0x3c  //OLED I2C bus address... even if OLED states 0x78 !!!

static void resetDisplay(void)
{
  displayOff();
  clearDisplay();
  displayOn();
}


void startOLED()
{
  init_OLED();
  resetDisplay();
  displayOff();
  setXY(0,0);
  clearDisplay();
  displayOn();
}


void displayOn(void)
{
    sendCommand(0xaf);        //display on
}


void displayOff(void)
{
  sendCommand(0xae);    //display off
}


static void clearDisplay(void)
{
  unsigned char i,k;
  for(k=0;k<8;k++)
  { 
    setXY(k,0);    
    {
      for(i=0;i<128;i++)     //clear all COL
      {
        sendChar(0);         //clear all COL
      }
    }
  }
}


// Actually this sends a byte, not a char to draw in the display. 
static void sendChar(unsigned char data) 
{
  Wire.beginTransmission(OLED_address);  // begin transmitting
  Wire.write(0x40);                      //data mode
  Wire.write(data);
    Wire.endTransmission();              // stop transmitting
}


// Prints a display char (not just a byte) in coordinates X Y,
static void sendCharXY(unsigned char data, int X, int Y)
{
  //if (interrupt && !doing_menu) return; // Stop printing only if interrupt is call but not in button functions
  setXY(X, Y);
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  
  for(int i=0;i<8;i++)
    Wire.write(pgm_read_byte(myFont[data-0x20]+i));
    
  Wire.endTransmission();    // stop transmitting
}


static void sendCommand(unsigned char com)
{
  Wire.beginTransmission(OLED_address);     //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
}


// Set the cursor position in a 16 COL * 8 ROW map.
static void setXY(unsigned char row,unsigned char col)
{
  sendCommand(0xb0+row);                //set page address
  sendCommand(0x00+(8*col&0x0f));       //set low col address
  sendCommand(0x10+((8*col>>4)&0x0f));  //set high col address
}


// Prints a string regardless the cursor position.
static void sendStr(unsigned char *string)
{
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      sendChar(pgm_read_byte(myFont[*string-0x20]+i));
    }
    *string++;
  }
}


// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
static void sendStrXY( char *string, int X, int Y)
{
  setXY(X,Y);
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      sendChar(pgm_read_byte(myFont[*string-0x20]+i));
    }
    *string++;
  }
}


// Inits oled and draws logo at startup
static void init_OLED(void)
{
  sendCommand(0xae);                //display off
  sendCommand(0xa6);                //Set Normal Display (default)
    sendCommand(0xAE);              //DISPLAYOFF
    sendCommand(0xD5);              //SETDISPLAYCLOCKDIV
    sendCommand(0x80);              // the suggested ratio 0x80
    sendCommand(0xA8);              //SSD1306_SETMULTIPLEX
    sendCommand(0x3F);
    sendCommand(0xD3);              //SETDISPLAYOFFSET
    sendCommand(0x0);               //no offset
    sendCommand(0x40 | 0x0);        //SETSTARTLINE
    sendCommand(0x8D);              //CHARGEPUMP
    sendCommand(0x14);
    sendCommand(0x20);              //MEMORYMODE
    sendCommand(0x00);              //0x0 act like ks0108
    //sendCommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
    sendCommand(0xA0);
    //sendCommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
    sendCommand(0xC0);
    sendCommand(0xDA);              //0xDA
    sendCommand(0x12);              //COMSCANDEC
    sendCommand(0x81);              //SETCONTRAS
    sendCommand(0xCF);
    sendCommand(0xd9);              //SETPRECHARGE 
    sendCommand(0xF1);
    sendCommand(0xDB);              //SETVCOMDETECT                
    sendCommand(0x40);
    sendCommand(0xA4);              //DISPLAYALLON_RESUME        
    sendCommand(0xA6);              //NORMALDISPLAY             

  clearDisplay();
  sendCommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
  //  sendCommand(0xa0);    //seg re-map 0->127(default)
  //  sendCommand(0xa1);    //seg re-map 127->0
  //  sendCommand(0xc8);
  //  delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendCommand(0xa7);  //Set Inverse Display  
  // sendCommand(0xae);   //display off
  sendCommand(0x20);            //Set Memory Addressing Mode
  sendCommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
  // sendCommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)  
  
 //  setXY(0,0);
  // Display Logo here :)
//  for(int i=0;i<128*8;i++)     // show 128* 64 Logo
 // {
  //  sendChar(pgm_read_byte(logo+i));
 // }
 // sendCommand(0xaf);    //display on
  
 // delay(5000); 
}
