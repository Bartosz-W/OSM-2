#include "main.h"
#include "moje_klasy.h"


#define WIN_PLOT
#define NazwaPlikuDanych "Data/RPiLABoutputData.csv"
#define CSV_NAGLOWEK "Wsp. x,Wsp. y1,Wsp. y2\n"
#define CSV_DANE "%li,%i,%i\n",presc, y[0], y[1]





#ifdef RPiLAB_WIN
#include "stdio.h"
char credits[51]="        RPiLAB                DEMO                ";
#endif
long TimerIsrPeriod = 1;            //czas podany w milisekundach

#define MaxObj 15000
int dx[MaxObj];
int dy[MaxObj];
int s[MaxObj];
int x[MaxObj];
int y[MaxObj];
int xt[MaxObj];
int yt[MaxObj];

unsigned long* SCREEN;

#ifdef RPiLAB_WIN
unsigned long SCREENBUF[640 * 480];
#ifdef WIN_PLOT
  FILE* outputCSV=0;
#endif
#elif defined RPiLAB_RPi
unsigned long* SCREENBUF=(unsigned long*)0x7000000;
#endif
int mode = 0;
int stage = 0;
int frame_count = 0;
int Jxt = 0, Jyt = 0, Jx = 0, Jy = 0, Jz = 0, JRz = 0;

////////// bitmapy START //////////
#include "bitmapaZad0.h"
#include "bitmapaZad30.h"
#include "bitmapaZad60.h"
#include "grzejnik_old_color.h"
#include "grzejnik_color.h"
#include "termometr_color.h"
#include "wiatrak3.h"
#include "wiatrak2.h"
#include "wiatrak1.h"
////////// bitmapy END //////////

////////// inicjalizacja zmiennych START //////////
int Tim = 0;                // Licznik uzytkownika
unsigned int Tim1 = 0;
unsigned int Tim2 = 0;
unsigned int Tim3 = 0;
unsigned int predkosc = 0;
unsigned int preScale = 0;  // Preskaler licznika uzytkownika
unsigned int Skaler = 0;	// Skaler potrzebny do przeliczania konkretnych Timow
bool st_zaklo = false;	// zmienna start/stop zaklocenia
volatile char EnableRefresh = 0;    //Zezwolenie na odswiezenie zawartosci pamieci graficznej

float wart_zad = 30;
float wyj_ukl = 50;
float uchyb = 0;
float syg_ster = 0;
float zaklo = 10;
unsigned char Key = 0;

unsigned char Bufor1[] = "Temperatura";
unsigned char Bufor2[] = "-----------";
char Bufor3[5] ;
unsigned char Bufor4[] = "Wartosc zadana:";
unsigned char Bufor5[] = "--------------";
char Bufor6[5];
////////// inicjalizacjz zmiennych END //////////

////////// inicjalizacja obiektow START //////////
PID Regul(0.8, 0.5, 1, -50, 50);
CalkModel Model;
////////// inicjalizacja obiektow END //////////


int main(void) {

	if (SystemInit())		return EXIT_HALT;
	//DataPrepare();


	while (1) {
		if (UpdateIO())	return 0;
		PrintDiagnosticInfo();
		Synchronize();
		ClearScreen();
		if (JoYAct.ButtonStates & BUTTON_SELECT) continue;
		//DrawObjects(); //rysowanie kwadratow mysle

		Key = getKey();

		//WYWOLANIE FUNKCJI WYSWIETLAJACYCH BITMAPY


		rysowanie_ekran(grzejnik_color,300,300,WIDTH_grzejnik_color,HEIGHT_grzejnik_color);
		rysowanie_ekran(termometr_color,550,50,WIDTH_termometr_color,HEIGHT_termometr_color);
		rysowanie_ekran(wiatrak1,50,150,WIDTH_wiatrak1,HEIGHT_wiatrak1);
		SetPixel(SCREENBUF,585,245,0xFF0000);
		SetPixel(SCREENBUF,586,245,0xFF0000);
		SetPixel(SCREENBUF,587,245,0xFF0000);
		SetPixel(SCREENBUF,588,245,0xFF0000);
		SetPixel(SCREENBUF,589,245,0xFF0000);
		SetPixel(SCREENBUF,560,245,0xFF0000);

		SetPixel(SCREENBUF,585,269,0xFF0000);
		SetPixel(SCREENBUF,586,269,0xFF0000);
		SetPixel(SCREENBUF,587,269,0xFF0000);
		SetPixel(SCREENBUF,588,269,0xFF0000);
		SetPixel(SCREENBUF,589,269,0xFF0000);
		SetPixel(SCREENBUF,560,269,0xFF0000);

		SetPixel(SCREENBUF,585,370,0xFF0000);
		SetPixel(SCREENBUF,586,370,0xFF0000);
		SetPixel(SCREENBUF,587,370,0xFF0000);
		SetPixel(SCREENBUF,588,370,0xFF0000);
		SetPixel(SCREENBUF,589,370,0xFF0000);
		SetPixel(SCREENBUF,560,370,0xFF0000);

	}
}

volatile long presc = 0;

