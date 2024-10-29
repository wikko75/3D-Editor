#include "ImGuiLayer.hpp"
#include "imgui.h"  
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "App.hpp"
#include <filesystem>

ImGuiLayer::ImGuiLayer(Window* window, std::string name)
: Layer    {name}
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


auto ImGuiLayer::beginFrame() -> void
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

auto ImGuiLayer::endFrame() -> void
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ImGuiLayer::onEvent(Event& event)
{
    // Logger::LOG("Layer | ImGui | onEvent()", Type::DEBUG);
    
}


auto ImGuiLayer::initImGui(Window* window) -> bool
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

    io.Fonts->AddFontFromFileTTF(
        std::filesystem::current_path().append("App/assets/fonts/Rubik/static/Rubik-Regular.ttf").c_str(),
        15);

    // Setup Platform/Renderer backends
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
    ImGui_ImplOpenGL3_Init();

    return true;
}



ImGuiLayer::~ImGuiLayer()
{
    Logger::LOG("Deleting up Dear Imgui layer...", Type::INFO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}




// io.DeltaTime = bd->Time > 0.0 ? (float)(current_time - bd->Time) : (float)(1.0f / 60.0f);
