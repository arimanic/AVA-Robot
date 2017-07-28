//Menu controlled variables
/*
    P I D control scaling constants for PID control. G is total gain constant for control
    T is the infrared sensor Threshold. S is the scaling constant for drive speed
    X is used to select a competition surface (L or R)
*/
int printCount;
int stage;

int mode = 0; // 0 for no debug, 1 for arm, 2 for ir, 3 for cross
String modes[] = {"Regular" , "Debug arm" , "Debug IR", "Debug ring", "Debug zipline*"};

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  attachISR(INT1, ISR1);
  attachISR(INT2, ISR2);
  attachISR(INT3, ISR3);
  enableExternalInterrupt(INT1, FALLING);
  enableExternalInterrupt(INT2, FALLING);
  enableExternalInterrupt(INT3, LOW);
  // set all variables and constants
  initConsts(12.5, 0, 4, 2, 1100, 0.40, 0.60, 0.20, 1, 0);
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
    delay(300);
    while (startbutton()) {
      mode = gatedKnobMap(6 , 0 , numModes - 1);

      printCount++;
      if (printCount > 500) {
        printCount = 0;
        LCD.clear();
        LCD.print(modes[mode]);
      }
    }
    switch (mode) {
      case 4:
        break;
      case 3:
        ringDebug();
        break;
      case 2:
        irDebug();
        break;
      case 1:
        armDebug();
        break;
      case 0:
        phase1();
        break;
    }
  }
}

void phase1() {

  /* Phase 1 moves the robot from the start point to the ring junction.
      It stops the robot at the 10 kHz gate, but only once.
      Edge sensors are used through external interrupts to prevent
  */

  // Phase 1 setup
  setStartTime(millis());
  alrdyStop = false;
  wheelTicks = 0;
  stage = 0;
  //moveUpperArm(drivePos);
  //moveLowerArm(drivePos);
  // end of setup

  LCD.clear();
  while (1) {
    /* if (sonarInterrupt) {
       if (offEdgeTurn == "L") {
         setMotors(0 , 255 , 0); // hard left turn
       } else {
         setMotors(255 , 0 , 0); // hard right turn*/

    switch (stage) {
      case beforeGateStage:
        /* go fast until you get close to the gate. Slow down when close.
          Stop while IR is on and then move to next stage when it turns off */

        if (wheelTicks < beforeGateTicks) {
          // fast on flats leading up to gate
          stageSpeed(stage);
          PID4follow();
        } else if (digitalRead(1)) { //gateStop()
          // Stop at gate and move to next stage
          while (digitalRead(1)) {  // gateStop()
            revStop();
          }
          wheelTicks = 0;
          stage++;
        } else if (wheelTicks < beforeGateTicks + 40) {
          // Slow down if close
          stageSpeed(slowStage);
          PID4follow();
        } else {
          // if you get too close stop and wait for the IRs to go 10khz
          // to ensure that you have a full 5 seconds to get throughs
          revStop();
        }
        break;

      case afterGateStage:
        if (wheelTicks < afterGateTicks) {
          stageSpeed(stage);
          PID4follow();
        } else {
          wheelTicks = 0;
          stage++;
        }

        break;

      case onRampStage:
        if (wheelTicks < onRampTicks) {
          stageSpeed(stage);
          PID4follow();
        } else {
          wheelTicks = 0;
          stage++;
        }
        break;

      case afterRampStage:
        if (wheelTicks < afterRampTicks) {
          stageSpeed(stage);
          PID4follow();
        } else if (atCross()) {
          wheelTicks = 0;
          stage++;
        } else {
          stageSpeed(slowStage);
          PID4follow();
        }
        break;

      case ringStage:
        phase2();
        break;

    }

    printCount++;
    if (printCount > 400) {
      printCount = 0;
      LCD.clear();
      printQRDs();
      LCD.print(getSpeedScale());
      LCD.print(" ");
      LCD.print(stage);

      LCD.setCursor(0, 1);

      LCD.print(" ");
      LCD.print(wheelTicks);
      LCD.print(" ");
      if (getLastTurn()) {
        LCD.print("L");
      } else {
        LCD.print("R");
      }
    }

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }


  }
}

void phase2() {
  stageSpeed(stage);
  crossTurn();
  setCrossPos(-1);
  setTargetPos(0);
  stageSpeed(ringStage);
  setStartTime((double)millis() - 55000);
  while (1) {
    if (moveToPos(getTargetPos()) && getTargetPos() != -1) {
      delay(500);
      setTargetPos(findNextToy(getCrossPos(), seconds()));
    }
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

void sonarTest() {
  while (1) {
    outPulse();
    LCD.clear();
    LCD.print(" ");
    LCD.print(getOffTime());
    LCD.setCursor(0, 1);
  }
}

void armDebug() {
  int pos;
  int servo;
  while (1) {
    printCount++;
    pos = map(analogRead(6), 0, 1023, 0, 4);
    servo = map(analogRead(7), 0, 1023, -2, 182);
    if (printCount > 400) {
      printCount = 0;

      LCD.clear();
      LCD.print(pos);
      LCD.print(" ");
      LCD.print(servo);
      LCD.print(" ");
      LCD.print(analogRead(armBasePotPin) * 5.0 / 1024);
      LCD.print(" ");
      LCD.print(analogRead(armHingePotPin) * 5.0 / 1024);

      LCD.setCursor(0, 1);
      LCD.print(atUpperPos(pos));
      LCD.print(" ");
      LCD.print(atLowerPos(pos));

    }
    moveLowerArm(pos);
    moveUpperArm(pos);
    moveBaseServo(servo);

  }
}

void irDebug() {
  while (1) {
    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }

    moveBaseServo(gatedKnobMap(7, 0, 180));

    printCount++;
    if (printCount > 4000) {
      printCount = 0;
      LCD.clear();
      LCD.print(analogRead(IR0pin));
      LCD.print(" ");
      LCD.print(analogRead(IR1pin));
      LCD.print(" ");
      LCD.print(analogRead(IR2pin));
      LCD.print(" ");

      LCD.setCursor(0, 1);
      LCD.print(gateStop());
    }

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }
  }
}

void ringDebug() {
  extern bool toyFallen[];
  setCrossPos(-1);
  setTargetPos(0);
  stageSpeed(ringStage);
  setStartTime((double)millis() - 55000);
  while (1) {

    toysInWater(seconds());
    if (moveToPos(getTargetPos()) && getTargetPos() != -1) {
      // do the arm thing and then set the target to the next toy
      setTargetPos(findNextToy(getCrossPos(), seconds()));
    }


    printCount++;
    if (printCount > 500) {
      printCount = 0;
      LCD.clear();
      printQRDs();
      LCD.print(getTargetPos());
      LCD.print(" ");
      LCD.print(getCrossPos());
      LCD.print(" ");
      LCD.print(seconds());
      LCD.print(" ");

      LCD.setCursor(0, 1);
      for (int i = 0; i < numToys; i++) {
        LCD.print(toyFallen[i]);
      }
    }

    if (startbutton()) {  // push start to reset the timer
      while (startbutton()) {
      }
      setStartTime((double)millis() - 55000);
      setCrossPos(-1);
      setTargetPos(0);
      toysInWater(seconds());
    }
  }
}

