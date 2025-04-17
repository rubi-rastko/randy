

#define		spert(a,b,c)	(((60.0F / (a / 4.0F)) * b) / 4) / c

#include "randy.h"
#include "mseq.h"
#include "mcomp.h"
#include "rmidi.h"

typedef unsigned long int DWORD; 

int s_tempo = 120;
int s_len = 384;
int s_reso = 48;
int s_loop = 1;
int s_spt = 0;

int MSEQ_Init()
{
	s_spt = spert(s_tempo, WFSR, s_reso);

	return 0;
}

int MSEQ_SetParams(int a, int b, int c)
{
	s_reso = a;
	s_len = b;
	s_tempo = c;

	s_spt = spert(s_tempo, WFSR, s_reso);

	return 1;
}

unsigned short int SequencerProc(MidiEvent *e, MidiEvent *a, unsigned long p)
{
	static unsigned long ec = 0UL; /* midi event pointer */
	static unsigned long ta = 0UL; /* tick accumulator */

	if (p / s_spt == e[ec].dt) {
		ta += e[ec].dt;

		a->mb = e[ec].mb;
		ec = ta % s_len ? ec + 1 : 0;

		return 1;
	}

	else 
		return 0;
}
