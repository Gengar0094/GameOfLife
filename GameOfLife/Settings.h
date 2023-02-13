#pragma once

struct Settings
{
	int s_speed;// = 50;
	int s_gridsize;// = 15;
	wxColor tru;// = *wxRED;
	wxColor fals;// = *wxWHITE;


	Settings()
	{
		s_speed = 50;
		s_gridsize = 15;
		tru = *wxRED;
		fals = *wxWHITE;
	}
};