#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)

EVT_PAINT(DrawingPanel::OnPaint)
//EVT_LEFT_UP(DrawingPanel::OnClick)

wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxFrame* ptr, Settings* options) : wxPanel(ptr, wxID_ANY, wxPoint(10, 10), wxSize(20, 20))
{
	settings = options;
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
	this->Bind(wxEVT_LEFT_UP, &DrawingPanel::OnClick, this);
}

void DrawingPanel::SetSizer(wxSize& screen) {
	SetSize(screen);
	Refresh();
}



#pragma region OnPaint Method
void DrawingPanel::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* gra = wxGraphicsContext::Create(dc);

	if (!gra)
	{
		return;
	}

	wxSize window = GetClientSize();

	gra->SetPen(*wxBLACK);
	gra->SetBrush(*wxWHITE);

	//grid location
	int x1 = 0;
	int y1 = 0;

	float width = window.GetWidth() / (float)settings->s_gridsize;
	float height = window.GetHeight() / (float)settings->s_gridsize;


	for (int i = 0; i < settings->s_gridsize; i++)
	{
		for (int j = 0; j < settings->s_gridsize; j++)
		{

			if ((*Board)[i][j])
			{
				gra->SetBrush(*wxLIGHT_GREY);
			}
			else
			{
				gra->SetBrush(*wxWHITE);
			}

			gra->DrawRectangle(x1 + width * i, y1 + height * j, width, height);
		}
	}

}
#pragma endregion

void DrawingPanel::SetBoard(std::vector<std::vector<bool>>* board) {
	Board = board;
}

void DrawingPanel::OnClick(wxMouseEvent& click) {
	int xPOS = click.GetX();
	int yPOS = click.GetY();
	wxSize window = GetClientSize();
	float width = window.GetWidth() / (float)settings->s_gridsize;
	float height = window.GetHeight() / (float)settings->s_gridsize;

	int xClick = xPOS / width;
	int yClick = yPOS / height; // This is added to stop the offset problem


	(*Board)[xClick][yClick] = !(*Board)[xClick][yClick];


	Refresh();
	click.Skip();
}

void DrawingPanel::SetGridSize(int grid) {
	settings->s_gridsize = grid;
}

DrawingPanel::~DrawingPanel() {


}
