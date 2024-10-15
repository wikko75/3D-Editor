#ifndef _IMGUI_LAYER_
#define _IMGUI_LAYER_

#include "Logger.hpp"
#include "LayersSystem.hpp"
#include "imgui.h"  
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Window.hpp"


class ImGuiLayer : public Layer
{

public:
    ImGuiLayer(Window* window, std::string name = "Dear ImGui")
    : Layer{name}
    , m_window {}
    {
        Logger::LOG("Setting up Dear Imgui layer...", Type::INFO);
        
        if(!initImGui(window))
        {
            Logger::LOG("Failed to initialize Dear ImGui!", Type::ERROR);
            return;
        }

        m_window = window;
    }


    void onUpdate() override
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // render ImGui content here:
        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    } 


    void onEvent(Event& event) override
    {
        // Logger::LOG("Layer | ImGui | onEvent()", Type::DEBUG)
    }


// io.DeltaTime = bd->Time > 0.0 ? (float)(current_time - bd->Time) : (float)(1.0f / 60.0f);
    
    ~ImGuiLayer()
    {
        Logger::LOG("Deleting up Dear Imgui layer...", Type::INFO);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

private:

    auto initImGui(Window* window) -> bool
    {
        if (!window)
        {
            Logger::LOG("ImGui leyer | no window provided!", Type::ERROR);
            return false;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();


        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     //! Enable Keyboard Controls  (left for now though interferes with camera)
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        // Setup Platform/Renderer backends
        // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
        ImGui_ImplOpenGL3_Init();

        return true;
    }
    
private:
    Window* m_window;
};

#endif