add_library(Renderer STATIC)

target_sources(Renderer 
                    PRIVATE
                        inc/Vertex.hpp
                        inc/Shader.hpp
                        src/Shader.cpp
                        inc/VertexBuffer.hpp
                        inc/IndexBuffer.hpp
                        inc/VertexArray.hpp
                        inc/FrameBuffer.hpp
                        inc/Renderer.hpp
)

target_include_directories(Renderer PUBLIC  inc)

target_compile_options(
    Renderer PRIVATE
    -std=c++23
    -Wall 
    -Wextra 
    -Wuninitialized
)
target_link_libraries(Renderer PRIVATE Utils fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE Renderer)

message(STATUS "Hello from the Renderer!")