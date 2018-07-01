#pragma once
#include "Control.h"
#include "PlotSet.h"

class Plotter : public win32helper::Control
{
public:
	Plotter(int x, int y, int width, int height, Control *parent, int id, HINSTANCE hInstance);
	virtual ~Plotter();

	void Save(LPCTSTR filename, const LPCLSID clsId);
	void Redraw();
	void SetPlotSet(PlotSet *plotSet);
	PlotSet *GetPlotSet() const;
protected:
	virtual LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LPCTSTR GetWindowClass(HINSTANCE hInstance);
private:
	void Paint(HDC hdc);
	bool _shouldRedraw;
	PlotSet *_plotSet;
	Gdiplus::Bitmap *_bitmap;
};
