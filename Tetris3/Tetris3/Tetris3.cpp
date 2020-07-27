#include <Windows.h>
#include "Resource.h"
#include <mmsystem.h>		// Sound 출력을 위해
#pragma comment(lib, "winmm.lib")		// Sound 출력을 위해

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Tetris4");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstancem, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = 0;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&Message, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return (int)Message.wParam;
}

// 함수 원형
void DrawScreen(HDC hdc);		// 화면을 그리는 함수
void MakeNewBrick();		// 새 벽돌을 만드는 함수
int GetAround(int x, int y, int b, int r);		// 
BOOL MoveDown();		// 벽돌을 아래로 한 칸 내리는 함수 벽돌이 바닥에 닿았는지를 리턴한다.
void TestFull();
void PrintTile(HDC hdc, int x, int y, int c);		// 타일을 찍는 함수
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);		// bimap을 그리는 함수
void PlayEffectSound(UINT Sound);		// Sound를 출력하는 함수
void AdjustMainWindow();		// 게임판의 크기를 변경하는 함수

// 매크로 및 전역 변수들
#define random(n) (rand() % n)		// 난수를 생성하는 매크로
#define TS 24		// 비트맵 타일의 크기를 지정하는 매크로

struct Point {		// 블록들의 x, y좌표값
	int x, y;
};

