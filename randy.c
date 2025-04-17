/***************************\
 * randy 		   *
 * napisao Rastko Petrovic *
\***************************/

#include <windows.h>
#include <winuser.h>
#include <mmsystem.h>

#include "randy.h"
#include "mseq.h"
#include "rmidi.h"


DWORD WINAPI auoutStart(LPVOID);
int MAUSTR_LoadPlugin(char *);
int MAUSTR_StopOut();
int MAUSTR_Init(MidiEvent *);
int MCOMP_Init();

/***************/
/* global vars */

HANDLE dspthh;

char scnt[255];
char tmf[255];

FILE *se;

int prmdlg[16] = {0};

MidiEvent jebiga[SZ_SEQUENCE];
MidiEvent *midistream = jebiga;

/*		*/
/****************/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hpInst, LPSTR cl, int ncs)
{
	MSG msg;
	HWND hwnd;	
	DWORD dspthid;
	int i = 0;

	hwnd = (HWND)winit(hInst);
	
	Randy_InitSync();
	MAUSTR_Init(midistream);
	MCOMP_Init();
	genRndNotes(midistream, SZ_SEQUENCE);
	MAUSTR_LoadPlugin("SYNAWTS");

	if (auoutStart == NULL)
		return 1;


	dspthh = CreateThread((LPSECURITY_ATTRIBUTES)NULL,
		0, (LPTHREAD_START_ROUTINE)auoutStart, (LPVOID)NULL, 
		(DWORD)CREATE_SUSPENDED, &dspthid);


	if (dspthh == NULL){
		printf("Thread handle is NULL\n");
		return 0;
	}

	else {
		;
	} 

	ResumeThread(dspthh);


	while ( GetMessage( &msg, NULL, 0, 0 ) != 0 ){
		TranslateMessage( &msg );
		DispatchMessage( &msg );	
	}

	return msg.wParam;
}


