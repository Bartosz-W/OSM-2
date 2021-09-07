////////////////////////////////////////////////////////
///////      Podstawowa konfiguracja
////////////////////////////////////////////////////////

//#define CPP_EXAMPLE

#define BUFFERSYNC

#define WIN_PLOT
#define NazwaPlikuDanych "Data/TMSLABoutputData.csv"
#define CSV_NAGLOWEK "Wsp. x,Wsp. y1,Wsp. y2\n"
#define CSV_DANE "%i,%i,%i\n",Tim, y[0], y[1]

////////////////////////////////////////////////////////


#include "main.h"
#include "stdint.h"
#include "bitmapaZad0.h"
#include "bitmapaZad30.h"
#include "bitmapaZad60.h"
#include "moje_klasy.h"

#ifdef TMSLAB_WIN
  #include "stdio.h"
#endif

unsigned long *ekran; // Adres obszaru graficznego LCD [8*128*2]
#ifdef TMSLAB_C2000
unsigned char *textEkran; // Adres obszaru tekstowego [40*16/2]
#endif

#ifdef TMSLAB_WIN
unsigned short int *textEkran; // Adres obszaru tekstowego [40*16/2]
extern int (*PartialRefresh)();
char credits[43]="-               DEMO DISC                -";  // Tekst wyswietlany w naglowku symulatora
long Timer2IsrPeriod=1; // okres pracy symulowanego licznika Timer2 podany w przybli�eniu w ms
#ifdef WIN_PLOT
  FILE* outputCSV=0;
#endif
  #endif

int Tim = 0;                // Licznik uzytkownika
unsigned int Tim1 = 0;
unsigned int Tim2 = 0;
unsigned int Tim3 = 0;
unsigned int predkosc = 0;
unsigned int preScale = 0;  // Preskaler licznika uzytkownika
unsigned int Skaler = 0;	// Skaler potrzebny do przeliczania konkretnych Timow
bool st_zaklo = false;	// zmienna start/stop zaklocenia
volatile char EnableRefresh = 0;    //Zezwolenie na odswiezenie zawartosci pamieci graficznej


////////////////////// Zmienne
float wart_zad = 30;
float wyj_ukl = 50;
float uchyb = 0;
float syg_ster = 0;
float zaklo = 10;
unsigned char Key = 0;

PID Regul(0.8, 0.5, 1, -50, 50);
CalkModel Model(wyj_ukl);


//Tablice danych/obiektow graficznych
#define MaxObj 200
#ifdef CPP_EXAMPLE
square objects[MaxObj];
#else
int dx[MaxObj];
int dy[MaxObj];
int s[MaxObj];
int x[MaxObj];
int y[MaxObj];
#endif

unsigned char Bufor1[] = "Temperatura";
unsigned char Bufor2[] = "-----------";
char Bufor3[5] ;
unsigned char Bufor4[] = "Wartosc zadana:";
unsigned char Bufor5[] = "--------------";
char Bufor6[5];



    int main()

    {

#ifdef TMSLAB_C2000
        LCD.LCD_Init(ekran, textEkran);
#endif

#ifdef TMSLAB_WIN
        LCD.LCD_Init(&ekran,&textEkran);
#ifdef WIN_PLOT
  outputCSV=fopen(NazwaPlikuDanych,"w+" );
  fprintf(outputCSV,CSV_NAGLOWEK);
#endif
#endif





        //InitData();

        EnableInterrupts();

        while (1)
        {
            EnableRefresh = 1;

            EnableRefresh = 0;


           // Key = KEYBOARD.GetKey();




            y[0]=syg_ster;
            y[1]=wyj_ukl;

            PrintText(textEkran, Bufor1, 12, 19, 1);
            PrintText(textEkran, Bufor2, 12, 19, 2);
            PrintText(textEkran, Bufor3, 8, 19, 3);
            PrintText(textEkran, Bufor4, 14, 19, 5);
            PrintText(textEkran, Bufor5, 14, 19, 6);
            PrintText(textEkran, Bufor6, 14, 19, 7);
            ClearScreen();
          //  DrawPixels(Key);
#ifdef TMSLAB_WIN
            if(PartialRefresh()) return 0;
#ifdef WIN_PLOT
	// Zapis danych do pliku
	fprintf(outputCSV,CSV_DANE);
	printf("time %d syg_ster %f wyj_ukl %f zaklo %f\n",Tim,syg_ster,wyj_ukl,zaklo);
	sprintf(Bufor3, "%.1f", wyj_ukl);
	sprintf (Bufor6, "%.1f", wart_zad);
	fflush(outputCSV);
	fflush(stdout);
#endif
#endif

        }
    }

