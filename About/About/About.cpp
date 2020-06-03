#include<Windows.h>
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("About");

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

// 많은 양의 종보를 사용자로부터 효율적으로 입력받기 위해 주로 대화상자를 사용한다. 대화상자는 프로그램과 사용자간의 대화,곧 명령 및 정보 전달을 위한 특별한 윈도우이다. 대화상자에는 컨트롤들이 배치되기 때문에 컨트롤의 컨테이너라고도 한다.
// 대화상자는 동작 방식에 따라 크게 모달형과 모델리스형으로 나뉘어진다. 모달형은 대화상자를 닫기 전에 다른 윈도우로 전환할 수 없으며 반드시 대화상자를 닫아야 다른 윈도우로 전환할 수 있다. 대화가 종료되어야 다른 일을 할 수 있거나 대화의 내용에 따라 다음 할 일이 결정되는 경우 모달 대화상자가 사용되며 대부분의 대화상자는 모달형이다.
// 모델리스형은 대화상자를 열어 놓은 채로 다른 윈도우로 전환할 수 있는 대화상자이다. 프로그램의 상태를 나타내거나 작업을 하면서 참조해야 할 여러 가지 정보를 표시하는 윈도우가 모델리스형으로 만들어진다.
// 대화상자를 만들기 위해서는 기본적으로 다음 두가지가 있어야 한다.
// 대화상자 템플릿 : 대화상자의 모양과 대화상자 내의 컨트롤 배치 상태가 저장되는 이진 정보이며 리소스로 작성된다.
// 대화상자 프로시저 : 대화상자에서 발생하는 메시지를 처리한다.

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDOK:
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
	switch (iMessage) {
	case WM_LBUTTONDOWN:

		// int DialogBox(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent, DLGPROC lpDialogFunc); : 대화상자를 호출하는 함수이며 대화상자의 출력, 운영 및 종료까지 대화상자의 모든 처리를 다 책임진다.
		// hInstance : 대화상자 리소스를 가진 인스턴스의 핸들
		// lpTemplate : 대화상자 템플릿의 리소스 ID
		// hWndParent : 대화상자를 소유할 부모 윈도우
		// lpDialogFunc : 대화상자 프로시저의 이름
		// DialogBox는 hInstance의 리소스 중 lpTemplate가 지정하는 템플릿을 읽어와 대화상자를 만들어 화면에 출력한다. 이 때 템플릿에 기록된 차일드 컨트롤의 정보를 참조하여 포함된 컨트롤도 생성하여 배치한다.
		// 그리고 lpDialogFunc 함수로 대화상자 운용에 필요한 메시지를 보내 대화상자가 제데로 실행될 수 있도록 한다. lpDialogFunc 함수에서는 대화상자와 그 차일드들 간의 통신을 처리할 것이다.
		// 사용자가 대화상자를 닫으면 DialogBox 함수가 리턴하는데 이때 리턴되는 값은 대화상자의 종료 함수인 EndDialog 함수가 지정하는 값이다.
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}