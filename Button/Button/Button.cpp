#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MyButton");

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

	switch (iMessage) {
	case WM_CREATE:		// 컨트롤은 윈도우이기는 하지만 홀로 사용될 수 없으며 반드시 부모 윈도우의 차일드로 존재해야 한다. 차일드 컨트롤은 보통 부모 윈도우가 만들어질 때 즉, WM_CREATE 메시지가 발생했을 때 만든다.
		hWndMain = hWnd;

		// CreateWindow(TEXT("button"), TEXT("Click Me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		// "button" : 첫 번째 인수는 만들고자 하는 윈도우의 윈도우 클래스이다. 컨트롤은 운영체제에 의해 윈도우 클래스가 미리 등록되어 있으므로 별도로 등록할 필요없이 이 인수에 만들고자 하는 컨트롤의 윈도우 클래스명을 적어주면 된다.
		// "Click Me" : 두 번째 인수는 윈도우의 타이틀 바에 타나탈 윈도우의 제목이되 컨트롤에 따라 캡션이 나타날 위치가 달라진다. 캡션이 필요없고 내용만 있는 컨트롤은 NULL로 지정하면 된다.
		// 세 번째 인수는 윈도우의 속성값이다. 컨트롤은 차일드 윈도우이므로 예외없이 WS_CHILD 스타일은 반드시 주어야 한다. 또한, WS_VISIBLE 스타일을 주어야 ShowWindow 함수를 호출하지 않아도 컨트롤이 화면에 타나타므로 컨트롤의 경우 이 두 스타일값은 거의 예외없이 지정하는 것이 정석이다.
		// 4번째부터 7번째 인수까지는 윈도우의 위치와 크기를 지정한다. 컨트롤의 경우는 부모 윈도우의 작업영역을 기준으로 한 좌표가 사용된다.
		// 8번째 인수는 컨트롤의 부모 윈도우를 지정하는데 컨트롤은 차일드이므로 반드시 부모 윈도우가 있어야 한다.
		// 9번째 인수는 윈도우에서 사용할 메뉴의 핸들이다. 단, 차일드 컨트롤은 메뉴를 가지지 않으므로 컨트롤의 ID지정 용도로 사용된다. 원래 이 인수가 메뉴를 지정하므로 (HMENU)형으로 캐스팅해야 한다.
		// 10번째 인수는 이 윈도우를 만드는 인스턴스의 핸들이다. 11번째 인수는 사용자 정의 데이터이며 MDI에서 사용하는 구조체인데 차일드 컨트롤을 만들 때는 이 인수를 사용하지 않는다.
		CreateWindow(TEXT("button"), TEXT("Click Me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("Me Two"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 50, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("끝내기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 80, 100, 25, hWnd, (HMENU)2, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		// 버튼이 클릭될 경우 WM_COMMAND 메시지(통지 메시지)를 부모 윈도우에게 보내 어떤 사건이 발생했는지 알리며 이때 전달 되는 정보는 다음과 같다.
		// HIWORD(wParam) : 통지 코드(차일드 컨트롤이 왜 메시지를 보냈는가를 나타내는 값이다.)
		// LOWORD(wParam) : 컨트롤의 ID
		// lParam : 메시지를 보낸 차일드 윈도우의 윈도우 핸들
		switch (LOWORD(wParam)) {
		case 0:
			MessageBox(hWnd, TEXT("First Button Clicked"), TEXT("Button"), MB_OK);
			break;
		case 1:
			MessageBox(hWnd, TEXT("Second Button Clicked"), TEXT("Button"), MB_OK);
			break;
		case 2:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}