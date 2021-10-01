#include "Window.h"
#include "Globals.h"
#include "Settings.h"

ExecutablePathsPanel::ExecutablePathsPanel(int x, int y, int w, int h, wxWindow* parent) : EmptyPanel(parent, wxID_ANY, wxPoint(x, y), wxSize(w, h))
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("Select the youtube-dl executable"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	bSizer1->Add(m_staticText2, 0, wxALL, 5);

	m_filePicker2 = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.exe"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE);
	m_filePicker2->Bind(wxEVT_FILEPICKER_CHANGED, [this](auto arg) { Globals::GetSettings().youtube_dl_path = m_filePicker2->GetPath(); });
	bSizer1->Add(m_filePicker2, 0, wxALL, 5);

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Select the ffmpeg executable"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer1->Add(m_staticText1, 0, wxALL, 5);

	m_filePicker1 = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.exe"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE);
	m_filePicker1->Bind(wxEVT_FILEPICKER_CHANGED, [this](auto arg) { Globals::GetSettings().ffmpeg_path = m_filePicker1->GetPath(); });
	bSizer1->Add(m_filePicker1, 0, wxALL, 5);

	m_button_settings = new wxButton(this, wxID_ANY, wxT("Continue"), wxDefaultPosition, wxDefaultSize, 0);
	m_button_settings->Bind(wxEVT_BUTTON, [this](auto arg) {OnButtonClickedBack(arg); });
	bSizer1->Add(m_button_settings, 0, wxALL, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	Hide();
}

void ExecutablePathsPanel::OnButtonClickedBack(wxCommandEvent& _event)
{
	Globals::GetMainWindow().windowManager->Back();
	_event.Skip();
}

void ExecutablePathsPanel::OnShow()
{
	m_filePicker2->SetPath(Globals::GetSettings().youtube_dl_path);
	m_filePicker1->SetPath(Globals::GetSettings().ffmpeg_path);
}