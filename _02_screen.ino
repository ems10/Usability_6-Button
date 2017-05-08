

// ______________________________________________________________________________________________________
// FUNCTION DisplayTime
// DisplayTime is responsible for updating the LCD screen with the current protocol time.
// It takes in only the DispTime total millisecond value and then converts those to hours:mins:secs.
void DisplayTime()
{
  // TOTAL TIME
  int DisplayTotalHours, DisplayTotalMinutes , DisplayTotalSeconds;
  unsigned long TimeLeftover;

  DisplayTotalHours = int( ProtocolTime / 3600000 );
  TimeLeftover = ProtocolTime % 3600000;
  DisplayTotalMinutes = int(TimeLeftover / 60000);
  TimeLeftover = TimeLeftover % 60000;
  DisplayTotalSeconds = int(TimeLeftover / 1000);

  // CURRENT TIME
  int DisplayHours, DisplayMinutes, DisplaySeconds, LeftHours, LeftMinutes, LeftSeconds, CurrentLeftHours, CurrentLeftMinutes, CurrentLeftSeconds;
  //unsigned long TimeLeftover2;

  if (PauseState == false)
    DispTime = millis() - DispStartTime;
  TimeLeft = ProtocolTime - DispTime;

  TimeDisp = DispTime;

  DisplayHours = floor( (double)DispTime / (3600000) );
  DisplayMinutes = floor( ( (DispTime / 1000) % (3600) / 60 ) );
  DisplaySeconds = ceil(((DispTime / 1000) % (3600)) % 60);

  //Time Remaining based on Overall Protocol Time
  LeftHours = floor( (double)TimeLeft / (3600000) );
  LeftMinutes = floor( ( (TimeLeft / 1000) % (3600) / 60 ) );
  LeftSeconds = floor(((TimeLeft / 1000) % (3600)) % 60);

  HoursLeft = LeftHours;
  MinutesLeft = LeftMinutes;
}

// ______________________________________________________________________________________________________
// FUNCTION DisplayCurrentTime
// DisplayTime is responsible for updating the LCD screen with the current protocol time.
// It takes in only the DispTime total millisecond value and then converts those to hours:mins:secs.
void DisplayCurrentTime()
{

  // CURRENT TIME
  int CurrentLeftHours, CurrentLeftMinutes, CurrentLeftSeconds;
  //unsigned long TimeLeftover2;

  if (PauseState == false)
    CurrentDispTime = millis() - CurrentDispStartTime;
  CurrentTimeLeft = CurrentProtocolTime - CurrentDispTime;

  CurrentDisplayTime = CurrentDispTime;

  //Time Remaining based on Current Protocol Time
  CurrentLeftHours = floor( (double)CurrentTimeLeft / (3600000) );
  CurrentLeftMinutes = floor( ( (CurrentTimeLeft / 1000) % (3600) / 60 ) );
  CurrentLeftSeconds = ceil(((CurrentTimeLeft / 1000) % (3600)) % 60);

  CurrentRemainingMinutes = CurrentLeftMinutes;

}

//_____________________________________________________________________________________


// Helper functions for soft buttons

