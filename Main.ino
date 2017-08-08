//Menu controlled variables
/*
    P I D control scaling constants for PID control. G is total gain constant for control
    T is the infrared sensor Threshold. S is the scaling constant for drive speed
    X is used to select a competition surface (L or R)
*/
int printCount;
int stage;

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
  setIRThresh(420);
  setSpeeds(1.0, 1.0, 0.35);
  setErrors(10, 12, 16, 24);
  setArmSpeeds(580, 840);
  setSide(1);

  printCount = 0;
  //moveBaseServo(76);
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
  setStageTime(millis());
  setIRTimer(millis() - 5000); // this sets the time left on the gate to 0 by default at the start 
                               //to prevent accidentally driving through the gate if you never pick up IR
  stage = afterRampStage;
  // end of setup
  while (1) {
    if (startbutton()) {
      while (startbutton()) {
        moveArm(gatePos);
      }
      phase1();
    }

    switch (stage) {
      case beforeGateStage:
        if (gateStop()) {
          setIRTimer(millis());
        }

        // drive through the first gate regardless of IR gate state
        if (stageMilliseconds() < beforeGateMillis) {
          if (stageMilliseconds() < 400) {
            moveArm(gatePos);
          } else {
            moveArm(irPos);
          }
          stageSpeed(stage);
          PID4follow();

        // Stopping at the gate is the first priority. after stoppinng, move to the next stage
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
          } else if (timeLeft(millis()) > 1) {
          if (stageMilliseconds() < beforeGateMillis + 200) {
            PID4follow();
            moveArm(irPos);
          } else {
            setStageTime(millis());
            stage++;
          }
          
        // if you have moved close to the gate, and you dont have enough time left to get through you should slowly approach the gate
        } else if (stageMilliseconds() < beforeGateMillis + 300 && timeLeft(millis()) < 1) {
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
        break;

      case afterGateStage:
        moveArm(drivePos);
        if (flipSwitch()) {
          stageSpeed(stage);
          PID4follow();
        } else {
          setStageTime(millis());
          revStop();
          stage++;
        }

        break;

      case onRampStage:
      
        moveArm(drivePos);
        if (!flipSwitch()) {
          stageSpeed(stage);
          PID4follow();
        } else {
          setStageTime(millis());
          stage++;
        }
        break;

      case afterRampStage:
     
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
        break;

      case ringStage:
        stageSpeed(stage);
        revStop();
        delay(3000);
        crossTurn();
        delay(1000);
        phase2();
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
      LCD.print(leftSide());
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
  setStageTime((double)millis()); //probably dont need this line
  int nextPos;
  while (1) {
    // push start to start a new run from the beginning
    if (startbutton()) {
      while (startbutton()) {
        moveArm(gatePos);
      }
      phase1();
    }

    nextPos = getTargetPos();
    if (moveToPos(nextPos) && nextPos != -1) {
      delay(1000);
      setTargetPos(findNextToy(getCrossPos(), seconds()));
    } else if (getTargetPos() == -1) {
      if (moveToPos(4)) {
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

    if (startbutton()) {
      while (startbutton()) {
      }
      phase1();
    }
  }
}

void zipline() {
  while (1) {
    while(!moveToPos(zipPos)){
      
    }
    moveArm(zipPos);
    LCD.clear();
    printQRDs();
    setMotors(0, 0, 0);
  }
}


