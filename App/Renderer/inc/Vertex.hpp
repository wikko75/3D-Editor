#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include <fmt/core.h>
#include <glm/vec3.hpp>


enum class OFFSET
{
    POSITION,
    NORMALS,
    SELECTED,
};

class Vertex
{
public:
    Vertex(glm::vec3 position, glm::vec3 normal = {0.0f, 0.0f, 0.0f}, int selected = 0)
    : m_position {position}
    , m_normal   {normal}
    , m_selected {selected}
    {}

    auto count() const noexcept -> unsigned int 
    {
        return sizeof(Vertex) / sizeof(float); 
    }

    static auto getOffset(OFFSET type) -> unsigned int 
    {
        if (type == OFFSET::POSITION)
        {
            return 0;
        }

        if (type == OFFSET::NORMALS)
        {
            return sizeof(m_position);
        }

        if (type == OFFSET::SELECTED)
        {
            return sizeof(m_position) + sizeof(m_normal); 
        }

        return 0;
    }

    auto getPosition() const noexcept -> glm::vec3
    {
        return m_position;
    }

    auto getNormal() const noexcept -> glm::vec3
    {
        return m_normal;
    }

    auto setPosition(glm::vec3 position) -> void
    {
        m_position = position;
    }

    auto setNormal(glm::vec3 normal) -> void
    {
        m_normal = normal;
    }
    
    auto isSelected() const -> int
    {
        return m_selected;
    }

    auto select(int state) -> void
    {
        m_selected = state;
    }

    ~Vertex() = default;
private:
    glm::vec3 m_position;
    glm::vec3 m_normal;
    int m_selected;
};

#endif