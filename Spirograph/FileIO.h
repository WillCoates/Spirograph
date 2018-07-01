#pragma once
#include <fstream>
#include "PlotSet.h"
#include "Plot.h"

// Spirograph file format
// R WIDTH HEIGHT
// S OUTER INNER PEN X Y R G B THICKNESS
// # COMMENT

PlotSet *ReadPlotSet(std::wifstream &file);
void WritePlotSet(std::wofstream &file, PlotSet *plotset);