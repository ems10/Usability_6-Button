/*
 * Covers all screens during the infusion
 */

//_________________________________________________________
// Logs start/pause/stop of infusion
void logInfusion(int type)
{
  String toLog = GetTime();

  switch(type) {
    case _infStart: toLog = String(toLog + "Infusion started");
      break;

    case _infPause: toLog = String(toLog + "Infusion paused");
      break;

    case _infStop: toLog = String(toLog + "Infusion stopped");
      break;

    case _infComplete: toLog = String(toLog + "Infusion completed");
      break;
  }

  String fileName = String("Boot");
  fileName = String(fileName + bootNum + ".log");

  File logFile = SD.open(fileName, FILE_WRITE);
  logFile.println(toLog);
  logFile.close();
}


//__________________________________________________________________________
// Removes all files from the onboard SD card

void ClearSD()
{
  File root = SD.open("/");

  while (true) {
    File entry = root.openNextFile();
    if (! entry) {
      root.rewindDirectory();
      break;
    }

    char *fileName = entry.name();
    Serial.println(fileName);
    entry.close();
    SD.remove(fileName);
  }

  root.close();
}

//__________________________________________________________________________________________________________
//This function forms the frame for the Running Infusion Screen  and calls the tftRunningValueScreen function which updates the time softpotvolume values, time,and checks on the different condition and alarm screens
unsigned long tftRunningScreen() {

  tft.setTextWrap(false);
  tft.fillScreen(HX8357_WHITE);
  tft.drawRect(0, 0, 480, 65, HX8357_BLACK);
  tft.fillRect(0, 0, 480, 64, HX8357_GREEN);
  tft.setTextSize(1);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setCursor(115, 25);
  tft.setTextColor(HX8357_BLACK);
  tft.print("Infusing: ");
  if (MgSO4){
    tft.print("20% MgSO4");
  }
  else {
    tft.print("Manual Mode");
  }
  tft.setFont(&FreeSans12pt7b);
  tft.setTextSize(1);
  if (MgSO4){
    tft.setCursor(95, 55);
    if (Maintenance) {
      tft.println("5 mL/hr Maintenance Dose");
    } else if (Loading) {
      tft.setCursor(110, 55);
      tft.println("60 mL/hr Loading Dose");
    }
  }
  else {
    tft.setCursor(200, 55);
    tft.print(TheRate);  
    tft.println(" mL/hr");
  }

  tft.setTextColor(HX8357_BLACK);
  
  // Current infused
  tft.setCursor(60, 130);
  tft.setTextSize(3);
  tft.print(TempTheoreticalVolume);
  tft.print(" mL");

  // Total to infuse
  tft.setTextSize(1);
  tft.setCursor(330, 105);
  tft.print("of ");
  tft.print(PrescribedVol);
  tft.print(" mL");
  tft.setCursor(335, 130);
  tft.print("infused");

  // Syringe information
  tft.setCursor(150, 175);
  tft.print(TheBrand);
  tft.print(" ");
  tft.print(TheSize);
  tft.print(" mL syringe");

  // Time remaining
  tft.setFont(&FreeSansBold12pt7b);
  DisplayTime();
  tft.setCursor(120, 220);
  tft.print(HoursLeft);
  tft.print(" hr ");
  tft.print(MinutesLeft);
  tft.print(" min remaining");


  // Button instructions
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(HX8357_WHITE);
  tft.fillRect(0, 240, 480, 80, HX8357_BLACK);
  tft.setCursor(10, 270);
  tft.println("To pause infusion and view menu,");
  tft.setCursor(25, 300);
  tft.println("  press BACK/PAUSE");
  
  logInfusion(_infStart); // Log the starting of the infusion
  tftRunningValueScreen();

}

//_______________________________________________________________________________________________
//Function is the main core of infusion
//It calls the ProtocolRun function which displays time left for infusion,volume infused,updates motor ticks and softpot,and checks for current protocol infusion complete and request refills
//Function checks for battery status,device malfunction, and occlusion.
//Function checks for overall completion of the Infusion
//Added MotorLoop to increase check points when MotorTime >=MotorPulse

