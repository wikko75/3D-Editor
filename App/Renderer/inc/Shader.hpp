#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <GL/glew.h>
#include <filesystem>
#include <string>
#include <expected>


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

    auto setUniformf(const char* name, float v0) const noexcept -> void;

    auto setUniform3f(const char* name, float v0, float v1, float v2) const noexcept -> void;

    auto setUniform4f(const char* name, float v0, float v1, float v2, float v3) const noexcept -> void;

    auto setUniformMatrix4f(const char* name, GLboolean transpose, const GLfloat *value) const noexcept -> void;

    auto setUniformMatrix3f(const char* name, GLboolean transpose, const GLfloat *value) const noexcept -> void;

    ~Shader() = default;


private:
    auto loadShaderSource(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader) const noexcept -> std::expected<std::pair<std::string, std::string>, ERROR>;
   
    auto compileShader(const char* shader_src, unsigned int type) const noexcept -> std::expected<unsigned int, ERROR>;
   
    auto createShaderProgram(unsigned int vertex_shader, unsigned int fragment_shader) noexcept -> std::expected<void, ERROR>;
private:
    GLuint m_program;
};

#endif