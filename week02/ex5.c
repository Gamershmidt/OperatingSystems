#include <stdio.h>
int tribonacci(int n);
int main () {
	printf("Answer for 4: %d\n", tribonacci(4));
	printf("Answer for 36: %d\n", tribonacci(36));
	return 0;
}
int tribonacci(int n) {
	int t0 = 0, t1 = 1, t2 = 1, tans = 0;
	for(int i = 0; i < n - 2; i++) {
		tans = t0 + t1 + t2;
		t0 = t1;
		t1 = t2;
		t2 = tans;
	}
	return tans;
}
