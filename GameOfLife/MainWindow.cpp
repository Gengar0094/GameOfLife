#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "fstream"
#include <string>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnResize)
EVT_MENU(10003, MainWindow::OnNextClick)
EVT_MENU(10001, MainWindow::OnPlayClick)
EVT_MENU(10002, MainWindow::OnPauseClick)
EVT_MENU(10004, MainWindow::OnTrashClick)
EVT_TIMER(10005, MainWindow::Timer)





wxEND_EVENT_TABLE()




MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(15, 15), wxSize(500, 500))
{
	settings = new Settings();
	draw = new DrawingPanel(this, settings);



	sts = CreateStatusBar();
	sts->SetStatusText("Generation(s): " + std::to_string(generations));

	wxBitmap play(play_xpm);
	wxBitmap pause(pause_xpm);
	wxBitmap next(next_xpm);
	wxBitmap trash(trash_xpm);

	wxToolBar* toolBar = CreateToolBar();
	toolBar->AddTool(10001, "Start", play, "Start"); //These will need to be changed from ID ANY
	toolBar->AddTool(10002, "Pause", pause, "Pause");
	toolBar->AddTool(10003, "Next", next, "Next");
	toolBar->AddTool(10004, "Trash", trash, "Trash");

	toolBar->Realize();

	_timer = new wxTimer(this, 10005);
	boxSize = new wxBoxSizer(wxVERTICAL);
	boxSize->Add(draw, 1, wxEXPAND | wxALL);
	this->SetSizer(boxSize);
	this->Bind(wxEVT_SIZE, &MainWindow::OnResize, this);


	wxMenu* viewMenu = new wxMenu();
	_finite = new wxMenuItem(viewMenu, 10009, "finite", "", wxITEM_CHECK);
	_finite->SetCheckable(true);
	viewMenu->Append(_finite);

	_toroidal = new wxMenuItem(viewMenu, 10010, "toroidal", "", wxITEM_CHECK);
	_toroidal->SetCheckable(true);
	viewMenu->Append(_toroidal);

	GridInit();
	this->Layout();
}

void MainWindow::OnResize(wxSizeEvent& resizeEvent) {
	wxSize size = GetClientSize();
	draw->SetSizer(size);
	resizeEvent.Skip();
}


//void MainWindow::OnSaveClick(wxCommandEvent& event) {
//	wxFileDialog file(this, "Save Board", wxEmptyString, wxEmptyString, "Game of Life Board file", )
//		if (file.ShowModal() == wxID_CANCEL)
//		{
//			return;
//		}
//		std::ofstream fileStream;
//		fileStream.open((std::string)wxFileDialog.GetPath());
//		if (fileStream.is_open())
//		{
//			for (int i = 0; i < matrix.size(); i++)
//			{
//				for (int j = 0; j < matrix.size(); j++)
//				{
//					if (matrix[i][j])
//					{
//						fileStream << '*';
//					}
//					else
//					{
//						fileStream << '.';
//					}
//				}
//				fileStream << '\n';
//			}
//
//			fileStream.close();
//		}
//
//	event.Skip();
//}

//void MainWindow::OnLoadClick(wxCommandEvent& event) {
//
//
//
//	std::ifstream fileStream;
//	if (fileStream.is_open())
//	{
//		while (!fileStream.eof())
//		{
//
//		}
//		fileStream.close();
//	}
//
//	event.Skip();
//}


void MainWindow::OnPlayClick(wxCommandEvent& evnet)
{
	_timer->Start(speed);

}

void MainWindow::OnTrashClick(wxCommandEvent& event) {
	matrix.clear();
	GridInit();
	draw->Refresh();
	generations = 0;
	event.Skip();

}

void MainWindow::OnPauseClick(wxCommandEvent& event) {

	_timer->Stop();
}

void MainWindow::OnNextClick(wxCommandEvent& event) {
	NextGeneration();
	event.Skip();
}



void MainWindow::GridInit() {
	matrix.resize(settings->s_gridsize);
	for (int i = 0; i < matrix.size(); i++)
	{
		matrix[i].resize(settings->s_gridsize);

	}

	draw->SetBoard(&matrix);
}

int MainWindow::NeighborCount(int row, int col) {

	int count = 0;
	//row = settings->s_gridsize;
	//col = settings->s_gridsize;

	for (int rowOff = -1; rowOff <= 1; rowOff++)
	{

		for (int colOff = -1; colOff <= 1; colOff++)
		{
			int xCheck = row + rowOff;
			int yCheck = col + colOff;


			if (rowOff == 0 && colOff == 0)
			{
				continue;
			}

			if (xCheck < 0)
			{
				continue;
			}

			if (yCheck < 0)
			{
				continue;
			}

			if (xCheck >= settings->s_gridsize)
			{
				continue;
			}

			if (yCheck >= settings->s_gridsize)
			{
				continue;
			}

			if (matrix[xCheck][yCheck])
			{
				count++;
			}
		}
	}
	return count;
}

void MainWindow::NextGeneration() {


	sandbox.resize(settings->s_gridsize);
	for (int x = 0; x < sandbox.size(); x++)
	{
		sandbox[x].resize(settings->s_gridsize);
		for (int y = 0; y < sandbox.size(); y++)
		{
			int neighbor = NeighborCount(x, y);

			if (neighbor < 2 && matrix[x][y] == true)
			{
				sandbox[x][y] = false;
			}

			if (neighbor > 3 && matrix[x][y] == true)
			{
				sandbox[x][y] = false;
			}
			if ((neighbor == 2 || neighbor == 3) && matrix[x][y] == true)
			{
				sandbox[x][y] = true;
			}
			if (neighbor == 3 && matrix[x][y] == false)
			{
				sandbox[x][y] = true;
			}
		}
	}

	matrix.swap(sandbox);
	sandbox.clear();
	generations++;
	sts->SetStatusText("Generation(s): " + std::to_string(generations));
	draw->Refresh();
}

void MainWindow::Timer(wxTimerEvent& event) {
	NextGeneration();
	event.Skip();
}

MainWindow::~MainWindow() {
	delete settings;
	delete _timer;
	//delete boxSize;
	delete draw;
	delete _finite;
	delete _toroidal;
}

