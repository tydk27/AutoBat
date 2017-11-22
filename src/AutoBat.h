#pragma once

#include "Resource.h"

#define MAX_LOADSTRING 128

#define ID_LISTVIEW 101
#define ID_STATUSBAR 102
#define ID_LOGVIEW 103

#define WM_ENDTHREAD (WM_APP + 1)

WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HINSTANCE hInst;

HWND hList, hStatus, hLog;

HANDLE hThread;

BOOL                InitInstance(HINSTANCE, INT);
LRESULT CALLBACK    WindowProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI		RunProcess(LPVOID);