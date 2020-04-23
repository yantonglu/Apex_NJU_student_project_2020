int main()
{
    int x = 29;
    int i = 1;
    while (i < x)
    {
        if(x%i==0)   break;  
        i++;
    }
    if(i==x)        
        printf("%d是素数\n",x);        
    else        
        printf("%d不是素数\n",x);
    return 0;
}