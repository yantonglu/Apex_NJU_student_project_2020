#include<stdio.h> 


int main() {
	char str[10] = {0};
	scanf("%s", &str);
	int i = 0;
	int result = 0;
	while (str[i] != '\0')
	{
		str[i] = str[i] - '\0';
		result *= 10;
		result += str[i];
		i++;
	}
	printf("%d", result);

	return 0;
}
