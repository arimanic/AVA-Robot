
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

      if (params[param] == "T") {
        var = analogRead(6);
      } else if (params[param] == "S") {
        var = analogRead(6) / 1023.0;
      } else {
        var = map(analogRead(6), 0 , 1023 , 0 , paramMax);
      }

      LCD.clear();
      LCD.print(params[param]);
      LCD.print(var);
      LCD.setCursor(0, 1);
      if (params[param] == "T") {
        LCD.print("Rs");
        LCD.print(digitalRead(QRD1pin));
        LCD.print(" Ls");
        LCD.print(digitalRead(QRD2pin));
      } else {
        LCD.print(" Last = ");
        LCD.print(vars[param]);
      }
    }

    // Exit the menu and save parameters
    if (stopbutton()) {
      while (stopbutton()) {
      }
      setKP((int) vars[0]);
       setKI((int) vars[1]);
       setKD((int) vars[2]);
      setControlGain((int) vars[3]);
setSpeedScale(vars[5]);
      irThresh = (int) vars[4];
      // setIRThresh();
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




void setServoPos(int pos){
  
  if (pos > 179 || pos < 0){
    LCD.clear();
    LCD.println("Invalid servo");
    LCD.print(pos);
    delay(300);
    return;
  } 

RCServo0.write(pos);
  return;
}

void printParams(String names[] , double vals[]) {
// Print all parameters to screen
//!!! change these to get functions.
  LCD.clear();
  LCD.print(names[0]);
  LCD.print((int)vals[0]);
  LCD.print(" ");
  
  LCD.print(names[1]);
  LCD.print((int)vals[1]);
  LCD.print(" ");
  
  LCD.print(names[2]);
  LCD.print((int)vals[2]);
  LCD.print(" ");

  LCD.setCursor(0, 1);
  LCD.print(names[3]);
  LCD.print((int)vals[3]);
  LCD.print(" ");
  
  LCD.print(names[4]);
  LCD.print((int)vals[4]);
  LCD.print(" ");
  
  LCD.print(names[5]);
  LCD.print(vals[5]);
  LCD.print(" ");
}





