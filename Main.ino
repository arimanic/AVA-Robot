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

// TIMING VARIABLE
int timeElapsed = 0;
void setup()
{
#include <phys253setup.txt>
LCD.clear();
  Serial.begin(9600) ;
  enableExternalInterrupt(INT1, CHANGE);
  enableExternalInterrupt(INT2, CHANGE);
  enableExternalInterrupt(INT3, RISING);
  attachTimerInterrupt(1, timer1ISR);
  printCount = 0;
}

void loop() { // !!! final version. working as intended do not modify
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
    attachISR(INT1, ISR1);
    attachISR(INT2, ISR2);
    attachISR(INT3, ISR3);
    timeElapsed = 0;
    phase1();
  }
}

void phase1() { //!!! being used for testing functions right now

  while (1){    
  PID4follow();
  moveUpperArm(drivePos);
  LCD.clear();
  LCD.print(timeElapsed);
  delay(500);

if (stopbutton()) {
    while (stopbutton()) {
    }
    menu();
  }

  }
}

void timer1ISR(){
  timeElapsed++;
}


void ISR1(){//!!! make this work for sonar1
  LCD.clear();
  LCD.print("Vitor sucks");
  LCD.setCursor(0,1);
  LCD.print("at foosball");
}

void ISR2(){//!!! make this work for sonar2
  LCD.clear();
  LCD.print("Vitor sucks");
  LCD.setCursor(0,1);
  LCD.print("at foosball");
}

void ISR3(){//!!! make this work for wheel measurement
  LCD.clear();
  LCD.print("Vitor sucks");
  LCD.setCursor(0,1);
  LCD.print("at foosball");
}

