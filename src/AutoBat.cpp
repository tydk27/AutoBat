#include "AutoBat.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_AUTOBAT, szWindowClass, MAX_LOADSTRING);

	if (!InitInstance(hInstance, nCmdShow)) return FALSE;

	hInst = hInstance;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUTOBAT));
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, INT nCmdShow)
{
	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOBAT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AUTOBAT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindowEx(
		0,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hmenu;
	static MENUITEMINFO menuInfo;

	DWORD lpThreadId;

	RECT rect;

	switch (uMsg)
	{
	case WM_ENDTHREAD:
		if (CloseHandle(hThread)) hThread = NULL;
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_SHUTDOWN:
			menuInfo.fMask = MIIM_STATE;
			menuInfo.fState = (menuInfo.fState == MFS_CHECKED) ? MFS_UNCHECKED : MFS_CHECKED;
			SetMenuItemInfo(hmenu, IDM_SHUTDOWN, FALSE, &menuInfo);
			break;
		case IDM_EXECUTE:
			hThread = CreateThread(NULL, 0, RunProcess, hwnd, 0, &lpThreadId);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_VERSION), hwnd, DialogProc);
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		DragAcceptFiles(hwnd, TRUE);

		hmenu = GetMenu(hwnd);
		menuInfo.cbSize = sizeof(MENUITEMINFO);
		menuInfo.fState = MFS_UNCHECKED;



		GetClientRect(hwnd, &rect);



		hList = CreateWindowEx(
			0,
			WC_LISTVIEW,
			NULL,
			WS_CHILD | WS_VISIBLE | LVS_REPORT,
			0, 0, 0, rect.bottom / 2,
			hwnd,
			(HMENU)ID_LISTVIEW,
			hInst,
			NULL);

		if (!hList) break;

		LV_COLUMN col = {};

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 500;
		col.pszText = L"ファイル名";
		col.iSubItem = 0;
		SendMessage(hList, LVM_INSERTCOLUMN, 0, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
		col.cx = 100;
		col.iSubItem = 1;
		col.pszText = L"サイズ";
		SendMessage(hList, LVM_INSERTCOLUMN, 1, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 2;
		col.pszText = L"作成日時";
		SendMessage(hList, LVM_INSERTCOLUMN, 2, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 3;
		col.pszText = L"更新日時";
		SendMessage(hList, LVM_INSERTCOLUMN, 3, (LPARAM)&col);



		hLog = CreateWindowEx(
			0,
			WC_EDIT,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
			0, rect.bottom / 2, 0, rect.bottom / 2,
			hwnd,
			(HMENU)ID_LOGVIEW,
			hInst,
			NULL);

		if (!hLog) break;

		SendMessage(hLog, EM_SETLIMITTEXT, (WPARAM)255, 0);



		hStatus = CreateWindowEx(
			0,
			STATUSCLASSNAME,
			NULL,
			WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd,
			(HMENU)ID_STATUSBAR,
			hInst,
			NULL);

		if (!hStatus) break;

		SendMessage(hStatus, SB_SIMPLE, TRUE, 0L);



	}
	break;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);

		MoveWindow(hList, 0, 0, LOWORD(lParam), rect.bottom / 2, TRUE);
		MoveWindow(hLog, 0, rect.bottom / 2, LOWORD(lParam), rect.bottom / 2, TRUE);

		SendMessage(hStatus, WM_SIZE, wParam, lParam);
		break;
	case WM_CTLCOLOREDIT:
	{
		DWORD CtrlEsitoID = GetDlgCtrlID((HWND)lParam);
		if (CtrlEsitoID == ID_LOGVIEW) {
			HBRUSH hBrushCtrlBG = CreateSolidBrush(RGB(0, 0, 0));
			HDC hdcEdit = (HDC)wParam;
			SetTextColor(hdcEdit, RGB(255, 255, 255));	// 文字色：白
			SetBkMode(hdcEdit, TRANSPARENT);			// 背景色：黒
			return (LONG)hBrushCtrlBG;
		}
	}
	break;
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		TCHAR szFileName[512] = {};

		UINT uFileNo = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		for (INT i = 0; i < (INT)uFileNo; i++) {
			DragQueryFile(hDrop, i, szFileName, sizeof(szFileName));

			HANDLE hFile = CreateFile(
				szFileName,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hFile == INVALID_HANDLE_VALUE) continue;

			TCHAR szFileSize[512] = {};
			wsprintf(szFileSize, L"%d", GetFileSize(hFile, NULL));

			FILETIME lpCreationTime = {};
			FILETIME lpLastWriteTime = {};
			GetFileTime(hFile, &lpCreationTime, NULL, &lpLastWriteTime);

			SYSTEMTIME stFileTime = {};

			FileTimeToSystemTime(&lpCreationTime, &stFileTime);
			TCHAR szCreationTime[512] = {};
			wsprintf(szCreationTime, L"%d/%02d/%02d %02d:%02d:%02d",
				stFileTime.wYear, stFileTime.wMonth, stFileTime.wDay,
				stFileTime.wHour, stFileTime.wMinute, stFileTime.wSecond
			);

			FileTimeToSystemTime(&lpLastWriteTime, &stFileTime);
			TCHAR szLastWriteTime[512] = {};
			wsprintf(szLastWriteTime, L"%d/%02d/%02d %02d:%02d:%02d",
				stFileTime.wYear, stFileTime.wMonth,
				stFileTime.wDay, stFileTime.wHour,
				stFileTime.wMinute, stFileTime.wSecond
			);

			CloseHandle(hFile);

			LVITEM item = {};

			item.mask = LVIF_TEXT;
			item.pszText = szFileName;
			item.iItem = i;
			item.iSubItem = 0;
			ListView_InsertItem(hList, &item);

			item.pszText = szFileSize;
			item.iItem = i;
			item.iSubItem = 1;
			ListView_SetItem(hList, &item);

			item.pszText = szCreationTime;
			item.iItem = i;
			item.iSubItem = 2;
			ListView_SetItem(hList, &item);

			item.pszText = szLastWriteTime;
			item.iItem = i;
			item.iSubItem = 3;
			ListView_SetItem(hList, &item);
		}
		DragFinish(hDrop);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

DWORD WINAPI RunProcess(LPVOID lParam)
{
	// UNREFERENCED_PARAMETER(lpParameter);

	HANDLE hRead, hWrite, hProcess;

	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;	// パイプにハンドルを引き継ぐ

	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		if (CloseHandle(hRead)) hRead = NULL;
		if (CloseHandle(hWrite)) hWrite = NULL;

		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

		return FALSE;
	}

	if (!DuplicateHandle(
		GetCurrentProcess(),	// ソースプロセス
		hRead,					// duplicateするハンドル(オリジナルハンドル)
		GetCurrentProcess(),	// ターゲットプロセス(行先)
		NULL,					// 複製ハンドルへのポインタ(コピーハンドル)
		0,						// アクセス権
		FALSE,					// 子供がハンドルを継承するかどうか
		DUPLICATE_SAME_ACCESS)) // オプション
	{
		if (CloseHandle(hRead)) hRead = NULL;
		if (CloseHandle(hWrite)) hWrite = NULL;

		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

		return FALSE;
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;	// wShowWindow, hStdInput, hStdOutput, hStdErrorを有効にする
	si.wShowWindow = SW_HIDE;									// プロンプトを表示しない
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);				// ???
	si.hStdOutput = hWrite;
	si.hStdError = hWrite;
	// si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	ZeroMemory(&pi, sizeof(pi));

	TCHAR cmd[] = _T("E:\\App\\batch.bat");
	if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		if (CloseHandle(hRead)) hRead = NULL;
		if (CloseHandle(hWrite)) hWrite = NULL;

		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

		return FALSE;
	}

	hProcess = pi.hProcess;

	if (CloseHandle(pi.hThread)) pi.hThread = NULL;
	if (CloseHandle(hWrite)) hWrite = NULL;

	DWORD dwResult;

	// マルチバイト文字で返ってくるらしい
	CHAR buf[1024] = {};

	while (1)
	{
		BOOL bSuccess = ReadFile(hRead, buf, sizeof(buf), &dwResult, NULL);
		if (!bSuccess || dwResult == 0) break;

		TCHAR wcBuf[sizeof(buf) + 1] = {};
		int ret = MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			buf,
			sizeof(buf),
			wcBuf,
			sizeof(buf) + 1);

		int nCount = GetWindowTextLength(hLog);
		SendMessage(hLog, EM_SETSEL, nCount, nCount);
		SendMessage(hLog, EM_REPLACESEL, 0, (LPARAM)wcBuf);
	}

	if (CloseHandle(hRead)) hRead = NULL;
	if (CloseHandle(hProcess)) hProcess = NULL;

	SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

	// OutputDebugString(_T("終了\n"));

	return 0;
}