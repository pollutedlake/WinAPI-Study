#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("GraphOut");

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

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// COLORREF SetPixel(hdc, nXPos, nYPos, clrref) : (nXPos, nYPos) 좌표에 clrref 색상으로 화면에 점을 출력한다. DC의 색상정보를 쓰지 않는다.
		SetPixel(hdc, 10, 10, RGB(255, 0, 0));
		// DROWD MoveToEx(hdc, x, y, lpPoint) : CP를 지정한 좌표 (x, y)로 이동시키며 이동 전의 CP 좌표를 lpPoint에 대입하는데 이전 CP 값이 필요하지 않을 경우 NULL을 전달한다.
		MoveToEx(hdc, 50, 50, NULL);
		// BOOL LineTo(hdc, xEnd, yEnd) : CP에서부터 지정한 좌표까지 선을 그으며 CP를 끝점으로 이동시킨다.
		LineTo(hdc, 300, 90);
		// BOOL Rectangle(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect) : 직사각형 그리기
		Rectangle(hdc, 50, 100, 200, 180);
		// BOOL Ellipse(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect) : 직사각형에 외접하는 타원 그리기
		Ellipse(hdc, 220, 100, 400, 200);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}