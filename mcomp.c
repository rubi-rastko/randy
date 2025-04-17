#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

#include "mcomp.h"
#include "rmidi.h"

unsigned short int c_scale[SZ_SCALE] = {0, 2, 3, 5, 7, 8, 10, 12};
int c_base = 36;
int c_width = 8;
int c_velo = 16;
int c_quant = 24;
int c_rand = 0;

int MCOMP_Init()
{
	FILE *mcompparams;
	int i = 0;

	mcompparams = fopen("mcomp.dat", "rb");

	if (mcompparams){
		do {
			fscanf(mcompparams, "%i ", &c_scale[i]);
		}while (c_scale[i++] != 666);

		fscanf(mcompparams, "%i %i %i %i", &c_base, &c_width, &c_velo, &c_quant);
	}

	else {
		mcompparams = fopen("mcomp.dat", "wb");
		fprintf(mcompparams, "0 2 3 5 7 8 10 12 666\n36 8 16 24");
	}

	fclose(mcompparams);


	c_rand = time(0); /* this has to move into genRndNotes
					  because of dynamic generation of notes */

	return 1;
}

int MCOMP_SetParams(int a, int b, int c, int d)
{
	c_base = a;
	c_width = b;
	c_quant = c;
	c_velo = d;

	return 1;
}

int genRndNotes(MidiEvent *e, unsigned short int esz)
{
	int i = 0;

	srand(time(0));

	while(i < esz) {
		e[i].dt = c_quant + 0;
		e[i].mb = 
			(c_base + c_scale[rand() % c_width]) * 0x100 
			+ (rand() % 2) * c_velo;
		i++;
	}
	
	e[esz - 1].mb = 0x00000000;

	return 0;
}
