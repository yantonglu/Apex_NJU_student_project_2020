int GCD(int x, int y){

if(y == 0)
	return x;
else
	{
	int res = GCD(y, x%y);
	return res + x;
	}
}
int main(){

int a = 24, b = 5;
int ans = GCD(a, b);
printf("%d", ans);

}
