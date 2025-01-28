#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "Logger.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Triangle.hpp"
#include "utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <unordered_set>
#include <fstream>


class Mesh
{

public:
    struct Transformation
    {
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation {0.0f, 0.0f, 0.0f};
        glm::vec3 scale    {1.0f, 1.0f, 1.0f};

        auto toString() const -> std::string 
        {
            return fmt::format("{:.3f} {:.3f} {:.3f}\n{:.3f} {:.3f} {:.3f}\n{:.3f} {:.3f} {:.3f}\n",
                position.x, position.y, position.z,
                rotation.x, rotation.y, rotation.z,
                scale.x,    scale.y,    scale.z
            );
        }
    };

public:
    Mesh(const std::vector<Vertex>& vertices, const std::shared_ptr<Shader> shader)
    : m_vertices   {vertices}
    , m_indices    {}
    , m_shader     {shader}
    , m_model_mtx  {glm::mat4(1.0f)}
    , m_render_mode{GL_TRIANGLES}
    {
        Logger::LOG("Mesh created!", Type::ERROR);

        createTriangles();

        Logger::LOG("Calculating normals...", Type::INFO);

        for (auto& vertex : m_vertices)
        {
            fmt::print("Vertex normal [ {}, {}, {} ]\n", vertex.getNormal().x, vertex.getNormal().y, vertex.getNormal().z);
        }

        m_vao = std::make_unique<VertexArray>();
        m_vbo = std::make_shared<VertexBuffer>(m_vertices);

        m_vao->addBuffer(m_vbo);
        
        calculateNormals();

        // update buffer after normal calculation
        m_vao->bind();
        m_vbo->update(0, m_vertices);

        m_selected_vertices = std::vector<bool>(m_vertices.size(), false);
        fmt::print("Selected vertices: [{}]\n", fmt::join(m_selected_vertices, ","));

    }

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
    }

    auto createTriangles() -> void
    {
        for (size_t i {0}; i <= m_vertices.size() - 3; i += 3)
        {
            m_triangles.emplace_back(i, i + 1, i + 2);
            printTriangleData(m_triangles.back());
        }
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

    auto getTransform() -> Transformation
    {
        return m_transformation;
    }

    auto setTransform(Transformation& transformation)
    {
        m_transformation = transformation;
    }


    auto calculateNormals() -> void
    {
        
        for (auto [v0, v1, v2] : m_triangles)
        {
            m_vertices[v0].setNormal(glm::vec3{0.0f, 0.0f, 0.0f});
            m_vertices[v1].setNormal(glm::vec3{0.0f, 0.0f, 0.0f});
            m_vertices[v2].setNormal(glm::vec3{0.0f, 0.0f, 0.0f});
        }

        for (const auto& triangle : m_triangles)
        {
           printTriangleNormal(triangle);
        }

        glm::vec3 edge_1 {};
        glm::vec3 edge_2 {};
        glm::vec3 cross_product {};

        for (auto& [v0, v1, v2] : m_triangles)
        {
            edge_1 = m_vertices[v1].getPosition() - m_vertices[v0].getPosition();
            edge_2 = m_vertices[v2].getPosition() - m_vertices[v0].getPosition();

            cross_product = glm::cross(edge_1, edge_2);

            fmt::print("Triangle: v0={}, v1={}, v2={}\n", v0, v1, v2);
            fmt::print("Edge1: [{}, {}, {}]\n", edge_1.x, edge_1.y, edge_1.z);
            fmt::print("Edge2: [{}, {}, {}]\n", edge_2.x, edge_2.y, edge_2.z);
            fmt::print("CrossProduct: [{}, {}, {}]\n", cross_product.x, cross_product.y, cross_product.z);


            m_vertices[v0].setNormal(glm::normalize(cross_product));
            m_vertices[v1].setNormal(glm::normalize(cross_product));
            m_vertices[v2].setNormal(glm::normalize(cross_product));
        }

        for (const auto& triangle : m_triangles)
        {
           printTriangleNormal(triangle);
        }
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


    auto getVerticesAtPosition(glm::vec3 position) -> std::vector<Vertex>
    {
        std::vector<Vertex> vertices {};

        for (const auto& vertex : m_vertices)
        {
            if (glm::distance(vertex.getPosition(), position) < 0.05)
            {
                vertices.emplace_back(vertex);
            }
        }

        return vertices;
    }

    auto selectVertexAtPosition(glm::vec3 position) -> void
    {
        Logger::LOG("Selected Vertex: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " +
        std::to_string(position.z), Type::ERROR);

        for (size_t i {0}; i < m_vertices.size(); ++i)
        {
            if (glm::distance(m_vertices[i].getPosition(), position) < 0.05)
            {
                Logger::LOG("SELECTED!", Type::ERROR);
                m_selected_vertices[i] = !m_selected_vertices[i];
                m_vertices[i].select(static_cast<int>(m_selected_vertices[i]));
                fmt::print("m_vertices[i] selected: {}\n",static_cast<int>(m_vertices[i].isSelected()));
            }
        }

        fmt::print("Selected vertices: [{}]\n", fmt::join(m_selected_vertices, ","));

        // for (auto& vertex : m_vertices)
        // {
        //  fmt::print("Vertex is selected {}\n", vertex.isSelected());
        // }

        // update buffer
        m_vao->bind();
        m_vbo->update(0, m_vertices);
        m_vao->addBuffer(m_vbo);

    }

    auto deselectAllVertices() -> void
    {
        m_selected_vertices = std::vector<bool>(m_vertices.size(), false);

        for (auto& vertex : m_vertices)
        {
            vertex.select(0);
        }

        m_vao->bind();
        m_vbo->update(0, m_vertices);
        m_vao->addBuffer(m_vbo);
    }


    auto getSelectedVerticesCount() -> int
    {
        // since we might have many vertices in the same point in space
        // we wanna count such cases as `one` occurance 
        std::unordered_set<glm::vec3, Vec3Hash> selected_vertices_positions {};

        for (size_t i {0}; i < m_selected_vertices.size(); ++i)
        {
            if (!m_selected_vertices[i])
                continue;

            selected_vertices_positions.insert(m_vertices[i].getPosition());
            
        }

        return selected_vertices_positions.size();
    }

    auto getSelectedVertices() const -> std::vector<Vertex>
    {
        //  to store only unique vertices (since there might be many in one point in space)
        std::unordered_set<glm::vec3, Vec3Hash> selected_vertices_positions {};

        std::vector<Vertex> selected_vertices {};
        selected_vertices.reserve(m_selected_vertices.size());

        for (size_t i {0}; i < m_selected_vertices.size(); ++i)
        {
            if (m_selected_vertices[i])
            {
                if (selected_vertices_positions.contains(m_vertices[i].getPosition()))
                    continue;

                selected_vertices.emplace_back(m_vertices[i]);
                selected_vertices_positions.insert(m_vertices[i].getPosition());
            }
        }

        return selected_vertices;
    }

    auto updateSelectedVertices(const glm::vec4& color) -> void
    {
        Logger::LOG("New color: [" + std::to_string(color.x) + ", " + std::to_string(color.y) + ", " + std::to_string(color.z) + ", " + std::to_string(color.w), Type::ERROR);
        for (size_t i {0}; i < m_selected_vertices.size(); ++i)
        {
            if (m_selected_vertices[i])
            {
               m_vertices[i].setColor(color);
            }
        }

        // update VertexBuffer with new data
        m_vao->bind();
        m_vbo->update(0, m_vertices);
        m_vao->addBuffer(m_vbo);
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

        calculateNormals();
        // update VertexBuffer with new data
        m_vao->bind();
        m_vbo->update(0, m_vertices);
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

    // adds new vertex in between two vertices
    auto addVertex(const Vertex& v1, const Vertex& v2) -> void
    {
        fmt::print("V1 : {}, {}, {}\n", v1.getPosition().x, v1.getPosition().y, v1.getPosition().z);
        fmt::print("V2 : {}, {}, {}\n", v2.getPosition().x, v2.getPosition().y, v2.getPosition().z);   

        // calculate middle point in all axis

        glm::vec3 v1_position {v1.getPosition()};
        glm::vec3 v2_position {v2.getPosition()};

        glm::vec3 new_vertex_position { (v1_position.x + v2_position.x) / 2,
                                        (v1_position.y + v2_position.y) / 2,
                                        (v1_position.z + v2_position.z) / 2 };

        // get all vertices of v1 position
        auto vertices_at_v1_position {getVerticesAtPosition(v1_position)};

        fmt::print("Vertices at v1 position:\n");
        for (const auto& v : vertices_at_v1_position)
        {
            fmt::print("[{}, {}, {}]\n", v.getPosition().x, v.getPosition().y, v.getPosition().z);
        }


        auto vertices_at_v2_position {getVerticesAtPosition(v2_position)};
        fmt::print("Vertices at v2 position:\n");
        for (const auto& v : vertices_at_v2_position)
        {
            fmt::print("[{}, {}, {}]\n", v.getPosition().x, v.getPosition().y, v.getPosition().z);
        }

        Vertex new_vertex {new_vertex_position};

        // form triangles between all vertices at v1 position, new_vertex and v2 position
        for (const auto& first : vertices_at_v1_position)
        {
            for (const auto& second : vertices_at_v2_position)
            {
                // new vertex color = average of neighbors
                new_vertex.setColor(first.getColor() + second.getColor() / glm::vec4{2.f});

                m_vertices.emplace_back(first);
                m_vertices.emplace_back(new_vertex);
                m_vertices.emplace_back(second);

                m_triangles.push_back({ m_vertices.size() - 3, m_vertices.size() - 2, m_vertices.size() - 1 });

                Logger::LOG("Added new triangle!", Type::INFO);
                fmt::print("Position: [{}, {}, {}]\n", new_vertex_position.x, new_vertex_position.y, new_vertex_position.z);
            }
        }

        // select newly created vertex
        deselectAllVertices();
        selectVertexAtPosition(new_vertex_position);

        // update buffers

        m_vao->bind();
        m_vbo = std::make_shared<VertexBuffer>(m_vertices);
        m_vao->addBuffer(m_vbo);
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

    enum class SerializeType
    {
        MESH,
        TRANSFORM
    };

    auto serialize(std::ofstream& file, const SerializeType type) const -> void
    {
        if (type == SerializeType::MESH)
        {
            for (const auto& vertex : m_vertices)
            {
                file << vertex.toString();
            }
        }
        else if (type == SerializeType::TRANSFORM)
        {   
            file << m_transformation.toString();
        }
    }

    ~Mesh() = default;

private:

    auto printTriangleData(const Triangle& triangle) const noexcept -> void
    {
        glm::vec3 pos {};
        glm::vec3 nor {};
        
        const auto& [v0, v1, v2] {triangle};

        pos = m_vertices[v0].getPosition();
        nor = m_vertices[v0].getNormal();
        
        fmt::print("P [{}, {}, {}]\n", pos.x, pos.y, pos.z);
        fmt::print("N [{}, {}, {}]\n", nor.x, nor.y, nor.z);

        pos = m_vertices[v1].getPosition();
        nor = m_vertices[v1].getNormal();

        fmt::print("P [{}, {}, {}]\n", pos.x, pos.y, pos.z);
        fmt::print("N [{}, {}, {}]\n", nor.x, nor.y, nor.z);

        pos = m_vertices[v2].getPosition();
        nor = m_vertices[v2].getNormal();

        fmt::print("P [{}, {}, {}]\n", pos.x, pos.y, pos.z);
        fmt::print("N [{}, {}, {}]\n\n", nor.x, nor.y, nor.z);
    }

    auto printTriangleNormal(const Triangle& triangle) const noexcept -> void
    {

        const auto& [v0, v1, v2] {triangle};

        glm::vec3 nor {m_vertices[v0].getNormal()};

        fmt::print("N[{}, {}, {}]\n", nor.x, nor.y, nor.z);

        nor = m_vertices[v1].getNormal();

        fmt::print("N[{}, {}, {}]\n", nor.x, nor.y, nor.z);

        nor = m_vertices[v2].getNormal();

        fmt::print("N[{}, {}, {}]\n\n", nor.x, nor.y, nor.z);
    }

private:
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<bool> m_selected_vertices;
    std::vector<Triangle> m_triangles;

    std::unique_ptr<VertexArray>  m_vao;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer>  m_ib;
    std::shared_ptr<Shader>       m_shader;

    glm::mat4 m_model_mtx;

    Transformation m_transformation {};

    GLenum m_render_mode;
};


#endif