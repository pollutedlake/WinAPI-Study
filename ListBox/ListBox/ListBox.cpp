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

#define ID_LISTBOX 100
const TCHAR* Items[] = { TEXT("Apple"), TEXT("Orange"), TEXT("Melon"), TEXT("Grape"), TEXT("Strawberry") };		// 리스트 박스의 항목들
HWND hList;		// 리스트 박스의 핸들

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		// LBS_NOTIFY : 사용자가 목록중 하나를 선택했을 때 부모 윈도우로 통지 메시지를 보낸다.
		hList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY, 10, 10, 100, 200, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);		// 리스트 박스 생성
		for (i = 0; i < 5; i++) {
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)Items[i]);		// LB_ADDSTRING : 리스트 박스에 항복을 추가한다. lParam으로 추가하고자 하는 문자열의 번지를 넘겨준다.
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:		// LBN_SELCHANGE : 사용자에 의해 선택이 변경되었다는 통지 메시지.
				i = SendMessage(hList, LB_GETCURSEL, 0, 0);		// LB_GETCURSEL : 현재 선택된 항목의 번호(Index)를 조사한다.
				SendMessage(hList, LB_GETTEXT, i, (LPARAM)str);		// LB_GETTEXT : 지정한 항목의 문자열을 읽는다. wParam에 항목 번호, lParam에 문자열 버퍼의 번지를 넘기면 버퍼에 문자열을 채워준다.
				SetWindowText(hWnd, str);		// 윈도우의 타이틀 캡션 변경
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