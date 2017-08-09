//Menu controlled variables
/*
    P I D control scaling constants for PID control. G is total gain constant for control
    T is the infrared sensor Threshold. S is the scaling constant for drive speed
    X is used to select a competition surface (L or R)
*/
int printCount;
int stage;
bool armSafetyFlag;

int mode = 0; // 0 for no debug, 1 for arm, 2 for ir, 3 for cross
String modes[] = {"Regular" , "Debug arm" , "Debug IR", "Debug ring", "Debug PID", "Calib Arm", "Debug Motor", "Debug Zipline"};

void setup() {
#include <phys253setup.txt>
  Serial.begin(9600);
  attachISR(INT1, ISR1);
  attachISR(INT2, ISR2);
  attachISR(INT3, ISR3);
  enableExternalInterrupt(INT1, FALLING);
  enableExternalInterrupt(INT2, FALLING);
  enableExternalInterrupt(INT3, FALLING);
  // set all variables and constants
  /* void initConsts( p,  i,  d,  g,  IR,  flat,  ramp,  ring,
    //                  smallErr,  medErr,  largeErr,  hugeErr,  armSpeed,  fineArmSpeed,  side);
    //initConsts(12.5, 0, 4, 2, 300, 1.0, 1.0, 0.35, 4, 8, 16, 24, 500, 700, 0);*/
  setPIDG(5, 0, 4, 2);
  setIRThresh(350);
  setSpeeds(0.7, 1.0, 0.35);
  setErrors(10, 12, 16, 24);
  setArmSpeeds(10000, 10000);
  setSide(0);

  printCount = 0;
  armSafetyFlag = true;
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
  if (armSafetyFlag) {
    setArmSafe(false);
    armSafetyFlag = false;
  }
  moveArm(gatePos);
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
    setStartTime(millis());
    switch (mode) {
      case 7:
        ziplineDebug();
      case 6:
        motorDebug();
      case 5:
        armCalibrate();
      case 4:
        PIDdebug();
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
  // Phase 1 moves the robot from the start point to the ring junction.
  // Phase 1 setup
  stage = 0;
  armSafetyFlag = true;
  setStageTime(millis());
  setIRTimer(millis() - 5000); // this sets the time left on the gate to 0 by default at the start
  //to prevent accidentally driving through the gate if you never pick up IR
  // end of setup

  while (1) {
    if (startbutton()) {
      setArmSafe(false);
      while (startbutton()) {
        moveArm(gatePos);
      }
      phase1();
    }

    switch (stage) {
      case beforeGateStage:
        protoGate();
        break;

      case afterGateStage:
        afterGate();
        break;

      case onRampStage:
        onRamp();
        break;

      case afterRampStage:
        afterRamp();
        break;

      case ringStage:
        ring();
        break;

    }

    printCount++;
    if (printCount > 300) {
      printCount = 0;
      LCD.clear();
      printQRDs();
      LCD.print(getSpeedScale());
      LCD.print(" ");
      LCD.print(stage);

      LCD.setCursor(0, 1);

      LCD.print(" ");
      LCD.print(stageMilliseconds());
      LCD.print(" ");
      if (getLastTurn()) {
        LCD.print("L");
      } else {
        LCD.print("R");
      }
      LCD.print(" ");
      LCD.print(timeLeft(millis()));
    }

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }


  }
}

void phase2() {
  setCrossPos(-1);
  setTargetPos(0);
  int nextPos;
  setArmSafe(false);
  resetArmServo();
  while (1) {
    // push start to start a new run from the beginning
    if (startbutton()) {
      setArmSafe(false);
      while (startbutton()) {
        moveArm(gatePos);
      }
      phase1();
    }

    nextPos = getTargetPos();
    if (moveToPos(nextPos) && nextPos != -1) {
      PID4step(200);
      revStop();
      while (!atBothPos(nextPos)) {
        moveArm(nextPos);
      }
      activateArmServo();
      delay(100);
      resetArmServo();
      delay(500);
      setTargetPos(findNextToy(getCrossPos(), seconds()));
    } else if (getTargetPos() == -1) {
      if (moveToPos(4)) { //!!!
        zipline();
      }

    }

    printCount++;
    if (printCount > 500) {
      printCount = 0;
      LCD.clear();
      printQRDs();
      LCD.print(nextPos);
      LCD.print(" ");
      LCD.print(getCrossPos());
      LCD.print(" ");
      LCD.print(seconds());
      LCD.print(" ");
      LCD.print(getSpeedScale());
    }

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }
  }
}

void zipline() {
  while (1) {
    while (!moveToPos(zipPos)) {

    }
    revStop();
    moveLowerArm(zipPos);
    moveUpperArm(zipPos);
    stableLift();
    LCD.clear();
    printQRDs();
    setMotors(0, 0, 0);
    if (atBothPos(zipPos)) {
      while (atCross()) {
        setMotors(255, 255, 0);
      }

      while (!atCross()) {
        setMotors(100, 100, 0);
      }
      revStop();

      while (1) {
        LCD.clear();
        printQRDs();
      }
    }
  }
}

