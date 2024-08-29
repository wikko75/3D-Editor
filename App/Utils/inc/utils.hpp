#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <GLFW/glfw3.h>

auto initLog() noexcept -> void;

auto shaderStatusLogger(GLuint shader) -> void;

auto programStatusLogger(GLuint program) -> void;

auto proccessInput(GLFWwindow* window) noexcept -> void;

auto windowFramebufferSizeCallback(GLFWwindow* window, int width, int height) -> void;


#endif