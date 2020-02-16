#include "framework.h"
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Key");

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
		TranslateMessage(&Message);		// WM_KEYDOWN 메시지가 발생했다면 WM_CHAR 메시지 발생시킨다.
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR str[256];		// 지역변수이므로 메시지가 발생할 때마다 초기화되기 때문에 static을 붙여 정적 변수로 선언
	int len;

	switch (iMessage) {
	case WM_CHAR:		// 키보드 입력 시점 : 문자열을 조립하기만 한다.
		// WM_CHAR에서 출력도 가능하나 다른 프로그램에 의해 언커버되어 지워졌을 때 다시 복구되지 않는다.
		if ((TCHAR)wParam == ' ') {		// Space가 입력될 경우 문자열을 지운다.
			str[0] = 0;
		}
		else {
			// 입력된 문자들을 바로 출력하지 않고 반드시 문자열에 모아 두어야 하는 이유는 키보드 입력이 발생하는 시점과 문자열을 출력해야 할 시점이 분리되어 있기 때문
			//------ wParam으로 전달된 문자 코드를 str 문자 배열에 저장한다.
			len = lstrlen(str);
			str[len] = (TCHAR)wParam;		// 문자열의 제일 끝 부분에 wParam값을 써넣는다.
			str[len + 1] = 0;		// 바로 뒤쪽에 0을 써 넣어 문자열 끝을 표시한다.
			//------ wParam으로 전달된 문자 코드를 str 문자 배열에 저장한다.
		}
		// BOOL InvalidRect(HWND hWnd, CONST RECT *lpRect, BOOL bErase) : 윈도우 작업영역을 무효화하여 운영체제로 하여금 WM_PAINT 메시지를 해당 윈도우로 보내도록 한다.
		// hWnd : 무효화의 대상이 되는 윈도우, 즉 다시 그려져야 할 윈도우의 핸들이다.
		// lpRect : 무효화할 사각영역을 지정하되 이 값이 NULL이면 윈도우의 전 영역이 무효화된다.
		// bErase : 무효화되기 전에 배경을 모두 지운 후 다시 그릴 것인지 아니면 배경을 지우지 않고 그릴 것인지를 지정한다. (TRUE : 배경을 지운 후 다시 그린다.	FALSE : 배경을 지우지 않고 다시 그린다.)
		InvalidateRect(hWnd, NULL, TRUE);		// 화면 갱신 함수(WM_PAINT 메시지 발생)
		return 0;
	case WM_PAINT:		// 출력 처리
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}