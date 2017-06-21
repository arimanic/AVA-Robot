#include <phys253.h>
#include <LiquidCrystal.h>
#include <math.h>
#include <string.h>




// Things you can change
#define Rmotor 1
#define Lmotor 3
#define paramMax 150;
//Things you cant change

int numVars = 6;
String params[] = {"P", "I", "D", "G", "Th", "Sp"};
double vars[] = {0, 0, 0, 0, 0, 0};
bool QRDs[] = {0, 0, 0, 0, 0, 0}; // 1 if on the tape
int kp, ki, kd, controlGain, tapeThresh, printCount, screenToggle;
double speedScale;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  printCount = 0;
  screenToggle = 1;
}

void loop() {
  // put your main code here, to run repeatedly:

  // Push start to go into the menu
  if (startbutton()) {
    while (startbutton()) {
    }
    menu();
  }

  // This section is concerned with printing variables to the screen
  printCount++;
  if (printCount > 40) {
    printParams();
    printCount = 0;
  }

    /////////////////////////////////////////////////////////
   /////////////////Start Robot AI Here ////////////////////
  /////////////////////////////////////////////////////////

  
}

void menu() {
  int param, printMenu;
  double var;
  setMotors(0, 0);

  LCD.clear();
  LCD.print("Menu");
  delay(300);

  while (1) {

      param = map(analogRead(7), 0, 1000, 0, numVars - 1);
      if (param > numVars) {
        param = numVars;
      }

      if (param < 0) {
        param = 0;
      }

      if (params[param] == "Th") {
        var = analogRead(6);
      } else if (params[param] == "Sp") {
        var = analogRead(6)/1023.0;
      } else {
        var = map(analogRead(6), 0 , 1010 , 0 , 150);
      }

      LCD.clear();
      LCD.print(params[param]);
      LCD.print(var);
      LCD.setCursor(0, 1);
      LCD.print(" Last = ");
      LCD.print(vars[param]);
    }

    // Exit the menu and save parameters
    if (stopbutton()) {
      while (stopbutton()) {
      }
      kp = (int) vars[0];
      ki = (int) vars[1];
      kd = (int) vars[2];
      controlGain = (int) vars[3];
      speedScale =  vars[5];
      tapeThresh = (int) vars[4];
      LCD.clear();
      return;

    }

    // Save the current variable
    if (startbutton()) {
      while (startbutton()) {
      }
      vars[param] = var;
    }
  }
}

void setMotors(int L, int R) {
  motor.speed(Rmotor, R);
  motor.speed(Lmotor, L);
}

void printParams() {
  if (stopbutton()) {
    while (stopbutton()) {

    }
    screenToggle = !screenToggle;
  }

  if (screenToggle) {
    LCD.clear();
    LCD.print("S");
    LCD.print(speedScale);
    LCD.print(" T");
    LCD.print(tapeThresh);
    LCD.print(" G");
    LCD.print(controlGain);

    LCD.setCursor(0, 1);
    LCD.print("P");
    LCD.print(kp);
    LCD.print(" I");
    LCD.print(ki);
    LCD.print(" D");
    LCD.print(kd);

  } else {
    LCD.clear();
    LCD.print("Rs");
    LCD.print(sensors[3]);
    LCD.print(" Ls");
    LCD.print(sensors[2]);
    LCD.setCursor(0, 1);
    LCD.print("T");
    LCD.print(tapeThresh);
  }
}

//void readQRDs(){
//  for (int i = 0, i < sensors, i++){
//    QRDs[i] = analogRead(Ai) > tapeThresh;
//  }

