#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MessageBox");

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
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		// int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) : 메시지 박스 출력 함수
		// lpText : 메시지 박스에 출력할 문자열
		// lpCaption : 메시지 박스의 타이틀 바에 나타날 제목 문자열
		// uType : 메시지 박스에 어떤 종류의 버튼이 나타날 것인가를 지정하는 플래그들과 아이콘을 출력하는 플래그들
		// 리턴값으로 사용자가 누른 버튼값을 돌려준다.
		MessageBox(hWnd, TEXT("마우스 왼쪽 버튼을 눌렀습니다."), TEXT("메시지 박스"), MB_OK);
		if (MessageBox(hWnd, TEXT("게임을 계속 하겠습니까"), TEXT("질문"), MB_YESNO) == IDYES) {
			// 게임 계속 처리
		}
		else {
			// 게임 중지
		}
		// BOOL MessageBeep(UINT uType) : 비프음을 내는 종류의 출력 함수
		MessageBeep(MB_OK);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}