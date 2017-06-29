bool arrayEquals(bool a1[], bool a2[], int len){
  for (int i = 0; i < len; i++){
    if (a1[i] != a2[i]){
      return false;
    }
  }
  return true;
}
