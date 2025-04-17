
#include <math.h>

#include "randy.h"
#include "mseq.h"
#include "synawts.h"
#include "rmidi.h"

/*** watch portability! ***/

typedef signed long int AUSAMPLE;

float bufWave[SZ_WTMAX];	/* wavetable buf. */
float interSineWave[SZ_SINMAX];
float sf_SineWave[SZ_SINMAX];	/* spect. base buf. */
static float into_eqtmp[N_PCHMAX];
unsigned long eg_dec = 8192UL; /* this is a placeholder for other parameters
							   which may be added, and reformatted */
unsigned long tmb[SZ_SPECTRUM] = {
	0x00000002,0x00000003,0x00000004,0x00000005,
	0x00000006,0x00000007,0x00000008,0x00000009,
	0xa,0xb,0xc,0xd, 0xe,0xf,0x10,0x11
};

int MSYN_Init()
{
	FILE *tunfile;
	int i = 0;

	for (i = 0; i < N_PCHMAX; i++)
		into_eqtmp[i] = tune_equal(i + 1, 16.351F);

	tunfile = fopen("awtstune.dat", "rb");

	i = 0;

	if (tunfile){
		while (i++ < N_PCHMAX)
			fscanf(tunfile, "%6.3f ", &into_eqtmp[i]);
	}

	else {
		i = 0;
		tunfile = fopen("awtstune.dat", "wb");
		
		while (i++ < N_PCHMAX)
			fprintf(tunfile, "%6.3f\n", into_eqtmp[i]);
	}

	fclose(tunfile);

	for (i = 0; i < SZ_SINMAX; i++)
		sf_SineWave[i] = (sine_wave(1.0F, i / (float)SZ_SINMAX));

	MSYN_InitSync();
	MSYN_InitGUI();

	return 1;
}

int midimsg_decode(unsigned short *p, unsigned short *v, const MidiEvent *m)
{
	*p = ((m->mb) & 0xFF00) / 0x100;
	*v = ((m->mb) & 0x00FF) % 0x100;

	return 0;
}

float tune_equal(int p, int b)
{
	int o = p / 12;
	int x = p % 12;
	int a = b * pow(2, o);

	return pow(10, x * (log10(2) / 12) + log10(a));
}

float sine_wave(float f, float t)
{
    float w = 2.0 * 3.14159 * f;
	
	return sin(w * t);
}

float interpolate_presine(float *lut, unsigned short i, unsigned long s)
{
	int n = 0;
	int rn, rn2 = 0; /* ratio of the period size register */
	float r = 0.0F; /* ratio of interpolation */

	
	rn = (int)(((float)(SZ_SINMAX / (float)s)) * (float)i);
	rn2 = (int)(((float)(SZ_SINMAX / (float)s)) * (float)(i + 1));


	return (float)(lut[rn] + (((lut[rn2] - lut[rn]) / (float)(rn2 - rn)) * ((float)(SZ_SINMAX % s) / (float)s)));
}

int syn_period(unsigned long sr, float f, 
unsigned long a, unsigned short ssz)
{
	unsigned int i = 0, j = 0, k = 0;
	unsigned long nsmp = (unsigned long)(sr / f);
	float nsmpf = (float)(sr / f);
	unsigned long tmbl[SZ_SPECTRUM] = {0};

	synced_MSYN_GetTembre(tmbl, tmb); /* perhaps a more general
									  function such as GetParams */
	while ( i < nsmp ){
		bufWave[i] = interSineWave[i] = interpolate_presine(sf_SineWave, i, nsmpf);
		/* bufWave[i] = bufSine[i] = sine_wave(f, t, a); */
		i++;
	}
	
	i = 0; /* reusing counters is bad idea */

	for (j = 0; j < ssz; j++){
		i = 0;
		k = 0;
		
		while (i < nsmp){
			k = (i * (j + 2)) % nsmp;

			/* period mix buffer sample gets value
				of previous mix combined with
				a value of sine buffer's multiple */

			bufWave[i] += interSineWave[k] 
				* (1.0F / (float)(tmbl[j] % 0x100)); /* */
			i++;			
		}
	}

	return 0;
}


/* modified render_samples (should be renamed) with new parameters */
/* m = raw midi event, ne = new event trigger (could be handled by m as well) */


int render_samples(WORD *buf1, DWORD tpos, MidiEvent *m, unsigned long ne)
{
	static unsigned short n = 100; /* wavetable size (tone frequency) */
	static unsigned short mp = 0, mv = 0; /* pitch and velocity regs */
	AUSAMPLE csv = 0;	/* computed sample value */
	static unsigned long priv_eg_dec = 8192UL;

	synced_MSYN_CopyLong(&priv_eg_dec, &eg_dec);
	
	if (ne == 0) {
		midimsg_decode(&mp, &mv, m);
		n = (int)(WFSR / into_eqtmp[mp]);
		syn_period(WFSR, into_eqtmp[mp], mv * 0x7F,
				1 + rand() % SZ_SPECTRUM);
	}

	csv = (AUSAMPLE)((AUSAMPLE)(bufWave[ne % n] * (mv * 0x7F)) * (float)(1.0F - (float)(MYMAX(ne, priv_eg_dec) / (float)priv_eg_dec)));
		

		/* METRONOME CODE */
		/* if (((nsmp + i) / SmpPerTick(msp.t, WFSR, msp.r)) % 48 < 1)
			csv += rand() % 1024; */
		
	
	/* don't know why it worked before, but tsc/ne can get larger then
	BUFSZ if notes are longer, and then corrupts the program by
	accessing buf1[ne], and further calls pass wrong pointers */
	
	*buf1 += (WORD)csv; /* += for mix, = for overwrite */
	/* *buf1 /= 2; */ /* why doesn't this work */

	return 0;
}
