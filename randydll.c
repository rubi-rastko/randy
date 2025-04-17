#include <windows.h>

HINSTANCE hInst;
FARPROC render_samples;
FARPROC PLG_OpenParams;
FARPROC MSYN_Init;

int MAUSTR_LoadPlugin(char *n)
{
	if((hInst = LoadLibrary(n))){
		render_samples = GetProcAddress((HMODULE)hInst, "render_samples");
		PLG_OpenParams = GetProcAddress((HMODULE)hInst, "PLG_OpenParams");
		MSYN_Init = GetProcAddress((HMODULE)hInst, "MSYN_Init");
	}

	else {
		MessageBox(NULL, "Cannot load plug-in.", "Module AUSTREAM", MB_OK);
		return 0;
	}

	(*MSYN_Init)();


	return 1;
}