cmake_minimum_required(VERSION 3.0.0)
project(VideoConverter VERSION 0.1.1)

#include(CTest)
#enable_testing()

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME VideoConverter)

add_subdirectory("libs")

# ----- SET INCLUDE DIRS -----#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/LinkedList ${CMAKE_CURRENT_SOURCE_DIR}/vclib/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/libav/include)

# ----- SET UP IMGUI LIB -----#
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
set(IMGUI_SOURCES ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp ${IMGUI_DIR}/imgui.h)

set(LIBS IMGUI ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-vc2010-64/glfw3.lib)

# ----- SET UP FFMPEG EXE -----#
set(FFMPEG_EXE ${CMAKE_SOURCE_DIR}/libs/ffmpeg/ffmpeg.exe)
add_executable(FFMPEG IMPORTED)

set(LIBAV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/libav)
set(LIBAV ${LIBAV}
    ${LIBAV_DIR}/libavcodec.lib
    ${LIBAV_DIR}/libavdevice.lib
    ${LIBAV_DIR}/libavfilter.lib
    ${LIBAV_DIR}/libavformat.lib
    ${LIBAV_DIR}/libavresample.lib
    ${LIBAV_DIR}/libswscale.lib
    ${LIBAV_DIR}/libavutil.lib
    "Ws2_32.lib"
    "bcrypt.lib"
)

# ----- VIDCONVERTER -----#
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(SOURCES ${SOURCES} 
    ${IMGUI_SOURCES}
    vclib/logs/logs.cpp
    vclib/files/files.cpp
    vclib/files/images.cpp
    vclib/windows/vcwindows.cpp
    vclib/libav_backend/libav_impl.cpp
    vclib/include/libav_impl.hpp
    main.cpp
)

add_executable(VideoConverter ${SOURCES})

set(PCH_HEADERS 
    <Windows.h>
    <GL/gl3w.h>
    <GLFW/glfw3.h>
    <iostream>
    <SDL.h>
)

# DEBUG_MODE?
target_compile_definitions(VideoConverter PRIVATE DEBUG_MODE=1)

target_precompile_headers(VideoConverter PRIVATE ${PCH_HEADERS})
target_link_libraries(VideoConverter PUBLIC ${LIBS} ${LIBAV})

if(UNIX)
    target_link_libraries(VideoConverter PUBLIC pthread)
endif()

# ----- RULES FOR CPACK -----#
set(VC_RESOURCES ${VC_RESOURCES}
    ${CMAKE_SOURCE_DIR}/res/background.jpg
    ${CMAKE_SOURCE_DIR}/res/DroidSans.ttf
)

install(FILES ${VC_RESOURCES} DESTINATION res)
install(FILES ${FFMPEG_EXE} DESTINATION res)

install(TARGETS VideoConverter DESTINATION bin RENAME VideoConverter.exe)
include(InstallRequiredSystemLibraries)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS 
    "CreateShortCut '$DESKTOP\\\\VideoConverter.lnk' '$INSTDIR\\\\bin\\\\VideoConverter.exe'"
)

set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS 
    "Delete \\\"$DESKTOP\\\\VideoConverter.lnk\\\""
)

set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\VideoConverter.lnk' '$INSTDIR\\\\bin\\\\VideoConverter.exe' '-foo -bar'"
)

set(CPACK_NSIS_DELETE_ICONS_EXTRA
    "Delete '$SMPROGRAMS\\\\$START_MENU\\\\Uninstall.lnk'"
)

set(CPACK_NSIS_UNINSTALL_NAME Uninstall)

include(CPack)
