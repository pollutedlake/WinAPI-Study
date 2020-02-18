/*
파일 : SanSu.cpp
작성자 : 박상현
작성일 : 2020-02-18
*/

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MyTimer");

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
	static TCHAR sSansu[128];		// 덧셈식을 문자열로 변경하여 저장할 문자열, 덧셈식을 기억해야 하므로 정적 변수로 선언
	static RECT rt = { 100, 100, 400, 120 };		// 무효화 시킬 범위
	static int x;		// 덧셈할 변수
	static int y;		// 덧셈할 변수

	switch (iMessage) {
	case WM_CREATE:		// 윈도우가 처음 생성될 때 발생하는 메시지, 프로그램 시작시 꼭 한 번만 초기화해야 할 처리를 한다.(메모리 할당, 전역변수 초기화 등)
		// UINT SetTimer(HWND hWnd, UINT nlDEvent, UINT uElapse, TIMERPROC lpTimerFunc) : 타이머를 생성하는 함수
		// hWnd : 타이머 메시지를 받을 윈도우
		// nIDEvent : 타이머의 번호를 지정한다. 이 타이머 번호는 WM_TIMER 메시지에서 타이머를 구분하기 위한 표식으로 사용된다.
		// uElapse : 타이머의 주기 설정, 단위는 1/1000초이다. 아무리 짧게 해도 1초에 발생하는 타이머 메시지에 한계가 있다.
		// lpTimerFunc : 타이머 메시지가 발생할 때마다 호출될 함수를 지정한다. 사용하지 않을 경우 NULL로 설정한다.
		// 리턴 값은 타이머를 소유하는 윈도우 없이 타이머가 만들어 졌을 경우, 즉 hWnd 인수가 NULL일 경우에 한해 특별하게 사용한다.
		SetTimer(hWnd, 1, 3000, NULL);		// 타이머 생성

		// LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, lParam)	: 강제로 메시지가 발생한 것처럼 호출하는 함수
		// hWnd : 메시지를 받을 윈도우
		// Msg : 메시지의 종류
		// wParam, lParam : 메시지의 추가 정보
		// 리턴 값은 메시지 처리 함수가 리턴하는 값이며 메시지에 따라 해석 방법이 다르다.
		SendMessage(hWnd, WM_TIMER, 1, 0);		// 프로그램 시작 1초 후 WM_TIMER가 호출되므로 시작 후 바로 WM_TIMER 호출하여 시작 직후에 시간이 출력되도록 한다.
		return 0;
	case WM_TIMER:		// wParam으로 타이머 ID를 전달받으며 lParam으로 타이머 메시지 발생시 호출될 함수의 번지가 전달된다.
		x = rand() % 30;		// 30 미만의 무작위 정수로 초기화
		y = rand() % 30;		// 30 미만의 무작위 정수로 초기화
		wsprintf(sSansu, TEXT("%d + %d = ?"), x, y);		// 출력을 위해 sSansu 문자열로 변환해 둔다.
		InvalidateRect(hWnd, &rt, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, sSansu, lstrlen(sSansu));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		// BOOL KillTimer(HWND hWnd, UINT uIDEvent) : 설치된 타이머를 제거하는 함수, 타이머는 시스템 전역 자원이므로 더 이상 필요가 없어지면 파괴하는 것이 좋다
		// hWnd : 이 타이머를 소유한 윈도우 핸들
		// uIDEvent : 파괴할 타이머 ID를 지정한다.
		KillTimer(hWnd, 1);		// 설치된 타이머를 제거하는 함수
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}