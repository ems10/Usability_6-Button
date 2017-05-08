
// ___________________________________________________________________
// DEVICE VERSION
int AutoSyPversion = 2; // variable used to define which version of AutoSyP, which will change a few things:
// 1. AccelStepper pin connections
// 2. SoftPot pin power (34,36)
//Both devices set for V3
int PressureVersion = 2; //1 = adult, 2 = babybuz
// ___________________________________________________________________
// HARDWARE SETTINGS
// KEY VALUES AND SUCH
// The following key value order is based on the progression of the ADC values.

#define RightKey  0
#define UpKey     1
#define DownKey   2
#define LeftKey   3
#define SelectKey 4
#define NoKey     5

#define RightKeyADCUpper   688
#define RightKeyADCLower   678
#define UpKeyADCUpper      784
#define UpKeyADCLower      774
#define DownKeyADCUpper    860
#define DownKeyADCLower    850
#define LeftKeyADCUpper    900
#define LeftKeyADCLower    890
#define SelectKeyADCUpper  937
#define SelectKeyADCLower  927

#define NUM_KEYS  5

int KeyEvent = NoKey;
int KeyADC;
int LastKey = NoKey;
unsigned long KeyControlTime = 0;
#define KeyNormalRefresh 50   // Key detection rate when no key has been pressed
#define KeyPressRefresh  200   // Key detection rate when a key has been pressed (longer)
int KeyControlThreshold = KeyNormalRefresh;

//................................................
//SYRINGE SIZES AND CALIBRATIONS
int SyrSize[6] = { 
  1, 5, 10, 20, 30, 60 };//Different Syringe Sizes
int SyrSelect = 5; // Used as an index tracker.***CHANGED FROM 5mL=0 to 60mL=4 //TODO Check & Remove
int SyrSelectMax = 5; // Total number of syringe sizes minus 1. Look into using sizeof(). (CHANGED FROM 6)
int TheSize = 60; // TheSize is the user-selected syringe size. //TODO Reset to 0 
double SyrCalib3 [6];
double SyrCalibSlow[6];
double SyrCalib[6];
double SyrCalib30[6];
double SyrCalib40[6];
double SyrCalibBolus[6];

// V2 NUMBERS
double V2SyrCalib20 [6] = {
  .08, .080, .1194, .1434, .4750, .4           }; // mL/rev for flow rates 10-20mL/hr
double V2SyrCalib30 [6] = {
  .08, .080, .1194, .1434, .4876, .4          }; // mL/rev for flow rates 20-40mL/hr
double V2SyrCalibSlow [6] = {
  .07, .07980, .1300, .1434, .4686, .9747          }; // mL/rev for flow rates <= 10mL/hr //MALAWI
double V2SyrCalib [6] = {
 .08, .089, .14, .4929, .4962, .4          }; // mL/rev for flow rates > 40mL/hr ***UPDATED 20cc SYRINGE AND 60cc SYRINGE FOR DEVX
double V2SyrCalibBolus [6] = {
 .08, .089, .14, .1434, .4937, 1.686          }; //if rate is >60mL/hr

// V3 NUMBERS
double V3SyrCalib20 [6] = {
  .08, .080, .1194, .2857, .4750, .282           }; // mL/rev for flow rates 10-20mL/hr
double V3SyrCalib30 [6] = {
  .08, .080, .1194, .2857, .4876, .282           }; // mL/rev for flow rates 20-40mL/hr
double V3SyrCalibSlow [6] = {
  .07, .07980, .1300, .2857, .4686, .282           }; // mL/rev for flow rates <= 10mL/hr
double V3SyrCalib [6] = {
 .089, .089, .14, .2857, .4962, .282          }; // mL/rev for flow rates > 40mL/hr
//.................................
//Brand //TODO TheBrand should be initialized to null
String TheBrand = "B.D."; // TheBrand is the user-selected syringe brand. Needs to be converted to 2D array.
String SyrBrand[3]={
  "B.D.","Monoject"," Terumo "}; //Different Syringe Brands
char SyrBrandSelect = 0; // Used as an index tracker.
char SyrBrandSelectMax = 2; // Total number of syringe brands minus 1. 

// ...............................
// RATES
int TheRate = 10;   // TheRate is the user-selected syringe flow rate. BECAUSE THERE IS ONLY ONE RATE: THE RATE.
int MinRate = 1;  // Defining the minimum flow rate
int MaxRate = 60; // Defining the maximum flow rate // TODO Check & Remove
int MotorRotate = 20; //Sets stepper.move  DevX=20
int MotorDelay = 15;

