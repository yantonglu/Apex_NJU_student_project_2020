#include"stdio.h"
int a[7];
int main(){
	int tar = 7;
	int i = 0;
	for(; i < 7; i++)
	a[i] = i + 1;
	int left = 0, right = 6;
	while(left <= right){
	int mid = (left + right) / 2;
	if(a[mid] > tar)
		right = mid - 1;
	else if(a[mid] == tar)
		{printf("%d", mid);
		return 0;}
	else
		left = mid + 1; 
	
	}
	printf("-1");
	return 0;
}
