//Menu controlled variables
/*
    P I D control scaling constants for PID control. G is total gain constant for control
    T is the infrared sensor Threshold. S is the scaling constant for drive speed
    X is used to select a competition surface (L or R)
*/

//irThresh


////IR vars
//bool alrdyStop;
////int irThresh;
//double IRs[numIR] = {0};



void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  enableExternalInterrupt(INT1, FALLING);
  enableExternalInterrupt(INT2, FALLING);
  enableExternalInterrupt(INT3, FALLING);
  attachISR(INT1, ISR1);
  attachISR(INT2, ISR2);
  attachISR(INT3, ISR3);
  attachTimer0Interrupt(timer0ISR);

// set all variables and constants
initConsts(11, 0, 8, 2, 1100, 0.00, 1, 0);
alrdyStop = false;
  wheelTicks = 0;
  printCount = 0;
  moveBaseServo(76);
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
  if (printCount > 400) {
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
  /* Phase 1 moves the robot from the start point to the ring junction.
      It stops the robot at the 10 kHz gate, but only once.
      Edge sensors are used through external interrupts to prevent
  */

  // Phase 1 setup

  timeElapsed = 0;
  alrdyStop = false;

  //moveUpperArm(drivePos);
  //moveLowerArm(drivePos);
  // end of setup

  LCD.clear();
  while (1) {
       /* if (sonarInterrupt) {
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
        } else { */

   
   PID4follow();

   printCount++;
   if (printCount > 400) {
    printCount = 0;
    LCD.clear();
    LCD.print(getQRD(3));
    LCD.print(getQRD(2));
    LCD.print(getQRD(1));
    LCD.print(getQRD(0));
    LCD.print(" ");
    LCD.print(getP4());
    LCD.print(" ");
    LCD.print(getD());

    LCD.setCursor(0,1);
    LCD.print(getDist(wheelTicks));
    LCD.print(" ");
    LCD.print(getP4()+getD());
   }
//    if (atCross()) {
//      setMotors(0, 0, 0);
//          phase2();
//    }
//   }


    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }
  }
}

void phase2() {
  while (1) {
    //PID2follow();

    LCD.clear();
    LCD.print(getQRD(3));
    LCD.print(getQRD(2));
    LCD.print(getQRD(1));
    LCD.print(getQRD(0));
    LCD.print("Cross");

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }

    if (startbutton()) {
      while (startbutton()) {
      }
      phase1();
    }
  }
}

void sonarTest(){
  while(1){
    outPulse();
    LCD.clear();
    LCD.print(duration);
    LCD.print(" ");
    LCD.print(getOffTime());
    LCD.setCursor(0,1);
    LCD.print(overflowCount);
  }
}

void armDebug(){
  int pos;
  int servo;
  while(1){
    pos = map(analogRead(6),0,1023,0,4);
    servo = map(analogRead(7),0,1023,-2,182);

    moveLowerArm(pos);
    moveUpperArm(pos);
  }
}

