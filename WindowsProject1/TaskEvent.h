#pragma once

#include <wx/wx.h>

typedef std::function<void()> MainThreadCall;
class wxMainThreadEvent : public wxEvent
{
    DECLARE_DYNAMIC_CLASS(wxMainThreadEvent)

public:
    wxMainThreadEvent();
    wxMainThreadEvent(const wxMainThreadEvent& event);
    wxMainThreadEvent(MainThreadCall fn);
    virtual wxEvent* Clone() const;
    MainThreadCall GetFunction() { return fn; }
protected:
    MainThreadCall fn;
};

// here are the magic typedefs and the macro
typedef void(wxEvtHandler::* wxProgressEventFunction)(wxMainThreadEvent&);
#define MyEventHandler(func) wxEVENT_HANDLER_CAST(wxProgressEventFunction, func)

wxDECLARE_EVENT(wxEVT_PROGRESS_EVENT, wxMainThreadEvent);

#define EVT_PROGRESS_EVENT(id, fn) wx__DECLARE_EVT1(wxEVT_PROGRESS_EVENT, id, MyEventHandler(fn))


