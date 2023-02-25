#include "MainWindow.h"
#include "SettingsUI.h"
#include "Settings.h"
#include "play.xpm"
#include "set.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "random.xpm"
#include "export.xpm"
#include "import.xpm"
#include "fstream"
#include "wx/filedlg.h"
#include <string>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnResize)
EVT_MENU(10003, MainWindow::OnNextClick)
EVT_MENU(10001, MainWindow::OnPlayClick)
EVT_MENU(10002, MainWindow::OnPauseClick)
EVT_MENU(10004, MainWindow::OnTrashClick)
EVT_MENU(10006, MainWindow::OnSetClick)
EVT_MENU(10007, MainWindow::OnRandomClick)
EVT_MENU(wxID_SAVE, MainWindow::OnSaveClick)
EVT_MENU(wxID_OPEN, MainWindow::OnLoadClick)
//EVT_MENU(10011, MainWindow::OnSaveClick) // "Export"
//EVT_MENU(10012, MainWindow::OnLoadClick) //"Import"
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
	wxBitmap set(set_xpm);
	wxBitmap random(random_xpm);
	//wxBitmap save(export_xpm);
	//wxBitmap load(import_xpm);

	wxToolBar* toolBar = CreateToolBar();
	toolBar->AddTool(10001, "Start", play, "Start"); //These will need to be changed from ID ANY
	toolBar->AddTool(10002, "Pause", pause, "Pause");
	toolBar->AddTool(10003, "Next", next, "Next");
	toolBar->AddTool(10007, "Random", random, "Random");
	toolBar->AddTool(10004, "Trash", trash, "Trash");
	toolBar->AddTool(10006, "Settings", set, "Settings");
	//toolBar->AddTool(10011, "Save", save, "Save");
	//toolBar->AddTool(10012, "Load", load, "Load");

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

	wxMenuBar* menu = new wxMenuBar();
	SetMenuBar(menu);
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_OPEN);
	fileMenu->Append(wxID_SAVE);
	menu->Append(fileMenu, "Save/Load");

	GridInit();
	this->Layout();
}

void MainWindow::OnResize(wxSizeEvent& resizeEvent) {
	wxSize size = GetClientSize();
	draw->SetSizer(size);
	resizeEvent.Skip();
}

void MainWindow::OnRandomClick(wxCommandEvent& event) {

	RandomizeMatrix();
	event.Skip();
}

void MainWindow::OnSaveClick(wxCommandEvent& event) {
	wxFileDialog fileDialog(this, "Save Current Universe", "", "", "Game of Life File (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	std::ofstream output;
	output.open((std::string)fileDialog.GetPath());
	if (output.is_open())
	{
		for (int i = 0; i < matrix.size(); i++)
		{
			for (int j = 0; j < matrix[i].size(); j++)
			{
				if (matrix[i][j]) {
					output << '*';
				}
				else {
					output << '.';
				}
			}
			output << '\n';
		}

		output.close();
	}

	event.Skip();
}

void MainWindow::OnLoadClick(wxCommandEvent& event) {
	wxFileDialog fileDialog(this, "Load Stored Universe", "", "", "Game of Life File (*.cells) | *.cells", wxFD_OPEN);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	for (int i = 0; i < matrix.size(); i++)
	{
		matrix[i].clear();
		matrix.resize(0);
	}
	matrix.clear();
	matrix.resize(0);

	std::string buff;

	std::ifstream input;
	int ndx = 0;
	input.open((std::string)fileDialog.GetPath());
	if (input.is_open()) {
		while (!input.eof())
		{
			std::getline(input, buff);
			if (buff.size() == 0)
			{
				break;
			}
			if (matrix.size() == 0)
			{
				matrix.resize(buff.size());
			}
			matrix[ndx].resize(buff.size());
			for (int i = 0; i < buff.size(); i++)
			{
				if (buff[i] == '*')
				{
					matrix[ndx][i] = true;
				}
				else
				{
					matrix[ndx][i] = false;
				}
			}
			ndx++;
		}

		input.close();
	}

	event.Skip();
}


void MainWindow::OnPlayClick(wxCommandEvent& evnet)
{
	_timer->Start(settings->s_speed);

}

void MainWindow::OnSetClick(wxCommandEvent& event) {
	SettingsUI* dialog = new SettingsUI(this, settings);
	dialog->ShowModal();

	event.Skip();
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

void MainWindow::RandomizeMatrix() {
	srand(time(NULL));

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			int num = rand();
			if (num % 2 == 0)
			{
				matrix[i][j] = true;
			}
			else
			{
				matrix[i][j] = false;
			}

		}

	}

}

MainWindow::~MainWindow() {
	delete settings;
	delete _timer;
	//delete boxSize; Causes error on close?
	delete draw;
	delete _finite;
	delete _toroidal;
	delete sts;

}

