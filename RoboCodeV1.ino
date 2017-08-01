#include <armControl.h>
#include <arrayHelpers.h>
#include <globConsts.h>
#include <Interrupts.h>
#include <IR.h>
#include <motor.h>
#include <phys253.h>
#include <phys253pins.h>
#include <PID.h>
#include <ringControl.h>
#include <ServoTINAH.h>
#include <sonar.h>
#include <timing.h>
#include <TinahIO.h>



// Variables for internal interrupts and logic checks.
// All of these should be set during setup or phase transitions

//IR vars
bool alrdyStop;

// TIMING VARIABLE

// Sonar interrupt variables
bool sonarInterrupt = false;
String offEdgeTurn;

// Wheel interrupt variables
int wheelTicks;


///////////////////////
/// ISR definitions ///
///////////////////////


void ISR1() {
  offEdgeTurn = "R";

  //  if (duration >= getSonarThresh()) {
  //    sonarInterrupt = 1;
  //  } else {
  //    sonarInterrupt = 0;
  //  }
}

void ISR2() {
  offEdgeTurn = "L";

  //  if (duration >= getSonarThresh()) {
  //    sonarInterrupt = 1;
  //  } else {
  //    sonarInterrupt = 0;
  //  }
}



void ISR3() { //!!! make this work for wheel measurement
    wheelTicks++;
}






