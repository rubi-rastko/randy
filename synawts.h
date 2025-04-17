#ifndef SYNAWTS_H
#define SYNAWTS_H

#define MYMAX(a,b)	((a > b) ? b : a)

#include "rmidi.h"

#define		SZ_SPECTRUM	16	/* max broj harmonika */
#define		N_PCHMAX	128	/* max broj visina */
#define		SZ_WTMAX	4096	/* max velicina wavetable (smp) */
#define		SZ_SINMAX	SZ_WTMAX	/* -||- sinusoide */

typedef signed short int WORD;
typedef unsigned long int DWORD;

float tune_equal(int, int);
float sine_wave(float, float);
int syn_period(unsigned long, float, 
unsigned long, unsigned short);
int render_samples(WORD *, DWORD, MidiEvent *, unsigned long);


#endif
