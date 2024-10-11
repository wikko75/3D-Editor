#ifndef _IMGUI_LAYER_
#define _IMGUI_LAYER_

#include "LayersSystem.hpp"

class ImGuiLayer : public Layer
{

public:
    ImGuiLayer(std::string name = "Dear ImGui")
    : Layer{name}
    {
        
        // init code
        fmt::println("Setting up Dear Imgui layer...");
    }


    void onUpdate() override
    {
        // rendering stuff
        fmt::println("Layer | Imgui | onUpdate()");
    } 


    void onEvent(Event& event) override
    {
        // capture events
        fmt::println("Layer | ImGui | onEvent()");
    }

    
    ~ImGuiLayer()
    {
        fmt::println("Deleting up Dear Imgui layer...");
        
    }
};

#endif