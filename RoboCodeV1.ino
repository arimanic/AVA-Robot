#include <arrayHelpers.h>
#include <globConsts.h>
#include <Interrupts.h>
#include <motor.h>
#include <phys253.h>
#include <phys253pins.h>
#include <PID.h>
#include <ServoTINAH.h>

// Variables for internal interrupts and logic checks. 
// All of these should be set during setup or phase transitions

//IR vars
bool alrdyStop;

// TIMING VARIABLE
int timeElapsed;

// Sonar interrupt variables
bool sonarInterrupt = false;
String offEdgeTurn;


// Wheel interrupt variables
int wheelRotations;

// Misc variables
int printCount;

void timer1ISR() {
  timeElapsed++;
}

void ISR1() { 
 sonarInterrupt = !sonarInterrupt;
 offEdgeTurn = "R";
}

void ISR2() { 
  sonarInterrupt = !sonarInterrupt;
  offEdgeTurn = "L";
}

void ISR3() { //!!! make this work for wheel measurement
  wheelRotations++;
}
