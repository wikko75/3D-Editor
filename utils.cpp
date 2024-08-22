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

auto proccessInput(GLFWwindow *window) noexcept -> void {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

auto windowFramebufferSizeCallback(GLFWwindow *window, int width, int height) -> void
{
    glViewport(0, 0, width, height);
}