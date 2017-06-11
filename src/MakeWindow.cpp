#include "stdafx.h"
#include "MakeWindow.h"

MakeWindow::MakeWindow()
{
	InitCommonControls();
}

MakeWindow::~MakeWindow()
{
}

void MakeWindow::MakeListView(HWND hAppWnd, HINSTANCE hInst, HWND *hList)
{
	*hList = CreateWindowEx(
		WS_EX_ACCEPTFILES,
		WC_LISTVIEW,
		nullptr,
		WS_CHILD | WS_VISIBLE | LVS_REPORT,
		0, 0, 0, 0,
		hAppWnd,
		(HMENU)ID_LISTVIEW,
		hInst,
		nullptr);

	if (*hList != nullptr)
	{
		LV_COLUMN col = { 0 };

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 500;
		col.pszText = L"ファイル名";
		col.iSubItem = 0;
		SendMessage(*hList, LVM_INSERTCOLUMN, 0, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
		col.cx = 100;
		col.iSubItem = 1;
		col.pszText = L"サイズ";
		SendMessage(*hList, LVM_INSERTCOLUMN, 1, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 2;
		col.pszText = L"作成日時";
		SendMessage(*hList, LVM_INSERTCOLUMN, 2, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		col.cx = 100;
		col.iSubItem = 3;
		col.pszText = L"更新日時";
		SendMessage(*hList, LVM_INSERTCOLUMN, 3, (LPARAM)&col);
	}
}

void MakeWindow::MakeStatusBar(HWND hAppWnd, HINSTANCE hInst, HWND *hStatus)
{
	*hStatus = CreateWindowEx(
		0,
		STATUSCLASSNAME,
		nullptr,
		WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE,
		0, 0, 0, 0,
		hAppWnd,
		(HMENU)ID_STATUSBAR,
		hInst,
		nullptr);
}