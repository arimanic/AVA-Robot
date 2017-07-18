//Menu controlled variables
/* 
 *  P I D control scaling constants for PID control. G is total gain constant for control
 *  T is the infrared sensor Threshold. S is the scaling constant for drive speed
 *  X is used to select a competition surface (L or R)
 */
String params[] = {"P", "I", "D", "G", "T", "S", "X"};
double vars[numVars] = {0};
//irThresh


////IR vars
//bool alrdyStop;
////int irThresh;
//double IRs[numIR] = {0};



void setup()
{
#include <phys253setup.txt>
initConsts(12,0,3,10,1100,0.95,1);


  LCD.clear();
  Serial.begin(9600) ;

  enableExternalInterrupt(INT1, FALLING);
  enableExternalInterrupt(INT2, FALLING);
  enableExternalInterrupt(INT3, FALLING);
  attachISR(INT1, ISR1);
  attachISR(INT2, ISR2);
  attachISR(INT3, ISR3);
  //attachTimerInterrupt(1, timer1ISR);

  alrdyStop = false;

  wheelRotations = 0;

  printCount = 0;
}

void loop() { // final version. working as intended do not modify
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
    printParams(params , vars);
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
/* Phase 1 moves the robot from the start point to the ring junction. 
 *  It stops the robot at the 10 kHz gate, but only once. 
 *  Edge sensors are used through external interrupts to prevent 
 */
  
  // Phase 1 setup
  
  timeElapsed = 0;
  alrdyStop = false;

  //moveUpperArm(drivePos);
  //moveLowerArm(drivePos);
  // end of setup

LCD.clear();
  while (1) {    
    if (sonarInterrupt) {
      if (offEdgeTurn == "L") {
        setMotors(0 , 255 , 0); // hard left turn
      } else {
        setMotors(255 , 0 , 0); // hard right turn
      }
    } else if (gateStop() && !alrdyStop) {
      while(gateStop()){
      setMotors(0, 0, 0);
      }
      alrdyStop = true;
    } else if (atCross()) {
      setMotors(0, 0, 0);
      phase2();
    } else {
      PID4follow();
    }

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }
  }
}

void phase2(){
  
}


