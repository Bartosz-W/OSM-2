#include "utilities.h"

#ifdef RPiLAB_RPi
#ifdef __cplusplus
extern "C" {

void __start() {
	_start();
}
void abort() {
	while (1)		;
}
unsigned getNumberLenght(double x) {
	if (x >= 1000000000)
		return 10;
	if (x >= 100000000)
		return 9;
	if (x >= 10000000)
		return 8;
	if (x >= 1000000)
		return 7;
	if (x >= 100000)
		return 6;
	if (x >= 10000)
		return 5;
	if (x >= 1000)
		return 4;
	if (x >= 100)
		return 3;
	if (x >= 10)
		return 2;
	return 1;
}
int power(int base, int exp) {
	int result = 1;
	while (exp) {
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}
}
#endif
#endif

unsigned long randi = 334324324;
unsigned long randr(unsigned long range) {
	randi = randi * 1103515245UL + 12345;
	return randi % range;
}

