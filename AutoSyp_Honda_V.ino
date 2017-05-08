
#include <gfxfont.h>

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_HX8357.h"// Hardware-specific library
//#include <Adafruit_ILI9340.h> // Hardware-specific library, Smaller screen
#include <Fonts/FreeSans9pt7b.h> // Sans-Serif font for small-text screens
#include <Fonts/FreeSans12pt7b.h> // Sans-Serif font for large-text screens
#include <Fonts/FreeSansBold12pt7b.h> // Bold Sans-Serif for when bold is needed
#include <SPI.h>

// BEGIN CARD READING
#include <SD.h>
//END CARD READING

#include <AccelStepper.h> // library to control stepper motor
#include <Keypad.h>
#include "AutoSyp_Honda_V.h"



/* 
REPLACED A15 WITH A11
 */

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

#define _sclk 52
#define _miso 50
#define _mosi 51
#define _cs   9        // Chip select line for TFT display/pin 
#define _dc   10       // Data/command line for TFT
#define _rst  11       // Reset line for TFT (or connect to +5V)
#define _tftccs  8    // Card CS for screen
//#define lcdBacklight  10

Adafruit_HX8357 tft = Adafruit_HX8357(_cs, _dc, _rst);

#include "constants.h"

void setup(void) 
{
  // BEGIN CARD READING
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  //END CARD READING
  
  
  //TFT initialize
  Serial.begin(9600);
  while (!Serial);
  Serial.println("init");
  
  //BEGIN CARD READING FOR BOOT NUMBER
  if (!SD.begin(_tftccs)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
  } else {

    // Uncomment these to clear the logs
//    ClearSD();
    
    
    Serial.println("Wiring is correct and a card is present.");
    File boot = SD.open("boot.log", FILE_WRITE);
    String numStr;
    if(boot.size() == 0){
      char *toWrite = "Boot number is: "; // Initialize with basic info
      boot.write(toWrite);
      bootNum = 1;
    } else { // Assuming this is not the first boot
      boot.seek(16); // Go to the boot number
      int toRead = boot.size()-16;
      char count[toRead];
      boot.read(count, toRead);
      numStr = String(count); // Get the count from the data
      bootNum = numStr.toInt() + 1; // Change the value of the boot number
      Serial.print("Boot number: ");
      Serial.println(bootNum);
    } 
    boot.seek(16);
    boot.print(bootNum);
    boot.close();

    String fileName = String("BootNumber");
    fileName = String(fileName + bootNum + ".log");
    
    File logs = SD.open(fileName, FILE_WRITE);
    logs.write("Boot number: ");
    logs.println(bootNum);
    logs.println("");
    logs.close();
  }  
  //END CARD READING

  tft.begin(0xD);
  tft.setRotation(1);
  tft.setFont(&FreeSans12pt7b);
  //.........................................................................  
  //Welcome Screen Display
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(120, 80);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(2);
  tft.println("   Kasupe   ");
  tft.setCursor(90, 130);
  tft.println("Syringe Pump");

  tft.setTextSize(1);
  tft.setCursor(130, 220);
  tft.print("Software version 1.1.");
  tft.print(bootNum);
  delay(1000);

  //Initialize pins 
  pinMode(23, OUTPUT);//LED
  pinMode(27, OUTPUT);  
  pinMode(31, OUTPUT);//MOTOR
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(28, OUTPUT);//Softpot
  pinMode(29, OUTPUT);
  pinMode(34, OUTPUT);//Softpot
  pinMode(36, OUTPUT);
  pinMode(48, OUTPUT);//Set TFT and SD Chip Select pins as output

  digitalWrite(23, LOW);
  digitalWrite(27, LOW);
  digitalWrite(31, LOW);
  digitalWrite(33, LOW);
  digitalWrite(35, LOW);

  pinMode(redLED, OUTPUT);//Battery
  pinMode(greenLED, OUTPUT);  
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);

  pinMode(40, OUTPUT);//Buzzer was 38
  pinMode(34, OUTPUT);//Buzzer was 44
  digitalWrite(34,LOW); //Turns on in Buzzer function was 44

  //Set event listener for keypad
  keypad.addEventListener(KeyPress);

  //Checking battery Status and Displaying on the Welcome Screen      
  BatteryDisplay();
  delay(2500);




  //Selecting the right version for the device 
  VersionCheck();
  
  //Selecting Pressure Threshold mode
//  PressureCheck (); 



  //screen setup
  //analogWrite(lcdBacklight, 255);   // Turn Backlight on full
  tft.fillScreen(HX8357_BLACK);     // Fill screen with black // TODO Uncomment


  while(true){
    tftMenuInit();
  }
}

//_________________________________________________________________________________________
//Main loop
//Currently only set to run MenuSettingNavigate. The RunningScreen is triggered through MenuSettingNavigate>>StartInfusion
void loop() 
{
  //   if (ActiveProtocol == false)
  //      {
  tftMenuInit();

  //       }
  //    
  //    if(ActiveProtocol == true)
  //        {
  //        Serial.print (MotorPulse);
  //        tftRunningScreen();
  //        }
}


