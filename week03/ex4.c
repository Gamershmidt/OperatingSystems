#include <stdio.h>
#include<stdlib.h>
#include <float.h>
#include <limits.h>

void* additionInt(const void* first, const void*second)  {
	int * temp = malloc(sizeof(int)); 
	*temp = *(int *) first + *(int *) second;
	return temp;
}
void* additionDouble(const void* first, const void*second)  {
	double * temp= malloc(sizeof(double));
	*temp  = *(double*) first + *(double*) second;
	return temp;
}
void* multipicaionInt(const void* first, const void*second)  {
	int * temp = malloc(sizeof(int));
	*temp = *(int *) first * *(int *) second;
	return temp;
}
void* multipicaionDouble(const void* first, const void* second)  {
	double * temp= malloc(sizeof(double));
	*temp  = (*(double*)first) * (*(double*)second);
	return temp;
}
void* maxInt(const void* first, const void*second)  {
	int temp1=  *(int *)first > *(int *)second? *(int *)first : *(int *)second;
	int * temp = malloc(sizeof(int));
	*temp  = temp1;
	return temp;
}
void* maxDouble(const void* first, const void*second)  {
	double temp1= *(double *)first > *(double *)second? *(double *)first : *(double *)second;
	double * temp= malloc(sizeof(double));
	*temp = temp1;
	return temp;
}
void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
	void* temp = initial_value;
	for(int i = 0; i<n; i++) {
		temp = opr(base + i*size, temp);
	}
	return temp;	
}
int main() {
	int int_arr[5] = {1,2,3,6,5};
	double dbl_arr[5] = {1.5, 2.5, 3.5, 6.5, 5.5};
	double double_min = DBL_MIN;
	int int_min = INT_MIN;
	int init_sum = 0;
	double base_sum = 0, base_mul = 1;
	int init_mul = 1;
	int* tmp = aggregate(int_arr, sizeof(int), 5, &init_sum, &additionInt);
	printf("Sum of int: %d\n", *tmp);
	tmp = aggregate(int_arr, sizeof(int), 5, &init_mul, &multipicaionInt);
	printf("Mul of int:%d\n", *tmp);
	tmp = aggregate(int_arr, sizeof(int), 5, &int_min, &maxInt);
	printf("Max of int:%d\n", *tmp);
	double* temp = aggregate(dbl_arr, sizeof(double), 5, &base_sum, &additionDouble);
	printf("Sum of double:%lf\n", *temp);
	temp = aggregate(dbl_arr, sizeof(double), 5, &base_mul, &multipicaionDouble);
	printf("Mul of double:%lf\n", *temp);
	temp = aggregate(dbl_arr, sizeof(double), 5, &double_min , &maxDouble);
	printf("Max of double:%lf\n", *temp);
	return 0;
}
