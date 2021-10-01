#include "Window.h"
#include "Globals.h"
#include "Settings.h"
#include "DownloadSettings.h"
#include <wx/display.h>
#include <wx/event.h>

ListBox::ListBox(wxWindow* parent, int id) : wxListBox(parent, id, wxDefaultPosition, wxDefaultSize, wxArrayString(), wxLB_SINGLE)
{
	Append(ContainerToString(Container::WEBM));
	lookupmap1.insert(std::pair<Container, int>(Container::WEBM, 0));
	lookupmap2.insert(std::pair<int, Container>(0, Container::WEBM));
	Append(ContainerToString(Container::MP4));
	lookupmap1.insert(std::pair<Container, int>(Container::MP4, 1));
	lookupmap2.insert(std::pair<int, Container>(1, Container::MP4));
	Append(ContainerToString(Container::MP3));
	lookupmap1.insert(std::pair<Container, int>(Container::MP3, 2));
	lookupmap2.insert(std::pair<int, Container>(2, Container::MP3));
	Append(ContainerToString(Container::M4A));
	lookupmap1.insert(std::pair<Container, int>(Container::M4A, 3));
	lookupmap2.insert(std::pair<int, Container>(3, Container::M4A));
	Append(ContainerToString(Container::VORBIS));
	lookupmap1.insert(std::pair<Container, int>(Container::VORBIS, 4));
	lookupmap2.insert(std::pair<int, Container>(4, Container::VORBIS));
}

void ListBox::Select(int row)
{
	this->row = row;
	wxListBox::Select(row);
}

void ListBox::SetSelected(Container container)
{
	Select(lookupmap1[container]);
}

Container ListBox::GetSelected()
{
	return lookupmap2[row];
}

void ListBoxPopup::Popup(wxPoint pos, wxSize minSize)
{
	wxSize size;

	int me_width = minSize.GetWidth();

	int width = GetBestSize().GetWidth();
	if (width < me_width)
	{
		width = me_width;
	}

	int height = listbox->GetBestSize().GetHeight();

	SetSize(wxSize(width, height));
	SetPosition(pos);
	wxPopupTransientWindow::Popup();
}

ListBoxPopup::ListBoxPopup(wxWindow* parent, long style) : wxPopupTransientWindow(parent, style)
{
	listbox = nullptr;
}

bool ListBoxPopup::Create(wxWindow* parent, int flags)
{
	listbox = new ListBox(this, wxID_ANY);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(listbox, wxEXPAND);
	SetSizer(sizer);

	listbox->Bind(wxEVT_MOTION, [this](auto arg) {
		int row = listbox->HitTest(arg.GetPosition());
		if (row != wxNOT_FOUND)
		{
			listbox->Select(row);
		}
	});

	return true;
}

CustomComboBox::CustomComboBox(wxWindow* parent, int id, wxPoint pos, wxSize size, long style, std::string name)
	: EmptyPanel(parent, id, pos, size, style, name)
{
	auto on_button = [this](auto arg) {
		wxPoint point = textctrl->ClientToScreen(wxPoint(0, 0));
		wxSize size = textctrl->GetSize();
		listboxpopup->Popup(wxPoint(point.x - 2, point.y + size.GetHeight()), wxSize(GetSize().GetWidth(), size.GetHeight()));
	};

	textctrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	textctrl->Bind(wxEVT_SET_FOCUS, [=](auto arg) {
		button->Navigate();
		on_button(arg);
	});
	int height = textctrl->GetSize().GetY();

	listboxpopup = new ListBoxPopup(this, wxBORDER_NONE);
	listboxpopup->Create(this);
	listboxpopup->listbox->Bind(wxEVT_LEFT_DOWN, [this](auto arg) {
		listboxpopup->Dismiss();
		Globals::GetSettings().container = listboxpopup->listbox->GetSelected();
		textctrl->SetLabelText(ContainerToString(Globals::GetSettings().container));
	});

	button = new wxButton(this, wxID_ANY, L"▾", wxDefaultPosition, wxSize(height, height));
	button->Bind(wxEVT_BUTTON, on_button);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(textctrl);
	sizer->Add(button);
	SetSizer(sizer);
}

void CustomComboBox::OnShow()
{
	textctrl->SetLabelText(ContainerToString(Globals::GetSettings().container));
	listboxpopup->listbox->SetSelected(Globals::GetSettings().container);
}