#include <stdio.h>
void draw(int n, int s) {
	switch( n ) {
		case 1:
			for(int i = 1; i <= s; i++) {
				for(int j = 0; j < i; j++) {
					printf("*");
				}
				printf("\n");
			}
			break;
		case 2:
			for(int i = 1; i <= (s + 1)/2; i++) {
				for(int j = 0; j < i; j++) {
					printf("*");
				}
				printf("\n");
			}
			for(int i = (s + 1)/2; i > 0; i--) {
				for(int j = i - 1; j > 0; j--) {
					printf("*");
				}
				printf("\n");
			}
			break;	
		case 3:
			for(int i = 1; i <= s; i++) {
				for(int j = 0; j < 10; j++) {
					printf("*");
				}
				printf("\n");
			}
			break;
		default:
			printf("error");
	}
}
int main() {
	printf("Press 1 for triangle, 2 for equilateral triangle, 3 for square\n");
	int n = 0;
	scanf("%d", &n);
	printf("Enter size of shape\n");
	int s;
	scanf("%d", &s);
	draw(n, s);
	return 0;
}
