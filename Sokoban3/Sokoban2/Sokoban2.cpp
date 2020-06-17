#include<Windows.h>
#include "Resource.h"
#include<mmsystem.h>		// PlaySound 함수를 실행시키기 위해
#pragma comment(lib, "winmm.lib")		// PlaySound 함수를 실행시키기 위해

#define MAXSTAGE 3	// 스테이지 수
#define BW 32		// 출력에 사용할 비트맵의 가로 크기
#define BH 32		// 출력에 사용할 비트맵의 세로 크기
#define MAXUNDO 1000		// undo 기능을 위해 유저의 키조작을 저장하기 위한 자료 구조의 크기 

void DrawScreen(HDC hdc);		// 화면을 그리는 함수
BOOL TestEnd();		// 게임이 끝났는지 확인하고 끝났다면 메시지 출력
void Move(int dir);		// 사람과 짐을 이동시키는 함수
void InitStage();		// 스테이지 초기화 함수
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);		// 비트맵을 그리는 함수
void Undo();		// 이동을 취소하는 함수
void Redo();		// 취소한 동작을 재실행하는 함수
void ErasePack(int x, int y);		// 짐이 있던 자리를 공백으로 만드는 함수

char ns[18][21];		// 게임 상태를 저장하는 배열 입출력과는 직접적인 연관이 없다.
int nStage;		// 몇 번째 스테이지인지 나타내는 변수 
int nx, ny;		// 캐릭터의 위치를 저장한 변수
int nMove;		// 움직인 횟수
HBITMAP hBit[8];		// 비트맵들의 핸들을 저장하기 위한 배열
int ManBit;		// 사람이 보고있는 방향 4, 5, 6, 7 순으로 왼쪽, 오른쪽, 위, 아래
BOOL bSound = TRUE;		// 소리를 키고 끄는 옵션

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

struct tag_MoveInfo {		// 사용자의 키조작
	char dx : 3;		// dx, dy는 사람이 이동한 방향인데 한번에 한 방향으로만 이동할 수 있으므로 둘 중 하나는 1, -1의 값을 가지고 나머지는 0일 것이다.
	char dy : 3;		// 세 멤버 변수 모두 가능한 값의 범위가 아주 좁으므로 비트 필드로 정의했는데 int나 BOOL로 선언하는 것보다 12배의 메모리 절약 효과가 있다.
	char bWithPack : 2;		// 사람이 움직일 때 짐도 같이 움직였는지의 여부를 기억한다. 짐의 이동 방향은 굳이 저장하지 않아도 사람의 이동 방향만으로 쉽게 유추할 수 있다.
};
tag_MoveInfo MoveInfo[MAXUNDO];		// 사용자의 키조작을 저장하기 위한 자료 구조
int UndoIdx;		// MoveInfo 배열의 현재 위치이며 다음 동작을 저장할 배열 첨자이다.

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Sokoban3");

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
		for (i = 0; i < 8; i++) {		// 비트맵들을 hBit배열에 읽어온다.
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
		case 'Z':		// Z키면 이전 이동을 취소
			Undo();
			break;
		case 'Y':		// Y키면 취소한 동작을 재실행
			Redo();
			break;
		case 'S':		// 'S'키면 소리 옵션을 토글한다.
			bSound = !bSound;
			break;
		}
		return 0;
	case WM_DESTROY:		// 게임 종료시 비트맵지우기
		for (i = 0; i < 8; i++) {
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
	DrawBitmap(hdc, nx * BW, ny * BH, hBit[ManBit]);		// 배열에 문자 '@'이 있는 칸은 IDB_MAN 비트맵을 그린다.

	//--- 안내문 출력
	wsprintf(Message, TEXT("SOKOBAN"));		// 게임 이름
	TextOut(hdc, 700, 10, Message, lstrlen(Message));
	wsprintf(Message, TEXT("Q:종료, R:다시 시작"));		// 종료, 다시 시작 키 안내
	TextOut(hdc, 700, 30, Message, lstrlen(Message));
	wsprintf(Message, TEXT("N:다음, P:이전"));		// 다음, 이전 스테이지 이동 키 안내
	TextOut(hdc, 700, 50, Message, lstrlen(Message));
	wsprintf(Message, TEXT("Z:취소, Y:재실행"));		// 취소, 재실행 키 안내
	TextOut(hdc, 700, 70, Message, lstrlen(Message));
	wsprintf(Message, TEXT("S:소리 토글"));		// 소리 옵션 토글 키 안내
	TextOut(hdc, 700, 90, Message, lstrlen(Message));
	wsprintf(Message, TEXT("스테이지 : %d"), nStage + 1);		// 스테이지 번호 출력
	TextOut(hdc, 700, 120, Message, lstrlen(Message));
	wsprintf(Message, TEXT("이동 횟수 : %d"), nMove);		// 이동 횟수 출력
	TextOut(hdc, 700, 140, Message, lstrlen(Message));
	//--- 안내문 출력
}

BOOL TestEnd() {		// 배열의 내부 상태를 점검하여 모든 짐들이 창고에 잘 들어갔는지만 판단한다.
	int x, y;

	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (arStage[nStage][y][x] == '.' && ns[y][x] != '0') {		// 창고에 짐이 들어가있지 않으면 게임이 끝나지 않은 상태이다.
				return FALSE;
			}
		}
	}
	return TRUE;		// 창고에 짐이 모두 들어가 있으면 게임이 끝난 상태이다.
}

