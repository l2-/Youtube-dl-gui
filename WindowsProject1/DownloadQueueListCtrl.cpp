#include "DownloadQueueListCtrl.h"
#include <unordered_set>
#include <locale>
#include <codecvt>
#include <string>

using namespace std;

void DownloadListItem::Update()
{
	if (ctrl == nullptr)
	{
		return;
	}
	int col = 0;
	int count = ctrl->GetItemCount();
	ctrl->SetItem(m_itemId, col++, item.title);
	ctrl->SetItem(m_itemId, col++, item.channel);
	ctrl->SetItem(m_itemId, col++, item.url);
	ctrl->SetItem(m_itemId, col++, "." + ContainerToString(item.extension));

	ctrl->SetItem(m_itemId, col++, item.size < 0 ? "" : std::to_string(item.size));
	ctrl->SetItem(m_itemId, col++, item.percent < 0 ? "" : std::to_string(item.percent));
	ctrl->SetItem(m_itemId, col++, item.eta);
	ctrl->SetItem(m_itemId, col++, item.speed);
	ctrl->SetItem(m_itemId, col++, DownloadStateToString(item.state));
}

DownloadQueueListCtrl::DownloadQueueListCtrl(wxWindow* parent, int id, wxPoint pos, wxSize size, long style) :
	wxListView(parent, id, pos, size, style)
{
	int i = 0;
	InsertColumn(i++, "Title", wxLIST_FORMAT_LEFT, 120);
	InsertColumn(i++, "Channel", wxLIST_FORMAT_LEFT, 80);
	InsertColumn(i++, "URL", wxLIST_FORMAT_LEFT, 230);
	InsertColumn(i++, "Extension", wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);

	InsertColumn(i++, "Size", wxLIST_FORMAT_LEFT, 60);
	InsertColumn(i++, "Percent", wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	InsertColumn(i++, "ETA", wxLIST_FORMAT_LEFT, 60);
	InsertColumn(i++, "Speed", wxLIST_FORMAT_LEFT, 100);
	InsertColumn(i++, "Status", wxLIST_FORMAT_LEFT, 100);
}

DownloadItem& DownloadQueueListCtrl::Get(int row)
{
	return items[row]->item;
}

DownloadListItem* DownloadQueueListCtrl::Add(DownloadItem item)
{
	DownloadListItem* entry = new DownloadListItem(items.size(), this);
	entry->item = item;
	InsertItem(*entry);
	entry->Update();
	items.push_back(entry);
	return entry;
}

void DownloadQueueListCtrl::RemoveRefs(DownloadListItem* const* items_rem, int n)
{
	unordered_set<DownloadListItem*> _items_to_remove;
	for (int i = 0; i < n; i++)
	{
		_items_to_remove.insert(items_rem[i]);
	}

	vector<DownloadListItem*> items_to_keep;
	vector<DownloadListItem*> items_to_remove;
	for (int i = 0; i < this->items.size(); i++)
	{
		if (_items_to_remove.find(this->items[i]) == _items_to_remove.end())
		{
			items_to_keep.push_back(this->items[i]);
		}
		else
		{
			items_to_remove.push_back(this->items[i]);
		}
	}

	for (auto& item : items_to_remove)
	{
		DeleteItem(item->GetId());
		delete item;
	}

	items.clear();
	for (int i = 0; i < items_to_keep.size(); i++)
	{
		items_to_keep[i]->SetId(i);
		items_to_keep[i]->Update();
		items.push_back(items_to_keep[i]);
	}
}

void DownloadQueueListCtrl::Remove(int* _rows, int n)
{
	std::unordered_set<int> rows;
	for (int i = 0; i < n; i++)
	{
		rows.insert(_rows[i]);
	}

	std::vector<DownloadItem> items;
	for (int i = 0; i < this->items.size(); i++)
	{
		if (rows.find(i) == rows.end())
		{
			items.push_back(this->items[i]->item);
		}
	}

	DeleteAllItems();
	for (auto& item : this->items)
	{
		delete item;
	}
	this->items.clear();

	for (auto& item : items)
	{
		Add(item);
	}
}