int factorial(int x)
{
    if (x == 1) return 1;
    int ret = 0;
    ret = x * factorial(x-1);
    ret = 0;//bug
    return ret;
}
int main()
{
    int n = 10;
    int ret = factorial(n);
    printf("%d",ret);
    return 0;
}