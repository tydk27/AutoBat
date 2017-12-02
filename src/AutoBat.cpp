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

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	static MENUITEMINFO menuInfo;

	static HANDLE hMainThread;

	switch (uMsg)
	{
	case WM_ENDTHREAD:
		// ���s�N���b�N���ɐ�������X���b�h�̏I�����ɌĂ΂��
		if (CloseHandle(hMainThread)) hMainThread = NULL;
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_SHUTDOWN:
			// �V���b�g�_�E���̃`�F�b�N�{�b�N�X����
			menuInfo.fMask = MIIM_STATE;
			menuInfo.fState = (menuInfo.fState == MFS_CHECKED) ? MFS_UNCHECKED : MFS_CHECKED;
			SetMenuItemInfo(hMenu, IDM_SHUTDOWN, FALSE, &menuInfo);
			break;
		case IDM_EXECUTE:
		{
			// ���s
			DWORD lpThreadId = 0;
			hMainThread = CreateThread(NULL, 0, RunProcess, hWnd, 0, &lpThreadId);
		}
		break;
		case IDM_SELECTFILE:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = "(*.bat)\0*.bat\0All Files(*.*)\0*.*\0\0";
			ofn.lpstrFile = filePath;
			ofn.nMaxFile = MAX_FILEPATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = "bat";
			ofn.nMaxFileTitle = MAX_FILEPATH;
			ofn.lpstrFileTitle = fileName;

			GetOpenFileName(&ofn);

			DWORD len = lstrlen(filePath);
			if (len > 0)
			{
				SetWindowText(hInput, filePath);
				// ToDo: �X�e�[�^�X�o�[�̕`�ʂ����������Ȃ�i������j
			}
		}
		break;
		case IDM_ABOUT:
			// �o�[�W�������
			DialogBox(hInst, MAKEINTRESOURCE(IDD_VERSION), hWnd, DialogProc);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		// �E�C���h�E����ʂ̒��S�ɕ\��
		DWORD w, h, x, y;
		RECT windowRect = { 0 };;

		w = GetSystemMetrics(SM_CXSCREEN);
		h = GetSystemMetrics(SM_CYSCREEN);

		GetWindowRect(hWnd, &windowRect);
		x = (w - (windowRect.right - windowRect.left)) / 2;
		y = (h - (windowRect.bottom - windowRect.top)) / 2;

		MoveWindow(hWnd, x, y, (windowRect.right - windowRect.left), (windowRect.bottom - windowRect.top), TRUE);



		// �h���b�O���\�ɂ���
		DragAcceptFiles(hWnd, TRUE);

		hMenu = GetMenu(hWnd);
		menuInfo.cbSize = sizeof(MENUITEMINFO);
		menuInfo.fState = MFS_UNCHECKED;



		// �N���C�A���g�̈�̃T�C�Y�擾
		// �㑱�ō쐬����E�C���h�E�̈ʒu�����p
		RECT clientRect = { 0 };;
		GetClientRect(hWnd, &clientRect);



		// LISTVIEW�E�C���h�E�쐬
		hList = CreateWindowEx(
			0,
			WC_LISTVIEW,
			NULL,
			WS_CHILD | WS_VISIBLE | LVS_REPORT,
			0, 0, 0, clientRect.bottom / 2,
			hWnd,
			(HMENU)ID_LISTVIEW,
			hInst,
			NULL);

		if (!hList) break;

		ListView_SetExtendedListViewStyleEx(hList, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);

		LV_COLUMN col = {};

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 500;
		col.pszText = _T("�t�@�C����");
		col.iSubItem = 0;
		SendMessage(hList, LVM_INSERTCOLUMN, 0, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
		col.cx = 100;
		col.iSubItem = 1;
		col.pszText = _T("�T�C�Y");
		SendMessage(hList, LVM_INSERTCOLUMN, 1, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 2;
		col.pszText = _T("�쐬����");
		SendMessage(hList, LVM_INSERTCOLUMN, 2, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 3;
		col.pszText = _T("�X�V����");
		SendMessage(hList, LVM_INSERTCOLUMN, 3, (LPARAM)&col);



		// EXE�o�͕\���p��EDIT�E�C���h�E�쐬
		hLog = CreateWindowEx(
			0,
			WC_EDIT,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
			0, clientRect.bottom / 2, 0, (clientRect.bottom / 2) - INPUT_HEIGHT,
			hWnd,
			(HMENU)ID_LOG,
			hInst,
			NULL);

		if (!hLog) break;



		// �o�b�`�t�@�C�����͗p��EDIT�E�C���h�E�쐬
		hInput = CreateWindowEx(
			0,
			WC_EDIT,
			NULL,
			WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN,
			0, clientRect.bottom - INPUT_HEIGHT, 0, INPUT_HEIGHT,
			hWnd,
			(HMENU)ID_INPUT,
			hInst,
			NULL);

		if (!hInput) break;

		SendMessage(hInput, EM_SETLIMITTEXT, (WPARAM)INPUT_LIMIT, 0);

		// �T�u�N���X�Ńt�@�C���I���_�C�A���O���o����
		// defEditWndProc = (WNDPROC)SetWindowLongPtr(hInput, GWLP_WNDPROC, (LONG_PTR)EditWindowProc);


		// �X�e�[�^�X�o�[�쐬
		hStatus = CreateWindowEx(
			0,
			STATUSCLASSNAME,
			NULL,
			WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE,
			0, 0, 0, 0,
			hWnd,
			(HMENU)ID_STATUSBAR,
			hInst,
			NULL);

		if (!hStatus) break;

		SendMessage(hStatus, SB_SIMPLE, TRUE, 0L);
	}
	break;
	case WM_SIZE:
	{
		// �E�C���h�E�T�C�Y�ύX���̒���
		RECT rect = { 0 };
		GetClientRect(hWnd, &rect);

		MoveWindow(hList, 0, 0, LOWORD(lParam), rect.bottom / 2, TRUE);
		MoveWindow(hLog, 0, rect.bottom / 2, LOWORD(lParam), (rect.bottom / 2) - INPUT_HEIGHT, TRUE);
		MoveWindow(hInput, 0, rect.bottom - INPUT_HEIGHT, LOWORD(lParam), INPUT_HEIGHT, TRUE);

		SendMessage(hStatus, WM_SIZE, wParam, lParam);
	}
	break;
	case WM_CTLCOLOREDIT:
	{
		// �o�O��̂ŃR�����g�A�E�g
		/*
		DWORD CtrlEsitoID = GetDlgCtrlID((HWND)lParam);
		if (CtrlEsitoID == ID_LOGVIEW) {
		HBRUSH hBrushCtrlBG = CreateSolidBrush(RGB(0, 0, 0));
		HDC hdcEdit = (HDC)wParam;
		SetTextColor(hdcEdit, RGB(255, 255, 255));	// �����F�F��
		SetBkMode(hdcEdit, TRANSPARENT);	// �w�i�F�F��
		return (LONG)hBrushCtrlBG;
		}
		*/
	}
	break;
	case WM_DROPFILES:
	{
		// �t�@�C�����h���b�v���ꂽ��A���O�Ƃ��T�C�Y���擾���ă��X�g�r���[�̃J�����ɕ\��
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
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

/*
LRESULT CALLBACK EditWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN && wParam == VK_RETURN) {
		DWORD textLen = GetWindowTextLength(hWnd);
		LPTSTR lpString = (LPTSTR)malloc(textLen + 32);
		GetWindowText(hWnd, lpString, textLen + 32);

		MessageBox(NULL, lpString, _T("�t�@�C����"), MB_OK);

		free(lpString);

		return 0;
	}
	return CallWindowProc(defEditWndProc, hWnd, uMsg, wParam, lParam);
}
*/

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
	// UNREFERENCED_PARAMETER(lParam);

	HANDLE hOutputReadTmp, hOutputRead, hOutputWrite;
	HANDLE hErrorWrite;

	HANDLE hChildProcess = NULL;

	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;	// �p�C�v�Ƀn���h���������p��

	// �q�̏o�̓p�C�v�쐬
	if (!CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0))
	{
		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);
		return FALSE;
	}

	if (!DuplicateHandle(
		GetCurrentProcess(),
		hOutputWrite,
		GetCurrentProcess(),
		&hErrorWrite,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
	{
		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);
		return FALSE;
	}

	// �o�͓ǂݎ��p�̃n���h����V�K�쐬
	if (!DuplicateHandle(
		GetCurrentProcess(),
		hOutputReadTmp,
		GetCurrentProcess(),
		&hOutputRead,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS))
	{
		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);
		return FALSE;
	}

	if (CloseHandle(hOutputReadTmp)) hOutputReadTmp = NULL;



	// �v���Z�X�쐬
	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	// si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = hOutputWrite;
	si.hStdError = hErrorWrite;

	PROCESS_INFORMATION pi = { 0 };

	TCHAR cmd[] = _T("E:\\App\\batch2.bat"); // test
	if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);
		return FALSE;
	}

	hChildProcess = pi.hProcess;

	if (CloseHandle(pi.hThread)) pi.hThread = NULL;



	// �s�v�ȃn���h�����N���[�Y
	if (CloseHandle(hOutputWrite)) hOutputWrite = NULL;
	if (CloseHandle(hErrorWrite)) hErrorWrite = NULL;



	// �q�̏o�͂�e�̃E�C���h�E�ɏo�͂���
	TCHAR pipeBuf[MAX_PIPE_BUFFER] = { 0 };
	DWORD pipeLen = 0;

	while (1)
	{
		DWORD nBytesRead = 0;

		if (!PeekNamedPipe(hOutputRead, NULL, 0, NULL, &nBytesRead, NULL)) break;

		if (nBytesRead)
		{
			TCHAR tmpBuf[MAX_PIPE_BUFFER] = { 0 };
			DWORD lpNumberOfBytesRead = 0;

			// 1�o�C�g���ǂ�ł݂�
			BOOL bSuccess = ReadFile(hOutputRead, tmpBuf, 1, &lpNumberOfBytesRead, NULL);
			if (!bSuccess || lpNumberOfBytesRead == 0)
			{
				break;
			}

			// �ǂݍ��񂾃o�b�t�@�����s�R�[�h�Ȃ�
			// ���߂��o�b�t�@�����X�g�r���[�ɏo�͂���
			if (tmpBuf[0] == '\r' || tmpBuf[0] == '\n')
			{
				// if (pipeLen == 0) continue;

				pipeBuf[pipeLen] = '\0';
				// strcat_s(pipeBuf, sizeof(tmpBuf), tmpBuf);

				DWORD textLen = GetWindowTextLength(hLog);

				/*
				LPTSTR lpString = (LPTSTR)malloc(textLen + 32);
				GetWindowText(hLog, lpString, textLen + 32);
				free(lpString);
				*/

				SendMessage(hLog, EM_SETSEL, (WPARAM)textLen, (LPARAM)textLen);
				SendMessage(hLog, EM_REPLACESEL, (WPARAM)false, (LPARAM)pipeBuf);
				if (tmpBuf[0] == '\r')
				{
					SendMessage(hLog, EM_REPLACESEL, (WPARAM)false, (LPARAM)"\r\n");
				}

				ZeroMemory(&pipeBuf, sizeof(pipeBuf));
				pipeLen = 0;
			}
			else
			{
				// ���s�R�[�h������܂Ńo�b�t�@�ɒ��߂�
				pipeLen += lpNumberOfBytesRead;
				strcat_s(pipeBuf, sizeof(tmpBuf), tmpBuf);

				ZeroMemory(&tmpBuf, sizeof(tmpBuf));
			}
		}
	}

	if (CloseHandle(hOutputRead)) hOutputRead = NULL;

	SendMessage((HWND)lParam, WM_ENDTHREAD, 0, 0);

	return 0;
}