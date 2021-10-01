#include "DownloadSettings.h"
#include <iostream>
#include <unordered_map>

using namespace rapidjson;

#define SANITY_CHECK(key, doc, type) doc.HasMember(key) && !doc[key].IsNull() && doc[key].type

#define READ_STRING_SETTING(key, doc, vidinfo) if (SANITY_CHECK(#key, doc, IsString())) vidinfo.key = doc[#key].GetString();
#define READ_NUMERIC_SETTING(key, doc, vidinfo) if (SANITY_CHECK(#key, doc, IsInt())) vidinfo.key = doc[#key].GetInt();

std::unordered_map<DownloadState, std::string> downloadStateStringMap =
{
	{DownloadState::UNKNOWN, "Unknown"},
	{DownloadState::PENDING, "Pending"},
	{DownloadState::QUEUED, "Queued"},
	{DownloadState::DOWNLOADING, "Downloading"},
	{DownloadState::DOWNLOADED, "Downloaded"},
	{DownloadState::ABORTED, "Aborted"},
	{DownloadState::ERROR_, "Error"},
};

std::string DownloadStateToString(DownloadState state)
{
	return downloadStateStringMap[state];
}

std::unordered_map<Container, std::string> containerStringMap =
{
	{Container::WEBM, "webm"},
	{Container::MP4, "mp4"},
	{Container::MP3, "mp3"},
	{Container::M4A, "m4a"},
	{Container::VORBIS, "vorbis"},
};

std::unordered_map<std::string, Container> stringContainerMap =
{
	{"webm", Container::WEBM},
	{"mp4",Container::MP4},
	{"mp3",Container::MP3},
	{"m4a",Container::M4A},
	{"vorbis",Container::VORBIS},
};

Container StringToContainer(std::string container)
{
	return stringContainerMap[container];
}

std::string ContainerToString(Container container)
{
	return containerStringMap[container];
}

VideoInformation VideoInformation::FromJSON(rapidjson::Value& obj)
{
	VideoInformation vidinfo;
	bool isvalid = obj.IsObject();
	if (!isvalid)
	{
		std::cout << "ERROR parsing json for video info!" << std::endl;
	}
	else
	{
		READ_STRING_SETTING(title, obj, vidinfo);
		READ_STRING_SETTING(channel, obj, vidinfo);
		READ_STRING_SETTING(upload_date, obj, vidinfo);
		READ_STRING_SETTING(description, obj, vidinfo);
		READ_STRING_SETTING(thumbnail, obj, vidinfo);
		READ_STRING_SETTING(webpage_url, obj, vidinfo);
		READ_NUMERIC_SETTING(duration, obj, vidinfo);
		READ_NUMERIC_SETTING(view_count, obj, vidinfo);

		if (SANITY_CHECK("formats", obj, IsArray()))
		{
			for (unsigned int i = 0; i < obj["formats"].Size(); i++)
			{
				auto& i_obj = obj["formats"][i];
				VideoFormatInformation info = VideoFormatInformation::FromJSON(i_obj);
				vidinfo.formats.push_back(info);
			}
		}
	}

	return vidinfo;
}

VideoFormatInformation VideoFormatInformation::FromJSON(Value& obj)
{
	VideoFormatInformation vfi;

	READ_STRING_SETTING(format, obj, vfi);
	READ_STRING_SETTING(acodec, obj, vfi);
	READ_STRING_SETTING(vcodec, obj, vfi);
	READ_NUMERIC_SETTING(format_id, obj, vfi);
	READ_NUMERIC_SETTING(filesize, obj, vfi);
	READ_NUMERIC_SETTING(asr, obj, vfi);
	READ_NUMERIC_SETTING(width, obj, vfi);
	READ_NUMERIC_SETTING(height, obj, vfi);
	READ_NUMERIC_SETTING(fps, obj, vfi);
	READ_NUMERIC_SETTING(vbr, obj, vfi);
	return vfi;
}