//1_24_17
void tftRunningValueScreen()
{
  //ActiveProtocol==true;
   
  while (ActiveProtocol == true) { // Why does this while loop run MotorLoop up to 7 times?

    char key = keypad.getKey();
    bool flag = false;

    //Serial.println(loopcount);
    ProtocolRun();
    MotorLoop();

    //------------------------------------------------------
    //Checking Battery Status
    int SensorValue = analogRead(11);
    double BattVoltage = 1023; //SensorValue * (5.0 / 1023.0);
    double BattMax = 0.90;
    int BattPercent = 100; //(BattVoltage/BattMax)*100;

    if (BattPercent < BattMid)//Below 80% cautions low battery
    {
      tft.fillRect(0, 0, 240, 40, HX8357_YELLOW);
      tft.setCursor(5, 15);
      tft.setTextColor(HX8357_BLACK);
      tft.setTextSize(2);
      tft.println(" CAUTION:LOW BATTERY");
      tft.drawLine(0, 39, 240, 39, HX8357_WHITE);
      if (BattPercent < BattLow)//Below 60% cautions critical low battery and pauses infusion with alarm screen notification
      {
        if (PauseState == false)
        {
          PauseState == true;
          PauseStartTime = millis();
          Buzzer(2);
          tftBatteryScreen(); //Calls function to display the critical low battery alarm screen // TODO Make new battery screen?
        }
      }
    }
    MotorLoop();

    //---------------------------------------------------------------------
    //Checking Device Accuracy
    // TODO Currently not an issue
    TheoreticalVolume = (double)DispTime / (double)CurrentProtocolTime * (double)TotalVolume; //The theoretical volume
    ActualVolume = (double)SoftPotVolume ; //The actual volume from softpotR
    //DMERRORMALAWI DMerror== ((abs(ActualVolume-TheoreticalVolume))/TheoreticalVolume)*100;  //Changed from = 0 DMerror MALAWI
    //DMerror = 0; //((abs(TheoreticalVolume-ActualVolume))/TheoreticalVolume)*100;
         /* DMERROR MALAWI With this commented out, it may not sound the alarm until the tick after the error....
      if((DispTime>300000 && TheRate>30) || (DispTime>2400000&&TheRate<=30))//Due to initial error the device accuracy is checked only after 300000msec for fast flow rates and 1200s for slow
      {
        if (DMerror < DMerrorMax)
          {
            if (DMerror > DMerrorMin)//checking if the rate deviation is between  DMerrorMin(5%) and DMerrorMax(10%)
            {
            tft.fillRect(0, 0, 240,40, ILI9340_YELLOW); 
            tft.setCursor(0,15);tft.setTextColor(ILI9340_BLACK); tft.setTextSize(2); 
            tft.println("CAUTION:LOW ACCURACY");
            tft.drawLine(0, 39, 240,39, ILI9340_WHITE); 
            }
          }
         else if (DMerror >= DMerrorMax)//checking if the deviation is above DMerrorMax(10%)
              {
                Serial.println("DEVICE MALFUNC");   
                if (PauseState == false)
                {  
                 PauseState==true;
                 PauseStartTime=millis();
                 Buzzer(2);
                 tftDeviceMalFunScreen(); //calling the Device Malfunction Alarm Scrren
               }
              }
      }
      */ //DMERROR MALAWI
    //      
//2/8/2017 CHECKING FOR OCCLUSION ALARM

    //------------------------------------------------------
 

    //---------------------------------------------------------------------
    // TODO Currently not possible by contraints Erica set. Verify
    //Checking RefillSyringe Condition
    //If the Syringe Size is less than the Total Volume to be infused then refill is required.
    //So if CurrentProtocolTime is not equal to ProtocolTime then check the refill condition else if the CurrentProtocolTime is equal to ProtocolTime then no more refill is required.
    //This condition needs to be changed wrt the new condition ie remainder based protocol.
    if (CurrentProtocolTime != ProtocolTime)
    {
      MotorLoop();
      if (CurrentTimeLeft < TimeLeft)
      {
        MotorLoop();
        if (CurrentTimeLeft < 60000) // If the Current Protocol Time is less than 1 min then display Refill in 1 min
        {
          MotorLoop();
          tft.fillRect(0, 0, 240, 40, HX8357_YELLOW);
          tft.setCursor(0, 15);
          tft.setTextColor(HX8357_BLACK);
          tft.setTextSize(2);
          tft.println("  Refill in    min  ");
          DisplayCurrentTime(); //Change here to be MinutesLeft - Current ProtoRcol Time
          tft.setCursor(147, 15);
          tft.setTextColor(HX8357_BLACK);
          tft.setTextSize(2);
          tft.println(1); //(CurrentRemainingMinutes) - use for a greater count down
        }
        else if (CurrentTimeLeft < 300000)//If the Current Protocol Time is less than 5 min then display Refill Syringe Soon
        {
          MotorLoop ();
          tft.fillRect(0, 0, 240, 40, HX8357_YELLOW);
          tft.setCursor(5, 15);
          tft.setTextColor(HX8357_BLACK);
          tft.setTextSize(2);
          tft.println("Refill Syringe Soon");
          tft.drawLine(0, 39, 240, 39, HX8357_WHITE);
        }
      }
    }
    else
    {
      //If refill condition doesnt apply then display Infusing
      MotorLoop();
    }
    MotorLoop();
    MotorLoop();

    //Check if MotorTick is equal to MotorTickMax then quit from infusing
    if ((MotorTick == MotorTickMax))//||(DispTime > ProtocolTime+30000))//added time condition to end protocol once protocol goes over 30s
    {
      ActiveProtocol = false;
      ScreenStart = millis();
      Buzzer(2);
      logInfusion(_infComplete);
      tftInfusionComplete();//Call Infusion Complete Screen
    }
    // If a valid button is pressed and infusion is not currently paused, pause
    if (change && PauseState == false) {
      change = false;
      switch (key) {
        case 'B': PauseState = true;
          PauseStartTime = millis(); // Cannot put out of switch since invalid buttons exist
          LogKeyPress("Back", "Pause");
          logInfusion(_infPause);
          tftPauseScreen(); //tftAdjustInfusion and tftStopInfuion
          flag = true;
          break;
      }
    }
    if (flag) {
      break;
    }
    
  }

}


