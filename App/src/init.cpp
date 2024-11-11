#include "init.hpp"
#include "App.hpp"


auto setup_triangle_data() -> std::pair<std::vector<Vertex>, std::vector<unsigned int>>
{
    std::vector<Vertex> cubeVertices {
    // front
    Vertex({ 0.5f,  0.5f,  0.5f}),  // 0
    Vertex({ 0.5f, -0.5f,  0.5f}),  // 1
    Vertex({-0.5f, -0.5f,  0.5f}),  // 2
    Vertex({-0.5f,  0.5f,  0.5f}),  // 3

    // back
    Vertex({ 0.5f,  0.5f, -0.5f}),  // 4
    Vertex({ 0.5f, -0.5f, -0.5f}),  // 5
    Vertex({-0.5f, -0.5f, -0.5f}),  // 6
    Vertex({-0.5f,  0.5f, -0.5f}),  // 7

    // left
    Vertex({-0.5f,  0.5f,  0.5f}),  // 8 (3)
    Vertex({-0.5f, -0.5f,  0.5f}),  // 9 (2)
    Vertex({-0.5f, -0.5f, -0.5f}),  // 10 (6)
    Vertex({-0.5f,  0.5f, -0.5f}),  // 11 (7)

    // right
    Vertex({ 0.5f,  0.5f,  0.5f}),  // 12 (0)
    Vertex({ 0.5f, -0.5f,  0.5f}),  // 13 (1)
    Vertex({ 0.5f, -0.5f, -0.5f}),  // 14 (5)
    Vertex({ 0.5f,  0.5f, -0.5f}),  // 15 (4)

    // bottom
    Vertex({ 0.5f, -0.5f,  0.5f}),  // 16 (1)
    Vertex({ 0.5f, -0.5f, -0.5f}),  // 17 (5)
    Vertex({-0.5f, -0.5f, -0.5f}),  // 18 (6)
    Vertex({-0.5f, -0.5f,  0.5f}),  // 19 (2)

    // up
    Vertex({ 0.5f,  0.5f,  0.5f}),  // 20 (0)
    Vertex({ 0.5f,  0.5f, -0.5f}),  // 21 (4)
    Vertex({-0.5f,  0.5f, -0.5f}),  // 22 (7)
    Vertex({-0.5f,  0.5f,  0.5f})   // 23 (3)
    };

    std::vector<unsigned int> cubeIndices {
        // front
        0, 1, 2, 0, 2, 3,

        // back
        4, 5, 6, 4, 6, 7,

        // left
        8, 9, 10, 8, 10, 11,

        // right
        12, 13, 14, 12, 14, 15,

        // bottom
        16, 17, 18, 16, 18, 19,

        // up
        20, 21, 22, 20, 22, 23
};
    return {cubeVertices, cubeIndices};
}


auto initApp() noexcept -> bool 
{
    std::unique_ptr<App> app {std::make_unique<App>()};

    app->run();

    return 0;
}
