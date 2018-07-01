#include "stdafx.h"
#include "PlotSet.h"

PlotSet::PlotSet(): _pixelWidth(400), _pixelHeight(400)
{
	
}

PlotSet::~PlotSet()
{
	for (auto it = _plots.begin(); it != _plots.end(); it++)
	{
		delete *it;
	}
}

int PlotSet::GetHeight()
{
	return _pixelHeight;
}

int PlotSet::GetWidth()
{
	return _pixelWidth;
}

bool PlotSet::SetHeight(int height)
{
	if (height <= 0) return false;
	_pixelHeight = height;
	return true;
}

bool PlotSet::SetWidth(int width)
{
	if (width <= 0) return false;
	_pixelWidth = width;
	return true;
}

void PlotSet::AddPlot(Plot* plot)
{
	_plots.push_back(plot);
}

void PlotSet::RemovePlot(Plot* plot)
{
	_plots.remove(plot);
}

std::list<Plot*> &PlotSet::GetPlots()
{
	return _plots;
}