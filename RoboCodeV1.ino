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

#define hingeMotorPotPin A5
#define baseMotorPotPin A6

#define paramMax 150

#define numQRD 4
#define numIR 5
#define numVars 6



//Things you shouldnt change
String params[] = {"P", "I", "D", "G", "Th", "Sp"};
bool QRDs[numQRD] = {0}; // High means on tape
double IRs[numIR] = {0};
double vars[] = {0, 0, 0, 0, 0, 0};
int kp, ki, kd, controlGain, tapeThresh, printCount;
double speedScale;

// PID vars
// P
int pCon = 0;
int innerSpacing = 4;
int midSpacing = 8;
int outerSpacing = 12;

int lastError, error;
String lastTurn;
bool straight[numQRD] = {0, 1, 1, 0};
bool sLeft[numQRD] =    {1, 1, 0, 0};
bool sRight[numQRD] =   {0, 0, 1, 1};
bool mLeft[numQRD] =    {1, 0, 0, 0};
bool mRight[numQRD] =   {0, 0, 0, 1};
bool hardTurn[numQRD] = {0, 0, 0, 0};

// I
int iCon = 0;

//D
int dCon = 0;

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

void PIDfollow(){
  //!!!
  
  getQRDs();
  getP4();
  
  
}

int getP4(){
  // To make the car turn right P is positive.
  // If P is negative the car turns left
  if (arrayEquals(QRDs, straight, numQRD)){
    pCon = 0;
  } else if (arrayEquals(QRDs, sLeft, numQRD)){
    pCon = -innerSpacing;
    lastTurn = "L";
  } else if (arrayEquals(QRDs, sRight, numQRD)){
    pCon = innerSpacing;
    lastTurn = "R";
  } else if (arrayEquals(QRDs, mLeft, numQRD)){
    pCon = -midSpacing;
     lastTurn = "L";
  } else if (arrayEquals(QRDs, mRight, numQRD)){
    pCon = midSpacing;
     lastTurn = "R";
  } else if (arrayEquals(QRDs, hardTurn, numQRD)){
    if (lastTurn == "L"){
      pCon = -outerSpacing;
    } else if (lastTurn == "R"){
      pCon = outerSpacing;
    }
  }
}

int getP2(){
  // To make the car turn right P is positive.
  // If P is negative the car turns left
  arrSubset(QRDs,1,2);
  if (arrayEquals(QRDs, straight, numQRD)){
    pCon = 0;
  } else if (arrayEquals(QRDs, sLeft, numQRD)){
    pCon = -innerSpacing;
    lastTurn = "L";
  } else if (arrayEquals(QRDs, sRight, numQRD)){
    pCon = innerSpacing;
    lastTurn = "R";
  } else if (arrayEquals(QRDs, mLeft, numQRD)){
    pCon = -midSpacing;
     lastTurn = "L";
  } else if (arrayEquals(QRDs, mRight, numQRD)){
    pCon = midSpacing;
     lastTurn = "R";
  } else if (arrayEquals(QRDs, hardTurn, numQRD)){
    if (lastTurn == "L"){
      pCon = -outerSpacing;
    } else if (lastTurn == "R"){
      pCon = outerSpacing;
    }
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

double getWheelFreq(){
  //!!!
}

void servoPos(int pos){
  
  if (pos > 180 || pos < 0){
    LCD.clear();
    LCD.println("Invalid servo");
    LCD.print(pos);
    delay(300);
    return;
  } 

RCServo0.write(pos);
  return;
}

bool arrayEquals(bool a1[], bool a2[], int len){
  for (int i = 0; i < len; i++){
    if (a1[i] != a2[i]){
      return false;
    }
  }
  return true;
}

