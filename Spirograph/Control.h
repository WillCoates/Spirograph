#pragma once

// NOTE
// All documentation used to create this class is from MSDN
// Docs:
// Window classes and attributes: https://msdn.microsoft.com/en-us/library/windows/desktop/ff468926(v=vs.85).aspx
// Window: https://msdn.microsoft.com/en-us/library/windows/desktop/ff468919(v=vs.85).aspx
// Window procedures: https://msdn.microsoft.com/en-us/library/windows/desktop/ff468930(v=vs.85).aspx
// Window properties: https://msdn.microsoft.com/en-us/library/windows/desktop/ff468933(v=vs.85).aspx

namespace win32helper
{	
	class Control
	{
	public:
		Control();
		virtual ~Control();

		HWND GetHWnd() const;

		BOOL GetClientRect(LPRECT lpRect) const;

		void GetParent() const;
		void SetParent(Control *parent) const;
		// TODO If we ever use this, we might implement a helper of some form
		BOOL EnumChildren(WNDENUMPROC lpEnumFunc, LPARAM lParam) const;

		LPTSTR GetText() const;
		int GetText(LPTSTR text, int max) const;
		int GetTextLength() const;
		BOOL SetText(LPCTSTR text) const;

		BOOL Show() const;
		BOOL Show(int nCmdShow) const;
		BOOL Hide() const;

		BOOL Enable() const;
		BOOL Enable(bool enabled) const;
		BOOL Disable() const;

		LONG_PTR SetAttribute(int index, LONG_PTR value) const;
		LONG_PTR GetAttribute(int index) const;

		BOOL SetProperty(LPCTSTR name, HANDLE data) const;
		HANDLE GetProperty(LPCTSTR name) const;

		BOOL Destroy() const;
		BOOL Update() const;
		BOOL Invalidate() const;

		BOOL SetPosition(int x, int y) const;
		BOOL SetPosition(int x, int y, int w, int h) const;
		BOOL SetSize(int w, int h) const;

		void Focus() const;

	protected:
		// Used to handle events, only controls should be able to see this
		virtual LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
		// Aswell as the code responsible for calling this (Win32 can't handle this being in a class)
		friend LRESULT CALLBACK WindowProcProxy(HWND, UINT, WPARAM, LPARAM);

		// When implementing this, a static method should be used to register the windows class ONCE if needed
		virtual LPCTSTR GetWindowClass(HINSTANCE hInstance) = 0;

		// HACK We can't call GetWindowClass in our constructor (it wouldn't be defined), so we'll rely on controls calling this method once they're ready to create the window
		bool Create(DWORD exStyle, LPCTSTR name, DWORD style, int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance, LPVOID lpParam);
	private:
		static Control *_none;
		HWND _hWnd;
	};

	LRESULT CALLBACK WindowProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
