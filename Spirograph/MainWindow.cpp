#include "stdafx.h"
#include "MainWindow.h"
#include "Resource.h"
#include "PlotSet.h"
#include "FileIO.h"
#include <commdlg.h>
#include <string>
#include <sstream>
#include <vector>

using namespace Gdiplus;
using namespace win32helper;
using namespace std;

static const LPCTSTR WINDOW_CLASS = L"MainWindow";

enum
{
	PLOTTER = 100,
	PLOTSET_WIDTH,
	PLOTSET_HEIGHT,
	PLOT_LIST,
	ADD_PLOT,
	DEL_PLOT,
	PLOT_X,
	PLOT_Y,
	PLOT_THICKNESS,
	PLOT_OUTER,
	PLOT_INNER,
	PLOT_PEN,
	PLOT_ITERS,
	PLOT_COLOR
};

wstring PlotToListText(Plot *plot)
{
	wstringstream stringBuilder;
	stringBuilder << L'(' << plot->GetX() << ',' << plot->GetY() << ')';
	return stringBuilder.str();
}

MainWindow::MainWindow(HINSTANCE hInstance): _filename(nullptr), _modified(false)
{
	Create(WS_EX_CLIENTEDGE, L"Spirograph", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, 0, hInstance, nullptr);
	_plotter = new Plotter(0, 0, 0, 0, this, PLOTTER, hInstance);
	
	int y = 10;
	
	_plotSetWidthLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Width");
	_plotSetWidth = new EditControl(60, y, 130, 20, this, PLOTSET_WIDTH, hInstance);
	y += 25;

	_plotSetHeightLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Height");
	_plotSetHeight = new EditControl(60, y, 130, 20, this, PLOTSET_HEIGHT, hInstance);
	y += 25;

	_plotList = new ListBoxControl(10, y, 180, 100, this, PLOT_LIST, hInstance);
	y += 105;

	_addPlot = new ButtonControl(10, y, 85, 20, this, ADD_PLOT, hInstance, L"Add");
	_delPlot = new ButtonControl(100, y, 85, 20, this, DEL_PLOT, hInstance, L"Delete");
	y += 25;

	_plotXLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"X");
	_plotX = new EditControl(60, y, 130, 20, this, PLOT_X, hInstance);
	y += 25;

	_plotYLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Y");
	_plotY = new EditControl(60, y, 130, 20, this, PLOT_Y, hInstance);
	y += 25;

	_plotThicknessLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Width");
	_plotThickness = new EditControl(60, y, 130, 20, this, PLOT_THICKNESS, hInstance);
	y += 25;

	_plotIterLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Iters");
	_plotIter = new EditControl(60, y, 130, 20, this, PLOT_ITERS, hInstance);
	y += 25;

	_radiusLabel = new StaticControl(10, y, 180, 20, this, 0, hInstance, L"Radii");
	_radiusLabel->SetAttribute(GWL_STYLE, _radiusLabel->GetAttribute(GWL_STYLE) | SS_CENTER);
	y += 25;

	_plotOuterLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Outer");
	_plotOuter = new EditControl(60, y, 130, 20, this, PLOT_OUTER, hInstance);
	y += 25;

	_plotInnerLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Inner");
	_plotInner = new EditControl(60, y, 130, 20, this, PLOT_INNER, hInstance);
	y += 25;

	_plotPenLabel = new StaticControl(10, y, 50, 20, this, 0, hInstance, L"Pen");
	_plotPen = new EditControl(60, y, 130, 20, this, PLOT_PEN, hInstance);
	y += 25;

	_plotColor = new ButtonControl(10, y, 180, 20, this, PLOT_COLOR, hInstance, L"Set Color");

	SetPlotSet(new PlotSet());

	// Load custom colors
	HKEY config;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Will Coates\\Spirograph", 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &config, nullptr) == ERROR_SUCCESS)
	{
		DWORD colorsLen = sizeof(customColors);
		RegGetValue(config, nullptr, L"colors", RRF_RT_REG_BINARY, nullptr, customColors, &colorsLen);
		RegCloseKey(config);
	}
}

