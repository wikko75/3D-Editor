add_library(App STATIC)

target_sources(App PRIVATE
        src/init.cpp
        inc/init.hpp
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
    App PRIVATE
    -std=c++23
    -Wall 
    -Wextra 
    -Wuninitialized
)

target_include_directories(App PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends inc)

target_include_directories(App INTERFACE inc)

target_link_libraries(App PRIVATE GL glfw GLEW fmt Utils)

target_link_libraries(${PROJECT_NAME} PRIVATE App)
