add_library(Utils STATIC)

target_sources(Utils PRIVATE
    src/utils.cpp
    inc/utils.hpp
    inc/Logger.hpp
)

target_compile_options(
    Utils PRIVATE
    -std=c++23
    -Wall 
    -Wextra 
    -Wuninitialized
)

target_link_libraries(Utils PRIVATE GL glfw fmt GLEW)

target_include_directories(Utils PUBLIC inc)

target_link_libraries(${PROJECT_NAME} PRIVATE Utils)