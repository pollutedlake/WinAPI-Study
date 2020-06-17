#include<Windows.h>
#include "Resource.h"

#define MAXSTAGE 3	// 스테이지 수
#define BW 32		// 출력에 사용할 비트맵의 가로 크기
#define BH 32		// 출력에 사용할 비트맵의 세로 크기

void DrawScreen(HDC hdc);		// 화면을 그리는 함수
BOOL TestEnd();		// 게임이 끝났는지 확인하고 끝났다면 메시지 출력
void Move(int dir);		// 사람과 짐을 이동시키는 함수
void InitStage();		// 스테이지 초기화 함수
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);		// 비트맵을 그리는 함수

char ns[18][21];		// 게임 상태를 저장하는 배열 입출력과는 직접적인 연관이 없다.
int nStage;		// 몇 번째 스테이지인지 나타내는 변수 
int nx, ny;		// 캐릭터의 위치를 저장한 변수
int nMove;		// 움직인 횟수
HBITMAP hBit[5];		// 비트맵들의 핸들을 저장하기 위한 배열

char arStage[MAXSTAGE][18][21] = {	// 맵의 배치 상태를 기억하는 배열
	{
	"####################",
	"####################",
	"####################",
	"#####   ############",
	"#####0  ############",
	"#####  0############",
	"###  0 0 ###########",
	"### # ## ###########",
	"#   # ## #####  ..##",
	"# 0  0   @      ..##",
	"##### ### # ##  ..##",
	"#####     ##########",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
	{
	"####################",
	"####################",
	"####################",
	"####################",
	"####..  #     ######",
	"####..  # 0  0  ####",
	"####..  #0####  ####",
	"####..    @ ##  ####",
	"####..  # #  0 #####",
	"######### ##0 0 ####",
	"###### 0  0 0 0 ####",
	"######    #     ####",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
	{
	"####################",
	"####################",
	"####################",
	"####################",
	"##########     @####",
	"########## 0#0 #####",
	"########## 0  0#####",
	"###########0 0 #####",
	"########## 0 # #####",
	"##....  ## 0  0  ###",
	"##...     0  0   ###",
	"##....           ###",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Sokoban2");

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);		// 메인 윈도우의 크기를 조정할 수 없다.
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
	RECT crt;		// 윈도우의 크기를 지정할 RECT변수
	int i;		// hBit의 인덱스
	TCHAR Message[256];		// 게임이 끝났을 때 출력할 메시지

	switch (iMessage) {
	case WM_CREATE:		// 메인 윈도우의 크기를 게임에 꼭 맞게 조정한다. 스테이지 초기화
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, 900, BH * 18);		// 작업영역의 높이를 비트맵 18개의 높이로 하여 게임판 크기에 꼭 맞는 크기로 계산했다.
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);		// 윈도우의 크기를 작업영역 크기에 맞게 조정한다.
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);
		for (i = 0; i < 5; i++) {		// 비트맵들을 hBit배열에 읽어온다.
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL + i));
		}
		nStage = 0;
		InitStage();		// 스테이지 초기화
		return 0;
	case WM_PAINT:		// 직접 그리기를 하지 않고 DrawScreen 함수를 불러 그릭기를 하되 DC만 구해서 넘겨준다.
		hdc = BeginPaint(hWnd, &ps);
		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:		// 캐릭터 이동 키와 게임 종료 다시 시작 스테이지 이동 키
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			Move(wParam);		// 캐릭터 이동
			if (TestEnd()) {		// 캐릭터 이동마다 게임이 끝났는지 확인하기
				wsprintf(Message, TEXT("%d 스테이지를 풀었습니다.") TEXT("다음 스테이지로 이동합니다."), nStage + 1);		// 게임이 끝났으면 안내문 띄우고 스테이지 이동
				MessageBox(hWnd, Message, TEXT("알림"), MB_OK);
				if (nStage < MAXSTAGE - 1) {
					nStage++;
				}
				InitStage();		// 매 스테이지가 끝날 때마다 스테이지 초기화
			}
			break;
		case 'Q':		// Q이면 윈도우 종료
			DestroyWindow(hWnd);
			break;
		case 'R':		// R이면 스테이지 초기화
			InitStage();
			break;
		case 'N':		// N키면 다음 스테이지로 이동
			if (nStage < MAXSTAGE - 1) {
				nStage++;
				InitStage();
			}
			break;
		case 'P':		// P키면 이전 스테이지로 이동
			if (nStage > 0) {
				nStage--;
				InitStage();
			}
			break;
		}
		return 0;
	case WM_DESTROY:		// 게임 종료시 비트맵지우기
		for (i = 0; i < 5; i++) {
			DeleteObject(hBit[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawScreen(HDC hdc) {		// 배열 내의 문자와 대응되는 비트맵을 찾아 DrawBitmap으로 그린다.
	int x, y;
	int iBit;
	TCHAR Message[256];		// 출력할 안내문

	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			switch (ns[y][x]) {
			case '#':		// 배열에 문자 '#'이 있는 칸은 IDB_WALL 비트맵을 그린다.
				iBit = 0;
				break;
			case '0':		// 배열에 문자 '0'이 있는 칸은 IDB_PACK 비트맵을 그린다.
				iBit = 1;
				break;
			case '.':		// 배열에 문자 '.'이 있는 칸은 IDB_GOAL 비트맵을 그린다.
				iBit = 2;
				break;
			case ' ':		// 배열에 문자 ' '이 있는 칸은 IDB_BLANK 비트맵을 그린다.
				iBit = 3;
				break;
			}
			DrawBitmap(hdc, x * BW, y * BH, hBit[iBit]);		// 배열 문자에 따라 비트맵을 그린다.
		}
	}
	DrawBitmap(hdc, nx * BW, ny * BH, hBit[4]);		// 배열에 문자 '@'이 있는 칸은 IDB_MAN 비트맵을 그린다.

	//--- 안내문 출력
	wsprintf(Message, TEXT("SOKOBAN"));		// 게임 이름
	TextOut(hdc, 700, 10, Message, lstrlen(Message));
	wsprintf(Message, TEXT("Q:종료, R:다시 시작"));		// 종료 키 다시 시작 키
	TextOut(hdc, 700, 30, Message, lstrlen(Message));
	wsprintf(Message, TEXT("N:다음, P:이전"));		// 다음 이전 스테이지 이동 키
	TextOut(hdc, 700, 50, Message, lstrlen(Message));
	wsprintf(Message, TEXT("스테이지 : %d"), nStage + 1);		// 스테이지 번호 출력
	TextOut(hdc, 700, 70, Message, lstrlen(Message));
	wsprintf(Message, TEXT("이동 횟수 : %d"), nMove);		// 이동 횟수 출력
	TextOut(hdc, 700, 90, Message, lstrlen(Message));
	//--- 안내문 출력
}

BOOL TestEnd() {		// 배열의 내부 상태를 점검하여 모든 짐들이 창고에 잘 들어갔는지만 판단한다.
	int x, y;

	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (arStage[nStage][y][x] == '.' && ns[y][x] != '0') {		// 목표지점에 짐이 들어가있지 않으면 게임이 끝나지 않은 상태이다.
				return FALSE;
			}
		}
	}
	return TRUE;		// 목표지점들에 짐이 모두 들어가 있으면 게임이 끝난 상태이다.
}

