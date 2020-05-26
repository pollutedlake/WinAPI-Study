#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("SimplePaint");

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
	WndClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;		// 더블클릭 메시지를 받기 위해 CS_DBLCLKS 추가
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

enum {
	ID_R1 = 101,
	ID_R2,
	ID_R3
};
HWND red, blue, yellow, clear, thickness;
COLORREF Color = RGB(0, 0, 0);
BOOL bThick = FALSE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	//------ 현재 마우스의 위치를 저장할 변수 x, y
	static int x;
	static int y;
	//------ 현재 마우스의 위치를 저장할 변수 x, y
	static BOOL bNowDraw = FALSE;		// 현재 선을 그리고 있는 중인지를 저장할 변수
	HPEN MyPen, OldPen;

	switch (iMessage) {
	case WM_CREATE:
		CreateWindow(TEXT("button"), TEXT("Color"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 5, 120, 110, hWnd, (HMENU)0, g_hInst, NULL);
		red = CreateWindow(TEXT("button"), TEXT("RED"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 20, 100, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		blue = CreateWindow(TEXT("button"), TEXT("BLUE"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 50, 100, 30, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		yellow = CreateWindow(TEXT("button"), TEXT("YELLOW"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 80, 100, 30, hWnd, (HMENU)ID_R3, g_hInst, NULL);
		thickness = CreateWindow(TEXT("button"), TEXT("굵게"), WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 145, 5, 60, 55, hWnd, (HMENU)1, g_hInst, NULL);
		clear = CreateWindow(TEXT("button"), TEXT("CLEAR"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 225, 5, 60, 55, hWnd, (HMENU)2, g_hInst, NULL);
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_R1:
			Color = RGB(255, 0, 0);
			break;
		case ID_R2:
			Color = RGB(0, 0, 255);
			break;
		case ID_R3:
			Color = RGB(255, 255, 0);
			break;
		case 1:
			if (SendMessage(thickness, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(thickness, BM_SETCHECK, BST_CHECKED, 0);
				bThick = TRUE;
			}
			else {
				SendMessage(thickness, BM_SETCHECK, BST_UNCHECKED, 0);
				bThick = FALSE;
			}
			break;
		case 2:
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;
	case WM_LBUTTONDOWN:		// 왼쪽 마우스 버튼이 눌려지면 발생하는 메시지
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		bNowDraw = TRUE;
		return 0;
	case WM_MOUSEMOVE:			// 마우스를 움직였을 때 발생하는 메시지
		if (bNowDraw == TRUE) {
			hdc = GetDC(hWnd);		// WM_PAINT 메시지 이외의 부분에서 화면에 출력해야 할 때는 GetDC 함수를 호출하여 DC 핸들을 발급받아야 한다.
			if (!bThick) {
				MyPen = CreatePen(PS_SOLID, 1, Color);
			}
			else {
				MyPen = CreatePen(PS_SOLID, 10, Color);
			}
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, x, y, NULL);		// 직전 위치 설정
			x = LOWORD(lParam);		// 다음번 그리기를 위해 현재 위치를 저장해 놓는다.
			y = HIWORD(lParam);
			LineTo(hdc, x, y);		// 직전 위치에서 현재 위치까지 선 그리기
			SelectObject(hdc, OldPen);
			DeleteObject(MyPen);
			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_LBUTTONUP:			// 왼쪽 마우스 버튼이 눌려졌다가 떼졌을 때 발생하는 메시지
		bNowDraw = FALSE;		// 마우스가 움직여도 선이 그려지지 않게 FALSE로 변경
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDBLCLK:		// 마우스 왼쪽 버튼을 더블클릭 했을 때 발생하는 메시지
		InvalidateRect(hWnd, NULL, TRUE);		// WM_PAINT를 처리하지 않으므로 무효영역이 생기면 DefWindowProc이 WM_PAINT 메시지를 처리하며 이 함수는 배경색으로 윈도우를 지운다.
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}