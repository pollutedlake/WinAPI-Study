#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("BkMode");

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT hFont, OldFont;
	const TCHAR* str = TEXT("폰트 Test 1234");
	HBRUSH MyBrush, OldBrush;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MyBrush = CreateHatchBrush(HS_CROSS, RGB(0, 0, 255));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, 50, 50, 400, 200);
		SelectObject(hdc, OldBrush);
		hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		OldFont = (HFONT)SelectObject(hdc, hFont);

		// COLORREF SetTextColor(HDC hdc, COLORRREF crColor); : Text의 Color를 Set하는 함수
		SetTextColor(hdc, RGB(255, 0, 0));

		// COLORREF SetBkColor(HDC hdc, COLORREF crColor); : 글자 뒤쪽의 배경 색상을 지정하는 함수
		SetBkColor(hdc, RGB(255, 255, 0));
		TextOut(hdc, 100, 100, str, lstrlen(str));

		// int SetBkMode(HDC hdc, int iBkMode); : 배경 생상을 사용할 방법을 설정한다. OPAQUE와 TRANSPARENT 두 가지 옵션 중 한 가지를 선택할 수 있으며 디폴트는 OPAQUE이다.
		// OPAQUE : 불투면한 배경을 사용한다. 그래서 배경 색상에 의해 뒷쪽의 그림이 지워진다. 이 모드가 디폴트이다.
		// TRANSPARENT : 투명한 배경색상을 사용한다. 그래서 문자를 출력한 후에도 배경이 바뀌지 않는다. 즉 문자 획 사이의 여백에 있는 원래 배경이 지워지지 않는다.
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 100, 150, str, lstrlen(str));

		SelectObject(hdc, OldFont);
		DeleteObject(MyBrush);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}