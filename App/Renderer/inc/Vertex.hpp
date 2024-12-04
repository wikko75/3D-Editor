#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include <fmt/core.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


enum class OFFSET
{
    POSITION,
    NORMALS,
    COLOR,
    SELECTED,
};

class Vertex
{
public:
    Vertex(const glm::vec3& position, const glm::vec3& normal = {0.0f, 0.0f, 0.0f}, const glm::vec4& color = {8.0f, 8.0f, 8.0f, 1.0f}, int selected = 0)
    : m_position {position}
    , m_normal   {normal}
    , m_color    {color} 
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

        if (type == OFFSET::COLOR)
        {
            return sizeof(m_position) + sizeof(m_normal); 
        }

        if (type == OFFSET::SELECTED)
        {
            return sizeof(m_position) + sizeof(m_normal) + sizeof(m_color); 
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

    auto getColor() const -> glm::vec4
    {
        return m_color;
    }

    auto setPosition(glm::vec3 position) -> void
    {
        m_position = position;
    }

    auto setNormal(glm::vec3 normal) -> void
    {
        m_normal = normal;
    }

    auto setColor(const glm::vec4& color) -> void
    {
        m_color = color;
    }
    
    auto isSelected() const -> int
    {
        return m_selected;
    }

    auto select(int state) -> void
    {
        m_selected = state;
    }

    auto toString() const -> std::string
    {
        return fmt::format("{:.3f} {:.3f} {:.3f}\n{:.3f} {:.3f} {:.3f}\n{:.3f} {:.3f} {:.3f} {:.3f}\n{:d}\n",
         m_position.x, m_position.y, m_position.z,
         m_normal.x,   m_normal.y,   m_normal.z,
         m_color.x,    m_color.y,    m_color.z,  m_color.w,
         m_selected
        );
    }

    ~Vertex() = default;
private:
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec4 m_color;
    int m_selected;
};

#endif