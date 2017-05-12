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






void KeyPress(KeypadEvent key) {
  if (keypad.getState() == PRESSED) {
    change = true;
  }
}




