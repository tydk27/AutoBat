#pragma once

#include "stdafx.h"

#define ID_LISTVIEW 101
#define ID_STATUSBAR 102

class MakeWindow
{
public:
	MakeWindow();
	~MakeWindow();

	void MakeListView(HWND, HINSTANCE, HWND*);
	void MakeStatusBar(HWND, HINSTANCE, HWND*);
};

