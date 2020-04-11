#include"stdio.h"
int a[7] = {1,3,10,8,7,9,5};
void SWAP(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main(){
	SWAP(&a[1], &a[2]);
	printf("%d" , a[1]);
	return 0;
}
