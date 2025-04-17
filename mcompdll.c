#include <windows.h>

#include "randy.h"

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason){
		case DLL_PROCESS_ATTACH:
			break;

	}

	return TRUE;
}