void Move(int dir) {
	int dx = 0, dy = 0;		// 사람이 움직인 방향과 거리
	BOOL bWithPack = FALSE;		// 사람이 움직였을 때 짐도 함께 움직였는지
	int i;

	switch (dir) {
	case VK_LEFT:
		ManBit = 4;
		dx = -1;
		break;
	case VK_RIGHT:
		ManBit = 5;
		dx = 1;
		break;
	case VK_UP:
		ManBit = 6;
		dy = -1;
		break;
	case VK_DOWN:
		ManBit = 7;
		dy = 1;
		break;
	}

	if (ns[ny + dy][nx + dx] != '#') {		// 이동 후 위치가 벽이 아니라면
		if (ns[ny + dy][nx + dx] == '0') {		// 이동 후 위치가 짐이라면
			if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') {		// 짐이 이동한 위치가 빈 칸이거나 창고라면
				ErasePack(nx + dx, ny + dy);		// 이동 후 위치의 배열 값 유지
				ns[ny + dy * 2][nx + dx * 2] = '0';		// 짐 이동
				bWithPack = TRUE;
			}
			else {		// 아니라면 이동 안함
				return;
			}
		}
		nx += dx;		// 캐릭터 위치 변화
		ny += dy;		// 캐릭터 위치 변화
		nMove++;		// 이동 횟수 증가

		MoveInfo[UndoIdx].dx = dx;
		MoveInfo[UndoIdx].dy = dy;
		MoveInfo[UndoIdx].bWithPack = bWithPack;
		UndoIdx++;
		MoveInfo[UndoIdx].dx = -2;		// 취소한 상태에서 재실행할 때 뒤의 이동 기록을 무효화하는 역할을 한다.

		if (UndoIdx == MAXUNDO - 1) {		// MoveInfo 배열의 끝에 닿았을 때 메모리 관리
			for (i = 100; i < UndoIdx; i++) {		// 먼저 기록한 100개 레코드를 삭제
				MoveInfo[i - 100] = MoveInfo[i];
			}
			for (i = MAXUNDO - 100; i < MAXUNDO; i++) {		// 뒤쪽에 100개의 공간을 더 만든다.
				MoveInfo[i].dx = -2;
			}
			UndoIdx -= 100;
		}

		if (bSound) {
			PlaySound(MAKEINTRESOURCE(bWithPack ? IDR_WITHPACK : IDR_MOVE), g_hInst, SND_RESOURCE | SND_ASYNC);		// 움직일 때마다 사운드를 출력한다.
		}
		InvalidateRect(hWndMain, NULL, FALSE);		// 화면 다시 그리기
	}
}

void ErasePack(int x, int y) {		// 짐이 있던 자리를 공백으로 만드는 함수
	if (arStage[nStage][y][x] == '.') {		// 원본 배열을 참조하여 이 자리가 창고였다면	
		ns[y][x] = '.';
	}
	else {		// 공백이었다면
		ns[y][x] = ' ';
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
	UndoIdx = 0;		// 0으로 초기화하여 배열 선두에서부터 기록을 시작한다.
	for (x = 0; x < MAXUNDO; x++) {		// MoveInfo 배열 초기화
		MoveInfo[x].dx = -2;		// -2 특이값을 대입하여 이동 기록이 없는 것으로 초기화한다.
	}
	InvalidateRect(hWndMain, NULL, TRUE);		// 화면 다시 그린다.
	ManBit = 4;		// 왼쪽을 보게 초기화
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

void Undo() {		// 이동을 취소하는 함수
	if (UndoIdx != 0) {		// UndoIdx가 0인 경우는 게임을 시작한 상태, 즉 아직 이동 기록이 전혀 작성되지 않은 상태이므로 이때는 취소할 수 없다.
		UndoIdx--;		// UndoIdx가 다음 기록할 자리를 가리키므로 이 값보다 한칸 앞쪽의 기록대로 취소해야 한다.
		if (MoveInfo[UndoIdx].bWithPack) {		// 짐도 함께 움직였다면
			ErasePack(nx + MoveInfo[UndoIdx].dx, ny + MoveInfo[UndoIdx].dy);		// 취소 전의 짐의 위치에서 짐 삭제
			ns[ny][nx] = '0';		// 지금 사람이 있는 위치로 짐을 가져온다.
		}
		nx -= MoveInfo[UndoIdx].dx;		// 이동할 때 사람이 dx, dy만큼 움직였으므로 취소할 때는 이 값을 빼면 된다.
		ny -= MoveInfo[UndoIdx].dy;
		InvalidateRect(hWndMain, NULL, FALSE);
	}
}

void Redo() {		// 취소한 동작을 재실행하는 함수
	if (MoveInfo[UndoIdx].dx != -2) {
		nx += MoveInfo[UndoIdx].dx;		// 사람은 dx, dy에 기록된만큼 이동한다.
		ny += MoveInfo[UndoIdx].dy;
		if (MoveInfo[UndoIdx].bWithPack) {		// 짐도 같이 움직인 경우
			ErasePack(nx, ny);		// 사람이 움직인 새 위치의 짐을 삭제한다.
			ns[ny + MoveInfo[UndoIdx].dy][nx + MoveInfo[UndoIdx].dx] = '0';		// 사람이 움직인 방향쪽에 짐을 다시 놓는다.
		}
		InvalidateRect(hWndMain, NULL, FALSE);
		UndoIdx++;
	}
}