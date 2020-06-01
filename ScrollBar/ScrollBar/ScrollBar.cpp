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

// 다른 컨트롤들은 자신에게 변화가 있을 때 부모 윈도우로 WM_COMMAND 통지 메시지를 보내는데 비해 스크롤 바는 WM_HSCROLL(수평일 경우), WM_VSCROLL(수직일 경우)이라는 별도의 메시지를 부모 윈도우로 보낸다.
// 이 메시지들은 단순한 사건 발생뿐만 아니라 스크롤 상태에 대한 추가 정보를 같이 보내야 하는데 WM_COMMAND의 파라미터는 이미 용도가 예약되어 있어 이 정보를 보낼 수 없다.

#define ID_SCRRED 100
#define ID_SCRGREEN 101
#define ID_SCRBLUE 102
HWND hRed, hGreen, hBlue;		// Red, Green, Blud의 스크롤 바 핸들
int Red, Green, Blue;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;
	int TempPos;

	switch (iMessage) {
	case WM_CREATE:
		// 스크롤 바는 "scrollbar"윈도우 클래스로 생성하며 수평 스크롤 바일 경우 SBS_HORZ 스타일을, 수직 스크롤 바일 경우는 SBS_VERT 스타일을 지정한다.
		hRed = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, 10, 400, 20, hWnd, (HMENU)ID_SCRRED, g_hInst, NULL);		// Red, Green, Blue의 스크롤 바 생성
		hGreen = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, 40, 400, 20, hWnd, (HMENU)ID_SCRGREEN, g_hInst, NULL);
		hBlue = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, 70, 400, 20, hWnd, (HMENU)ID_SCRBLUE, g_hInst, NULL);

		// BOOL SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw); : 스크롤 바의 범위를 지정한다.
		// hWnd : 스크롤 바의 윈도우 핸들
		// nBarr : 메인 윈도우에 부착된 표준 스크롤 바 또는 별도의 스크롤 바 컨트롤을 지정하는데 이 값이 SB_CTL이면 별도의 컨트롤을 지정한다.
		// nMinPos, nMaxPos : 최솟값, 최댓값을 지정한다.
		SetScrollRange(hRed, SB_CTL, 0, 255, TRUE);

		// int SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bReDraw); : 스크롤 바의 현재 위치값을 지정한다.
		// nPos : 스크롤 바의 현재 위치
		SetScrollPos(hRed, SB_CTL, 0, TRUE);
		SetScrollRange(hGreen, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hGreen, SB_CTL, 0, TRUE);
		SetScrollRange(hBlue, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hBlue, SB_CTL, 0, TRUE);
		return 0;
	case WM_HSCROLL:		// 스크롤 바에 변화가 생김.
		if ((HWND)lParam == hRed) {		// lParam : 스크롤 바의 윈도우 핸들
			TempPos = Red;
		}
		if ((HWND)lParam == hGreen) {
			TempPos = Green;
		}
		if ((HWND)lParam == hBlue) {
			TempPos = Blue;
		}
		switch (LOWORD(wParam)) {		// LOWORD(wParam) : 스크롤 바 내의 어디를 눌렀는가?
		case SB_LINELEFT:		// 사용자가 왼쪽 화살표 버튼을 눌렀다는 뜻이며 이때는 왼쪽으로 한 단위 스크롤시킨다.
			TempPos = max(0, TempPos - 1);
			break;
		case SB_LINERIGHT:		// 사용자가 오른쪽 화살표 버튼을 눌렀다는 뜻이며 이때는 오른쪽으로 한 단위 스크롤시킨다.
			TempPos = min(255, TempPos + 1);
			break;
		case SB_PAGELEFT:		// 사용자가 왼쪽 몸통 부분을 눌렀다는 뜻이며 이떄는 한 페이지 왼쪽으로 스크롤시킨다.
			TempPos = max(0, TempPos - 10);
			break;
		case SB_PAGERIGHT:		// 사용자가 오른쪽 몸통 부분을 눌렀다는 뜻이며 이떄는 한 페이지 오른쪽으로 스크롤시킨다.
			TempPos = min(255, TempPos + 10);
			break;
		case SB_THUMBTRACK:		// 스크롤 박스를 드래그하고 있는 중이다. 이 메시지는 마우스 버튼을 놓을 때까지 계속 전달된다.
			TempPos = HIWORD(wParam);		// HIWORD(wParam) : 현재 위치
			break;
		}
		if ((HWND)lParam == hRed) {
			Red = TempPos;
		}
		if ((HWND)lParam == hGreen) {
			Green = TempPos;
		}
		if ((HWND)lParam == hBlue) {
			Blue = TempPos;
		}
		SetScrollPos((HWND)lParam, SB_CTL, TempPos, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MyBrush = CreateSolidBrush(RGB(Red, Green, Blue));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, 10, 100, 410, 300);
		SelectObject(hdc, OldBrush);
		DeleteObject(MyBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}