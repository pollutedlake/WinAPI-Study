#include<Windows.h>]
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Bitmap");

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

// 윈도우즈는 비트맵을 곧바로 화면 DC로 출력하는 함수는 제공하지 않는다. 왜냐하면 비트맵은 크기가 큰 데이터 덩어리이며 따라서 출력 속도가 형편없이 느리고 화면으로 곧바로 출력할 경우 여러 가지 꼴사나운 현상이 발생할 수 있기 때문이다.
// 비트맵은 화면에 직접 출력할 수 없으며 반드시 메모리 DC에 미리 준비해 놓고 사용해야 한다.
// 메모리 DC란 화면 DC와 동일한 특성을 가지며 그 내부에 출력 표면을 가진 메모리 영역이다.
// 더블 버퍼링 : 메모리 DC에 먼저 그림을 그린 후 사용자 눈에 그려지는 과정은 보여주지 않고 그 결과만 화면으로 고속 복사하는 방법
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, OldBitmap;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// HDC CreateCompatibleDC(HDC hdc); : 인수로 화면 DC의 핸들을 주면 화면 DC와 동일한 특성을 가지는 DC를 메모리에 만들어 그 핸들을 리턴한다.
		// 동일한 특성을 가진다(=호환된다)는 말은 사용하는 색상 수, 색상면이 같다는 뜻이다. 호환되지 않는 DC끼리는 정보를 공유할 수 없기 때문에 화면 DC와 호환되는 메모리 DC를 만들어야 한다.
		MemDC = CreateCompatibleDC(hdc);

		// HBITMAP LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName); : 비트맵을 읽어온다
		// hInstance : 비트맵 리소스를 가진 인스턴스의 핸들
		// lpBitmapName : 비트맵 리소스의 이름
		MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);

		// BOOL BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop); : 메모리 DC의 표면에 그려져 있는 비트맵을 화면 DC로 고속 복사함으로써 비트맵을 화면으로 출력한다.
		// hdcDest : 복사 대상 DC
		// nXDest, nYDest, nWidth, nHeight : 복사 대상의 XYWH
		// hdcSrc : 복사원의 DC\
		// nXSrc, nYSrc : 복사원의 좌표이되 BitBlt은 비트맵의 크기를 변경하지 않고 복사를 수행하므로 폭과 높이는 복사 대상에서 한 번만 지정하고 복사원에서는 이 값을 그대로 사용한다.
		// dwRop : 래스터 연산 방법
		BitBlt(hdc, 0, 0, 123, 160, MemDC, 0, 0, SRCCOPY);

		// BOOL StretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop); : DC간에 비트맵을 전송하는데 확대 및 축소가 가능하다.
		// 인수를 보면 복사 대상과 복사원이 모드 포고가 높이를 가지고 있다. 복사원의 지정한 영역이 복사 대상의 지정한 영역의 크기만큼 확대, 축소되어 출력된다.
		StretchBlt(hdc, 123, 0, 369, 320, MemDC, 0, 0, 123, 160, SRCCOPY);

		// 비트맵 출력이 끝난 후에는 비트맵 자체와 메모리 DC를 해제해야 한다.
		SelectObject(MemDC, OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}