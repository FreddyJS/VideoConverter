#include "files.h"

bool fileExists(const std::string& path)
{
    FILE* file;
    fopen_s(&file, path.c_str(), "r");                    
    if (file) {
        fclose(file);
        return true;
    }

    return false;
}

void fillFileInfo(vidFile* file, const std::string& path)
{
    file->path = path;
    file->name = getNameFromPath(path);
    file->format = getExtFromPath(path);
}


std::string getNameFromPath(const std::string& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

std::string getExtFromPath(const std::string& path)
{
    size_t i = path.rfind('.', path.length());
    if (i != std::string::npos) {
        return path.substr(i+1, path.length() - i);
    }

    return "";
}

std::string getFolderFromPath(const std::string& path)
{
    return path.substr(0,path.length() - getNameFromPath(path).length());
}

std::string changeFileExtension(const std::string& path, const std::string ext)
{
    std::string name = getNameFromPath(path);
    std::string format = getExtFromPath(path);

    return path.substr(0,path.length() - name.length()) + name.substr(0, name.length() - format.length()) + ext;
}

