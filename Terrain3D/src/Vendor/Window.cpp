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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;
    initGlew();
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    glEnable(GL_DEPTH_TEST);

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