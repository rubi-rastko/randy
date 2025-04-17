#ifndef RANDY_H
#define RANDY_H

#include <stdio.h>
#include <windows.h>

#define		WFCH	1U	/* audio channels */
#define		WFSR	44100UL /* audio sample rate */
#define		WFRE	16U	/* audio precision */

#define		BUFSZ	5512	/* audio-buffer size */
#define 	NBUFS	5	/* number of audio-buffers */

#define		SZ_SEQUENCE	1024	/* max number of (MIDI) events */
#define		MAX_MTRX	16		/* max number of simultaneous MIDI streams */


typedef struct {
	int mres;
	int slen;
	int temp;
	int bnot;
	int sclr;
	int rval;
	int bvel;
}ParamDlg;


#endif
