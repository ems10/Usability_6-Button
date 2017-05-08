// __________________________________________________________________________________________
// FUNCTION GetTime
// Returns a String of the current time since the most recent boot
// Meant for logging information

String GetTime()
{
  int hour = 0;
  int minute = 0;
  int second = 0;
  unsigned long timeLeft;
  String logtime;

  // Begin Timestamp
  unsigned long theTime = millis();
  hour = int(theTime / 3600000);
  logtime = String(hour);
  timeLeft = theTime - (hour*3600000); // Now dealing with minutes
  minute = int(timeLeft / 60000);
  timeLeft = timeLeft - (minute*60000); // Now dealing with seconds
  second = int(timeLeft / 1000);

  logtime = String(logtime + ":" + minute + ":" + second + " - "); // Form of "h:m:s - "

  return logtime;
  
}

void LogAlarm(int type)
{
  String toLog = GetTime();

  switch(type) {
    case _occ: toLog = String(toLog + "Occlusion alarm sounded");
      break;

    case _batt: toLog = String(toLog + "Battery alarm sounded");
      break;
  }

  String fileName = String("Boot");
  fileName = String(fileName + bootNum + ".log");

  File logFile = SD.open(fileName, FILE_WRITE);
  logFile.println(toLog);
  logFile.close();
}

// __________________________________________________________________________________________
// FUNCTION VersionCheck
// Meant to correct the changes between versions.
void VersionCheck()
{
  switch (AutoSyPversion)
  {
    case 2:
      digitalWrite(28, LOW); // for V2 SoftPot
      digitalWrite(29, HIGH); // for V2 SoftPot

      for (int i = 0; i < 6; ++i)
      {
        SyrCalib3[i] = V2SyrCalib20[i];
        SyrCalibSlow[i] = V2SyrCalibSlow[i];
        SyrCalib[i] = V2SyrCalib[i];
        SyrCalib30[i] = V2SyrCalib30[i];
        SyrCalibBolus[i] = V2SyrCalibBolus[i];
        mLtoSP[i] = mLtoSP2[i];
        mLtoSPSlow[i] = mLtoSPSlow2[i];
      }

      //    stepperpin1 = 49; //changed to 39
      //    stepperpin2 = 47; //changed to 41
      break;

    case 3:
      digitalWrite(36, LOW); // for V3 SoftPot(?)//53
      digitalWrite(34, HIGH); // for V3 SoftPot(?)//50

      for (int i = 0; i < 6; ++i)
      {
        SyrCalib3[i] = V3SyrCalib20[i];
        SyrCalibSlow[i] = V3SyrCalibSlow[i];
        SyrCalib[i] = V3SyrCalib[i];
        SyrCalib30[i] = V3SyrCalib30[i];
        mLtoSP[i] = mLtoSP25[i];
        mLtoSPSlow[i] = mLtoSPSlow25[i];
      }
      //    stepperpin1 = 39;
      //    stepperpin2 = 41;
      break;
  }
}

// ______________________________________________________________________________________________________
// FUNCTION SoftPotUpdate()
// Runs all of the necessary SoftPot functions, serves to compartmentalize.
void SoftPotUpdate( int CurrentMotorTick )
{

  int SoftPotReading = analogRead(SoftPotPin);
  int SoftPotResistance = SoftPotReading*4.8/1023*50/(4.8-(SoftPotReading*4.8/1023)); //Calculating the resistance from the voltage divider for the rotary pot

  if (CurrentMotorTick == 0)
  {
    SoftPotStart = SoftPotResistance;
  } //added to read the initial value as the same as the first value

  // calculates volume dispensed with SP conversion values and rate
  if (TheRate <= 10) {
    SoftPotVolume = ((float)SoftPotResistance * mLtoSPSlow[SyrSelect]) - ((float)SoftPotStart * mLtoSPSlow[SyrSelect]) ; //calculations for slow rates
  }
  else {
    SoftPotVolume = ((float)SoftPotResistance * mLtoSP[SyrSelect]) - ((float)SoftPotStart * mLtoSP[SyrSelect]) ; //calculations fo rfast rates
  }

  SoftPotVolume = SoftPotVolume + SoftPotVolumeInfused; //to addup the Volume Infused between Refills

  // Pressure Pad Reading
  float PressurePadVoltage = analogRead(PressurePadPin) * (5.0 / 1023.0);

 //TheoreticalVolume= (double)DispTime/(double)ProtocolTime*(double)TotalVolume; //The theoretical volume //DMERROR MALAWI
  ActualVolume= (double)SoftPotVolume ; //The actual volume from softpot 
  DMerror=((abs(TheoreticalVolume-ActualVolume))/TheoreticalVolume)*100; 


  Serial.print(DispTime / 1000);
  Serial.print("        ");
  Serial.print(MotorTick);
  Serial.print("       ");
  Serial.print(SoftPotReading);
  Serial.print("    ");
  Serial.print(SoftPotVolume);
  Serial.print("    ");
  Serial.print(PremotorPressurePadVoltage);
  Serial.print("     ");
  Serial.print(DMerror);
  Serial.print("      ");
  Serial.print(TheoreticalVolume);
  Serial.println();// ALL values
  //        TheoreticalVolume= (double)DispTime/(double)ProtocolTime*(double)TotalVolume; //The theoretical volume
  //        ActualVolume= (double)SoftPotVolume ; //The actual volume from softpot
  //        DMerror=((abs(TheoreticalVolume-ActualVolume))/TheoreticalVolume)*100;

}

