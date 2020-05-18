﻿#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return(int)Message.wParam;
}

void TestResource(BOOL bUnSel, BOOL bDelete, int num) {
	HDC hdc;
	HPEN hPen, OldPen;
	int i;
	TCHAR str[256];

	hdc = GetDC(hWndMain);
	for (i = 0; i < num; i++) {
		hPen = CreatePen(PS_SOLID, rand() % 5 + 1, RGB(rand() % 256, rand() % 256, rand() % 256));
		OldPen = (HPEN)SelectObject(hdc, hPen);
		MoveToEx(hdc, rand() % 300, rand() % 200 + 40, NULL);
		LineTo(hdc, rand() % 300, rand() % 200 + 40);
		if (bUnSel) {
			SelectObject(hdc, OldPen);
		}
		if (bDelete) {
			DeleteObject(hPen);
		}
		wsprintf(str, TEXT("%d"), i);
		SetWindowText(hWndMain, str);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	const TCHAR* Mess = TEXT("1 : 정상적인 경우, 2 : 선택 해제없이 삭제만, 3 : 삭제하지 않음");

	switch (iMessage) {
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			TestResource(TRUE, TRUE, 10000);
			break;
		case '2':
			TestResource(FALSE, TRUE, 10000);
			break;
		case '3':
			TestResource(FALSE, FALSE, 10000);
			break;
		}
		return 0;
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mess, lstrlen(Mess));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}