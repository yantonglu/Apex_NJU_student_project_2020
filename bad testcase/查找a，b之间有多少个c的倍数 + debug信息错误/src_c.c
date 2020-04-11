#include <stdio.h>
 int main(void){
    long long int a,b,x,d;
    scanf("%lld %lld %lld",&a,&b,&x);
        d=b/x-a/x;
    if(a%x==0){
        d++;
    }
        printf("%lld\n",d);
 }

