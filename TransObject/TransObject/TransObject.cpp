#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

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
	int i;
	HBRUSH MyBrush, OldBrush;
	HPEN MyPen, OldPen;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 250; i += 5) {
			MoveToEx(hdc, 0, i, NULL);
			LineTo(hdc, 600, i);
		}

		// 빨간 펜 초록 브러시
		MyBrush = CreateSolidBrush(RGB(0, 255, 0));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		MyPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		OldPen = (HPEN)SelectObject(hdc, MyPen);
		Ellipse(hdc, 20, 20, 150, 150);

		// 빨간 펜 널 브러시
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, 220, 20, 350, 150);

		// 널 펜 초록 브러시
		SelectObject(hdc, MyBrush);
		SelectObject(hdc, GetStockObject(NULL_PEN));
		Ellipse(hdc, 420, 20, 550, 150);

		DeleteObject(SelectObject(hdc, OldBrush));
		DeleteObject(SelectObject(hdc, OldPen));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}