#include "camera.hpp"

#include "glm/glm.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp> // glm::pi

Camera::Camera(float aspect_ratio) : aspect_ratio(aspect_ratio) {
    pos.z = 3.0f;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAtRH(pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix() {
    const float fov = glm::pi<float>() / 2.0f;
    const float n = 0.5f;
    const float f = 50.0f;
    const float t = tanf(fov / 2.0f) * n;
    const float r = t * aspect_ratio;
    glm::mat4 projection(
        n/r, 0.0f, 0.0f, 0.0f,
        0.0f, n/t, 0.0f, 0.0f,
        0.0f, 0.0f, (n+f)/(n-f), -1.0f,
        0.0f, 0.0f, ((2.0f)*n*f)/(n-f), 0.0f
    );
    return projection;
}

void Camera::orbit(glm::vec3 axis, float angle) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
    glm::vec4 pos4(pos.x, pos.y, pos.z, 1.0f);
    pos4 = pos4 * rotation;
    pos.x = pos4.x;
    pos.y = pos4.y;
    pos.z = pos4.z;
}