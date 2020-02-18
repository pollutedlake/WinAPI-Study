/*
파일 : TwoTimer.cpp
작성자 : 박상현
작성일 : 2020-02-18
*/

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_Inst;
LPCTSTR lpszClass = TEXT("TwoTimer");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_Inst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	static TCHAR sTime[128];
	static RECT rt = { 100,100,400,120 };

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);		// 1번 ID로 1초 간격의 타이머 생성
		SetTimer(hWnd, 2, 5000, NULL);		// 2번 ID로 5초 간격의 타이머 생성
		SendMessage(hWnd, WM_TIMER, 1, 0);
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:		// 1번 타이머라면 시간 갱신 후 화면 다시 그리기
			GetLocalTime(&st);
			wsprintf(sTime, TEXT("지금 시간은 %d:%d:%d입니다."), st.wHour, st.wMinute, st.wSecond);
			InvalidateRect(hWnd, &rt, TRUE);
			break;
		case 2:		// 2번 타이머라면 비프음을 낸다.
			MessageBeep(0);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, sTime, lstrlen(sTime));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:		// 설치된 타이머를 모두 해제해야 한다.
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}