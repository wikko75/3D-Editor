#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "GLFW/glfw3.h"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "fmt/color.h"
#include "glm/vec4.hpp"

class Renderer
{
public:

    Renderer(GLFWwindow *window)
    : m_window {window}
    {
        if (!window) {
            fmt::print(fg(fmt::color::red), "Failed to initialize Renderer!\nReason: No window provided!\n");
            return;
        }
    }

    auto render(const VertexArray& vao, const Shader& shader) const noexcept -> void {
        shader.useShader();
        glBindVertexArray(vao.getId());
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    auto clear(glm::vec4 clear_color = {1.0f, 1.0f, 1.0f, 1.0f}) const noexcept -> void {
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }
       
    ~Renderer() = default;

private:
    GLFWwindow* m_window;
};


#endif