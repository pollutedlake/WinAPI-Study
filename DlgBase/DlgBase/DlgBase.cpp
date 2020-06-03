// 대화상자 프로시저와 윈도우 프로시저는 비슷한 역할을 하며 모양도 비슷하지만 차이점도 있다.
// 첫 번째로 두 프로시저의 리턴값이 다른데 윈도우 프로시저는 LRESULT(long)형의 값을 리턴하지만 대화상자 프로시저는 BOOL형의 값을 리턴한다.
// 윈도우 프로시저는 메시지를 처리했을 때 0을 리턴하고 관심없는 메시지는 DefWindowProc으로 보내지만 대화상자 프로시저는 메시지를 처리했으면 TRUE를 리턴하고 처리하지 못했으면 FALSE를 리턴한다.
// 만약 대화상자 프로시저가 메시지를 처리하지 못해 FALSE를 리턴했다면 그 메시지에 대한 나머지 처리는 운영체제가 알아서 디폴트 처리한다. 대화상자 프로시저는 관심없는 메시지에 대해 DefWindowProc을 호출할 필요가 없으며 대신 FALSE를 리턴하면 된다. 그래서 대화상자 프로시저의 끝은 항상 return FALSE이다.
// 두 번째로 대화상자 프로시저는 WM_CREATE 메시지 대신 WM_INITDIALOG 메시지를 받아들이며 이 메시지를 받은 시점에서 대화상자에 필요한 초기화를 하면 된다.
// 대화상자 프로시저에서 주로 처리하는 메시지는 WM_CREATE이다. 이 메시지는 대화상자에서 컨트롤이나 메뉴항목이 선택될 경우 컨트롤들이 부모 윈도우인 대화상자로 보내는 통지 메시지이다.
// 일반 윈도우에서와 마찬가지로 LOWORD(wParam)에 메시지를 보낸 컨트롤의 ID가 전달되며 HIWORD(wParam)에 통지 코드가 전달된다.

// 대화상자가 메인 윈도우가 되는 형태의 프로그램을 대화상자 기반의 프로그램(Dialog Base)이라고 한다.

#include<Windows.h>
#include "Resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);		// 대화상자의 부모는 HWND_DESKTOP으로 지정하여(NULL로 지정하는 것과 동일하다) 이 대화상자가 메인 윈도우가 되도록 했다.
	return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:		// IDCANCEL은 Cancel버튼의 ID임과 동시에 대화상자의 닫기 버튼(X버튼)의 ID이기도 하므로 이 버튼을 제거했다 하더라도 반드시 IDCANCEL에 대한 통지 메시지는 처리해야 한다. 그렇지 않으면 대화상자가 닫히지 않는다.

			// BOOL EndDialog(HWND hDlg, int nResult); : 이 함수가 호출되면 모달 대화상자가 닫힌다.
			// nResult : 대화상자를 호출한 DialogBox 함수의 리턴값으로 전달되는데 주로 취소 여부를 리턴한다. 대개의 경우 DialogBox를 호출한 곳에서는 이 함수의 리턴값을 보고 사용자가 대화상자에서 어떤 버튼을 눌렀는지 판별하고 다음 동작을 결정한다.
			EndDialog(hDlg, IDOK);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}