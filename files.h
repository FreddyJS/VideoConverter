#pragma once

#include <iostream>

struct vidFile
{
    std::string name;
    std::string path;
    std::string format;
};

bool fileExists(const std::string& path);

void fillFileInfo(vidFile* file, const std::string& path);
std::string getNameFromPath(const std::string& path);
std::string getExtFromPath(const std::string& path);
std::string getFolderFromPath(const std::string& path);
std::string changeFileExtension(const std::string& path, const std::string ext);