#include <windows.h>

#include "randy.h"
#include "rmidi.h"

CRITICAL_SECTION cts;

int Randy_InitSync()
{
	InitializeCriticalSection(&cts);

	return 0;
}

int synced_Randy_GetSequence(MidiEvent *d, MidiEvent *s)
{
	int i = 0;

	EnterCriticalSection(&cts);

	while (i < SZ_SEQUENCE){
		d[i].dt = s[i].dt;
		d[i].mb = s[i].mb;
		i++;
	}

	LeaveCriticalSection(&cts);

	return 0;
}

int synced_Randy_SetSequence(MidiEvent *d, MidiEvent *s)
{
	int i = 0;

	EnterCriticalSection(&cts);

	while (i < SZ_SEQUENCE){
		d[i].dt = s[i].dt;
		d[i].mb = s[i].mb;
		i++;
	}

	LeaveCriticalSection(&cts);

	return 0;
}