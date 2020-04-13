#include<stdio.h> 

int main() {
  int a = 9999;
  int n = 0;
  while (a > 0)
{
  n++;
  a = a/10;
}
  printf("%d",n);
  return 0;
}
