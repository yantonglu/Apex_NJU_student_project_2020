int vec_a[6] = {1,2,3,4,5,6};
int vec_b[6] = {-1,3,6,5,8,10};

int vec_mul(int a[],int b[],int l)
{
    int ret = 0;
    int i = l - 1;
    for(; i > 0; i--)
    {
        int t = a[i] * b[i];
        ret += t;
    }
        
    return ret;
}
int main()
{
    
    int r = vec_mul(vec_a,vec_b,6);
    printf("%d",r);
    return 0;
}