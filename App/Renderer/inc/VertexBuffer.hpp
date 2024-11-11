#ifndef _VERTEX_BUFFER_HPP
#define _VERTEX_BUFFER_HPP

#include <GL/glew.h>
#include "Vertex.hpp"
#include <vector>
#include "fmt/core.h"

class VertexBuffer
{
public:
    VertexBuffer(const std::vector<Vertex>& vertices)
    : m_id{}
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
        

        const auto vertex {vertices[0]};

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::getOffset(OFFSET::POSITION)); 
        glEnableVertexAttribArray(0);

        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::getOffset(OFFSET::NORMALS)); 
        glEnableVertexAttribArray(1);
    }

    auto bind() const noexcept -> void 
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    auto unbind() const noexcept -> void 
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    auto getId() const noexcept -> GLuint 
    {
        return m_id;
    }

    // auto destroy() noexcept -> void
    // {
    //     // if (!glIsBuffer(m_id))
    //     // {
    //     //     return;
    //     // }

    //     // glDeleteBuffers(1, &m_id);
    //     // fmt::println("Vertex buffer destroyed!");
    // }

    ~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);

    };
private:
    GLuint m_id;
};

#endif