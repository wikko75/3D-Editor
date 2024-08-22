#include "init.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/color.h>
#include "utils.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

auto init() noexcept -> bool {
    if (!glfwInit()) {
        fmt::print(fg(fmt::color::red), "Failed to initialize GLFW!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow*  window { glfwCreateWindow(640, 480, "3D Editor", nullptr, nullptr) };
    
    if (!window)
    {
       fmt::print(fg(fmt::color::red), "Failed to initialize window!\n");
       return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        fmt::print(fg(fmt::color::red), "Failed to initialize GLEW!");
        return -1;
    }

    initLog();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glViewport(0, 0, 640, 480);
    glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
    
    bool visible {false};
   
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        proccessInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)
        
        if(ImGui::Button("Test Button")) {
            visible =! visible;

        }
        if (visible) 
            ImGui::Text("Clicked!");
        else
            ImGui::Text("Not Clicked!");

        glClearColor(1, 0.5, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}