#include<stdio.h> 
int main(void){ 
  long a,b,n;
  int cnt=0;
  scanf("%ld %ld %ld",&a,&b,&n);
  if(a)
  while(a<=b){
    if(a%n==0){
      cnt++;
    }
    a+=1;
  }
  printf("%d",cnt);
  return 0;
 }

