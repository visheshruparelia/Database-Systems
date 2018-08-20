#include <stdio.h>
#include <string.h>

struct Student{
  int rollno, age;
  char name[30];
};

int main(int argc, char *argv[]){

  struct Student s[5];


  FILE *f;
  f=fopen(argv[1],"rb");
  fread(s,sizeof(struct Student),5,f);
  fclose(f);

  printf("rollno,age,name\n");
  for(int i=0; i<5; i++){
    printf("%d,%s,%d\n",s[i].rollno,s[i].name,s[i].age);
  }

}
