
void menu() {
  int param, printMenu;
  double var;
  setMotors(0, 0, 0);

  // Load in preloaded variables
  vars[0] = getKP();
  vars[1] = getKI();
  vars[2] = getKD();
  vars[3] = getControlGain();
  vars[4] = getIRThresh();
  vars[5] = getSpeedScale();

  LCD.clear();

  while (1) {

    printMenu++;
    if (printMenu > 400) {
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
      } else if (params[param] == "X"){
        var = map(analogRead(6), 0 , 1023 , 0 , 1);
      } else {
        var = map(analogRead(6), 0 , 1023 , 0 , paramMax);
      }

      if (params[param] == "X"){       
        LCD.clear();
        LCD.print("Select side");
        LCD.setCursor(0,1);
        if (var == 1){
          LCD.print("R");
        } else {
          LCD.print("L");
        }
        
      } else {
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

void printParams(String names[] , double vals[]) {
  // Print all parameters to screen
  //!!! change these to get functions.
  //P
  LCD.clear();
  LCD.print(names[0]);
  LCD.print(getKP());
  LCD.print(" ");

  // I
  LCD.print(names[1]);
  LCD.print(getKI());
  LCD.print(" ");

  // D
  LCD.print(names[2]);
  LCD.print(getKD());
  LCD.print(" ");

  // controlGain
  LCD.setCursor(0, 1);
  LCD.print(names[3]);
  LCD.print(getControlGain());
  LCD.print(" ");

  // irThresh
  LCD.print(names[4]);
  LCD.print(getIRThresh());
  LCD.print(" ");

  // speedScale
  LCD.print(names[5]);
  LCD.print(getSpeedScale());
  LCD.print(" ");
}

void initConsts(int p, int i, int d, int g, int t, double s, int x){
  double arr[numVars] = {p, i, d, g, t, s, x};
  setArray(vars, arr, numVars);
  setKP(p);
  setKI(i);
  setKD(d);
  setControlGain(g);
  setSpeedScale(s);
  setIRThresh(t);
}