// ______________________________________________________________________________________________________
// FUNCTION Buzzer(beeps)
// Powers buzzer on and off to beep the input number of beeps.
void Buzzer(int beeps)
{
  if (BuzzerOn == true)
  {
    boolean sound = false;
    beeps = beeps * 2; // doubles to also have a silent period
    for (int b = 0; b < beeps; b++)
    {
      sound = !sound; // alternates whether it will beep or not
      digitalWrite(40, sound); // buzzer on was 38
      delay(100); // duration of beep
    }
  }
  digitalWrite(34, LOW); // to make sure that it stops beeping
  digitalWrite(40, LOW); // to make sure that it stops beeping
}



//_______________________________________________________________________________________________
//Pressure Control:Set the user-selected pressure threshold
int tftPressure() // TODO Confer about removal. No replacement menu, but path no longer extant
{

  tft.fillRect(0, 200, 240, 60, HX8357_WHITE);
  tft.setCursor(0, 230);
  tft.setTextColor(HX8357_BLACK);
  tft.setTextSize(2);
  tft.println ("     <    mmHg> (OK)");
  tft.setCursor(72, 230);
  tft.println (PressureThreshold);

  while (true) {
    char key = keypad.getKey();
    if (change) {
      if (key == '>') {
        if ((PressureThreshold >= 100) && (PressureThreshold < 500))
          PressureThreshold += 50;
        else if (PressureThreshold == 500)
          PressureThreshold = 100;
      } else if (key == '<') {
        if ((PressureThreshold <= 500) && (PressureThreshold > 100))
          PressureThreshold -= 50;
        else if (PressureThreshold == 100)
          PressureThreshold = 500;
      } else if (key == 'E') {
        tft.fillRect(0, 200, 240, 60, HX8357_WHITE);
        tft.setCursor(0, 230);
        tft.setTextColor(HX8357_BLUE);
        tft.setTextSize(2);
        tft.println ("     -    mmHg-     ");
        tft.setCursor(72, 230);
        tft.println (PressureThreshold);
        break;
      }
      if (key == '>' || key == '<') {
        tft.fillRect(0, 200, 240, 60, HX8357_WHITE);
        tft.setCursor(0, 230);
        tft.setTextColor(HX8357_BLACK);
        tft.setTextSize(2);
        tft.println ("     <    mmHg> (OK)");
        tft.setCursor(72, 230);
        tft.println (PressureThreshold);
      }
    }
  }
}

//
//________________________________________________________________________________________________________
//Rounds up for partial refills to ensure total volume is delivered
void RefillRounding () {
  RRefillNumber = round (RefillNumber); //rounds number for partial syringe refills
  Overflow = RefillNumber - RRefillNumber; //determine if number is rounded up or down
  if (Overflow > 0) {
    RefillNumber = RRefillNumber + 1; //add to rounded number
  }
  else {
    RefillNumber = RRefillNumber;  //keep rounded number
  }
}





