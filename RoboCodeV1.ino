#include <phys253.h>
#include <LiquidCrystal.h>
#include <math.h>
#include <string.h>




// Things you can change
// Motor Number Outputs
#define RmotorPin 0
#define LmotorPin 1
#define armBaseMotorPin 2
#define armHingeMotorPin 3

// Digital IO pins
  //External Interrupt Pins
#define LSonarPin 0
#define RSonarPin 1
#define interrupt2 2
#define interrupt3 3
  // Standard Digital IO pins
#define QRD0pin 4
#define QRD1pin 5
#define QRD2pin 6
#define QRD3pin 7

#define speedPotPin 8

// Analog IO Pins
#define IR0pin A0
#define IR1pin A1
#define IR2pin A2
#define IR3pin A3
#define IR4pin A4

#define hingeMotorPotPin 45
#define baseMotorPotPin 46

#define paramMax 150

#define numQRD 6
#define numIR 5
#define numVars 6



//Things you shouldnt change
String params[] = {"P", "I", "D", "G", "Th", "Sp"};
bool QRDs[numQRD] = {0};
double IRs[numIR] = {0};
double vars[] = {0, 0, 0, 0, 0, 0};
int kp, ki, kd, controlGain, tapeThresh, printCount;
double speedScale;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  printCount = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  // Push stop to go into the menu
  if (stopbutton()) {
    while (stopbutton()) {
    }
    menu();
  }

  // This section is concerned with printing variables to the screen
  printCount++;
  if (printCount > 40) {
    printParams();
    printCount = 0;
  }

  /////////////////////////////
  ///// Robot AI //////////////
  /////////////////////////////

  if (startbutton()) {
    phase1();
  }
}

void phase1() {
  LCD.clear();
  LCD.print("phase 1");
  while (1){
//  Serial.println(getQRD(D1));
  Serial.println(getQRD(QRD0pin));
  }
}

void menu() {
  int param, printMenu;
  double var;
  setMotors(0, 0, 0);

  LCD.clear();
  LCD.print("Menu");
  delay(300);

  while (1) {

    printMenu++;
    if (printMenu > 40) {
      printMenu = 0;

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
        var = analogRead(6) / 1023.0;
      } else {
        var = map(analogRead(6), 0 , 1010 , 0 , 150);
      }

      LCD.clear();
      LCD.print(params[param]);
      LCD.print(var);
      LCD.setCursor(0, 1);
      if (params[param] == "Th") {
        LCD.print("Rs");
        LCD.print(QRDs[3]);
        LCD.print(" Ls");
        LCD.print(QRDs[2]);
      } else {
        LCD.print(" Last = ");
        LCD.print(vars[param]);
      }
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

void setMotors(int L, int R, int ctrl) {
  // Calculates and sets motor speed and control
  motor.speed(RmotorPin, (R + ctrl)*speedScale);
  motor.speed(LmotorPin, (L - ctrl)*speedScale);
}

void printParams() {
// Print all parameters to screen
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
}

void getQRDs() {
  // Reads all QRD sensors and stores boolean values in QRDs array
  for (int i = 0; i < numQRD; i++) {
    QRDs[i] = digitalRead(i);
  }
}

bool getQRD(int QRDnum) {
  // Reads the given QRD sensor and stores boolean value in QRDs array 
  // Returns true or false for the given QRD
    QRDs[QRDnum] = digitalRead(QRDnum);
    return QRDs[QRDnum];
  }

void getIRs(){
  // Reads all IR sensors and stores values in IRs array
  for (int IRindex = 0; IRindex < numIR; IRindex++){
    IRs[IRindex] = analogRead(40 + IRindex);
  }
}

int getIR(int IRNum){
  // Reads the given IR sensor, stores the value in IRs array and returns the value
  IRs[IRNum] = analogRead(IRNum);
  return IRs[IRNum];
}

int getBaseMotorPot(){
  //Read the analog voltage value of the Potentiometer at the base of the arm
  // 0 - 1023
  return analogRead(baseMotorPotPin);
}

int getHingeMotorPot(){
  //Read the analog voltage value of the Potentiometer at the hinge of the arm
  // 0 -1023
  return analogRead(hingeMotorPotPin);
}





