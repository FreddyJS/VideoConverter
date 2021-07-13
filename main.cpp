// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <GL/gl3w.h>            // Initialize with gl3wInit()
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#include <SDL.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifndef DEBUG_MODE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

// Here we add our includes
#include <vcwindows.hpp>
#include <LinkedList.hpp>
#include <files.hpp>
#include <logs.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <images.hpp>

#include <libav_impl.hpp>

LinkedList<videoFile>* fileList = new LinkedList<videoFile>();

static void glfw_error_callback(int error, const char* description)
{
    vclog(VCLOGERROR, "glfw error %d: %s\n", error, description);
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    int i;
    for (i = 0;  i < count;  i++)
    {
        videoFile droped;
        std::string file(paths[i]);

        bool valid = VidConv::fillFileInfo(&droped, file);
        vclog(VCLOGINFO, "Dropped File: %s", droped.path.c_str());

        // Should go after the if !valid but i dont support other files than mov
        //VidConv::showVideoInfo(file.c_str());

        if (!valid)
        {
            vclog(VCLOGWARNING, "Not supported file: %s", droped.name.c_str());
            continue;
        }

        fileList->addLast(droped);
    }
}

std::string getInstallationPath(char* path)
{
    std::string launchPath(path);
    std::string install_dir = VidConv::getFolderFromPath(launchPath);
    install_dir = install_dir.substr(0, install_dir.length() -1);

    return VidConv::getFolderFromPath(install_dir.c_str());
}

int main(int argc, char** argv)
{
    avcodec_register_all();
    av_register_all();
    
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(640, 360, "VidConverter ImGUI GLFW+OpenGL3", NULL, NULL);

    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSetDropCallback(window, drop_callback);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gl3wInit() != 0;

    if (err)
    {
        vclog(VCLOGERROR, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    std::string installDir = getInstallationPath(argv[0]);
    vclog(VCLOGINFO, "---- VideoConverter v0.1.0 Initialized ----");
    vclog(VCLOGINFO, "Installation Directory: %s", installDir.c_str());

    IMGUI_CHECKVERSION();
    ImGuiContext* imguictx = ImGui::CreateContext();
    vclog(VCLOGINFO, "Created imgui context %p" , imguictx);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::string fontFile = installDir + std::string("res/DroidSans.ttf");
    io.Fonts->AddFontFromFileTTF(fontFile.c_str(), 18.0f);

    // Our state
    bool dark_mode = true;
    ImVec4 bgcolor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;

    std::string bgfile = installDir + "res/background.jpg";
    std::string ffmpegFile = installDir + "res/ffmpeg.exe";
    bool ret = LoadTextureFromFile(bgfile.c_str(), &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    static ImVec2 lastWindowSize(0.f, 0.f);

    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        
        // Switch theme mode
        if(dark_mode)
            ImGui::StyleColorsDark();
        else
            ImGui::StyleColorsLight();

        if (fileList->size) 
        {
            VidConv::showConvertWindow(fileList, ffmpegFile);
        }
        
        VidConv::showInfoWindow(&dark_mode);
        VidConv::showBackgroundWindow(my_image_texture);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(bgcolor.x * bgcolor.w, bgcolor.y * bgcolor.w, bgcolor.z * bgcolor.w, bgcolor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete(fileList);

    return 0;
}
