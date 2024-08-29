#include "init.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/color.h>
#include "utils.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include "Shader.hpp"
#include "Vertex.hpp"


auto setup_triangle_data() -> std::pair<GLuint, GLuint> {

    GLuint vao{};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo {};
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    std::vector<Vertex> triangle {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.0f, 0.5f, 0.0f},    
    };

    fmt::print("sizeof triangle {}\ntriangel[0].size() {}\nsizeof Vertex {}", sizeof(triangle), triangle[0].size(), sizeof(Vertex));

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * triangle.size(), triangle.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, triangle[0].size(), GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return {vao, vbo};
}

auto draw_triangle(GLuint vao, const Shader& shader) -> void {   
   shader.useShader();
   glBindVertexArray(vao);
   glDrawArrays(GL_TRIANGLES, 0, 3);
}


auto init() noexcept -> bool {
    if (!glfwInit()) {
        fmt::print(fg(fmt::color::red), "Failed to initialize GLFW!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    static constexpr int window_width {1920};
    static constexpr int window_height {1080};

    GLFWwindow*  window { glfwCreateWindow(window_width, window_height, "3D Editor", nullptr, nullptr) };
    
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

    glViewport(0, 0, window_width, window_height);
    glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
    
    auto [vao, vbo] { setup_triangle_data() };

    Shader triangle_shader {
        std::filesystem::current_path() / "App"  / "assets" / "shaders" / "test_vertex.glsl",
        std::filesystem::current_path() / "App" / "assets" / "shaders" / "test_fragment.glsl"
    };

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

        glClearColor(0.902, 0.878, 0.796, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_triangle(vao, triangle_shader);

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