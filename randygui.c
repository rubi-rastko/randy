#include <windows.h>
#include <commctrl.h>

#include "randy.h"
#include "rmidi.h"
#include "resource.h"

extern int prmdlg[];
extern MidiEvent *midistream;
(*PLG_OpenParams)();

LRESULT CALLBACK wproc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK wproccli(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK dproc(HWND, UINT, WPARAM, LPARAM);

HWND winit(HINSTANCE hInst)
{
	HWND hwndframe;
	WNDCLASS wcl = {
		(DWORD)NULL,
		wproc,
		0, 0,
		(HANDLE)NULL,
		(HICON)NULL, (HCURSOR)NULL,
		(HBRUSH)(COLOR_APPWORKSPACE + 1),
		"MAINMENU",
		"RandyWnd"}; /* these all gotta be constants */
	WNDCLASS wclc = {(DWORD)NULL, wproccli, 0, 0, (HANDLE)NULL,
		(HICON)NULL, (HCURSOR)NULL, (HBRUSH)(COLOR_WINDOW + 1),
		NULL, "RandyWndCli"};

	InitCommonControls();
	wcl.hInstance = hInst;
	wclc.hInstance = hInst;
	RegisterClass(&wcl);
	RegisterClass(&wclc);
	hwndframe = CreateWindowEx(0, "RandyWnd", "Randy",
		WS_OVERLAPPED | WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 
		(HWND)NULL, (HMENU)NULL, hInst, (LPVOID)NULL);
	

	return hwndframe;
}

int RandyWnd_OnPaint(HWND h)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(h, &ps);
	Rectangle(hdc, 0, 0, 640, 480);
	EndPaint(h, &ps);

	return 0;
}

int RandyWnd_OnKeyDown(HWND h, WPARAM w)
{
	switch(w){
		case 0x20:
			MAUSTR_StopOut();
			break;
		default:
			break;
	}

	return 0;
}

int RandyWnd_OnMenuCommand(HWND h, WPARAM w)
{
	switch (w){
		case 101:
			PostQuitMessage(0);
			break;
		default:
			break;
	}
	
	return 0;
}

int RandyWnd_OnLeftClick(void)
{
	MidiEvent evs[SZ_SEQUENCE];

	genRndNotes(evs, SZ_SEQUENCE);

	synced_Randy_SetSequence(midistream, evs);

	return 0;
}

int RandyWnd_OnRightClick(HWND h)
{
	(*PLG_OpenParams)(h);

	return 0;
}

BOOL CALLBACK wenumproc(HWND h, LPARAM l)
{
	RECT *rc;

	rc = (RECT *)l;

	MoveWindow(h, 0, 0, rc->right, rc->bottom, TRUE);

	return TRUE;
}

LRESULT CALLBACK wproccli(HWND h, UINT u, WPARAM w, LPARAM l)
{
	return DefWindowProc(h, u, w, l);
}

LRESULT CALLBACK wproc(HWND h, UINT u, WPARAM w, LPARAM l)
{
	RECT rcClient;

	switch (u){

		case WM_CREATE:
			CreateWindowEx(WS_EX_CLIENTEDGE, "RandyWndCli", "",
				WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0, 
				(HWND)h, (HMENU)31,
				((CREATESTRUCT *)l)->hInstance, (LPVOID)NULL);
			
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			RandyWnd_OnPaint(h);
			break;
		case WM_COMMAND:
			if (HIWORD(w) == 0)
				RandyWnd_OnMenuCommand(h, LOWORD(w));

			break;
		case WM_KEYDOWN:
			RandyWnd_OnKeyDown(h, w);
			break;
		case WM_LBUTTONDOWN:
			RandyWnd_OnLeftClick();
			break;

		case WM_RBUTTONDOWN:
			RandyWnd_OnRightClick(h);
			break;
		default:
			return DefWindowProc(h, u, w, l); /* */
			break;
	}

	return 0;
}



