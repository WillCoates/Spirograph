#include "stdafx.h"
#include "Plot.h"
#include <cmath>

using namespace Gdiplus;
/*
SpirographPlot::SpirographPlot(): _outer_radius(100), _inner_radius(25), _pen_radius(25)
{	
}

SpirographPlot::SpirographPlot(double x, double y, double outer, double inner, double pen, double thickness, int r, int g, int b):
Plot(x, y, thickness, r, g, b), _outer_radius(outer), _inner_radius(inner), _pen_radius(pen)
{	
}


SpirographPlot::~SpirographPlot()
{
	
}

void SpirographPlot::Draw(Gdiplus::BitmapData &bitmapData)
{
	// Maths from https://linuxgazette.net/133/misc/luana/spirograph.pdf
	//for (double t = 0; t < 400; t += 0.02)
	for (int tint = 0; tint < 20000; tint++)
	{
		double t = tint * 0.02;
		double x = (_outer_radius - _inner_radius) * cos(t) + _pen_radius * cos((_outer_radius - _inner_radius) * t / _inner_radius);
		double y = (_outer_radius - _inner_radius) * sin(t) - _pen_radius * sin((_outer_radius - _inner_radius) * t / _inner_radius);
		double thickness = GetThickness();
		char *pixels = static_cast<char*>(bitmapData.Scan0);
		// TODO: Change for loops from doubles to ints
		for (double i = -0.5 * thickness; i < 0.5 * thickness; i += 0.5)
		{
			for (double j = -0.5 * thickness; j < 0.5 * thickness; j += 0.5)
			{
				int bmp_x = static_cast<int>(round(x + GetX() + i));
				int bmp_y = static_cast<int>(round(y + GetY() + j));
				if (bmp_x >= 0 && bmp_y >= 0 && bmp_x < bitmapData.Width && bmp_y < bitmapData.Height)
				{
					// There's probably a better way of doing this
					*reinterpret_cast<int*>(&pixels[bmp_x * 4 + bmp_y * bitmapData.Stride]) = GetColor();
				}
			}
		}
	}
}

void SpirographPlot::Draw(Gdiplus::Graphics &g)
{
	// Maths from https://linuxgazette.net/133/misc/luana/spirograph.pdf

	double prev_x;
	double prev_y;

	Color color = Color(GetColor());
	Pen pen(color, GetThickness());

	//for (double t = 0; t < 400; t += 0.02)
	for (int tint = 0; tint < 10000; tint++)
	{
		double t = tint * 0.005;
		double x = (_outer_radius - _inner_radius) * cos(t) + _pen_radius * cos((_outer_radius - _inner_radius) * t / _inner_radius);
		double y = (_outer_radius - _inner_radius) * sin(t) - _pen_radius * sin((_outer_radius - _inner_radius) * t / _inner_radius);
		
		double real_x = x + GetX();
		double real_y = y + GetY();
		
		if (tint != 0)
		{
			g.DrawLine(&pen, static_cast<REAL>(prev_x), static_cast<REAL>(prev_y), static_cast<REAL>(real_x), static_cast<REAL>(real_y));
		}

		prev_x = real_x;
		prev_y = real_y;
	}
}
*/