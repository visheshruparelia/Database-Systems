#include <stdio.h>

int main(int argc, char *argv[]){

  FILE *f;
  char* filename;

  filename = argv[1];
  f = fopen(filename, "w");

  int i=0;

  for(i=0;i<20;i++){

    fprintf(f, "%d\n", i);
  }

  fclose(f);

}
