int endCycle;
int startCycle;

 void setDelay(int cycles){
  startCycle = overflowCount;
    endCycle = startCycle + cycles;
  }

bool checkDelay(){
  if (overflowCount >= endCycle){
    return true;
  } else {
    return false;
  }
}

// make a new long that counts total timer overflows and never resets.
// when a pulse starts set a variable equal to the current totaloverflows
// when a pulse ends compare the start value to the new current totaloverflows
// this gives a time metric that can be calibrated and then used by the sonar sensors

