#include <GL/glew.h>
#include "utils.hpp"
#include <fmt/color.h>

auto initLog() noexcept -> void {
    const char* version  { reinterpret_cast<const char*>(glGetString(GL_VERSION)) };
    const char* renderer { reinterpret_cast<const char*>(glGetString(GL_RENDERER)) };
    const char* vendor   { reinterpret_cast<const char*>(glGetString(GL_VENDOR)) };

    
    fmt::print(fg(fmt::color::yellow), "OpenGL version: {}\n", version);
    fmt::print(fg(fmt::color::yellow), "Renderer: {}\n",  renderer);
    fmt::print(fg(fmt::color::yellow), "Vendor: {}\n",  vendor);

    int profile;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    if (profile & GL_CONTEXT_CORE_PROFILE_BIT) {
        fmt::print(fg(fmt::color::yellow), "OpenGL profile: Core\n");
    }
    else if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) {
        fmt::print(fg(fmt::color::yellow), "OpenGL profile: Compatibility\n");
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    fmt::print(fg(fmt::color::yellow), "Maximum nr of vertex attributes supported: {}\n", nrAttributes);
}



// std::unexpected
auto shaderStatusLogger(GLuint shader) -> std::expected<void, bool> {
    char log[512];
    int STATUS_OK {0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &STATUS_OK);

    if (!STATUS_OK)
    {
        glGetShaderInfoLog(shader, 512, nullptr, log);
        fmt::print(fg(fmt::color::red), "Shader: {}\nShader compilation failed!\nLOG: {}", shader, log);

        return std::unexpected{false};
    }

    return {};
}


auto programStatusLogger(GLuint program) -> std::expected<void, bool> {
    char log[512];
    int STATUS_OK {0};
    glGetProgramiv(program, GL_LINK_STATUS, &STATUS_OK);

    if (!STATUS_OK)
    {
        glGetProgramInfoLog(program, 512, nullptr, log);
        fmt::print(fg(fmt::color::red), "Program: {}\nProgram linking failed!\nLOG: {}", program, log);

        return std::unexpected{false};
    }

    return {};
}

auto proccessInput(GLFWwindow *window) noexcept -> void {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

auto windowFramebufferSizeCallback(GLFWwindow *window, int width, int height) -> void {
    glViewport(0, 0, width, height);
}