#include "files.hpp"

#include <algorithm>
#include <string>
#include <logs.hpp>

bool VidConv::fileExists(const std::string& path)
{
    FILE* file;
    fopen_s(&file, path.c_str(), "r");                    
    if (file) {
        fclose(file);
        return true;
    }

    return false;
}

bool VidConv::fillFileInfo(vidFile* file, const std::string& path)
{
    file->path = path;
    file->name = getNameFromPath(path);
    file->long_format = getExtFromPath(path);

    std::for_each(file->long_format.begin(), file->long_format.end(), [](char & c) {
        c = ::toupper(c);
    });

    bool supported = false;
    
    if (file->long_format == "MOV")
    {
        file->format = VC_FILEFORMAT_MOV;
        supported = true;
    } else if (file->long_format == "MP4") {
        file->format = VC_FILEFORMAT_MP4;
        supported = true;
    }

    if(supported)
    {
        AVFormatContext* avFormatContext = avformat_alloc_context();
        avformat_open_input(&avFormatContext, file->path.c_str(), NULL, NULL);

        avformat_find_stream_info(avFormatContext, NULL);
        
        // Only support 2 streams, video and audio
        int streamsNo = avFormatContext->nb_streams;
        if (streamsNo > 2)
        {
            vclog(VCLOGERROR, "File with more than 2 streams");
            supported = false;
        } else {
            AVStream* inputStream = avFormatContext->streams[0];
            const AVCodec* streamCodec = avcodec_find_decoder(inputStream->codecpar->codec_id);
            
            if (inputStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                file->videoCodecId = inputStream->codecpar->codec_id;
                file->videoCodecName = streamCodec->name;
            } else if (inputStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                file->audioCodecId = inputStream->codecpar->codec_id;
                file->audioCodecName = streamCodec->name;
            }

            if (streamsNo == 2)
            {
                inputStream = avFormatContext->streams[1];
                streamCodec = avcodec_find_decoder(inputStream->codecpar->codec_id);
                
                if (inputStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                    file->videoCodecId = inputStream->codecpar->codec_id;
                    file->videoCodecName = streamCodec->name;
                } else if (inputStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                    file->audioCodecId = inputStream->codecpar->codec_id;
                    file->audioCodecName = streamCodec->name;
                }
            }

            avformat_close_input(&avFormatContext);
            avformat_free_context(avFormatContext);
        }

    }

    return supported;
}

void VidConv::convertFileTo(vidFile toConvert, std::string format, std::string ffmpegFile)
{
    std::string outfile = changeFileExtension(toConvert.path, "mp4");
    std::string command = ffmpegFile + " -i " + toConvert.path + " -vcodec copy -acodec copy -y " + outfile;// + " >nul 2>nul";
    
    std::cout << "Converting file: " << toConvert.name << "... [" << outfile << "]" << std::endl;
    //WinExec(command.c_str(), SW_HIDE);
    system(command.c_str());
}

std::string VidConv::getNameFromPath(const std::string& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

std::string VidConv::getExtFromPath(const std::string& path)
{
    size_t i = path.rfind('.', path.length());
    if (i != std::string::npos) {
        return path.substr(i+1, path.length() - i);
    }

    return "";
}

std::string VidConv::getFolderFromPath(const std::string& path)
{
    return path.substr(0,path.length() - getNameFromPath(path).length());
}

std::string VidConv::changeFileExtension(const std::string& path, const std::string ext)
{
    std::string name = getNameFromPath(path);
    std::string format = getExtFromPath(path);

    return path.substr(0,path.length() - name.length()) + name.substr(0, name.length() - format.length()) + ext;
}

