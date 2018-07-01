#pragma once
#include <list>
#include "Plot.h"

class PlotSet
{
public:
	PlotSet();
	~PlotSet();
	
	int GetWidth();
	int GetHeight();

	bool SetWidth(int width);
	bool SetHeight(int height);
	
	void AddPlot(Plot *plot);
	void RemovePlot(Plot *plot);

	std::list<Plot*> &GetPlots();
private:
	int _pixelWidth;
	int _pixelHeight;
	std::list<Plot*> _plots;
};
