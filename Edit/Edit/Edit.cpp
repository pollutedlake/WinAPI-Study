#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Title");

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

// 에디트는 "edit"윈도우 클래스로부터 생성하며 자신의 변화에 대해 통지 메시지를 부모 윈도우로 보낸다.
// EN_UPDATE는 문자열이 변경된 후 화면에 출력하기 직전에 보내는 메시지이며 EN_CHANGE는 문자열이 화면으로 출력되고 난 후 보내지는 메시지이다. 즉, 에디트는 문자열이 변경된 후 EN_UPDATE 메시지를 보내고 화면에 그린 후 다시 EN_CHANGE 메시지를 보낸다.

#define ID_EDIT 100
HWND hEdit;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:		// 에디트를 생성
		hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 10, 10, 200, 25, hWnd, (HMENU)ID_EDIT, g_hInst, NULL);		// ES_AUTOHSCROLL 스타일을 주어 수평 스크롤이 가능하도록 하였다. 이 스타일을 주지 않으면 에디트의 오른쪽 끝까지만 문자열을 입력할 수 있으나 이 스타일을 주면 에디트 끝부분에 닿았을 때 자동으로 수평 스크롤되어 자신의 폭보다 긴 문자열도 입력할 수 있다.
		return 0;
	case WM_COMMAND:		// EN_CHANGE 통지 메시지가 전달될 때 에디트의 텍스트를 읽어 부모 윈도우의 타이틀 바로 출력하였다.
		switch (LOWORD(wParam)) {
		case ID_EDIT:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				GetWindowText(hEdit, str, 128);		// 에디트에 입력된 텍스트를 읽는다.
				SetWindowText(hWnd, str);		// 윈도우의 타이틀 바로 출력한다.
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}