MainWindow::~MainWindow()
{
	// Save custom colors
	HKEY config;
	int err = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Will Coates\\Spirograph", 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &config, nullptr);
	if (err == ERROR_SUCCESS)
	{
		RegSetValueEx(config, L"colors", 0, REG_BINARY, reinterpret_cast<BYTE*>(customColors), sizeof(customColors));
		RegCloseKey(config);
	}
	else
	{
		LPTSTR error = new TCHAR[1024];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, err, 0, error, 1024, nullptr);
		MessageBox(GetHWnd(), error, L"Failed to save config", 0);
		delete[] error;
	}

	// Delete controls
	delete _plotter;
	delete _currentSet;
	delete _plotSetWidthLabel;
	delete _plotSetWidth;
	delete _plotSetHeightLabel;
	delete _plotSetHeight;
	delete _plotList;
	delete _addPlot;
	delete _delPlot;
	delete _plotXLabel;
	delete _plotX;
	delete _plotYLabel;
	delete _plotY;
	delete _plotThicknessLabel;
	delete _plotThickness;
	delete _radiusLabel;
	delete _plotOuterLabel;
	delete _plotOuter;
	delete _plotInnerLabel;
	delete _plotInner;
	delete _plotPenLabel;
	delete _plotPen;
	delete _plotIterLabel;
	delete _plotIter;
	delete _plotColor;
}

void MainWindow::SetPlotSet(PlotSet* set)
{
	delete _currentSet;
	_currentSet = set;
	_plotter->SetPlotSet(set);
	_plotSetWidth->SetText(to_wstring(set->GetWidth()).c_str());
	_plotSetHeight->SetText(to_wstring(set->GetHeight()).c_str());
	_modified = false;
	
	_plotList->Clear();
	PlotListSelectionChanged();

	for (auto plot = set->GetPlots().cbegin(); plot != set->GetPlots().cend(); plot++)
	{
		int index = _plotList->Add(PlotToListText(*plot).c_str());
		_plotList->SetItemData(index, *plot);
	}
}

// I'm lazy and it's easier to fix bugs if you only have this defined once
template <typename cls>
void MainWindow::SetIntProp(EditControl *edit, cls *instance, Getter<cls, int> getter, Setter<cls, int> setter)
{
	LPTSTR text = edit->GetText();
	int value = wcstol(text, nullptr, 0);
	if (value == 0 && text[0] != L'0')
	{
		MessageBox(GetHWnd(), L"Value must be an integer", L"Spiroscope", MB_ICONERROR);
		edit->Focus();
	}
	else if (value != (instance->*getter)())
	{
		if ((instance->*setter)(value))
		{
			_plotter->Redraw();
			_modified = true;
		}
		else
		{
			MessageBox(GetHWnd(), L"Value out of range", L"Spiroscope", MB_ICONERROR);
			edit->Focus();
		}
	}
	delete[] text;
}

template <typename cls>
void MainWindow::SetDoubleProp(EditControl *edit, cls *instance, Getter<cls, double> getter, Setter<cls, double> setter)
{
	LPTSTR text = edit->GetText();
	double value = wcstod(text, nullptr);
	if (value == 0 && text[0] != L'0')
	{
		MessageBox(GetHWnd(), L"Value must be a number", L"Spiroscope", MB_ICONERROR);
		edit->Focus();
	}
	else if (value != (instance->*getter)())
	{
		if ((instance->*setter)(value))
		{
			_plotter->Redraw();
			_modified = true;
		}
		else
		{
			MessageBox(GetHWnd(), L"Value out of range", L"Spiroscope", MB_ICONERROR);
			edit->Focus();
		}
	}
	delete[] text;
}

bool MainWindow::PromptSave()
{
	if (_modified)
	{
		int result = MessageBox(GetHWnd(), L"Do you want to save changes?", L"Unsaved changes", MB_ICONINFORMATION | MB_YESNOCANCEL);
		if (result == IDYES)
		{
			return SaveFile();
		}
		else if (result == IDCANCEL)
		{
			return false;
		}
	}
	return true;
}

