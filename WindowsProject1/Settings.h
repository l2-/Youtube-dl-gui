#pragma once

#include <string>

enum class Container;
class Settings
{
public:
	std::string youtube_dl_path;
	std::string ffmpeg_path;
	std::string download_path;
	Container container;

	std::string ToJSON();
	static Settings FromJSON(std::string string);
};
