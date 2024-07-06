// Window.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

#pragma once
#ifndef WINDOW_H
#define WINDOW_H

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Window {
    int SCREEN_WIDTH, SCREEN_HEIGHT;
public:
    Window(int width, int height, const std::string& title);
    ~Window();
    void initGlew();
    bool shouldClose() const;
    GLFWwindow* m_window;
    GLFWwindow* GetWindow() { return m_window; }
    int getWidth() { return SCREEN_WIDTH; }
    int getHeight() { return SCREEN_HEIGHT; }
    void calculateDeltaTime(float &deltaTime, float &lastFrame) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
};



#endif // WINDOW_H