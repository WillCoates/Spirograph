#include "stdafx.h"
#include "Plotter.h"
#include <string>
#include <cassert>

using namespace win32helper;
using namespace Gdiplus;

static const LPCTSTR WINDOW_CLASS = L"PlotterControl";
const Color COLOR_WHITE = Color(255, 255, 255);
const Color COLOR_BLACK = Color(0, 0, 0);

Plotter::Plotter(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance) : _plotSet(nullptr), _bitmap(nullptr), _shouldRedraw(true)
{
	Create(WS_EX_CLIENTEDGE, L"Spirograph", WS_CHILD | WS_VISIBLE, x, y, width, height, parent, id, hInstance, nullptr);
}

Plotter::~Plotter()
{
}

void Plotter::Save(LPCTSTR filename, const LPCLSID clsId)
{
	_bitmap->Save(filename, clsId);
}

void Plotter::Redraw()
{
	_shouldRedraw = true;
	Invalidate(); // Tell Windows that all the graphical data we painted is out of date
}

void Plotter::SetPlotSet(PlotSet* plotSet)
{
	_plotSet = plotSet;
	Redraw();
}

PlotSet* Plotter::GetPlotSet() const
{
	return _plotSet;
}

static void RegisterWindowClass(HINSTANCE hInstance)
{
	// Static variable is persisted accross calls
	static bool registered = false;
	if (registered) return;
	registered = true;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = win32helper::WindowProcProxy;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszClassName = WINDOW_CLASS;

	RegisterClassEx(&wc);
}

LPCTSTR Plotter::GetWindowClass(HINSTANCE hInstance)
{
	RegisterWindowClass(hInstance);
	return WINDOW_CLASS;
}

void Plotter::Paint(HDC hdc)
{
	assert(_plotSet != nullptr);

	if (_bitmap == nullptr || _bitmap->GetWidth() != _plotSet->GetWidth() || _bitmap->GetHeight() != _plotSet->GetHeight())
	{
		if (_bitmap != nullptr) delete _bitmap;
		_bitmap = new Bitmap(_plotSet->GetWidth(), _plotSet->GetHeight(), PixelFormat32bppARGB);
		_shouldRedraw = true;
	}

	// Only rerender the spirograph if we know it has changed
	// One of the examples took 125ms to render, imagine that happening all the time while resizing the window
	if (_shouldRedraw)
	{
		Graphics bmpGraphics(_bitmap);
		bmpGraphics.Clear(COLOR_WHITE);
		for (auto it = _plotSet->GetPlots().cbegin(); it != _plotSet->GetPlots().cend(); it++)
		{
			(*it)->Draw(bmpGraphics);
		}
		_shouldRedraw = false;
	}

	/* OLD RENDERING CODE
	Rect fullArea(0, 0, _bitmap->GetWidth(), _bitmap->GetHeight());
	BitmapData lockedData;
	// Doing this should make rendering spirographs faster than using SetPixel
	_bitmap->LockBits(&fullArea, ImageLockModeWrite, PixelFormat32bppARGB, &lockedData);

	// Clear the bitmap to white
	char *pixels = reinterpret_cast<char*>(lockedData.Scan0);
	for (int x = 0; x < lockedData.Width; x++)
	{
		for (int y = 0; y < lockedData.Height; y++)
		{
			*reinterpret_cast<int*>(pixels + x * 4 + y * lockedData.Stride) = 0xFFFFFFFFF;
		}
	}

	// Draw all the spirographs (and any of plots if we define them)
	for (auto it = _plotSet->GetPlots().cbegin(); it != _plotSet->GetPlots().cend(); it++)
	{
		(*it)->Draw(lockedData);
	}

	_bitmap->UnlockBits(&lockedData);
	*/

	Graphics g(hdc);
	Rect dest;
	RECT clientSize; // Rect is GDI+, RECT is Win32
	GetClientRect(&clientSize);

	if (clientSize.right >= _plotSet->GetWidth() && clientSize.bottom >= _plotSet->GetHeight())
	{
		dest.Width = _plotSet->GetWidth();
		dest.Height = _plotSet->GetHeight();
		dest.X = (clientSize.right - dest.Width) / 2;
		dest.Y = (clientSize.bottom - dest.Height) / 2;
	}
	else
	{
		// Fill, should make it keep aspect ratio at some point
		dest.Width = clientSize.right;
		dest.Height = clientSize.bottom;
		dest.X = 0;
		dest.Y = 0;
	}

	Pen pen(COLOR_BLACK);
	g.DrawImage(_bitmap, dest);
	g.DrawRectangle(&pen, dest);
}

LRESULT Plotter::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(GetHWnd(), &ps);
		FillRect(hdc, &ps.rcPaint, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
		Paint(hdc);
		EndPaint(GetHWnd(), &ps);
		}
		break;
	case WM_SIZE:
		Invalidate();
		break;
	default:
		return DefWindowProc(this->GetHWnd(), msg, wParam, lParam);
	}
	return 0;
}
