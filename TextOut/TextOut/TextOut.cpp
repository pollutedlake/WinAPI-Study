#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void MyTextOut(HDC hdc, int x, int y, LPCTSTR Text);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("TextOut");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParamn, int nCmdShow) {
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
	HDC hdc;
	PAINTSTRUCT ps;
	int Score = 85;

	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextAlign(hdc, TA_CENTER);		// SetTextAlign(HDC hdc, UINT fMode) : 문자열의 정렬 방법 변경 함수
		// fMode가 지정하는 정렬 정보에 따라 hdc의 정렬 상태를 변경하며 이후부터 hdc를 참조하여 출력되는 모든 문자열은 이 함수가 지정한 정렬 상태를 따른다.
		TextOut(hdc, 200, 60, TEXT("Beautiful Korea"), 15);
		TextOut(hdc, 200, 80, TEXT("is My"), 5);
		TextOut(hdc, 200, 100, TEXT("Lovely Home Country"), 19);
		SetTextAlign(hdc, TA_UPDATECP);		// 출력 위치를 지정하는 인수를 무시하고 항상 CP의 위치에 문자열을 출력하며 출력 후에 CP를 문자열의 바로 다음 위치로 옮긴다.
		// CP : 텍스트 모드에서 다음 문자가 출력될 위치를 커서가 표시하듯이 그래픽 모드에서 다음 그래픽이 출력될 위치를 가리킨다.
		TextOut(hdc, 200, 60, TEXT("One "), 4);
		TextOut(hdc, 200, 80, TEXT("Two "), 4);
		TextOut(hdc, 200, 100, TEXT("Three"), 5);
		// TextOut 출력문은 오로지 문자열만 다룰 수 있다. 정수나 실수를 출력하는 함수는 따로 없으므로 서식화하여 문자열로 만들어서 출력해야 한다.
		TCHAR str[128];		// 문자열 조립을 위한 임시 버퍼 성언
		wsprintf(str, TEXT("현재 점수는 %d점입니다."), Score);		// 문자열 형태로 만든다.
		TextOut(hdc, 10, 10, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		TextOut(hdc, 100, 100, TEXT("Beautiful Korea"), lstrlen(TEXT("Beautiful Korea")));
		ReleaseDC(hWnd, hdc);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void MyTextOut(HDC hdc, int x, int y, LPCTSTR Text) {
	TextOut(hdc, x, y, Text, lstrlen(Text));
}