void softButtons(int numButtons) {
  // Designed to be responsive to different screen sizes (Using 480x320, landscape-320 height)

  int w = tft.width();
  int h = tft.height();
  int inc =  h / 16; // 20 in base
  int triHeight = inc * 2; // 40 in base
  int triWidth = triHeight / 3; // 13 in base

  // X-Axis bounds of triangles (invariant)
  int leftPointX = w - triWidth;
  int rightPointX = w;

  // Y-Axis bounds of triangle points (variant)
  int low = h - 10;//inc; // Changed for oval buttons
  int mid = low - inc;
  int top = mid - inc;

  /*
   * Lowering triangles to match new, smaller buttons
   */

  if (numButtons >= 1) { // Draw bottom button
    low = low - 2 * inc - 25; //move to draw button 3 first
    mid = low - inc;
    top = mid - inc;
    tft.fillTriangle(leftPointX, mid, rightPointX, low, rightPointX, top, HX8357_BLUE);
    low = low - 2 * inc - 25; // then to button 2
    mid = low - inc;
    top = mid - inc;

    if (numButtons >= 2) { // Draw third button
      tft.fillTriangle(leftPointX, mid, rightPointX, low, rightPointX, top, HX8357_BLUE);
      low = low + 2 * inc + 10;
      mid = low + inc;
      top = mid + inc;
      low = low + 2 * inc;
      mid = low + inc;
      top = mid + inc;

      if (numButtons >= 3) { // Draw second button
        tft.fillTriangle(leftPointX, mid, rightPointX, low, rightPointX, top, HX8357_BLUE);
        low = low - 2 * inc -40; // -4*inc for large square buttons
        mid = low - inc;
        top = mid - inc;
        low = low - 2 * inc; // -4*inc for large square buttons
        mid = low - inc;
        top = mid - inc;
        low = low - 2 * inc; // -4*inc for large square buttons
        mid = low - inc;
        top = mid - inc;

        if (numButtons >= 4) { // Draw top button
          tft.fillTriangle(leftPointX, mid, rightPointX, low, rightPointX, top, HX8357_BLUE);
        }
      }
    }
  }
}

void menuOption(int loc, int textSize, char* text)
{
  // Note that these require single-line text for each button
  // As before, made for the 480x320 screen, landscape
  int w = tft.width();
  int h = tft.height();
  int inc =  h / 16; // 20 in base
  int triWidth = (inc * 2) / 3; // 13 in base

  // Get the width of the text
  int16_t x1, y1;
  uint16_t tw, th; // Text width/height
  tft.getTextBounds(text, 30, 30, &x1, &y1, &tw, &th);
  
  // Preliminary text location info  // Changed from large square buttons to small oval ones
    // I am most upset by no longer using responsive variables and multiples of 5 (12 hurts)
  int topY = 4 * inc + 12 + (th/2);//2 * inc + (th / 2); // Y-loc for text at position 1

  // Location of text
  int xLoc = w - tw - triWidth - 10; // 5 counts as a buffer
  int yLoc = topY + ((inc * 2 + 25) * (loc - 1));

  // Write text
  tft.setTextSize(textSize);
  tft.fillRect(xLoc, yLoc-th, tw, th+5, HX8357_BLACK);
  tft.setCursor(xLoc, yLoc);
  tft.print(text);

}

//____________________________________________________________________
// Initial menu for setting up the infusion or changing alarm/occlusion settings
void tftMenuInit()
{
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.print(" Syringe Pump Setup");

  // Draw the triangles
  softButtons(3);

  // Add labels
  menuOption(2, 1, "");
  menuOption(2, 1, "Set up infusion");
  menuOption(3, 1, "Change alarm settings");
  menuOption(4, 1, "Change occlusion settings");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("2", "Set up infusion");
          submenu();
          flag = true;
          break;

        case '+': LogKeyPress("3", "Change alarm settings");
          tftAlarmSettings();
          flag = true;
          break;

        case '#': LogKeyPress("4", "Change occlusion settings");
          tftOcclusionSettings();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

//---------------ALARM SETTINGS------------------------

// Set/Mute occlusion and accuracy alarms
void tftAlarmSettings() {
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Change alarm settings:");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, "");
  menuOption(2, 1, "Sound audible alarms");
  menuOption(3, 1, "Mute all alarms");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("2", "Sound audible alarms");
          tftAlarmsSet();
          flag = true;
          break;

        case '+': LogKeyPress("3", "Mute all alarms");
          tftAlarmsMute();
          flag = true;
          break;

        case 'B': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}


void tftAlarmsSet() {
  // New menu construction with soft buttons
  tft.setFont(&FreeSans9pt7b); // Small-text screen
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 30);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Device will sound all alarms.");


  BuzzerOn = true;    // Set the buzzer on

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case 'B': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
        case 'K': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

void tftAlarmsMute() {
  // New menu construction with soft buttons
  tft.setFont(&FreeSans9pt7b); // Small-text screen
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 30);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" All alarms are muted.");

  BuzzerOn = false;    // Set the buzzer off

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case 'B': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
        case 'K': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}


