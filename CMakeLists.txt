cmake_minimum_required(VERSION 3.0.0)
project(VideoConverter VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(VideoConverter main.cpp)
target_link_libraries(VideoConverter PUBLIC IMGUI)

add_subdirectory("libs")
if(BUILD_EXAMPLES)
        add_subdirectory("examples")
endif()

install(TARGETS VideoConverter DESTINATION vidconv)

include(InstallRequiredSystemLibraries)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
