#include <stdio.h>
#include <string.h>
void count(char str[], char letter) {
	int calc = 0;
	for(int i = 0; i < strlen(str); i++) {
		if((int) str[i] == (int)letter
			|| (int) str[i]  - 32 == (int)letter
				|| (int) str[i]  + 32 == (int)letter) {
		 	calc+=1;
		}
	}
	printf("%c:%d", letter, calc);
}
void countAll(char str[]) {
	for(int i = 0; i < strlen(str) - 1; i++) {
		count(str, str[i]);
		printf(", ");
	}
	count(str, str[strlen(str) - 1]);
}
int main() {
	char str[256];
	fgets(str, 256, stdin);
	countAll(str);
	return 0;
}
