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
  LCD.clear();
  bool arr1[] = {1 , 0 , 1 , 1 , 1};
  bool arr2[] = {1 , 0 , 1 , 1 , 0};
  arrSubset(mLeft,1,2,arr1);
  arrSubset(straight,1,2, arr2);
 bool testBool = arrayEquals(arr1,arr2,2);
  LCD.print(testBool);
  }
}


