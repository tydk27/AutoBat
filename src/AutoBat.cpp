#include "AutoBat.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_AUTOBAT, szWindowClass, MAX_LOADSTRING);

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
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_AUTOBAT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindowEx(
		0,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
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
		//スクリーンの幅、高さ、座標
		DWORD w, h, x, y;
		RECT rc;

		w = GetSystemMetrics(SM_CXSCREEN);
		h = GetSystemMetrics(SM_CYSCREEN);

		GetWindowRect(hwnd, &rc);
		x = (w - (rc.right - rc.left)) / 2;
		y = (h - (rc.bottom - rc.top)) / 2;

		MoveWindow(hwnd, x, y, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);



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

		ListView_SetExtendedListViewStyleEx(hList, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);

		LV_COLUMN col = {};

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 500;
		col.pszText = _T("ファイル名");
		col.iSubItem = 0;
		SendMessage(hList, LVM_INSERTCOLUMN, 0, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
		col.cx = 100;
		col.iSubItem = 1;
		col.pszText = _T("サイズ");
		SendMessage(hList, LVM_INSERTCOLUMN, 1, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 2;
		col.pszText = _T("作成日時");
		SendMessage(hList, LVM_INSERTCOLUMN, 2, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 3;
		col.pszText = _T("更新日時");
		SendMessage(hList, LVM_INSERTCOLUMN, 3, (LPARAM)&col);



		hLog = CreateWindowEx(
			0,
			WC_EDIT,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
			0, rect.bottom / 2, 0, rect.bottom / 2,
			hwnd,
			(HMENU)ID_LOGVIEW,
			hInst,
			NULL);

		if (!hLog) break;



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
		/*
		DWORD CtrlEsitoID = GetDlgCtrlID((HWND)lParam);
		if (CtrlEsitoID == ID_LOGVIEW) {
			HBRUSH hBrushCtrlBG = CreateSolidBrush(RGB(0, 0, 0));
			HDC hdcEdit = (HDC)wParam;
			SetTextColor(hdcEdit, RGB(255, 255, 255));	// 文字色：白
			SetBkMode(hdcEdit, TRANSPARENT);	// 背景色：黒
			return (LONG)hBrushCtrlBG;
		}
		*/
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

			TCHAR szFileSize[512] = { 0 };
			wsprintf(szFileSize, _T("%d"), GetFileSize(hFile, NULL));

			FILETIME lpCreationTime = {};
			FILETIME lpLastWriteTime = {};
			GetFileTime(hFile, &lpCreationTime, NULL, &lpLastWriteTime);

			SYSTEMTIME stFileTime = {};

			FileTimeToSystemTime(&lpCreationTime, &stFileTime);
			TCHAR szCreationTime[512] = {};
			wsprintf(szCreationTime, _T("%d/%02d/%02d %02d:%02d:%02d"),
				stFileTime.wYear, stFileTime.wMonth, stFileTime.wDay,
				stFileTime.wHour, stFileTime.wMinute, stFileTime.wSecond
			);

			FileTimeToSystemTime(&lpLastWriteTime, &stFileTime);
			TCHAR szLastWriteTime[512] = {};
			wsprintf(szLastWriteTime, _T("%d/%02d/%02d %02d:%02d:%02d"),
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
	UNREFERENCED_PARAMETER(lParam);

	HANDLE hOutputReadTmp, hOutputRead, hOutputWrite;
	HANDLE hErrorWrite;

	HANDLE hProcess;

	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;	// パイプにハンドルを引き継ぐ

	if (!CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0))
	{
		if (CloseHandle(hOutputReadTmp)) hOutputReadTmp = NULL;
		if (CloseHandle(hOutputWrite)) hOutputWrite = NULL;

		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

		return FALSE;
	}

	if (!DuplicateHandle(
		GetCurrentProcess(),
		hOutputWrite,
		GetCurrentProcess(),
		&hErrorWrite,
		0, TRUE, DUPLICATE_SAME_ACCESS))
	{
		if (CloseHandle(hOutputReadTmp)) hOutputReadTmp = NULL;
		if (CloseHandle(hOutputWrite)) hOutputWrite = NULL;
		if (CloseHandle(hErrorWrite)) hErrorWrite = NULL;

		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

		return FALSE;
	}

	if (!DuplicateHandle(
		GetCurrentProcess(),
		hOutputReadTmp,
		GetCurrentProcess(),
		&hOutputRead,
		0,
		FALSE, // Make it uninheritable.
		DUPLICATE_SAME_ACCESS))
	{
		if (CloseHandle(hOutputReadTmp)) hOutputReadTmp = NULL;
		if (CloseHandle(hOutputRead)) hOutputRead = NULL;
		if (CloseHandle(hOutputWrite)) hOutputWrite = NULL;
		if (CloseHandle(hErrorWrite)) hErrorWrite = NULL;

		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

		return FALSE;
	}

	if (CloseHandle(hOutputReadTmp)) hOutputReadTmp = NULL;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = hOutputWrite;
	si.hStdError = hErrorWrite;

	ZeroMemory(&pi, sizeof(pi));

	TCHAR cmd[] = _T("E:\\Encode\\l-smash_r1450\\remuxer.exe -i F:\\test2.mp4?1:remove -o F:\\demuxed\\test2.m4a");
	// TCHAR cmd[] = _T("E:\\App\\batch.bat");
	if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		if (CloseHandle(hOutputRead)) hOutputRead = NULL;
		if (CloseHandle(hOutputWrite)) hOutputWrite = NULL;
		if (CloseHandle(hErrorWrite)) hErrorWrite = NULL;

		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

		return FALSE;
	}

	hProcess = pi.hProcess;

	if (CloseHandle(pi.hThread)) pi.hThread = NULL;

	if (CloseHandle(hOutputWrite)) hOutputWrite = NULL;
	if (CloseHandle(hErrorWrite)) hErrorWrite = NULL;

	DWORD dwResult = 0;
	TCHAR tmp[1024] = { 0 };
	TCHAR buf[1024] = { 0 };

	DWORD len = 0;

	while (1)
	{
		if (!PeekNamedPipe(hOutputRead, NULL, 0, NULL, &dwResult, NULL)) break;

		if (dwResult)
		{
			BOOL bSuccess = ReadFile(hOutputRead, tmp, 1, &dwResult, NULL);
			if (!bSuccess || dwResult == 0) break;

			// 読み込んだバッファが改行コードなら
			if (tmp[dwResult - 1] == '\r' || tmp[dwResult - 1] == '\n')
			{

				/*
				// 空白削除
				int i = len;
				int count = 0;
				while (--i >= 0 && buf[i] == ' ') count++;
				i = 0;
				while (buf[i] != '\0' && buf[i] == ' ') i++;

				if ((i > 0 && count == 0) || (i > 0 && count > 0))
				{
					len = 0;
					ZeroMemory(&buf, sizeof(buf));
					continue;
				}
				*/

				buf[len] = '\0';
				//strcat_s(buf, sizeof(tmp), tmp);

				DWORD len = SendMessage(hLog, WM_GETTEXTLENGTH, 0, 0);
				SendMessage(hLog, EM_SETSEL, (WPARAM)len, (LPARAM)len);

				//TCHAR tmpLen[32] = {};
				//sprintf_s(tmpLen, sizeof(tmpLen), "%d\n", len);
				//OutputDebugString(tmpLen);

				SendMessage(hLog, EM_REPLACESEL, (WPARAM)false, (LPARAM)buf);
				SendMessage(hLog, EM_REPLACESEL, (WPARAM)false, (LPARAM)_T("\r\n"));



				/*
				// 復帰文字をコンソール同様に表示してみたかった
				DWORD logLen = GetWindowTextLength(hLog) + 128;
				LPSTR buf2 = (LPSTR)malloc(logLen);;
				GetWindowText(hLog, buf2, logLen);

				int i = logLen - 128;
				CHAR test = {};
				if (logLen > 128 && tmp[dwResult - 1] == '\r')
				{
					while (--i >= 0 && buf2[i - 1] != '\r');
					test = buf2[i - 1];
				}
				if (i > 0)
				{
					buf2[i] = '\0';
					GetWindowText(hLog, buf2, i);
				}

				free(buf2);
				*/

				len = 0;
				ZeroMemory(&buf, sizeof(buf));
			}
			else
			{
				len += dwResult;
				strcat_s(buf, sizeof(tmp), tmp);
				ZeroMemory(&tmp, sizeof(tmp));
			}
		}
	}

	if (CloseHandle(hOutputRead)) hOutputRead = NULL;

	SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

	return 0;
}
