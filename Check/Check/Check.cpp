#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Check");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
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
	static HWND c1, c2, c3, c4;		// 4가지 종류의 체크 박스를 만들 윈도우 핸들 변수 네 개
	static BOOL bEllipse = FALSE;		// WM_PAINT에서 bEllipse 변수갑셍 따라 타원 또는 사각형을 그린다.

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;

		// c1의 선택 상태에 따라 사각형이 그려지기도 하고 타원형이 그려지기도 한다. 체크 박스가 눌러지는 즉시 이 체크 박스를 부모 윈도우로 BN_CLICKED 통지 메시지를 보낸다ㅏ/
		c1 = CreateWindow(TEXT("button"), TEXT("Draw Ellipse?"), WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 20, 20, 160, 25, hWnd, (HMENU)0, g_hInst, NULL);

		// 선택 상태를 변경하는 시점에서는 아무런 동작도 하지 않지만 이 체크 박스가 선택되어 있으면 프로그램 종료 전에 메시지 박스르 출력한다. 체크 상태를 토글하ㅏ는데 별다ㅏ른 조건이 없고 부모가 개입할 필요가 없으므로 자동 체크 박스로 만들었다.
		c2 = CreateWindow(TEXT("button"), TEXT("Good bye Message?"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 50, 160, 25, hWnd, (HMENU)1, g_hInst, NULL);

		// 세 번째, 네 번째 체크 박스는 세 가지 상태를 가지는 체크 박스의 모양을 살펴보기 위해 만들어만 놓았으며 기능은 가지고 있지 않다.
		c3 = CreateWindow(TEXT("button"), TEXT("3State"), WS_CHILD | WS_VISIBLE | BS_3STATE, 20, 80, 160, 25, hWnd, (HMENU)2, g_hInst, NULL);
		c4 = CreateWindow(TEXT("button"), TEXT("Auto 3State"), WS_CHILD | WS_VISIBLE | BS_AUTO3STATE, 20, 110, 160, 25, hWnd, (HMENU)3, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		// 컨트롤은 자신에게 어떤 변화가 있을 때마다 부모 윈도우로 메시지를 보내며 이 메시지를 통지 메시지라고 부른다.
		// 체크 박스의 경우 사용자가 마우스로 클릭할 때마다 부모 윈도우로 BN_CLICKED 메시지로 보낸다.
		// 컨트롤이 부모 윈도우로 보내는 통지 메시지와는 달리 부모 윈도우가 체크 박스의 현재 상태를 알아보거나 상태를 바꾸고자 할 때도 차일드 윈도우로 메시지를 보낸다. 통지 메시지는 차일드가 부모로 보내는 보고 메시지이고 그냥 메시지는 부모가 차일드에게 어떤 지시를 내리기 위해 보내는 명령이다.
		// 메시지의 종료는 컨트롤마다 다르며 부모 윈도우가 체크 박스로 보낼 수 있는 메시지에는 BM_GETCHEK, BM_SETCHEK 두 가지가 있다.
		// BM_GETCHECK : 체크 박스가 현재 체크되어 있는 상태인지를 조사하며 wParam, lParam은 사용하지 않는다. 체크 상태는 리턴값으로 돌려진다.
		// BM_SETCHECK : 체크 박스의 체크 상태를 변경하며 wParam에 변경할 체크 상태를 지정한다.
		// BM_GETCHECK에 의해 리턴되는 값, 또는 BM_SETCHECK에 의해 설정되는 체크 박스의 상태는 BST_UNCHECK, BST_CHECKED, BST_INDETERMINATE 세 가지가 있다.
		// BST_UNCHECKED : 현재 체크되어 있지 않다. 값은 0이다.
		// BST_CHECKED : 현재 체크되어 있다. 값은 1이다.
		// BST_INDETERMINATE : 체크도 아니고 안 체크도 아닌 상태. 값은 2이다.
		switch (LOWORD(wParam)) {
		case 0:		// 부모 윈도우는 0번 컨트롤인 Draw Ellipse 체크 박스가 눌러질 때마다 체크 박스의 상태를 조사해서 체크 상태를 토글하고 bEllipse변수의 값도 같이 토글시킨다.
			if (SendMessage(c1, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {	// c1의 상태를 조사하고 체크되어 있지 않다면 체크하라고 메시지를 보낸다.
				SendMessage(c1, BM_SETCHECK, BST_CHECKED, 0);
				bEllipse = TRUE;
			}
			else {		// 체크되어 있다면 체크 표시를 해제하라는 메시지를 보낸다.
				SendMessage(c1, BM_SETCHECK, BST_UNCHECKED, 0);
				bEllipse = FALSE;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:		// ID 0과 2는 수동 체크 박스이기 때문에 BN_CLICKED 통지 메시지가 발생했을 때 부모가 체크 상태를 변경해야 한다. 그렇지 않다면 아무리 눌러도 상태가 바뀌지 않는다.
			if (SendMessage(c3, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(c3, BM_SETCHECK, BST_CHECKED, 0);
			}
			else {
				if (SendMessage(c3, BM_GETCHECK, 0, 0) == BST_INDETERMINATE) {
					SendMessage(c3, BM_SETCHECK, BST_UNCHECKED, 0);
				}
				else {
					SendMessage(c3, BM_SETCHECK, BST_INDETERMINATE, 0);
				}
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (bEllipse == TRUE) {
			Ellipse(hdc, 200, 100, 400, 200);
		}
		else {
			Rectangle(hdc, 200, 100, 400, 200);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		if (SendMessage(c2, BM_GETCHECK, 0, 0) == BST_CHECKED) {
			MessageBox(hWnd, TEXT("Good Bye"), TEXT("Check"), MB_OK);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}