void TimerIsr() {
	presc++;
	if ((presc / 250) % 2)
		RegisterSet(GPSET1, 1 << (35 - 32));
	else
		RegisterSet(GPCLR1, 1 << (35 - 32));
	if ((presc / 500) % 2)
		RegisterSet(GPSET1, 1 << (47 - 32));
	else
		RegisterSet(GPCLR1, 1 << (47 - 32));

	if(++preScale==500)
	        {
	        	preScale=0;Tim++;
	//mamy st_zaklo i Skaler
	        	if (Key == 92) zaklo++;
	        	else if (Key == 93) zaklo--;
	        	else if (Key == 95) zaklo=zaklo+3;
	        	else if (Key == 96) zaklo=zaklo-3;

	        	if (Key == 87) wart_zad++;
	        	else if (Key == 86) wart_zad--;

	        	uchyb = wart_zad - wyj_ukl;
	        	uchyb = -uchyb;
	        	Regul.setInput(uchyb);
	        	Regul.Calculate();
	        	syg_ster = Regul.getOutput();
	        	Model.setInput(syg_ster, zaklo);
	        	Model.Calculate();
	        	wyj_ukl = Model.getOutput();
	        }
	        if (preScale % 80 == 0)
	        {
	        	Tim1++;
	        	if (Tim1 % 2 == 0) Tim2++;
	        	else if (Tim2 % 2 == 0) Tim3++;
	        }
}

//Odpowiada za rysowanie kwadratow
void DrawObjects() {
	unsigned long* GRAPH = SCREENBUF;
	Jx = (Jx * 93 + 7 * JoYAct.AxisX) / 100;
	Jy = (Jy * 93 + 7 * JoYAct.AxisY) / 100;
	Jz = (Jz * 93 + 7 * ((JoYAct.ButtonStates & BUTTON_2 ? -128 : 0) + (JoYAct.ButtonStates & BUTTON_4 ? 127 : 0))) / 100;
	JRz = (JRz * 93 + 7 * ((JoYAct.ButtonStates & BUTTON_1 ? -128 : 0) + (JoYAct.ButtonStates & BUTTON_3 ? 127 : 0))) / 100;

	for (int c = 0; c < MaxObj; c++) {
		int srt = abs(dy[c]) + abs(dx[c]);
		for (int g = 0; g < srt / 10 + 1; g++) {
			int ddf = 20;
			ddf += -(JRz) / (srt + 1);
			if (ddf < 8)
				ddf = 8;
			y[c] += (ddf * dy[c]) / 30 + (Jyt - Jy) * srt / 20;
			x[c] += (ddf * dx[c]) / 30 + (Jxt - Jx) * srt / 20;
			y[c] -= ((xt[c] - x[c]) * Jz) / 200;
			x[c] += ((yt[c] - y[c]) * Jz) / 200;
			xt[c] = x[c];
			yt[c] = y[c];
			if (x[c] < 0 || y[c] < 0 || x[c] > 639 * 16 - s[c] || y[c] > 479 * 16 - s[c]) {
				do {
					s[c] = randr(0x1f) + 1;
					xt[c] = x[c] = randr(519 * 16 - s[c]) + 60 * 16;
					yt[c] = y[c] = randr(379 * 16 - s[c]) + 50 * 16;
					int spd = randr(15) + 7;
					dx[c] = (x[c] - 320 * 16 - 16 * Jx - (Jx - Jxt) / 2) * spd;
					dy[c] = (y[c] - 240 * 16 - 16 * Jy - (Jy - Jyt) / 2) * spd;

					while (abs(dx[c]) > (-JRz / 5 + 40) || abs(dy[c]) > (-JRz / 5 + 35)) {
						dx[c] /= 3;
						dy[c] /= 3;
					}
				} while ((abs(dy[c]) + abs(dx[c])) <= 8);
			}
			int size = s[c];

			int col = (y[c] * 255 / (480 << 4)) + 0xf0000 * size + (0x100 * ((x[c] * 255 / (640 << 4))));

			for (int b = y[c] >> 4; b < (y[c] + size) >> 4; b++)
				for (int a = x[c] >> 4; a < (x[c] + size) >> 4; a++)
					SetPixel(GRAPH, a, b, col);
		}
	}
	Jxt = Jx;
	Jyt = Jy;
//////////////////////////////////////////////////////////////////////////////////////////
}
void ClearScreen() {
	unsigned long bckgcol = 0;
	if (JoYAct.ButtonStates & BUTTON_L1)
		bckgcol = 0x3f0000;
	if (JoYAct.ButtonStates & BUTTON_L2)
		bckgcol = 0x3f00;
	if (JoYAct.ButtonStates & BUTTON_R1)
		bckgcol = 0x3f;
	if (JoYAct.ButtonStates & BUTTON_R2)
		bckgcol = 0x3f3f3f;
	int stage_max = getKey() - KEY_1 + 1;

	if (stage_max < 2)
		stage_max = 2;
	if (stage_max > 9)
		stage_max = 9;

	if (JoYAct.ButtonStates & BUTTON_SELECT)
		stage_max = 24;
	unsigned long* GRAPH = SCREENBUF;
	stage++;
	for (int a = stage % stage_max; a < (640 * 480); a += stage_max) {
		SCREEN[a] = GRAPH[a];
		GRAPH[a] = ((GRAPH[a] & 0xfefefe) >> 1) | bckgcol;
	}
}
void PrintDiagnosticInfo() {
	frame_count++;
#ifdef RPiLAB_RPi
	TString strbuf;
	String(&strbuf);

	StringFormat(&strbuf, "%d: %d, %d, %d, %x\r\n",frame_count, getKey(), Jx, Jy, JoYAct.ButtonStates);
	if ((frame_count % 20) == 0)
		StringUart(StringGet(&strbuf));
	_String(&strbuf);

	#endif

#ifdef RPiLAB_WIN
	char strbuf[1000];
	sprintf(strbuf,"%i: %i, %i, %i, %x\r\n",frame_count,getKey(), Jx, Jy,JoYAct.ButtonStates);
	if((frame_count%20)==0) StringUart(strbuf);
#endif
}
void DataPrepare() {
	for (unsigned char a = 0; a < sizeof(struct JoY); a++)
		((char*) &JoYAct)[a] = 0;

	for (int a = 0; a < MaxObj; a++) {
		do {
			s[a] = randr(0x1f) + 1;
			xt[a] = x[a] = randr(519 * 16 - s[a]) + 60 * 16;
			yt[a] = y[a] = randr(379 * 16 - s[a]) + 50 * 16;
			int spd = randr(15) + 7;
			dx[a] = (x[a] - 320 * 16) * spd;
			dy[a] = (y[a] - 240 * 16) * spd;

			while (abs(dx[a]) > (40) || abs(dy[a]) > (35)) {
				dx[a] /= 3;
				dy[a] /= 3;
			}
		} while ((abs(dy[a]) + abs(dx[a])) <= 2);
	}
}
#ifdef RPiLAB_RPi

