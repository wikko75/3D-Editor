cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.0  
    GIT_SHALLOW TRUE
)

message("Fetching GLM...")

FetchContent_MakeAvailable(glm)

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

add_subdirectory(App)

target_sources(${PROJECT_NAME} 
    PRIVATE 
        main.cpp
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -std=c++23
    -Wall 
    -Wextra 
    -Wuninitialized
)

target_link_libraries(${PROJECT_NAME} PRIVATE GL glfw GLEW glm::glm fmt)

# message(STATUS "Available variables:")
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName} = ${${_variableName}}")
# endforeach()