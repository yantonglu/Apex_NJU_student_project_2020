#include"stdio.h"
int a[7] = {1,3,10,8,7,9,5};
void SWAP(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
int partition(int a[],int p, int r){
	int pivot = a[r];
	int i = p - 1;
	int j = 0;
	for(j = p; j <= r - 1; j++){
		if(a[j] < pivot){
			i++;
			SWAP(&a[i], &a[j]);
		}
	}
	SWAP(&a[i + 1], &a[r]);
	return i + 1;
}
void quicksort(int a[], int p, int r){
	if(p < r){
		int q = partition(a, p ,r);
		quicksort(a, p, q - 1);
		quicksort(a, q + 1, r);
	}
}
int main(){
	quicksort(a, 0, 6);
	int median = a[3];
	printf("%d" , median);
	return 0;
}