void TimerHandler(unsigned hTimer, void *pParam, void *pContext) {
	TimerIsr();
	StartKernelTimer(TimerIsrPeriod, TimerHandler, (void *) 0, (void *) 0);
}

unsigned long timeElapse = 0;


void Synchronize() {
	while (*((volatile unsigned long*) ARM_SYSTIMER_CLO) - timeElapse < 20 * CLOCKHZ / 1000)
		;
	timeElapse = *((volatile unsigned long*) ARM_SYSTIMER_CLO);
}

int SystemInit() {

	RegisterAccess(GPFSEL4, ~(7 << 21), 1 << 21); // Warto???ci ustawiaj???ce tryby dzia???ania portow IO nale???y ustalac z duza rozwaga !!!
	RegisterAccess(GPFSEL3, ~(7 << 15), 1 << 15); // Warto???ci ustawiaj???ce tryby dzia???ania portow IO nale???y ustalac z duza rozwaga !!!

	if (!USPiEnvInitialize())
		return -1;
	ScreenDeviceClearDisplay(USPiEnvGetScreen());
	if (!USPiInitialize()) {
		LogWrite("USPi Error", LOG_ERROR, "Cannot initialize USPi");
		USPiEnvClose();
		return -1;
	}

	StartKernelTimer(TimerIsrPeriod, TimerHandler, (void *) 0, (void *) 0);

	USPiKeyboardRegisterKeyStatusHandlerRaw(KeyboardHandlerA);
	USPiGamePadRegisterStatusHandler(GamePadStatusHandlerA);
	uart_init();
	TScreenDevice* screen = USPiEnvGetScreen();
	SCREEN = (unsigned long*) screen->m_pBuffer;
	timeElapse = *((volatile unsigned long*) ARM_SYSTIMER_CLO);

	return 0;
}

int UpdateIO() {
	USPiGamePadGetStatus(0);
	return 0;
}

#endif

#ifdef RPiLAB_WIN

int SystemInit()
{
	RegisterAccess(GPFSEL4,~(7<<21),1<<21);
	RegisterAccess(GPFSEL3,~(7<<15),1<<15);
#ifdef WIN_PLOT
  outputCSV=fopen(NazwaPlikuDanych,"w+" );
  fprintf(outputCSV,CSV_NAGLOWEK);
#endif

	return 0;
}
int UpdateIO()
{

#ifdef WIN_PLOT
	// Zapis danych do pliku
	fprintf(outputCSV,CSV_DANE);
	printf("time %li syg_ster %f wyj_ukl %f zaklo %f getkey %d\n",presc,syg_ster,wyj_ukl,zaklo,Key);
	fflush(outputCSV);
	fflush(stdout);
#endif

	GamePadGetStatus();
	KeyboardGetStatus();
	return SimulatorUpdate();
}
#endif

void rysowanie_ekran(uint32_t* tab, int posX, int posY, int length, int height)
{
	int dy=0;
	int dx=0;
	for(dy=0;dy<height;dy++)
	{
		for(dx=0;dx<length;dx++)
		{
			if (tab[dy * length + dx] != 0xFFFFFF)
			{
				SetPixel(SCREENBUF,(dx + posX),(dy + posY),tab[dy * length + dx]);
			}
		}
	}
}

void rysuj_temper(void)
{

}













