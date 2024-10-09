#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "Triangle.hpp"
#include <vector>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <memory>
#include "glm/glm.hpp"

class Mesh
{
public:
    Mesh(const std::vector<Triangle>& triangles)
    : m_triangles {triangles}
    {
        fmt::println("Mesh created!\nPrinting Triangles:");

        for (const auto& triangle : triangles)
        {
            printTriangleData(triangle);
        }

        fmt::print("No of triangles: {}\n", m_triangles.size());
    }


    auto calculateNormals() -> void
    {
        
        for (auto& [v0, v1, v2] : m_triangles)
        {
            v0.setNormal(glm::vec3{0.0f, 0.0f, 0.0f});
            v1.setNormal(glm::vec3{0.0f, 0.0f, 0.0f});
            v2.setNormal(glm::vec3{0.0f, 0.0f, 0.0f});
        }

        for (const auto& triangle : m_triangles)
        {
           printTriangleNormal(triangle);
        }

        glm::vec3 edge_1 {};
        glm::vec3 edge_2 {};

        glm::vec3 cross_product {};
        for (auto& [v0, v1, v2] : m_triangles)
        {
            edge_1 = v0.getPosition() - v1.getPosition();
            edge_2 = v2.getPosition() - v1.getPosition();

            cross_product = glm::cross(edge_1, edge_2);

            v0.setNormal(v0.getNormal() + cross_product);
            v1.setNormal(v1.getNormal() + cross_product);
            v2.setNormal(v2.getNormal() + cross_product);
        }

        for (auto [v0, v1, v2] : m_triangles)
        {
            v0.setNormal(glm::normalize(v0.getNormal()));
            v1.setNormal(glm::normalize(v1.getNormal()));
            v2.setNormal(glm::normalize(v2.getNormal()));
        }

        for (const auto& triangle : m_triangles)
        {
           printTriangleNormal(triangle);
        }

    }

    auto addTriangle(const Triangle& triangle) -> void
    {
        m_triangles.emplace_back(triangle);
    }


    auto getTriangles() -> std::vector<Triangle>&
    {
        return m_triangles;
    }

    ~Mesh() = default;


private:
    auto printTriangleData(const Triangle& triangle) const noexcept -> void
    {
        glm::vec3 pos {};
        glm::vec3 nor {};
        
        const auto& [v0, v1, v2] {triangle};

        pos = v0.getPosition();
        nor = v0.getNormal();
        
        fmt::print("P[{}, {}, {}]\n", pos.x, pos.y, pos.z);
        fmt::print("N[{}, {}, {}]\n", nor.x, nor.y, nor.z);

        pos = v1.getPosition();
        nor = v1.getNormal();

        fmt::print("P[{}, {}, {}]\n", pos.x, pos.y, pos.z);
        fmt::print("N[{}, {}, {}]\n", nor.x, nor.y, nor.z);

        pos = v2.getPosition();
        nor = v2.getNormal();

        fmt::print("P[{}, {}, {}]\n", pos.x, pos.y, pos.z);
        fmt::print("N[{}, {}, {}]\n\n", nor.x, nor.y, nor.z);
    }

    auto printTriangleNormal(const Triangle& triangle) const noexcept -> void
    {

        const auto& [v0, v1, v2] {triangle};

        glm::vec3 nor {v0.getNormal()};

        fmt::print("N[{}, {}, {}]\n", nor.x, nor.y, nor.z);

        nor = v1.getNormal();

        fmt::print("N[{}, {}, {}]\n", nor.x, nor.y, nor.z);

        nor = v2.getNormal();

        fmt::print("N[{}, {}, {}]\n\n", nor.x, nor.y, nor.z);
    }

private:
    std::vector<Triangle> m_triangles;
    glm::vec3 m_position { 0, 0, 0 };
    glm::vec3 m_rotation { 0, 0, 0 };
    glm::vec3 m_scale    { 1, 1, 1 };
    glm::mat4 m_model_mtx{};
};


#endif