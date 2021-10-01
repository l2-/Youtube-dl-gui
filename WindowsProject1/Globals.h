#pragma once
#include <functional>
#include "ConcurrentCollection.h"

class EmptyWindow;
class Settings;
class wxIdleEvent;
typedef std::function<bool(wxIdleEvent& evt)> IdleThreadCall;
typedef std::function<void()> MainThreadCall;

namespace Globals
{
	constexpr auto GUI_NAME = "Youtube-dl GUI";
	constexpr auto SETTINGS_FILE_NAME = "settings.properties";

	extern EmptyWindow & GetMainWindow();
	extern Settings& GetSettings();
	extern void SetSettings(Settings&);
	extern ConcurrentCollection<IdleThreadCall>& GetIdleThreadCallbacks();
	extern void InvokeOnMain(MainThreadCall cb);
};

