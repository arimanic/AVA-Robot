int crossPos;
bool onCross = false;

bool toyFallen[6] = {0};
bool toys85[6] = {1, 1, 1, 1, 1, 1};
bool toys80[6] = {1, 1, 1, 1, 1, 0};
bool toys75[6] = {1, 1, 1, 1, 0, 0};
bool toys70[6] = {1, 1, 1, 0, 0, 0};
bool toys65[6] = {1, 1, 0, 0, 0, 0};
bool toys60[6] = {1, 0, 0, 0, 0, 0};

int findCross(int crossNum) {
  int dist;
  if (crossNum >= crossPos) {
    dist = crossNum - crossPos ;
  } else {
    dist = crossNum + 6 - crossPos;
  }

  return dist;
}

void setCrossPos(int newPos) {
  crossPos = newPos;
}

bool moveToPos(int pos) {
  if (pos > 5) {
    LCD.clear();
    LCD.print("Invalid pos");
    return false;
  }

  if (crossPos != pos) {
    if (atCross() && !onCross) {
      onCross = true;
      if (crossPos >= numToys - 1) {
        crossPos = 0;
      } else {
        crossPos++;
      }
    } else if (!atCross() && onCross) {
      onCross = false;
    }

    PID2follow(); 
    return false;   
  } else {
    revStop();
    return true;
  }
}

void toysInWater(double curTime) {
  /*
     Given a time, determine which toys have already fallen off their platforms
  */
  if (curTime > 85) {
    setArray(toyFallen, toys85, numToys);
  } else if (curTime > 80) {
    setArray(toyFallen, toys80, numToys);
  } else if (curTime > 75) {
    setArray(toyFallen, toys75, numToys);
  } else if (curTime > 70) {
    setArray(toyFallen, toys70, numToys);
  } else if (curTime > 65) {
    setArray(toyFallen, toys65, numToys);
  } else if (curTime > 60) {
    setArray(toyFallen, toys60, numToys);
  }
}

void fullCircleMove() {
  int crossCount = 0;


  if (crossCount <= 6) {
    if (atCross() && !onCross) {
      onCross = true;
      crossCount++;
      crossPos++;
    } else if (!atCross() && onCross) {
      onCross = false;
    }

// If a transition happens here a cross count will be missed.
    PID2follow();
  }
}

int getCrossPos() {
  return crossPos;
}

//void circleMove() {
//  int crossCount = 0;
//
//  if(atCross()){
//    onCross = true;
//  } else {
//    onCross = false;
//  }
//
//  while (crossCount <= 6){
//  if (atCross() && !onCross) {
//    crossCount++;
//  } else if (!atCross() && onCross) {
//    onCross = false;
//  }
//
//  PID2follow();
//}
//}

int findNextToy(int pos , double curTime){
  if (pos == 5){
    pos = -1;
  }
  
  for (int i = pos+1; i < numToys; i++){
    if (!toyFallen[i]){
      return i;
    }
  }

  return -1;
}