// ...............................
// VOLUMES
int TotalVolume = 60;   // TotalVolume is the user-selected total volume in the syringe.
int PrescribedVol = 60; // PrescribedVol is the user-selected volume to be infused.
int MinVolume = 0;    // Defining the minimum volume that could be infused.
int MaxVolume = 60;  // Defining the maximum volume that could be infused. ***CHANGE TO 60 from 200

//................................
// PRESSURE
float PremotorPressurePadVoltage = 0; // "Low point" in pressure; Taken in motorloop function
float PostmotorPressurePadVoltage = 0; // "High point" in pressure; Taken in motorloop function
double KinkPressureThreshold = .9; //USABILITY 4.0; //  Threshold to determine kink NEED TO MAKE THIS AN ARRAY FOR DIFFERENT SYRINGE SIZES
int MinPressureThreshold = 100; // Defining the minimum PressureThreshold.
int MaxPressureThreshold = 500; // Defining the maximum PressureThreshold.
double BaselineVoltage = 0.0; 
double BaselineVoltage1 = 0.0;
double BaselineVoltage2 = 0.0; 
double BaselineVoltage3 = 0.0; 
double GradualThreshold = 0.0; 
double BaselineThreshold [6];//Matrix for baseline differences
int OCTickMax = 3; 
boolean NoOC = true; //could change to a boolean (true/false)?
//double AdultBaseline [5]; //Differences added to baseline voltage based on adult or child
//double ChildBaseline [5];
// Baseline numbers
double ChildBaseline [6] = {
 4, 4, 4, 4, 4, 4 }; //USABILITY 
 // .5, 1, 1, 1.39, 3, 3.23           }; // Voltage diff. for each syringe size, only calibrated for 300 mmHg for sizes 20 and 60cc
double AdultBaseline [6] = {
 4, 4, 4, 4, 4, 4 }; //USABILITY
 // .5, 1, 1, 1.39, 3, 3.23           }; // Voltage diff. for each syringe size, 

int PressureThreshold = 100; //change this to the off option  

//.................................
//RefillScreenCondition
String TheRefillManualCondition; // TheRefillManualCondition is the user-selected refill condition.
String RefillManualCondition[2]={
  "Refill & Resume (OK)","Quit Infusion   (OK)"}; // Different refill conditions.
char RefillManualConditionSelect = 0; // Used as an index tracker.
char RefillManualConditionSelectMax = 1; //Total number of refill screen condition minus 1. 
double TheRemainder = 0; //remainder to calculate refill ticks
double RefillNumber = 0; //counts number of refills
double RRefillNumber = 0;// rounded refill number
double Overflow = 0; //calculate overflow from rounding
int RefillCount = 1; //counts refills that have been completed
double ActualRefillTicks = 0; //Actuall value of ticks, not truncated
double ExtraTicks = 0;
int AddedProtocolTick = 0; //Protocol with 1 tick added to end

//.................................
//QuitScreenCondition
String TheQuitCondition;  // TheQuitCondition is the user-selected quit condition.
String QuitCondition[2]={
  "YES   (OK)","NO   (OK)"}; // Different quit conditions where YES denotes quitting out the infusion and No denotes going back to the paused or alarm screens.
char QuitConditionSelect = 0; // Used as an index tracker.
char QuitConditionSelectMax = 1; //Total number of quit screen condition minus 1.

//.................................
//BatteryScreenCondition
String TheAlarmCondition;  // TheAlarmCondition is the user-selected Critical Battery Alarm Screen condition.
String AlarmCondition[2]={
  "Resume Infusion (OK)","Quit Infusion   (OK)"};// Different Critical Battery Alarm Screen conditions.
char AlarmConditionSelect = 0; // Used as an index tracker.
char AlarmConditionSelectMax = 1; //Total number of battery screen condition minus 1.

//...................................
//LDMDScreenCondition
String TheLDMDCondition;
String LDMDCondition [2]={
  "< Loading Dose >", "<Maintenance Dose>"};
String LDMDEndCondition [2]={
  "-  Loading Dose  -", "-Maintenance Dose-"};
signed char LDMDConditionSelect = 0;
char LDMDConditionMax = 2; 
boolean MgSO4 = false; 
int TotalGrams = 4; 
boolean Maintenance = false; 
boolean Loading = true;

// ...........................
// LEDs
int redLED = 28; // initialize Arduino pin for red LED
int greenLED =30;// initialize Arduino pin for green LED

