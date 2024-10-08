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

    enum class DRAW_TYPE
    {
        ARRAYS,
        INDICES
    };

    auto render(const VertexArray& vao, const Shader& shader, DRAW_TYPE type, GLenum render_mode) const noexcept -> void {
        shader.useShader();
        vao.bind();

        if (type == DRAW_TYPE::ARRAYS)
        {
            glDrawArrays(render_mode, 0, 3);
        }
        else if (type == DRAW_TYPE::INDICES)
        {
            glDrawElements(render_mode, 36, GL_UNSIGNED_INT, 0);
        }
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