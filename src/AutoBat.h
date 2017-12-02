#pragma once

#include "Resource.h"
#include "commdlg.h"

#define MAX_LOADSTRING 128
#define MAX_FILEPATH 512
#define MAX_PIPE_BUFFER 4096
#define INPUT_HEIGHT 45
#define INPUT_LIMIT 64

#define ID_LISTVIEW 10001
#define ID_STATUSBAR 10002
#define ID_LOG 10003
#define ID_INPUT 10004

#define WM_ENDTHREAD (WM_APP + 1)

TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

TCHAR filePath[MAX_FILEPATH] = { 0 };
TCHAR fileName[MAX_FILEPATH] = { 0 };

HINSTANCE hInst;

HWND hList, hStatus, hLog, hInput;

// WNDPROC defEditWndProc;

BOOL                InitInstance(HINSTANCE, INT);
LRESULT CALLBACK    WindowProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogProc(HWND, UINT, WPARAM, LPARAM);
// LRESULT CALLBACK	EditWindowProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI		RunProcess(LPVOID);