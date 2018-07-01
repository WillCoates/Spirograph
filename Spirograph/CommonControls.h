#pragma once
#include "Control.h"

// A bunch of controls Windows defines for us
// I don't implement full support, only support for the functions I need
namespace win32helper
{
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb775458(v=vs.85).aspx
	class EditControl : public Control
	{
	public:
		EditControl(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance);
	protected:
		virtual LPCTSTR GetWindowClass(HINSTANCE hInstance);
	};

	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb775146(v=vs.85).aspx
	class ListBoxControl : public Control
	{
	public:
		ListBoxControl(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance);

		int Add(LPCTSTR text) const;
		int Delete(int index) const;
		void Clear() const;
		int GetCount() const;

		void *GetItemData(int index) const;
		bool SetItemData(int index, void *data) const;

		void Select(int index) const;
		int IsSelected(int index) const;
		int GetSelectedCount() const;
		int GetSelectedItems(int *buffer, int count) const;
		int GetSelectedItem() const;
	protected:
		virtual LPCTSTR GetWindowClass(HINSTANCE hInstance);
	};

	// I noticed this when I found about the constants in commctrl.h
	class StaticControl : public Control
	{
	public:
		StaticControl(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance, LPCTSTR text);
	protected:
		virtual LPCTSTR GetWindowClass(HINSTANCE hInstance);
	};

	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh298342(v=vs.85).aspx
	class ButtonControl : public Control
	{
	public:
		ButtonControl(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance, LPCTSTR text);
	protected:
		virtual LPCTSTR GetWindowClass(HINSTANCE hInstance);
	};
}