#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings
{
	int s_speed = 50; //Tic speed
	int s_gridsize = 20; //Size of grid

	unsigned int livingAlpha = 255; //These represent the color of a living cell
	unsigned int livingRed = 128;
	unsigned int livingGreen = 128;
	unsigned int livingBlue = 128;

	unsigned int deadRed = 255; //These represent the color of a dead cell
	unsigned int deadGreen = 255;
	unsigned int deadBlue = 255;
	unsigned int deadAlpha = 255;

	void SetLiveColor(wxColor live) {
		livingRed = live.GetRed();
		livingGreen = live.GetGreen();
		livingBlue = live.GetBlue();
	}

	wxColor GetLiveColor() {

		wxColor color(livingRed, livingGreen, livingBlue, livingAlpha);
		return color;
	}

	void SetDeadColor(wxColor dead) {

		deadRed = dead.GetRed();
		deadGreen = dead.GetGreen();
		deadBlue = dead.GetBlue();

	}

	wxColor GetDeadColor() {
		wxColor color(deadRed, deadGreen, deadBlue, deadAlpha);
		return color;
	}

};