Point Shape[][4][4] = {		// 벽돌의 모양을 정의하는 배열
	{{0, 0, 1, 0, 2, 0, -1, 0}, {0, 0, 0, 1, 0, -1, 0, -2}, {0, 0, 1, 0, 2, 0, -1, 0}, {0, 0, 0, 1, 0, -1, 0, -2}},		// l자 블록
	{{0, 0, 1, 0, 0, 1, 1, 1}, {0, 0, 1, 0, 0, 1, 1, 1}, {0, 0, 1, 0, 0, 1, 1, 1}, {0, 0, 1, 0, 0, 1, 1, 1}},		// ㅁ자 블록
	{{0, 0, -1, 0, 0, -1, 1, -1}, {0, 0, 0, 1, -1, 0, -1, -1}, {0, 0, -1, 0, 0, -1, 1, -1}, {0, 0, 0, 1, -1, 0, -1, -1}},		// Z자 블록
	{{0, 0, -1, -1, 0, -1, 1, 0}, {0, 0, -1, 0, -1, 1, 0, -1}, {0, 0, -1, -1, 0, -1, 1, 0}, {0, 0, -1, 0, -1, 1, 0, -1}},		// S자 블록
	{{0, 0, -1, 0, 1, 0, -1, -1}, {0, 0, 0, -1, 0, 1, -1, 1}, {0, 0, -1, 0, 1, 0, 1, 1}, {0, 0, 0, -1, 0, 1, 1, -1}},		// L자 블록
	{{0, 0, 1, 0, -1, 0, 1, -1}, {0, 0, 0, 1, 0, -1, -1, -1}, {0, 0, 1, 0, -1, 0, -1, 1}, {0, 0, 0, -1, 0, 1, 1, 1}},		// J자 블록
	{{0, 0, -1, 0, 1, 0, 0, 1}, {0, 0, 0, -1, 0, 1, 1, 0}, {0, 0, -1, 0, 1, 0, 0, -1}, {0, 0, -1, 0, 0, -1, 0, 1}},		// T자 블록
	{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}},		// 점 블록
	{{0, 0, 0, 0, 0, -1, 1, 0}, {0, 0, 0, 0, -1, 0, 0, -1}, {0, 0, 0, 0, 0, 1, -1, 0}, {0, 0, 0, 0, 0, 1, 1, 0}},		// ㄱ자 블록
};
int arBW[] = { 8, 10, 12, 15, 20 };		// 선택 가능한 게임판의 폭
int arBH[] = { 15, 20, 25, 30, 32 };		// 선택 가능한 게임판의 높이
int BW = 10;		// 게임판의 가로 크기
int BH = 20;		// 게임판의 세로 크기
enum { EMPTY, BRICK, WALL = sizeof(Shape) / sizeof(Shape[0]) + 1 };		// board 배열에 기록되는 열거값, WALL값이 Shape 배열 크기보다 하나 더 큰 값으로 정의되었다. 벽돌의 색상을 기억해야 하므로 WALL은 벽돌의 종류만큼 뒤쪽으로 이동하고 EMPTY가 0이므로 1을 더해줘야 한다.
int board[22][34];		// 게임판의 벽돌들을 기억하는 배열 board의 크기는 사용자가 선택한 게임판의 최대 크기에 양쪽 벽까지 포함한 22, 34 크기이다.
int nx, ny;		// 내려가는 벽돌의 위치
int brick, rot;		// 현재 벽돌의 종류, 회전값
int nbrick;		// 다음 벽돌의 종류
int score;		// 점수
int bricknum;		// 벽돌의 개수
enum tag_Status { GAMEOVER, RUNNING, PAUSE };		// 현재 게임의 상태
tag_Status GameStatus;		// 현재 게임의 상태를 기억하는 열거형 변수, 윈도우즈 프로그램은 가만히 있는 동안에도 무수히 많은 메세지를 처리해야 하기 때문에 대기를 할 수는 없고 상태를 기억해 놓고 각 메시지에서 이 값에 따라 처리를 달리 해야 한다.
int Interval;		// 게임의 실행 속도를 통제하는 타이머 주기(벽돌이 내려오는 시간, 점수가 높아지면 점점 감소하여 게임 속도가 빨라진다.)
HBITMAP hBit[11];		// 비트맵 핸들을 저장하는 배열(EMPTY와 벽돌 9개, WALL까지 총 11개의 비트맵이 필요하다.)
BOOL bShowSpace = TRUE;		// 공백 보이기 옵션을 기억할 전역 변수
BOOL bQuiet = FALSE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	int i;
	int trot;
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;

	switch (iMessage) {
	case WM_CREATE:		// 게임 실행
		hWndMain = hWnd;
		AdjustMainWindow();
		GameStatus = GAMEOVER;		// 게임 정지 상태로 시작된다.
		srand(GetTickCount());
		for (i = 0; i < 11; i++) {		// 비트맵을 로드한다. 크기가 매우 크므로 출력할 때마다 읽지 않고 실행 시 한 번만 읽어온다.
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
		}
		return 0;
	case WM_COMMAND:
		if (LOWORD(wParam) >= IDM_GAME_SIZE1 && LOWORD(wParam) <= IDM_GAME_SIZE5) {		// 게임판의 크기 변경
			if (GameStatus != GAMEOVER) {
				return 0;
			}
			BW = arBW[LOWORD(wParam) - IDM_GAME_SIZE1];
			BH = arBH[LOWORD(wParam) - IDM_GAME_SIZE1];
			AdjustMainWindow();
			memset(board, 0, sizeof(board));
			InvalidateRect(hWnd, NULL, TRUE);
		}
		switch (LOWORD(wParam)) {
		case IDM_GAME_START:		// 메뉴의 시작 명령을 선택하거나 또는 엑셀러레이터 S를 눌러야 게임이 시작된다.
			if (GameStatus != GAMEOVER) {		// 게임이 이미 진행줄일 때는 초기화를 거부한다.
				break;
			}
			for (x = 0; x < BW + 2; x++) {		// board 배열을 벽과 빈공간으로 채운다. 
				for (y = 0; y < BH + 2; y++) {
					board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
				}
			}
			// --- 전역변수 초기화
			score = 0;
			bricknum = 0;
			GameStatus = RUNNING;		// GameStatus를 RUNNING으로 설정하여 게임이 시작되었음을 알린다.
			nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
			MakeNewBrick();		// 새 벽돌을 만든다.
			Interval = 1000;
			// --- 전역변수 초기화
			SetTimer(hWnd, 1, Interval, NULL);		// Interval 간격으로 타이머를 설치 이 타이머에 의해 게임이 진행된다.
			break;
		case IDM_GAME_PAUSE:		// 게임을 멈추거나 재개한다. 메뉴의 중지 명령을 선택하거나 또는 엑셀러레이터 P를 눌러야 한다.
			if (GameStatus == RUNNING) {		// 게임이 진행줄일 때는 멈추고 타이머를 제거한다.
				GameStatus = PAUSE;
				KillTimer(hWnd, 1);		// 타이머를 파괴하면 벽돌이 더 이상 내려오지 않는다.
			}
			else if (GameStatus == PAUSE) {		// 게임이 멈춰있을 때는 게임을 재개하고 타이머를 설정한다.
				GameStatus = RUNNING;
				SetTimer(hWnd, 1, Interval, NULL);
			}
			break;
		case IDM_GAME_EXIT:		// 게임 종료. 메뉴의 종료 명령을 선택하거나 또는 엑셀러레이터 Alt+F4를 눌러야 한다.
			DestroyWindow(hWnd);
			break;
		case IDM_GAME_VIEWSPACE:		// 공백을 보이게 하는 옵션을 토글시킨다.
			bShowSpace = !bShowSpace;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case IDM_GAME_QUIET:		// 액셀러레이터 Q를 누르면 bQuiet를 토글하여 PlayEffectSound 함수의 사운드 발생 여부를 통제한다.
			bQuiet = !bQuiet;
			break;
		}
		return 0;
	case WM_INITMENU:		// 메뉴가 열릴 때 전달된다.
		CheckMenuItem((HMENU)wParam, IDM_GAME_VIEWSPACE, MF_BYCOMMAND | (bShowSpace ? MF_CHECKED : MF_UNCHECKED));		// bShowSpace의 값에 따라서 메뉴의 체크 상태를 변경한다.
		CheckMenuItem((HMENU)wParam, IDM_GAME_QUIET, MF_BYCOMMAND | (bQuiet ? MF_CHECKED : MF_UNCHECKED));		// bQuiet의 값에 따라서 메뉴의 체크 상태를 변경한다.
		return 0;
	case WM_TIMER:
		if (MoveDown() == TRUE) {		// 벽돌을 한 칸씩 아래로 떨어뜨리고 바닥에 닿으면 새 벽돌을 만든다.
			MakeNewBrick();
		}
		return 0;
	case WM_KEYDOWN:
		if (GameStatus != RUNNING || brick == -1) {		// 게임이 진행 중이 아니거나 벽돌의 종류가 이상하면 키 입력을 받지 않는다.
			return 0;
		}
		switch (wParam) {
		case VK_LEFT:
			if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
				if ((lParam & 0x40000000) == 0) {
					PlayEffectSound(IDR_WAVE4);
				}
				nx--;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_RIGHT:
			if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
				if ((lParam & 0x40000000) == 0) {
					PlayEffectSound(IDR_WAVE4);
				}
				nx++;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_UP:		// 벽돌을 회전시켜 폼을 바꾼다.
			trot = (rot == 3 ? 0 : rot + 1);
			if (GetAround(nx, ny, brick, trot) == EMPTY) {
				PlayEffectSound(IDR_WAVE1);
				rot = trot;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_DOWN:		// 벽돌을 빠르게 아래로 떨어뜨린다.
			if (MoveDown() == TRUE) {
				MakeNewBrick();
			}
			break;
		case VK_SPACE:		// Space를 누르면 벽돌이 닿을 때까지 떨어진다.
			PlayEffectSound(IDR_WAVE3);
			while (MoveDown() == FALSE) {
				;
			}
			MakeNewBrick();
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:		// 게임 종료
		KillTimer(hWndMain, 1);
		for (i = 0; i < 11; i++) {		// 비트맵 제거
			DeleteObject(hBit[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void AdjustMainWindow() {		// 게임판의 크기를 변경하는 함수
	RECT crt;
	SetRect(&crt, 0, 0, (BW + 12) * TS, (BH + 2) * TS);		// 윈도우의 사이즈 정의
	AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE);
	SetWindowPos(hWndMain, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);

}

void PlayEffectSound(UINT Sound) {		// Sound를 출력하는 함수
	if (!bQuiet) {
		PlaySound(MAKEINTRESOURCE(Sound), g_hInst, SND_RESOURCE | SND_ASYNC);
	}
}

void DrawScreen(HDC hdc) {
	int x, y, i;
	TCHAR str[128];

	// 테두리 그림
	for (x = 0; x < BW + 1; x++) {
		PrintTile(hdc, x, 0, WALL);
		PrintTile(hdc, x, BH + 1, WALL);
	}
	for (y = 0; y < BH + 2; y++) {
		PrintTile(hdc, 0, y, WALL);
		PrintTile(hdc, BW + 1, y, WALL);
	}

	// 게임판 그림
	for (x = 1; x < BW + 1; x++) {
		for (y = 1; y < BH + 1; y++) {
			PrintTile(hdc, x, y, board[x][y]);
		}
	}

	// 이동중인 벽돌 그림
	if (GameStatus != GAMEOVER && brick != -1) {
		for (i = 0; i < 4; i++) {
			PrintTile(hdc, nx + Shape[brick][rot][i].x, ny + Shape[brick][rot][i].y, brick + 1);
		}
	}

	// 다음 벽돌 그림
	for (x = BW + 3; x <= BW + 11; x++) {
		for (y = BH - 5; y <= BH + 1; y++) {
			if (x == BW + 3 || x == BW + 11 || y == BH - 5 || y == BH + 1) {
				PrintTile(hdc, x, y, WALL);
			}
			else {
				PrintTile(hdc, x, y, 0);
			}
		}
	}
	if (GameStatus != GAMEOVER) {
		for (i = 0; i < 4; i++) {
			PrintTile(hdc, BW + 7 + Shape[nbrick][0][i].x, BH - 2 + Shape[nbrick][0][i].y, nbrick + 1);
		}
	}

	// 정보 출력
	lstrcpy(str, TEXT("Tetris Ver 1.3"));
	TextOut(hdc, (BW + 4) * TS, 30, str, lstrlen(str));
	wsprintf(str, TEXT("점수 : %d"), score);
	TextOut(hdc, (BW + 4) * TS, 60, str, lstrlen(str));
	wsprintf(str, TEXT("벽돌 : %d 개"), bricknum);
	TextOut(hdc, (BW + 4) * TS, 80, str, lstrlen(str));
}

void MakeNewBrick() {
	bricknum++;		// 벽돌의 개수 증가
	brick = nbrick;
	nbrick = random(sizeof(Shape) / sizeof(Shape[0]));		// 다음 벽돌의 종류 랜덤으로 지정
	// --- 내려가는 벽돌의 위치 회전값 초기화
	nx = BW / 2;
	ny = 3;
	rot = 0;
	// --- 내려가는 벽돌의 위치 회전값 초기화
	InvalidateRect(hWndMain, NULL, FALSE);
	if (GetAround(nx, ny, brick, rot) != EMPTY) {		// 새로 만들어진 벽돌 자리가 빈 공간이 아니라면 쌓인 벽돌에 부딪친 것이므로 게임을 종료한다.
		KillTimer(hWndMain, 1);
		GameStatus = GAMEOVER;
		MessageBox(hWndMain, TEXT("게임이 끝났습니다. 다시 시작하려면 게임/시작 항목(S)을 선택해 주십시오."), TEXT("알림"), MB_OK);
	}
}

int GetAround(int x, int y, int b, int r) {		// b벽돌의 r폼일 때 x, y 위치에서 board배열의 값이 비어있는지 알아낸다.
	int i, k = EMPTY;
	for (i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown() {
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {		// 바닥에 닿으면 TRUE리턴
		TestFull();		// board배열에 벽돌을 기록하고 한줄이 벽돌로 채워졌는지 확인한다.
		return TRUE;
	}
	ny++;		// 매 주기마다 벽돌은 한 칸씩 떨어뜨린다.
	// Space키를 누를 때 한칸씩 내려갈 때마다 벽돌이 보여야 하므로 다음 WM_PAINT 메시지가 발생할 때까지 그리기를 연기해서는 안되며 UpdateWinodw 함수를 호출하여 즉시 그려서 벽돌이 내려가는 모양을 보여 준다.
	InvalidateRect(hWndMain, NULL, FALSE);
	UpdateWindow(hWndMain);		// WM_PAINT 메시지를 강제로 발생시켜 현재 화면을 즉시 갱신하는 역할을 하는데 그래야 벽돌이 내려가는 모습을 볼 수 있다.
	return FALSE;
}

void TestFull() {
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = { 0,1,3,8,20 };		// 한번에 없앤 줄의 개수에 따른 점수

	for (i = 0; i < 4; i++) {		// board 배열에 쌓인 벽돌을 기록한다.
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = brick + 1;
	}
	// TestFull 실행 중인 동안에는 이동중인 벽돌이 잠시 없는 상태
	brick = -1;		// 이 처리를 하지 않으면 화면을 다시 그릴 때 이미 착륙한 벽돌이 마치 허공에 떠 있는 것처러 ㅁ그려져 잔상이 남는다. TestFull 실행중에 WM_PAINT 메시지가 재진입되는 것이다.

	for (y = 1; y < BH + 1; y++) {		// 한 줄에 벽돌이 다 채워졌는지 확인한다.
		for (x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) {
				break;
			}
		}
		if (x == BW + 1) {
			PlayEffectSound(IDR_WAVE2);
			count++;		// 없앤 줄의 개수
			for (ty = y; ty > 1; ty--) {		// 한 줄이 없어졌으니 위 배열의 값을 한 칸씩 아래로 내린다.
				for (x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			InvalidateRect(hWndMain, NULL, FALSE);
			UpdateWindow(hWndMain);		// 이 호출이 없으면 벽돌이 사라지는 모습을 불 수 없다.
			Sleep(150);
		}
	}
	score += arScoreInc[count];		// 점수 핪산
	if (bricknum % 10 == 0 && Interval > 200) {		// 게임 속도 증가
		Interval -= 50;
		SetTimer(hWndMain, 1, Interval, NULL);
	}
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void PrintTile(HDC hdc, int x, int y, int c) {
	DrawBitmap(hdc, x * TS, y * TS, hBit[c]);
	if (c == EMPTY && bShowSpace) {		// bShowSpace옵션이 켜져있으면 공백을 표시한다.
		Rectangle(hdc, x * TS + TS / 2 - 1, y * TS + TS / 2 - 1, x * TS + TS / 2 + 1, y * TS + TS / 2 + 1);
	}
	return;
}