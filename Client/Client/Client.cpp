/*
파일 : Client.cpp
작성자 : 박상현
작성일 : 2020.03.05
*/
// 작업영역의 중앙에 텍스트를 출력하는 프로그램
// WM_PAINT에서 작업영역을 조사해도 결과는 같으나 WM_PAINT는 무효화된 윈도우를 다시 그리는 것만이 맡은 임무일 뿐이지 그리기 위한 정보를 조사하는 것은 임무가 아니기 때문에 일반적으로 그렇게 만들지 않음.
// WM_PAINT는 아주 빈번히 그리고 지속적으로 전달되는 메시지인데 이때마다 프로그램의 상태를 조사한다면 그리기 효율이 떨어진다. 그래서 나머지 메시지에서 그릴 정보를 미리 준비해 놓고 WM_PAINT는 그 정보를 읽어 그리기만 하는 것이 이상적이다.

// WM_MOVE 메시지 : 윈도우의 위치가 변경될 때마다 보내진다. lParam의 하위 워드에 윈도우의 새 X좌표, 상위 워드에 새 Y좌표가 전달된다.

#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Client");

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
	return(int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt;

	switch (iMessage) {
	case WM_SIZE:		// 윈도우의 크기가 변경될 때 발생하는 메시지, lParam의 하위 워드에는 변경된 후의 윈도우 폭이, 상위 워드에서는 높이가 전달되며 wParam에는 이 메시지가 발생한 이유를 나타내는 플래그가 전달된다.
		// BOOL GetClientRect(HWND hWnd, LPRECT lpRect);	<--- Client가 차지하고 있는 Rect를 Get하는 함수이다.
		// hWnd : 대상 윈도우 핸들
		// lpRect : 리턴값을 돌려받기 위한 RECT 구조체의 포인터, left, top은 원점이므로 항상 0이며 right, bottom에 우하단의 좌표가 대입된다.
		GetClientRect(hWnd, &rt);		// 작업영역의 크기를 구하는 함수
		// rt.bottom = HIWORD(lParam);
		// rt.bottom = LOWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextAlign(hdc, TA_CENTER);		// 정확하게 중앙이 되도록 하기 위해 문자열을 중앙 정렬
		TextOut(hdc, rt.right / 2, rt.bottom / 2, TEXT("Center String"), 13);		// 작업영역의 중앙 좌표를 구해 문자열을 줄력
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}