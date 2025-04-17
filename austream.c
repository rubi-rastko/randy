#define		WFBA	(WFRE / 8) * WFCH /* audio byte-alignment */

#include <windows.h>
#include <mmsystem.h>

#include "randy.h"
#include "rmidi.h"

int (*render_samples)(WORD *, DWORD, MidiEvent *, unsigned long); /* should be renamed to something like 'synth_tones' */
int MSEQ_Init();
int SequencerProc(MidiEvent *, MidiEvent *, unsigned long MMTimer);
int tproc(HWND, UINT, WPARAM, LPARAM);
int process_ai_buffer(WORD *, DWORD);

HWAVEOUT hwo = (HWAVEOUT)NULL;
HGLOBAL	sdh[NBUFS];
HPSTR	sdp[NBUFS];
HGLOBAL	whdrh[NBUFS];
LPWAVEHDR whdrp[NBUFS];
WAVEFORMATEX wfx = {
		WAVE_FORMAT_PCM,
		WFCH, WFSR, WFSR * WFBA, WFBA, WFRE,
		(WORD)NULL
};
static char errTxt[255];

MidiEvent *estr;

int MAUSTR_Init(MidiEvent *e)
{
	estr = e;

	MSEQ_Init();

	return 1;
}

int MAUSTR_StopOut(void)
{
	static paused = 0;

	if (hwo != (HWAVEOUT)NULL)
   		if (paused)
			paused = waveOutRestart(hwo) * 0;
		else 
			paused = waveOutPause(hwo) + 1;

	return 0;
}

DWORD WINAPI auoutStart(LPVOID p)
{
	MSG msg;
	WAVEOUTCAPS wdc;
	MMRESULT x;
	MMTIME mmt;
	int i = 0;

	mmt.wType = TIME_SAMPLES;

	x = waveOutGetNumDevs(); 
	x = waveOutGetDevCaps(0, &wdc, sizeof(WAVEOUTCAPS));
	x = waveOutOpen(&hwo, 0, &wfx, (DWORD)GetCurrentThreadId(),
		(DWORD)NULL, CALLBACK_THREAD | WAVE_ALLOWSYNC);

	if (x != MMSYSERR_NOERROR){
		waveOutGetErrorText(x, errTxt, 255);
		printf("MAUSTR: [%s]\n", errTxt);
		return 0;
	}


	for (i = 0; i < NBUFS; i++){
		sdh[i] = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, BUFSZ * sizeof(WORD));
		sdp[i] = GlobalLock(sdh[i]);
		process_ai_buffer((WORD *)sdp[i], BUFSZ * i);
		whdrh[i] = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE,
			(DWORD)sizeof(WAVEHDR));
		whdrp[i] = (LPWAVEHDR)GlobalLock(whdrh[i]);
		whdrp[i]->lpData = sdp[i];
		whdrp[i]->dwBufferLength = BUFSZ * sizeof(WORD);
		whdrp[i]->dwFlags = 0;
		whdrp[i]->dwLoops = 0;
		x = waveOutPrepareHeader(hwo, whdrp[i], sizeof(WAVEHDR));
		x = waveOutWrite(hwo, whdrp[i], sizeof(WAVEHDR));
	}

	
	while(1){
		WaitMessage();
		if (GetMessage(&msg, NULL, MM_WOM_DONE, MM_WOM_DONE)){
			tproc(msg.hwnd, msg.message, msg.wParam,
			msg.lParam);
		}
	}

	x = waveOutReset(hwo);
	x = waveOutClose(hwo);

	return msg.wParam;
}

int tproc(HWND h, UINT u, WPARAM w, LPARAM l)
{
	static HWAVEOUT hwo;
	static MMTIME mmt;
	LPWAVEHDR lpwh;
	DWORD x;
	FLOAT protime;

	hwo = (HWAVEOUT)w;
	lpwh = (LPWAVEHDR)l;

	x = waveOutUnprepareHeader(hwo, lpwh, sizeof(WAVEHDR));
	x = waveOutGetPosition(hwo, &mmt, sizeof(MMTIME));							
	protime = mmt.u.sample / (float)WFSR;	
	process_ai_buffer((WORD *)(lpwh->lpData), mmt.u.sample);
	x = waveOutGetPosition(hwo, &mmt, sizeof(MMTIME));
	protime = mmt.u.sample / (float)WFSR - protime;
	x = waveOutPrepareHeader(hwo, lpwh, sizeof(WAVEHDR));
	x = waveOutWrite(hwo, lpwh, sizeof(WAVEHDR));
	
	return 0;
}


/* funkcija za ispisivanje 'sintisajzer plug-in/audio instrument' bafera */
/* mbuf_ai = adresa ai bafera, hwpos = pozicija mm. tajmera */


int process_ai_buffer(WORD *mbuf_ai, DWORD hwpos)
{
	static unsigned long tsc = 0; /* tone sample counter */
	static MidiEvent e[SZ_SEQUENCE] = {{0, 0}}; /* current MIDI event register */
	static MidiEvent el = {0, 0};
	int i = 0;

	synced_Randy_GetSequence(e, estr);

	for (i = 0; i < BUFSZ; i++){
		mbuf_ai[i] *= 0;

		if (SequencerProc(e, &el, tsc)){
			tsc = 0;
			

		}

		/* it is better to use pointer arithmetic here then
		indexing in 'synawts'; plus, it fixes clicking */

		(*render_samples)(mbuf_ai + i, hwpos, &el, tsc);
		tsc++;
	}

	return 0;

}
