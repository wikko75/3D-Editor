#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include <fmt/core.h>

class Vertex
{
private:
    float x;
    float y;
    float z;
    // normal
    // ARGB color
public:
    Vertex(float pos_x, float pos_y, float pos_z)
    : x {pos_x}
    , y {pos_y}
    , z {pos_z} 
    {}

    auto size() const noexcept-> int {
        return 3;
    }

    ~Vertex() = default;
};

#endif