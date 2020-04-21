int a[6] = {10,20,-30,50,60,-40};
int d[7];
int max(int a, int b)
{
	if(a > b)
		return a;
	return b;

}
int maxsub(int a[], int n){
int sum = 0;
for(int i = 1; i < n; i++){
	if(d[i - 1] < d[i - 1] + a[i - 1])
		d[i] = d[i - 1] + a[i - 1];
	else
		d[i] = d[i - 1];
	if(sum < d[i])
		sum = d[i];

}
return sum;


}

int main(){
int ans = maxsub(a, 6);
printf("%d", ans);
return 0;
} 
