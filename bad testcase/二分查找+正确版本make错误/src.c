#include"stdio.h"
int N;
int HI;
int LO;
int ARRAY[7];
int result;
int bsearch(int a[], int n, int lo, int hi)
{
  int mid;
  int retval;

  if (hi < lo)
    retval = 0;
  else {
    mid = (hi + lo) / 2;

    if (a[mid] == n)
      retval = 1;
    else if (a[mid] > n)
      retval = bsearch(a, n, lo, mid - 1);
    else
      retval = bsearch(a, n, mid + 1, hi);
  }

  return retval;
}
int main(void)
{
  scanf("%d",&N);
  scanf("%d",&HI);
  scanf("%d",&LO);
  for(int i=0;i<7;++i) scanf("%d",&ARRAY[i]);
  result = bsearch(ARRAY, N, LO, HI);
  printf ("%d\n", result);
  return 0;
}
