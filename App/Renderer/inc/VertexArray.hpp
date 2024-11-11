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

    auto addBuffer(VertexBuffer* buffer) -> void 
    {
        bind();
        buffer->bind();
        m_vertex_buffer.emplace_back(buffer);
    }

    auto addBuffer(IndexBuffer* buffer) -> void 
    {
        bind();
        buffer->bind();
        m_index_buffer.emplace_back(buffer);
    }

    auto getId() const noexcept -> GLuint 
    {
        return m_id;
    }

    // auto destroy() noexcept -> void
    // {
    //     for (auto buffer : m_vertex_buffer) 
    //     {
    //         buffer->destroy();
    //     }

    //     for (auto buffer : m_index_buffer)
    //     {
    //         buffer->destroy();
    //     }

    //     glDeleteVertexArrays(1, &m_id);

    //     fmt::println("Vertex array destroyed!");
    // }

    ~VertexArray()
    {

        glDeleteVertexArrays(1, &m_id);
    };

private:
    GLuint m_id;
    std::vector<VertexBuffer*> m_vertex_buffer;
    std::vector<IndexBuffer*> m_index_buffer;
};


#endif