//---------------OCCLUSION SETTINGS------------------------

// Change occlusion for child or adult
void tftOcclusionSettings() {
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println("Change occlusion");
  tft.print("settings:");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, "");
  menuOption(2, 1, "Adult threshold (8 psi)");
  menuOption(3, 1, "Child threshold (5 psi)");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("3", "Adult threshold (8 psi)");
          tftAdultOcclusion();
          flag = true;
          break;

        case '+': LogKeyPress("4", "Child threshold (5 psi)");
          tftChildOcclusion();
          flag = true;
          break;

        case 'B': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

void tftAdultOcclusion() {
  // New menu construction with soft buttons
  tft.setFont(&FreeSans9pt7b); // Small-text screen
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 30);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Occlusion threshold has");
  tft.println(" been changed to 'Adult");
  tft.println(" threshold (8 psi)'");



  for (int i = 0; i < 6; ++i)
  {
    BaselineThreshold[i] = AdultBaseline[i];
  }


  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case 'B': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
        case 'K': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

void tftChildOcclusion() {
  // New menu construction with soft buttons
  tft.setFont(&FreeSans9pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 30);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Occlusion threshold has");
  tft.println(" been changed to 'Child");
  tft.println(" threshold (5 psi)'");


  for (int i = 0; i < 6; ++i)
  {
    BaselineThreshold[i] = ChildBaseline[i];
  }


  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case 'B': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
        case 'K': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}



//________________________________________________________________________________________________
//Function inside Infusion Settings
//Shows up the 2 Reminder Screens follwed by the Different Modes of Infusion
void submenu() {

  //tft.fillScreen(HX8357_WHITE);
  //tftPLS();                            // Reminder Screen1 Function
  //tft.fillScreen(HX8357_WHITE);
  //tftPL();                             // Reminder Screen2 Function
  //tftMgSO4Mode();                         //Automatically go to MgSO4 Screen

  tftSubMenuInit();                    // Draw menu
  //  submenu_select=1;                    // Select 1st menu item
  //  tftSubMenuSelect(submenu_select);    // High selected menu item
  //  MenuModeNavigation();                //Function to control the navigation of modes in Infusion Settings

}

//__________________________________________________________________________________________________
//Function inside MgSO4 to remind nurses about concentration

void MgSO4Reminders() { // TODO Remove?
  if (Maintenance == false) {
    tft.fillScreen(HX8357_WHITE);
    tftLoadingRemind();                           // Reminder Screen for Loading dose
  }
  else
  { tft.fillScreen(HX8357_WHITE);
    tftMaintenanceRemind();
  }                       // Reminder Screen for maintenance dose
}

//__________________________________________________________________________________________________
//Reminder Screen for loading
int tftLoadingRemind() // TODO Remove?
{
  KeyEvent = KeyControl();
  tft.setCursor(0, 100);
  tft.setTextColor(HX8357_BLACK);
  tft.setTextSize(2);
  tft.println(" Loading dose ");
  tft.setCursor(0, 150);
  tft.println("     with ");
  tft.setCursor(0, 200);
  tft.println(" 4g of MgSO4? ");
  tft.setCursor(150, 280);
  tft.println(" (OK) ");

  if (KeyEvent != SelectKey)
  {
    tftLoadingRemind();
  }
}

