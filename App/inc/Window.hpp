#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "GLFW/glfw3.h"
#include <functional>
#include <string_view>
#include "fmt/color.h"

struct Properties
{
    std::string title;
    int width;
    int height;
};

class Event;

static bool is_GLFW_initialized = false;

class Window
{
public:
    Window(const Properties& properties = {"3D Editor", 960, 540})
    : m_properties {properties}
    {

        if (m_properties.width < 10 || m_properties.height < 10) 
        {
            fmt::print(fg(fmt::color::red), "Failed to initialize Window!\nImproper size provided!\n");
            return;
        }

        if (!is_GLFW_initialized)
        {
            if (!glfwInit()) 
            {
                fmt::print(fg(fmt::color::red), "Failed to initialize GLFW!\n");
                return;
            }

            // some hints
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        
        m_window = glfwCreateWindow(
            m_properties.width,
            m_properties.height,
            m_properties.title.c_str(),
            nullptr,
            nullptr
        );
    
        if (!m_window) {
            fmt::print(fg(fmt::color::red), "Failed to initialize Window!\n");
        }

        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, &m_properties);

        fmt::print("Window created!\n");
    }

    auto setEventCallback(std::function<void(Event&)> callback) -> void
    {
        // callback setup
    }

    auto onUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    auto getWidth() const noexcept -> int
    {
        m_properties.width;
    }

    auto getHeight() const noexcept -> int
    {
        m_properties.height;
    }

    auto getWindow() -> GLFWwindow*
    {
        return m_window;
    }

    ~Window()
    {
        glfwDestroyWindow(m_window);
    }
private:
    Properties m_properties;
    GLFWwindow* m_window;
};

#endif