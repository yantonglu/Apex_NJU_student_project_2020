#include<stdio.h> 

int main() {
  int W,H,N;
  int x[100],y[100],a[100];
  int XU,XL=0,YU,YL=0,X,Y;

  scanf("%d %d %d",&W,&H,&N);
  XU=W; YU=H;
  for(int i=0;i<N;i++)
    scanf("%d %d %d",&x[i],&y[i],&a[i]);
    for(int i=0;i<N;i++){
    switch(a[i]){
      case 1:
        if(x[i]<XL) continue;
	XL=x[i];
	break;
      case 2:
	if(x[i]>XU) continue;
 	XU=x[i];
	break;
      case 3:
 	if(y[i]<YL) continue;
 	YL=y[i];
	break;
      case 4:
 	if(y[i]>YU) continue;
	YU=y[i];
	break;
      default:
	continue;
  }
  }
  X=XU-XL;  Y=YU-YL;
  if(X<0) X=0;
  if(Y<0) Y=0;
  printf("%d",X*Y);
   return 0;
 }