//__________________________________________________________________________________________________
//Reminder Screen for maintenance dose
int tftMaintenanceRemind() // TODO Remove?
{
  KeyEvent = KeyControl();
  tft.setCursor(0, 50);
  tft.setTextColor(HX8357_BLACK);
  tft.setTextSize(2);
  tft.println(" Maintenance ");
  tft.setCursor(0, 100);
  tft.println(" dose loaded ");
  tft.setCursor(0, 150);
  tft.println(" with ");
  tft.setCursor(0, 200);
  tft.println(" 20% MgSO4? ");
  tft.setCursor(150, 280);
  tft.println(" (OK) ");

  if (KeyEvent != SelectKey)
  {
    tftMaintenanceRemind();
  }
}

//__________________________________________________________________________________________________
//Reminder Screen1
int tftPLS() // TODO Remove?
{
  KeyEvent = KeyControl();
  tft.setCursor(0, 100);
  tft.setTextColor(HX8357_BLACK);
  tft.setTextSize(2);
  tft.println("  Pull back ");
  tft.setCursor(0, 150);
  tft.println("    and ");
  tft.setCursor(0, 200);
  tft.println(" load syringe ");
  tft.setCursor(150, 280);
  tft.println(" (OK) ");

  if (KeyEvent != SelectKey)
  {
    tftPLS();
  }
}

//__________________________________________________________________________________________________
//Reminder Screen2
int tftPL() // TODO Remove?
{
  KeyEvent = KeyControl();
  tft.setCursor(50, 100);
  tft.setTextColor(HX8357_BLACK);
  tft.setTextSize(2);
  tft.println(" Prime ");
  tft.setCursor(65, 150);
  tft.println(" IV ");
  tft.setCursor(50, 200);
  tft.println(" line ");
  tft.setCursor(150, 280);
  tft.println(" (OK) ");

  if (KeyEvent != SelectKey)
  {
    tftPL();
  }
}

//________________________________________________________________________________________________
// Clear screen and display the menu for modes in Infusion Settings

void tftSubMenuInit()
{
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select");
  tft.println(" Infusion Mode");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, ""); // See menuOption() for reason
  menuOption(2, 1, "Manual");
  menuOption(3, 1, "MgSO4");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': MgSO4 = false;
          LogKeyPress("2", "Manual");
          tftMgSO4Brand();
          flag = true;
          break;

        case '+': LogKeyPress("3", "MgSO4");
          tftMgSO4Mode(); // MgSO4 variable change happens here
          flag = true;
          break;

        case 'B': LogKeyPress("Back", "Return to Pump Setup");
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

//_______________________________________________________________________________________________
//Adjust infusion settings(AIS) for the infusion after infusion paused manually
void tftAISOnOff()
{

  PauseState = false;
  PauseTime = millis() - PauseStartTime;
  DispStartTime = DispStartTime + PauseTime; // Update DispTime to eliminate time paused.
  MotorStartTime = MotorStartTime + PauseTime; // Update MotorTime to eliminate time paused.
  ActiveProtocol = false;
  Buzzer(2);
  if (!MgSO4)
  {
    tftMgSO4Brand();// Calls the Manual Mode Screen to change the variables for infusion, need to go to MgSO4
    // Changed to tftMgSO4Brand() following initial 2017 menu map
  }
  else
  {
    tftMgSO4Mode();
  }
}
//
//________________________________________________________________________
//Resets values on Manual Menu after user quits protocol
void ResetValues () {
  char SyrBrandSelect = 0; // Used as an index tracker.
  int SyrSelect = 4; // Used as an index tracker. //***Start at 60 = 4 in index, changed from 5ML = 0
  int TheRate = 10;    //***Change to 10 from 5
  int TotalVolume = 60; //***change to 60mL for ease of use from 5mL
  int TotalGrams = 4;
}

//________________________________________________________________________________________________
//Fuction for displaying the Occlusion Alarm Screen and calling the condition screen at the bottom

