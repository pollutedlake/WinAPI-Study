/*
파일 : TimerText2.cpp
작성자 : 박상현
작성일 : 2020.03.04
*/
// 일정 시간 간격으로 문자열들을 교체하되 문자열이 아래에서 위로 스크롤되면서 올라오고 교체할 시간이 되면 Y좌표 100에 문자열을 출력하고 점점 위로 올라와 Y좌표 10에서 잠시 머무른 후 다음 문자열로 교체한다.

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 100, NULL, (HMENU)NULL, hInstance, NULL);
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
	TCHAR test[2][128] = { {L"test1"}, {L"test2"} };		// 출력할 문자열들
	static int textIdx = 0;		// 출력할 문자열을 선택하는 인덱스 변수
	static int textY = 100;		// 문자열이 출력될 Y좌표

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 2, 50, NULL);	// 문자열을 스크롤되면서 올라오게끔하는 타이머
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			KillTimer(hWnd, 1);
			if (textIdx == 1) {		// 문자열 인덱스의 끝이면 다시 처음으로 교체
				textIdx = 0;
			}
			else {		// 문자열 교체
				textIdx++;
			}
			textY = 100;		// 교체가 되면 Y좌표 다시 100으로
			SetTimer(hWnd, 2, 50, NULL);		// 교체가 되면 다시 스크롤 시작
			break;
		case 2:
			if (textY == 10) {		// Y좌표가 10이면
				KillTimer(hWnd, 2);		// 스크롤을 멈춘다.
				SetTimer(hWnd, 1, 1000, NULL);		// 문자열을 교체할 타이머
			}
			else {
				textY--;		// 문자열이 스크롤되면서 올라온다.
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, textY, test[textIdx], lstrlen(test[textIdx]));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}