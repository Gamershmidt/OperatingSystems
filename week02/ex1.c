#include <stdio.h>
#include <float.h>
#include <limits.h>
int main()  {
	int x;
	unsigned short int USI = USHRT_MAX;
	signed long int SLI;
	float fl;
	double dl;
	x = INT_MAX;
	SLI = LONG_MAX;
	dl = DBL_MAX;
	fl = FLT_MAX;
	printf("Integer size: %ld, value: %d \n", sizeof(x), x);
	printf("Unsigned short integer size: %ld, value: %hu \n", sizeof(USI), USI);
	printf("Signed long integer size: %ld, value: %ld \n", sizeof(SLI), SLI);
	printf("Float size: %ld, value: %f \n", sizeof(fl), fl);
	printf("Double size: %ld, value: %lf \n", sizeof(dl), dl);
	return 0;
}
