// PID vars
// P
int innerSpacing = 4;
int midSpacing = 8;
int outerSpacing = 12;
String lastTurn;

// D
int lastError, error , recentError;
int prevDTime = 0;
int curDTime = 0;

// 4QRD arrays
bool straight4[numQRD] = {0, 1, 1, 0};
bool sLeft4[numQRD] =    {0, 1, 0, 0};
bool sRight4[numQRD] =   {0, 0, 1, 0};
bool mLeft4[numQRD] =    {1, 1, 0, 0};
bool mRight4[numQRD] =   {0, 0, 1, 1};
bool hLeft4[numQRD] =    {1, 0, 0, 0};
bool hRight4[numQRD] =   {0, 0, 0, 1};
bool hardTurn4[numQRD] = {0, 0, 0, 0};

// 2QRD arrays
bool straight2[numQRD] = {1, 1};
bool sLeft2[numQRD] =    {1, 0};
bool sRight2[numQRD] =   {0, 1};
bool hardTurn2[numQRD] = {0, 0};
bool QRDs2 [2] = {0};


void PID4follow(){
  //!!!
  
  getQRDs();
  getP4();
  getD();

  setMotors(255,255,controlGain*(pCon+dCon));
  
}

void PID2follow(){
  //!!!
  
  getQRDs();
  getP2();
  getD();
  
}

int getP4(){
  // To make the car turn right P is positive.
  // If P is negative the car turns left
  // Sets the error to be used by D control
  if (arrayEquals(QRDs, straight4, numQRD)){
    pCon = 0;
  } else if (arrayEquals(QRDs, sLeft4, numQRD)){
    pCon = -innerSpacing;
    lastTurn = "L";
  } else if (arrayEquals(QRDs, sRight4, numQRD)){
    pCon = innerSpacing;
    lastTurn = "R";
  } else if (arrayEquals(QRDs, mLeft4, numQRD)){
    pCon = -midSpacing;
     lastTurn = "L";
  } else if (arrayEquals(QRDs, mRight4, numQRD)){
    pCon = midSpacing;
     lastTurn = "R";
  } else if (arrayEquals(QRDs, hLeft4, numQRD)){
    pCon = -outerSpacing;
     lastTurn = "L";
  } else if (arrayEquals(QRDs, hRight4, numQRD)){
    pCon = outerSpacing;
     lastTurn = "R";
  } else if (arrayEquals(QRDs, hardTurn4, numQRD)){
    if (lastTurn == "L"){
      pCon = -outerSpacing - innerSpacing;
    } else if (lastTurn == "R"){
      pCon = outerSpacing + innerSpacing;
    }
  }
  error = pCon;
  pCon = kp*pCon;
  return pCon;
}

int getP2(){
  // To make the car turn right P is positive.
  // If P is negative the car turns left
  arraySubset(QRDs , 1 , 2 , QRDs2);
  if (arrayEquals(QRDs2, straight2, numQRD)){
    pCon = 0;
  } else if (arrayEquals(QRDs2, sLeft2, numQRD)){
    pCon = -innerSpacing;
    lastTurn = "L";
  } else if (arrayEquals(QRDs2, sRight2, numQRD)){
    pCon = innerSpacing;
    lastTurn = "R";
  } else if (arrayEquals(QRDs2, hardTurn2, numQRD)){
    if (lastTurn == "L"){
      pCon = -midSpacing;
    } else if (lastTurn == "R"){
      pCon = midSpacing;
    }
  }
  
error = pCon;
  pCon = kp*pCon;
  return pCon;
}

double getD(){
  // To make the car turn right D is positive.
  // If D is negative the car turns left
  // error is set by getPx()

  if (lastError != error) {
    recentError = lastError;
    prevDTime = curDTime;
    curDTime = 0;
  }
  curDTime++;
  lastError = error;
  dCon = ((double)kd*(error - recentError))/((double) (prevDTime + curDTime));
  return dCon;
}

