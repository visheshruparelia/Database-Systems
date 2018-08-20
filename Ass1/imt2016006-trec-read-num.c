#include <stdio.h>
#include <string.h>

struct Student{
  int rollno;
  char name[30];
  int age;
};

int main(int argc, char *argv[]){

  FILE *f;
  char* filename;

  filename = argv[1];
  f = fopen(filename, "r");
  int i=0;
  struct Student s[5];


    for(i=0;i<5;i++){
      fscanf(f, "%d,%[^,],%d\n",&s[i].rollno,s[i].name,&s[i].age);
      printf("%d,%s,%d\n",s[i].rollno,s[i].name,s[i].age);
    }

    fclose(f);

}
