#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "Logger.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>


class Mesh
{

public:
    struct Transformation
    {
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation {0.0f, 0.0f, 0.0f};
        glm::vec3 scale    {1.0f, 1.0f, 1.0f};
    };

public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::shared_ptr<Shader> shader)
    : m_vertices   {vertices}
    , m_indices    {indices}
    , m_shader     {shader}
    , m_model_mtx  {glm::mat4(1.0f)}
    , m_render_mode{GL_TRIANGLES}
    {
        Logger::LOG("Mesh created!", Type::ERROR);

        m_vao = std::make_unique<VertexArray>();
        m_vbo = std::make_shared<VertexBuffer>(m_vertices);
        m_ib  = std::make_shared<IndexBuffer>(indices);

        m_vao->addBuffer(m_vbo);
        m_vao->addBuffer(m_ib);

        m_selected_vertices = std::vector<bool>(m_vertices.size(), false);
        fmt::print("Selected vertices: [{}]\n", fmt::join(m_selected_vertices, ","));
    }
    
    auto setVertices(const std::vector<Vertex>& vertices)
    {
        m_vertices = vertices;
    }

    auto setIndices(const std::vector<unsigned int>& indices)
    {
        m_indices = indices;
    }

    auto setShader(std::shared_ptr<Shader> shader) -> void
    {
        m_shader = shader;
    }

    auto getTransform() -> Transformation&
    {
        return m_transformation;
    }

    auto recalculateModelMatrix() -> void
    {
        m_model_mtx = glm::mat4{1.0f};
        m_model_mtx = glm::translate(m_model_mtx, m_transformation.position);
        m_model_mtx = glm::rotate(m_model_mtx, glm::radians(m_transformation.rotation[0]), {1.0, 0.0, 0.0});
        m_model_mtx = glm::rotate(m_model_mtx, glm::radians(m_transformation.rotation[1]), {0.0, 1.0, 0.0});
        m_model_mtx = glm::rotate(m_model_mtx, glm::radians(m_transformation.rotation[2]), {0.0, 0.0, 1.0});
        m_model_mtx = glm::scale(m_model_mtx, m_transformation.scale);
    }


    auto selectVertexAtPosition(glm::vec3 position) -> void
    {
        Logger::LOG("Selected Vertex: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " +
        std::to_string(position.z), Type::ERROR);

        for (size_t i {0}; i < m_vertices.size(); ++i)
        {
            if (glm::distance(m_vertices[i].getPosition(), position) < 1.0)
            {
                Logger::LOG("SELECTED!", Type::ERROR);
                m_selected_vertices[i] = !m_selected_vertices[i];
                m_vertices[i].select(static_cast<int>(m_selected_vertices[i]));
                fmt::print("m_vertices[i] selected: {}\n",static_cast<int>(m_vertices[i].isSelected()));
            }
        }

        fmt::print("Selected vertices: [{}]\n", fmt::join(m_selected_vertices, ","));

        for (auto& vertex : m_vertices)
        {
         fmt::print("Vertex is selected {}\n", vertex.isSelected());
        }

        // update buffer
        m_vbo = std::make_shared<VertexBuffer>(m_vertices);
        m_vao->addBuffer(m_vbo);

    }

    auto deselectAllVertices() -> void
    {
        m_selected_vertices = std::vector<bool>(m_vertices.size(), false);

        for (auto& vertex : m_vertices)
        {
            vertex.select(0);
        }

        m_vbo = std::make_shared<VertexBuffer>(m_vertices);
        m_vao->addBuffer(m_vbo);
        
    }


    auto getSelectedVerticesCount() -> int
    {
        return std::count_if(m_selected_vertices.begin(), m_selected_vertices.end(), [](bool selected)
        {
            return selected == true;
        });
    }

    auto updateSelectedVertices(const glm::vec3& offset) -> void
    {
        Logger::LOG("Position offset: [" + std::to_string(offset.x) + ", " + std::to_string(offset.y) + ", " + std::to_string(offset.z), Type::ERROR);
        for (size_t i {0}; i < m_selected_vertices.size(); ++i)
        {
            if (m_selected_vertices[i])
            {
               updateVertexPosition(i, offset);
            }
        }

        // update VertexBuffer with new data
        m_vbo = std::make_shared<VertexBuffer>(m_vertices);
        m_vao->addBuffer(m_vbo);

    }


    auto updateVertexPosition(int index, const glm::vec3& offset) -> void
    {
        auto position { m_vertices[index].getPosition() };
        m_vertices[index].setPosition(position + offset);
        //! debug only
        position =  m_vertices[index].getPosition();
        Logger::LOG("Updated Position: [" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z), Type::ERROR);
    }

    auto setRenderMode(GLenum render_mode) -> void
    {
        m_render_mode = render_mode;
    }

    auto getShader() -> Shader*
    {
        return m_shader.get();
    }
        
    auto getModelMatrix() -> const glm::mat4&
    {
        return m_model_mtx;
    }

    auto getVao() -> VertexArray*
    {
        return m_vao.get();
    }

    auto getVerticesCount() -> int
    {
        return m_vertices.size();
    }       

    auto getIndicesCount() -> int
    {
        return m_indices.size();
    } 

    auto getRenderMode() -> GLenum
    {
        return m_render_mode;
    }

    ~Mesh() = default;

private:
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<bool> m_selected_vertices;

    std::unique_ptr<VertexArray>  m_vao;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer>  m_ib;
    std::shared_ptr<Shader>       m_shader;

    glm::mat4 m_model_mtx;

    Transformation m_transformation {};

    GLenum m_render_mode;
};


#endif