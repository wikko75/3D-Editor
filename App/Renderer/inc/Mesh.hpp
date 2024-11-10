#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "Logger.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



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
        m_vbo = std::make_unique<VertexBuffer>(m_vertices);
        m_ib  = std::make_unique<IndexBuffer>(indices);

        m_vao->addBuffer(m_vbo.get());
        m_vao->addBuffer(m_ib.get());

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

    auto selectVertex(int index) -> void
    {
        m_selected_vertices[index] = true;
    }

    auto getSelectedVertices() -> std::vector<bool>&
    {
        return m_selected_vertices;
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
    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer>  m_ib;
    std::shared_ptr<Shader>       m_shader;

    glm::mat4 m_model_mtx;

    Transformation m_transformation {};

    GLenum m_render_mode;
};


#endif