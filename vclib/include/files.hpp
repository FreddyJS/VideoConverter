#pragma once

#include <iostream>
#include <Windows.h>
#include <libav_impl.hpp>
namespace VidConv {

    enum fileFormat {
        VC_FILEFORMAT_MP4,
        VC_FILEFORMAT_MOV
    };

    struct vidFile
    {
        std::string name;
        std::string path;
        std::string long_format;
        fileFormat format;

        std::string videoCodecName;
        std::string audioCodecName;    

        AVCodecID videoCodecId = AV_CODEC_ID_NONE;
        AVCodecID audioCodecId = AV_CODEC_ID_NONE;
    };

    bool fileExists(const std::string& path);
    bool fillFileInfo(vidFile* file, const std::string& path);
    void convertFileTo(vidFile toConvert, std::string format, std::string ffmpegFile);

    std::string getNameFromPath(const std::string& path);
    std::string getExtFromPath(const std::string& path);
    std::string getFolderFromPath(const std::string& path);
    std::string changeFileExtension(const std::string& path, const std::string ext);

}

typedef struct VidConv::vidFile videoFile;