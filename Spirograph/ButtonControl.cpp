#include "stdafx.h"
#include "CommonControls.h"
#include <commctrl.h>

using namespace win32helper;

ButtonControl::ButtonControl(int x, int y, int width, int height, Control* parent, int id, HINSTANCE hInstance, LPCTSTR text)
{
	Create(0, text, WS_TABSTOP | WS_CHILD | WS_VISIBLE, x, y, width, height, parent, id, hInstance, nullptr);
}

LPCTSTR ButtonControl::GetWindowClass(HINSTANCE hInstance)
{
	return WC_BUTTON;
}

