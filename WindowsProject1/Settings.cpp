#include "Settings.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "DownloadSettings.h"

using namespace rapidjson;

#define WRITE_STRING_SETTING(key, writer) writer.String(#key); write_string(writer, key);
#define READ_STRING_SETTING(key, doc, settings) if (doc.HasMember(#key)) settings.key = doc[#key].GetString();

void write_string(PrettyWriter<StringBuffer>& writer, std::string string)
{
    writer.String(string.c_str(), static_cast<SizeType>(string.length()));
}

std::string Settings::ToJSON()
{
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    writer.StartObject();

    WRITE_STRING_SETTING(youtube_dl_path, writer);
    WRITE_STRING_SETTING(ffmpeg_path, writer);
    WRITE_STRING_SETTING(download_path, writer);
    writer.String("container"); write_string(writer, ContainerToString(container));

    writer.EndObject();
    return sb.GetString();
}

Settings Settings::FromJSON(std::string string)
{
    Settings settings;

    Document d;
    d.Parse(string.c_str());
    READ_STRING_SETTING(youtube_dl_path, d, settings);
    READ_STRING_SETTING(ffmpeg_path, d, settings);
    READ_STRING_SETTING(download_path, d, settings);
    if (d.HasMember("container")) settings.container = StringToContainer(d["container"].GetString());

    return settings;
}
