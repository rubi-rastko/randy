#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HINSTANCE hInst = NULL;

	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;
	}

	return TRUE;
}


