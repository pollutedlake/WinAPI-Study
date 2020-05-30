#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("EditWnd");

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

// 컨트롤이 윈도우라는 결정적인 증거는 스스로 메시지를 처리할 수 있는 능력을 가진다는 점이다. 윈도우에 적용할 수 있는 함수는 대부분 컨트롤에도 적용할 수 있다.

#define ID_EDIT 100
HWND hEdit;
int nTop = 10;
BOOL bShow = TRUE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	const TCHAR* Mes = TEXT("왼쪽 클릭 : 에디트 이동, 오른쪽 클릭 : 보임/숨김");

	switch (iMessage) {
	case WM_CREATE:
		hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 10, nTop, 200, 25, hWnd, (HMENU)ID_EDIT, g_hInst, NULL);		// 에디트 윈도우를 만든 후 윈도우 핸들을 리턴
		SetWindowText(hEdit, TEXT("에디트도 윈도우다."));		// 캡션을 변경하여 에디트에 문자열을 대입
		return 0;
	case WM_LBUTTONDOWN:		// 마우스 왼쪽 버튼을 누르면 에디트를 조금 아래로 이동시킨다.
		nTop += 10;
		MoveWindow(hEdit, 10, nTop, 200, 25, TRUE);
		return 0;
	case WM_RBUTTONDOWN:		// 마우스 오른쪽 버튼을 누르면 에디트를 숨기거나 보이게 한다.
		if (bShow) {
			bShow = FALSE;
			ShowWindow(hEdit, SW_HIDE);	
		}
		else {
			bShow = TRUE;
			ShowWindow(hEdit, SW_SHOW);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 200, 100, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}