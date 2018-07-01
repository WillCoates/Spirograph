#include "stdafx.h"
#include "FileIO.h"

using namespace std;

void ReadResolution(wifstream &file, PlotSet *plotSet);
void ReadSpirograph(wifstream &file, PlotSet *plotSet);

PlotSet *ReadPlotSet(wifstream &file)
{
	// Allows me to be lazy and not requiring to convert older files
	int version = 0;

	PlotSet *plotSet = new PlotSet();
	while (!file.eof())
	{
		wchar_t cmd = L'\0';
		file >> cmd;
		switch (cmd)
		{
		case L'v':
		case L'V':
			file >> version;
			break;
		case L'r':
		case L'R':
		{
			int width, height;
			file >> width >> height;
			plotSet->SetWidth(width);
			plotSet->SetHeight(height);
		}
			break;
		case L's':
		case L'S':
		{
			double outer, inner, pen, x, y, thickness;
			int r, g, b;
			file >> outer >> inner >> pen >> x >> y >> r >> g >> b >> thickness;
			Plot *plot = new Plot(x, y, outer, inner, pen, thickness, r, g, b);
			plotSet->AddPlot(plot);
			if (version > 0)
			{
				int iterations;
				file >> iterations;
				plot->SetIterations(iterations);
			}
		}
			break;
		case L'#':
			file.ignore(INT_MAX, L'\n');
			break;
		case L'\0':
			return plotSet;
		default:
			delete plotSet;
			return nullptr;
		}

		if (file.bad())
		{
			delete plotSet;
			return nullptr;
		}
	}
	return plotSet;
}

void WritePlotSet(wofstream &file, PlotSet *plotset)
{
	file << "V 1" << endl;
	file << "R " << plotset->GetWidth() << " " << plotset->GetHeight() << endl;
	for (auto plot = plotset->GetPlots().cbegin(); plot != plotset->GetPlots().cend(); plot++)
	{
		int r, g, b;
		IntToRGB((*plot)->GetColor(), &r, &g, &b);
		file << "S "
			<< (*plot)->GetOuterRadius() << " "
			<< (*plot)->GetInnerRadius() << " "
			<< (*plot)->GetPenRadius() << " "
			<< (*plot)->GetX() << " "
			<< (*plot)->GetY() << " "
			<< r << " "
			<< g << " "
			<< b << " "
			<< (*plot)->GetThickness() << " "
			<< (*plot)->GetIterations() << endl;
	}
	file.flush();
}