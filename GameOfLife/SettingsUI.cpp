#include "SettingsUI.h"
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"

wxBEGIN_EVENT_TABLE(SettingsUI, wxDialog)
EVT_SPINCTRL(10060, SettingsUI::SetGridSize)
EVT_SPINCTRL(10063, SettingsUI::SetSpeed)
wxEND_EVENT_TABLE()


SettingsUI::SettingsUI(wxWindow* parent, Settings* _settings) 
	: wxDialog(parent, wxID_ANY, "Settings", wxPoint(100,100), wxSize(275, 310)) {

	settings = _settings;
	
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	wxBoxSizer* numberSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* numberSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* colorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* colorSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* confirmPress = CreateButtonSizer(wxOK | wxCANCEL);

	_spinner = new wxSpinCtrl(this, 10060);
	_spinner2 = new wxSpinCtrl(this, 10063);
	_colorPicker = new wxColourPickerCtrl(this, 10061);
	_colorPicker2 = new wxColourPickerCtrl(this, 10062);

	wxStaticText* spinnerText = new wxStaticText(this, 9008, "Grid Size");
	spinnerText->SetMinSize(wxSize(60, 60));
	wxStaticText* spinnerText2 = new wxStaticText(this, 9008, "Game Speed");
	spinnerText2->SetMinSize(wxSize(60, 60));
	wxStaticText* liveText = new wxStaticText(this, 9009, "Live Cell Color");
	liveText->SetMinSize(wxSize(60, 60));
	wxStaticText* deadText = new wxStaticText(this, 9010, "Dead Cell Size");
	deadText->SetMinSize(wxSize(60, 60));


	numberSizer->Add(spinnerText);
	numberSizer->Add(_spinner);

	numberSizer2->Add(spinnerText2);
	numberSizer2->Add(_spinner2);

	colorSizer->Add(liveText);
	colorSizer->Add(_colorPicker);

	colorSizer2->Add(deadText);
	colorSizer2->Add(_colorPicker2);
	

	sizer->Add(numberSizer);
	sizer->Add(numberSizer2);
	sizer->Add(colorSizer);
	sizer->Add(colorSizer2);
	
	sizer->Add(confirmPress);
	

	_spinner->SetValue(settings->s_gridsize);
	_spinner2->SetValue(settings->s_speed);
}

void SettingsUI::SetGridSize(wxSpinEvent& event) {
	settings->s_gridsize = _spinner->GetValue();
	//Do a grid resize?

	event.Skip();
}

void SettingsUI::SetSpeed(wxSpinEvent& event) {
	settings->s_speed = _spinner2->GetValue();
	event.Skip();
}


SettingsUI::~SettingsUI() {
	//Causes errors on close?
	//delete _spinner;
	//delete _colorPicker;

}