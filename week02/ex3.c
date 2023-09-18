#include<stdio.h>
#include <math.h> 
#include <string.h> 
void convert(long long int value, int before, int after);
int main() {
	char number[256];
	long long int value;
	printf("Enter number: ");
	fgets(number, 256, stdin);
	sscanf(number, "%lli", &value);
	
	int t, s;
	printf("Enter numeral systems: ");
	scanf("%d %d", &s, &t);
	convert(value, s, t);
	return 0;
}
void convert(long long int value, int before, int after) {
	if(before >10 || before < 2 || after > 10 || after < 2) {
			printf("cannot convert");
			return;
	}
	long long int temp = value;
	while (temp > 0) {
		if(temp%10 >= before) {
			printf("cannot convert%lli", temp%10);
			return;
		}
		temp/=10;
	}
	
	long long int toDecimal = 0, power = 0;
	while(value > 0) {
		toDecimal =toDecimal + pow(before, power) * (value%10);
		power++;
		value/=10;
	}
	int len = log(toDecimal) + 3, i = 0;
	char ans[len];
	while(toDecimal > 0) {
		char tmp[len];
		strcpy(tmp, ans);
		sprintf(ans, "%s%lli", tmp, toDecimal%after);
		toDecimal/=after;
		i++;
	}
	for(int j = strlen(ans) - 1; j >= strlen(ans) - i; j--) {
		printf("%c", ans[j]);
	}
}
