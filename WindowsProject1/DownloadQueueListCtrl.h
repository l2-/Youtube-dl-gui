#pragma once
#include "DownloadSettings.h"
#include <wx/listctrl.h>
#include <vector>

class DownloadListItem : public wxListItem
{
public:
	DownloadItem item;
	wxListCtrl* ctrl = nullptr;

	DownloadListItem() {}
	DownloadListItem(int id, wxListCtrl* ctrl)
	{
		m_itemId = id;
		this->ctrl = ctrl;
	}

	void Update();
};

class DownloadQueueListCtrl : public wxListView
{
	std::vector<DownloadListItem*> items;
public:
	DownloadQueueListCtrl(wxWindow* parent, int id, wxPoint pos, wxSize size, long style);
	DownloadListItem* Add(DownloadItem item);
	void Remove(int* rows, int n);
	void RemoveRefs(DownloadListItem* const* item, int n);
	DownloadItem& Get(int row);

	~DownloadQueueListCtrl()
	{
		for (auto& item : items)
		{
			delete item;
		}
	}
};