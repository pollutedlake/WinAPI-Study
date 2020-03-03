/*
파일 : WmCreate.cpp
작성자 : 박상현
작성일 : 2020.03.04
*/
// 윈도우와 관련된 메시지 중 가장 간단한 메시지는 WM_CREATE와 WM_DESTROY 두 가지가 있다. 두 메시지는 윈도우의 일생을 통틀어 딱 한 번씩만 전달되는 특성이 있어 일회적인 작업인 초기, 종료 처리에 사용할 수 있다.
// 두 메시지는 특정한 윈도우에 관련된 초기/종료 처리를 하는데 사용하는 것이 좋고 WinMain에서는 프로그램 전역적인 초기/종료 처리를 하는 것이 좋다.

#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("WmCreate");
TCHAR* mem;		// 동적으로 할당된 메모리 번지를 기억하기 위한 전역변수

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	// mem = (TCHAR*)malloc(1048576);	<--- CreateWindow 함수 호출문 다음에 초기화 코드를 작성해도 WM_CREATE 메시지를 받은 것과 효과는 같다.
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	// free(mem);	<--- 메시지 루프 다음에 종료 처리 코드를 작성해도 WM_DESTROY 메시지를 받은 것과 같다.
	return(int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	// WM_CREATE 메시지는 윈도우가 생성될 때 보내지므로 각종 초기화를 하기에 적합한 장소이다.
	case WM_CREATE:
		mem = (TCHAR*)malloc(1048576);
		return 0;
	// WM_DESTROY 메시지는 윈도우가 파괴될 때 보내지므로 종료처리를 하기에 적합하다.
	case WM_DESTROY:
		free(mem);
		// PostQuitMessage : 메시지 큐에 WM_QUIT를 넣음으로써 WinMain에 있는 메시지 루프를 종료시키고 WinMain 함수 자체를 종료하여 운영체제로 복귀시키는 역할을 한다.
		PostQuitMessage(0);		// 메인 윈도우의 경우 반드시 PostQuitMessage 함수를 호출해야 한다.
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}