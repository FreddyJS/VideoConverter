#pragma once

#include <GL/gl3w.h> 
#include <imgui.h>

#include <LinkedList.hpp>
#include <files.hpp>

namespace VidConv 
{
    void initOpenGL();
    void showInfoWindow(bool* dark_mode);
    void showConvertWindow(LinkedList<vidFile>* fileList, std::string ffmpegFile);   
    void showBackgroundWindow(GLuint bgimage); 
}