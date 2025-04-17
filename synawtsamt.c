#include <windows.h>
#include "synawts.h"

CRITICAL_SECTION cts;


int MSYN_InitSync()
{
	InitializeCriticalSection(&cts);

	return 0;
}

int synced_MSYN_GetTembre(unsigned long *d, unsigned long *s)
{
	int i = 0;

	EnterCriticalSection(&cts);

	while (i < SZ_SPECTRUM){
		d[i] = s[i];
		i++;
	}

	LeaveCriticalSection(&cts);

	return 0;
}

int synced_MSYN_SetTembre(unsigned long *d, unsigned long *s)
{
	int i = 0;

	EnterCriticalSection(&cts);

	while (i < SZ_SPECTRUM){
		d[i] = s[i];
		i++;
	}

	LeaveCriticalSection(&cts);

	return 0;
}

int synced_MSYN_CopyLong(unsigned long *d, unsigned long *s)
{
	int i = 0;

	EnterCriticalSection(&cts);

	*d = *s;

	LeaveCriticalSection(&cts);

	return 0;
}