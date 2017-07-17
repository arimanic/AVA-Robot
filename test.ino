int crossPos;
bool onCross;

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

void moveToPos(int pos) {
  if (pos > 6) {
    LCD.clear();
    LCD.print("Invalid pos");
    return;
  }

  while (crossPos != pos) {
    if (atCross() && !onCross) {
      onCross = true;
      if (crossPos >= 6) {
        crossPos = 1;
      } else {
        crossPos++;
      }
    } else if (!atCross() && onCross) {
      onCross = false;
    }

    PID2follow();
  }
}

void toysInWater(int curTime) {
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

  if (atCross()) {
    onCross = true;
  } else {
    onCross = false;
  }

  while (crossCount <= 6) {
    if (atCross() && !onCross) {
      crossCount++;
    } else if (!atCross() && onCross) {
      onCross = false;
    }

    PID2follow();
  }
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


