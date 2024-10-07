#include "Camera.hpp"
#include "fmt/color.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>



Camera::Camera(GLFWwindow* window, PROJECTION type, float pitch, float yaw, float sensitivity, float speed, const glm::vec3& position, bool active) 
    : m_window {window}
    , m_projection_type {type}
    , m_pitch {pitch}
    , m_yaw {yaw}
    , m_sensitivity {sensitivity}
    , m_speed {speed}
    , m_position {position}
    , m_is_active {active}
    , m_lastX {}
    , m_lastY {}
    , m_direction {}
    , m_first_movement {true}
    , m_view_mtx {}
    , m_projection_mtx {}
    , m_view_projection_matrix {}
{
    if (!m_window)
    {
        fmt::print(fg(fmt::color::red), "CAMERA: no window provided!\n");
        return;
    }  
    glfwSetWindowUserPointer(m_window, this);
    glfwSetCursorPosCallback(m_window, cursorPosCallbackStatic);

    m_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_direction.y = sin(glm::radians(m_pitch));
    m_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    
    setDirection(glm::normalize(m_direction));

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    m_lastX = window_width / 2;
    m_lastY = window_height / 2;

    m_view_mtx = glm::lookAt(m_position, m_position + m_direction, { 0.0f, 1.0f, 0.0f });
    m_projection_mtx = (type == PROJECTION::PERSPECTIVE)
                                ? 
                                glm::perspective(glm::radians(90.f), static_cast<float>(window_width) / window_height, 0.1f, 100.f)
                                :  
                                //not working yet
                                (glm::mat4)glm::ortho(0.0f, static_cast<float>(window_width), 0.0f, static_cast<float>(window_height), 0.1f, 100.0f);

    m_view_projection_matrix = m_projection_mtx * m_view_mtx;
   }

auto Camera::update(const float delta_time) noexcept -> void
{
    proccessInput();

    updatePosition(delta_time);

    //  update projection - view matrix
    int window_width, window_height;
    glfwGetWindowSize(m_window, &window_width, &window_height);

    m_view_mtx = glm::lookAt(m_position, m_position + m_direction, { 0.0f, 1.0f, 0.0f });

    m_view_projection_matrix = m_projection_mtx * m_view_mtx;
}


auto Camera::updatePosition(const float deltaTime) noexcept -> void 
{
    if (!m_is_active)
        return;
    
    glm::vec3 cameraPosition { this->getPosition() };
    glm::vec3 up { 0.0, 1.0f, 0.0f };

    // -------- WSAD movement --------
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= m_speed * deltaTime * glm::normalize(glm::cross(m_direction, up));
    }
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += m_speed * deltaTime *  glm::normalize(glm::cross(m_direction, up));
    }
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += m_speed * deltaTime * m_direction;
    }
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= m_speed * deltaTime * m_direction;
    }
    if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPosition.y += m_speed * deltaTime;
    }
    if(glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPosition.y -= m_speed * deltaTime;
    }

    setPosition(cameraPosition);
}


auto Camera::setDirection(const glm::vec3 &newDirection) noexcept -> void 
{
    m_direction = newDirection;
}


auto Camera::setPosition(const glm::vec3 &newPosition) noexcept -> void 
{
    m_position = newPosition;
}


auto Camera::setSensitivity(float newSensitivity) noexcept -> void 
{
    if (newSensitivity < 0.01f)
    {
        fmt::print(fg(fmt::color::yellow), "Camera Sensitivity must be > 0!\n");

        m_sensitivity = 0.05f;

        return;
    }
    
    m_sensitivity = newSensitivity;
}


auto Camera::setSpeed(float newSpeed) noexcept -> void 
{
    
    if (newSpeed < 0.01f)
    {
        fmt::print(fg(fmt::color::yellow), "Camera Speed must be > 0!\n");

        m_speed = 3.5f;

        return;
    }
    
    m_speed = newSpeed;
}


auto Camera::getDirection() const noexcept -> glm::vec3 
{
    return m_direction;
}

auto Camera::getPosition() const noexcept -> glm::vec3 
{
    return m_position;
}


auto Camera::getSensitivity() const noexcept -> float 
{
    return m_sensitivity;
}


auto Camera::getSpeed() const noexcept -> float 
{
    return m_speed;
}


auto Camera::cursorPosCallbackStatic(GLFWwindow *window, double xpos, double ypos) -> void 
{
    Camera* camera { static_cast<Camera*>(glfwGetWindowUserPointer(window))};

    if (!camera)
        return;

    if(ImGui::GetIO().WantCaptureMouse)
        return;
    
    camera->cursorPosCallback(window, xpos, ypos);
}


auto Camera::getYaw() const noexcept -> float 
{
    return m_yaw;
}


auto Camera::getPitch() const noexcept -> float 
{
    return m_pitch;
}


auto Camera::cameraLog() const noexcept -> void 
{
    fmt::print(
        fg(fmt::color::yellow),
        "Camera position: [{}, {}, {}]\nCamera Yaw: [{}]\nCamera Pitch: [{}]\nCamera direction: [{}, {}, {}]\n\n",
        getPosition().x, getPosition().y, getPosition().z, getYaw(), getPitch(), m_direction.x, m_direction.y, m_direction.z
    );
}


auto Camera::proccessInput() noexcept -> void 
{
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        m_is_active = true;
        fmt::print("Active!\n");
    }
    
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        m_is_active = false;
    }
}

auto Camera::getViewMatrix() const noexcept -> glm::mat4
{
    return m_view_mtx;
}


auto Camera::getProjectionMatrix() const noexcept -> glm::mat4
{
    return m_projection_mtx;
}


auto Camera::getViewProjectionMatrix() const noexcept -> glm::mat4
{
    return m_view_projection_matrix;
}


auto Camera::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) -> void 
{
    if (!m_is_active) 
    {
        m_lastX = xpos;
        m_lastY = ypos;
        return;
    }

    if (m_first_movement) 
    {
        m_lastX = static_cast<float>(xpos);
        m_lastY = static_cast<float>(ypos);
        m_first_movement = false;
    }   

    const float xOffset { static_cast<float>((xpos - m_lastX) * m_sensitivity) };
    const float yOffset { static_cast<float>((m_lastY - ypos) * m_sensitivity) };

    fmt::print("xOffset: [{}]\nyOffset: [{}]\n\n", xOffset, yOffset);

    m_yaw += xOffset;
    m_pitch += yOffset;

    m_lastX = static_cast<float>(xpos);
    m_lastY = static_cast<float>(ypos);

    if (m_pitch > 89.0f) {
        m_pitch = 89.0f; 
    }
    if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }

    m_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_direction.y = sin(glm::radians(m_pitch));
    m_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_direction = glm::normalize(m_direction);
}