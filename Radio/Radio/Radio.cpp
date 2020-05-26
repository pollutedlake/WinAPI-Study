#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Radio");

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

enum {		// 라디오 버튼의 ID
	ID_R1 = 101,
	ID_R2,
	ID_R3,
	ID_R4,
	ID_R5,
	ID_R6
};
HWND r1, r2, r3, r4, r5, r6;		// 라디오 버튼 핸들들
int Graph = 0;	// 선택된 도형
COLORREF Color = RGB(0, 0, 0);		// 선택된 색

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		// 라디오 버튼은 여러 가지 선택 사항 중 한 가지만 선택할 필요가 있을 때 사용한다. 그래서 하나의 선택 사항에 대해 여러 개의 라디오 버튼들이 그룹을 이루어 사용된다는 특징이 있다.
		// 어떤 라디오 버튼들이 같은 그룹에 속하는지를 사용자에게 표시하기 위해 그룹 박스 컨트롤이 사용되는데 그룹 박스는 BS_GROUPBOX 스타일을 가지는 버튼의 일종이지만 화면으로 보여지기만 할 뿐 사용자의 입력을 받아들이거나 어떤 기능을 가지는 것은 아니다.
		CreateWindow(TEXT("button"), TEXT("Graph"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 5, 120, 110, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("Color"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 155, 5, 120, 110, hWnd, (HMENU)1, g_hInst, NULL);
		// 그룹을 이루는 첫 번째 라디오 버튼에만 WS_GROUP 스타일을 주고 나머지 라디오 버튼은 WS_GROUP 스타일을 주지 않으면 라디오 버튼의 그룹을 구성할 수 있다.
		r1= CreateWindow(TEXT("button"), TEXT("Rectangle"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, 20, 100, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);		
		r2= CreateWindow(TEXT("button"), TEXT("Ellipse"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 50, 100, 30, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		r3= CreateWindow(TEXT("button"), TEXT("Line"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 80, 100, 30, hWnd, (HMENU)ID_R3, g_hInst, NULL);
		r4= CreateWindow(TEXT("button"), TEXT("Black"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 150, 20, 100, 30, hWnd, (HMENU)ID_R4, g_hInst, NULL);
		r5= CreateWindow(TEXT("button"), TEXT("Red"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 150, 50, 100, 30, hWnd, (HMENU)ID_R5, g_hInst, NULL);
		r6= CreateWindow(TEXT("button"), TEXT("Blue"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 150, 80, 100, 30, hWnd, (HMENU)ID_R6, g_hInst, NULL);

		// BOOL CheckRadioButton(HWND hDlg, int nIDFirst, int nIDLast, int nIDCheck);
		// hDlg : 라디오 버튼을 가지는 부모 윈도우의 핸들
		// nIDFirst : 그룹의 시작 버튼
		// nIDLast : 그룹의 끝 버튼
		// nIDCheck : 선택될 버튼의 ID
		CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);
		CheckRadioButton(hWnd, ID_R4, ID_R6, ID_R4);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_R1:
			Graph = 0;
			break;
		case ID_R2:
			Graph = 1;
			break;
		case ID_R3:
			Graph = 2;
			break;
		case ID_R4:
			Color = RGB(0, 0, 0);
			break;
		case ID_R5:
			Color = RGB(255, 0, 0);
			break;
		case ID_R6:
			Color = RGB(0, 0, 255);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MyBrush = CreateSolidBrush(Color);
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		switch (Graph) {
		case 0:
			Rectangle(hdc, 10, 200, 200, 300);
			break;
		case 1:
			Ellipse(hdc, 10, 200, 200, 300);
			break;
		case 2:
			MoveToEx(hdc, 10, 200, NULL);
			LineTo(hdc, 200, 300);
			break;
		}
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