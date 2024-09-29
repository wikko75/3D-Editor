#ifndef _VERTEX_BUFFER_HPP
#define _VERTEX_BUFFER_HPP

#include <GL/glew.h>
#include "Vertex.hpp"
#include <vector>

class VertexBuffer
{
public:
    VertexBuffer(const std::vector<Vertex>& vertices, unsigned long size)
    : m_id{}
    , size {0}  //! to be added
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, vertices[0].size(), GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
        glEnableVertexAttribArray(0);
    }

    auto bind() const noexcept -> void {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    auto unbind() const noexcept -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    auto getId() const noexcept -> GLuint {
        return m_id;
    }

    ~VertexBuffer() {
        glDeleteBuffers(0, &m_id);
    }

private:
    GLuint m_id;
    unsigned long size;  //! to be added
};

#endif