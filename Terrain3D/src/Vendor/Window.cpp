// Window.cpp
#include "Window.h"
#include <iostream>



void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OPENGL ERROR] (" << error << "):" << function <<
            " " << file << ":" << std::endl;

        return false;
    }
    return true;
}


Window::Window(int width, int height, const std::string& title) : m_window(nullptr) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;
    initGlew();

}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::initGlew() {
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
    }

}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}