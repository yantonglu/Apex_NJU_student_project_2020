int a[6] = {10,20,-30,50,60,-40};
int dp[7];
int max(int a, int b)
{
	if(a > b)
		return a;
	else 
		return b;

}
int max_subarray(int a[], int n){
int sum = 0;
for(int i = 1; i < n; i++){
	if(a[i - 1] < dp[i - 1] + a[i - 1])
		dp[i] = dp[i - 1] + a[i - 1];
	else
		dp[i] = a[i - 1];
	if(sum < dp[i])
		sum = dp[i];

}
return sum;


}

int main(){
int ans = max_subarray(a, 6);
printf("%d", ans);
return 0;
} 
