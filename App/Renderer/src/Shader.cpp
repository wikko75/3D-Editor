#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <fmt/color.h>
#include "utils.hpp"


Shader::Shader(const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath)
: m_program {}
, m_uniform_location_map {} 
{
    auto load_result { loadShaderSource(vertexShaderPath, fragmentShaderPath) };

    if (!load_result.has_value())
    {
        return;
    }
    
    auto [vertexShaderString, fragmentShaderString] {load_result.value()};

    const char* vertexShaderData {vertexShaderString.c_str()};
    const char* fragmentShaderData {fragmentShaderString.c_str()};

    auto vertex_shader   { compileShader(vertexShaderData, GL_VERTEX_SHADER) };
    auto fragment_shader { compileShader(fragmentShaderData, GL_FRAGMENT_SHADER) };

    if (!vertex_shader.has_value() || !fragment_shader.has_value())
    {
        return;
    }

    auto shader_program { createShaderProgram(vertex_shader.value(), fragment_shader.value()) };

    if (!shader_program.has_value())
    {
        return;
    }
}

auto Shader::useShader() const noexcept -> void {
    glUseProgram(this->m_program);
}

auto Shader::getProgram() const noexcept -> GLuint {
    return this->m_program;
}

auto Shader::setUniformf(const char* name, const float v0) noexcept -> void 
{
    if (m_uniform_location_map.find(name) == m_uniform_location_map.end())
    {
        m_uniform_location_map[name] = glGetUniformLocation(m_program, name);
    }

    glUniform1f(m_uniform_location_map.at(name), v0);
}

auto Shader::setUniform3f(const char* name, const float v0, const float v1, const float v2) noexcept -> void 
{
    if (m_uniform_location_map.find(name) == m_uniform_location_map.end())
    {
        m_uniform_location_map[name] = glGetUniformLocation(m_program, name);
    }
    
    glUniform3f(m_uniform_location_map.at(name), v0, v1, v2);
}

auto Shader::setUniform4f(const char* name, const float v0, const float v1, const float v2, const float v3) noexcept -> void 
{
    if (m_uniform_location_map.find(name) == m_uniform_location_map.end())
    {
        m_uniform_location_map[name] = glGetUniformLocation(m_program, name);
    }
    
    glUniform4f(m_uniform_location_map.at(name), v0, v1, v2, v3);
}

auto Shader::setUniformMatrix3f(const char* name, GLboolean transpose, const GLfloat *value) noexcept -> void 
{
    if (m_uniform_location_map.find(name) == m_uniform_location_map.end())
    {
        m_uniform_location_map[name] = glGetUniformLocation(m_program, name);
    }
    
    glUniformMatrix3fv(m_uniform_location_map.at(name), 1, transpose, value);
}

Shader::~Shader()
{
    // glDeleteProgram(m_program);
}

auto Shader::setUniformMatrix4f(const char* name, GLboolean transpose, const GLfloat *value) noexcept -> void 
{
    if (m_uniform_location_map.find(name) == m_uniform_location_map.end())
    {
        m_uniform_location_map[name] = glGetUniformLocation(m_program, name);
    }
    
    glUniformMatrix4fv(m_uniform_location_map.at(name), 1, transpose, value);
}

auto Shader::loadShaderSource(const std::filesystem::path &vertexShader, const std::filesystem::path &fragmentShader) const noexcept -> std::expected<std::pair<std::string, std::string>, ERROR>
{
    std::ifstream vertexShaderFile(vertexShader, std::ios::in | std::ios::binary);
    std::ifstream fragmentShaderFile(fragmentShader, std::ios::in | std::ios::binary);

    if (!vertexShaderFile.is_open())
    {
        fmt::print(fg(fmt::color::red), "Error loading shader!\nShader path incorect!\nPath: {}", vertexShader.string());
        return std::unexpected {ERROR::LOADING};
    }

    if (!fragmentShaderFile.is_open())
    {
        fmt::print(fg(fmt::color::red), "Error loading shader!\nShader path incorect!\nPath: {}", fragmentShader.string());
        return std::unexpected {ERROR::LOADING};
    }

    std::stringstream vertexBuffer {};
    std::stringstream fragmentBuffer {};

    vertexBuffer << vertexShaderFile.rdbuf();
    fragmentBuffer << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    return {std::pair{vertexBuffer.str(), fragmentBuffer.str()}};
}

auto Shader::compileShader(const char* shader_src, unsigned int type) const noexcept -> std::expected<unsigned int, ERROR>
{
    auto shader { glCreateShader(type) };
    glShaderSource(shader, 1, &shader_src, 0);
    glCompileShader(shader);

    if (shaderStatusLogger(shader).error()) 
    {
        return std::unexpected{ERROR::COMPILING};
    }
    
    return shader;
}

auto Shader::createShaderProgram(unsigned int vertex_shader, unsigned int fragment_shader) noexcept -> std::expected<void, ERROR>
{
    m_program = glCreateProgram();

    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);

    glLinkProgram(m_program);

    if (programStatusLogger(this->m_program).error())
    {
        return std::unexpected{ERROR::LINKING};
    }

    glDeleteShader(vertex_shader);  
    glDeleteShader(fragment_shader);

    return {};
}