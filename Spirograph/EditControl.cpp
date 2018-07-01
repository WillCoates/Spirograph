#include "stdafx.h"
#include "CommonControls.h"
#include <CommCtrl.h>
using namespace win32helper;

EditControl::EditControl(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance)
{
	Create(WS_EX_CLIENTEDGE, L"", WS_TABSTOP | WS_CHILD | WS_VISIBLE, x, y, width, height, parent, id, hInstance, nullptr);
}

LPCTSTR EditControl::GetWindowClass(HINSTANCE hInstance)
{
	return WC_EDIT;
}