void tftOcclusionAlarmScreen()
{

  LogAlarm(_occ); // Logs the occlusion alarm
  
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_WHITE);
  tft.drawRect(0, 0, 480, 65, HX8357_BLACK);
  tft.fillRect(0, 0, 480, 64, HX8357_RED);
  tft.setTextSize(2);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setCursor(5, 45);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Occlusion Detected!");
  
  tft.setTextSize(1);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(105, 95);
  tft.print("Check patient and IV line");
  tft.setFont(&FreeSans12pt7b);
  
  // Button instructions
  tft.setTextColor(HX8357_WHITE);
  tft.fillRect(0, 240, 238, 80, HX8357_BLACK);
  tft.setCursor(20, 275);
  tft.println("To QUIT infusion");
  tft.println(" press BACK/PAUSE");
  tft.fillRect(242, 240, 238, 80, HX8357_BLACK);
  tft.setCursor(275, 275);
  tft.println("To resume infusion");
  tft.setCursor(310, 305);
  tft.print("press OK");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case 'K': LogKeyPress("K", "Resume infusion");
          tft.setFont(&FreeSans12pt7b);
          tftResume();
          tftRunningScreen();
          flag = true;
          break;

        case 'B': LogKeyPress("B", "Quit infusion");
          tft.setFont(&FreeSans12pt7b);
          tftMenuInit();
          flag = true;
          break;         
      }
    }

    if (flag) {
      break;
    }
  }
}


  
  //OLD CODE
  /*
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setFont(&FreeSans9pt7b);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_RED);
  tft.println(" OCCLUSION DETECTED");
  tft.println(" Check patient and IV line");
  tft.setTextColor(HX8357_WHITE);
  
  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(3, 1, ""); // See menuOption() for reason
  //menuOption(1, 1, "Mute alarm"); USABILITY TEST...TOOK OUT MUTE ALARM OPTION
  menuOption(2, 1, "Resume infusion");
  menuOption(3, 1, "Quit infusion");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
//        case '-': LogKeyPress("1", "Mute alarm");
//          tft.setFont(&FreeSans12pt7b);
//          tftMuteAlarm();
 //         flag = true;
//          break;

        case '+': LogKeyPress("2", "Resume infusion");
          tft.setFont(&FreeSans12pt7b);
          tftResume();
          tftRunningScreen();
          flag = true;
          break;

        case '#': LogKeyPress("3", "Quit infusion");
          tft.setFont(&FreeSans12pt7b);
          tftMenuInit();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

*/

