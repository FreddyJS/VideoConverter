cmake_minimum_required(VERSION 3.0.0)
project(VideoConverter VERSION 0.1.0)

include(CTest)
enable_testing()

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/LinkedList)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
set(IMGUI_SOURCES ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

set(LIBS IMGUI ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-vc2010-64/glfw3.lib)

add_executable(VideoConverter main.cpp ${IMGUI_SOURCES})
target_link_libraries(VideoConverter PUBLIC ${LIBS})

if(UNIX)
    target_link_libraries(VideoConverter PUBLIC pthread)
endif()

add_subdirectory("libs")
if(BUILD_EXAMPLES)
        add_subdirectory("examples")
endif()

install(TARGETS VideoConverter DESTINATION .)

include(InstallRequiredSystemLibraries)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
