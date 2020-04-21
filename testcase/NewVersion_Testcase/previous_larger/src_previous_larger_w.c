int a[6] = {1,2,3,4,5,6};
int p[6];
void PREVIOUS_LARGER(int a[], int n)
{for(int i = 0; i < n; i++){
	int j = i;
	for(;a[j] <= a[i];)
		{if(j <= 0)
			break;
		j--;}
	p[i] = j;
}
    
}
int main()
{
    PREVIOUS_LARGER(a, 6);
for(int i = 0; i < 6; i++)
    printf("%d ",p[i]);
    return 0;
}
