#pragma once
#include "wx/wx.h"
#include "MainWindow.h"
#include "DrawingPanel.h"
#include "Settings.h"


class SettingsUI :
    public wxDialog
{
private:
    wxWindow* _window = nullptr;
    Settings* settings;
    wxBoxSizer* _opSize;



public:

    SettingsUI(Settings* settings);


    ~SettingsUI();
};

