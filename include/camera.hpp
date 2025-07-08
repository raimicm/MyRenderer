#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float aspect_ratio);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void orbit(glm::vec3, float angle);

    glm::vec3 pos;
private: 
    float aspect_ratio;
};