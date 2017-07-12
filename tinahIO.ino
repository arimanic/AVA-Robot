void menu() {
  int param, printMenu;
  double var;
  setMotors(0, 0, 0);

  LCD.clear();
  LCD.print("Menu");
  delay(300);

  while (1) {

    printMenu++;
    if (printMenu > 40) {
      printMenu = 0;

      param = map(analogRead(7), 0, 1000, 0, numVars - 1);
      if (param > numVars) {
        param = numVars;
      }

      if (param < 0) {
        param = 0;
      }

      if (params[param] == "Th") {
        var = analogRead(6);
        getQRDs();
      } else if (params[param] == "Sp") {
        var = analogRead(6) / 1023.0;
      } else {
        var = map(analogRead(6), 0 , 1023 , 0 , 150);
      }

      LCD.clear();
      LCD.print(params[param]);
      LCD.print(var);
      LCD.setCursor(0, 1);
      if (params[param] == "Th") {
        LCD.print("Rs");
        LCD.print(QRDs[2]);
        LCD.print(" Ls");
        LCD.print(QRDs[1]);
      } else {
        LCD.print(" Last = ");
        LCD.print(vars[param]);
      }
    }

    // Exit the menu and save parameters
    if (stopbutton()) {
      while (stopbutton()) {
      }
      kp = (int) vars[0];
      ki = (int) vars[1];
      kd = (int) vars[2];
      controlGain = (int) vars[3];
      speedScale =  vars[5];
      irThresh = (int) vars[4];
      LCD.clear();
      return;

    }

    // Save the current variable
    if (startbutton()) {
      while (startbutton()) {
      }
      vars[param] = var;
    }
  }
}

void setMotors(int L, int R, int ctrl) {
  // Calculates and sets motor speed and control
  motor.speed(RmotorPin, (R + ctrl)*speedScale);
  motor.speed(LmotorPin, (L - ctrl)*speedScale);
}

void printParams() {
// Print all parameters to screen
  LCD.clear();
  LCD.print("S");
  LCD.print(speedScale);
  LCD.print(" T");
  LCD.print(irThresh);
  LCD.print(" G");
  LCD.print(controlGain);

  LCD.setCursor(0, 1);
  LCD.print("P");
  LCD.print(kp);
  LCD.print(" I");
  LCD.print(ki);
  LCD.print(" D");
  LCD.print(kd);
}

void getQRDs() {
  // Reads all QRD sensors and stores boolean values in QRDs array
  for (int i = 0; i < numQRD; i++) {
    QRDs[i] = digitalRead(i);
  }
}

bool getQRD(int QRDnum) {
  // Reads the given QRD sensor and stores boolean value in QRDs array 
  // Returns true or false for the given QRD
    QRDs[QRDnum] = digitalRead(QRDnum);
    return QRDs[QRDnum];
  }

double getWheelFreq(){
  //!!!
}

void setServoPos(int pos){
  
  if (pos > 180 || pos < 0){
    LCD.clear();
    LCD.println("Invalid servo");
    LCD.print(pos);
    delay(300);
    return;
  } 

RCServo0.write(pos);
  return;
}


