#include "peripherals.h"

struct JoY JoYAct;

unsigned char gucModifiers = 0;
unsigned char gRawKeys[6] = { 0, 0, 0, 0, 0, 0 };

int checkKey(int code) {
	int a = 0;
	while (a < 6)
		if (gRawKeys[a++] == code)
			return 1;
	return 0;
}
int getKey() {
	int a = 0;
	while (a < 6)
		if (gRawKeys[a++] != 0)
			return gRawKeys[--a];
	return 0;
}

#ifdef RPiLAB_RPi

volatile int ra;
void uart_init(void) {

	AUX_ENABLES = 1;
	AUX_MU_IER_REG = 0;
	AUX_MU_CNTL_REG = 0;
	AUX_MU_LCR_REG = 3;
	AUX_MU_MCR_REG = 0;
	AUX_MU_IER_REG = 0;
	AUX_MU_IIR_REG = 0xC6;
	AUX_MU_BAUD_REG = 66;
	GPFSEL1 = (GPFSEL1 & (~(7 << 12))) | (2 << 12);
	GPFSEL1 = (GPFSEL1 & (~(7 << 15))) | (2 << 15);
	GPPUD = 0;
	for (ra = 0; ra < 150; ra++)
		ra++;
	GPPUDCLK0 = (1 << 14) | (1 << 15);
	for (ra = 0; ra < 150; ra++)
		ra++;
	GPPUDCLK0 = 0;
	AUX_MU_CNTL_REG = 3;
}

void KeyboardHandlerA(unsigned char ucModifiers, const unsigned char RawKeys[6]) {
	gucModifiers = ucModifiers;
	for (int a = 0; a < 6; a++)
		gRawKeys[a] = RawKeys[a];
}

void GamePadStatusHandlerA(unsigned int nDeviceIndex, const USPiGamePadState *pState) {
	JoYAct.AxisX = pState->axes[0].value - pState->axes[0].minimum - 128;
	JoYAct.AxisY = pState->axes[1].value - pState->axes[1].minimum - 128;
	JoYAct.AxisZ = pState->axes[2].value - pState->axes[2].minimum - 128;
	JoYAct.AxisR = pState->axes[3].value - pState->axes[3].minimum - 128;
	JoYAct.ButtonStates = pState->buttons;
}

void StringUart(const char *str) {
	int dane = 0;
	while (1) {
		char s = str[dane++];
		if (s == 0)
			break;
		while (1) {
			if (AUX_MU_LSR_REG & 0x20)
				break;
		}
		AUX_MU_IO_REG = s;

	}

}
#endif
