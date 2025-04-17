#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "synawts.h"

BOOL CALLBACK dproc(HWND, UINT, WPARAM, LPARAM);
extern HINSTANCE hInst;
extern unsigned long tmb[SZ_SPECTRUM];
extern unsigned long eg_dec;
static ifdo = 0;
unsigned long hey = 0;

int MSYN_InitGUI(void)
{

	

	return 1;
}

int PLG_OpenParams(HWND h)
{

	if (ifdo)
		;
	else
	{
		ifdo = 1;

		CreateDialog(hInst, "JEBIGA", h, dproc);
	}


}

void OnParamDlg_OK(HWND h)
{
	char editStr[32] = {0};
	unsigned long tmbl[SZ_SPECTRUM];
	int i = 0;
}

BOOL CALLBACK dproc(HWND h, UINT u, WPARAM w, LPARAM l)
{
	int i = 0;
	static unsigned long tmbl[SZ_SPECTRUM];


	switch (u){
		case WM_HSCROLL:
		case WM_VSCROLL:
			if (LOWORD(w) == TB_THUMBTRACK){
				hey = SendMessage(GetDlgItem(h, 111), TBM_GETPOS, 0, 0);

				for (i = 120; i < 136; i++)
					tmbl[i - 120] = SendMessage(GetDlgItem(h, i), TBM_GETPOS, 0, 0);

				synced_MSYN_SetTembre(tmb, tmbl);
				synced_MSYN_CopyLong(&eg_dec, &hey);

			}
			break;
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(h, 111), TBM_SETRANGE, 1, (LPARAM)MAKELONG(0, 16384));

			for (i = 120; i < 136; i++){
				SendMessage(GetDlgItem(h, i), TBM_SETRANGE, 1, (LPARAM)MAKELONG(i - 118, 18));
			}

			break;
		case WM_COMMAND:
			if (LOWORD(w) >= 10 && LOWORD(w) <= 26){
				OnParamDlg_OK(h);
			}
			break;
		case WM_CLOSE:
			ifdo = 0;
			DestroyWindow(h);
			break;
		default:
			break;
	}

	return 0;
}