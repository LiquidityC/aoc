#include <stdio.h>

int main(void)
{
	int number = 500;
	for (int i = 0; i < 10; i++) {
		number *= number;
		printf("%u\n", number);
	}
	return 0;
}
