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

#define ID_COMBOBOX 100
const TCHAR* Items[] = { TEXT("Apple"), TEXT("Orange"), TEXT("Melon"), TEXT("Grape"), TEXT("Strawberry") };		// 콤보박스의 항목 리스트
HWND hCombo;		// 콤보박스의 핸들

// 콤보 박스는 에디트 컨트롤과 리스트 박스를 결합해 놓은 컨트롤이다.

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		// CBS_DROPDOWN : 에디트와 리스트 박스를 가진다.
		hCombo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 10, 10, 100, 200, hWnd, (HMENU)ID_COMBOBOX, g_hInst, NULL);		// 콤보 박스 생성
		for (i = 0; i < 5; i++) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)Items[i]);		// 콤보 박스에 항목 다섯 개 추가
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_COMBOBOX:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:		// 콤보 박스 리스트 항목 중 하나가 선택되었을 경우
				i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);		// 항목의 인덱스를 읽어온다.
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)str);		// 인덱스의 TEXT값을 읽어온다.
				SetWindowText(hWnd, str);		// 윈도우의 타이틀 캡션을 변경한다.
				break;
			case CBN_EDITCHANGE:		// 콤보 박스 에디트 박스에 값이 변경되었을 경우
				GetWindowText(hCombo, str, 128);		// 텍스트 값을 읽어온다.
				SetWindowText(hWnd, str);
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}