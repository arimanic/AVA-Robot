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

// Sonar interrupt variables
bool sonarInterrupt;
String offEdgeTurn;

void setup()
{
#include <phys253setup.txt>
  LCD.clear();
  Serial.begin(9600) ;
  enableExternalInterrupt(INT1, CHANGE);
  enableExternalInterrupt(INT2, CHANGE);
  enableExternalInterrupt(INT3, RISING);
  attachTimerInterrupt(60000, timer1ISR);
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
    phase1();
  }
}

void phase1() { //!!! being used for testing functions right now
  // Phase 1 setup
  attachISR(INT1, ISR1);
  attachISR(INT2, ISR2);
  attachISR(INT3, ISR3);
  timeElapsed = 0;

  // end of setup


  while (1) {
    if (sonarInterrupt) {
      if (offEdgeTurn == "L") {
        setMotors(0 , 255 , 0); // hard left turn
      } else  {
        setMotors(255 , 0 , 0); // hard right turn
      }
    } else {

      motor.speed(0, 255);
  motor.speed(1, 255);
  motor.speed(2, 255);
  motor.speed(3, 255);

  delay(1000);
  motor.speed(0, 0);
  motor.speed(1, 0);
  motor.speed(2, 0);
  motor.speed(3, 0);
  
 //     PID4follow();
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
}

void timer1ISR() {
  timeElapsed++;
}


void ISR1() { //!!! make this work for Lsonar
  sonarInterrupt = !sonarInterrupt;
  offEdgeTurn = "R";
}

void ISR2() { //!!! make this work for Rsonar
  sonarInterrupt = !sonarInterrupt;
  offEdgeTurn = "R";
}

void ISR3() { //!!! make this work for wheel measurement
  LCD.clear();
  LCD.print("Vitor sucks");
  LCD.setCursor(0, 1);
  LCD.print("at foosball");
}

