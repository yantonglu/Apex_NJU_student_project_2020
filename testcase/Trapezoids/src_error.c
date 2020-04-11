#include <stdio.h>
int main() {    long a, b, h;    int area;
    scanf( "%ld\n%ld\n%ld", &a, &b, &h );
    area = a + b * h / 2;    printf( "%d\n", area );
    return 0; }

