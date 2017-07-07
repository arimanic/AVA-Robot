
// define a numerical value for each position
// define the voltage reading of the upper and lower Pot for each position.
// There are 4 positions: Driving   (D)
//                        Collecting (C)
//                        Fishing   (F)
//                        Zipline   (Z)
#define drivePos 0
#define baseD 0
#define hingeD 5

#define collectPos 1
#define baseC 0
#define hingeC 5

#define fishPos 2
#define baseF 0
#define hingeF 5

#define zipPos 3
#define baseZ 5
#define hingeZ 0

// define a threshold for measurement error. It is unlikely to read the pot when it is at the exact value defined above.
// instead try to catch it when it is close;
#define voltageRange 0.2


// Determines if the lower portion of the arm is at the position passed as pos within the error range passed as error. 
bool atLowerPosition(int pos , double error){
  if (pos == drivePos){
    
  } else if (pos == collectPos){
    
  } else if (pos == fishPos){
    
  } else if (pos == zipPos){
    
  }
}

// Determines if the lower portion of the arm is at the position passed as pos within the error range passed as error. 
bool atUpperPosition(int pos , double error){
  if (pos == drivePos){
    
  } else if (pos == collectPos){
    
  } else if (pos == fishPos){
    
  } else if (pos == zipPos){
    
  }
}

// Determines if the lower portion of the arm is at the position passed as pos within the error range passed as error.  
bool atBothPosition(int pos , double error){
  if (pos == drivePos){
    
  } else if (pos == collectPos){
    
  } else if (pos == fishPos){
    
  } else if (pos == zipPos){
    
  }
}

