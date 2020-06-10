#include<Windows.h>
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("InfoDlg");
int x;		// 좌표를 기억하는 값
int y;
TCHAR str[128];		// 출력할 문자열

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

// HWND GetDlgItem(HWND hDlg, int nIDDlgItem); : 차일드 컨트롤의 ID를 알고 있으면 이 컨트롤의 윈도우 핸들을 구할 수 있다.
// hDlg : 대화상자의 핸들
// nIDDlgItem : 컨트롤의 ID
// 컨트롤의 윈도우 핸들을 리턴한다.

// int GetDlgCtrlID(HWND hwndCtrl); : 차일드 컨트롤의 윈도우 핸들로부터 ID를 구하는데 사용한다.

// 컨트롤은 윈도우이며 따라서 윈도우를 관리하기 위해서는 핸들이 필요하다. 그런데 핸들이라는 것은 그 특성상 운영체제가 일방적으로 발급하는 것이기 때문에 번호의 연속성이 없으며 그러다 보니 반복적인 처리에는 사용할 수 없다는 문제가 있다.
// 그래서 연속성을 가질 수 있고 사용자가 직접 번호를 지정할 수 있는 ID라는 보조 식별자가 필요한 것이다.

// LONG SendDlgItemMessage(HWND hDlg, int nID, UINT Msg, WPARAM wParam, LPARAM lParam); : SendMessage 함수로 차일드 컨트롤에 메시지를 보내는 방법은 대상 윈도우의 핸들을 요구한다. 만약 ID만 알고 있다면 이 함수로 부모 대화상자의 핸들과 컨트롤의 ID만으로 편리하게 메시지를 보낼 수 있다.
// hDlg 대화상자의 nID 컨트롤에게 Msg 메시지를 보내는데 GetDlgItem과 SendMessage를 호출하는 래퍼 함수라고 할 수 있다.

BOOL CALLBACK InfoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_INITDIALOG:		// 세 개의 컨트롤에 전역변수 값을 대입하여 사용자에게 현재의 좌표와 문자열을 보여준다.

		// BOOL SetDlgItemText(HWND hDlg, int nID, LPCTSTR lpString); : 컨트롤로 문자열을 출력하는 함수
		// hDlg : 대화상자의 윈도우 핸들
		// nID : 값을 쓸 컨트롤의 ID, 만약 컨트롤의 핸들만 알고 있다면 GetDlgCtrlID 함수로 ID를 조사하면 된다.
		// lpString : 대입하고자 하는 문자열
		SetDlgItemText(hDlg, IDC_STR, str);

		// BOOL SetDlgItem(HWND hDlg, int nID, UINT uValue, BOOL bSigned); : 컨트롤로 정수를 출력하는 함수
		// hDlg : 대화상자의 윈도우 핸들
		// nID : 값을 쓸 컨트롤의 ID, 만약 컨트롤의 핸들만 알고 있다면 GetDlgCtrlID 함수로 ID를 조사하면 된다.
		// uValue : 대입하고자 하는 정수값
		// bSigned : uValue값이 부호가 있는지 없는지를 지정한다. 이 값이 TRUE이고 uValue가 음수이면 음수 기호가 나타나고 FALSE이면 uValue는 부호없는 정수가 된다. 
		SetDlgItemInt(hDlg, IDC_X, x, FALSE);
		SetDlgItemInt(hDlg, IDC_Y, y, FALSE);
		return TRUE;
	case WM_COMMAND:		// 두 개의 컨트롤로부터 전달된 통지 메시지를 처리한다.
		switch (LOWORD(wParam)) {
		case IDOK:		// OK버튼이 전달되었을 경우는 사용자가 컨트롤에 입력해 놓은 값을 읽어 x, y, str 전역변수에 대입하였ㄷ다.

			// UINT GetDlgItemText(HWND hDlg, int nID, LPTSTR lpString, int nMaxCount); : 컨트롤로부터 문자열을 읽어온다.
			// hDlg : 대화상자의 윈도우 핸들
			// nID : 값을 읽을 컨트롤의 ID, 만약 컨트롤의 핸들만 알고 있다면 GetDlgCtrlID 함수로 ID를 조사하면 된다.
			// lpString : 문자열을 읽을 버퍼
			// nMaxCount : 버퍼의 길이
			GetDlgItemText(hDlg, IDC_STR, str, 128);

			// UINT GetDlgItemInt(HWND hDlg, int nID, BOOL *lpTranslated, BOOL bSigned); : 컨트롤로부터 정수값을 읽어온다.\
			// hDlg : 대화상자의 윈도우 핸들
			// nID : 값을 쓸 컨트롤의 ID, 만약 컨트롤의 핸들만 알고 있다면 GetDlgCtrlID 함수로 ID를 조사하면 된다.
			// lpTranslated : 입력된 정수를 읽어들일 때 숫자 이외에 문자가 있거나 숫자가 너무 클 경우 에러가 발생하는데 이럴 경우 세 번째 인수로 지정된 BOOL형 포인터에 에러가 있었는지 없었는지를 대입한다. 에러 검사를 할 필요가 없을 때는 세 번째 인수로 NULL값을 전달한다.
			// bSigned : TURE일 경우는 부호있는 정수값을 읽고 FALSE일 경우는 부호를 무시하고 무조건 양수로 읽는다.
			x = GetDlgItemInt(hDlg, IDC_X, NULL, FALSE);
			y = GetDlgItemInt(hDlg, IDC_Y, NULL, FALSE);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:	// Cancel 버튼이 전달되었을 경우는 사용자가 입력한 값을 무시하고 곧바로 대화상자를 종료해 버린다.
			EndDialog(hDlg, IDCANCEL);
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
		if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, InfoDlgProc) == IDOK) {		// 대화상자를 호출하고 OK버튼을 눌러 종료했을 때 다시 그린다.
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_CREATE:		// 좌표와 문자열을 초기화한다.
		hWndMain = hWnd;
		x = 100;
		y = 100;
		lstrcpy(str, TEXT("String"));
		return 0;
	case WM_PAINT:		// (x, y)좌표에 str 문자열을 출력한다.
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