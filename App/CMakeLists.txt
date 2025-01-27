target_sources(${PROJECT_NAME} PRIVATE
        inc/Camera.hpp
        src/Camera.cpp
        inc/Events.hpp
        inc/Window.hpp
        inc/App.hpp
        src/App.cpp
        inc/LayersSystem.hpp
        inc/ImGuiLayer.hpp
        src/ImGuiLayer.cpp
        inc/EditorLayer.hpp
        src/EditorLayer.cpp
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
        ${imguifiledialog_SOURCE_DIR}/ImGuiFileDialog.h
        ${imguifiledialog_SOURCE_DIR}/ImGuiFileDialogConfig.h
        ${imguifiledialog_SOURCE_DIR}/ImGuiFileDialog.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends ${imguifiledialog_SOURCE_DIR} inc)

target_include_directories(${PROJECT_NAME} PRIVATE inc)

add_subdirectory(Utils)

add_subdirectory(Renderer)
