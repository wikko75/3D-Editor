#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Events.hpp"

enum class PROJECTION
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};


struct CameraSettings
{
    PROJECTION projection = PROJECTION::PERSPECTIVE;
    float aspect_ratio = 16/9.0f;
    float pitch =  0.0f;
    float yaw = -90.0f;
    float sensitivity = 0.5f;
    float speed = 3.f;
    bool  active = false;
};

class Camera
{
public:
    Camera(GLFWwindow* window, const glm::vec3& position, const CameraSettings& settings = {} /*DEFAULT SETTINGS*/);

    auto update(const float delta_time) noexcept -> void;

    auto onEvent(Event& event) -> void;

    auto onMouseMove(float x, float y) -> void;

    auto updatePosition(const float deltaTime) noexcept -> void;

    auto setDirection(const glm::vec3& direction) noexcept -> void;

    auto setPosition(const glm::vec3& position) noexcept -> void;

    auto setSensitivity(float sensitivity) noexcept -> void;

    auto setSpeed(float speed) noexcept -> void;

    auto setAspectRatio(float aspect_ratio) -> void;

    auto getDirection() const noexcept -> glm::vec3;

    auto getPosition() const noexcept -> glm::vec3;

    auto getSensitivity() const noexcept -> float;

    auto getSpeed() const noexcept -> float;

    auto getYaw() const noexcept -> float;

    auto getPitch() const noexcept -> float;

    auto cameraLog() const noexcept -> void;

    auto proccessInput() noexcept -> void;

    auto is_active() noexcept -> bool;

    // TODO & instead of copy
    auto getViewMatrix() const noexcept -> glm::mat4;

    auto getProjectionMatrix() const noexcept -> glm::mat4;
    
    auto getViewProjectionMatrix() const noexcept -> glm::mat4;

    ~Camera() = default;

private:
    GLFWwindow* m_window;
    glm::vec3 m_position;
    CameraSettings m_settings;
    glm::vec3 m_direction;
    glm::mat4 m_view_mtx;
    glm::mat4 m_projection_mtx;
    glm::mat4 m_view_projection_matrix;

    float m_lastX = 0.0f;
    float m_lastY = 0.0f;
    bool m_first_movement = true;
};

#endif