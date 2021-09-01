#ifndef utils_CL
#define utils_CL

#include "settings.h"
#ifdef RPiLAB_RPi
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#endif

#ifdef __cplusplus
extern "C" {

void _start();
void __start() __attribute__((section (".begin")));
void abort();
unsigned getNumberLenght(double x);
int power(int base, int exp);
unsigned long randr(unsigned long range);

#define abs(a) (a>0?a:-a)
}
#endif

#endif
