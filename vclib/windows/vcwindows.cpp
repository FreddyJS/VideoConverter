#include <vcwindows.h>

static ImVec2 lastWindowSize(0.f, 0.f);

void TextCenter(std::string text) {
    float font_size = ImGui::GetFontSize() * text.size() / 2;
    ImGui::SameLine(ImGui::GetWindowSize().x / 2 - (font_size / 2));
    ImGui::Text(text.c_str());
}

void VidConv::initOpenGL()
{

}
void VidConv::showInfoWindow(bool* dark_mode)
{
    // Info window
    // Set window pos and size
    ImVec2 size = ImGui::GetMainViewport()->WorkSize;
    size.y /= 4;
        
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
        
    // Some window flags no resize
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Configuration Window", NULL, window_flags);                 // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is a video converter program, drag and drop any file to convert it :)");               // Display some text (you can use a format strings too)

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::NewLine();
    ImGui::Checkbox("Dark Mode", dark_mode);      // Edit bools storing our window open/close state
    
    ImGui::End();
}

void VidConv::showConvertWindow(LinkedList<vidFile>* fileList, std::string ffmpegFile)
{
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    if(!lastWindowSize.x == 0)
    {
        ImVec2 wpos = main_viewport->GetWorkCenter();
        wpos.x = wpos.x - lastWindowSize.x/2; wpos.y = wpos.y - lastWindowSize.y/2;

        ImGui::SetNextWindowPos(wpos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(0,0), ImGuiCond_Always);            
    }

    struct vidFile toConvert = fileList->getFirst();
            
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowFocus();

    ImGui::Begin("Convert Window", NULL, window_flags);
            
    ImGui::Text("Total Files: %i", fileList->size);
    ImGui::Text("File Name: %s", toConvert.name.c_str());
    ImGui::Text("File Format: %s", toConvert.format.c_str());

    ImGui::NewLine();

    if(ImGui::Button("Convert to MP4"))
    {
        if (toConvert.format == "mov") {
            std::string outfile = changeFileExtension(toConvert.path, "mp4");
            std::string command = ffmpegFile + " -i " + toConvert.path + " -vcodec copy -acodec copy " + outfile;// + ">nul 2>nul";
            std::cout << command << std::endl;

            if (!fileExists(outfile))
            {
                convertFileTo(toConvert, std::string("mp4"), ffmpegFile);
            }   

            fileList->remove(0);
        }
    }
            
    ImGui::SameLine();

    if(ImGui::Button("Cancel"))
    {
        fileList->remove(0);
    }

    lastWindowSize = ImGui::GetWindowSize();
    ImGui::End();
}   
void VidConv::showBackgroundWindow(GLuint bgimage)
{
    // Set the window position
    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize, ImGuiCond_Always);
    // Set the window to be transparent
    ImGui::SetNextWindowBgAlpha(0);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // Set the window to no border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("Background Window", NULL, ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoInputs |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoScrollbar);

    ImGui::Image((void*)(intptr_t)bgimage, ImGui::GetContentRegionAvail());
    ImGui::End();

    ImGui::PopStyleVar(2);
} 