#include<Windows.h>
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Mless");
int x;		// 좌표를 기억하는 값
int y;
TCHAR str[128];		// 출력할 문자열
HWND hMDlg;

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
		// IsDialogMessage라는 함수는 메시지가 대화상자를 위한 메시지인지 검사해 보고 만약 그렇다ㅏ면 대화상자로 이 메시지를 보내는 일을 한다.
		// 단, 이런 처리는 모델리스 대화상자가 떠 있을 때만 해야 하며 hMDlg가 유효한 윈도우가 아니면 아무 것도 할 필요가 없다. 만약 모델리스 대화상자가 여러 개 있다면 그 수 만큼 똑같은 조건문을 반복해서 써야 한다.
		if (!IsWindow(hMDlg) || !IsDialogMessage(hMDlg, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return(int)Message.wParam;
}

BOOL CALLBACK MlessDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_STR, str);
		SetDlgItemInt(hDlg, IDC_X, x, FALSE);
		SetDlgItemInt(hDlg, IDC_Y, y, FALSE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_CHANGE:
			GetDlgItemText(hDlg, IDC_STR, str, 128);
			x = GetDlgItemInt(hDlg, IDC_X, NULL, FALSE);
			y = GetDlgItemInt(hDlg, IDC_Y, NULL, FALSE);
			InvalidateRect(hWndMain, NULL, TRUE);
			return TRUE;
		case IDCANCEL:		// 타이틀 바에 있는 닫기(X) 버튼의 ID
		case ID_CLOSE:
			DestroyWindow(hMDlg);		// 대화상자를 만들 때 CreateDialog 함수를 사용했으므로 대화상자를 닫을 때는 DestroyWindow 함수를 사용해야 한다.
			hMDlg = NULL;		// 대화상자를 파괴한 후 대화상자의 윈도우 핸들에 NULL을 대입하여 다시 대화상자를 만들수 있도록 해야 한다. 그래야 IsWindow 함수가 FALSE를 리턴할 것이다.
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_LBUTTONDOWN:

		// BOOL IsWindow(HWND hWnd);
		// hWnd가 유효한 윈도우 핸들인지 조사해 보고 윈도우가 존재하면 TRUE를 리턴하고 그렇지 않으면 FALSE를 리턴한다.
		if (!IsWindow(hMDlg)) {		// 대화상자가 두 번 만들어지지 않게 하기 위해서 대화상자를 만들기 전에 대화상자의 윈도우 핸들을 조사해 보고 이 핸들이 유효한 윈도우 핸들인지를 먼저 점검한다.
			// DialogBox 함수는 모달형의 대화상자를 만들고 실행하는 모든 일을 다 관장하며 대화상자를 종료하기 전에는 리턴하지 않는 특성을 가지고 있으므로 이 함수를 사용해서는 모델리스형 대화상자를 만들 수 없다.
			// CreaateDialog 함수는 DialogBox와 동일한 인수를 사용하되 대화상자를 만들기만 하며 만든 즉시 대화상자의 핸들을 리턴하고 대화상자를 운용하지는 않는다.
			hMDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MlessDlgProc);
			ShowWindow(hMDlg, SW_SHOW);		// 디폴트로 대화상자는 WS_VISLBE 스타일을 가지지 않으므로 ShowWindow 함수를 호출하지 않으면 대화상자가 만들어지기만 할 뿐 화면으로는 보이지 않는다.
		}
		return 0;
	case WM_CREATE:
		hWndMain = hWnd;
		x = 100;
		y = 100;
		lstrcpy(str, TEXT("String"));
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, x, y, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}