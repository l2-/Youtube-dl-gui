#pragma once
#include <string>
#include <functional>
#include <wx/process.h>
#include "wx/thread.h"
#include <sstream>

namespace ExeInterface
{
	// callback for every line of stdout
	typedef std::function<void(std::string)> stdoutCallback;
	// callback for the final output
	typedef std::function<void(std::string)> outputCallback;
	// callback for the error output
	typedef std::function<void(std::string)> errorOutputCallback;

	// Cross-platform using wxExecute
	void Execute1(std::string path_to_exe, std::string command, stdoutCallback stdout_callback = nullptr, outputCallback output_callback = nullptr, errorOutputCallback error_output = nullptr);
	// Windows only using CreateProcessA
	void Execute(std::string path_to_exe, std::string command, stdoutCallback stdout_callback = nullptr, outputCallback output_callback = nullptr);
};

class SimpleProcess : public wxProcess
{
	bool hasTerminated = false;
public:
	std::stringstream ss;
	ExeInterface::stdoutCallback _stdoutCallback = nullptr;
	ExeInterface::outputCallback _outputCallback = nullptr;
	ExeInterface::outputCallback _error_output = nullptr;

	SimpleProcess(int flags);

	bool HasInput();
	bool HasErrorInput();
	virtual void OnTerminate(int pid, int status);
	bool HasTerminated() { return hasTerminated; }
};

class SimpleThread : public wxThread
{
	std::function<void()> fn;
public:
	SimpleThread(std::function<void()> fn, wxThreadKind kind = wxTHREAD_DETACHED);

	virtual void* Entry();
};