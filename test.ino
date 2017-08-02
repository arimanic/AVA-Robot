double knobToVolt(int knob){
  return analogRead(knob)*5.0/1024.0;
}

double constrainNum(double val, double min, double max){
  if (val < min){
    return min;
  }
  if (val > max){
    return max;
  }
  return val;
}

