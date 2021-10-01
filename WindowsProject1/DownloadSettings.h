#pragma once
#include <string>
#include <vector>
#include "rapidjson/document.h"

enum class DownloadState
{
	UNKNOWN,
	PENDING,
	QUEUED,
	DOWNLOADING,
	DOWNLOADED,
	ABORTED,
	ERROR_
};

enum class Container
{
	WEBM,
	MP4,
	MP3,
	M4A,
	VORBIS
};

extern std::string ContainerToString(Container state);
extern Container StringToContainer(std::string container);
extern std::string DownloadStateToString(DownloadState state);

class DownloadItem
{
public:
	std::string title;
	std::string channel;
	std::string url;
	Container extension = Container::MP4;
	int size = -1;
	float percent = -1;
	std::string eta;
	std::string speed;
	DownloadState state = DownloadState::UNKNOWN;

	DownloadItem() = default;
	DownloadItem(const DownloadItem& item)
	{
		title = item.title;
		channel = item.channel;
		url = item.url;
		extension = item.extension;
		size = item.size;
		percent = item.percent;
		eta = item.eta;
		speed = item.speed;
		state = item.state;
	}
};

class YoutubeDlOptionBuilder
{
public:
	void GetAllFromPlayList() { command += " --yes-playlist"; }
	void Url(std::string url) { command += " \"" + url + "\""; }
	void Json() { command += " --dump-json"; };
	std::string ToString() { return command; }
private:
	std::string command;
};

class VideoFormatInformation
{
public:
	std::string format = "null";
	int format_id = -1;
	int filesize = -1;

	//audio
	int asr = -1;
	std::string acodec = "null";

	//video
	int width = -1;
	int height = -1;
	int fps = -1;
	int vbr = -1;
	std::string vcodec = "null";

	static VideoFormatInformation FromJSON(rapidjson::Value& obj);
};

class VideoInformation
{
public:
	std::string title = "null";
	std::string channel = "null";
	std::string upload_date = "null";
	std::string description = "null";
	std::string thumbnail = "";
	std::string webpage_url = "";
	int duration = -1;
	int view_count = -1;

	std::vector<VideoFormatInformation> formats;

	static VideoInformation FromJSON(rapidjson::Value& obj);
};