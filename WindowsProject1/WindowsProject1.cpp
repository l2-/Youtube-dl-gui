#include "WindowsProject1.h"
#include "Window.h"
#include "Globals.h"
#include "Settings.h"
#include <iostream>
#include <fstream>
#include <sstream>

wxIMPLEMENT_APP_CONSOLE(App);

using namespace std;
bool App::OnInit()
{
	ReadSettings();

	Globals::GetMainWindow().OnInit();
	Globals::GetMainWindow().Show();
	return true;
}

int App::OnExit()
{
	WriteSettings();
	return 0;
}

void App::ReadSettings()
{
	ifstream settingsFile(Globals::SETTINGS_FILE_NAME);
	if (settingsFile.is_open())
	{
		stringstream buffer;
		buffer << settingsFile.rdbuf();
		Settings settings = Settings::FromJSON(buffer.str());
		Globals::SetSettings(settings);
		settingsFile.close();
	}
}

void App::WriteSettings()
{
	ofstream settingsFile;
	settingsFile.open(Globals::SETTINGS_FILE_NAME);
	if (settingsFile.is_open())
	{
		settingsFile << Globals::GetSettings().ToJSON();
		settingsFile.close();
	}
}
