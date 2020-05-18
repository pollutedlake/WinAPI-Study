#include<Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	HACCEL hAccel;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDC_ROPMODE);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROPMODE));
	while (GetMessage(&Message, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return(int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	// sx, sy : 처음 마우스를 누른 위치를 가진다.
	// oldx, oldy : 지워져야 할 선의 끝 좌표를 가진다.
	// bNowDraw : 현재 선을 그리고 있는 중인가에 대한 정보를 가진다.
	static int sx, sy, oldx, oldy;
	static BOOL bNowDraw = FALSE;
	static int drawObject = -1;
	static HBRUSH OldBrush;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;

	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_CURVE:
			drawObject = 0;
			break;
		case ID_LINE:
			drawObject = 1;
			break;
		case ID_ELLIPSE:
			drawObject = 2;
			break;
		case ID_RECTANGLE:
			drawObject = 3;
			break;
		}
	case WM_LBUTTONDOWN:
		if (drawObject != -1) {
			sx = LOWORD(lParam);
			sy = HIWORD(lParam);
			oldx = sx;
			oldy = sy;
			GetClientRect(hWnd, &rt);
			if (sx > rt.left&& sx < rt.right && sy > rt.top&& sy < rt.bottom) {
				bNowDraw = TRUE;
			}
		}
		return 0;
	case WM_MOUSEMOVE:
		if (bNowDraw) {
			hdc = GetDC(hWnd);
			OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

			// int SetROP2(HDC hdc, int fnDrawMode); : 그리기 모드를 설정한다.
			// hdc : 그리기 모드를 변경하고자 하는 DC의 핸들
			// fnDrawMode : 그리기 모드 값
			SetROP2(hdc, R2_NOT);
			switch (drawObject) {
			case 0:
				MoveToEx(hdc, sx, sy, NULL);
				sx = LOWORD(lParam);
				sy = HIWORD(lParam);
				LineTo(hdc, sx, sy);
				break;
			case 1:
				MoveToEx(hdc, sx, sy, NULL);
				LineTo(hdc, oldx, oldy);
				oldx = LOWORD(lParam);
				oldy = HIWORD(lParam);
				MoveToEx(hdc, sx, sy, NULL);
				LineTo(hdc, oldx, oldy);
				break;
			case 2:
				Ellipse(hdc, sx, sy, oldx, oldy);
				oldx = LOWORD(lParam);
				oldy = HIWORD(lParam);
				Ellipse(hdc, sx, sy, oldx, oldy);
				break;
			case 3:
				Rectangle(hdc, sx, sy, oldx, oldy);
				oldx = LOWORD(lParam);
				oldy = HIWORD(lParam);
				Rectangle(hdc, sx, sy, oldx, oldy);
				break;
			}
			SelectObject(hdc, OldBrush);
			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		if (bNowDraw) {
			bNowDraw = FALSE;
			hdc = GetDC(hWnd);
			OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			switch (drawObject) {
			case 0:
				MoveToEx(hdc, sx, sy, NULL);
				sx = LOWORD(lParam);
				sy = HIWORD(lParam);
				LineTo(hdc, sx, sy);
				break;
			case 1:
				MoveToEx(hdc, sx, sy, NULL);
				LineTo(hdc, oldx, oldy);
				break;
			case 2:
				Ellipse(hdc, sx, sy, oldx, oldy);
				break;
			case 3:
				Rectangle(hdc, sx, sy, oldx, oldy);
				break;
			}
			SelectObject(hdc, OldBrush);
			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}