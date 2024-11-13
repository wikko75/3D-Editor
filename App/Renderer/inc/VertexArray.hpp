#ifndef _VERTEX_ARRAY_HPP
#define _VERTEX_ARRAY_HPP

#include <GL/glew.h>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <vector>
class VertexArray
{
public:
    VertexArray()
    : m_id{}
    , m_vertex_buffer{} 
    , m_index_buffer{}
    {
        glGenVertexArrays(1, &m_id);
        glBindVertexArray(m_id);
    }

    auto bind() const noexcept -> void 
    {
        glBindVertexArray(m_id);
    }

    auto unbind() const noexcept -> void 
    {
        glBindVertexArray(0);
    }

    auto addBuffer(std::shared_ptr<VertexBuffer> buffer) -> void 
    {
        bind();
        buffer->bind();
        m_vertex_buffer = buffer;
    }

    auto addBuffer(std::shared_ptr<IndexBuffer> buffer) -> void 
    {
        bind();
        buffer->bind();
        m_index_buffer = buffer;
    }

    auto getId() const noexcept -> GLuint 
    {
        return m_id;
    }

    ~VertexArray()
    {

        glDeleteVertexArrays(1, &m_id);
    };

private:
    GLuint m_id;
    std::shared_ptr<VertexBuffer> m_vertex_buffer;
    std::shared_ptr<IndexBuffer>  m_index_buffer;
};


#endif