#include"stdio.h"
int a[7] = {1,3,10,8,7,9,5};
void SWAP(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
void BubbleSort(int a[], int n){
	for(int i = n - 1; i >= 0; i--)
		for(int j = 0; j < i; j++)
			{
				if(a[j] > a[j + 1])
					{
						SWAP(&a[j], &a[j + 1]);						
					}
			}
} 
int main(){
	BubbleSort(a, 7);
	int median = a[3];
	printf("%d" , median);
	return 0;
}
