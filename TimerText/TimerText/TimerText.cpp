/*
파일 : TimerText.cpp
작성자 : 박상현
작성일 : 2020.03.03
*/
// 일정 개수의 문자열을 일정한 시간 간격(2초 정도)으로 교체하여 출력하는 예제

#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("TimerText");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass , WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 100, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return(int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR msg[2][128] = { {L"TimerText1"}, {L"TimerText2"} };		// 출력할 문자열 배열
	static int msgIdx = 0;		// 출력할 문자열을 선택하는 인덱스

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 2000, NULL);		// 2초 타이머
		return 0;
	case WM_TIMER:
		if (msgIdx == 1) {		// 문자열 배열의 끝이면 처음 문자열 선택
			msgIdx = 0;
		}
		else {		// 아니라면 다음 출력할 문자열 선택
			msgIdx++;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, msg[msgIdx], lstrlen(msg[msgIdx]));
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}