// ...............................
// TIME INITIALIZATION
// Initializes all of the variables used in the timekeeping functions.
// There are three parallel times being monitored: the overall protocol time left, current protocol time left and the local motor time.
unsigned long DispTime = 0;
unsigned long CurrentDispTime = 0;
signed long TimeLeft = 0;
signed long CurrentTimeLeft = 0; // Reset all to 0
long DispStartTime = 0;
long CurrentDispStartTime = 0;
long PauseTime = 0;
long PauseStartTime = 0;
long MotorTime = 0; 
long MotorStartTime = 0;
long ScreenTime = 0;
unsigned long ProtocolTime = 0;
unsigned long CurrentProtocolTime = 0;
int stepCount = 0;         // number of steps the motor has taken
long ScreenStart = millis();
unsigned long ScreenSleep = 60*1000; // Sleep timer, multiplied by 1000 for milliseconds
boolean SleepState = false;
int HoursLeft=0;
int MinutesLeft=0;
int CurrentRemainingMinutes = 0;
float CurrentDisplayTime=0;
float TimeDisp=0;
int loopcount = 0;
int CurrMinutesLeft = 0; // Used to update remaining time only once the minute changes

// ...............................
// RATE ACCURACY VARIABLE INITIALIZATION
double TheoreticalVolume;//The expected flow rate in msec
double ActualVolume;//The calculated flow rate in msec
double DMerror; //Deviation error percentage between the expected and calculated flow rates
double DMerrorMax=500; //Maximum limit of error percentage
double DMerrorMin=5; //Minimum limit of error percentage

// ...............................
//BATTERY VARIABLE INITIALIZATION
int BattMid=85; //Define the Battery Percent level below which the "CAUTION:LOW BATT" condition should be displayed.
int BattLow=45; //Define the Battery Percent level below which the "CRITICALLY LOW BATT" alarm screen should be displayed and infusion to be stopped. (CHANGE TO 75%) 
// ...............................
// OTHER VARIABLE INITIALIZATION
boolean ActiveProtocol = false; // TODO was true, why?
boolean PauseState = false;

//................................
// MOTOR-SPECIFIC VARIABLES
double MotorPulse = 0; 
int MotorTick = 0; // Dynamic counting variable for the motor ticks.
int CurrentMotorTick = 0; // Dynamic counting variable for the current motor ticks.
double MotorTickMaxCalc = 0; // Static variable for the motor tick limit.
int MotorTickMax = 0; //Static variable for rounded motor tick limit.
int CurrentMotorTickMax = 0; // Static variable for the current motor tick limit.
int RefillMotorTickMax = 0; //Value needed to calculate TheRemainder
int MotorPauseTime = 0;
int stepperpin1=39; //motor pins
int stepperpin2 = 41; //motor pins


// ..............................
// SOFTPOT VARIABLES
int SoftPotPin = A3; // initializes pin for SoftPot
int SoftPotStart = 0; // initial position value
float SoftPotVolume = 0.000; // mL dispensed value
float SoftPotVolumeInfused= 0.000;//mL dispensed value before each refills
float TempTheoreticalVolume = 0.000;
float CurrTheoreticalVolume = 0.000; // Used to check if volume has changed
int SoftPotVolumeRound; //rounding mL dispensed to be displayed on the paused screens
int OldSoftPotReading = 0; //MALAWI

int TempTheoreticalVolumeRound;
double mLtoSPSlow[6]; //general slow sp matrix
double mLtoSPSlow2[6] = { // mL to SP conversion factor <10mL/hr for V2
   -0.2, -.022, -0.031, -0.053, -0.070, -0.14166}; //MALAWI
double mLtoSPSlow25[6] = { // mL to SP conversion factor <10mL/hr for V2.5
   -0.2, -.022, -0.031, -0.053, -0.070, -0.106};
   
double mLtoSP[6]; //general Softpot matrix 
double mLtoSP25[6] = { // mL to SP conversion factor for rates > 10mL/hrdouble mLtoSPSlow[5] = { // mL to SP conversion factor <10mL/hr
   -0.2, -.022, -0.0316, -0.07078, -0.072, -0.1099};   //Values for V2.5 >10mL/hr
double mLtoSP2[6] = {
   -0.2, .022, -0.0316, -0.0735, -0.072, -0.1099};  //Values for V2 >10mL/hr //MALAWI

//
// ...............................
// PRESSURE VARIABLES
int PressurePadPin = A2; // pin for analog read-in of Pressure Pad
//float PressurePadVoltage = 0.000; // voltage of pressure pad
//................................
// Setting Menu matrix
int Settings[3]={
      1,2,3}; //3 levels of settings, 1 = protocol, 2 = Size, 3 = Vol. in syringe 
int SetSelect = 0; 
int SetSelectMax = 2; 
int set=1; //starts settings at the first selection

//................................
// BUZZER

boolean BuzzerOn = true; // boolean that controls whether will buzzer will beep at all



//................................
// Keypad pressed

boolean change = false;


//................................
// Variables and constants for logging purposes
int bootNum;
#define _infStart     0
#define _infPause     1
#define _infStop      2
#define _infComplete  3
#define _occ          4
#define _batt         5