void beforeGate() {
  if (gateStop()) {
    setIRTimer(millis());
  }

  // drive through the first gate regardless of IR gate state
  if (stageMilliseconds() < beforeGateMillis) {
    if (stageMilliseconds() < 600) {
      moveArm(gatePos);
      stageSpeed(stage);
    } else {
      if (!atBothPos(irPos)) {
        stageSpeed(slowStage);
      } else {
        stageSpeed(stage);
      }
      moveArm(irPos);
    }

    PID4follow();
    // Stopping at the gate is the first priority. after stopping, move to the next stage
  } else if (gateStop()) {
    // Stop at gate and move to next stage
    while (gateStop()) {
      setIRTimer(millis());
      moveArm(irPos);
      revStop();
    }
    setStageTime(millis());
    stage++;

    // if you have enough time left to make it through the gate you should go for it, but make sure to increment to the next stage and slow down
  } else if (timeLeft(millis()) > 1000) {
    if (stageMilliseconds() < 1300) {
      stageSpeed(stage);
      PID4follow();
      moveArm(irPos);
    } else {
      setStageTime(millis());
      stage++;
    }

    // if you have moved close to the gate, and you dont have enough time left to get through you should slowly approach the gate
  } else if (stageMilliseconds() < beforeGateMillis + 300 && timeLeft(millis()) < 1000) {
    // Slow down if close
    moveArm(irPos);
    stageSpeed(slowStage);
    PID4follow();

    // if you get too close stop and wait for the IRs to go 10khz
    // to ensure that you have a full 5 seconds to get throughs
  } else {
    moveArm(irPos);
    revStop();
  }
}

void afterGate() {
  moveArm(drivePos);
  if (tiltSwitch() || stageMilliseconds() < 1500) {
    stageSpeed(stage);
    PID4follow();
  } else {
    delay(5);
    if (!tiltSwitch()) {
      setStageTime(millis());
      stage++;
    }
  }
}

void onRamp() {
  moveArm(drivePos);
  if (tiltSwitch() && stageMilliseconds() > 2000) {
    delay(50);
    if (tiltSwitch()) {
      setStageTime(millis());
      stage++;
    }
  } else {
    stageSpeed(stage);
    PID4follow();

  }
}

void afterRamp() {
  moveArm(drivePos);
  if (stageMilliseconds() < afterRampMillis) {
    stageSpeed(stage);
    PID4follow();
  } else if (atCross()) {
    setStageTime(millis());
    stage++;
  } else {
    stageSpeed(slowStage);
    PID4follow();
  }
}

void ring() {
  stageSpeed(stage);
  revStop();
  motor.stop_all();
  delay(1000);
  crossTurn();
  delay(1000);
  phase2();
}


//void protoGate() {
//  long pushTime;
//  if (gateStop()) {
//    setIRTimer(millis());
//  }
//
//  if (stageMilliseconds() < 600) {
//    moveArm(gatePos);
//    if (atBothPos(gatePos)) {
//      stageSpeed(stage);
//      PID4follow();
//    } else {
//      //setStageTime(millis());
//    }
//  } else if (gateStop() && stageMilliseconds() > 1100) {
//    while (gateStop()) {
//      setIRTimer(millis());
//      moveArm(irPos);
//      revStop();
//    }
//    setStageTime(millis());
//    stage++;
//  } else if (stageMilliseconds() < beforeGateMillis + 500) {
//    moveArm(irPos);
//    stageSpeed(slowStage);
//    PID4follow();
//  } else {
//    moveArm(irPos);
//    revStop();
//  }
//}


void protoGate() {
  long pushTime;
  if (gateStop()) {
    setIRTimer(millis());
  }

  if (stageMilliseconds() < 600) {
    Serial.print(1);
    moveArm(gatePos);
    if (atBothPos(gatePos)) {
      stageSpeed(stage);
      PID4follow();
    } else {
      //setStageTime(millis());
    }
  } else if (gateStop() && stageMilliseconds() > 1100) {
     Serial.print(2);
    while (gateStop()) {
      setIRTimer(millis());
      moveArm(irPos);
      revStop();
    }
    setStageTime(millis());
    stage++;
  } else if (timeLeft(millis()) > 1500 ) {
     Serial.print(3);
    moveArm(irPos);
    if (atBothPos(irPos) && !gateStop()) {
      pushTime = millis();
      stageSpeed(stage);
      if (millis() - pushTime < 800) {
        PID4follow();
      } else {
        setStageTime(millis());
        stage++;
      }
    } else {
      stageSpeed(slowStage);
      PID4follow();
    }
  } else if (stageMilliseconds() < beforeGateMillis + 500) {
     Serial.print(4);
    moveArm(irPos);
    stageSpeed(slowStage);
    PID4follow();
  } else {
     Serial.print(5);
    moveArm(irPos);
    revStop();
  }
}

