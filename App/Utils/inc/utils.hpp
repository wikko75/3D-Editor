#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <expected>
#include <GLFW/glfw3.h>

auto initLog() noexcept -> void;

auto shaderStatusLogger(GLuint shader) -> std::expected<void, bool>;

auto programStatusLogger(GLuint program) -> std::expected<void, bool>;

auto proccessInput(GLFWwindow* window) noexcept -> void;

auto windowFramebufferSizeCallback(GLFWwindow* window, int width, int height) -> void;


#endif