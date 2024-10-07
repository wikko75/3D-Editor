#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <GL/glew.h>
#include <filesystem>
#include <string>
#include <expected>
#include <unordered_map>


class Shader
{

public:

    enum ERROR {
        LOADING,
        COMPILING,
        LINKING,
    };

public:
    Shader(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader);

    auto useShader() const noexcept -> void;

    auto getProgram() const noexcept -> GLuint;

    auto setUniformf(const char* name, const float v0) noexcept -> void;

    auto setUniform3f(const char* name, const float v0, const float v1, const float v2) noexcept -> void;

    auto setUniform4f(const char* name, const float v0, const float v1, const float v2, const float v3) noexcept -> void;

    auto setUniformMatrix4f(const char* name, const GLboolean transpose, const GLfloat *value) noexcept -> void;

    auto setUniformMatrix3f(const char* name, const GLboolean transpose, const GLfloat *value) noexcept -> void;

    ~Shader();


private:
    auto loadShaderSource(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader) const noexcept -> std::expected<std::pair<std::string, std::string>, ERROR>;
   
    auto compileShader(const char* shader_src, unsigned int type) const noexcept -> std::expected<unsigned int, ERROR>;
   
    auto createShaderProgram(unsigned int vertex_shader, unsigned int fragment_shader) noexcept -> std::expected<void, ERROR>;
private:
    GLuint m_program;
    std::unordered_map<std::string, int> m_uniform_location_map;
};

#endif