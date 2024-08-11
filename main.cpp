#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <print>


int main() {

    if (!glfwInit()) {
        std::println("Failed : (");
        return -1;
    }

    if (!glewInit()) {
        std::println("Failed to initialize GLEW!");
        return -1;
    }    

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", nullptr, nullptr);
    if (!window)
    {
       std::println("Failed to initialize window!");
       return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        
    }
    
    glfwTerminate();
    return 0;
}