#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef CA_H
#include "CA.h"
#endif

int add(int a, int b) { return a + b; }
int mult(int a, int b) { return a * b; }

int (*getMath(size_t id))(int, int) {
	const int(*math[])(int, int) = 
	{ 
		add, 
		mult 
	};
	return math[id];
}
	

int main() {
	printf("1 + 2 * 4 = %i", getMath(0)(1, getMath(1)(2,4)));
	return 0;
}