// Pause screen for infusion

void tftPauseScreen(){

  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.drawRect(0, 0, 480, 65, HX8357_BLACK);
  tft.fillRect(0, 0, 480, 64, HX8357_RED);
  tft.setTextSize(1);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setCursor(60, 25);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Infusion PAUSED: ");
  if (MgSO4){
    tft.print("20% MgSO4");
  }
  else {
    tft.print("Manual Mode");
  }
  
  tft.setFont(&FreeSans12pt7b);
  tft.setTextSize(1);
  if (MgSO4){
      tft.setCursor(95, 55);
    if (Maintenance) {
      tft.println("5 mL/hr Maintenance Dose");
    } else if (Loading) {
      tft.println("60 mL/hr Loading Dose");
    }
  }
  else {
        tft.setCursor(200, 55);
        tft.print(TheRate);
        tft.println(" mL/hr");
  }
  tft.fillRect(0, 66, 300, 400, HX8357_WHITE);
  tft.setTextColor(HX8357_RED);
  
  // Current infused
  tft.setCursor(30, 165);
  tft.setTextSize(3);
  tft.print(TempTheoreticalVolume);
  tft.print(" mL");

  // Total to infuse
  tft.setTextSize(1);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(75, 205);
  tft.print("of ");
  tft.print(PrescribedVol);
  tft.print(" mL infused");

  // Syringe information
  tft.setCursor(60, 255);
  tft.print(TheBrand);
  tft.print(" ");
  tft.print(TheSize);
  tft.print(" mL syringe");
  
  // Time remaining
  DisplayTime();
  tft.setCursor(35, 300);
  tft.print(HoursLeft);
  tft.print(" hr ");
  tft.print(MinutesLeft);
  tft.print(" min remaining");
  //tft.setFont(&FreeSans12pt7b);

  // Draw the triangles
  softButtons(3);

  // Add labels
  tft.setTextColor(HX8357_WHITE);
  menuOption(2, 1, ""); // See menuOption() for reason
  menuOption(2, 1, "Resume");
  menuOption(3, 1, "Adjust infusion"); // Allow to resume, adjust, or stop
  menuOption(4, 1, "Stop infusion");
//  //tft.println(SoftPotVolume); // Use SoftPotVolume for something?


  while(true){
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("2", "Resume");
          tftResume();
          tftRunningScreen(); // Logs starting
          flag = true;
          break;

        case '+': LogKeyPress("3", "Adjust infusion");
          tftResume();
          tftSubMenuInit();
          flag = true;
          break;

        case '#': LogKeyPress("4", "Stop infusion");
          tftStopInfusion();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }

}

// Screen to verify adjusting the infusion
void tftAdjustInfusion(){
   // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.setFont(&FreeSans12pt7b);
  tft.println(" Are you sure you want to");
  tft.println(" adjust the infusion?");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, "");
  menuOption(2, 1, "No");
  menuOption(3, 1, "Yes, go back to infusion selection");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("2", "No, don't adjust infusion");
          tftResume();  // Resume the infusion
          tftRunningScreen();
          flag = true;
          break;
        case '+': LogKeyPress("3", "Yes, go back to infusion selection");
          tftResume();
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

