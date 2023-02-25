#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"



class SettingsUI :
    public wxDialog
{
private:
  
    wxSpinCtrl* _spinner = nullptr;
    wxSpinCtrl* _spinner2 = nullptr;
    wxColourPickerCtrl* _colorPicker = nullptr;
    wxColourPickerCtrl* _colorPicker2 = nullptr;

public:

    Settings* settings;
    SettingsUI(wxWindow* parent, Settings* _settings);
    void SetGridSize(wxSpinEvent& event);
    void SetSpeed(wxSpinEvent& event);

    ~SettingsUI();
    wxDECLARE_EVENT_TABLE();
};

