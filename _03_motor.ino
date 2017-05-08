// _______________________
// FUNCTION ProtocolRun()
// Responsible for controlling the motor during the set protocols. Acts as loop that can be easily updated
// in order to connect / include in the Arduino "void loop()."
void ProtocolRun()
{
    if (TheRate < 10.5) {
      MotorPulse = (100 / ((TheRate / SyrCalibSlow[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //usability //DevX=20
      MotorDelay = 200;
    }
    if (TheRate < 20.5 && TheRate > 10.5) {
      MotorPulse = (100 / ((TheRate / SyrCalib3[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //USABILITY //DevX=20
      MotorDelay = 200;
    }
    if (TheRate > 20.5 && TheRate < 40.5) {
      MotorPulse = (100 / ((TheRate / SyrCalib30[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //USABILITY DevX=20
      MotorDelay = 200;
    }
    if (TheRate > 40.5 && TheRate < 65) {
      MotorPulse = (100 / ((TheRate / SyrCalib[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //USABILITY //DevX=20
      MotorDelay = 200;
    }
    if (TheRate > 65) {
      MotorPulse = (100 / ((TheRate / SyrCalibBolus[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //USABILITY //DevX100;
      MotorDelay = 992;
    }
  
  
  MotorLoop();


  // IF NOT PAUSED AND NOT DONE


  if ((PauseState == false) && (MotorTick < MotorTickMax) )

  {
    MotorLoop();
    // If it's time to tick, activate motor and update SoftPot values.
  }

  //Display Volume
  if (CurrTheoreticalVolume != TempTheoreticalVolume){
    CurrTheoreticalVolume = TempTheoreticalVolume;
    tft.fillRect(0, 70, 310, 80, HX8357_WHITE);
    tft.setCursor(60, 130);
    tft.setTextSize(3);
    tft.setTextColor(HX8357_BLACK);       
    tft.print(TempTheoreticalVolume);
    tft.print(" mL");
  }
  // tft.println(SoftPotVolume);
  MotorLoop();

  //Display Total Time Left
//  DisplayTime();

  if (HoursLeft >= 0) { //only positive times are displayed on LCD screen
    DisplayTime();
    if (CurrMinutesLeft != MinutesLeft){
      CurrMinutesLeft = MinutesLeft;
      tft.setFont(&FreeSansBold12pt7b);
      tft.fillRect(120, 220, 350, 10, HX8357_WHITE);
      tft.setCursor(120, 220);
      tft.setTextSize(1);
      tft.setTextColor(HX8357_BLACK);
      tft.print(HoursLeft);
      tft.print(" hr ");
      tft.fillRect(170, 200, 30, 30, HX8357_WHITE);
      tft.print(MinutesLeft);
      tft.print(" min remaining");
      tft.setFont(&FreeSans12pt7b);
    }
  }

  //Display Current Time Left
  DisplayCurrentTime();
  MotorLoop();
  //if CurrentProtocol is done then request for refill
  if (((MotorTickMax != MotorTick) && (CurrentMotorTickMax == CurrentMotorTick))) //***||(CurrentDispTime > CurrentProtocolTime+30000))//Will stop protocol if time runs more than 30sec or reaches max
  {
    Serial.println(CurrentDispTime);

    if (CurrentDispTime >= CurrentProtocolTime)
    {
      if (PauseState == false)
      {
        PauseState = true;
        PauseStartTime = millis();
        CurrentDispTime = 0; //***need to reset CurrentDispTime back to 0 before each refill
        CurrentDispStartTime = millis(); //maybe CurrentDispTime = millis-CurrentDispStartTime??
        //  SoftPotVolumeInfused = SoftPotVolume;
        SoftPotVolumeInfused = TheoreticalVolume;
        Serial.println (CurrentDispTime);
        Serial.println (CurrentProtocolTime);
        //ActiveProtocol = false;
        //Buzzer(1);
//        tftRefillManual();//Call the Refill based Paused Screen // TODO Figure out refills
      }
    }
  }
}



//________________________________________________________________________________________________
//FUNCTION MotorLoop
//Turns on motor loop for timer and helps increase check points when motorTime >= MotorPulse
//1_24_17
void MotorLoop() {
  MotorTime = millis() - MotorStartTime; // Update MotorTime

  if (MotorTime >= MotorPulse - 10)
  {

    PremotorPressurePadVoltage = analogRead(PressurePadPin) * (5.0 / 1023.0);
       //Checking for Pressure Occlusion
    OcclusionAlarm(CurrentMotorTick);
    MotorTickRun(); // Executes 1 motor tick: 180 degress, 100 steps, 992 milliseconds
    //      Serial.print(MotorTime);
    //      Serial.print ("  ");
    //      Serial.println(DispTime);
    PostmotorPressurePadVoltage = analogRead(PressurePadPin) * (5.0 / 1023.0);
    MotorStartTime = millis();
    MotorTime = 0;
    CurrentMotorTick = CurrentMotorTick + 1;
    MotorTick = MotorTick + 1;
    //Alert
    SoftPotUpdate(CurrentMotorTick);
    TempTheoreticalVolume = TheoreticalVolume;
 
  }
}


// ______________________________________________________________________________________________________
// FUNCTION MotorTickRun()
// This function acts to initialize the stepper motor pins to be able to run the motor.
// This function activates the motor to turn 180 degrees, or 100 steps, in 1 second.
// It was separated as its own function in order to keep the motor control in one area,
// rather than spread through the protocols.

// Not sure how this will work. May need to initalize earlier and use global variables.

void MotorTickRun()
{

  AccelStepper stepper(1, stepperpin1, stepperpin2); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
  // AccelStepper mystepper(1, pinStep, pinDirection);
  // A stepper motor controlled by a dedicated driver board.
  stepper.setMaxSpeed(1000);

  digitalWrite(37, HIGH); // wakes the motor up, added a 1ms delay because the driver needs that to get out of sleep
  delay(1);
  stepper.move(MotorRotate); // distance set is in steps
  stepper.setSpeed(100); // speed is in steps/seconds = takes 1 second to go 100 steps
  while (stepper.distanceToGo() != 0) // distance from the current position to the target position
  {
    stepper.runSpeedToPosition(); // runs at the currently selected speed until the target position is reached
  }
  digitalWrite(37, LOW); //CHANGED TO ELIMIATE ELSE LOW STATEMENT IN PROTOCOL RUN
}




//____________________________________________________________________________________________________________
//Start Infusion Function initializes the variables triggering the motor and softpot
void tftStartInfusion(void)
{  
  if (ActiveProtocol == false)
  {
    ScreenStart = millis();
    // PROTOCOL START
    // Calibration equation based on selected flow rate.
    if (TheRate < 10.5) {
      MotorPulse = (100 / ((TheRate / SyrCalibSlow[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //DevX=20
      MotorDelay = 200;
    }
    if (TheRate < 20.5 && TheRate > 10.5) {
      MotorPulse = (100 / ((TheRate / SyrCalib3[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //DevX=20
      MotorDelay = 200;
    }
    if (TheRate > 20.5 && TheRate < 40.5) {
      MotorPulse = (100 / ((TheRate / SyrCalib30[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //DevX=20
      MotorDelay = 200;
    }
    if (TheRate > 40.5 && TheRate < 65) {
      MotorPulse = (100 / ((TheRate / SyrCalib[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //DevX=20
      MotorDelay = 200;
    }
    if (TheRate > 65) {
      MotorPulse = (100 / ((TheRate / SyrCalibBolus[SyrSelect]) * 200 * .0002777) * 1000); //Does not include 992
      MotorRotate = 60; //100;
      MotorDelay = 992;
    }

    CurrentProtocolTime = ((double)TheSize / (double)TheRate) * 3600000; // calculates current protocol run time in milliseconds
    CurrentMotorTickMax = CurrentProtocolTime / MotorPulse; // calculates total number of current protocol motor ticks

    //MgSO4 vs. Manual and LD vs. MD
    if (Maintenance == true) {
      ProtocolTime = 86400000;
    }//24 hr protocol for maintenance dose
    else {
      ProtocolTime = ((double)TotalVolume / (double)TheRate) * 3600000;
    } // calculates total protocol run time in milliseconds

    RefillNumber = ((double)TotalVolume / (double)TheSize); //counts number of refills
    RefillRounding(); //rounds up number to include partial refills


    MotorTickMaxCalc = ProtocolTime / MotorPulse; // calculates total number of motor ticks
    MotorTickMax = round (MotorTickMaxCalc); //Rounds number to int
    ActualRefillTicks = ((double)MotorTickMax / (double)RefillNumber); //untruncated number of ticks/refill
    MotorPulse = MotorPulse - MotorDelay; // Correct MotorPulse to include duration of motor tick (992 milliseconds), add additional delays 1.136

    DispTime = 0;
    DispStartTime = millis();

    Serial.println(MotorPulse);
    Serial.println(MotorTickMax);
    Serial.println(ProtocolTime);
    Serial.println(CurrentMotorTickMax);//Reset in RefillManual to have multiple refills

    CurrentDispTime = 0;
    CurrentDispStartTime = millis();
    PauseTime = 0;
    MotorTime = 0;
    MotorStartTime = millis() - MotorPulse * 0.5;
    MotorPauseTime = 0;
    CurrentMotorTick = 0;
    MotorTick = 0;
    SoftPotVolumeInfused = 0;
    ActiveProtocol = true;


    Serial.print("DispTime ");
    // Serial.print("Time ");
    Serial.print("Ticks   ");
    Serial.print("SP   ");
    Serial.print("mL ");
    Serial.print("  PressurePad ");
    Serial.print("Accuracy");
    Serial.print(" Theo. Volume");
    Serial.println(); // sets up data acquisition

    SoftPotUpdate(CurrentMotorTick);

    Buzzer(1);
    if (MgSO4) {
      //MgSO4Reminders(); //check that LD or MD concentration in syringe and IV bag
      tftRunningScreen();
    }//Function starting the infusion.This function should be commented if it is included in the Main Loop.
    //Currently the Infusion is triggered through tftStartInfusion function.
    else
      tftRunningScreen();
  } else {
    tftRunningScreen(); //TODO Maybe remove?
  }

}
