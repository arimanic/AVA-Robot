void arrSubset(bool arr[], int startIndex, int newLength, bool newArr[]){
  newArr[newLength] = {0};
  for ( int i = 0; i <  newLength; i++){
    newArr[i] = arr[startIndex + i];
  }
  return;
}

