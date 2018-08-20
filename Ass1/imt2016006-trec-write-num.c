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
  f = fopen(filename, "w");
  int i=0;
  struct Student s[5];
  int rollno;
  char name[30];

  strcpy(s[0].name,"ncx4");
  s[0].rollno=11;
  s[0].age=10;

  strcpy(s[1].name,"ncx1");
  s[1].rollno=12;
  s[1].age=11;

  strcpy(s[2].name,"ncx2");
  s[2].rollno=13;
  s[2].age=12;

  strcpy(s[3].name,"ncx3");
  s[3].rollno=14;
  s[3].age=13;

  strcpy(s[4].name,"ncx4");
  s[4].rollno=15;
  s[4].age=14;


  for(i=0;i<5;i++){
    fprintf(f, "%d,%s,%d\n",s[i].rollno,s[i].name,s[i].age);
  }

  fclose(f);

}
