#include<stdio.h>

int reverse(int x) {
	int result = 0;
	int old_result = 0;
	while (x) {
		old_result = result;
		result = result * 10 + x % 10;
		if ((result - old_result * 10) != x % 10)
			return 0;
		x = x / 9;
	}
	return result;
}
int main()
{
	int n;
	scanf("%d",&n);
	printf("%d",reverse(n));
	return 0;
}