// Screen to verify stopping the infusion
void tftStopInfusion(){
   // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.setFont(&FreeSans12pt7b);
  tft.println(" Are you sure you want to");
  tft.println(" stop the infusion?");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, "");
  menuOption(2, 1, "No");
  menuOption(3, 1, "Yes, go back to main screen");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': LogKeyPress("2", "No, don't stop infusion");
          tftResume();  // Resume the infusion
          tftRunningScreen();
          flag = true;
          break;
        case '+': LogKeyPress("3", "Yes, go back to main screen");
          logInfusion(_infStop);
          tftResume();
          ResetInfusion();
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

//___________________________________________________________________________
//Resume Infusion after infusion paused manually
void tftResume()
{
  PauseState = false;
  PauseTime = millis() - PauseStartTime;
  DispStartTime = DispStartTime + PauseTime; // Update DispTime to eliminate time paused.
  MotorStartTime = MotorStartTime + PauseTime; // Update MotorTime to eliminate time paused.
  CurrentDispStartTime = CurrentDispStartTime + PauseTime; //Update CurrentDispTime to eliminate time paused
  Buzzer(2);
  // Calling the running screen again is the job of the calling function.
}

//___________________________________________________________________________
//Resume Infusion after infusion paused manually
void tftRefillResume()
{
  PauseState = false;
  PauseTime = millis() - PauseStartTime;
  DispStartTime = DispStartTime + PauseTime; // Update DispTime to eliminate time paused.
  MotorStartTime = MotorStartTime + PauseTime; // Update MotorTime to eliminate time paused.
  CurrentDispStartTime = CurrentDispStartTime + PauseTime; //Update CurrentDispTime to eliminate time paused
  int SoftPotReading = analogRead(SoftPotPin);
  SoftPotStart = SoftPotReading; //added to read the initial value as the same as the first value
  Buzzer(2);
  if (MgSO4) {
    tftMgSO4Mode();
    tftRunningScreen();
  }
  else {
    tftRunningScreen();
  }//Calls the tftRunningScreen function which calls the tftRunningValueScreen function whicn controls the Infusing
}

