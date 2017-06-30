// PID vars
// P
int pCon = 0;
int innerSpacing = 4;
int midSpacing = 8;
int outerSpacing = 12;

int lastError, error;
String lastTurn;
// 4QRD arrays
bool straight4[numQRD] = {0, 1, 1, 0};
bool sLeft4[numQRD] =    {1, 1, 0, 0};
bool sRight4[numQRD] =   {0, 0, 1, 1};
bool mLeft4[numQRD] =    {1, 0, 0, 0};
bool mRight4[numQRD] =   {0, 0, 0, 1};
bool hardTurn4[numQRD] = {0, 0, 0, 0};

// 2QRD arrays
bool straight2[numQRD] = {1, 1};
bool sLeft2[numQRD] =    {1, 0};
bool sRight2[numQRD] =   {0, 1};
bool hardTurn2[numQRD] = {0, 0};

// I
int iCon = 0;

//D
int dCon = 0;

void PIDfollow(){
  //!!!
  
  getQRDs();
  getP4();
  
  
}

int getP4(){
  // To make the car turn right P is positive.
  // If P is negative the car turns left
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
  } else if (arrayEquals(QRDs, hardTurn4, numQRD)){
    if (lastTurn == "L"){
      pCon = -outerSpacing;
    } else if (lastTurn == "R"){
      pCon = outerSpacing;
    }
  }
}

int getP2(){
  // To make the car turn right P is positive.
  // If P is negative the car turns left
  if (arrayEquals(QRDs, straight2, numQRD)){
    pCon = 0;
  } else if (arrayEquals(QRDs, sLeft2, numQRD)){
    pCon = -innerSpacing;
    lastTurn = "L";
  } else if (arrayEquals(QRDs, sRight2, numQRD)){
    pCon = innerSpacing;
    lastTurn = "R";
  } else if (arrayEquals(QRDs, hardTurn2, numQRD)){
    if (lastTurn == "L"){
      pCon = -midSpacing;
    } else if (lastTurn == "R"){
      pCon = midSpacing;
    }
  }
}

