#include<Windows.h>
#include "Resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("DlgCheck");
BOOL bMale;		// 성별을 나타내는 변수
enum tag_Major { MANAGE, KOREAN, ENGLISH, LAW } Major;		// 전공을 나타내는 열거형 변수
const TCHAR* arMajor[] = { TEXT("경영학과"), TEXT("국문학과"), TEXT("영문학과"), TEXT("법학과") };

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

// 라디오 버튼이 너무 많다면 루프를 돌며 각 버튼들에게 질문하여 체크된 버튼을 만나면 그 버튼의 id를 리턴하는 함수를 만들어서 사용할 수 있다.
// 단, 라디오 버튼들이 반드시 연속적인 ID를 가지고 있어야 하므로 resource.h 파일을 열어 ID 순서를 조정해야 하며 탭 순서도 ID 순서에 맞게 조정해야 그룹이 제대로 구성된다.
int GetCheckedRadioButton(HWND hDlg, int First, int Last) {
	int id;

	for (id = First; id < Last; id++) {
		if (IsDlgButtonChecked(hDlg, id) == BST_CHECKED) {
			return id;
		}
	}
	return -1;
}

BOOL CALLBACK MajorDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_INITDIALOG:

		// BOOL CheckDlgButton(HWND hDlg, int nIDButton, UINT uCheck); : 버튼의 체크 상태를 변경한다.
		// hDlg : 부모 윈도우의 핸들
		// nIDButton : 체크 상태를 변경할 버튼의 ID
		// uCheck : 원하는 체크 상태
		// = SendMessage(GetDlgItem(hDlg, nIDButton), BM_SETCHECK, uCheck, 0);
		// = SendDlgItemMessage(hDlg, nIDButton, BM_SETCHECK, uCheck, 0);
		CheckDlgButton(hDlg, IDC_MALE, bMale ? BST_CHECKED : BST_UNCHECKED);		// bMale 변수의 상태를 CheckDlgButton으로 IDC_MALE 체크 박스에 표시한다.
		CheckRadioButton(hDlg, IDC_MAJOR1, IDC_MAJOR4, IDC_MAJOR1 + Major);		// CheckRadioButton으로 Major 변수의 값을 IDC_MAJOR1 ~ IDC_MAJOR4 사이의 IDC_MAJOR1에 Major를 더한 라디오 버튼에 표시한다.
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:

			// UINT IsDlgButtonChecked(HWND hDlg, int nIDButton); : 버튼의 체크 상태를 읽어온다.
			// hDlg : 부모 윈도우의 핸들
			// nIDButton : 체크 상태를 읽어올 버튼의 ID
			// = SendMessage(GetDlgItem(hDlg, nIDButton), BM_GETCHECK, 0, 0);
			// = SendDlgItemMessage(hDlg, nIDButton, BM_GETCHECK, 0, 0);
			bMale = (IsDlgButtonChecked(hDlg, IDC_MALE) == BST_CHECKED);
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR1) == BST_CHECKED) {
				Major = MANAGE;
			}
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR2) == BST_CHECKED) {
				Major = KOREAN;
			}
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR3) == BST_CHECKED) {
				Major = ENGLISH;
			}
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR4) == BST_CHECKED) {
				Major = LAW;
			}
			// Major = tag_Major(GetCheckedRadioButton(hDlg, IDC_MAJOR1, IDC_MAJOR4) - IDC_MAJOR1);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
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
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:		// bMale, Major 초기화
		bMale = TRUE;
		Major = KOREAN;
		return 0;
	case WM_LBUTTONDOWN:		// 마우스 왼쪽 클릭 시 DialogBox호출하고 OK버튼을 누르면 화면을 다시 그린다. 
		if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MajorDlgProc) == IDOK) {
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:		// 성별과 전공 출력
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, TEXT("성별 = %s"), bMale ? TEXT("남자") : TEXT("여자"));
		TextOut(hdc, 10, 10, str, lstrlen(str));
		wsprintf(str, TEXT("전공 = %s"), arMajor[Major]);
		TextOut(hdc, 10, 30, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}