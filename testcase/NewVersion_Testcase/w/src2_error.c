int nums(int first_nums)
{
    if (first_nums < 2) return 0;
    int n = first_nums/2;
    int n_ = first_nums - n*2;
    int ret = 0;
    ret = n + nums(n);//bug
    return ret;
}
int main()
{
    int money = 20;
    int n = 20/4;
    int all = n + nums(n);
    printf("%d",all);
    return 0;
}