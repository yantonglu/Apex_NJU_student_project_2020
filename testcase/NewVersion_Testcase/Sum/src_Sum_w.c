int a[6] = {10,20,30,40,50,60};
int Sum(int a[], int i){
if(i == 0)
	return 0;
return Sum(a, i - 1) + a[i];

}

int main(){
 int ans = Sum(a, 5);
 printf("%d", ans);
 return 0;

}
