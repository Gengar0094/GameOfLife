#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include <vector>

class MainWindow :
    public wxFrame
{
private:

	//int settings->s_gridsize = 15;
	wxBoxSizer* boxSize;
	DrawingPanel* draw;
	std::vector<std::vector<bool>> matrix;
	std::vector<std::vector<bool>> sandbox;
	int generations = 0;
	int liveCells = 0;
	int timecount = 50;
	wxStatusBar* sts;
	wxMenuItem* _finite = nullptr; //null?
	wxMenuItem* _toroidal = nullptr; //null?
	wxTimer* _timer;
	int speed = 50;

public:

	Settings* settings;
	void GridInit();
	MainWindow();
	~MainWindow();
	int NeighborCount(int row, int col);
	void NextGeneration();
	void OnResize(wxSizeEvent& resizeEvent);
	void OnSaveClick(wxCommandEvent& event);
	void OnLoadClick(wxCommandEvent& event);
	void OnPlayClick(wxCommandEvent& event);
	void OnNextClick(wxCommandEvent& event);
	void OnPauseClick(wxCommandEvent& event);
	void OnTrashClick(wxCommandEvent& event);
	void Timer(wxTimerEvent& event);


	wxDECLARE_EVENT_TABLE();



};

