#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>
static FILE* fpexec_avoid_crash = NULL;
static FILE* fpargs_avoid_crash = NULL;
static FILE* fpvars_avoid_crash = NULL;
static FILE* fpinput_avoid_crash = NULL;
static void* memory_avoid_crash = NULL;
 int args_avoid_crash = 0;
 int argc_avoid_crash;
 char** argv_avoid_crash;

void __instrument1(long id, unsigned long value) {
  
char file_name[8];file_name[0]='e';file_name[1]='x';file_name[2]='e';
file_name[3]='c';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[8]='\0';
  char format[15];format[0]='(';format[1]='I';format[2]=' ';format[3]='%';format[4]='l';format[5]='d';format[6]=' ';format[7]='%';format[8]='l';format[9]='d';
format[10]=' ';format[11]='1';format[12]=')';format[13]='\n';format[14]='\0';
  char open[2];
open[0]='w';open[1]='\0';
  


  if (!fpexec_avoid_crash) fpexec_avoid_crash = fopen(file_name, open);
  fprintf(fpexec_avoid_crash, format, id, value);
  fflush(fpexec_avoid_crash);
}

void __instrument2(long id, unsigned long value) {
  char file_name[8];file_name[0]='e';file_name[1]='x';file_name[2]='e';
file_name[3]='c';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[8]='\0';
  char format[15];format[0]='(';format[1]='I';format[2]=' ';format[3]='%';format[4]='l';format[5]='d';format[6]=' ';format[7]='%';format[8]='l';format[9]='d';
format[10]=' ';format[11]='2';format[12]=')';format[13]='\n';format[14]='\0';
  char open[2];
open[0]='w';open[1]='\0';
 

 if (!fpexec_avoid_crash) fpexec_avoid_crash = fopen(file_name, open);
  fprintf(fpexec_avoid_crash, format, id, value);
  fflush(fpexec_avoid_crash);
}

void __main_args(void* ptr) {

  char file_name[8];file_name[0]='a';file_name[1]='r';file_name[2]='g';
file_name[3]='s';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[8]='\0';
  char format[8];format[0]='%';format[1]='l';format[2]='x';format[3]=' ';format[4]='%';format[5]='d';format[6]='\n';format[7]='\0';
  char open[2];
open[0]='w';open[1]='\0';
char context[2];
context[0]='\n'; context[1]='\0';
 


 if (!fpargs_avoid_crash) fpargs_avoid_crash = fopen(file_name, open);
  // fprintf(fpargs, "(args %d %ld)\n", args, ptr);
  // fflush(fpargs);

  if (args_avoid_crash == 0)
    argc_avoid_crash = (int)ptr;
  else {
    argv_avoid_crash = (char**)ptr;
    for(int i = 1; i < argc_avoid_crash; i++) {
      char* p = argv_avoid_crash[i];
      do {
        fprintf(fpargs_avoid_crash,format, (unsigned long)p, *p);
      } while(*(p++));
      fprintf(fpargs_avoid_crash, context);
    }
  }
  fflush(fpargs_avoid_crash);
	
  args_avoid_crash++;
}

