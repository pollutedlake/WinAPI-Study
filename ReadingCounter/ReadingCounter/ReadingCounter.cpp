#include<Windows.h>
#include"Resource.h"

BOOL CALLBACK ReadingDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	g_hInst = hInstance;
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_READING), HWND_DESKTOP, ReadingDlgProc);
	return 0;
}

int PageStart, PageEnd, NowPage;		// 페이지의 범위의 시작 페이지와 끝 페이지, 현재 페이지
int PageSec, NowSec;		// 한 페이지를 읽을 시간과 현재 페이지의 남은 시간
enum tag_Status { WAIT, RUN, PAUSE };
tag_Status Status;		// 현재 프로그램의 상태가 중지 상태인지 카운트를 하고 있는 중인지를 기억하기 위해 Status라는 열거형 변수를 사용한다.

BOOL CALLBACK ReadingDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	TCHAR Mes[256];		// 몇 페이지를 읽는지와 몇 초 남았는지 출력할 문자열

	switch (iMessage) {
	case WM_INITDIALOG:		// 프로그램의 상태 초기화
		hDlgMain = hDlg;
		Status = WAIT;
		return TRUE;
	case WM_TIMER:
		NowSec--;
		if (NowSec == 0) { // 남은 시간이 0이 되면 페이지가 바뀐다.
			if (IsDlgButtonChecked(hDlg, IDC_CHKBEEP) == BST_CHECKED) {		// IDC_CHKBEEP이 체크되어 있다면 페이지가 바뀔 때마다 소리로 알려준다.
				MessageBeep(0);
			}
			NowPage++;		// 페이지가 바뀐다.
			NowSec = PageSec;		// 남은 시간을 한 페이지를 읽을 시간으로 초기화
			if (NowPage > PageEnd) {		// 현재 페이지가 끝 페이지를 넘을 경우
				SetDlgItemText(hDlg, IDC_STTIME, TEXT("다 읽었습니다."));
				KillTimer(hDlg, 1);		// 타이머 제거
				Status = WAIT;		// 프로그램 상태 WAIT로 변경
				return TRUE;
			}
		}
		wsprintf(Mes, TEXT("%d 페이지 읽는 중 : %d 초 남음"), NowPage, NowSec);
		SetDlgItemText(hDlg, IDC_STTIME, Mes);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTNSTART:		// 독서 시작 버튼 누를 시 PageStart와 PageEnd읽어온다.
			PageStart = GetDlgItemInt(hDlg, IDC_EDSTART, NULL, FALSE);
			PageEnd = GetDlgItemInt(hDlg, IDC_EDEND, NULL, FALSE);
			if (PageStart == 0 || PageEnd == 0 || PageStart >= PageEnd) {		// PageStart와 PageEnd입력이 올바르지 않을 경우 메시지박스 출력
				MessageBox(hDlg, TEXT("페이지 번호를 다시 입력하시오"), TEXT("알림"), MB_OK);
				return TRUE;
			}
			NowPage = PageStart;		// 현재 페이지를 PageStart로 초기화
			PageSec = GetDlgItemInt(hDlg, IDC_EDTIME, NULL, FALSE);		// 한 페이지를 읽을 시간을 읽어온다.
			if (PageSec <= 0) {		// PageSec 입력이 올바르지 않을 경우 메시지박스 출력
				MessageBox(hDlg, TEXT("시간을 다시 입력하시오"), TEXT("알림"), MB_OK);
				return TRUE;
			}
			NowSec = PageSec;		// 남은 시간을 PageSec으로 초기화
			SetTimer(hDlg, 1, 1000, NULL);		// 타이머 설정
			SendMessage(hDlg, WM_TIMER, 1, 0);		// 타이머 메시지를 보낸다.
			Status = RUN;
			return TRUE;
		case IDC_BTNPAUSE:		// 잠시 중지 버튼을 누를 시
			if (Status == PAUSE) {		// PAUSE 상태라면 타이머를 다시 작동시킨다.
				SetTimer(hDlg, 1, 1000, NULL);
				Status = RUN;
			}
			else if (Status == RUN) {		// 작동 중이라면 타이머를 없애고 멈춤
				KillTimer(hDlg, 1);
				Status = PAUSE;
			}
			return TRUE;
		case IDCANCEL:		// 닫기 버튼을 누를 시Timer제거와 Dialog종료
			KillTimer(hDlg, 1);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}