#ifdef TMSLAB_C2000

    interrupt
    void Timer2Isr()
    {

#ifdef BUFFERSYNC
        if (EnableRefresh)
        LCD.PartialRefresh();
#else
        LCD.PartialRefresh();
#endif

        KEYBOARD.PartialRefresh();

        if (++preScale == 50000)
        {
            preScale = 0;
            Tim++;
        }
    }

    unsigned long ADRFTECHED = 0;
    interrupt
    void NoIsr()
    {
        ADRFTECHED = PieCtrlRegs.PIECTRL.bit.PIEVECT;
        asm(" ESTOP0");
    }

    void EnableInterrupts()
    {

        EALLOW;
        //Ustawienie wektorow przerwan
        unsigned long VECTBEG = (unsigned long) &PieVectTable;
        unsigned long VECTLAST = (unsigned long) &PieVectTable
        + sizeof(PieVectTable);
        while (VECTBEG >= VECTLAST)
        *(unsigned long*) VECTBEG++ = (unsigned long) NoIsr;
        PieVectTable.TIMER2_INT = Timer2Isr;

        CpuTimer2Regs.TCR.bit.TIE = 1;
        CpuTimer2Regs.TCR.bit.TRB = 1;

        IER = IER_MASK;//Odblokuj przerwania
        asm(" push ier");
        asm(" pop dbgier");

        PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
        PieCtrlRegs.PIEACK.all = 0xffff;
        EDIS;
        EINT;
    }

    void SetUpPeripherials()
    {
        SetupCoreSystem();
        ekran = (unsigned long *) 0x8000;              //[8*128*2]
        textEkran = (unsigned char*) 0x8a00;//[40*16/2]
        EALLOW;
        //Okres licznika T2
        CpuTimer2Regs.PRD.all = System_Clk * Timer2ISR_Period;
        EDIS;
    }
    extern "C"
    {
    int _system_pre_init()
    {
        EALLOW;
        WdRegs.WDWCR.all = 0x68;
        EDIS;
        return (1);
    }
    }
#endif

//#ifdef TMSLAB_WIN
    void EnableInterrupts()
    {}
    void SetUpPeripherials()
    {}
    void Timer2Isr()
    {
        if(++preScale==500)
        {
        	preScale=0;Tim++;
//mamy st_zaklo i Skaler
        	if (Key == 1) zaklo++;
        	else if (Key == 2) zaklo--;
        	else if (Key == 4) zaklo=zaklo+3;
        	else if (Key == 5) zaklo=zaklo-3;
        	else if (Key == 7) zaklo=zaklo+6;
        	else if (Key == 8) zaklo=zaklo-6;

        	if (Key == 12) wart_zad++;
        	else if (Key == 13) wart_zad--;

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
//#endif

   void InitData()
    {
        for (int a = 0; a < (128 * 8); a++)
            ekran[a] = 0;
        for (int a = 0; a < (40 * 16); a++)
            textEkran[a] = ' ';
    #ifndef CPP_EXAMPLE
        for (int a = 0; a < MaxObj; a++)
        {
            s[a] = (rand() & 0x1f) + 1;
            x[a] = rand() & 0x03ff;
            if (x[a] > 239 * 4 - s[a])
                x[a] -= 239 * 4 - s[a];
            y[a] = rand() & 0x1ff;
            if (y[a] > 127 * 4 - s[a])
                y[a] -= 127 * 4 - s[a];
            dx[a] = ((rand() & 0x1) ? 1 : -1) * (((rand() & 0x7) + 2) >> 1);
            dy[a] = ((rand() & 0x1) ? 1 : -1) * (((rand() & 0x7) + 2) >> 1);
        }
    #endif
    }
    void ClearScreen()
    {

        for (int a = 0; a < (128 * 8); a++)
        ekran[a] = 0;
    }
   void DrawPixels(int Key)
    {
    #ifdef CPP_EXAMPLE
        for (int c=0;c<MaxObj;c++)
        {
            objects[c].move(Key,Key);
            objects[c].draw();
        }
    #else

        unsigned char* ekran_bajt = (unsigned char*)ekran;
        for (int i=0; i< (256*128/8); i = i+4)
        {
        	if (syg_ster <= 7.5) predkosc = Tim3;
        	else if (syg_ster > 7.5 && syg_ster <= 18) predkosc = Tim2;
        	else if (syg_ster > 18) predkosc = Tim1;
        	if (predkosc % 3 == 0)
        	{
				ekran_bajt[i] = wiatraktermogrzalkaNowa0_h[i+3];
				ekran_bajt[i+1] = wiatraktermogrzalkaNowa0_h[i+2];
				ekran_bajt[i+2] = wiatraktermogrzalkaNowa0_h[i+1];
				ekran_bajt[i+3] = wiatraktermogrzalkaNowa0_h[i];
        	}
        	else if (predkosc % 3 == 1)
        	{
        		ekran_bajt[i] = wiatraktermogrzalkaNowa60_h[i+3];
        		ekran_bajt[i+1] = wiatraktermogrzalkaNowa60_h[i+2];
        		ekran_bajt[i+2] = wiatraktermogrzalkaNowa60_h[i+1];
        		ekran_bajt[i+3] = wiatraktermogrzalkaNowa60_h[i];
        	}
        	else if (predkosc % 3 == 2)
        	{
        		ekran_bajt[i] = wiatraktermogrzalkaNowa30_h[i+3];
        		ekran_bajt[i+1] = wiatraktermogrzalkaNowa30_h[i+2];
        		ekran_bajt[i+2] = wiatraktermogrzalkaNowa30_h[i+1];
        		ekran_bajt[i+3] = wiatraktermogrzalkaNowa30_h[i];
        	}

        	if (i == 2004)
        	{
        		ekran_bajt[i] = 1;
        	}
        	if (i == 2000)
        	{
        		ekran_bajt[i] = 255;
        	}
        }

    #endif
    }

   //////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////