void Move(int dir) {
	int dx = 0, dy = 0;

	switch (dir) {
	case VK_LEFT:
		dx = -1;
		break;
	case VK_RIGHT:
		dx = 1;
		break;
	case VK_UP:
		dy = -1;
		break;
	case VK_DOWN:
		dy = 1;
		break;
	}

	if (ns[ny + dy][nx + dx] != '#') {		// 이동 후 위치가 벽이 아니라면
		if (ns[ny + dy][nx + dx] == '0') {		// 이동 후 위치가 짐이라면
			if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') {		// 짐이 이동한 위치가 빈 칸이거나 목표 지점이라면
				if (arStage[nStage][ny + dy][nx + dx] == '.') {		// 이동 후 위치의 배열 값 유지
					ns[ny + dy][nx + dx] = '.';
				}
				else {
					ns[ny + dy][nx + dx] = ' ';
				}
				ns[ny + dy * 2][nx + dx * 2] = '0';		// 짐 이동
			}
			else {		// 아니라면 이동 안함
				return;
			}
		}
		nx += dx;		// 캐릭터 위치 변화
		ny += dy;		// 캐릭터 위치 변화
		nMove++;		// 이동 횟수 증가
		InvalidateRect(hWndMain, NULL, FALSE);		// 화면 다시 그리기
	}
}

void InitStage() {
	int x, y;

	memcpy(ns, arStage[nStage], sizeof(ns));		// ns에 처음 게임상태 저장
	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (ns[y][x] == '@') {		// 캐릭터의 위치를 저장하고 빈 칸으로 바꾼다.
				nx = x;
				ny = y;
				ns[y][x] = ' ';
			}
		}
	}
	nMove = 0;		// 이동 횟수 초기화
	InvalidateRect(hWndMain, NULL, TRUE);		// 화면 다시 그린다.
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	HDC MemDC;		// 메모리 DC
	HBITMAP OldBitmap;
	int bx, by;		// 비트맵의 사이즈
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);		// BackDC생성
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);		// 비트맵의 크기를 조사하기 위한 함수
	bx = bit.bmWidth;		// 비트맵의 가로 사이즈
	by = bit.bmHeight;		// 비트맵의 세로 사이즈

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);		// 비트맵 그리기

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);		// 메모리 DC제거
}