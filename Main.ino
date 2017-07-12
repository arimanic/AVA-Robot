//Things you shouldnt change
String params[] = {"P", "I", "D", "G", "Th", "Sp"};
bool QRDs[numQRD] = {0}; // High means on tape
double vars[] = {0, 0, 0, 0, 0, 0};
int kp, ki, kd, controlGain, printCount;
double speedScale;
int irThresh;
double IRs[numIR] = {0};

// PID vars
// P
int pCon = 0;
// I
int iCon = 0;
// D
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
  PID4follow();
  moveUpperArm(drivePos);
  LCD.clear();



if (stopbutton()) {
    while (stopbutton()) {
    }
    menu();
  }
  
  LCD.print(pCon);
  Serial.println(dCon);
  delay(300);
  }
}