bool MainWindow::NewFile()
{
	if (PromptSave())
	{
		SetPlotSet(new PlotSet());
		delete[] _filename;
		_filename = nullptr;
		return true;
	}
	return false;
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646960(v=vs.85).aspx
bool MainWindow::OpenFile()
{
	if (PromptSave())
	{
		OPENFILENAME openFileName;
		LPTSTR filename = new TCHAR[FILENAME_MAX];

		ZeroMemory(filename, FILENAME_MAX * sizeof(TCHAR));
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));

		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = GetHWnd();
		openFileName.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0\0";
		openFileName.lpstrFile = filename;
		openFileName.lpstrDefExt = L"txt";
		openFileName.nMaxFile = FILENAME_MAX;
		openFileName.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

		if (GetOpenFileName(&openFileName) != 0)
		{
			wifstream input(filename);
			PlotSet *newSet = ReadPlotSet(input);
			if (newSet != nullptr)
			{
				SetPlotSet(newSet);
				delete[] _filename;
				int len = wcslen(filename) + 1;
				_filename = new TCHAR[len];
				wcscpy_s(_filename, len, filename);
				return true;
			}
			else
			{
				MessageBox(GetHWnd(), L"Error while opening file", L"Spirograph", MB_ICONERROR);
			}
		}
	}
	return false;
}

bool MainWindow::SaveFile()
{
	if (_filename == nullptr)
	{
		return SaveFileAs();
	}
	wofstream output(_filename);
	WritePlotSet(output, _currentSet);
	_modified = false;
	return true;
}

bool MainWindow::SaveFileAs()
{
	OPENFILENAME openFileName;
	LPTSTR filename = new TCHAR[FILENAME_MAX];

	ZeroMemory(filename, FILENAME_MAX * sizeof(TCHAR));
	ZeroMemory(&openFileName, sizeof(OPENFILENAME));

	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = GetHWnd();
	openFileName.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0\0";
	openFileName.lpstrFile = filename;
	openFileName.lpstrDefExt = L"txt";
	openFileName.nMaxFile = FILENAME_MAX;
	openFileName.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

	if (GetSaveFileName(&openFileName) != 0)
	{
		wofstream output(filename);
		WritePlotSet(output, _currentSet);
		delete[] _filename;
		int len = wcslen(filename) + 1;
		_filename = new TCHAR[len];
		wcscpy_s(_filename, len, filename);
		_modified = false;
		return true;
	}

	return false;
}

void MainWindow::ExportImage()
{
	/* NIce idea, but couldn't get multiple filetypes to work, so just doing bitmaps
	UINT numEncoders;
	UINT size;
	if (GetImageEncodersSize(&numEncoders, &size) != Ok)
	{
		MessageBox(GetHWnd(), L"Failed to retrieve image encoders", L"Failed to export image", MB_ICONERROR);
		return;
	}

	ImageCodecInfo *encoders = new ImageCodecInfo[numEncoders];
	if (GetImageEncoders(numEncoders, size, encoders) != Ok)
	{
		MessageBox(GetHWnd(), L"Failed to retrieve image encoders", L"Failed to export image", MB_ICONERROR);
		delete[] encoders;
		return;
	}

	wstringstream filterBuilder;
	for (int i = 0; i < numEncoders; i++)
	{
		filterBuilder << encoders[i].CodecName << L'\0';
		filterBuilder << encoders[i].FilenameExtension << L'\0';
	}
	filterBuilder << L'\0';
	wstring filterStr = filterBuilder.str();
	// Windows complains if we pass c_str() directly
	// We'll copy into a C-style string then pass
	LPTSTR filter = new TCHAR[filterStr.length() + 1];
	// We got nulls in this string...
	//wcscpy_s(filter, filterStr.length() + 1, filterStr.c_str());
	memcpy(filter, filterStr.c_str(), (filterStr.length() + 1) * sizeof(TCHAR));
	*/

	OPENFILENAME openFileName;
	LPTSTR filename = new TCHAR[FILENAME_MAX];

	ZeroMemory(filename, FILENAME_MAX * sizeof(TCHAR));
	ZeroMemory(&openFileName, sizeof(OPENFILENAME));

	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = GetHWnd();
	openFileName.lpstrFilter = L"Bitmap\0*.bmp\0\0";
	openFileName.lpstrFile = filename;
	openFileName.lpstrDefExt = L"BMP";
	openFileName.nMaxFile = FILENAME_MAX;
	openFileName.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

	if (GetSaveFileName(&openFileName) != 0)
	{
		// CLSID from https://stackoverflow.com/questions/5345803/does-gdi-have-standard-image-encoder-clsids
		// image/bmp  : {557cf400-1a04-11d3-9a73-0000f81ef32e}
		CLSID clsId{ 0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
		_plotter->Save(filename, &clsId);
	}
	//delete[] filter;
	//delete[] encoders;
}

void MainWindow::PlotListSelectionChanged() const
{
	int selected = _plotList->GetSelectedItem();
	bool enabled = selected != LB_ERR;
	int nCmdShow = enabled ? SW_SHOWNOACTIVATE : SW_HIDE;

	if (enabled)
	{
		Plot *plot = static_cast<Plot*>(_plotList->GetItemData(selected));
		_plotX->SetText(to_wstring(plot->GetX()).c_str());
		_plotY->SetText(to_wstring(plot->GetY()).c_str());
		_plotThickness->SetText(to_wstring(plot->GetThickness()).c_str());
		_plotOuter->SetText(to_wstring(plot->GetOuterRadius()).c_str());
		_plotInner->SetText(to_wstring(plot->GetInnerRadius()).c_str());
		_plotPen->SetText(to_wstring(plot->GetPenRadius()).c_str());
		_plotIter->SetText(to_wstring(plot->GetIterations()).c_str());
	}

	_delPlot->Enable(enabled);
	_plotXLabel->Show(nCmdShow);
	_plotX->Show(nCmdShow);
	_plotYLabel->Show(nCmdShow);
	_plotY->Show(nCmdShow);
	_plotThicknessLabel->Show(nCmdShow);
	_plotThickness->Show(nCmdShow);
	_radiusLabel->Show(nCmdShow);
	_plotOuterLabel->Show(nCmdShow);
	_plotOuter->Show(nCmdShow);
	_plotInnerLabel->Show(nCmdShow);
	_plotInner->Show(nCmdShow);
	_plotPenLabel->Show(nCmdShow);
	_plotPen->Show(nCmdShow);
	_plotIterLabel->Show(nCmdShow);
	_plotIter->Show(nCmdShow);
	_plotColor->Show(nCmdShow);
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
	wc.lpfnWndProc = WindowProcProxy;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW); // Idk why Microsoft uses COLOR_WINDOW + 1, makes no sense unless you want your window to be the same color as the borders
	wc.lpszClassName = WINDOW_CLASS;

	RegisterClassEx(&wc);
}

