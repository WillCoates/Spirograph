#include "stdafx.h"
#include "Plot.h"
#include <cmath>

using namespace Gdiplus;

const double TOTAL_RADS = 80;
const int DEFAULT_ITERATIONS = 20000;

int RGBToInt(int r, int g, int b)
{
	return 0xFF << 24 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | b & 0xFF;
}

void IntToRGB(int rgb, int *r, int *g, int *b)
{
	*r = (rgb >> 16) & 0xFF;
	*g = (rgb >> 8) & 0xFF;
	*b = (rgb) & 0xFF;
}

Plot::Plot(): _x(0), _y(0), _color(0xFF000000), _thickness(1), _outer_radius(100), _inner_radius(2), _pen_radius(80), _iterations(DEFAULT_ITERATIONS)
{
	
}

Plot::Plot(double x, double y, double outer, double inner, double pen, double thickness, int r, int g, int b) :
	_x(x), _y(y), _thickness(thickness), _color(RGBToInt(r, g, b)), _outer_radius(outer), _inner_radius(inner), _pen_radius(pen), _iterations(DEFAULT_ITERATIONS)
{
}

Plot::~Plot()
{	
}

double Plot::GetX()
{
	return _x;
}

double Plot::GetY()
{
	return _y;
}

int Plot::GetColor()
{
	return _color;
}

double Plot::GetThickness()
{
	return _thickness;
}

double Plot::GetInnerRadius()
{
	return _inner_radius;
}

double Plot::GetOuterRadius()
{
	return _outer_radius;
}

double Plot::GetPenRadius()
{
	return _pen_radius;
}

int Plot::GetIterations()
{
	return _iterations;
}

bool Plot::SetX(double x)
{
	_x = x;
	return true;
}

bool Plot::SetY(double y)
{
	_y = y;
	return true;
}

bool Plot::SetColor(int color)
{
	_color = color | 0xFF000000; // Ensure alpha is 255
	return true;
}

bool Plot::SetThickness(double thickness)
{
	if (thickness <= 0) return false;
	_thickness = thickness;
	return true;
}

bool Plot::SetInnerRadius(double r)
{
	_inner_radius = r;
	return true;
}

bool Plot::SetOuterRadius(double r)
{
	_outer_radius = r;
	return true;
}

bool Plot::SetPenRaidus(double r)
{
	_pen_radius = r;
	return true;
}

bool Plot::SetIterations(int iterations)
{
	if (iterations < 2) return false;
	_iterations = iterations;
	return true;
}


/* OLD RENDERER - This might have been faster, but couldn't handle high resolutions
void Plot::Draw(BitmapData &bitmapData)
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
*/

void Plot::Draw(Gdiplus::Graphics &g)
{
	double prev_x;
	double prev_y;

	Color color = Color(GetColor());
	Pen pen(color, static_cast<REAL>(GetThickness()));

	double step = TOTAL_RADS / _iterations;

	//for (double t = 0; t < 400; t += 0.02)
	for (int i = 0; i < _iterations; i++)
	{
		double t = i * step;
		// Maths from https://linuxgazette.net/133/misc/luana/spirograph.pdf
		double x = (_outer_radius - _inner_radius) * cos(t) + _pen_radius * cos((_outer_radius - _inner_radius) * t / _inner_radius);
		double y = (_outer_radius - _inner_radius) * sin(t) - _pen_radius * sin((_outer_radius - _inner_radius) * t / _inner_radius);
		// End https://linuxgazette.net/133/misc/luana/spirograph.pdf

		x += GetX();
		y += GetY();

		// We need two points to draw a line, on first iteration we'll only have one point
		if (i != 0)
		{
			// ReSharper disable CppLocalVariableMightNotBeInitialized
			g.DrawLine(&pen, static_cast<REAL>(prev_x), static_cast<REAL>(prev_y), static_cast<REAL>(x), static_cast<REAL>(y));
			// ReSharper restore CppLocalVariableMightNotBeInitialized
		}

		prev_x = x;
		prev_y = y;
	}
}
