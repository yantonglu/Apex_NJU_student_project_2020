int Euclid(int x, int y){

if(y == 0)
	return x;
else
	{
		int res = Euclid(y, x%y);
		return res;
	}
}
int main(){

int x = 24, y = 5;
int res = Euclid(x, y);
printf("%d", res);

}
