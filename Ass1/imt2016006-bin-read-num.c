#include <stdio.h>

int main(int argc, char *argv[]){

  int i=0;
  int arr[20];


  FILE *f;
  f=fopen(argv[1],"rb");
  fread(arr,sizeof(int),20,f);
  printf("a\n");
  for(i=0; i<20; i++){
    printf("%d\n", arr[i]);
  }
  fclose(f);
}
