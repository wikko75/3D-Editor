#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <expected>
#include <GLFW/glfw3.h>
#include <functional>
#include <glm/glm.hpp>

struct Vec3Hash {
    size_t operator()(const glm::vec3& v) const {
        size_t h1 = std::hash<float>{}(v.x);
        size_t h2 = std::hash<float>{}(v.y);
        size_t h3 = std::hash<float>{}(v.z);
        
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

auto initLog() noexcept -> void;

auto shaderStatusLogger(GLuint shader) -> std::expected<void, bool>;

auto programStatusLogger(GLuint program) -> std::expected<void, bool>;

auto proccessInput(GLFWwindow* window) noexcept -> void;

auto windowFramebufferSizeCallback(GLFWwindow* window, int width, int height) -> void;


#endif