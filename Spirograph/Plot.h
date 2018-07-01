#pragma once

// Bitmaps: https://msdn.microsoft.com/en-us/library/windows/desktop/ms534420(v=vs.85).aspx
// This stuff seems very similar to the Bitmap class in C#...

int RGBToInt(int r, int g, int b);
void IntToRGB(int rgb, int *r, int *g, int *b);

// Base class for all plots
class Plot
{
public:
	Plot();
	Plot(double x, double y, double outer, double inner, double pen, double thickness, int r, int g, int b);
	~Plot();

	double GetX();
	double GetY();
	int GetColor();
	double GetThickness();
	double GetOuterRadius();
	double GetInnerRadius();
	double GetPenRadius();
	int GetIterations();

	bool SetX(double x);
	bool SetY(double y);
	bool SetColor(int color);
	bool SetThickness(double thickness);
	bool SetOuterRadius(double r);
	bool SetInnerRadius(double r);
	bool SetPenRaidus(double r);
	bool SetIterations(int iterations);
	
	//OLD RENDERER void Draw(Gdiplus::BitmapData &bitmapData);
	void Draw(Gdiplus::Graphics &g);
private:
	double _x;
	double _y;
	int _color;
	double _thickness;
	double _outer_radius;
	double _inner_radius;
	double _pen_radius;
	int _iterations;
};
