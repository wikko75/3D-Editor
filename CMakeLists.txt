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

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -std=c++23
    -Wall 
    -Wextra 
    -Wuninitialized
)


target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW)
