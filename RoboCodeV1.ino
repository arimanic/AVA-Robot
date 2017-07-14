#include <phys253.h>
#include <LiquidCrystal.h>
#include <math.h>
#include <string.h>
#include <arrayHelpers.h>
#include <globConsts.h>
#include <Interrupts.h>
#include <PID.h>

// Variables for internal interrupts and logic checks. 
// All of these should be set during setup or phase transitions

//IR vars
bool alrdyStop;

// TIMING VARIABLE
int timeElapsed;

// Sonar interrupt variables
bool sonarInterrupt;
String offEdgeTurn;

// Wheel interrupt variables
int wheelRotations;

// Misc variables
int printCount;

void timer1ISR() {
  timeElapsed++;
}

void ISR1() { //!!! make this work for Lsonar
  sonarInterrupt = !sonarInterrupt;
  offEdgeTurn = "R";
}

void ISR2() { //!!! make this work for Rsonar
  sonarInterrupt = !sonarInterrupt;
  offEdgeTurn = "L";
}

void ISR3() { //!!! make this work for wheel measurement
  wheelRotations++;
}
