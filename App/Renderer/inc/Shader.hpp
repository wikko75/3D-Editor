#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <GL/glew.h>
#include <filesystem>
#include <string>

class Shader
{
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
    GLuint program;
};

#endif