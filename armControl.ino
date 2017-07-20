// Arm movement is controlled by these functions.
// Movement is split into upper arm, lower arm, and base servo
// The upper and lower arms are controlled by motor feedback loops. 
// On each iteration the loop reads the displacement from the desired position and produces a 
// motor speed towards the desired position. The speed is proportional to the displacement.


// define a threshold for measurement error. It is unlikely to read the pot when it is at the exact value defined above.
// instead try to catch it when it is close;
#define voltageRange 0.1

// define a scaling factor for the movement speed of the arm. motor speed = armSpeed*displacement
#define armSpeed 50


double getRelLowerPos(int pos) {
  // Determines if the lower portion of the arm is at the position passed as pos
  // returns a negative number if the position is low. Positive number if high. 0 if at the right spot
  double voltRead = getBaseMotorPot();
  if (pos == drivePos) {
    return voltRead - baseD;
  } else if (pos == collectPos) {
    return voltRead - baseC;
  } else if (pos == fishPos) {
    return voltRead - baseF;
  } else if (pos == zipPos) {
    return voltRead - baseZ;
  }
}

double getRelUpperPos(int pos) {
  // Determines if the upper portion of the arm is at the position passed as pos
  // returns a negative number if the position is low. Positive number if high. 0 if at the right spot
  double voltRead = getHingeMotorPot();
  if (pos == drivePos) {
    return voltRead - hingeD;
  } else if (pos == collectPos) {
    return voltRead - hingeC;
  } else if (pos == fishPos) {
    return voltRead - hingeF;
  } else if (pos == zipPos) {
    return voltRead - hingeZ;
  }
}

bool atLowerPos(int pos) {
  //Determine if the lower portion of the arm are in the position passed
  double relLower = getRelLowerPos(pos);
  return abs(relLower) <= voltageRange;
}

bool atUpperPos(int pos) {
  //Determine if the upper portion of the arm are in the position passed
  double relUpper = getRelUpperPos(pos);
  return abs(relUpper) <= voltageRange;
}

bool atBothPos(int pos) {
  // Determines if the upper and lower portions of the arm are at the position passed as pos within the error range defined as voltageRange
  return atUpperPos(pos) && atLowerPos(pos);
}

void moveLowerArm(int pos) {
  while (!atLowerPos(pos)) {
    motor.speed(armHingeMotorPin, armSpeed * getRelLowerPos(pos));
  }
  motor.stop(armBaseMotorPin);
  return;
}

void moveUpperArm(int pos) {
  while (!atUpperPos(pos)) {
    motor.speed(armHingeMotorPin, armSpeed * getRelUpperPos(pos));
  }
  motor.stop(armHingeMotorPin);
  return;
}

void moveBaseServo(int val){
 // Moves the base servo to the desired position. 90 is pointed forward
  if (val > 179){
    val = 179;
  } else if (val < 0){
    val = 0;
  }

RCServo0.write(val);
  return;
}

double getBaseMotorPot(){
  //Read the analog voltage value of the Potentiometer at the base of the arm
  // 0 - 5
  return analogRead(armBasePotPin)*5.0/1024.0;
}

double getHingeMotorPot(){
  //Read the analog voltage value of the Potentiometer at the hinge of the arm
  // 0 -5
  return analogRead(armHingePotPin)*5.0/1024.0;
}
