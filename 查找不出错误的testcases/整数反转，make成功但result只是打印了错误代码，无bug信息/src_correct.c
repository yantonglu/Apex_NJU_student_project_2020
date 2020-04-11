#include<stdio.h>

int reverse(int x) {
	// Note: The Solution object is instantiated only once and is reused by each test case.
	int result = 0;

	int flag = 0;
	if (x < 0)
	{
		flag = 1;
		x = -x;
	}

	int lastDigit = 0;
	while (x > 0)
	{
		lastDigit = x - x / 10 * 10;
		result = result * 10 + lastDigit;
		x /= 10;
	}

	if (flag == 1)
	{
		result = -result;
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
