cmake_minimum_required(VERSION 3.25)

project(
    3D_Editor
    VERSION 1.0 
    LANGUAGES CXX 
    HOMEPAGE_URL "https://github.com/wikko75/3D-Editor"
)

message("Project:\n${PROJECT_NAME} ${PROJECT_VERSION}\n${PROJECT_HOMEPAGE_URL}")

include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.10   
    GIT_SHALLOW TRUE
)

message("Fetching GLFW...")

FetchContent_MakeAvailable(glfw)

# Fetch GLEW
FetchContent_Declare(
    GLEW
    GIT_REPOSITORY https://github.com/nigels-com/glew.git
    GIT_TAG glew-2.2.0  
    GIT_SHALLOW TRUE
)

message("Fetching GLEW...")

FetchContent_MakeAvailable(GLEW)

# Fetch DearImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.0-docking
    GIT_SHALLOW TRUE
)

message("Fetching Dear ImGui...")

FetchContent_MakeAvailable(imgui)

# Fetch fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.0.2
    GIT_SHALLOW TRUE       
)

message("Fetching fmt...")

FetchContent_MakeAvailable(fmt)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    PRIVATE 
        main.cpp 
        utils.cpp 
        utils.hpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -std=c++23
    -Wall 
    -Wextra 
    -Wuninitialized
)

target_include_directories(${PROJECT_NAME} PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

target_link_libraries(${PROJECT_NAME} PRIVATE GL glfw GLEW fmt)


message(STATUS "Available variables:")
get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName} = ${${_variableName}}")
endforeach()