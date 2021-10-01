#include "ExeInterface.h"
#include <windows.h>
#include <string>
#include <tchar.h>
#include <iostream>

#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include "Globals.h"

using namespace std;

void SimpleProcess::OnTerminate(int pid, int status)
{
	if (_outputCallback != nullptr) { _outputCallback(ss.str()); }
	hasTerminated = true;
	//wxProcess::OnTerminate(pid, status);
}

SimpleProcess::SimpleProcess(int flags) : wxProcess(flags)
{
	Bind(wxEVT_END_PROCESS, [this](auto arg) {
		OnTerminate(arg.GetPid(), arg.GetExitCode());
	});
}

bool SimpleProcess::HasInput()
{
	bool hasInput = false;

	if (IsInputAvailable())
	{
		wxTextInputStream tis(*GetInputStream());

		// this assumes that the output is always line buffered
		wxString line = tis.ReadLine();
		ss << line << endl;
		if (_stdoutCallback != nullptr) { _stdoutCallback(line.ToStdString()); }

		hasInput = true;
	}

	if (IsErrorAvailable())
	{
		wxTextInputStream tis(*GetErrorStream());

		// this assumes that the output is always line buffered
		wxString line = tis.ReadLine();
		if (_error_output != nullptr) { _error_output(line.ToStdString()); }

		hasInput = true;
	}

	return hasInput;
}

void ExeInterface::Execute1(std::string path_to_exe, std::string command, stdoutCallback stdout_callback, outputCallback output_callback, errorOutputCallback error_output)
{
	SimpleProcess* proc = new SimpleProcess(wxPROCESS_REDIRECT);
	proc->_stdoutCallback = stdout_callback;
	proc->_outputCallback = output_callback;
	proc->_error_output = error_output;
	auto call = [=]()
	{
		while (!proc->HasTerminated())
		{
			proc->HasInput();
			// busy wait..... dont know wxwidgets alternatice
			wxMilliSleep(10);
		}

		delete proc;
	};

	int return_code = wxExecute(path_to_exe + command, wxEXEC_ASYNC, proc);
	SimpleThread* thread = new SimpleThread(call);
	thread->Run();
}

SimpleThread::SimpleThread(std::function<void()> fn, wxThreadKind kind) : wxThread(kind)
{
	this->fn = fn;
}

wxThread::ExitCode SimpleThread::Entry()
{
	fn();

	return NULL;
}

void ExeInterface::Execute(std::string path_to_exe, std::string command, stdoutCallback stdout_callback, outputCallback output_callback)
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFOA startupInfo;
	SECURITY_ATTRIBUTES saAttr;

	HANDLE stdoutReadHandle = NULL;
	HANDLE stdoutWriteHandle = NULL;

	static char cmdline[65536];
	DWORD bytes_read;

	DWORD exitcode;

	strcpy_s(cmdline, sizeof(cmdline), (path_to_exe + command).c_str());

	memset(&saAttr, 0, sizeof(saAttr));
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if (!CreatePipe(&stdoutReadHandle, &stdoutWriteHandle, &saAttr, 5000))
	{
		printf("CreatePipe: %u\n", GetLastError());
		return;
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(stdoutReadHandle, HANDLE_FLAG_INHERIT, 0))
	{
		printf("SetHandleInformation: %u\n", GetLastError());
		return;
	}

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.hStdError = stdoutWriteHandle;
	startupInfo.hStdOutput = stdoutWriteHandle;
	startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	startupInfo.dwFlags |= STARTF_USESTDHANDLES;

	// memset(&processInfo, 0, sizeof(processInfo));  // Not actually necessary

	//printf("Starting.\n");

	if (!CreateProcessA(NULL, cmdline, NULL, NULL, TRUE,
		CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT, NULL, 0, &startupInfo, &processInfo))
	{
		printf("CreateProcessA ERROR: %u\n", GetLastError());
		return;
	}

	CloseHandle(stdoutWriteHandle);

	static char tBuf[65536];
	stringstream ss;

	for (;;) {
		//printf("Just before ReadFile(...)\n");
		if (!ReadFile(stdoutReadHandle, tBuf, 65536 - 1, &bytes_read, NULL))
		{
			int error = GetLastError();
			if (error != 109) // pipe closed
			{
				printf("ReadFile ERROR: %u\n", error);
			}
			break;
		}
		//printf("Just after ReadFile, read %u byte(s)\n", bytes_read);
		if (bytes_read > 0)
		{
			tBuf[bytes_read] = '\0';
			if (stdout_callback != nullptr)
			{
				string str = string(tBuf);
				stdout_callback(str);
			}
			//printf("Output: %s^\n", tBuf);
			ss << tBuf;
		}
	}

	if (output_callback != nullptr) { output_callback(ss.str()); }
	//printf("Output finished: %s^\n", ss.str().c_str());

	if (WaitForSingleObject(processInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
	{
		printf("WaitForSingleObject ERROR: %u\n", GetLastError());
		return;
	}

	if (!GetExitCodeProcess(processInfo.hProcess, &exitcode))
	{
		printf("GetExitCodeProcess ERROR: %u\n", GetLastError());
		return;
	}

	//printf("Exit code: %u\n", exitcode);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}