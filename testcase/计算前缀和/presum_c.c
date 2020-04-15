#include <stdio.h>
int a[5] = {5, 6, 3, 4, 5};
int pre_sum[6] = {0, 0, 0, 0, 0, 0};
int main(void)
{
int i = 0;
for(i = 1; i < 6; i++){
pre_sum[i] = pre_sum[i - 1] + a[i - 1]; 
}
for(i = 1; i < 6; i++)
printf("%d ", pre_sum[i]);
return 0;
}
