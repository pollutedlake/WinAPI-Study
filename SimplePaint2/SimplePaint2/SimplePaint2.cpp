#include <Windows.h>
#include "Resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK PaintDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
HWND g_hWnd;
LPCTSTR lpszClass = TEXT("SimplePaint2");

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

	g_hWnd = hWnd;

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

COLORREF Color = RGB(0, 0, 0);		// 펜의 칼라값
BOOL bThick = FALSE;		// 펜의 두꼐설정

BOOL CALLBACK PaintDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_INITDIALOG:		// DIALOG 시작하면 Color값과 bThick값에 따라 라디오버튼과 체크박스 표시하기
		CheckDlgButton(hDlg, IDC_THICKER, bThick);
		if (Color == RGB(255, 0, 0)) {
			CheckRadioButton(hDlg, IDC_RED, IDC_YELLOW, IDC_RED);
		}
		else if (Color == RGB(0, 0, 255)) {
			CheckRadioButton(hDlg, IDC_RED, IDC_YELLOW, IDC_BLUE);
		}
		else if (Color == RGB(255, 255, 0)) {
			CheckRadioButton(hDlg, IDC_RED, IDC_YELLOW, IDC_YELLOW);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CLEAR:		// Clear버튼을 누르면 화면은 지우나 Dialog 종료 안한다.
			InvalidateRect(g_hWnd, NULL, TRUE);
			return FALSE;
		case IDOK:		// OK버튼 누르면 설정한 색, 두께 저장하고 Dialog 종료
			if (IsDlgButtonChecked(hDlg, IDC_RED) == BST_CHECKED) {
				Color = RGB(255, 0, 0);
			}
			else if (IsDlgButtonChecked(hDlg, IDC_BLUE) == BST_CHECKED) {
				Color = RGB(0, 0, 255);
			}
			else if (IsDlgButtonChecked(hDlg, IDC_YELLOW) == BST_CHECKED) {
				Color = RGB(255, 255, 0);
			}
			else {
				Color = RGB(0, 0, 0);
			}
			if (IsDlgButtonChecked(hDlg, IDC_THICKER) == BST_CHECKED) {
				bThick = TRUE;
			}
			else {
				bThick = FALSE;
			}
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:		// Cancel 버튼을 누르면 설정한 값 저장하지 않고 종료
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	//------ 현재 마우스의 위치를 저장할 변수 x, y
	static int x;
	static int y;
	//------ 현재 마우스의 위치를 저장할 변수 x, y
	static BOOL bNowDraw = FALSE;		// 현재 선을 그리고 있는 중인지를 저장할 변수
	HPEN MyPen, OldPen;

	switch (iMessage) {
	case WM_LBUTTONDOWN:		// 왼쪽 마우스 버튼이 눌려지면 발생하는 메시지
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		bNowDraw = TRUE;
		return 0;
	case WM_RBUTTONDOWN:		// 마우스 우클릭 시 DIalog 호출
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_PAINT), hWnd, PaintDlgProc);
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