#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include <fmt/core.h>
#include <glm/vec3.hpp>


enum class OFFSET
{
    POSITION,
    NORMALS
};

class Vertex
{
public:
    Vertex(glm::vec3 position, glm::vec3 normal)
    : m_position {position}
    , m_normal   {normal}
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

        return 0;
    }

    ~Vertex() = default;
private:
    glm::vec3 m_position;
    glm::vec3 m_normal;
};

#endif