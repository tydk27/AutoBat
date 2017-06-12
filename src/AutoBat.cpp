#include "AutoBat.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_AUTOBAT, szWindowClass, MAX_LOADSTRING);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUTOBAT));
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOBAT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AUTOBAT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		nullptr,
		nullptr,
		hInst,
		nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	MakeWindow mw;
	mw.MakeListView(hWnd, hInst, &hList);
	mw.MakeStatusBar(hWnd, hInst, &hStatus);

	if (!hList || !hStatus)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_CREATE:
		DragAcceptFiles(hWnd, TRUE);
		break;
	case WM_SIZE:
		MoveWindow(hList, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		SendMessage(hStatus, WM_SIZE, wParam, lParam);
		break;
	case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP)wParam;
			TCHAR szFileName[512] = { 0 };
	
			UINT uFileNo = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
			for (int i = 0; i < (int)uFileNo; i++) {
				DragQueryFile(hDrop, i, szFileName, sizeof(szFileName));
	
				HANDLE hFile = CreateFile(
					szFileName,
					GENERIC_READ,
					FILE_SHARE_READ,
					nullptr,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					nullptr);
	
				if (hFile == INVALID_HANDLE_VALUE) {
					continue;
				}
	
				TCHAR szFileSize[512] = { 0 };
				wsprintf(szFileSize, L"%d", GetFileSize(hFile, nullptr));
	
				FILETIME lpCreationTime = { 0 };
				FILETIME lpLastWriteTime = { 0 };
				GetFileTime(hFile, &lpCreationTime, nullptr, &lpLastWriteTime);

				SYSTEMTIME stFileTime = { 0 };

				FileTimeToSystemTime(&lpCreationTime, &stFileTime);
				TCHAR szCreationTime[512] = { 0 };
				wsprintf(szCreationTime, L"%d/%02d/%02d %02d:%02d:%02d",
					stFileTime.wYear, stFileTime.wMonth, stFileTime.wDay,
					stFileTime.wHour, stFileTime.wMinute, stFileTime.wSecond
				);
	
				FileTimeToSystemTime(&lpLastWriteTime, &stFileTime);
				TCHAR szLastWriteTime[512] = { 0 };
				wsprintf(szLastWriteTime, L"%d/%02d/%02d %02d:%02d:%02d",
					stFileTime.wYear, stFileTime.wMonth,
					stFileTime.wDay, stFileTime.wHour,
					stFileTime.wMinute, stFileTime.wSecond
				);
	
				CloseHandle(hFile);
	
				LVITEM item = { 0 };
	
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
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
