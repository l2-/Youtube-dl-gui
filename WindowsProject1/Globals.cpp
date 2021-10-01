#include "Globals.h"
#include "Window.h"
#include "Settings.h"

using namespace Globals;

EmptyWindow* mainWindow = new EmptyWindow(GUI_NAME, 60, 60, 900, 600);
ConcurrentCollection<IdleThreadCall> idle_thread_calls;

EmptyWindow& Globals::GetMainWindow()
{
	return *mainWindow;
}

Settings settings;
Settings& Globals::GetSettings()
{
	return settings;
}

void Globals::SetSettings(Settings& _settings)
{
	settings = _settings;
}

ConcurrentCollection<IdleThreadCall>& Globals::GetIdleThreadCallbacks()
{
	return idle_thread_calls;
}

void Globals::InvokeOnMain(MainThreadCall cb)
{
	wxMainThreadEvent evt(cb);
	mainWindow->QueueEvent(evt.Clone());
}
