#include "Window.h"
#include "ExeInterface.h"
#include "Globals.h"
#include "Settings.h"
#include "rapidjson/document.h"
#include <sstream>
#include <functional>
#include <thread>
#include <vector>
#include <wx/utils.h> 

using namespace std;

void MainPanel::InitializeComponents()
{
	m_button_add->Bind(wxEVT_BUTTON, [this](auto arg) {OnAddButtonClicked(arg); });
	m_button_settings->Bind(wxEVT_BUTTON, [this](auto arg) {OnButtonClickedSettings(arg); });
}

MainPanel::MainPanel(int x, int y, int w, int h, wxWindow* parent) : EmptyPanel(parent, wxID_ANY, wxPoint(x, y), wxSize(w, h))
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	fgSizer1->AddGrowableCol(0);
	fgSizer1->AddGrowableRow(0);
	fgSizer1->SetFlexibleDirection(wxBOTH);
	fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
	fgSizer3->AddGrowableCol(0);
	fgSizer3->AddGrowableRow(1);
	fgSizer3->AddGrowableRow(4);
	fgSizer3->SetFlexibleDirection(wxBOTH);
	fgSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer(1, 2, 0, 0);

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Enter URLs below"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	gSizer2->Add(m_staticText1, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL, 5);

	m_button_settings = new wxButton(this, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer2->Add(m_button_settings, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 5);


	fgSizer3->Add(gSizer2, 0, wxEXPAND, 5);

	m_textCtrl3 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	fgSizer3->Add(m_textCtrl3, 0, wxALL | wxEXPAND, 5);

	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer(1, 2, 0, 0);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText6 = new wxStaticText(this, wxID_ANY, wxT("Download folder"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6->Wrap(-1);
	bSizer3->Add(m_staticText6, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_dirPicker1 = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
	m_dirPicker1->Bind(wxEVT_DIRPICKER_CHANGED, [this](auto arg) {
		Globals::GetSettings().download_path = m_dirPicker1->GetPath();
	});
	m_dirPicker1->SetMinSize(wxSize(450, -1));
	bSizer3->Add(m_dirPicker1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	gSizer3->Add(bSizer3, 1, wxALIGN_LEFT, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	combo_box = new CustomComboBox(this, wxID_ANY);
	bSizer4->Add(combo_box, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_button_add = new wxButton(this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(m_button_add, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	gSizer3->Add(bSizer4, 1, wxALIGN_RIGHT, 5);


	fgSizer3->Add(gSizer3, 0, wxEXPAND, 5);

	m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("Download list"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	fgSizer3->Add(m_staticText4, 0, wxALL, 5);

	m_listCtrl_download_queue = new DownloadQueueListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	fgSizer3->Add(m_listCtrl_download_queue, 0, wxALL | wxEXPAND, 5);


	fgSizer1->Add(fgSizer3, 1, wxEXPAND, 5);

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	fgSizer4->AddGrowableCol(0);
	fgSizer4->SetFlexibleDirection(wxBOTH);
	fgSizer4->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer(1, 2, 0, 0);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxHORIZONTAL);

	m_button_remove = new wxButton(this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0);
	m_button_remove->Bind(wxEVT_BUTTON, [this](auto arg) {
		vector<int> rows;
		long selected = m_listCtrl_download_queue->GetFirstSelected();
		while (selected > -1)
		{
			rows.push_back(selected);
			selected = m_listCtrl_download_queue->GetNextSelected(selected);
		}

		m_listCtrl_download_queue->Remove(rows.data(), rows.size());
	});
	bSizer6->Add(m_button_remove, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL, 5);

	m_button_open_link = new wxButton(this, wxID_ANY, wxT("OpenLink"), wxDefaultPosition, wxDefaultSize, 0);
	m_button_open_link->Bind(wxEVT_BUTTON, [this](auto arg) {
		long selected = m_listCtrl_download_queue->GetFirstSelected();
		while (selected > -1)
		{
			DownloadItem& item = m_listCtrl_download_queue->Get(selected);
			wxLaunchDefaultBrowser(item.url);
			selected = m_listCtrl_download_queue->GetNextSelected(selected);
		}
	});
	bSizer6->Add(m_button_open_link, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL, 5);

	gSizer4->Add(bSizer6, 0, 0, 5);

	m_button_download = new wxButton(this, wxID_ANY, wxT("Download"), wxDefaultPosition, wxDefaultSize, 0);
	m_button_download->Bind(wxEVT_BUTTON, &MainPanel::OnDownloadButtonClicked, this);
	gSizer4->Add(m_button_download, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 5);


	fgSizer4->Add(gSizer4, 0, wxEXPAND, 5);

	m_staticText_status = new wxStaticText(this, wxID_ANY, wxT("Status"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText_status->Wrap(-1);
	fgSizer4->Add(m_staticText_status, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);


	fgSizer1->Add(fgSizer4, 1, wxALIGN_BOTTOM | wxEXPAND, 5);


	this->SetSizer(fgSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	this->Hide();

	InitializeComponents();
}

void HandleYoutubeDLOutputStream(MainPanel* mp, string str, DownloadItem item, DownloadListItem* item_ref)
{
	rapidjson::Document d;
	rapidjson::StringStream stream(str.c_str());

	for (int j = 0;; j++)
	{
		bool success = !d.ParseStream<rapidjson::kParseStopWhenDoneFlag>(stream).HasParseError();
		if (!success)
		{
			break;
		}
		if (d.IsObject())
		{
			VideoInformation info = VideoInformation::FromJSON(d);

			DownloadItem item;
			item.url = info.webpage_url;
			item.title = info.title;
			item.channel = info.channel;
			item.percent = 0;
			item.extension = Globals::GetSettings().container;
			item.state = DownloadState::QUEUED;

			Globals::InvokeOnMain([=]()
			{
				mp->AddDownloadListItem(item);

				mp->Update();
			});
		}
	}

	Globals::InvokeOnMain([mp, item_ref]()
	{
		mp->RemoveDownloadListItem(item_ref);
	});
}

void MainPanel::OnDownloadButtonClicked(wxCommandEvent& _event)
{
	auto items = m_listCtrl_download_queue->GetListItems();
	for (auto& item : items)
	{
		if (item->item.state == DownloadState::QUEUED)
		{
			ExeInterface::outputCallback parseFinal = [this, item, items](std::string str) -> void {
				Globals::InvokeOnMain([=]()
				{
					item->item.state = DownloadState::DOWNLOADED;
					item->Update();
					Update();
				});
			};
			ExeInterface::errorOutputCallback parseError = [this, item, items](std::string str) -> void {
				Globals::InvokeOnMain([=]()
				{
					item->item.state = DownloadState::ERROR_;
					item->Update();
					Update();
				});
			};

			item->item.state = DownloadState::DOWNLOADING;
			item->Update();
			Update();

			YoutubeDlOptionBuilder builder;
			builder.SetOutputPath(Globals::GetSettings().download_path);
			builder.Url(item->item.url);

			cout << builder.ToString() << endl;
			ExeInterface::Execute1(Globals::GetSettings().youtube_dl_path, builder.ToString(), nullptr, parseFinal, parseError);
		}
	}
}

void MainPanel::OnAddButtonClicked(wxCommandEvent& _event)
{
	vector<DownloadItem> items;
	for (int i = 0; i < m_textCtrl3->GetNumberOfLines(); i++)
	{
		string url = m_textCtrl3->GetLineText(i).ToStdString();
		if (url.length() > 0)
		{
			DownloadItem item;
			item.url = url;
			item.extension = Globals::GetSettings().container;
			item.state = DownloadState::PENDING;
			items.push_back(item);
		}
	}

	//{
	//	std::lock_guard<std::mutex> lock(mutex);
	//	busy_count += items.size();
	//}

	vector<DownloadListItem*> item_refs;
	for (auto& item : items)
	{
		item_refs.push_back(AddDownloadListItem(item));
	}

	Update();

	for (int i = 0; i < items.size(); i++)
	{
		DownloadItem item = items[i];
		DownloadListItem* item_ref = item_refs[i];

		ExeInterface::outputCallback parseFinal = [this, item, item_ref](std::string str) -> void {
			HandleYoutubeDLOutputStream(this, str, item, item_ref);
			//{
			//	std::lock_guard<std::mutex> lock{ mutex };
			//	busy_count--;
			//}
		};
		ExeInterface::errorOutputCallback parseError = [this, item, item_ref](std::string str) -> void {
			DownloadItem _item = item;
			_item.state = DownloadState::ERROR_;

			Globals::InvokeOnMain([=]()
			{
				AddDownloadListItem(_item);

				Update();
			});
		};

		YoutubeDlOptionBuilder builder;
		builder.Json();
		builder.GetAllFromPlayList();
		builder.Url(item.url);

		ExeInterface::Execute1(Globals::GetSettings().youtube_dl_path, builder.ToString(), nullptr, parseFinal, parseError);
	}

	_event.Skip();
}

void MainPanel::OnShow()
{
	m_dirPicker1->SetPath(Globals::GetSettings().download_path);
	combo_box->OnShow();
}

void MainPanel::OnButtonClickedSettings(wxCommandEvent& _event)
{
	Globals::GetMainWindow().windowManager->Next(Globals::GetMainWindow().executablePathsPanel);
	_event.Skip();
}