/*
void INPUT_CHAR_ARRAY(int* var) {
char file_name[8];file_name[0]='a';file_name[1]='r';file_name[2]='g';
file_name[3]='s';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[7]='\0';
  char format[15];format[0]='%';format[1]='l';format[2]='x';format[3]=' ';format[4]='%';format[5]='d';format[6]='\n';format[7]='\0';
  char open[2];
open[0]='w';open[1]='\0';
  if (!fpargs_avoid_crash) fpargs_avoid_crash = fopen(file_name, open);

file_name[0]='i';file_name[1]='n';file_name[2]='p';
file_name[3]='u';file_name[4]='t';file_name[5]='\0';
open[0]='r';open[1]='\0';
  if (!fpinput_avoid_crash) fpinput_avoid_crash = fopen(file_name, open);

  char c;

  do {
    *(var) = c = fgetc(fpinput_avoid_crash);
    fprintf(fpargs_avoid_crash, format, (unsigned long)var, *var);
    fflush(fpargs_avoid_crash);
    ++var;
  } while (c != EOF);
  *(var) = EOF;

  // *var = fgetc(fpinput);
  //fscanf(fpinput, "%c", var);
}

void INPUT_VARIABLE(int* var) {
char file_name[8];file_name[0]='a';file_name[1]='r';file_name[2]='g';
file_name[3]='s';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[7]='\0';
  char format[15];format[0]='%';format[1]='d';format[2]='\0';format[3]=' ';format[4]='%';format[5]='d';format[6]='\n';format[7]='\0';
  char open[2];
open[0]='w';open[1]='\0';
  if (!fpargs_avoid_crash) fpargs_avoid_crash = fopen(file_name, open);

file_name[0]='i';file_name[1]='n';file_name[2]='p';
file_name[3]='u';file_name[4]='t';file_name[5]='\0';
open[0]='r';open[1]='\0';
  if (!fpinput_avoid_crash) fpinput_avoid_crash = fopen(file_name, open);

  fscanf(fpinput_avoid_crash, format, var);
format[0]='%';format[1]='l';format[2]='x';format[3]=' ';format[4]='%';format[5]='d';format[6]='\n';format[7]='\0';
  fprintf(fpargs_avoid_crash, "%lx %d\n", (unsigned long)var, *var);
  fflush(fpargs_avoid_crash);
}

void INPUT_ARRAY(int* array, int len, int n) {
  char file_name[8];file_name[0]='a';file_name[1]='r';file_name[2]='g';
file_name[3]='s';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[7]='\0';
  char format[15];format[0]='%';format[1]='l';format[2]='x';format[3]=' ';format[4]='%';format[5]='d';format[6]='\n';format[7]='\0';
  char open[2];
open[0]='w';open[1]='\0';
  if (!fpargs_avoid_crash) fpargs_avoid_crash = fopen(file_name, open);

file_name[0]='i';file_name[1]='n';file_name[2]='p';
file_name[3]='u';file_name[4]='t';file_name[5]='\0';
open[0]='r';open[1]='\0';
  if (!fpinput_avoid_crash) fpinput_avoid_crash = fopen(file_name, open);

char format2[3];format2[0]='%';format2[1]='d';format2[2]='\0';
  for (int i = 0; i < n; i++) {
    int *v = &array[i];
    fscanf(fpinput_avoid_crash, format2, v);
    fprintf(fpargs_avoid_crash, format, (unsigned long)v, *v);
    fflush(fpargs_avoid_crash);
  }
}

void INPUT_MATRIX(int* matrix, int row, int col, int m, int n) {
  char file_name[8];file_name[0]='a';file_name[1]='r';file_name[2]='g';
file_name[3]='s';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[7]='\0';
  char format[15];format[0]='%';format[1]='l';format[2]='x';format[3]=' ';format[4]='%';format[5]='d';format[6]='\n';format[7]='\0';
  char open[2];
open[0]='w';open[1]='\0';
  if (!fpargs_avoid_crash) fpargs_avoid_crash = fopen(file_name, open);
char format2[3];format2[0]='%';format2[1]='d';format2[2]='\0';
file_name[0]='i';file_name[1]='n';file_name[2]='p';
file_name[3]='u';file_name[4]='t';file_name[5]='\0';
open[0]='r';open[1]='\0';
  if (!fpinput_avoid_crash) fpinput_avoid_crash = fopen(file_name, open);


  for (int r = 0; r < m; r++) {
    for (int c = 0; c < n; c++) {
      int *v = &matrix[r * col + c];
      fscanf(fpinput_avoid_crash, format2, v);
      fprintf(fpargs_avoid_crash, format, (unsigned long)v, *v);
      fflush(fpargs_avoid_crash);
    }
  }
  fflush(fpargs_avoid_crash);
}

void* __my_malloc(int size) {
  if (!memory_avoid_crash) {
    memory_avoid_crash = mmap((void*)0xa0000000, 0x10000000, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_FIXED | MAP_ANON, -1, 0);
    assert (memory != MAP_FAILED);
  }
  void* addr = memory;
  memory += size;
  return addr;
}


void INTERNAL_ARRAY(void** array, int len) {
  char file_name[8];file_name[0]='v';file_name[1]='a';file_name[2]='r';
file_name[3]='s';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[7]='\0';
  char format[15];format[0]='%';format[1]='l';format[2]='x';format[3]=' ';format[4]='%';format[5]='l';format[6]='d';format[7]='\n';format[8]='\0';
  char open[2];
open[0]='w';open[1]='\0';
  if (!fpvars_avoid_crash) fpvars_avoid_crash = fopen(file_name, open);

  *array = __my_malloc(len);
  fprintf(fpvars_avoid_crash, format, (unsigned long)array, (unsigned long)*array);
  fflush(fpvars_avoid_crash);
}

void INTERNAL_VARIABLE(int* var) {
  char file_name[8];file_name[0]='v';file_name[1]='a';file_name[2]='r';
file_name[3]='s';file_name[4]='.';file_name[5]='o';file_name[6]='u';file_name[7]='t';file_name[7]='\0';
  char format[15];format[0]='%';format[1]='l';format[2]='x';format[3]=' ';format[4]='%';format[5]='d';format[6]='\n';format[7]='\0';
  char open[2];
open[0]='w';open[1]='\0';
  if (!fpvars_avoid_crash) fpvars_avoid_crash = fopen(file_name, open);

  fprintf(fpvars_avoid_crash, format, (unsigned long)var, *var);
  fflush(fpvars_avoid_crash);
}

*/


int pow(int x, int n) {
  int r = 1;
  for (int i = 0; i < n; i++)
    r *= x;
  return r;
}

void DESCRIPTION(const char* str) { }
void END_DESCRIPTION() { }

