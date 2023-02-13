#pragma once
#include "wx/wx.h"
#include <vector>
#include "Settings.h"

class DrawingPanel :
	public wxPanel
{
private:
	wxFrame* _parent = nullptr;
	std::vector<std::vector<bool>>* Board;
	

public:

	Settings* settings;
	void SetSizer(wxSize& screen);
	DrawingPanel(wxFrame* ptr, Settings* options);
	~DrawingPanel();
	void OnPaint(wxPaintEvent&);
	void SetGridSize(int grid);
	void SetBoard(std::vector<std::vector<bool>>* board);
	void OnClick(wxMouseEvent& click);
	

	wxDECLARE_EVENT_TABLE();

};

