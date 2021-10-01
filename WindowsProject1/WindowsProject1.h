#pragma once
#include "wx/wx.h"
#include "Window.h"

class App : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	void WriteSettings();
	void ReadSettings();
};