#include<Windows.h>
// 메뉴 항목의 ID는 정수로 정의하되 사람이 일일이 정수값을 기억하기 곤란하므로 매크로 상수를 사용하여 ID를 정의하고 메뉴 편집기는 사용자가 입력한 ID를 정수 매크로로 정의하여 resource.h를 작성한다.
// 그래서 우리는 속성 편집기에서 메뉴의 ID를 문자열 형태의 매크로 상수로 입력하고 resource.h만 인클루드하면 된다.
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Menu");

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
	// 숫자로 정의된 메뉴 이름을 문자열 형태로 바꾸기 위해 MAKEINTRESOURCE 매크로를 사용한다. 
	// 윈도우에서 사용할 메뉴에 관한 정보를 가지는 lpszMenuName은 문자열 변수이되 우리가 작성한 메뉴의 ID는 정수로 되어 있기 때문이다.
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);		
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
	switch (iMessage) {
	case WM_COMMAND:		// 메뉴 항목을 선택할 경우 WM_COMMAND라는 메시지가 전달된다.
		// 어떤 메뉴 항목이 선택되었는가는 wParam의 하위 워드로 전달되므로 LOWORD(wParam)을 읽어 판단할 수 있다.
		// lParam : 통지 메시지를 발생시킨 컨트롤의 윈도우 핸들
		// LOWORD(wParam) : 메뉴나 액셀러레이터, 컨트롤의 ID
		// HIWORD(wParam) : 컨트롤이 보내는 통지 메시지, 메뉴가 선택된 경우는 0이 되며 액셀러레이터가 선택된 경우는 1이 된다.
		switch (LOWORD(wParam)) {
		case ID_FILE_MENU1:
			MessageBox(hWnd, TEXT("첫 번째 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_MENU2:
			MessageBox(hWnd, TEXT("두 번째 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);	// 메인 윈도우를 파괴함으로써 프로그램을 종료한다.
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}