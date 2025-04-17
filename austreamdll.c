#include <windows.h>

HINSTANCE hInst;


BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason){
		case DLL_PROCESS_ATTACH:
			break;
	}

	return TRUE;
}


