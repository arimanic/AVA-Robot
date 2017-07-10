bool arrayEquals(bool a1[], bool a2[], int len){
  for (int i = 0; i < len; i++){
    if (a1[i] != a2[i]){
      return false;
    }
  }
  return true;
}

void arraySubset(bool arr[], int startIndex, int newLength, bool newArr[]){
  newArr[newLength] = {0};
  for ( int i = 0; i <  newLength; i++){
    newArr[i] = arr[startIndex + i];
  }
  return;
}
