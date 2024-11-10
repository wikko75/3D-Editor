#ifndef _APP_HPP_
#define _APP_HPP_

#include <memory>
#include "Window.hpp"
#include "Events.hpp"
#include "ImGuiLayer.hpp"
#include "EditorLayer.hpp"

class App
{
public:
    App();

    auto run() -> void;

    auto onEvent(Event& event) -> void;

    auto onClose() -> void;

    static auto get() -> App*;

    ~App() = default;
    
private:
    std::unique_ptr<Window>      m_window;
    std::unique_ptr<ImGuiLayer>  m_imgui_layer;
    std::unique_ptr<EditorLayer> m_editor_layer;
    bool m_should_exit;

    static App* s_instance;
};

#endif