/*
 * Consists of all choices for:
 *  - Brand
 *  - Size
 *  - Volume
 *  - Flow rate
 */

//_____________________Brand of Syringe_________________________
void tftMgSO4Brand()
{
  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Syringe");
  tft.println(" Brand");

  // Draw the triangles
  softButtons(3);

  // Add labels
  menuOption(2, 1, ""); // See menuOption() for reason
  menuOption(2, 1, "B.D.");
  menuOption(3, 1, "Terumo");
  menuOption(4, 1, "Monoject");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': TheBrand = SyrBrand[0];
          SyrBrandSelect = 0;
          LogKeyPress("2", "B.D.");
          tftMgSO4Size();
          flag = true;
          break;
        case '+': TheBrand = SyrBrand[2];
          SyrBrandSelect = 2;
          LogKeyPress("3", "Terumo");
          tftMgSO4Size();
          flag = true;
          break;

        case '#': TheBrand = SyrBrand[1];
          SyrBrandSelect = 1;
          LogKeyPress("4", "Monoject");
          tftMgSO4Size();
          flag = true;
          break;

        case 'B':
          LogKeyPress("Back", "");
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

//_____________________Size(s) of Syringe_________________________
void tftMgSO4Size() {
  tftMgSO4Size1();
}

void tftMgSO4Size1()
{
  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Syringe");
  tft.println(" Size");

  // Draw the triangles
  softButtons(4);

  // Add labels
  menuOption(1, 1, ""); // See menuOption() for reason
  menuOption(1, 1, "60 mL");
  menuOption(2, 1, "30 mL");
  menuOption(3, 1, "20 mL");
  menuOption(4, 1, "More sizes");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '*': TheSize = 60;
          LogKeyPress("1", "60 mL");
          tftMgSO4Total();
          flag = true;
          break;
        case '-': TheSize = 30;
          LogKeyPress("2", "30 mL");
          tftMgSO4Total();
          flag = true;
          break;

        case '+': TheSize = 20;
          LogKeyPress("3", "20 mL");
          tftMgSO4Total();
          flag = true;
          break;

        case '#': LogKeyPress("4", "More sizes");
          tftMgSO4Size2();
          break;

        case 'B': LogKeyPress("Back", "");
          tftMgSO4Brand();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

void tftMgSO4Size2()
{
  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b);  
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Syringe");
  tft.println(" Size");

  // Draw the triangles
  softButtons(4);

  // Add labels
  menuOption(1, 1, ""); // See menuOption() for reason
  menuOption(1, 1, "10 mL");
  menuOption(2, 1, "5 mL");
  menuOption(3, 1, "3 mL");
  menuOption(4, 1, "More sizes");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '*': TheSize = 10;
          LogKeyPress("1", "10 mL");
          tftMgSO4Total();
          flag = true;
          break;
        case '-': TheSize = 5;
          LogKeyPress("2", "5 mL");
          tftMgSO4Total();
          flag = true;
          break;

        case '+': TheSize = 3;
          LogKeyPress("3", "3 mL");
          tftMgSO4Total();
          flag = true;
          break;

        case '#': LogKeyPress("4", "More sizes");
          tftMgSO4Size3();
          break;
       
        case 'B': LogKeyPress("Back", "");
          tftMgSO4Size();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

void tftMgSO4Size3()
{
  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Syringe");
  tft.println(" Size");

  // Draw the triangles
  softButtons(2);

  // Add labels
  menuOption(2, 1, ""); // See menuOption() for reason
  menuOption(2, 1, "1 mL");
  menuOption(3, 1, "More sizes");

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': TheSize = 1;
          LogKeyPress("2", "1 mL");
          tftMgSO4Total();
          flag = true;
          break;

        case '+': LogKeyPress("3", "More sizes");
          tftMgSO4Size(); // Restarts size options
          break;

        case 'B': LogKeyPress("Back", "");
          tftMgSO4Size2();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

//_____________________Fluid in Syringe_________________________
void tftMgSO4Total()
{

  TotalVolume = TheSize;

  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Amount of Fluid");
  tft.println(" in Syringe");

  // Add the total volume
  tft.setCursor(140, 190);
  tft.setTextSize(3);
  tft.print(TotalVolume);
  tft.setCursor(220, 190);
  tft.print(" mL");

    
  // Draw the triangles
  softButtons(4);

  // Add labels
  menuOption(4, 1, ""); // See menuOption() for reason
  menuOption(1, 1, "+5 mL");
  menuOption(2, 1, "+1 mL");
  menuOption(3, 1, "-1 mL");
  menuOption(4, 1, "-5 mL");
  
 // Instructions
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(1);
  tft.setCursor(80, 240);
  tft.println(" Adjust amount by adding (+)");
  tft.setCursor(80, 270);
  tft.println("    or subtracting (-) mL");
  tft.setCursor(80, 300);
  tft.println(" then press 'OK' to continue"); 

  while (true) {
    char key = keypad.getKey();
    bool flag = false;
    bool volChange = false;


    if (change) {
      change = false;
      switch (key) {
        case '*': TotalVolume += 5;
          volChange = true;
          LogKeyPress("1", "+5 mL");
          break;
          
        case '-': TotalVolume += 1;
          volChange = true;
          LogKeyPress("2", "+1 mL");
          break;
          
        case '+': TotalVolume -= 1;
          volChange = true;
          LogKeyPress("3", "-1 mL");
          break;

        case '#': TotalVolume -= 5;
          volChange = true;
          LogKeyPress("4", "-5 mL");
          break;

        case 'K': 
          LogKeyPress("OK", "");
          if (!MgSO4){ 
            tftManualFlow1();
          }
          tftMgSO4VolCheck();
          flag = true;
          break;

        case 'B':
          LogKeyPress("Back", "");
          flag = true;
          tftMgSO4Size();
          break;
      }
    }

    if (TotalVolume > TheSize) {
      TotalVolume = TheSize;
    } else if (TotalVolume < 0) {
      TotalVolume = 0;
    }

    if (volChange) {
      tft.fillRect(140, 110, 80, 100, HX8357_BLACK);
      tft.setTextColor(HX8357_WHITE);
      tft.setCursor(140, 190);
      tft.setTextSize(3);
      tft.print(TotalVolume);
    }

    if (flag) {
      break;
    }
  }
}

//_____________________Check volume to infuse_________________________
void tftMgSO4VolCheck() {
  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b); 
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Would you like to");
  tft.print(" infuse all ");
  tft.print(TotalVolume);
  tft.print("mL of fluid?");

  // Draw the triangles
  softButtons(2);


  // Make the strings before translating to char*
  String vol = String(TotalVolume);
  String ml = " mL";
  vol.concat(ml);
  String opt1 = "Yes, infuse all ";
  String opt2 = "No, infuse less than ";
  opt1.concat(vol);
  opt2.concat(vol);
  char option1[opt1.length() + 1];
  char option2[opt2.length() + 1];
  // Translate opt1 and opt2 to char*
  opt1.toCharArray(option1, opt1.length() + 1);
  opt2.toCharArray(option2, opt2.length() + 1);

  // Add labels
  menuOption(2, 1, ""); // See menuOption() for reason
  menuOption(2, 1, option1);
  menuOption(3, 1, option2);

  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case '-': PrescribedVol = TotalVolume;
          LogKeyPress("2", opt1);
          tftMgSO4Verify();
          flag = true;
          break;

        case '+': LogKeyPress("3", opt2);
          tftMgSO4Prescribed(); // Change prescribed infusion volume
          flag = true;
          break;

        case 'K': PrescribedVol = TotalVolume;
          LogKeyPress("2", opt1);
          tftMgSO4Verify();
          flag = true;
          break;

        case 'B': LogKeyPress("Back", "");
          if (!MgSO4){ 
            tftManualFlow1();
          }
          else {
          tftMgSO4VolCheck();
          tftMgSO4Total();
          }
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}

//_____________________Prescribed Volume to Infuse_________________________
// TODO: Doesn't actually do anything, since I cannot find the variable this should affect
void tftMgSO4Prescribed()
{
  PrescribedVol = TotalVolume;

  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Amount of");
  tft.println(" Fluid to Infuse");

  // Draw the triangles
  softButtons(4);

  // Add labels
  menuOption(4, 1, ""); // See menuOption() for reason
  menuOption(1, 1, "+5 mL");
  menuOption(2, 1, "+1 mL");
  menuOption(3, 1, "-1 mL");
  menuOption(4, 1, "-5 mL");


  // Add the total volume
  tft.setCursor(140, 190);
  tft.setTextSize(3);
  tft.print(PrescribedVol);
  tft.setCursor(220, 190);
  tft.print(" mL");

 // Instructions
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(1);
  tft.setCursor(80, 240);
  tft.println(" Adjust amount by adding (+)");
  tft.setCursor(80, 270);
  tft.println("    or subtracting (-) mL");
  tft.setCursor(80, 300);
  tft.println(" then press 'OK' to continue"); 


  while (true) {
    char key = keypad.getKey();
    bool flag = false;
    bool volChange = false;


    if (change) {
      change = false;
      switch (key) {
        case '*': PrescribedVol += 5;
          volChange = true;
          LogKeyPress("1", "+5 mL");
          break;
        case '-': PrescribedVol += 1;
          volChange = true;
          LogKeyPress("2", "+1 mL");
          break;

        case '+': PrescribedVol -= 1;
          volChange = true;
          LogKeyPress("3", "-1 mL");
          break;

        case '#': PrescribedVol -= 5;
          volChange = true;
          LogKeyPress("4", "-5 mL");
          break;
        
        case 'K': 
          LogKeyPress("OK", "");
          TotalVolume==PrescribedVol; //CHANGED
          tftMgSO4Verify();
          flag = true;
          break;

        case 'B':
          LogKeyPress("Back", "");
          flag = true;
          tftMgSO4VolCheck();
          break;
        

      }
    }

    if (PrescribedVol > TotalVolume) {
      PrescribedVol = TotalVolume;
    } else if (PrescribedVol < 0) {
      PrescribedVol = 0;
    }

    // Repaint the number
    if (volChange) {
      tft.fillRect(140, 110, 80, 100, HX8357_BLACK);
      tft.setCursor(140, 190);
      tft.setTextColor(HX8357_WHITE);
      tft.setTextSize(3);
      tft.print(PrescribedVol);
    }

    if (flag) {
      break;
    }
  }
}

//_____________________(Manual) Flow Rate_________________________
void tftManualFlow1()
{
  // New menu construction with soft buttons
  tft.setFont(&FreeSans12pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.println(" Select Flow");
  tft.println(" Rate");

  // Draw the triangles
  softButtons(4);

  // Add labels
  menuOption(4, 1, ""); // See menuOption() for reason
  menuOption(1, 1, "+5 mL/hr");
  menuOption(2, 1, "+1 mL/hr");
  menuOption(3, 1, "-1 mL/hr");
  menuOption(4, 1, "-5 mL/hr");

  // Add the total volume
  tft.setCursor(60, 190);
  tft.setTextSize(3);
  tft.print(TheRate);
  tft.setCursor(140, 190);
  tft.print(" mL/hr");

 // Instructions
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(1);
  tft.setCursor(60, 240);
  tft.println(" Adjust rate by adding (+)");
  tft.setCursor(60, 270);
  tft.println("  or subtracting (-) mL/hr");
  tft.setCursor(60, 300);
  tft.println("then press 'OK' to continue"); 

 while (true) {
    char key = keypad.getKey();
    bool flag = false;
    bool rateChange = false;


    if (change) {
      change = false;
      switch (key) {
        case '*': TheRate += 5;
          rateChange = true; 
          LogKeyPress("1", "+5 mL");
          break;
        case '-': TheRate += 1;
          rateChange = true;
          LogKeyPress("2", "+1 mL");
          break;

        case '+': TheRate -= 1;
          rateChange = true;
          LogKeyPress("3", "-1 mL");
          break;

        case '#': TheRate -= 5;
          rateChange = true;
          LogKeyPress("4", "-5 mL");
          break;
        
        case 'K': 
          LogKeyPress("OK", "");
          tftMgSO4VolCheck();
          flag = true;
          break;

        case 'B':
          LogKeyPress("Back", "");
          flag = true;
          tftMgSO4Total();
          break;

      }
    }

      if (TheRate > 120) {
      TheRate = 120;
    } else if (TheRate < 0) {
      TheRate = 0;
    }

    if (rateChange) {
      tft.fillRect(30, 110, 120, 100, HX8357_BLACK);
      tft.setTextColor(HX8357_WHITE);
      if (TheRate > 99) {
        tft.setCursor(30,190);
      }
        else {
        tft.setCursor(60, 190);
      }
      tft.setTextSize(3);
      tft.print(TheRate);
    }

    if (flag) {
      break;
    }
  }
}

//___________________Final Info Check_________________________________
void tftMgSO4Verify()
{
  tft.setFont(&FreeSans12pt7b);
  tft.setTextWrap(false);
  tft.fillScreen(HX8357_WHITE);
  tft.drawRect(0, 0, 480, 65, HX8357_BLACK);
  tft.fillRect(0, 0, 480, 64, HX8357_BLUE);
  tft.setTextSize(2);
  tft.setCursor(115, 45);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Verify Settings");

  tft.setTextSize(1);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(HX8357_BLACK);
  tft.setCursor(165, 95);
  tft.print("Mode:");
  tft.setCursor(137, 125);
  tft.print("Delivery:");
  tft.setCursor(21, 155);
  tft.print("Syringe Type/Size:");
  tft.setCursor(49, 185);
  tft.print("Fluid in Syringe:");
  tft.setCursor(30, 215);
  tft.print("Amount to Infuse:");
  tft.setFont(&FreeSans12pt7b);

  if (MgSO4){
    tft.setCursor(240, 95);
    tft.print("20% MgSO4");
    tft.setCursor(240, 125);
    if (Maintenance) {
      tft.println("1g/hr Maintenance");
    } else if (Loading) {
      tft.println("4g Loading Dose");
    }
  } else {
    tft.setCursor(240, 95);
    tft.print("Manual");
    tft.setCursor(240, 125);
    tft.print(TheRate);
    tft.print(" mL/hr");
  }
  
  // Syringe Type
  tft.setCursor(240, 155);
  tft.print(TheBrand);
  tft.print(" ");
  tft.print(TheSize);
  tft.print(" mL");

  // Volume in Syringe
  tft.setCursor(240, 185);
  tft.print(TotalVolume);
  tft.print(" mL");

  // Volume to infuse
  tft.setCursor(240, 215);
  tft.print(PrescribedVol);
  tft.print(" mL");

  


  // Button instructions
  tft.setTextColor(HX8357_WHITE);
  tft.fillRect(0, 240, 238, 80, HX8357_BLACK);
  tft.setCursor(15, 275);
  tft.println("To change settings");
  tft.println(" press BACK/PAUSE");
  tft.fillRect(242, 240, 238, 80, HX8357_BLACK);
  tft.setCursor(280, 275);
  tft.println("To start infusion");
  tft.setCursor(310, 305);
  tft.print("press OK");
  
  while (true) {
    char key = keypad.getKey();
    bool flag = false;

    if (change) {
      change = false;
      switch (key) {
        case 'K': LogKeyPress("OK", "Start infusion");
          tftStartInfusion();
          break;

        case 'B': LogKeyPress("Back", "Change settings");
          tftMgSO4Prescribed();
          flag = true;
          break;
      }
    }

    if (flag) {
      break;
    }
  }
}
