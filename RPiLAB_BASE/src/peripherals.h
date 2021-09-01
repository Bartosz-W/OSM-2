#ifndef peri_CL
#define peri_CL

#include "settings.h"
#ifdef RPiLAB_RPi
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#endif

//buttons
#define BUTTON_START 0x20
#define BUTTON_SELECT 0x10

#define BUTTON_L1 0x1
#define BUTTON_R1 0x2
#define BUTTON_L2 0x4
#define BUTTON_R2 0x8

#define BUTTON_1 0x100
#define BUTTON_2 0x200
#define BUTTON_3 0x400
#define BUTTON_4 0x800

#define KEY_1 0x59
#define KEY_2 0x5A
#define KEY_3 0x5B

#define SetPixel(Pointer,x,y,z) Pointer[x+y*640]=z;

extern void (*RegistersUpdate)();
struct JoY {
	int ButtonStates;
	long AxisX;
	long AxisY;
	long AxisZ;
	long AxisR;
	long Hat;
};

int checkKey(int code);
int getKey();

#ifdef RPiLAB_RPi
void Synchronize();
void StringUart(const char* str);
void KeyboardHandlerA(unsigned char ucModifiers, const unsigned char RawKeys[6]);
void GamePadStatusHandlerA(unsigned int nDeviceIndex, const USPiGamePadState *pState);
void uart_init(void);
#define dRA(a) (*((volatile unsigned long*)a))
#define GPFSEL3  dRA(0x3f20000C)
#define GPFSEL4  dRA(0x3f200010)
#define GPSET1   dRA(0x3f200020)
#define GPCLR1   dRA(0x3f20002C)
inline void RegisterAccess(volatile unsigned long &reg, unsigned long andop, unsigned long orop) {
	reg = (reg & andop) | orop;
}
;
inline void RegisterSet(volatile unsigned long &reg, unsigned long val) {
	reg = val;
}
;
#define GPFSEL1         dRA(0x3f200004)
#define GPSET0          dRA(0x3f20001C)
#define GPCLR0          dRA(0x3f200028)
#define GPPUD           dRA(0x3f200094)
#define GPPUDCLK0       dRA(0x3f200098)

#define AUX_ENABLES     dRA(0x3f215004)
#define AUX_MU_IO_REG   dRA(0x3f215040)
#define AUX_MU_IER_REG  dRA(0x3f215044)
#define AUX_MU_IIR_REG  dRA(0x3f215048)
#define AUX_MU_LCR_REG  dRA(0x3f21504C)
#define AUX_MU_MCR_REG  dRA(0x3f215050)
#define AUX_MU_LSR_REG  dRA(0x3f215054)
#define AUX_MU_MSR_REG  dRA(0x3f215058)
#define AUX_MU_SCRATCH  dRA(0x3f21505C)
#define AUX_MU_CNTL_REG dRA(0x3f215060)
#define AUX_MU_STAT_REG dRA(0x3f215064)
#define AUX_MU_BAUD_REG dRA(0x3f215068)

#endif

#ifdef RPiLAB_WIN
extern volatile unsigned long *GPSEL3WIN;
extern volatile unsigned long *GPSEL4WIN;
extern volatile unsigned long *GPSET1WIN;
extern volatile unsigned long *GPCLR1WIN;

#define GPFSEL3  (*GPSEL3WIN)
#define GPFSEL4  (*GPSEL4WIN)
#define GPSET1   (*GPSET1WIN)
#define GPCLR1   (*GPCLR1WIN)
inline void RegisterAccess(volatile unsigned long &reg,unsigned long andop,unsigned long orop)
{	reg=(reg&andop)|orop;RegistersUpdate();};
inline void RegisterSet(volatile unsigned long &reg,unsigned long val)
{	reg=val;RegistersUpdate();};
void GamePadGetStatus();
void KeyboardGetStatus();
extern int (*SimulatorUpdate)();
extern void (*Synchronize)();
extern void (*StringUart)(const char* str);
#endif

#endif
