#include <stdio.h>

int main(int argc, char *argv[]){

  int i=0;
  int arr[20];

  for(i=0; i<20; i++){
    arr[i]=i+1;
  }

  FILE *f;
  f=fopen(argv[1],"wb");
  fwrite(arr,sizeof(int),20,f);
  fclose(f);
}
