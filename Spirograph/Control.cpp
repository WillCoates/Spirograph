#include "stdafx.h"
#include "Control.h"

using namespace win32helper;

Control::Control()
{
}

bool Control::Create(DWORD exStyle, LPCTSTR name, DWORD style, int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND hParent = nullptr;
	if (parent != nullptr)
	{
		hParent = parent->GetHWnd();
	}

	_hWnd = CreateWindowEx(exStyle,
		GetWindowClass(hInstance),
		name,
		style,
		x, y,
		width, height,
		hParent,
		reinterpret_cast<HMENU>(id),
		hInstance,
		lpParam);

	SetProperty(L"CONTROL_CLASS", this);

	return true;
}

Control::~Control()
{
}

HWND Control::GetHWnd() const
{
	return _hWnd;
}

BOOL Control::GetClientRect(LPRECT lpRect) const
{
	return ::GetClientRect(_hWnd, lpRect);
}

void Control::GetParent() const
{
	::GetParent(_hWnd);
}

void Control::SetParent(Control* parent) const
{
	HWND hParent = nullptr;
	if (parent != nullptr)
	{
		hParent = parent->GetHWnd();
	}
	::SetParent(_hWnd, hParent);
}

BOOL Control::EnumChildren(WNDENUMPROC lpEnumFunc, LPARAM lParam) const
{
	return EnumChildWindows(_hWnd, lpEnumFunc, lParam);
}

LPTSTR Control::GetText() const
{
	int len = GetTextLength();
	LPTSTR text = new TCHAR[len+1];
	GetText(text, len+1);
	//text[len] = 0;
	return text;
}

int Control::GetText(LPTSTR text, int max) const
{
	return GetWindowText(_hWnd, text, max);
}

int Control::GetTextLength() const
{
	return GetWindowTextLength(_hWnd);
}

BOOL Control::SetText(LPCTSTR text) const
{
	return SetWindowText(_hWnd, text);
}

BOOL Control::Show() const
{
	return ShowWindow(_hWnd, SW_SHOWNOACTIVATE);
}

BOOL Control::Show(int nCmdShow) const
{
	return ShowWindow(_hWnd, nCmdShow);
}

BOOL Control::Hide() const
{
	return ShowWindow(_hWnd, SW_HIDE);
}

BOOL Control::Enable() const
{
	return EnableWindow(_hWnd, true);
}

BOOL Control::Enable(bool enabled) const
{
	return EnableWindow(_hWnd, enabled);
}

BOOL Control::Disable() const
{
	return EnableWindow(_hWnd, false);
}

LONG_PTR Control::SetAttribute(int index, LONG_PTR value) const
{
	return SetWindowLongPtr(_hWnd, index, value);
}

LONG_PTR Control::GetAttribute(int index) const
{
	return GetWindowLongPtr(_hWnd, index);
}

BOOL Control::SetProperty(LPCTSTR name, HANDLE data) const
{
	return SetProp(_hWnd, name, data);
}

HANDLE Control::GetProperty(LPCTSTR name) const
{
	return GetProp(_hWnd, name);
}

BOOL Control::Destroy() const
{
	return DestroyWindow(_hWnd);
}

BOOL Control::Update() const
{
	return UpdateWindow(_hWnd);
}

BOOL Control::Invalidate() const
{
	return InvalidateRect(_hWnd, nullptr, true);
}

BOOL Control::SetPosition(int x, int y) const
{
	return SetWindowPos(_hWnd, nullptr, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSIZE);
}

BOOL Control::SetPosition(int x, int y, int w, int h) const
{
	return SetWindowPos(_hWnd, nullptr, x, y, w, h, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

BOOL Control::SetSize(int w, int h) const
{
	return SetWindowPos(_hWnd, nullptr, 0, 0, w, h, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE);
}

void Control::Focus() const
{
	SetFocus(_hWnd);
}

LRESULT Control::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(GetHWnd(), msg, wParam, lParam);
}


// WindowProcProxy
// Win32 only supports the stdcall calling convention (specified by CALLBACK)
// Classes in C++ only support the thiscall calling convention
// Controls should use this whenever registering their own Window Class and set the
// CONTROL_CLASS prop to the this pointer
// Begin https://stackoverflow.com/questions/17221815/why-cant-my-wndproc-be-in-a-class
LRESULT CALLBACK win32helper::WindowProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Control *control = reinterpret_cast<Control*>(GetProp(hWnd, L"CONTROL_CLASS"));
	if (control)
	{
		return control->WindowProc(msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
// End https://stackoverflow.com/questions/17221815/why-cant-my-wndproc-be-in-a-class