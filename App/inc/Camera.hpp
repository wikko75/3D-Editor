#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(GLFWwindow* window, float windowWidth, float windowHight, float pitch, float yaw, float sensitivity, float speed, const glm::vec3& position);

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

    ~Camera() = default;

private:
    GLFWwindow* window;
    float pitch;
    float yaw;
    float lastX;
    float lastY;
    bool firstMovement;
    float sensitivity;
    float speed;
    glm::vec3 direction;
    glm::vec3 position;
};

#endif