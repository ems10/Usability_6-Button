/* New */
const byte ROWS = 6; //six rows
const byte COLS = 1; //one column
char keys[ROWS][COLS] = {
  {'*'},
  {'-'},
  {'+'},
  {'#'},
  {'B'},
  {'K'}
};
byte rowPins[ROWS] = {6, 5, 4, 3, 2, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// ______________________________________________________________________
// FUNCTION LogKeyPress
// Called when a button is pressed. Logs the time, button pressed (1-6), and option chosen

void LogKeyPress(String button, String option)
{
  String logging = GetTime();

  if (option.length() > 0) // Change to a parameter for soft/hard button, then propagate through code
    logging = String(logging + "Button " + button + " pressed: '" + option + "'");
  else
    logging = String(logging + button + " button pressed");

  // Log the button press
  String fileName = String("Boot");
  fileName = String(fileName + bootNum + ".log");
  Serial.print("Logging keypress to: ");
  Serial.println(fileName);
  File logs = SD.open(fileName, FILE_WRITE);
  logs.println(logging);
  logs.close();
  
}



// ______________________________________________________________________
// FUNCTION KeyControl
// Serves to control the key detection rates. TODO Remove?
int KeyControl()
{
  if (millis() - KeyControlTime >= KeyControlThreshold)
  {
    KeyControlTime = millis();
    KeyEvent = GetKey();
    if (KeyEvent != NoKey)
      KeyControlThreshold = KeyPressRefresh;
    else
      KeyControlThreshold = KeyNormalRefresh;
  }
  else
  {
    KeyEvent = NoKey;
  }
  return KeyEvent;
}

//________________________________________________________________________


void KeyPress(KeypadEvent key) {
  if (keypad.getState() == PRESSED) {
    change = true;
  }
}



//________________________________________________________________________

int GetKeyOld() // TODO Remove?
{
  KeyADC = analogRead(0);
  KeyEvent = CategorizeKey(KeyADC);
  LastKey = KeyEvent;
  return KeyEvent;
}


int GetKey() // TODO Remove?
{
  char key = keypad.getKey();
  switch (key) {
    case '>':
      KeyEvent = RightKey;
      break;
    case '^':
      KeyEvent = UpKey;
      break;
    case '<':
      KeyEvent = LeftKey;
      break;
    case 'v':
      KeyEvent = DownKey;
      break;
    case 'E':
      KeyEvent = SelectKey;
      break;
    default:
      KeyEvent = NoKey;
  }
  LastKey = KeyEvent;

  return KeyEvent;
}


// ______________________________________________________________________
// FUNCTION CategorizeKey TODO Remove?
// This categorizes the input ADC value into a usable key value.
int CategorizeKey(int KeyADC)
{
  if ( KeyADC <= 688)
    KeyEvent = RightKey;
  if (KeyADC >= UpKeyADCLower && KeyADC <= UpKeyADCUpper)
    KeyEvent = UpKey;
  else if (KeyADC >= LeftKeyADCLower && KeyADC <= LeftKeyADCUpper)
    KeyEvent = LeftKey;
  else if (KeyADC >= SelectKeyADCLower && KeyADC <= SelectKeyADCUpper)
    KeyEvent = SelectKey;
  else if (KeyADC >= DownKeyADCLower && KeyADC <= DownKeyADCUpper)
    KeyEvent = DownKey;
  else if  (KeyADC >= 678 && KeyADC <= 688)
    KeyEvent = RightKey;
  else
    KeyEvent = NoKey;

  return KeyEvent;
}


