#ifndef _INDEX_BUFFER_HPP_
#define _INDEX_BUFFER_HPP_

#include <GL/glew.h>
#include <vector>

class IndexBuffer
{
public:
    IndexBuffer(const std::vector<unsigned int>& indices)
    {
        glGenBuffers(1,&m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    }

    auto bind() const noexcept -> void
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    } 

    auto unbind() const noexcept -> void
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    auto destroy() noexcept -> void
    {
        if (!glIsBuffer(m_id))
        {
            return;
        }

        glDeleteBuffers(1, &m_id);
        fmt::println("Vertex buffer destroyed!");

    }

    ~IndexBuffer() = default;
private:
    unsigned int m_id;
};

#endif