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

long overflowCount = 0;
void timer0ISR() {
  overflowCount++;
  if (overflowCount >= 61){
  timeElapsed++;
  overflowCount = 0;
  }
}
int timeElapsedA = 0;
void timer3ISR() {
  timeElapsedA++;
  
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






namespace {
  void(*timer)();
}
ISR(TIMER0_COMP_vect) {
  (*timer)();
}

/* Configures Timer1 to call an interrupt routine with the desired frequency  */
/* The interrupt routine that is called is ISR(TIMER1_COMPA_vect)             */
/* Valid interrupt frequencies: 1Hz to 65535 Hz                               */
/* If the frequency is impossible to achieve, no interrupt will be configured */
/* Timer 1 affects Motor 1 */
void attachTimer0Interrupt(unsigned int interruptFrequencyHz, void(*f)()){
int overflowsNeeded = 100;

      cli();
      TCCR0 = 0;                         /* Clear current comparison value */
      TCNT0 = 0;                         /* Clear current timer value      */
      TCCR0 = (1 << CS00);              /* Set timer comparison mode      */
      TCCR0 |= (1 << CS01);
      TCCR0 |= (1 << CS02);
      TIMSK |= (1 << OCIE0);            /* Set timer interrupt enable     */
      timer = f;
      sei();
      return;

}
