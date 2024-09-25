#include "init.hpp"
#include <fmt/color.h>
#include "utils.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include "Shader.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * triangle.size(), triangle.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, triangle[0].size(), GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return {vao, vbo};
}

auto render_triangle(GLuint vao, const Shader& shader) -> void {   
   shader.useShader();
   glBindVertexArray(vao);
   glDrawArrays(GL_TRIANGLES, 0, 3);
}


auto initApp() noexcept -> bool {
    if (!glfwInit()) {
        fmt::print(fg(fmt::color::red), "Failed to initialize GLFW!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    static constexpr int window_width {1920 / 2};
    static constexpr int window_height {1080 / 2};

    GLFWwindow* window { glfwCreateWindow(window_width, window_height, "3D Editor", nullptr, nullptr) };
    
    if (!window) {
       fmt::print(fg(fmt::color::red), "Failed to initialize window!\n");
       return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        fmt::print(fg(fmt::color::red), "Failed to initialize GLEW!");
        return -1;
    }

    initLog();

    // all callbacks have to be registered before ImGui initialization !!!
    Camera main_camera {window, window_width, window_height, 0.0f, -90.0f, 0.5f, 3.f, {0.0f, 0.0f, 3.0f}, false};

    if(!initImGui(window)) {
       fmt::print(fg(fmt::color::red), "Failed to initialize Dear ImGui!\n");
       return -1;
    }

    glViewport(0, 0, window_width, window_height);
    glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
    
    auto [vao, vbo] { setup_triangle_data() };

    Shader triangle_shader {
        std::filesystem::current_path() / "App"  / "assets" / "shaders" / "test_vertex.glsl",
        std::filesystem::current_path() / "App" / "assets" / "shaders" / "test_fragment.glsl"
    };

    glm::mat4 model_mtx { 1.f };
    glm::mat4 view_mtx {};
    glm::mat4 projection_mtx {};

    double prev_frame {glfwGetTime()};
    double curr_frame {};
    double delta_time {};
    
    while (!glfwWindowShouldClose(window)) {
        curr_frame = glfwGetTime();
        delta_time = curr_frame - prev_frame;
        prev_frame = curr_frame;

        glfwPollEvents();
        proccessInput(window); // to be removed
        main_camera.proccessInput();

        main_camera.updatePosition(static_cast<float>(delta_time));
        main_camera.cameraLog();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)
        
        glClearColor(0.902, 0.878, 0.796, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        view_mtx = glm::lookAt(main_camera.getPosition(), main_camera.getPosition() + main_camera.getDirection(), { 0.0f, 1.0f, 0.0f });
        projection_mtx = glm::perspective(glm::radians(90.f), (float)window_width / window_height, 0.1f, 100.f);

        model_mtx = glm::mat4(1.0f);
        model_mtx = glm::translate(model_mtx, {0.0f, 0.0f, 0.f});

        triangle_shader.setUniformMatrix4f("model_mtx", GL_FALSE, glm::value_ptr(model_mtx));
        triangle_shader.setUniformMatrix4f("view_mtx", GL_FALSE, glm::value_ptr(view_mtx));
        triangle_shader.setUniformMatrix4f("projection_mtx", GL_FALSE, glm::value_ptr(projection_mtx));

        render_triangle(vao, triangle_shader);

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

auto initImGui(GLFWwindow *window) noexcept -> bool
{
    if (!window)
        return false;
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     //! Enable Keyboard Controls  (left for now though interferes with camera)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    return true;
}
