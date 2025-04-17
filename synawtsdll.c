#include <windows.h>

HINSTANCE hInst;

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	

	switch (fdwReason){
		case DLL_PROCESS_ATTACH:
			hInst = hInstDLL;
			break;

		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}
