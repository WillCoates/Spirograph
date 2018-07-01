#include "stdafx.h"
#include "CommonControls.h"
#include <CommCtrl.h>
using namespace win32helper;

StaticControl::StaticControl(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance, LPCTSTR text)
{
	Create(0, text, WS_CHILD | WS_VISIBLE, x, y, width, height, parent, id, hInstance, nullptr);
}

LPCTSTR StaticControl::GetWindowClass(HINSTANCE hInstance)
{
	return WC_STATIC;
}
