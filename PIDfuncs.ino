void PIDfollow(){
  //!!!
  
  getQRDs();
  getP4();
  
  
}

int getP4(){
  // To make the car turn right P is positive.
  // If P is negative the car turns left
  if (arrayEquals(QRDs, straight, numQRD)){
    pCon = 0;
  } else if (arrayEquals(QRDs, sLeft, numQRD)){
    pCon = -innerSpacing;
    lastTurn = "L";
  } else if (arrayEquals(QRDs, sRight, numQRD)){
    pCon = innerSpacing;
    lastTurn = "R";
  } else if (arrayEquals(QRDs, mLeft, numQRD)){
    pCon = -midSpacing;
     lastTurn = "L";
  } else if (arrayEquals(QRDs, mRight, numQRD)){
    pCon = midSpacing;
     lastTurn = "R";
  } else if (arrayEquals(QRDs, hardTurn, numQRD)){
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
  if (arrayEquals(QRDs, straight, numQRD)){
    pCon = 0;
  } else if (arrayEquals(QRDs, sLeft, numQRD)){
    pCon = -innerSpacing;
    lastTurn = "L";
  } else if (arrayEquals(QRDs, sRight, numQRD)){
    pCon = innerSpacing;
    lastTurn = "R";
  } else if (arrayEquals(QRDs, mLeft, numQRD)){
    pCon = -midSpacing;
     lastTurn = "L";
  } else if (arrayEquals(QRDs, mRight, numQRD)){
    pCon = midSpacing;
     lastTurn = "R";
  } else if (arrayEquals(QRDs, hardTurn, numQRD)){
    if (lastTurn == "L"){
      pCon = -outerSpacing;
    } else if (lastTurn == "R"){
      pCon = outerSpacing;
    }
  }
}

