#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include "settings.h"
#ifdef RPiLAB_RPi
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/synchronize.h>
#include <uspienv/memio.h>
#else
#define EXIT_HALT 0
#endif

#include "utilities.h"
#include "peripherals.h"
#include "stdint.h"

extern struct JoY JoYAct;
int UpdateIO();
int SystemInit();
void DataPrepare();
void ClearScreen();
void DrawObjects();
void PrintDiagnosticInfo();

void rysowanie_ekran(uint32_t* tab, int posX, int posY, int length, int height);
void rysuj_temper(void);

#endif /* SRC_MAIN_H_ */
