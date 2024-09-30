#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(GLFWwindow* window, float windowWidth, float windowHight, float pitch, float yaw, float sensitivity, float speed, const glm::vec3& position, bool is_active);

    static auto cursorPosCallbackStatic(GLFWwindow* window, double xpos, double ypos) -> void;

    auto cursorPosCallback(GLFWwindow* window, double xpos, double ypos) -> void;

    auto updatePosition(float deltaTime) noexcept -> void;

    auto setDirection(const glm::vec3& direction) noexcept -> void;

    auto setPosition(const glm::vec3& position) noexcept -> void;

    auto setSensitivity(float sensitivity) noexcept -> void;

    auto setSpeed(float speed) noexcept -> void;

    auto getDirection() const noexcept -> glm::vec3;

    auto getPosition() const noexcept -> glm::vec3;

    auto getSensitivity() const noexcept -> float;

    auto getSpeed() const noexcept -> float;

    auto getYaw() const noexcept -> float;

    auto getPitch() const noexcept -> float;

    auto cameraLog() const noexcept -> void;

    auto proccessInput() noexcept -> void;

    ~Camera() = default;

private:
    GLFWwindow* m_window;
    float m_pitch;
    float m_yaw;
    float m_sensitivity;
    float m_speed;
    glm::vec3 m_position;
    bool m_is_active;
    float m_lastX;
    float m_lastY;
    glm::vec3 m_direction;
    bool m_firstMovement;
};

#endif