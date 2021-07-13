#include <vcwindows.hpp>

#include <libav_impl.hpp>
#include <logs.hpp>

static ImVec2 lastWindowSize(0.f, 0.f);

static void TextCenter(std::string text) {
    float font_size = ImGui::GetFontSize() * text.size() / 2;
    ImGui::SameLine(ImGui::GetWindowSize().x / 2 - (font_size / 2));
    ImGui::Text(text.c_str());
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void VidConv::initOpenGL()
{

}

void VidConv::showInfoWindow(bool* dark_mode)
{
    // Info window
    // Set window pos and size
    ImVec2 size = ImGui::GetMainViewport()->WorkSize;
    size.y = 0;
        
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.8f);
        
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
#define VC_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!

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

    // If set it will close all combos so we will not have selectable options :(
    //ImGui::SetNextWindowFocus();

    ImGui::Begin("Convert Window", NULL, window_flags);

    // Maybe useless to print the no of files?      
    //ImGui::Text("Total Files: %i", fileList->size);
    ImGui::Text("Input Name: %s", toConvert.name.c_str());
    ImGui::Text("Input Format: %s", toConvert.long_format.c_str());
    ImGui::Text("Input Video Codec: %s", toConvert.videoCodecName.c_str());

    if (toConvert.audioCodecId != AV_CODEC_ID_NONE)
    {
        ImGui::Text("Input Audio Codec: %s", toConvert.audioCodecName.c_str());
    }

    // Output Options
    ImGui::NewLine();
    ImGui::Text("Output Parameters");

    static char outputName[128] = "";
    ImGui::InputTextWithHint("Output Name", "File name", outputName, IM_ARRAYSIZE(outputName));
    ImGui::SameLine(); HelpMarker("File of the new video generated.\nWithout any extension (NO video.mp4)");

    static int item_current = 0;
    ImGui::Combo("Output Format", &item_current, vc_supported_formats, IM_ARRAYSIZE(vc_supported_formats));
    ImGui::SameLine(); HelpMarker("Select the output video extension (container)");

    ImGui::NewLine();
    std::string convert_str = "Convert to " + std::string(vc_supported_formats[item_current]);

    if(ImGui::Button(convert_str.c_str()))
    {
        // Not supporting yet just changes of codecs
        if (strcmp(vc_supported_formats[item_current], toConvert.long_format.c_str()) != 0) 
        {
            std::string outfile;
                
            if (strlen(outputName) == 0) outfile = changeFileExtension(toConvert.path, vc_supported_formats[item_current]);
            else outfile = getFolderFromPath(toConvert.path) + std::string(outputName) + "." + vc_supported_formats[item_current];

            std::string command = ffmpegFile + " -i " + toConvert.path + " -vcodec copy -acodec copy " + outfile;// + ">nul 2>nul";
            vclog(VCLOGINFO, command.c_str());
            
            switch (toConvert.format)
            {
                case VC_FILEFORMAT_MOV:
                {
                    if (!VidConv::fileExists(outfile))
                    {
                        //convertFileTo(toConvert, std::string("mp4"), ffmpegFile);
                        WinExec(command.c_str(), SW_HIDE);
                    }
                } break;

                case VC_FILEFORMAT_MP4:
                {
                    if (!VidConv::fileExists(outfile))
                    {
                        //convertFileTo(toConvert, std::string("mp4"), ffmpegFile);
                        WinExec(command.c_str(), SW_HIDE);
                    }
                } break;

                default:
                    break;
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