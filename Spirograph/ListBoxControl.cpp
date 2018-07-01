#include "stdafx.h"
#include "CommonControls.h"
#include <CommCtrl.h>
using namespace win32helper;

ListBoxControl::ListBoxControl(int x, int y, int width, int height, Control* parent, int id, HINSTANCE hInstance)
{
	Create(WS_EX_CLIENTEDGE, L"", WS_TABSTOP | WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_OWNERDRAWFIXED, x, y, width, height, parent, id, hInstance, nullptr);
}

LPCTSTR ListBoxControl::GetWindowClass(HINSTANCE hInstance)
{
	return WC_LISTBOX;
}

int ListBoxControl::Add(LPCTSTR text) const
{
	return SendMessage(GetHWnd(), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text));
}

int ListBoxControl::Delete(int index) const
{
	return SendMessage(GetHWnd(), LB_DELETESTRING, index, 0);
}

void ListBoxControl::Clear() const
{
	SendMessage(GetHWnd(), LB_RESETCONTENT, 0, 0);
}


int ListBoxControl::GetCount() const
{
	return SendMessage(GetHWnd(), LB_GETCOUNT, 0, 0);
}

void* ListBoxControl::GetItemData(int index) const
{
	return reinterpret_cast<void*>(SendMessage(GetHWnd(), LB_GETITEMDATA, index, 0));
}

bool ListBoxControl::SetItemData(int index, void* data) const
{
	return SendMessage(GetHWnd(), LB_SETITEMDATA, index, reinterpret_cast<LPARAM>(data));
}

void ListBoxControl::Select(int index) const
{
	SendMessage(GetHWnd(), LB_SETCURSEL, index, 0);
}

int ListBoxControl::IsSelected(int index) const
{
	return SendMessage(GetHWnd(), LB_GETSEL, index, 0);
}

int ListBoxControl::GetSelectedCount() const
{
	return SendMessage(GetHWnd(), LB_GETSELCOUNT, 0, 0);
}

int ListBoxControl::GetSelectedItems(int* buffer, int count) const
{
	return SendMessage(GetHWnd(), LB_GETSELITEMS, count, reinterpret_cast<LPARAM>(buffer));
}

int ListBoxControl::GetSelectedItem() const
{
	return SendMessage(GetHWnd(), LB_GETCURSEL, 0, 0);
}
