#include <stdio.h>
#include <string.h>

struct Student{
  int rollno, age;
  char name[30];
};

int main(int argc, char *argv[]){

  struct Student s[5];
  char buf[20];

  for(int i=0; i<5; i++){
    sprintf(buf, "somename_%d", i);
    s[i].rollno=i+50;
    s[i].age=i+20;
    strcpy(s[i].name,buf);
  }

  FILE *f;
  f=fopen(argv[1],"wb");
  fwrite(s,sizeof(struct Student),5,f);
  fclose(f);
}
