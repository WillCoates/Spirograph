#pragma once
#include "Control.h"
#include "CommonControls.h"
#include "Plotter.h"

template <typename cls, typename type>
using Getter = type (cls::*)();

template <typename cls, typename type>
using Setter = bool (cls::*)(type);

class MainWindow: public win32helper::Control
{
public:
	MainWindow(HINSTANCE hInstance);
	virtual ~MainWindow();

	void SetPlotSet(PlotSet *set);
protected:
	virtual LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LPCTSTR GetWindowClass(HINSTANCE hInstance);
private:
	template <typename cls>
	void SetIntProp(win32helper::EditControl *edit, cls *instance, Getter<cls, int> getter, Setter<cls, int> setter);

	template <typename cls>
	void SetDoubleProp(win32helper::EditControl *edit, cls *instance, Getter<cls, double> getter, Setter<cls, double> setter);

	bool PromptSave();
	bool NewFile();
	bool OpenFile();
	bool SaveFile();
	bool SaveFileAs();
	void ExportImage();

	void PlotListSelectionChanged() const;

	LPTSTR _filename;
	bool _modified;
	PlotSet *_currentSet;
	COLORREF customColors[16];

	Plotter *_plotter;
	win32helper::StaticControl *_plotSetWidthLabel;
	win32helper::EditControl *_plotSetWidth;
	win32helper::StaticControl *_plotSetHeightLabel;
	win32helper::EditControl *_plotSetHeight;

	win32helper::ListBoxControl *_plotList;
	win32helper::ButtonControl *_addPlot;
	win32helper::ButtonControl *_delPlot;
	
	// I miss having a UI editor
	win32helper::StaticControl *_plotXLabel;
	win32helper::EditControl *_plotX;
	win32helper::StaticControl *_plotYLabel;
	win32helper::EditControl *_plotY;
	win32helper::StaticControl *_plotThicknessLabel;
	win32helper::EditControl *_plotThickness;
	win32helper::StaticControl *_radiusLabel;
	win32helper::StaticControl *_plotOuterLabel;
	win32helper::EditControl *_plotOuter;
	win32helper::StaticControl *_plotInnerLabel;
	win32helper::EditControl *_plotInner;
	win32helper::StaticControl *_plotPenLabel;
	win32helper::EditControl *_plotPen;
	win32helper::StaticControl *_plotIterLabel;
	win32helper::EditControl *_plotIter;

	win32helper::ButtonControl *_plotColor;
};
