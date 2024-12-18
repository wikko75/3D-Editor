#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "fmt/color.h"
#include "Events.hpp"
#include "Logger.hpp"



static bool is_GLFW_initialized = false;
static bool is_GLEW_initialized = false;

struct Properties
{
    std::string title;
    int width;
    int height;
    std::function<void(Event&)> callback;
};


class Window
{
public:
    Window(const Properties& properties = {"3D Editor", 1200, 800, nullptr})
    : m_properties {properties}
    {

        if (m_properties.width < 10 || m_properties.height < 10) 
        {
            Logger::LOG( "Failed to initialize Window!\nImproper size provided!", Type::ERROR);
            return;
        }

        // GLFW setup
        if (!is_GLFW_initialized)
        {
            if (!glfwInit()) 
            {
                Logger::LOG("Failed to initialize GLFW!", Type::ERROR);
                return;
            }

            // some hints
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            // below?
        }
        
        m_window = glfwCreateWindow(
            m_properties.width,
            m_properties.height,
            m_properties.title.c_str(),
            nullptr,
            nullptr
        );
    
        if (!m_window) 
        {
            Logger::LOG("Failed to initialize Window!", Type::ERROR);
            return;
        }

        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, &m_properties);

        Logger::LOG("Window Created!", Type::INFO);

        // GLEW setup
        if (!is_GLEW_initialized)
        {
            if (glewInit() != GLEW_OK) 
            {
                Logger::LOG("Failed to initialize GLEW!", Type::ERROR);
                return;
            }
        }
       

       setWindowCallbacks();

    }

    auto setEventCallback(std::function<void(Event&)> callback) -> void
    {
        m_properties.callback = callback;
    }

    auto onUpdate() -> void
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }


    auto getWidth() const noexcept -> int
    {
        return m_properties.width;
    }

    auto getHeight() const noexcept -> int
    {
        return m_properties.height;
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
    auto setWindowCallbacks() -> void
    {
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            // getting data back from glfw window
            auto window_data { static_cast<Properties*>(glfwGetWindowUserPointer(window)) };
            window_data->width  = width;
            window_data->height = height;

            WindowResizeEvent event {width, height};

            window_data->callback(event);
        });


        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
            auto window_data { static_cast<Properties*>(glfwGetWindowUserPointer(window)) };
            WindowCloseEvent event{};
            window_data->callback(event);
        });


        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            auto window_data { static_cast<Properties*>(glfwGetWindowUserPointer(window)) };
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event {key, 0};
                    window_data->callback(event);
                }
                break;
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event {key};
                    window_data->callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event {key, 1};
                    window_data->callback(event);
                    break;
                }
            }
        });


        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods)
        {
            auto window_data { static_cast<Properties*>(glfwGetWindowUserPointer(window)) };
            
            switch (action)
            {
                case GLFW_PRESS:
                {

                    MouseButtonPressedEvent event {button};
                    window_data->callback(event);                
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event {button};
                    window_data->callback(event);
                    break;
                }
                default:
                    break;
            }   
        });


        glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xoffset, double yoffset)
        {
            auto window_data { static_cast<Properties*>(glfwGetWindowUserPointer(window)) };

            MouseScrolledEvent event { static_cast<float>(xoffset),static_cast<float>(yoffset) };

            window_data->callback(event);
        });


        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xpos, double ypos)
        {
            auto window_data { static_cast<Properties*>(glfwGetWindowUserPointer(window)) };

            MouseMovedEvent event { static_cast<float>(xpos), static_cast<float>(ypos) };

            window_data->callback(event);
        });
    }

private:
    Properties m_properties;
    GLFWwindow* m_window;
};

#endif