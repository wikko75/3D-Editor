#ifndef _IMGUI_LAYER_
#define _IMGUI_LAYER_

#include "LayersSystem.hpp"
#include "Window.hpp"
#include "FrameBuffer.hpp"
#include "imgui.h"

class ImGuiLayer : public Layer
{

public:
    ImGuiLayer(Window* window, std::string name = "Dear ImGui");

    auto beginFrame() -> void;
    
    auto endFrame() -> void;

    void onEvent(Event& event) override;
    
    ~ImGuiLayer();

private:
    auto initImGui(Window* window) -> bool;

private:
    Window* m_window;
};

#endif