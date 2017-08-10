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
  setIRThresh(450);
  setSpeeds(0.7, 1.0, 0.35);
  setErrors(10, 12, 16, 24);
  setSide(0);
  setArmSpeeds(1000, 2500);
  printCount = 0;
  resetArmServo();
  armSafetyFlag = true;
}

void loop() { // final version. working as intended do not modify
  // put your main code here, to run repeatedly:
  // Push stop to go into the menu
  resetArmServo();
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
        hardGateStop();
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
      //LCD.print(gateStop());
    }

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menu();
    }


  }
}

void phase2() {
  if (leftSide()) {

    stageSpeed(ringStage);
    while (!atBothPos(zipPos)) {
      moveArm(zipPos);
    }
    motor.stop_all();
    moveBaseServo(0);
    setCrossPos(-2);
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
      activateArmServo();
      nextPos = getTargetPos();
      if (moveToPos(nextPos) && nextPos != -1) {
        PID4step(40);
        revStop();
        while (!atBothPos(nextPos)) {
          moveArm(nextPos);
        }
        moveArm(nextPos);
        resetArmServo();
        delay(500);
        setTargetPos(findNextToy(getCrossPos(), seconds()));
      } else if (getTargetPos() == -1) {
        if (!atBothPos(zipPos)) {
          moveUpperArm(zipPos);
          moveLowerArm(zipPos);
          stableLift();
        } else {
          if (moveToPos(4) && atBothPos(zipPos)) { //!!!
            zipline();
          }
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
  else {
    stageSpeed(ringStage);
    while (!atBothPos(zipPos)) {
      moveArm(zipPos);
    }
    motor.stop_all();
    moveBaseServo(0);
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
      activateArmServo();
      nextPos = getTargetPos();
      if (moveToPos(nextPos) && nextPos != -1) {
        PID4step(40);
        revStop();
        while (!atBothPos(nextPos)) {
          moveArm(nextPos);
        }
        moveArm(nextPos);
        resetArmServo();
        delay(500);
        setTargetPos(findNextToy(getCrossPos(), seconds()));
      } else if (getTargetPos() == -1) {
        if (!atBothPos(zipPos)) {
          moveUpperArm(zipPos);
          moveLowerArm(zipPos);
          stableLift();
        } else {
          if (moveToPos(4) && atBothPos(zipPos)) { //!!!
            zipline();
          }
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
}

void zipline() {
  bool zipAttach = false;

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
      while (!zipAttach) {
        if (leftSide()) {
          turnAround(300);
        }
        while (atCross()) {
          setMotors(255, 255, 0);
        }

        while (!atCross()) {
          setMotors(100, 100, 0);
        }
        revStop();
        LCD.clear();
        printQRDs();
      }
    }
  }
}


void afterGate() {
  if(stageMilliseconds() > 300){
    moveArm(drivePos);
  } else {
    moveUpperArm(drivePos);
    moveLowerArm(drivePos);
  }
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
  if (tiltSwitch() && stageMilliseconds() > 1200) {
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

  if (stageMilliseconds() < afterRampMillis) {
    moveArm(drivePos);
    stageSpeed(stage);
    PID4follow();
  } else if (anyCross()) {
    moveArm(zipPos);
    setStageTime(millis());
    stage++;
  } else {
    moveArm(zipPos);
    setSpeedScale(0.25);
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
    moveArm(gatePos);
    if (atBothPos(gatePos)) {
      stageSpeed(stage);
      PID4follow();
    } else {
      //setStageTime(millis());
    }
  } else if (gateStop() && stageMilliseconds() > 900) {
    revStop();
    while (gateStop()) {
      setIRTimer(millis());
      moveArm(irPos);
    }
    motor.stop_all();
    // LCD.print("wait");
    //  delay(10000);
    setStageTime(millis());
    stage++;
  } else if (timeLeft(millis()) > 2000 ) {
    moveArm(irPos);
    if (atBothPos(irPos) && !gateStop()) {
      pushTime = millis();
      stageSpeed(stage);
      if (millis() - pushTime < 800) {
        PID4follow();
      } else {
        //  LCD.print("run");
        //   motor.stop_all();
        //  delay(10000);
        setStageTime(millis());
        stage++;
      }
    } else {
      stageSpeed(slowStage);
      PID4follow();
    }
  } else if (stageMilliseconds() < beforeGateMillis + 500) {
    moveArm(irPos);
    stageSpeed(slowStage);
    PID4follow();
  } else {
    moveArm(irPos);
    revStop();
  }
}

void hardGateStop() {
  if (stageMilliseconds() < 800) {
    moveArm(gatePos);
  } else {
    moveArm(irPos);
  }

  if (stageMilliseconds() < 1400) {
    if (stageMilliseconds() < 1300) {
      stageSpeed(stage);
    } else {
      stageSpeed(slowStage);
    }
    PID4follow();
  } else {
    revStop();
    if (gateStop() && atBothPos(irPos)) {
      while (gateStop()) {
        moveArm(irPos);
      }
      setStageTime(millis());
      stage++;
    }
  }

}

