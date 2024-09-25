#ifndef _VERTEX_ARRAY_HPP
#define _VERTEX_ARRAY_HPP

#include <GL/glew.h>
#include <VertexBuffer.hpp>
#include <vector>

class VertexArray
{
public:
    VertexArray()
    : m_id{}
    , m_buffers{} 
    {
        glGenVertexArrays(1, &m_id);
        glBindVertexArray(m_id);
    }

    auto bind() const noexcept -> void {
        glBindVertexArray(m_id);
    }

    auto unbind() const noexcept -> void {
        glBindVertexArray(0);
    }

    auto addBuffer(VertexBuffer& buffer) -> void {
        m_buffers.emplace_back(buffer);
    }

    auto bindBuffers() const noexcept -> void {
        for (const auto buffer : m_buffers) {
            buffer.bind();
        }
    }

    auto unbindBuffers() const noexcept -> void {
        for (const auto buffer : m_buffers) {
            buffer.unbind();
        }
    }

    auto getId() const noexcept -> GLuint {
        return m_id;
    }

    ~VertexArray() = default;

private:
    GLuint m_id;
    std::vector<VertexBuffer> m_buffers;
};


#endif