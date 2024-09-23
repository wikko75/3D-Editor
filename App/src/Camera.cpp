#include "Camera.hpp"
#include "fmt/color.h"
#include "imgui.h"

Camera::Camera(GLFWwindow* window, float windowWidth, float windowHight, float pitch, float yaw, float sensitivity, float speed,  const glm::vec3& position) 
    : window {window}
    , firstMovement {true}
    , pitch {pitch}, yaw {yaw}
    , lastX {windowWidth / 2}
    , lastY {windowHight / 2}
    , sensitivity {sensitivity}
    , speed {speed}
    , position {position}
    , direction {}
{        
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, cursorPosCallbackStatic);

    this->direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->direction.y = sin(glm::radians(pitch));
    this->direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    setDirection(glm::normalize(this->direction));
}

auto Camera::updatePosition(float deltaTime) noexcept -> void 
{
    glm::vec3 cameraPosition { this->getPosition() };
    glm::vec3 up { 0.0, 1.0f, 0.0f };

    // -------- WSAD movement --------
    if(glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -=  this->speed * deltaTime * glm::normalize(glm::cross(this->direction, up));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition +=  this->speed * deltaTime *  glm::normalize(glm::cross(this->direction, up));
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += this->speed * deltaTime * this->direction;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= this->speed * deltaTime * this->direction;
    }
    if(glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPosition.y += this->speed * deltaTime;
    }
    if(glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPosition.y -= this->speed * deltaTime;
    }

    this->setPosition(cameraPosition);
}


auto Camera::setDirection(const glm::vec3 &newDirection) noexcept -> void
{
    this->direction = newDirection;
}


auto Camera::setPosition(const glm::vec3 &newPosition) noexcept -> void
{
    this->position = newPosition;
}


auto Camera::setSensitivity(float newSensitivity) noexcept -> void
{
    if (newSensitivity < 0.01f)
    {
        fmt::print(fg(fmt::color::yellow), "Camera Sensitivity must be > 0!\n");

        this->sensitivity = 0.05f;

        return;
    }
    
    this->sensitivity = newSensitivity;
}


auto Camera::setSpeed(float newSpeed) noexcept -> void
{
    
    if (newSpeed < 0.01f)
    {
        fmt::print(fg(fmt::color::yellow), "Camera Speed must be > 0!\n");

        this->speed = 3.5f;

        return;
    }
    
    this->speed = newSpeed;
}


auto Camera::getDirection() const noexcept -> glm::vec3
{
    return this->direction;
}

auto Camera::getPosition() const noexcept -> glm::vec3
{
    return this->position;
}


auto Camera::getSensitivity() const noexcept -> float
{
    return this->sensitivity;
}


auto Camera::getSpeed() const noexcept -> float
{
    return this->speed;
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
    return this->yaw;
}


auto Camera::getPitch() const noexcept -> float
{
    return this->pitch;
}


auto Camera::cameraLog() const noexcept -> void
{
    fmt::print(fg(fmt::color::yellow),
     "Camera position: [{}, {}, {}]\nCamera Yaw: [{}]\nCamera Pitch: [{}]\nCamera direction: [{}, {}, {}]\n\n",
      this->getPosition().x, this->getPosition().y, this->getPosition().z, this->getYaw(), this->getPitch(), this->direction.x, this->direction.y, this->direction.z);
}


auto Camera::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) -> void
{
    // fmt::print("{}\n", firstMovement);
    if (firstMovement)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMovement = false;
    }   

    float xOffset { static_cast<float>((xpos - lastX) * sensitivity) };
    float yOffset { static_cast<float>((lastY - ypos) * sensitivity) };

    // fmt::print("xOffset: [{}]\nyOffset: [{}]\n\n", xOffset, yOffset);

    yaw += xOffset;
    pitch += yOffset;

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    if (pitch > 89.0f)
    {
        pitch = 89.0f; 
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    direction = glm::normalize(direction);
}