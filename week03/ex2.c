#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Point {
	double x;
	double y;
};

double distance(struct Point a, struct Point b) {
	return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}

double area(struct Point a, struct Point b, struct Point c) { 
	return fabs(a.x*b.y - b.x*a.y + b.x*c.y - c.x*b.y + c.x*a.y - a.x*c.y)/2;
}

int main () {
	struct Point A,B,C;
	A.x = 2.5;
	A.y = 6;
	B.x = 1;
	B.y = 2.2;
	C.x = 10;
	C.y = 6;
	printf("Distance: %lf\nArea: %lf", distance(A, B), area(A,B,C));
	return 0;
}
