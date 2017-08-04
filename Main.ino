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
  setIRThresh(300);
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

  /* Phase 1 moves the robot from the start point to the ring junction.
      It stops the robot at the 10 kHz gate, but only once.
      Edge sensors are used through external interrupts to prevent
  */

  // Phase 1 setup
  setStartTime(millis());
  setStageTime(millis());
  stage = 0;
  //moveUpperArm(drivePos);
  //moveLowerArm(drivePos);
  // end of setup

  LCD.clear();
  while (1) {
    switch (stage) {
      case beforeGateStage:
        /* go fast until you get close to the gate. Slow down when close.
          Stop while IR is on and then move to next stage when it turns off */

        if (stageMilliseconds() < beforeGateMillis) {
          // fast on flats leading up to gate

          if (stageMilliseconds() < 400) {
            moveArm(gatePos);
          } else {
            moveArm(irPos);
          }

          stageSpeed(stage);
          PID4follow();
        } else if (gateStop()) { //gateStop()
          // Stop at gate and move to next stage
          while (gateStop()) {  // gateStop()
            revStop();
          }
          setStageTime(millis());
          stage++;

        } else if (stageMilliseconds() < beforeGateMillis + 300) {
          // Slow down if close
          moveArm(irPos);
          stageSpeed(slowStage);
          PID4follow();
        } else {
          // if you get too close stop and wait for the IRs to go 10khz
          // to ensure that you have a full 5 seconds to get throughs
          moveArm(irPos);
          revStop();
        }
        break;

      case afterGateStage:
        moveArm(drivePos);
        if (stageMilliseconds() < afterGateMillis) {
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
        if (stageMilliseconds() < onRampMillis) {
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
        //delay(3000);
        crossTurn();
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
      LCD.print(stageMilliseconds());
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
  setCrossPos(-1);
  setTargetPos(0);
  setStartTime((double)millis());
  while (1) {
    if (moveToPos(getTargetPos()) && getTargetPos() != -1) {
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
      LCD.print(getTargetPos());
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
    LCD.clear();
    printQRDs();
    setMotors(0, 0, 0);
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
    pos = gatedKnobMap(6, 0, 9);
    servo = map(analogRead(7), 0, 1023, -2, 182);
    if (printCount > 400) {
      printCount = 0;

      LCD.clear();
      LCD.print(pos);
      LCD.print(" ");
      LCD.print(servo);
      LCD.print(" ");
      LCD.print(getRelLowerPos(pos));
      LCD.print(" ");
      LCD.print(getRelUpperPos(pos));

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
  int nextPos = getTargetPos();
  stageSpeed(ringStage);
  setStartTime(millis() - 55000L);
  while (1) {
    toysInWater(seconds());
    nextPos = getTargetPos();
    resetArmServo();

    if (moveToPos(nextPos) && nextPos != -1) {
      moveArm(nextPos);
      activateArmServo();

      delay(3000);
      stepMotors(100);
      //turnAround();
      // do the arm thing and then set the target to the next toy
      setTargetPos(findNextToy(getCrossPos(), seconds()));
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

      LCD.setCursor(0, 1);
      for (int i = 0; i < numToys; i++) {
        LCD.print(toyFallen[i]);
      }
    }

    if (startbutton()) {  // push start to reset the timer

      setStartTime((double)millis() - 57000);
      setCrossPos(-1);
      setTargetPos(0);
      toysInWater(seconds());
      while (startbutton()) {
        Serial.print(seconds());
      }
    }

    if (stopbutton()) {
      while (stopbutton()) {

      }
      menu();
    }
  }
}

void PIDdebug() {
  long testMillis = 0;
  int testStage = 0;
  setStageTime(millis());
  while (1) {
    stageSpeed(testStage);
    printCount++;
    testMillis = gatedKnobMap(7, 0, 5000);
    testStage = gatedKnobMap(6, 0, 5);

    if (stageMilliseconds() < testMillis) {
      setStopFlag(0);
      PID4follow();
    } else {
      revStop();
    }

    if (stopbutton()) {
      while (stopbutton()) {

      }
      menu();
    }

    if (startbutton()) {
      setStageTime(millis());
    }

    if (printCount > 400) {
      getQRDs();
      LCD.clear();
      printQRDs();
      LCD.print(stageMilliseconds());
      LCD.setCursor(0, 1);
      LCD.print(testMillis);
      LCD.print(" ");
      LCD.print(testStage);
      LCD.print(" ");
      LCD.print(getSpeedScale());

    }
  }
}

void armCalibrate() {
  double lowerVoltage;
  double upperVoltage;
  int servoVal;
  int tempServo;
  int servo0 = 0;
  int servo1 = 0;
  int servo2 = 0;
  int menuCounter = 0;

  while (1) {
    printCount++;
    lowerVoltage = constrainNum(knobToVolt(7), lowerBaseBound, upperBaseBound);
    upperVoltage = constrainNum(knobToVolt(6), lowerHingeBound, upperHingeBound);
    servoVal = gatedKnobMap(6, 0, 180);

    switch (menuCounter % 5) {
      case 0:
        moveUpperArm(upperVoltage);
        moveLowerArm(lowerVoltage);

        if (printCount > 400) {
          printCount = 0;
          LCD.clear();
          LCD.print("L");
          LCD.print(lowerVoltage);
          LCD.print(" ");
          LCD.print(getBaseMotorPot());
          LCD.print(" ");
          LCD.print(getRelLowerPos(lowerVoltage));

          LCD.setCursor(0, 1);
          LCD.print("U");
          LCD.print(upperVoltage);
          LCD.print(" ");
          LCD.print(getHingeMotorPot());
          LCD.print(" ");
          LCD.print(getRelUpperPos(upperVoltage));
        }
        break;

      case 1:
        RCServo0.write(servo0);
        tempServo = constrainNum(servoVal, 0, 180);
        if (startbutton()) {
          while (startbutton()) {
          }
          servo0 = tempServo;
        }
        if (printCount > 400) {
          printCount = 0;
          LCD.clear();
          LCD.print("Servo 0 was ");
          LCD.print(servo0);
          LCD.setCursor(0, 1);
          LCD.print(tempServo);
        }
        break;

      case 2:
        RCServo1.write(servo1);
        tempServo = constrainNum(servoVal, 0, 180);
        if (startbutton()) {
          while (startbutton()) {
          }
          servo1 = tempServo;
        }
        if (printCount > 400) {
          printCount = 0;
          LCD.clear();
          LCD.print("Servo 1 was ");
          LCD.print(servo1);
          LCD.setCursor(0, 1);
          LCD.print(tempServo);
        }
        break;

      case 3:
        RCServo2.write(servo2);
        tempServo = constrainNum(servoVal, 0, 180);
        if (startbutton()) {
          while (startbutton()) {
          }
          servo2 = tempServo;
        }
        if (printCount > 400) {
          printCount = 0;
          LCD.clear();
          LCD.print("Servo 2 was ");
          LCD.print(servo2);
          LCD.setCursor(0, 1);
          LCD.print(tempServo);
        }
        break;

      case 4:
        while (stopbutton()) {

        }
        menu();
        menuCounter++;
        break;

    }

    if (stopbutton()) {
      while (stopbutton()) {
      }
      menuCounter++;
    }
  }
}

void motorDebug() {
  int x = 0;
  int y = 0;
  setSpeedScale(1);
  while (1) {
    x = gatedKnobMap(7, -255, 255);
    y = gatedKnobMap(6, -255, 255);
    setMotors(x, y, 0);
    LCD.clear();
    LCD.print(x);
    LCD.print(" ");
    LCD.print(y);
  }
}

void ziplineDebug() {
  moveArm(drivePos);
  if (atBothPos(drivePos)) {
    while (1) {
      printCount++;

      moveArm(zipPos);

      if (printCount > 400) {
        LCD.clear();
        printQRDs();
      }
    }
  }
}

