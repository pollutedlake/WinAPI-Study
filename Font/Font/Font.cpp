#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Font");

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT hFont, OldFont;
	const TCHAR* str = TEXT("폰트 Test 1234");
	LOGFONT lf;		// CreateFont 함수의 인수 전체를 멤버로 가지는 구조체이며 이 구조체에 원하는 값을 대입한 후 CreateFontIndirect 함수로 이 구조체의 번지를 넘기면 된다.

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCTSTR lpszFace);
		// nHeight : 폰트의 높이를 논리적인 단위로 지정한다. 0일 경우 디폴트 크기가 사용된다.
		// nWidth : 폰트의 폭을 지정하되 0이면 nHeight에서 지정한 높이에 따라 폭을 자동으로 결정한다.
		// nEscapement : 폰트의 각도를 0.1도 단위로 설정한다. 이 각도는 문자가 출력될 X축과 문자열과의 각도이며 세 시 방향이 0도로 사용되고 반시계 방향으로 각도가 증가한다.
		// nOrientation : 글자 한 자와 X축과의 각도를 지정한다. nEscapement는 전체 문자열의 기울기를 지정하는데 비해 이 인수는 개별 문자의 기울기를 설정한다.
		// fnWeight : 폰트의 두께를 0~1000까지의 값으로 설정한다. 보통 굵기인 FW_NORMAL이 400이다.
		// fdwStrikeOut, fdwItalic, fdwUnderline : 관통선, 기울임체, 밑줄 속성을 설정한다. 데이터형이 DWORD형이지만 불린형처럼 사용한다. 속성을 주고 싶으면 0 이외의 값을 주고 속성을 주지 않으려면 0을 준다.
		// fdwCharSet : 문자 코드와 문자의 대응관계를 정의하는 문자셋이다. 폰트의 정보 중 가장 중요하다.
		// fdwOutputPrecision : 출력 정확도를 설정한다.
		// fdwClipPrecision : 클리핑 정확도를 설정한다.
		// fdwQuality : 논리적 폰트를 물리적 폰트에 얼마나 근접시킬 것인가를 지정한다.
		// fdwPitchAndFamily : 폰트의 피치와 그룹을 설정한다.
		// lpszFace : 글꼴의 이름을 나타내는 문자열을 설정한다.
		hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		OldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc, 100, 100, str, lstrlen(str));
		lf.lfHeight = 50;
		lf.lfWidth = 0;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfWeight = 0;
		lf.lfItalic = 0;
		lf.lfUnderline = 0;
		lf.lfStrikeOut = 0;
		lf.lfCharSet = HANGEUL_CHARSET;
		lf.lfOutPrecision = 0;
		lf.lfClipPrecision = 0;
		lf.lfQuality = 0;
		lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(lf.lfFaceName, TEXT("궁서"));

		// HFONT CreateFontIndirect(CONST LOGFONT* lplf);
		// lplf : LOGFONT 구조체의 포인터
		hFont = CreateFontIndirect(&lf);
		OldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc, 100, 150, str, lstrlen(str));
		SelectObject(hdc, OldFont);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}