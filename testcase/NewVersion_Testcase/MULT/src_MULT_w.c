int mult(int x, int y){
	if(y == 0)
		return 0;
	else
		{
			int tmp = mult(2*x, y/2);
			y = y%x;
			return tmp + x*y;
	
		}


}
int main(){
 int x = 5, y = 6;
 int ans = mult(x, y);
 printf("%d", ans);
 return 0;

}
