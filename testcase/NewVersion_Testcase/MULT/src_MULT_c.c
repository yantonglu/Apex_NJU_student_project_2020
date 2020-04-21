int INT_MULT(int y, int z){
	if(z == 0)
		return 0;
	else
		{
			int temp = INT_MULT(2*y, z/2);
			int z_mod2 = z%2;
			return temp + z_mod2*y;
	
		}


}
int main(){
 int x = 5, y = 6;
 int ans = INT_MULT(x, y);
 printf("%d", ans);
 return 0;

}
