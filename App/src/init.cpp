#include "init.hpp"
#include <fmt/color.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include "Shader.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"


auto setup_triangle_data() -> std::pair<std::vector<Vertex>, std::vector<unsigned int>>
{
    std::vector<Vertex> cubeVertices {
    // front
    Vertex({ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}),  // 0
    Vertex({ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}),  // 1
    Vertex({-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}),  // 2
    Vertex({-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}),  // 3

    // back
    Vertex({ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}),  // 4
    Vertex({ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}),  // 5
    Vertex({-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}),  // 6
    Vertex({-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}),  // 7

    // left
    Vertex({-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}),  // 8 (3)
    Vertex({-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}),  // 9 (2)
    Vertex({-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}),  // 10 (6)
    Vertex({-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}),  // 11 (7)

    // right
    Vertex({ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}),  // 12 (0)
    Vertex({ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}),  // 13 (1)
    Vertex({ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}),  // 14 (5)
    Vertex({ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}),  // 15 (4)

    // bottom
    Vertex({ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}),  // 16 (1)
    Vertex({ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}),  // 17 (5)
    Vertex({-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}),  // 18 (6)
    Vertex({-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}),  // 19 (2)

    // up
    Vertex({ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}),  // 20 (0)
    Vertex({ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}),  // 21 (4)
    Vertex({-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}),  // 22 (7)
    Vertex({-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f})   // 23 (3)
    };

    std::vector<unsigned int> cubeIndices {
        // front
        0, 1, 2, 0, 2, 3,

        // back
        4, 5, 6, 4, 6, 7,

        // left
        8, 9, 10, 8, 10, 11,

        // right
        12, 13, 14, 12, 14, 15,

        // bottom
        16, 17, 18, 16, 18, 19,

        // up
        20, 21, 22, 20, 22, 23
};
    return {cubeVertices, cubeIndices};
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
    Camera main_camera {window, PROJECTION::PERSPECTIVE, 0.0f, -90.0f, 0.5f, 3.f, {0.0f, 0.0f, 3.0f}, false};

    if(!initImGui(window)) {
       fmt::print(fg(fmt::color::red), "Failed to initialize Dear ImGui!\n");
       return -1;
    }

    glViewport(0, 0, window_width, window_height);
    glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
    
    auto [vertices, indices] { setup_triangle_data() };

    VertexArray vao {};
    VertexBuffer vbo {vertices};
    IndexBuffer ib {indices};
   
    vao.addBuffer(vbo);
    vao.addBuffer(ib);

    Shader cube_shader {
        std::filesystem::current_path() / "App"  / "assets" / "shaders" / "test_vertex.glsl",
        std::filesystem::current_path() / "App" / "assets" / "shaders" / "test_fragment.glsl"
    };

    glm::mat4 model_mtx { 1.f };

    double prev_frame {glfwGetTime()};
    double curr_frame {};
    double delta_time {};
    
    float color[4] {1.0f, 1.0f, 1.0f, 1.0f};

    Renderer renderer {window};

    while (!glfwWindowShouldClose(window)) {
        curr_frame = glfwGetTime();
        delta_time = curr_frame - prev_frame;
        prev_frame = curr_frame;

        glfwPollEvents();
        proccessInput(window); // to be removed

        main_camera.cameraLog();
        main_camera.update(delta_time);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow(); // Show demo window! :)
        ImGui::Text("Color");
        ImGui::ColorEdit4("Color", color);
        ImGui::BeginTable("tab1", 2);
        ImGui::TableSetupColumn("COLUMMMMMN");
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Row1 | column1");
        ImGui::TableNextColumn();
        ImGui::Text("Row1 | column2");
        ImGui::Button("Button");
        ImGui::EndTable();


        if (ImGui::BeginTooltip())
        {

            if(ImGui::BeginItemTooltip())
            {
                ImGui::Text("subbb");   
            }
            ImGui::EndTooltip();
        }

        renderer.clear({0.902, 0.878, 0.796, 1});

        model_mtx = glm::mat4(1.0f);
        model_mtx = glm::translate(model_mtx, {0.0f, 0.0f, 0.f});

        cube_shader.setUniformMatrix4f("u_model_mtx", GL_FALSE, glm::value_ptr(model_mtx));
        cube_shader.setUniformMatrix4f("u_view_mtx", GL_FALSE, glm::value_ptr(main_camera.getViewMatrix()));
        cube_shader.setUniformMatrix4f("u_view_projection_mtx", GL_FALSE, glm::value_ptr(main_camera.getViewProjectionMatrix()));
        cube_shader.setUniform4f("in_color", color[0], color[1], color[2], color[3]);

        renderer.render(vao, cube_shader, Renderer::DRAW_TYPE::INDICES, GL_TRIANGLES);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }


    std::vector<Triangle> cube_triangles {};

    for (size_t i {0}; i < indices.size(); i += 3)
    {
        cube_triangles.emplace_back(Triangle{vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]]});
    }

    Mesh cube_mesh {cube_triangles};

    vao.destroy();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    fmt::print(fg(fmt::color::ghost_white), "Bye :D\n");

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