// Mute Alarm and return to previous menu
void tftMuteAlarm(){
   digitalWrite(38, LOW); // Set to high to turn alarm back on

  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Alarm has been muted");

  // Draw the triangles
  softButtons(1);

  // Add labels
  menuOption(4, 1, ""); // See menuOption() for reason
  menuOption(4, 1, "OK");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
        switch (key) {
        case '#': LogKeyPress("4", "OK");
          tftOcclusionAlarmScreen();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
   
}

//_______________________________________________________________________

//Fuction for displaying the Low Battery Screen and calling the condition screen at the bottom

void tftLowBatteryAlarmScreen()
{
  
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setFont(&FreeSans9pt7b);
  tft.setTextSize(2);
  tft.setCursor(60, 40);
  tft.setTextColor(HX8357_YELLOW);
  tft.println("CAUTION: Low Battery");
  tft.setCursor(90, 120);
  tft.println("Plug in power cord");

  // Button instructions
  tft.setTextColor(HX8357_WHITE);
  tft.fillRect(242, 240, 238, 80, HX8357_BLACK);
  tft.setCursor(270, 275);
  tft.println("To continue");
  tft.setCursor(280, 305);
  tft.print("press 'OK'");
  
  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false; 
      switch (key) {
        case 'K': LogKeyPress("OK", "Continue after Low Battery Screen");
          tftMenuInit();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}


// ______________________________________________________________________
// FUNCTION Battery Display
// This functions controls the LEDs for the battery power as well as the battery charge
// indicator on the LCD screen.
void BatteryDisplay() // TODO Fix so this actually gets a reading
{
  int SensorValue = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  double BattVoltage = SensorValue * (5.0 / 1023.0);
  // 0.78 corresponds to 7.4 V
  // 0.87 corresponds to 8.3 V
  // From the battery a line comes that runs through a voltage divider with a 9.1K and 1.1Kohm resistors
  // The voltage measured by the Arduino is from the 1.1kohm resistor
  // going to the led the 5V goes across an 180 ohm resistor then to the LED.
  // Arbitrary temporary decision to set 7V (Arduino = 0.75) as cut-off for LOW battery (of the 7.4V battery).
  // If LOW, green = off, red = on.
  // If GOOD, green = on, red = off.

  // LOW BATTERY
  if (BattVoltage <= 4.5785) //Low battery if voltage is <50% or 
  {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, HIGH);
  }
  else
    // MID-HIGH BATTERY
  {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  }

  // BATTERY PERCENTAGE INDICATOR
  double BattMin = 4.0; //Range of 7-8.4 V
  double BattMax = 4.995; 
  int BattPercent = 87; //USABILITY ((BattVoltage-BattMin)/BattMax)*100;

  tft.setCursor(150, 290);
  tft.setTextColor(HX8357_YELLOW);
  tft.setTextSize(1);
  tft.print("BATTERY=");
  tft.print(BattPercent);
  tft.print('%');
  
  if (BattPercent < 50)
  { tftLowBatteryAlarmScreen();
  }



}



//___________________________________________________________________________________________
//Function to initialize the MgSO4 Mode
void tftMgSO4Mode(void)
{
  MgSO4 = true; //Changed menu to MgSO4 menu

  
  tftMgSO4Check();

}

//___________________________________________________________________________________________
// Function to check whether the syringe and line have 20% MgSO4
void tftMgSO4Check()
{
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Are the syringe and");
  tft.println(" IV line loaded with");
  tft.println(" 20% MgSO4?");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, ""); // See menuOption() for reason
  menuOption(2, 1, "Yes");
  menuOption(3, 1, "No");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("2", "Yes");
          tftMgSO4Dose();
          flag = true;
          break;

        case '+': LogKeyPress("3", "No");
          tftMgSO4Fill();
          flag = true;
          break;

        case 'B': LogKeyPress("4", "Back to mode selection");
          tftSubMenuInit();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

void tftMgSO4Fill() {
  // New menu construction with soft buttons
  tft.setFont(&FreeSans9pt7b); // Small-text screen
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Fill syringe and IV line with");
  tft.println(" 20% MgSO4.");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(3, 1, ""); // See menuOption() for reason
  menuOption(2, 1, "Filled, continue");
  menuOption(3, 1, "Back to mode selection");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("2", "Filled, continue");
          tft.setFont(&FreeSans12pt7b); // Small-text screen
          tftMgSO4Dose();
          flag = true;
          break;

        case '+': LogKeyPress("3", "Back to mode selection");
          tft.setFont(&FreeSans12pt7b); // Small-text screen
          tftMgSO4Mode();
          flag = true;
          break;

        case 'B': LogKeyPress("4", "Back to mode selection");
          tftMgSO4Mode();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

void tftMgSO4Dose()
{
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Dose");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, ""); // See menuOption() for reason
  menuOption(2, 1, "4g Loading Dose (20 min)");
  menuOption(3, 1, "1g/hr Maintenance Dose");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': Loading = true;
          Maintenance = false;
          SyrSelect = 3;
          TheRate = 60;
          LogKeyPress("2", "4g Loading Dose (20 min)");
          tftMgSO4Brand();
          flag = true;
          break;

        case '+': Maintenance = true;
          SyrSelect = 5;
          TheRate = 5;
          LogKeyPress("3", "1g/hr Maintenance Dose");
          tftMgSO4Brand();
          flag = true;
          break;

        case 'B': LogKeyPress("Back", "Back to mode selection");
          tftSubMenuInit();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}
