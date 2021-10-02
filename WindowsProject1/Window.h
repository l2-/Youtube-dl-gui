#pragma once
#include "wx/wx.h"
#include <wx/aui/aui.h>
#include <string>
#include <stack>
#include <wx/filepicker.h>
#include <wx/gbsizer.h>
#include <wx/listctrl.h>
#include <wx/popupwin.h>
#include "DownloadQueueListCtrl.h"
#include "DownloadSettings.h"
#include <unordered_map>
#include <wx/event.h>
#include <functional>
#include "TaskEvent.h"
#include <mutex>

const wxAuiPaneInfo DEFAULT_PANE_HINTS = wxAuiPaneInfo().CenterPane().CaptionVisible(false).CloseButton(false);

class EmptyPanel : public wxPanel
{
public:
	EmptyPanel(wxWindow* parent, int id = wxID_ANY, wxPoint point = wxDefaultPosition, wxSize size = wxDefaultSize, int style = 0, wxString title = "") : wxPanel(parent, wxID_ANY, point, size, style) {};
	virtual void OnShow() {};
};

class ListBox : public wxListBox
{
	int row = 0;
	std::unordered_map<Container, int> lookupmap1;
	std::unordered_map<int, Container> lookupmap2;
public:
	ListBox(wxWindow* parent, int id);
	void SetSelected(Container container);
	Container GetSelected();
	virtual void Select(int row);
};

class ListBoxPopup : public wxPopupTransientWindow
{
public:
	ListBox* listbox;
	ListBoxPopup(wxWindow* parent, long style);

	void Popup(wxPoint pos, wxSize minSize);
	virtual bool Create(wxWindow* parent, int flags = wxBORDER_NONE);
};

class CustomComboBox : public EmptyPanel
{
	wxTextCtrl* textctrl;
	wxButton* button;
	ListBoxPopup* listboxpopup;
public:

	CustomComboBox(wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, long style = 0, std::string name = "");

	virtual void OnShow();
};

class ExecutablePathsPanel : public EmptyPanel
{
	wxStaticText* m_staticText2;
	wxFilePickerCtrl* m_filePicker2;
	wxStaticText* m_staticText1;
	wxFilePickerCtrl* m_filePicker1;
	wxButton* m_button_settings;
public:
	ExecutablePathsPanel(int x, int y, int w, int h, wxWindow* parent = nullptr);

	void OnButtonClickedBack(wxCommandEvent& _event);
	virtual void OnShow();
};

class MainPanel : public EmptyPanel
{
	wxStaticText* m_staticText1;
	wxStaticText* m_staticText2;
	wxStaticText* m_staticText3;
	wxStaticText* m_staticText4;
	wxStaticText* m_staticText_status;
	wxStaticText* m_staticText6;
	wxButton* m_button_settings;
	wxButton* m_button2;
	wxButton* m_button3;
	wxButton* m_button_add;
	wxButton* m_button_remove;
	wxButton* m_button_open_link;
	wxButton* m_button_download;
	wxTextCtrl* m_textCtrl3;
	DownloadQueueListCtrl* m_listCtrl_download_queue;
	wxDirPickerCtrl* m_dirPicker1;
	CustomComboBox* combo_box;
	//TODO: thread worker pool
	int busy_count = 0;
	std::mutex mutex;
public:
	MainPanel(int x, int y, int w, int h, wxWindow* parent = nullptr);
	void InitializeComponents();
	void OnDownloadButtonClicked(wxCommandEvent& _event);
	void OnAddButtonClicked(wxCommandEvent& _event);
	void OnButtonClickedSettings(wxCommandEvent& _event);
	DownloadListItem* AddDownloadListItem(DownloadItem item) { return m_listCtrl_download_queue->Add(item); } // returns the row
	void RemoveDownloadListItem(DownloadListItem* item) { m_listCtrl_download_queue->RemoveRefs(&item, 1); }
	virtual void OnShow();
};

class WindowManager : public wxAuiManager
{
public:
	std::stack<EmptyPanel*> panelStack;

	void Next(EmptyPanel* frame);
	void Back();
};

class EmptyWindow : public wxFrame
{
public:
	WindowManager* windowManager = nullptr;

	// SCENES
	ExecutablePathsPanel* executablePathsPanel;
	MainPanel* mainPanel;

	EmptyWindow(std::string name, int x, int y, int w, int h, wxWindow* parent = nullptr);
	void OnInit();
	void OnIdle(wxIdleEvent& evt);
	//void OnTask(MyEvent& evt);
	void OnUpdateProgress(wxMainThreadEvent& event);

	~EmptyWindow()
	{
		delete windowManager;
	}

	void QueueEvent(wxEvent* evt) { wxFrame::QueueEvent(evt); }
};