//____________________________TODO Fix this_____________________________________
//Infusion Completed Screen displaying the total volume infused, syringe size, flow rate and turning on the alarm alerting the infusion is complete
void tftInfusionComplete() {
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_WHITE);
  tft.drawRect(0, 0, 480, 65, HX8357_BLACK);
  tft.fillRect(0, 0, 480, 64, HX8357_BLUE);
  tft.setTextSize(2);
  tft.setCursor(60, 45);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Infusion Complete");

  tft.setTextSize(1);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(155, 115);
  tft.print("Mode:");
  tft.setCursor(125, 145);
  tft.print("Delivery:");
  tft.setCursor(65, 175);
  tft.print("Syringe Type:");
  tft.setCursor(40, 205);
  tft.print("Volume Infused:");
  tft.setFont(&FreeSans12pt7b);

  tft.setCursor(230, 115);
  if (MgSO4){
    tft.print("20% MgSO4");
    tft.setCursor(230, 145);
    if (Maintenance) {
      tft.println("5 mL/hr Maintenance Dose");
    } else if (Loading) {
      tft.println("60 mL/hr Loading Dose");
    }
  } else {
    tft.print("Manual");
    tft.setCursor(230, 145);
    tft.print(TheRate);
    tft.print(" mL/hr");
  }
  
  // Syringe Type
  tft.setCursor(230, 175);
  tft.print(TheBrand);
  tft.print(" ");
  tft.print(TheSize);
  tft.print(" mL");

  // Volume to infuse
  tft.setCursor(230, 205);
  tft.print(PrescribedVol);
  tft.print(" mL");

  // Button instructions
  tft.setTextColor(HX8357_WHITE);
  tft.fillRect(0, 240, 238, 80, HX8357_BLACK);
  tft.setCursor(15, 275);
  tft.println("To finish treatment");
  tft.println(" press BACK/PAUSE");
  tft.fillRect(242, 240, 238, 80, HX8357_BLACK);
  tft.setCursor(255, 275);
  tft.println("To start new infusion");
  tft.setCursor(310, 305);
  tft.print("press OK");

    while (true) {
    char key = keypad.getKey();
    bool flag = false;
    
    if (change) {
      change = false;
      switch (key) {
        case 'K': LogKeyPress("OK", "Set up new infusion");
          tftSubMenuInit();
          break;

        case 'B': LogKeyPress("Back", "Finish treatment");
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



void tftBatteryScreen()
{

  LogAlarm(_batt);  // Logs battery alarm
  
  // New menu construction with soft buttons
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 50);
  tft.setTextColor(HX8357_RED);
  tft.println("LOW BATTERY!");
  
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(1);
  tft.setCursor(50, 100);
  tft.print("Plug in device, then press 'Continue");
  tft.setCursor(180, 130);
  tft.println("treatment.'");
  

  
  

  // Draw the triangles
  softButtons(1);

  // Add labels
  menuOption(4, 1, ""); // See menuOption() for reason
  menuOption(4, 1, "Continue treatment");

  while(true){
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '#': LogKeyPress("4", "Continue treatment");
          tftResume();
          tftRunningScreen();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

//_____________________________TODO Figure this out_______________________________________________
//Infusion Complete Exit Screen Condition
//
//void tftInfusionCompleteCondition()
//{
//  tft.setCursor(0,270);
//  tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//  tft.setTextSize(2); 
//  tft.println("Back to MainSettings"); 
//  tft.setCursor(96,286);
//  tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//  tft.setTextSize(2); 
//  tft.println("(OK)");
//
//  KeyEvent=KeyControl();
//  switch (KeyEvent) // detects key pressed
//  {
//  case RightKey:
//  case LeftKey:
//    tft.setCursor(6,220);
//    tft.setTextColor(HX8357_BLACK, HX8357_WHITE);
//    digitalWrite(38, LOW);
//    tft.println("   -Alarm Muted-   ");     
//    break;
//    break;
//  } 
//  if(KeyEvent != SelectKey)
//  {
//    tft.setCursor(0,270);
//    tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//    tft.setTextSize(2); 
//    tft.println("Back to MainSettings");
//    tft.setCursor(96,286);
//    tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//    tft.setTextSize(2); 
//    tft.println("(OK)"); 
//    tftInfusionCompleteCondition();
//  }
//  
// else if(KeyEvent == SelectKey)
//     {
//      Buzzer(1);
//      ActiveProtocol = false; //***Removed PauseState=false, reset pause screen?, pause can be completed on future protocols - YES
//      ScreenStart = millis();
//      tft.fillScreen(HX8357_WHITE);
//      ResetValues(); 
//      tftSubMenuInit();                   // Draw menu ***Does not clear old menu settings 
//      submenu_select=1;                   // Select 1st menu item 
//      tftSubMenuSelect(submenu_select);  // Highlight the selected item
//      MenuModeNavigation();             //Displays the Menu Mode screen. Hit Exit to reset the screen and go to Menu Setting Screen to avoid overlapping of RunningValueScreen over Menu Setting Screen
//    }
//}
//
////___________________________________________________________________________________________________
////Infusion Complete Exit Screen Coondition for MgSO4
//
//void tftInfusionCompleteConditionMgSO4()
//{
//  tft.setCursor(15,270);
//  tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//  tft.setTextSize(2); 
//  if (Loading==true)
//      {tft.println("Begin Maintenance");
//      tft.setCursor(75,286);
//      tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//      tft.setTextSize(2); 
//      tft.println("Dose (OK)"); }
//  else
//    {tft.println("Back to MainSettings");  
//    tft.setCursor(96,286);
//    tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//    tft.setTextSize(2); 
//    tft.println("(OK)");}
//
//  KeyEvent=KeyControl();
//  switch (KeyEvent) // detects key pressed
//  {
//  case RightKey:
//  case LeftKey:
//    tft.setCursor(6,220);
//    tft.setTextColor(HX8357_BLACK, HX8357_WHITE);
//    digitalWrite(38, LOW);
//    tft.println("   -Alarm Muted-   ");     
//    break;
//    break;
//  } 
//  if(KeyEvent != SelectKey)
//  {
//    tft.setCursor(15,270);
//    tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//    tft.setTextSize(2); 
//    if (Loading==true)
//      {tft.println("Begin Maintenance");
//      tft.setCursor(75,286);
//      tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//      tft.setTextSize(2); 
//      tft.println("Dose (OK)"); }
//    else
//      {tft.println("Back to MainSettings");  
//      tft.setCursor(96,286);
//      tft.setTextColor(HX8357_YELLOW,HX8357_BLUE); 
//      tft.setTextSize(2); 
//      tft.println("(OK)"); }
//    tftInfusionCompleteConditionMgSO4();
//  }
//  
// else if(KeyEvent == SelectKey)
//     {
//      Buzzer(1);
//      ActiveProtocol = false; //***Removed PauseState=false, reset pause screen?, pause can be completed on future protocols - YES
//      ScreenStart = millis();
//      tft.fillScreen(HX8357_WHITE);
//      ResetValues(); 
//      tftMgSO4Mode();//Returns back to MgSO4 Manual Screen 
//      LDMDConditionSelect = 1;
//    }
//}

