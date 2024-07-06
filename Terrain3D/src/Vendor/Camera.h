#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "glm/gtc/matrix_transform.hpp"

class Camera {
private:
    glm::mat4 viewMatrix; // This is our view matrix, which will position the vertices to view space
    glm::vec3 worldUp;    // vector to define the up direction
    glm::vec3 cameraPos;  // current camera position
    glm::vec3 cameraFront;// current camera front direction
    glm::vec3 cameraRight;// camera right direction
    glm::vec3 cameraUp;   // camera up direction calculated using world up direction

    float yaw;            // defines camera orientation on x-axis
    float pitch;          // defines camera orientation on y-axis

    float movementSpeed;  // camera movment speed
    float sensitivity;    // camerea mouse sensitivity
    void updateCameraVectors(); // this function works on updating the camera variables whenever the mouse moves

public:
    // Constructor and Destructor
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp) {
        this->viewMatrix = glm::mat4(1.f); // initalize with 1.f mat
        this->movementSpeed = 10.f;
        this->sensitivity = 8.f;
        this->worldUp = worldUp;
        this->cameraPos = position;
        this->cameraRight = glm::vec3(0.f);
        this->cameraUp = worldUp;

        this->yaw = -90.f;
        this->pitch = 0.f;
        this->updateCameraVectors();
    };
    ~Camera() {

    }

    // Setters Functions
    void setCameraPos(glm::vec3 cp) {
        cameraPos = cp;
    };

    void setCameraFront(glm::vec3 cf) {
        cameraFront = cf;
    };

    void setCameraUp(glm::vec3 cu) {
        cameraUp = cu;
    };
    glm::mat4 setViewMat(glm::vec3 cp, glm::vec3 cf, glm::vec3 cu)
    {
        viewMatrix =  glm::lookAt(cp, cp + cf, cu);
    };

    //Getters Functions
    const glm::vec3 getCameraPos() const { return cameraPos; }

    const glm::mat4 getViewMatrix() { 
        this->updateCameraVectors();
        this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
        return this->viewMatrix;
    }

    // Inputs Functions
    void updateKeyboardInput(const float& dt, const int direction);
    void updateMouseInput(const float dt,const double& offsetX, const double& offsetY);
    void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY) {
        this->updateKeyboardInput(dt, direction);
        this->updateMouseInput( dt,offsetX, offsetY);
    
    }
    
    //void ProcessInputs(GLFWwindow *window,float cameraSpeed,float deltaTime);
    //void ProcessMouseMovement(float xpos, float ypos, GLboolean constrainPitch);
    //void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

};