LPCTSTR MainWindow::GetWindowClass(HINSTANCE hInstance)
{
	RegisterWindowClass(hInstance);
	return WINDOW_CLASS;
}

LRESULT MainWindow::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int selectedPlotIndex = LB_ERR;
	// This can be called before we even create the list, make sure it isn't null
	if (_plotList != nullptr)
	{
		selectedPlotIndex = _plotList->GetSelectedItem();
	}

	Plot *selectedPlot = nullptr;
	if (selectedPlotIndex != LB_ERR)
	{
		selectedPlot = static_cast<Plot*>(_plotList->GetItemData(selectedPlotIndex));
	}

	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			// Menu bar
		case ID_FILE_NEW:
			NewFile();
			break;
		case ID_FILE_OPEN:
			OpenFile();
			break;
		case ID_FILE_SAVE:
			SaveFile();
			break;
		case ID_FILE_SAVEAS:
			SaveFileAs();
			break;
		case ID_FILE_EXPORT:
			ExportImage();
			break;
		case ID_FILE_EXIT:
			if (PromptSave())
			{
				Destroy();
			}
			break;

			// Plot size
		case PLOTSET_WIDTH:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetIntProp(_plotSetWidth, _currentSet, &PlotSet::GetWidth, &PlotSet::SetWidth);
			}
			break;
		case PLOTSET_HEIGHT:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetIntProp(_plotSetHeight, _currentSet, &PlotSet::GetHeight, &PlotSet::SetHeight);
			}
			break;

			// Plot list
		case PLOT_LIST:
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				PlotListSelectionChanged();
			}
			break;

			// Add and remove plot
		case ADD_PLOT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				Plot *plot = new Plot();

				int index = _plotList->Add(PlotToListText(plot).c_str());
				_plotList->SetItemData(index, plot);
				_plotList->Select(index); // So aparently this doesn't cause a selection changed message to be sent
				PlotListSelectionChanged();
				_currentSet->AddPlot(plot);
				_plotter->Redraw();
				_modified = true;
			}
			break;
		case DEL_PLOT:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				int selected = _plotList->GetSelectedItem();
				if (selected == LB_ERR)
				{
					// Hopefully we should never get here, but just in case, disguise it as an error
					MessageBox(GetHWnd(), L"You can't delete nothing", L"I'm sorry Dave", MB_ICONERROR);
				}
				else
				{
					Plot *plot = static_cast<Plot*>(_plotList->GetItemData(selected));
					_currentSet->RemovePlot(plot);
					_plotList->Delete(selected); // This doesn't send a message either
					PlotListSelectionChanged();
					_plotter->Redraw();
					_modified = true;
					delete plot;
				}
			}
			break;

			// Plot settings
		case PLOT_X:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetDoubleProp(_plotX, selectedPlot, &Plot::GetX, &Plot::SetX);
				_plotList->Invalidate();
			}
			break;
		case PLOT_Y:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetDoubleProp(_plotY, selectedPlot, &Plot::GetY, &Plot::SetY);
				_plotList->Invalidate();
			}
			break;
		case PLOT_THICKNESS:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetDoubleProp(_plotThickness, selectedPlot, &Plot::GetThickness, &Plot::SetThickness);
			}
			break;
		case PLOT_OUTER:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetDoubleProp(_plotOuter, selectedPlot, &Plot::GetOuterRadius, &Plot::SetOuterRadius);
			}
			break;
		case PLOT_INNER:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetDoubleProp(_plotInner, selectedPlot, &Plot::GetInnerRadius, &Plot::SetInnerRadius);
			}
			break;
		case PLOT_PEN:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetDoubleProp(_plotPen, selectedPlot, &Plot::GetPenRadius, &Plot::SetPenRaidus);
			}
			break;
		case PLOT_ITERS:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetIntProp(_plotIter, selectedPlot, &Plot::GetIterations, &Plot::SetIterations);
			}
			break;
		case PLOT_COLOR:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// Windows uses XBGR, we use ARGB, so we need to convert
				int r, g, b;
				IntToRGB(selectedPlot->GetColor(), &r, &g, &b);
				// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646375(v=vs.85).aspx
				CHOOSECOLOR chooseColor;
				ZeroMemory(&chooseColor, sizeof(CHOOSECOLOR));
				chooseColor.lStructSize = sizeof(CHOOSECOLOR);
				chooseColor.hwndOwner = GetHWnd();
				chooseColor.rgbResult = RGB(r, g, b);
				chooseColor.lpCustColors = customColors;
				chooseColor.Flags = CC_RGBINIT;
				if (ChooseColor(&chooseColor))
				{
					r = GetRValue(chooseColor.rgbResult);
					g = GetGValue(chooseColor.rgbResult);
					b = GetBValue(chooseColor.rgbResult);
					selectedPlot->SetColor(RGBToInt(r, g, b));
					_plotter->Redraw();
					_plotList->Invalidate();
					_modified = true;
				}
			}
			break;
		default:
			return DefWindowProc(this->GetHWnd(), msg, wParam, lParam);
		}
		break;

	case WM_CLOSE:
		if (PromptSave())
		{
			Destroy();
		}
		break;

	// case WM_MEASUREITEM: Seems out we don't need to implement this for just text
		
	case WM_DRAWITEM:
		// There's no SETTEXT message for list boxes, only add and delete
		// This allows us to dynamically display text based off what has been set
		// Plus this allows us to color items in the list box
		if (wParam == PLOT_LIST)
		{
			LPDRAWITEMSTRUCT dis = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

			if (dis->itemID == -1) break;

			if (dis->itemAction == ODA_SELECT || dis->itemAction == ODA_DRAWENTIRE)
			{
				// We use ARGB, Windows uses XBGR
				int r, g, b;
				Plot * plot = static_cast<Plot*>(_plotList->GetItemData(dis->itemID));
				wstring text = PlotToListText(plot);
				IntToRGB(plot->GetColor(), &r, &g, &b);

				FillRect(dis->hDC, &dis->rcItem, (HBRUSH)(COLOR_WINDOW + 1));

				SetTextColor(dis->hDC, RGB(r, g, b));
				TextOut(dis->hDC, dis->rcItem.left, dis->rcItem.top, text.c_str(), text.length());
				SetTextColor(dis->hDC, RGB(0, 0, 0));
				if (dis->itemState & ODS_SELECTED)
				{
					DrawFocusRect(dis->hDC, &dis->rcItem);
				}
			}
			break;
		}
		else
		{
			return DefWindowProc(this->GetHWnd(), msg, wParam, lParam);
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		{
		int newWidth = LOWORD(lParam);
		int newHeight = HIWORD(lParam);
		_plotter->SetPosition(200, 0, newWidth - 200, newHeight);
		}
		break;

	default:
		return DefWindowProc(this->GetHWnd(), msg, wParam, lParam);
	}

	return 0;
}
