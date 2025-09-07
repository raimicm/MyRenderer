#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float aspect_ratio, float view_radius);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void orbit(glm::vec3, float angle);
    void changeZoom(float factor);

    glm::vec3 pos;
private: 
    float aspect_ratio;
    float zoom = 1.0f;
};