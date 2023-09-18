#include <stdio.h>
#include <string.h>
int main () {
	char str[257];
	printf("Enter a string: ");
	fgets(str, 256, stdin);
	char reverse[256];
	for(int i = 0; i < strlen(str); i++) {
		if(str[i] != '.') {
			reverse[i] = str[i];
		} else break;
	}
	for(int i = 0; i < strlen(reverse)/2; i++) {
		char temp = reverse[i];
		reverse[i] = reverse[strlen(reverse) - i - 1];
		reverse[strlen(reverse) - i - 1] = temp;
	}	
	printf("%s", reverse);
	return 0;
}