// ______________________________________________________________________________________________________
// FUNCTION OcclusionAlarm
void OcclusionAlarm(int CurrentMotorTick)
{

  // Threshold Alarm


  if (CurrentMotorTick == OCTickMax - 2) { //Triggers alarm for gradual presure increases after 10 motor ticks, MotorTick-1? for increased pressures?
    BaselineVoltage1 = PremotorPressurePadVoltage;
  }
  if (CurrentMotorTick == OCTickMax - 1) { //Triggers alarm for gradual presure increases after 10 motor ticks, MotorTick-1? for increased pressures?
    BaselineVoltage2 = PremotorPressurePadVoltage;
  }
  if (CurrentMotorTick == OCTickMax) { //Triggers alarm for gradual presure increases after 10 motor ticks, MotorTick-1? for increased pressures?
    BaselineVoltage3 = PremotorPressurePadVoltage;
    NoOC = true;
  }

/* //TAKEN OUT FOR USABILITY STUDY
  if (CurrentMotorTick > OCTickMax && NoOC == true) {
    BaselineVoltage = (BaselineVoltage1 + BaselineVoltage2 + BaselineVoltage3) / 3;
    GradualThreshold = BaselineVoltage + BaselineThreshold [SyrSelect];
   
    if (PremotorPressurePadVoltage >= GradualThreshold) {
      PauseState = true;
      PauseStartTime = millis();
      PremotorPressurePadVoltage = BaselineVoltage; //get rid of this? how do we refresh?, change to =BaselineVoltage
      //                NoOC=1;
      //                OCTickMax=CurrentMotorTick+10 //Not needed, kink alarm will catch all calibration ticks
      Buzzer(5);
      Serial.println ("OCCLUSION");
      Serial.println (GradualThreshold);
      digitalWrite(37, LOW); // Ensure motor is  off
      tftOcclusionAlarmScreen();
    }
    
  }
*/

  if (PremotorPressurePadVoltage > KinkPressureThreshold && TheSize==30) //Detects for kinks in IV line at all times //FOR USABILITY STUDY, MADE IT ONLY OCCLUDE FOR 30CC SYRINGE
  {
    if (PauseState == false)
    {
      PauseState = true;
      PauseStartTime = millis();
      PremotorPressurePadVoltage = BaselineVoltage; //Change to equal baseline voltage?
      Buzzer(3);
      NoOC = false;
      OCTickMax = CurrentMotorTick + 10;
      Serial.println(OCTickMax);
      Serial.println ("KINK");
      tftOcclusionAlarmScreen(); //Call the Refill based Paused Screen
    }

    digitalWrite(37, LOW); // Ensure motor is off
  }

}

/*
 * void ResetInfusion
 * 
 * Used when stopping an infusion and starting a new one, in order to reset all variables
 * not changed in the menus.
 */

 void ResetInfusion(void)
 {
    // SoftPot variables
    SoftPotStart = 0; // initial position value
    SoftPotVolume = 0.000; // mL dispensed value
    SoftPotVolumeInfused= 0.000;//mL dispensed value before each refills
    TempTheoreticalVolume = 0.000;
    CurrTheoreticalVolume = 0.000; // Used to check if volume has changed
    TheoreticalVolume = 0.000;
    ActualVolume = 0.000;

    // Motor variables
    MotorPulse = 0; 
    MotorTick = 0; // Dynamic counting variable for the motor ticks.
    CurrentMotorTick = 0; // Dynamic counting variable for the current motor ticks.
    MotorTickMaxCalc = 0; // Static variable for the motor tick limit.
    MotorTickMax = 0; //Static variable for rounded motor tick limit.
    CurrentMotorTickMax = 0; // Static variable for the current motor tick limit.
    RefillMotorTickMax = 0; //Value needed to calculate TheRemainder
    MotorPauseTime = 0;

    // Time progress variables
    TimeLeft = 0;
    CurrentTimeLeft = 0; // Reset all to 0
    DispStartTime = 0;
    CurrentDispStartTime = 0;
    PauseTime = 0;
    PauseStartTime = 0;
    MotorTime = 0; 
    MotorStartTime = 0;
    ScreenTime = 0;
    ProtocolTime = 0;
    CurrentProtocolTime = 0;
    stepCount = 0;         // number of steps the motor has taken
    HoursLeft=0;
    MinutesLeft=0;
    CurrentRemainingMinutes = 0;
    CurrentDisplayTime=0;
    TimeDisp=0;
    loopcount = 0;
    CurrMinutesLeft = 0; // Used to update remaining time only once the minute changes
 }

