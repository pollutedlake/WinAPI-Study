#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("KeyDown");

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
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	// 문자 'A'를 출력할 좌표값을 기억해야 하므로 static 변수로 선언
	static int x = 100;
	static int y = 100;
	static bool isSpDown = false;		// Space키가 눌렸는지 확인하는 변수

	switch (iMessage) {
	case WM_KEYDOWN:
		switch (wParam) {		// wParam을 읽어 이동키일 경우 x, y를 커서 이동키의 방향에 따라 조정함으로써 'A'를 화면에서 이동시킨다.
		case VK_LEFT:
			x -= 8;
			break;
		case VK_RIGHT:
			x += 8;
			break;
		case VK_UP:
			y -= 8;
			break;
		case VK_DOWN:
			y += 8;
			break;
		case VK_SPACE:		// Space키가 눌렸다면 isSpDown을 반대로 바꾼다.
			isSpDown = !isSpDown;
		}
		InvalidateRect(hWnd, NULL,TRUE);		// 조정된 좌표가 화면에 반영되도록 InvalidRect 함수를 호출
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (isSpDown) {		// Space키가 눌렸다면 A와 #문자로 토글한다.
			TextOut(hdc, x, y, TEXT("#"), 1);
		}
		else {
			TextOut(hdc, x, y, TEXT("A"), 1);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}