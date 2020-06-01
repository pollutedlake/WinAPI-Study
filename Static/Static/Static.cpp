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

// 스태틱은 사용자로부터 입력을 받아들이는 기능은 없고 오로지 문자열을 부여주는 것이 기능의 전부이다. 주로 에디트나 다른 컨트롤 옆에 위치하며 컨트롤의 용도를 설명하는 역할을 한다. 스태틱 컨트롤을 만들 때는 윈도우 클래스를 "static"으로 지정한다.
// TexOut으로 출력한 문자열은 그냥 문자열일 뿐이므로 언제든지 지워질 수 있고 그래서 WM_PAINT에서 계속 복구해야 한다. 만면 스태틱 컨트롤은 스스로 메시지를 처리할 수 있는 윈도우이기 때문에 일단 배치해 놓기만 하면 더 이상 신경쓰지 않아도 된다.
// 또한 색상, 글꼴 크기 등 운영체제의 세팅이 바뀔 경우 이런 변화에 대해서도 스스로 대처한다는 장점이 있다.
// 컨트롤의 3요소 : 스타일, 통지 메시지, 부모 윈도우가 보내는 메시지

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_CREATE:
		CreateWindow(TEXT("static"), TEXT("Only Text"), WS_CHILD | WS_VISIBLE, 20, 20, 100, 25, hWnd, (HMENU)-1, g_hInst, NULL);		// static 컨트롤은 실행중에 부모 윈도우로 통지 메시지를 보낼 필요가 없기 때문에 ID를 -1로 지정하며 여러 개의 스태틱 컨트롤이 있을 경우 모두 -1의 같은 ID를 사용해도 상관없다. ID -1은 ID를 주지 않는다는 뜻이며 더 이상 이 컨트롤을 프로그래밍할 필요가 없을 때 사용한다. 스태틱은 그 자리에 있는 자체가 존재의 의미이며 다른 컨트롤과 구분할 필요조차도 없기 때문이다.
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}