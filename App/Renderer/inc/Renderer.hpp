#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "GLFW/glfw3.h"
#include "Mesh.hpp"
#include "fmt/color.h"
#include "glm/vec4.hpp"
#include "Mesh.hpp"
#include <memory.h>
class Renderer
{
public:

    Renderer(GLFWwindow *window)
    {
        if (!window) {
            fmt::print(fg(fmt::color::red), "Failed to initialize Renderer!\nReason: No window provided!\n");
            return;
        }

        m_window = window;

        // glDepthRangef(0.01, 100);

        glEnable(GL_PROGRAM_POINT_SIZE);
        enableDepthTest();
    }

    enum class DRAW_TYPE
    {
        ARRAYS,
        INDICES
    };


    auto render(const std::shared_ptr<Mesh>& mesh) const -> void
    {
        mesh->getShader()->useShader();
        mesh->getVao()->bind();

        if (mesh->getIndicesCount() == 0)
        {
            glDrawArrays(mesh->getRenderMode(), 0, mesh->getVerticesCount());
        }
        else
        {
            glDrawElements(mesh->getRenderMode(), mesh->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    auto clear(glm::vec4 clear_color = {1.0f, 1.0f, 1.0f, 1.0f}) const noexcept -> void {

        // glClearColor(0.902, 0.878, 0.796, 1);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        if (m_depth_test)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    auto submit(std::pair<std::shared_ptr<VertexArray>, std::shared_ptr<Shader>> drawable_data) -> void
    {
        m_drawable_list.emplace_back(drawable_data);
    }

    auto enableDepthTest() -> void
    {
        glEnable(GL_DEPTH_TEST);
        m_depth_test = true;
    }

    auto disableDepthTest() -> void
    {
        glDisable(GL_DEPTH_TEST);
        m_depth_test = false;
    }

    auto enableWireframeMode() const -> void
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto disableWireframeMode() const -> void
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
       
    ~Renderer() = default;

private:
    GLFWwindow* m_window;
    std::vector<std::pair<std::shared_ptr<VertexArray>, std::shared_ptr<Shader>>> m_drawable_list;
    bool m_depth_test;
};


#endif