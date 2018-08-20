#include <stdio.h>

int main(int argc, char *argv[]){

  FILE *f;
  char* filename;

  filename = argv[1];
  f = fopen(filename, "r");

  int i=0;
  int num;
  for(i=0;i<20;i++){

    fscanf(f, "%d", &num);
    printf("%d\n",num);
  }

  fclose(f);

}
