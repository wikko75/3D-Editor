#ifndef UTILS_HPP
#define UTILS_HPP

#include <GLFW/glfw3.h>

auto initLog() noexcept -> void;

// void shaderStatusLogger(GLuint shader);

// void programStatusLogger(GLuint program);

auto proccessInput(GLFWwindow* window) noexcept -> void;

auto windowFramebufferSizeCallback(GLFWwindow* window, int width, int height) -> void;


#endif