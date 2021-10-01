#include "TaskEvent.h"

wxDEFINE_EVENT(wxEVT_PROGRESS_EVENT, wxMainThreadEvent);

IMPLEMENT_DYNAMIC_CLASS(wxMainThreadEvent, wxEvent)

wxMainThreadEvent::wxMainThreadEvent()
    :wxEvent(wxID_ANY, wxEVT_PROGRESS_EVENT)
{
}

wxMainThreadEvent::wxMainThreadEvent(const wxMainThreadEvent& event)
    : wxEvent(event.GetId(), event.GetEventType())
{
    this->fn = event.fn;
}

wxMainThreadEvent::wxMainThreadEvent(MainThreadCall fn) : wxEvent(wxID_ANY, wxEVT_PROGRESS_EVENT)
{
    this->fn = fn;
}

wxEvent* wxMainThreadEvent::Clone() const
{
    return (new wxMainThreadEvent(*this));
}

