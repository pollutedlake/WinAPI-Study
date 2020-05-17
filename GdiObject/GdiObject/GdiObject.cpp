#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiObject");

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
	// MyBrush는 그리기에 사용할 브러시 핸들, OldBrush는 원래의 브러시를 잠시 저장하기 위한 용도로 사용된다.
	HBRUSH MyBrush, OldBrush;
	// MyPen는 그리기에 사용할 펜 핸들, OldPen는 원래의 펜을 잠시 저장하기 위한 용도로 사용된다.
	HPEN MyPen, OldPen;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// Stock Object는 윈도우즈가 기본적으로 제공하는 GDI 오브젝트를 말한다.
		// HGDIOBJ GetStockObject(int fnObject); : 스톡 오브젝트의 핸들을 얻어온다. 여러 종류의 스톡 오브젝트를 리턴하므로 대입하기 전에 원하는 타입으로 캐스팅해야 한다.
		// fnObject : 사용하고자 하는 스톡 오브젝트를 지정한다.
		MyBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);

		// HBRUSH CreateSolidBrush(COLORREF crColor); : 단색의 브러시만들 만들 수 있으며 브러시의 색상만 인수로 전달한다.
		// HBRUSH CreateHatchBrush(int fnStyle, COLORREF clrref); : 색상뿐만 아니라 무늬도 같이 지정할 수 있다.
		MyBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 255));

		// HPEN CreatePen(int fnPenStyle, int nWidth, COLORREF crColor); : 새로 만든 펜의 핸들이 리턴되는데 이 값을 잘 보관해 두어야 만들어진 펜을 사용할 수 있으며 사용한 후 파괴할 수도 있다.
		// fnPenStyle : 그려질 선의 모양을 지정한다. 선의 모양은 굵기가 1일 때만 효과가 있으며 굵기가 2이상이면 무조건 실선으로 그려진다.
		// nWidth : 선의 폭을 지정한다. 디폴트 선의 굵기는 1이지만 0일 경우는 맵핑 모드에 상관없이 무조건 1픽셀 두께의 선이 만들어진다.
		// crColor : 선의 색상을 지정한다. COLORREF 형이므로 RGB 매크로 함수를 사용하면 된다.
		MyPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));

		// HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj); : DC에 해당 오브젝트를 선택한다. 이후부터 GDI는 그래픽을 출력할 때 선택된 오브젝트를 사용한다. 새로 선택되는 오브젝트 이전에 선택되어 있던 같은 종류의 오브젝트 핸들을 리턴한다.
		// hdc : DC의 핸들
		// hgdiobj : GDI 오브젝트의 핸들
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		OldPen = (HPEN)SelectObject(hdc, MyPen);
		Rectangle(hdc, 50, 50, 300, 200);
		SelectObject(hdc, OldBrush);

		// BOOL DeleteObject(HGDIOBJ hObject); : GDI 오브젝트는 사용한 후 메모리를 사용하기 때문에 반드시 삭제해야 한다. 해제하지 않으면 시스템의 메모리와 리소스를 갉아먹게 될 것이다. 삭제하고자 하는 GDI 오브젝트의 핸들만 인수로 넘겨주면 된다.
		// 단, DC에 현재 선택되어 있는 GDI 오브제트는 삭제할 수 없다. 그래서 삭제하기 전에 DC에 선택된 객체를 선택 해제해야 하는데 가장 간단한 방법은 같은 종류의 다른 GDI 오브젝트를 선택하는 것이다.
		DeleteObject(SelectObject(hdc, OldPen));
		DeleteObject(MyBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}