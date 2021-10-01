#include "Window.h"
#include "Globals.h"
#include "Settings.h"
#include <iostream>

#include <wx/event.h>
#include "TaskEvent.h"

using namespace std;
void WindowManager::Next(EmptyPanel* frame)
{
	WindowManager* manager = Globals::GetMainWindow().windowManager;
	if (manager->panelStack.size() > 0)
	{
		auto current = manager->panelStack.top();
		current->Hide();
		manager->DetachPane(current);
	}
	frame->Show();
	frame->OnShow();
	manager->AddPane(frame, DEFAULT_PANE_HINTS);
	manager->panelStack.push(frame);

	manager->Update();
}

void WindowManager::Back()
{
	WindowManager* manager = Globals::GetMainWindow().windowManager;
	if (manager->panelStack.size() > 1)
	{
		auto current = manager->panelStack.top();
		manager->panelStack.pop();
		current->Hide();
		manager->DetachPane(current);

		current = manager->panelStack.top();
		current->Show();
		current->OnShow();
		manager->AddPane(current, DEFAULT_PANE_HINTS);
	}
}

EmptyWindow::EmptyWindow(std::string name, int x, int y, int w, int h, wxWindow* parent) : wxFrame(parent, wxID_ANY, name, wxPoint(x, y), wxSize(w, h))
{
	executablePathsPanel = new ExecutablePathsPanel(-1, -1, -1, -1, this);
	mainPanel = new MainPanel(-1, -1, -1, -1, this);
	Bind(wxEVT_IDLE, [this](auto arg) {OnIdle(arg); });
}

void EmptyWindow::OnInit()
{
	windowManager = new WindowManager();
	windowManager->SetManagedWindow(this);

	windowManager->Next(mainPanel);
	if (Globals::GetSettings().youtube_dl_path == "")
	{
		windowManager->Next(executablePathsPanel);
	}

	//  HAS TO BE HERE????
	Bind(wxEVT_PROGRESS_EVENT, &EmptyWindow::OnUpdateProgress, this, wxID_ANY);

}

void EmptyWindow::OnIdle(wxIdleEvent& evt)
{
	Globals::GetIdleThreadCallbacks().remove_if([&](auto arg) {
		return arg(evt);
	});
}

void EmptyWindow::OnUpdateProgress(wxMainThreadEvent& evt)
{
	evt.GetFunction()();
}